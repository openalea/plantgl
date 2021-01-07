from openalea.plantgl.gui.qt import QtCore, QtGui, QtWidgets
from openalea.plantgl.all import *
from PyQGLViewer import *
import OpenGL.GL as ogl
try:
    from .editablectrlpoint import * 
except ImportError as ie:
    from editablectrlpoint import * 
from math import cos,sin,sqrt,fabs,pi
import numpy as np
import itertools


from openalea.plantgl.gui.qt.QtCore import QObject, QRect, Qt, pyqtSignal
from openalea.plantgl.gui.qt.QtWidgets import QApplication

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

class NurbsPatchEditorView(QGLViewer):
    """ The class NurbsPatchEditor is the viewer of the scene, it contains all the informations about the NurbsPatch, that's why a getter and a setter have been created, the NubrsPatch is defined by a 2 dimensional Array of 3d Vectors"""
   
    Edit,Rotate = list(range(2))
    valueChanged = pyqtSignal()
    manipulated = pyqtSignal()
    
    def __init__(self,parent):
        """ Constructor 
            :param parent: the parent widget
        """
        QGLViewer.__init__(self,parent)
        self.setStateFileName('.nurbspatcheditor.xml') 
     
        self.mode = NurbsPatchEditorView.Edit
        
        # the nurbs patch
        self.nurbsPatch = None
        
        # matrix of ctrl point manipulator
        self.ctrlPointManipulators = {}
        
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

        self.ctrlPointDisplay = None   
        
        #   Current selection
        self.selectionManipulator = None 
        self.focus = None
        
        # rectangular selection
        self.selectionRect=False # for rectangular selection (with control)
        self.__rectangle = QRect()
        self.__rectangleInit = None

        self._drawPatch = True
        self._drawGrid = True
    
        #creation of a default NurbsPatch
        self.setNurbsPatch(self.newDefaultNurbsPatch())

    def clear(self):
        """ clear current edition """
        self.ctrlPointManipulators = {}
        self.clearSelectionManipulator()

    def getCtrlPoints(self):
        return self.nurbsPatch.ctrlPointMatrix

    def getCtrlPointsDim(self):
        return self.getCtrlPoints().sizes()

    def getCtrlPointManipulators(self):
        return self.ctrlPointManipulators

    def getBounds(self):
        """ Get the Bounding Box of the scene:
            return the minpos and maxpos that can be used by setSceneBoundingBox function"""
        self.nurbsPatch.apply(self.bboxcomputer)
        res = self.bboxcomputer.result
        return Vec(*res.lowerLeftCorner),Vec(*res.upperRightCorner)

    def __propagate_valuechanged__(self, pid=None):
        """ emit a SIGNAL every time a value changed """
        self.setSceneBoundingBox(*self.getBounds())
        self.valueChanged.emit() 
        self.resetDisplay()

    def resetDisplay(self, pid = None):
        self.ctrlPointDisplay = None
    
    def init(self):
        """ init function """
        self.setSceneBoundingBox(*self.getBounds())
        self.restoreStateFromFile()

        # init mouse interaction 
        self.mode = NurbsPatchEditorView.Edit
        self.setInteractionMode(False)

        self.setMouseBindingDescription(Qt.ShiftModifier,Qt.LeftButton,"Rectangular selection")
        self.setMouseBindingDescription(Qt.NoModifier,Qt.LeftButton,"Camera/Control Points manipulation")
        self.setMouseBindingDescription(Qt.NoModifier,Qt.LeftButton,"When double clicking on a line, create a new line",True)
        
        self.setKeyDescription(Qt.Key_K, "Toggles control point wire display")
        self.setKeyDescription(Qt.Key_G, "Toggles patch display")

    def setFocus(self,point):
        """ Set focus to given control point """
        if self.focus:
            self.focus.hasFocus = False
        self.focus = point
        if self.focus:
            point.hasFocus = True

    def clearSelectionManipulator(self):
        """ clear the selection manipulator """
        if not self.selectionManipulator is None:
            self.selectionManipulator.clear()
            self.selectionManipulator.valueChanged.disconnect(self.__propagate_valuechanged__) # QObject.disconnect(self.selectionManipulator,SIGNAL("valueChanged()"),self.__propagate_valuechanged__)
            self.selectionManipulator = None
            self.resetDisplay()


    def createSelectionManipulator(self):
        """ ensure that the selection manipulator is existing and valid """
        if not self.selectionManipulator:
            self.selectionManipulator = SelectionManipulator()
            self.selectionManipulator.valueChanged.connect(self.__propagate_valuechanged__) # QObject.connect(self.selectionManipulator,SIGNAL("valueChanged()"),self.__propagate_valuechanged__)
            self.resetDisplay()
        
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
            if not self.selectionManipulator is None:
                pointSelection = self.selectionManipulator.checkIfAxisGrabsMouse(event,self)
            if not pointSelection:
                for cCtrlPoint in self.getCtrlPointManipulators().values():
                        cCtrlPoint.checkIfGrabsMouse(event.x(), event.y(), self.camera())
                        if cCtrlPoint.grabsMouse():
                            pointSelection = True
                            self.setFocus(cCtrlPoint)
                            if self.selectionManipulator and cCtrlPoint.selected :
                                self.setManipulatedFrame(self.selectionManipulator)
                            else :
                                self.setManipulatedFrame(cCtrlPoint)
                            break
            self.setInteractionMode(pointSelection)
            self.updateGL()
            
        QGLViewer.mousePressEvent(self,event)

    def setInteractionMode(self,frame=True):
        if frame:
            if self.mode != NurbsPatchEditorView.Edit:
                self.mode = NurbsPatchEditorView.Edit
                self.setMouseBinding(Qt.NoModifier,Qt.LeftButton,QGLViewer.FRAME,QGLViewer.TRANSLATE)
                self.setMouseBinding(Qt.NoModifier,Qt.RightButton,QGLViewer.FRAME,QGLViewer.NO_MOUSE_ACTION)

                self.setMouseBinding(Qt.ControlModifier,Qt.LeftButton,  QGLViewer.CAMERA,QGLViewer.ROTATE)
                self.setMouseBinding(Qt.ControlModifier,Qt.RightButton, QGLViewer.CAMERA,QGLViewer.TRANSLATE)
                self.setMouseBinding(Qt.ControlModifier,Qt.MiddleButton,QGLViewer.CAMERA,QGLViewer.ZOOM)     
        else:
            if self.mode != NurbsPatchEditorView.Rotate:
                self.mode = NurbsPatchEditorView.Rotate
                self.setMouseBinding(Qt.ControlModifier,Qt.LeftButton,QGLViewer.FRAME,QGLViewer.TRANSLATE)
                self.setMouseBinding(Qt.ControlModifier,Qt.RightButton,QGLViewer.FRAME,QGLViewer.NO_MOUSE_ACTION)
                self.setMouseBinding(Qt.NoModifier,Qt.LeftButton,QGLViewer.CAMERA,QGLViewer.ROTATE)
                self.setMouseBinding(Qt.NoModifier,Qt.RightButton,QGLViewer.CAMERA,QGLViewer.TRANSLATE)     
                self.setMouseBinding(Qt.NoModifier,Qt.MiddleButton,QGLViewer.CAMERA,QGLViewer.ZOOM)     

    def mouseDoubleClickEvent(self,event):
        """ mouseDoubleClickEvent: add a control point to the selection double clicking on it, empty the selection double clicking elsewhere """
        if event.modifiers() & Qt.ShiftModifier:
            selection=False
            #Doubleclick on a control point add/delete it from the selection array#
            for cCtrlPoint in self.getCtrlPointManipulators().values():
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
                selectedPoints = self.addElement(selection)
                if selectedPoints:
                    self.createSelectionManipulator()
                    for index in selectedPoints:
                        self.selectionManipulator.add(self.ctrlPointManipulators[index])
                else:
                    self.clearSelectionManipulator()
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
        self.setInteractionMode(False)
        self.updateGL()

    def selectionFromRect(self,rect):
        """ check if control point projections are in the given rectangle. If yes, put point in selection """
        for cCtrlPoint in self.getCtrlPointManipulators().values():
                point=self.camera().projectedCoordinatesOf(cCtrlPoint.position())
                if (self.__rectangle.contains(point.x,point.y)):
                    if not self.selectionManipulator:
                        self.selectionManipulator = SelectionManipulator()
                        self.selectionManipulator.valueChanged.connect(self.__propagate_valuechanged__) # QObject.connect(self.selectionManipulator,SIGNAL("valueChanged()"),self.__propagate_valuechanged__)
                    self.selectionManipulator.toogleSelection(cCtrlPoint)
        if self.selectionManipulator and self.selectionManipulator.empty():
            self.clearSelectionManipulator()
        self.ctrlPointDisplay = None

    def keyPressEvent(self,e):
        modifiers = e.modifiers()
        # A simple switch on e->key() is not sufficient if we want to take state key into account.
        # With a switch, it would have been impossible to separate 'F' from 'CTRL+F'.
        # That's why we use imbricated if...else and a "handled" boolean.
        if ((e.key()==Qt.Key_K) and (modifiers==Qt.NoModifier)):
            self._drawGrid = not self._drawGrid
            self.updateGL()
        elif (e.key()==Qt.Key_G) and (modifiers==Qt.NoModifier):
            self._drawPatch = not self._drawPatch
            self.updateGL()
        else:
            QGLViewer.keyPressEvent(self,e)

    def addElement(self, selection):
        nbcolumn = self.getCtrlPoints().getColumnNb()
        minp,maxp = self.getBounds()
        diff = norm(maxp-minp)/20
        if selection >= nbcolumn:
            row = selection-nbcolumn
            newpatch = addRowToPatch(self.nurbsPatch,row,Vector4(diff,0))
            selectedPoints = [(row,i) for i in range(nbcolumn)]
        else:
            newpatch = addColumnToPatch(self.nurbsPatch,selection,Vector4(diff,0))
            selectedPoints = [(i,selection) for i in range(self.getCtrlPoints().getRowNb())]
        self.setNurbsPatch(newpatch)
        return selectedPoints

    def draw(self):
        """ 
        Draw function :
        Control points of the nurbsPatch are represented by spheres.
        If some of them are selected, a medium axis will be drawn by lines and little spheres, 
        allowing us to grab one of them to perform a constrained move
        """

        ogl.glDisable(ogl.GL_LIGHTING)
        ogl.glPolygonMode(ogl.GL_FRONT_AND_BACK,ogl.GL_LINE)
        # draw the wire of the patch
        ogl.glLineWidth(1)
        
        if self._drawPatch:
            self.drawPatchWire()
        if self._drawGrid:
            self.drawCtrlWire()

        # patch and control point shpere rendering
        ogl.glPolygonMode(ogl.GL_FRONT_AND_BACK,ogl.GL_FILL)
        ogl.glEnable(ogl.GL_LIGHTING)
        ogl.glLightModeli(ogl.GL_LIGHT_MODEL_TWO_SIDE, ogl.GL_TRUE)
        ogl.glEnable(ogl.GL_BLEND)
        ogl.glBlendFunc(ogl.GL_SRC_ALPHA,ogl.GL_ONE_MINUS_SRC_ALPHA)

        if self._drawPatch:
            self.drawPatch()
        self.drawCtrlPoints()

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

    def drawCtrlPoints(self):
        if self.ctrlPointDisplay is None:
            scradius = self.sceneRadius()
            pointsize = scradius/30.
           
            shapes=[]
            #define the nurbsShape and the control points geometry
            for ctrlPoint in self.getCtrlPointManipulators().values():
                shapes.append(ctrlPoint.representation(pointsize))

            # draw the frame of multiple selection
            if self.selectionManipulator:
                shapes+=self.selectionManipulator.representation(pointsize*1.2)
            self.ctrlPointDisplay=Scene(shapes)

        # control point shpere rendering
        self.ctrlPointDisplay.apply(self.glrenderer)

    def drawCtrlWire(self):

        # ctrl line rendering
        Material((255,255,255),1).apply(self.glrenderer)        
        self.nurbsPatch.apply(self.ctrlrenderer)

    def drawPatch(self):
        sc=Scene([self.nurbsShape])

        # patch rendering
        sc.apply(self.glrenderer)

    def drawPatchWire(self):
        shwire = Shape(self.nurbsPatch,Material((120,255,0),0.8))
        shwire.apply(self.glrenderer)


    def drawWithNames(self):
        """ draw control lines with names """
        pointmatrix = self.getCtrlPoints()
        lines = []
        m =Material()
        maxcolumn = pointmatrix.getColumnNb()
        for colid in range(maxcolumn):
            lines.append(Shape(Polyline(Point4Array(pointmatrix.getColumn(colid)).project(),width=3),m,colid))
        for rowid in range(pointmatrix.getRowNb()):
            lines.append(Shape(Polyline(Point4Array(pointmatrix.getRow(rowid)).project(),width=3),m,maxcolumn+rowid))
        for sh in lines:
          ogl.glPushName(sh.id)
          sh.apply(self.glrenderer)
          ogl.glPopName()
    
    @staticmethod
    def newDefaultNurbsPatch():
        """ return a default nurbs patch """
        return NurbsPatch([[Vector4(j-1.5,i-1.5,0,1) for j in range(4)] for i in range(4)], ustride=10, vstride=10)

    def getNurbsPatch(self):
        """ return the edited nurbs patch """
        return self.nurbsPatch

    def setNurbsPatch(self,nurbsPatch):
        """ set the nurbs patch to edit """
        self.clear()
        self.nurbsPatch=nurbsPatch
        self.setNurbsPatchView()
        self.setControlPointManipulators()

    def setControlPointManipulators(self):
        import itertools
        dims = list(map(float, self.getCtrlPointsDim()))
        def color(idx):
            r = 30 + int(220*idx[0]/dims[0])
            g = 30 + int(220*idx[1]/dims[1]) if len(dims) > 1 else 250
            b = 30 + int(220*idx[2]/dims[2]) if len(dims) > 2 else 250
            return (r,g,b)

        ctrlpoints = self.getCtrlPoints()
        self.ctrlPointManipulators = {}
        idxiter = itertools.product(*[list(range(d)) for d in self.getCtrlPointsDim()])
        for pid, index in enumerate(idxiter):
            ctrlPoint = CtrlPoint(ctrlpoints[index].project(), Pos4Setter(self.getCtrlPoints(),index),color=color(index),id=pid)
            ctrlPoint.setCallBack(self.__propagate_valuechanged__)
            self.ctrlPointManipulators[index] = ctrlPoint

        self.setSceneBoundingBox(*self.getBounds())

    def setNurbsPatchView(self):
        self.nurbsShape.geometry=self.nurbsPatch
    

    def helpString(self):
        return helpstr


    def closeEvent(self,event):
        """close the windows properly"""
        helpwidget = self.helpWidget()
        if helpwidget and helpwidget.isVisible() :
            helpwidget.hide()
        QGLViewer.closeEvent(self,event)

    def setUStride(self, value):
        self.nurbsPatch.ustride = value
        self.valueChanged.emit() 
        self.updateGL()

    def setVStride(self, value):
        self.nurbsPatch.vstride = value
        self.valueChanged.emit() 
        self.updateGL()



