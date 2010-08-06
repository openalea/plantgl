from PyQt4.QtGui import *
from PyQt4.Qt import *
from openalea.plantgl.all import *
from PyQGLViewer import *
import OpenGL.GL as ogl
from editablectrlpoint import * 
from math import cos,sin,sqrt,fabs,pi

helpstr = """<h2>N u r b s P a t c h E d i t o r</h2>
This application allows the user to edit a nurbsPatch dynamically.
The user can add a column or a row respectively pressing shift+RightMouseButton or shift+LeftRightButton.
The user can move the control points selecting one or many of them double clicking on it, then grabbing it or one of the axis
to move them in the way he wants. double clicking anywhere but on a control point de-select every control points """

#############################################################

def addRowToPatch(patch,row,decal):
    """ create a patch with an additionnal row """
    cpoints = patch.ctrlPointMatrix
    r = cpoints.getRow(row)
    r = [p+decal for p in r]
    cpoints2 = Point4Matrix(cpoints)
    cpoints2.insertRow(row+1,r)
    return NurbsPatch(cpoints2)

def addColumnToPatch(patch,col,decal):
    """ create a patch with an additionnal column """
    cpoints = patch.ctrlPointMatrix
    r = cpoints.getColumn(col)
    r = [p+decal for p in r]
    cpoints2 = Point4Matrix(cpoints)
    cpoints2.insertColumn(col+1,r)
    return NurbsPatch(cpoints2)


#############################################################

