from PyQt4.QtGui import *
from PyQt4.Qt import *
from openalea.plantgl.all import *
from PyQGLViewer import *
import OpenGL.GL as ogl
import OpenGL.GLU as glu
from random import *
from math import cos,sin,sqrt,fabs
from math import pi

helpstr = """<h2>N u r b s P a t c h E d i t o r</h2>
This application allows the user to edit a nurbsPatch dynamically.
The user can add a column or a row respectively pressing shift+RightMouseButton or shift+LeftRightButton.
The user can move the control points selecting one or many of them double clicking on it, then grabbing it or one of the axis
to move them in the way he wants. double clicking anywhere but on a control point de-select every control points """




  


###########################################

class Pos3Setter:
    def __init__(self,ctrlpointset,index):
        self.ctrlpointset = ctrlpointset
        self.index = index
    def __call__(self,pos):
        self.ctrlpointset[self.index] = pos

class Pos4Setter:
    def __init__(self,ctrlpointset,index):
        self.ctrlpointset = ctrlpointset
        self.index = index
    def __call__(self,pos):
        self.ctrlpointset[self.index] = Vector4(pos,1)
        

class CtrlPoint(ManipulatedFrame):
    """the class CtrlPoint  represent the control points of a NurbsPatch by PlantGL Spheres. 
        The user can grab them and move them dynamically. 
        Change of value is automatically propagate to the initial structure using the position_setter function """   

    def __init__(self, position, position_setter):
        """Constructor 
           :param position: initial position of the control point
           :param position_setter: makes it possible to propagate position change to initial structure
        """
        ManipulatedFrame.__init__(self)
        # current position and method to propagate displacement on original structure
        self.setPosition(Vec(position[0],position[1],position[2]))
        self.position_setter = position_setter
        
        # point selection or focus
        self.selected = False
        self.hasFocus = False
        
        # previous position to compute displacement
        self.previousPosition=self.position()
        
        # Constraint the direction of translation
        self.constraint = WorldConstraint()
        self.constraint.setRotationConstraintType(AxisPlaneConstraint.FORBIDDEN)
        self.setConstraint(self.constraint)
        
        # signal to propagate displacement
        QObject.connect(self,SIGNAL('modified()'),self.__propagate_position_change__)
        QObject.connect(self,SIGNAL('manipulated()'),self.__emit_translation__)
    
    def __push_position__(self):
        """ push current position status as previous position """
        self.previousPosition=self.position()

    def displacement(self):
        """ get displacement applied to the point """ 
        delta = self.position() - self.previousPosition 
        self.__push_position__()
        return delta
    
    def __propagate_position_change__(self):
        """ apply position change to initial structure """
        self.position_setter(self.position())
    
    def __emit_translation__(self):
        """ emit a signal when a translation has been applied to the point """
        pos = self.position()
        tr = pos - self.previousPosition
        self.emit(SIGNAL("translated(PyQt_PyObject,PyQt_PyObject)"),self,tr)
        self.emit(SIGNAL("valueChanged()"))
     
    def apply_translation(self,tr):
        """ apply a displacement. USefull when point is part of selection and a displacement  occur on another point """
        self.setPosition(self.position()+tr)
        self.__push_position__()
    
    def representation(self) :
        """ Compute representation of the control points as PlantGL Spheres"""
        p=self.position()
        #we change the color of the point if it is selected or has focus
        if self.hasFocus:   m=Material((250,30,30),1)
        else:               m=Material((30,30,250),1)
        if self.selected : m.ambient.green+=200
        #the point is represented by a PlantGl Sphere
        return Shape(Translated(Vector3(p.x,p.y,p.z),Sphere(0.05)),m)
    
    def getx(self):
        """ Get the x coordinate """
        return self.position().x
    def gety(self):
        """ Get the y coordinate """
        return self.position().y
    def getz(self):
        """ Get the z coordinate """
        return self.position().z
    def setx(self,x):
        """ Set the x coordinate """
        p = self.position()
        p.x = x
        self.setPosition(p)
    
    x  = property(getx,""" The x coordinate property """)
    y  = property(gety,""" The y coordinate property """)
    z  = property(getz,""" The z coordinate property """)

###########################################