class NurbsPatchEditor(QtWidgets.QWidget):
    valueChanged = pyqtSignal()
    manipulated = pyqtSignal()

    def __init__(self, parent, dimension = 2):
        QtWidgets.QWidget.__init__(self, parent)
        self.gridLayout = QtWidgets.QGridLayout(self)
        self.gridLayout.setContentsMargins(2, 0, 2, 2)

        self.ViewClass = NurbsPatchEditorView if dimension == 2 else NurbsPatch3DEditorView
        self.view = self.ViewClass(self)
        self.view.valueChanged.connect(self.propagate_valuechanged)
        self.view.manipulated.connect(self.propagate_manipulated)

        self.stridenames = [n for n in ['u','v','w'] if hasattr(self.view,'set'+n.upper()+'Stride')]
        dim = len(self.stridenames)
        maxrow = 1+2*dim

        self.gridLayout.addWidget(self.view, 0, 0, 1, maxrow)
        label = QtWidgets.QLabel(self)
        label.setText( "Discretization")
        self.gridLayout.addWidget(label, 1, 0, 1, 1)


        self.sliders = []
        for i,stridename in enumerate(self.stridenames):
            strideSlider = QtWidgets.QSlider(self)
            strideSlider.setMinimum(2)
            strideSlider.setMaximum(50)
            strideSlider.setOrientation(QtCore.Qt.Horizontal)
            strideSlider.setValue(getattr(self.view.nurbsPatch,stridename+'stride'))
            strideSlider.valueChanged.connect(getattr(self.view,'set'+stridename.upper()+'Stride'))
            self.sliders.append(strideSlider)
            self.gridLayout.addWidget(strideSlider, 1, 1+2*i, 1, 1)
            spacerItem = QtWidgets.QSpacerItem(20, 20, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Minimum)
            self.gridLayout.addItem(spacerItem, 1, 1+2*i+1, 1, 1)

        
    def setNurbsPatch(self,nurbsPatch):
        self.view.setNurbsPatch(nurbsPatch)
        for slider, stridename in zip(self.sliders,self.stridenames):
            slider.setValue(getattr(self.view.nurbsPatch,stridename+'stride'))

    def getNurbsPatch(self):
        return self.view.getNurbsPatch()
 
    def propagate_valuechanged(self):
        self.valueChanged.emit()

    def propagate_manipulated(self):
        self.manipulated.emit()