class NurbsPatchEditor(QGLViewer):
    """ The class NurbsPatchEditor is the viewer of the scene, it contains all the informations about the NurbsPatch, that's why a getter and a setter have been created, the NubrsPatch is defined by a 2 dimensional Array of 3d Vectors"""
   
    Edit,Rotate = range(2)
    
    def __init__(self,parent):
        """ Constructor 
            :param parent: the parent widget
        """
        QGLViewer.__init__(self,parent)
        self.setStateFileName('.nurbspatcheditor.xml') 
     
        self.mode = NurbsPatchEditor.Edit
        
        # the nurbs patch
        self.nurbsPatch = None
        
        # matrix of ctrl point manipulator
        self.ctrlPointMatrix=[]
        
        # shape and material to display the patch
        self.nurbsShape = Shape()
        self.defaultMaterial=Material((30,200,30),1,transparency=0.5)
        self.nurbsShape.appearance = self.defaultMaterial

        # plantgl basic object to display nurbs
        self.discretizer = Discretizer()
        self.glrenderer = GLRenderer(self.discretizer)
        self.glrenderer.renderingMode = GLRenderer.Dynamic
        self.ctrlrenderer=GLCtrlPointRenderer(self.discretizer)
        self.bboxcomputer = BBoxComputer(self.discretizer)        
        
        #   Current selection
        self.selectionManipulator = None 
        self.focus = None
        
        # rectangular selection
        self.selectionRect=False # for rectangular selection (with control)
        self.__rectangle = QRect()
        self.__rectangleInit = None
    
        #creation of a default NurbsPatch
        if len(self.ctrlPointMatrix)==0 :
            self.setNurbsPatch(self.newDefaultNurbsPatch())
            
  
    def __propagate_valuechanged__(self,pid):
        """ emit a signal every time a value changed """
        self.setSceneBoundingBox(*self.getBounds())
        self.emit(SIGNAL("valueChanged()"))
    
    def init(self):
        """ init function """
        self.setSceneBoundingBox(*self.getBounds())
        self.restoreStateFromFile()
        self.setHandlerKeyboardModifiers(QGLViewer.CAMERA, Qt.AltModifier)
        self.setHandlerKeyboardModifiers(QGLViewer.FRAME,  Qt.NoModifier)
        self.setHandlerKeyboardModifiers(QGLViewer.CAMERA, Qt.ControlModifier)
        self.setMouseBinding(Qt.LeftButton,QGLViewer.FRAME,QGLViewer.TRANSLATE)
        self.setMouseBindingDescription(Qt.ShiftModifier+Qt.LeftButton,"Rectangular selection")
        self.setMouseBindingDescription(Qt.LeftButton,"Camera/Control Points manipulation")
        self.setMouseBindingDescription(Qt.LeftButton,"When double clicking on a line, create a new line",True)
    
    
    def getBounds(self):
        """ Get the Bounding Box of the scene:
            return the minpos and maxpos that can be used by setSceneBoundingBox function"""
        self.nurbsPatch.apply(self.bboxcomputer)
        res = self.bboxcomputer.result
        return Vec(*res.lowerLeftCorner),Vec(*res.upperRightCorner)

    def setFocus(self,point):
        """ Set focus to given control point """
        if self.focus:
            self.focus.hasFocus = False
        self.focus = point
        if self.focus:
            point.hasFocus = True

    def clearSelectionManipulator(self):
        """ clear the selection manipulator """
        if self.selectionManipulator:
            self.selectionManipulator.clear()
            QObject.disconnect(self.selectionManipulator,SIGNAL("valueChanged()"),self.__propagate_valuechanged__)
            self.selectionManipulator = None

    def createSelectionManipulator(self):
        """ ensure that the selection manipulator is existing and valid """
        if not self.selectionManipulator:
            self.selectionManipulator = SelectionManipulator()
            QObject.connect(self.selectionManipulator,SIGNAL("valueChanged()"),self.__propagate_valuechanged__)
        
    def mousePressEvent(self,event):
        """ Check for eventual operations the user asks: 
            shift start rectangular selection
            else check for which point is selected
        """
        #Rectangular selection
        if event.modifiers() == Qt.ShiftModifier:
            if event.button() == Qt.LeftButton :
                self.__rectangle = QRect(event.pos(), event.pos())
                self.__rectangleInit = event.pos()
                self.selectionRect=True
            elif event.button() == Qt.RightButton :
                self.clearSelectionManipulator()
            self.updateGL()
        else:
            pointSelection = False
            if self.selectionManipulator:
                pointSelection = self.selectionManipulator.checkIfAxisGrabsMouse(event,self)
            if not pointSelection:
                for ctrlPointRow in self.ctrlPointMatrix:
                    for cCtrlPoint in ctrlPointRow:
                        cCtrlPoint.checkIfGrabsMouse(event.x(), event.y(), self.camera())
                        if cCtrlPoint.grabsMouse():
                            pointSelection = True
                            self.setFocus(cCtrlPoint)
                            if self.selectionManipulator and cCtrlPoint.selected :
                                self.setManipulatedFrame(self.selectionManipulator)
                            else :
                                self.setManipulatedFrame(cCtrlPoint)
                            break
            self.updateGL()
            
            if pointSelection :
                # if no point is selected, then move camera
                if self.mode != NurbsPatchEditor.Edit:
                    self.mode = NurbsPatchEditor.Edit
                    self.setHandlerKeyboardModifiers(QGLViewer.CAMERA, Qt.AltModifier)
                    self.setHandlerKeyboardModifiers(QGLViewer.FRAME,  Qt.NoModifier)
                    self.setHandlerKeyboardModifiers(QGLViewer.CAMERA, Qt.ControlModifier)
            else: # move manipulated frame
                if self.mode != NurbsPatchEditor.Rotate:
                    self.mode = NurbsPatchEditor.Rotate
                    self.setHandlerKeyboardModifiers(QGLViewer.FRAME, Qt.AltModifier)
                    self.setHandlerKeyboardModifiers(QGLViewer.CAMERA,  Qt.NoModifier)
                    self.setHandlerKeyboardModifiers(QGLViewer.FRAME, Qt.ControlModifier)

        QGLViewer.mousePressEvent(self,event)

            
    def mouseDoubleClickEvent(self,event):
        """ mouseDoubleClickEvent: add a control point to the selection double clicking on it, empty the selection double clicking elsewhere """
        if event.modifiers() & Qt.ShiftModifier:
            selection=False
            #Doubleclick on a control point add/delete it from the selection array#
            for ctrlPointRow in self.ctrlPointMatrix:
                for cCtrlPoint in ctrlPointRow:
                    cCtrlPoint.checkIfGrabsMouse(event.x(), event.y(), self.camera())
                    if cCtrlPoint.grabsMouse():
                        cCtrlPoint.selected = not cCtrlPoint.selected
                        selection = True
                        self.createSelectionManipulator()
                        self.selectionManipulator.toogleSelection(cCtrlPoint)
            #Double click with shift anywhere but on a control point -> empty selection #
            if not selection:
                self.clearSelectionManipulator()                
        else:
            ## Adding a column on the NurbsPatch#            
            """ when double click on a line, the line is doubled """
            self.select(event.pos()) # select call drwWithName that render line of control points with ids
            selection = self.selectedName()
            if selection >= 0:
                nbcolumn = self.nurbsPatch.ctrlPointMatrix.getColumnNb()
                minp,maxp = self.getBounds()
                diff = norm(maxp-minp)/20
                if selection >= nbcolumn:
                    row = selection-nbcolumn
                    newpatch = addRowToPatch(self.nurbsPatch,row,Vector4(diff,0))
                    selectedPoints = [(row,i) for i in xrange(nbcolumn)]
                else:
                    newpatch = addColumnToPatch(self.nurbsPatch,selection,Vector4(diff,0))
                    selectedPoints = [(i,selection) for i in xrange(self.nurbsPatch.ctrlPointMatrix.getRowNb())]
                self.setNurbsPatch(newpatch)
                self.createSelectionManipulator()
                for index in selectedPoints:
                    self.selectionManipulator.add(self.ctrlPointMatrix[index[0]][index[1]])
            else:
                self.clearSelectionManipulator()                
                
    def mouseMoveEvent(self,event):
        """ mouseMoveEvent """ 
        
        #rectangular selection
        if self.selectionRect:
            self.__rectangle =  QRect(self.__rectangleInit,event.pos()).normalized()
            self.updateGL()
        
        # by default camera or manipulated frame is manipulated
        QGLViewer.mouseMoveEvent(self,event);

    def mouseReleaseEvent(self,event):
        """On mouse release, we release every grabbed objects"""
        QGLViewer.mouseReleaseEvent(self,event)
        # clear manipulated object
        self.setManipulatedFrame(None)
        self.setFocus(None)
        if self.selectionManipulator:
            self.selectionManipulator.clearFocus()
        
        # end of rectangular selection : 
        if self.selectionRect:
            self.selectionRect=False
            # Possibly swap left/right and top/bottom to make rectangle_ valid.
            self.__rectangle =  QRect(self.__rectangleInit,event.pos()).normalized()
            # make rectangle with a minimal size
            if self.__rectangle.width() < 10 or self.__rectangle.height() < 10:           
                self.__rectangle = QRect(event.pos().x()-5,event.pos().y()-5,10,10)
            self.selectionFromRect(self.__rectangle)
        self.updateGL()

    def selectionFromRect(self,rect):
        """ check if control point projections are in the given rectangle. If yes, put point in selection """
        for ctrlPointRow in self.ctrlPointMatrix:
            for cCtrlPoint in ctrlPointRow:
                point=self.camera().projectedCoordinatesOf(cCtrlPoint.position())
                if (self.__rectangle.contains(point.x,point.y)):
                    if not self.selectionManipulator:
                        self.selectionManipulator = SelectionManipulator()
                        QObject.connect(self.selectionManipulator,SIGNAL("valueChanged()"),self.__propagate_valuechanged__)
                    self.selectionManipulator.toogleSelection(cCtrlPoint)
        if self.selectionManipulator and self.selectionManipulator.empty():
            self.clearSelectionManipulator()

    def draw(self):
        """ 
        Draw function :
        Control points of the nurbsPatch are represented by spheres.
        If some of them are selected, a medium axis will be drawn by lines and little spheres, 
        allowing us to grab one of them to perform a constrained move
        """
        scradius = self.sceneRadius()
        pointsize = scradius/30.
       
        shapes=[]
        #define the nurbsShape and the control points geometry
        #self.p=[] 
        for ctrlPointLine in self.ctrlPointMatrix:
            for ctrlPoint in ctrlPointLine:
                shapes.append(ctrlPoint.representation(pointsize))

        # draw the frame of multiple selection
        if self.selectionManipulator:
            shapes+=self.selectionManipulator.representation(pointsize*1.2)

        shapes.append(self.nurbsShape)
        sc=Scene(shapes)
        
        ogl.glDisable(ogl.GL_LIGHTING)
        ogl.glPolygonMode(ogl.GL_FRONT_AND_BACK,ogl.GL_LINE)
        # draw the wire of the patch
        ogl.glLineWidth(1)
        shwire = Shape(self.nurbsPatch,Material((120,255,0),0.8))
        shwire.apply(self.glrenderer)
        # ctrl line rendering
        ogl.glLineWidth(1)
        Material((255,255,255),1).apply(self.glrenderer)        
        self.nurbsPatch.apply(self.ctrlrenderer)
        # patch and control point shpere rendering
        ogl.glPolygonMode(ogl.GL_FRONT_AND_BACK,ogl.GL_FILL)
        ogl.glEnable(ogl.GL_LIGHTING)
        ogl.glLightModeli(ogl.GL_LIGHT_MODEL_TWO_SIDE, ogl.GL_TRUE)
        ogl.glEnable(ogl.GL_BLEND)
        ogl.glBlendFunc(ogl.GL_SRC_ALPHA,ogl.GL_ONE_MINUS_SRC_ALPHA)
        sc.apply(self.glrenderer)
        ogl.glDisable(ogl.GL_BLEND)
        
        if self.selectionRect:
            # Draw rectangular selection
            self.startScreenCoordinatesSystem() # use screen coordinates
            r = self.__rectangle
            rect = Shape(Polyline2D([(r.x(),r.y()),(r.x()+r.width(),r.y()),
                                     (r.x()+r.width(),r.y()+r.height()),
                                     (r.x(),r.y()+r.height()),(r.x(),r.y())]),
                                         Material((200,200,200),1))
            ogl.glEnable (ogl.GL_LINE_STIPPLE)
            ogl.glLineStipple(1,0x0FFF)       
            ogl.glLineWidth(2)
            rect.apply(self.glrenderer)
            ogl.glDisable (ogl.GL_LINE_STIPPLE)
            self.stopScreenCoordinatesSystem()
        ogl.glLineWidth(1)

    def drawWithNames(self):
        """ draw control lines with names """
        pointmatrix = self.nurbsPatch.ctrlPointMatrix
        lines = []
        m =Material()
        maxcolumn = pointmatrix.getColumnNb()
        for colid in xrange(maxcolumn):
            lines.append(Shape(Polyline(Point4Array(pointmatrix.getColumn(colid)).project(),width=3),m,colid))
        for rowid in xrange(pointmatrix.getRowNb()):
            lines.append(Shape(Polyline(Point4Array(pointmatrix.getRow(rowid)).project(),width=3),m,maxcolumn+rowid))
        for sh in lines:
          ogl.glPushName(sh.id)
          sh.apply(self.glrenderer)
          ogl.glPopName()
    
    @staticmethod
    def newDefaultNurbsPatch():
        """ return a default nurbs patch """
        return NurbsPatch([[Vector4(j-1.5,i-1.5,0,1) for j in range(4)] for i in range(4)])

    def getNurbsPatch(self):
        """ return the edited nurbs patch """
        return self.nurbsPatch

    def setNurbsPatch(self,nurbsPatch):
        """ set the nurbs patch to edit """
        self.clear()
        self.nurbsPatch=nurbsPatch
        self.nurbsShape.geometry=self.nurbsPatch
        nbLines = float(len(self.nurbsPatch.ctrlPointMatrix))
        pid = 1
        for j,linePoint in enumerate(self.nurbsPatch.ctrlPointMatrix):
            lineCtrlPoint = []
            nbCols =  len(linePoint)
            for i in range(nbCols):
                ctrlPoint = CtrlPoint(linePoint[i].project(), Pos4Setter(self.nurbsPatch.ctrlPointMatrix,(j,i)),color=(30+int(220*j/nbLines),30+int(220*i/nbCols),250),id=pid)
                pid += 1
                ctrlPoint.setCallBack(self.__propagate_valuechanged__)
                #QObject.connect(ctrlPoint,SIGNAL("valueChanged(int)"),self.__propagate_valuechanged__)
                lineCtrlPoint.append(ctrlPoint)
            self.ctrlPointMatrix.append(lineCtrlPoint)
        self.setSceneBoundingBox(*self.getBounds())
    
    def clear(self):
        """ clear current edition """
        # for ctrlPointRow in self.ctrlPointMatrix:
            # for cCtrlPoint in ctrlPointRow:
                # if not cCtrlPoint is None:
                    # QObject.disconnect(cCtrlPoint,SIGNAL("valueChanged(int)"),self.__propagate_valuechanged__)
        self.ctrlPointMatrix = []
        self.clearSelectionManipulator()

    def helpString(self):
        return helpstr


    def closeEvent(self,event):
        """close the windows properly"""
        helpwidget = self.helpWidget()
        if helpwidget and helpwidget.isVisible() :
            helpwidget.hide()
        QGLViewer.closeEvent(self,event)

#########################################################

def main():
    """the main program, here we create a NurbsPatchEditor and make it draw itself"""
    qapp = QApplication([])
    viewer = NurbsPatchEditor(None)
    viewer.setWindowTitle("NurbsPatchEditor")
    viewer.show()
    qapp.exec_()

if __name__ == '__main__':
    main()