class SelectionAxis (ManipulatedFrame):
    """ The selection Axis class. 
        We consider here that x, y and z axis are represented. 
        These axis are represented by plantGL polylines and spheres 
        and must be grabbable for constrained displacements
    """
    O,X,Y,Z = (Vec(0,0,0), Vec(1,0,0), Vec(0,1,0),  Vec(0,0,1)) # predefined axis direction
    AxisLength = 0.2  # length of axis representation
    
    def __init__(self, parent, direction= Vec(), material =Material(), selectionMaterial = Material((0,0,0),1)):
        """Constructor 
        
        :param parent : the reference frame on which it is defined. In practice, it is a SelectionManipulator.
        :param direction: the direction of the axis.
        :param material: color of the representation of this axis
        :param selectionMaterial: color of the representation of this axis when it is selected
        """
        ManipulatedFrame.__init__(self)
        self.setReferenceFrame(parent)
        
        # selection
        self.hasFocus = False
        
        # direction and position
        self.direction = direction
        self.initpos = direction*SelectionAxis.AxisLength
        self.setTranslation(self.initpos)
        
        # display option
        self.material = material
        self.selectionMaterial = selectionMaterial
        
        #displacement computation
        self.previousPosition = Vec()
        
        # Constraint the direction of translation
        self.constraint = WorldConstraint()
        self.constraint.setRotationConstraintType(AxisPlaneConstraint.FORBIDDEN) 
        self.constraint.setTranslationConstraint(AxisPlaneConstraint.AXIS,self.direction)
        self.setConstraint(self.constraint)
        
        QObject.connect(self,SIGNAL("manipulated()"),self.__emit_translation__)
    
   
    def representation(self):
        """ Compute representation of the SelectionAxis as line and sphere. """
        p=self.position()
        p2 = p - self.direction * SelectionAxis.AxisLength
        if self.hasFocus : material = self.selectionMaterial
        else: material = self.material
        sh  =[Shape(Polyline([Vector3(*p2),Vector3(*p)]),material)]
        sh +=[Shape(Translated(Vector3(*p),Sphere(0.05)),material)]
        return sh

    def __emit_translation__(self):
        """ emit a signal when a translation occur on this axis """
        tr = self.translation()
        self.setTranslation(self.initpos)
        self.emit(SIGNAL("translated(PyQt_PyObject)"),tr-self.initpos)
    
#############################################################