class NurbsPatch3DEditorView(NurbsPatchEditorView):
    def __init__(self,parent):
        """ Constructor 
            :param parent: the parent widget
        """
        NurbsPatchEditorView.__init__(self,parent)
        self.setStateFileName('.nurbspatch3deditor.xml') 
        self.patchWire = None
        self.ctrlWire = None



    def getBounds(self):
        """ Get the Bounding Box of the scene:
            return the minpos and maxpos that can be used by setSceneBoundingBox function"""
        bbx = self.nurbsPatch.getBoundingBox()
        return Vec(*bbx.lowerLeftCorner),Vec(*bbx.upperRightCorner)

    @staticmethod
    def newDefaultNurbsPatch():
        """ return a default nurbs patch """
        from openalea.plantgl.scenegraph.nurbspatch_nd import NurbsPatch3D
        return NurbsPatch3D([[[Vector4(j-1.5,i-1.5,k-1.5,1) for j in range(4)] for i in range(4)] for k in range(4)], ustride=5,vstride=5,wstride=5)

    def setNurbsPatchView(self):
        pass

    def getCtrlPoints(self):
        return self.nurbsPatch.points

    def getCtrlPointsDim(self):
        return self.getCtrlPoints().shape

    def resetDisplay(self, pid = None):
        NurbsPatchEditorView.resetDisplay(self, pid)
        self.patchWire = None
        self.ctrlWire = None

    def drawPatch(self):
        pass

    def drawPatchWire(self):
        stride = 5
        if True : #self.patchWire is None:
            self.patchWire = Scene()

            du = (self.nurbsPatch.uknotList[-1]-self.nurbsPatch.uknotList[0])/self.nurbsPatch.ustride
            for u in list(np.arange(self.nurbsPatch.uknotList[0],
                               self.nurbsPatch.uknotList[-1], 
                               du))+[self.nurbsPatch.uknotList[-1]]:
                p = self.nurbsPatch.getUPatch(u)

                pdu = (p.lastUKnot-p.firstUKnot)/p.vstride
                for pu in list(np.arange(p.firstUKnot,p.lastUKnot, pdu))+[p.lastUKnot]:
                    c = p.getIsoUSectionAt(pu)
                    self.patchWire.add(c)

                pdv = (p.lastVKnot-p.firstVKnot)/p.ustride
                for pv in list(np.arange(p.firstVKnot,p.lastVKnot, pdv))+[p.lastVKnot]:
                    c = p.getIsoVSectionAt(pv)
                    self.patchWire.add(c)

            dv = (self.nurbsPatch.vknotList[-1]-self.nurbsPatch.vknotList[0])/self.nurbsPatch.vstride
            for v in list(np.arange(self.nurbsPatch.vknotList[0],
                               self.nurbsPatch.vknotList[-1], 
                               dv))+[self.nurbsPatch.vknotList[-1]]:
                p = self.nurbsPatch.getVPatch(v)

                pdv = (p.lastVKnot-p.firstVKnot)/p.vstride
                for pv in list(np.arange(p.firstVKnot,p.lastVKnot, pdv))+[p.lastVKnot]:
                    c = p.getIsoVSectionAt(pv)
                    self.patchWire.add(c)

        self.patchWire.apply(self.glrenderer)

    def drawCtrlWire(self):
        if True : #self.ctrlWire is None:
            m = Material((0,0,255),1)
            self.ctrlWire = Scene()
            opoints = self.nurbsPatch.points
            for i0,i1 in [(0,1),(1,2),(0,2)]:
                for index in itertools.product(*[list(range(d)) for d in [opoints.shape[i0],opoints.shape[i1]]]):
                    i = [slice(None,None),slice(None,None),slice(None,None)]
                    i[i0] = index[0]
                    i[i1] = index[1]
                    points = opoints[tuple(i)]
                    pid = ((i[2]*10000 if not isinstance(i[2],slice) else 0)+
                           (i[1]*100 if not isinstance(i[1],slice) else 0)+
                           (i[0] if not isinstance(i[0],slice) else 0))
                    self.ctrlWire.add(Shape(Polyline([p.project() for p in points], width=2), m, id = pid))

        self.ctrlWire.apply(self.glrenderer)

    def drawWithNames(self):
        """ draw control lines with names """
        if self.ctrlWire:
            for sh in self.ctrlWire:
              ogl.glPushName(sh.id)
              sh.apply(self.glrenderer)
              ogl.glPopName()

    def addElement(self, id):
        print("For now, adding control points is not implemented")
        pass

    def setWStride(self, value):
        self.nurbsPatch.wstride = value
        self.valueChanged.emit() 
        self.updateGL()



#########################################################

def main(patch3d = False):
    """the main program, here we create a NurbsPatchEditor and make it draw itself"""
    import sys
    qapp = QApplication([])
    if patch3d:
        viewer = NurbsPatchEditor(None, 3)
    else:
        viewer = NurbsPatchEditor(None)
    viewer.setWindowTitle("NurbsPatchEditor")
    viewer.show()
    qapp.exec_()

if __name__ == '__main__':
    import sys
    patch3d = ('3' in sys.argv)
    main(patch3d)