class SelectionManipulator (ManipulatedFrame):
    def __init__(self):
        ManipulatedFrame.__init__(self)
        # set of selected point
        self.selection = []
        # axis for axial displacement
        self.axis=[ SelectionAxis(self,SelectionAxis.X,material=Material((250,30,30),1)), 
                    SelectionAxis(self,SelectionAxis.Y,material=Material((30,250,30),1)), 
                    SelectionAxis(self,SelectionAxis.Z,material=Material((250,250,250),1))]
        
        # previous position to compute displacement
        self.previousPosition = Vec()        
        QObject.connect(self,SIGNAL("manipulated()"),self.propagatePointTranslation)
        for axis in self.axis:
            QObject.connect(axis,SIGNAL("translated(PyQt_PyObject)"),self.propagateAxisTranslation)        
    
    def __push_position__(self):
        """ push current position status as previous position """
        self.previousPosition=self.position()

    def displacement(self):
        """ Gives current displacement of the frame """
        delta = self.position() - self.previousPosition 
        self.__push_position__()
        return delta
        
    def propagateAxisTranslation(self,displacement):
        """ Apply the displacement of the axis on the set of selected control points """
        self.setPosition(self.position()+displacement)
        self.__push_position__()
        for point in self.selection:
            point.apply_translation(displacement)
        self.emit(SIGNAL("valueChanged()"))

    
    def propagatePointTranslation(self):
        """ Apply the displacement of self on the set of selected control points """
        displacement = self.displacement()
        for point in self.selection:
            point.apply_translation(displacement)
        self.emit(SIGNAL("valueChanged()"))
    
    def clear(self):
        """ remove all control points """
        QObject.disconnect(self,SIGNAL("manipulated()"),self.propagatePointTranslation)
        for ctrlpoint in self.selection:
            ctrlpoint.selected = False
            QObject.disconnect(ctrlpoint,SIGNAL("translated(PyQt_PyObject,PyQt_PyObject)"),self.propagatePointTranslation)
        self.selection = None
            
    def add(self,ctrlpoint):
        """ add a ctrl point to the selection """
        self.selection.append(ctrlpoint)
        ctrlpoint.selected = True
        #QObject.connect(ctrlpoint,SIGNAL("translated(PyQt_PyObject,PyQt_PyObject)"),self.propagatePointTranslation)
        self.__update__()
    
    def remove(self,ctrlpoint):
        """ remove a ctrl point to the selection """
        ctrlpoint.selected = False
        #QObject.connect(ctrlpoint,SIGNAL("translated(PyQt_PyObject,PyQt_PyObject)"),self.propagatePointTranslation)
        del self.selection[self.selection.index(ctrlpoint)]
        self.__update__()
        
    def toogleSelection(self,ctrlpoint):
        """ add a ctrl point to the selection or remove it if already in it"""
        if ctrlpoint in self.selection:
            self.remove(ctrlpoint)
        else:
            self.add(ctrlpoint)
    
    def __update__(self):
        """ update the central point position when selection has changed """
        self.centralPoint = sum([i.position() for i in self.selection],Vec()) / len(self.selection)
        self.setPosition(self.centralPoint)
        self.__push_position__()
    
    def representation(self):
        """ define the representation of this manipulator """
        shapes = []
        shapes+=self.axis[0].representation()
        shapes+=self.axis[1].representation()
        shapes+=self.axis[2].representation()    
        return shapes
    
    def empty(self):
        """ Tell whether the selection set is empty """
        return len(self.selection) == 0
    
    def checkIfAxisGrabsMouse(self,event,viewer):
        """ check whether an axis has been selected """
        for axis in self.axis:
            axis.checkIfGrabsMouse(event.x(),event.y(),viewer.camera())
            if axis.grabsMouse():
                viewer.setManipulatedFrame(axis)
                axis.hasFocus = True
                return True
        return False
    
    def clearFocus(self):
        """ clear focus """
        for axis in self.axis:
            axis.hasFocus = False

#############################################################

def addRowToPatch(patch,row,decal):
    """ create a patch with an additionnal row """
    cpoints = patch.ctrlPointMatrix
    r = cpoints.getRow(row)
    r = [p+decal for p in r]
    cpoints2 = Point4Matrix(cpoints)
    cpoints2.insertRow(row,r)
    return NurbsPatch(cpoints2)

def addColumnToPatch(patch,col,decal):
    """ create a patch with an additionnal column """
    cpoints = patch.ctrlPointMatrix
    r = cpoints.getColumn(col)
    r = [p+decal for p in r]
    cpoints2 = Point4Matrix(cpoints)
    cpoints2.insertColumn(col,r)
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
            
  
    def __propagate_valuechanged__(self):
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
    
       
        shapes=[]
        #define the nurbsShape and the control points geometry
        #self.p=[] 
        for ctrlPointLine in self.ctrlPointMatrix:
            for ctrlPoint in ctrlPointLine:
                shapes.append(ctrlPoint.representation())

        # draw the frame of multiple selection
        if self.selectionManipulator:
            shapes+=self.selectionManipulator.representation()

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
    
    def newDefaultNurbsPatch(self):
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
        for j,linePoint in enumerate(self.nurbsPatch.ctrlPointMatrix):
            lineCtrlPoint = []
            for i in range(len(linePoint)):
                ctrlPoint = CtrlPoint(linePoint[i].project(), Pos4Setter(self.nurbsPatch.ctrlPointMatrix,(j,i)))
                QObject.connect(ctrlPoint,SIGNAL("valueChanged()"),self.__propagate_valuechanged__)
                lineCtrlPoint.append(ctrlPoint)
            self.ctrlPointMatrix.append(lineCtrlPoint)
    
    def clear(self):
        """ clear current edition """
        for ctrlPointRow in self.ctrlPointMatrix:
            for cCtrlPoint in ctrlPointRow:
                QObject.disconnect(cCtrlPoint,SIGNAL("valueChanged()"),self.__propagate_valuechanged__)
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
