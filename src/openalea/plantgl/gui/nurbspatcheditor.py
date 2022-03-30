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

helpstr = """<h2>N u r b s  E d i t o r</h2>
This application allows the user to edit a nurbs object dynamically.
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

class AbstractNurbsObjectEditorView(QGLViewer):
    """ The class NurbsPatchEditor is the viewer of the scene, it contains all the informations about the NurbsPatch, that's why a getter and a setter have been created, the NubrsPatch is defined by a 2 dimensional Array of 3d Vectors"""
   
    Edit,Rotate = list(range(2))
    valueChanged = pyqtSignal()
    manipulated = pyqtSignal()
    selectionChanged = pyqtSignal()
    
    def __init__(self,parent):
        """ Constructor 
            :param parent: the parent widget
        """
        QGLViewer.__init__(self,parent)
        self.setStateFileName('.nurbsobjecteditor.xml') 
     
        self.mode = AbstractNurbsObjectEditorView.Edit
        
        # the nurbs patch
        self.nurbsObject = None
        
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

        self._drawNurbsObject = True
        self._drawGrid = True
    
        #creation of a default NurbsPatch
        self.setNurbsObject(self.newDefaultNurbsObject())

    def getSelection(self):
        if self.selectionManipulator:
            return self.selectionManipulator.selection
        else:
            return []

    def clear(self):
        """ clear current edition """
        self.ctrlPointManipulators = {}
        self.clearSelectionManipulator()
        self.resetDisplay()

    def getCtrlPoints(self):
        return self.nurbsObject.ctrlPointMatrix

    def getCtrlPointsDim(self):
        return self.getCtrlPoints().sizes()

    def getCtrlPointManipulators(self):
        return self.ctrlPointManipulators

    def getBounds(self):
        """ Get the Bounding Box of the scene:
            return the minpos and maxpos that can be used by setSceneBoundingBox function"""
        self.nurbsObject.apply(self.bboxcomputer)
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
        self.mode = AbstractNurbsObjectEditorView.Edit
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
                self.selectionChanged.emit()
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
            if self.mode != AbstractNurbsObjectEditorView.Edit:
                self.mode = AbstractNurbsObjectEditorView.Edit
                self.setMouseBinding(Qt.NoModifier,Qt.LeftButton,QGLViewer.FRAME,QGLViewer.TRANSLATE)
                self.setMouseBinding(Qt.NoModifier,Qt.RightButton,QGLViewer.FRAME,QGLViewer.NO_MOUSE_ACTION)

                self.setMouseBinding(Qt.ControlModifier,Qt.LeftButton,  QGLViewer.CAMERA,QGLViewer.ROTATE)
                self.setMouseBinding(Qt.ControlModifier,Qt.RightButton, QGLViewer.CAMERA,QGLViewer.TRANSLATE)
                self.setMouseBinding(Qt.ControlModifier,Qt.MiddleButton,QGLViewer.CAMERA,QGLViewer.ZOOM)     
        else:
            if self.mode != AbstractNurbsObjectEditorView.Rotate:
                self.mode = AbstractNurbsObjectEditorView.Rotate
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
            self.selectionChanged.emit()               
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
            self.selectionChanged.emit()
                
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
        selectionOccur = False
        for cCtrlPoint in self.getCtrlPointManipulators().values():
                point=self.camera().projectedCoordinatesOf(cCtrlPoint.position())
                if (self.__rectangle.contains(int(point.x),int(point.y))):
                    selectionOccur = True
                    if not self.selectionManipulator:
                        self.selectionManipulator = SelectionManipulator()
                        self.selectionManipulator.valueChanged.connect(self.__propagate_valuechanged__) # QObject.connect(self.selectionManipulator,SIGNAL("valueChanged()"),self.__propagate_valuechanged__)
                    self.selectionManipulator.toogleSelection(cCtrlPoint)
        if self.selectionManipulator and self.selectionManipulator.empty():
            self.clearSelectionManipulator()
        self.ctrlPointDisplay = None
        if selectionOccur :
            self.selectionChanged.emit()

    def keyPressEvent(self,e):
        modifiers = e.modifiers()
        # A simple switch on e->key() is not sufficient if we want to take state key into account.
        # With a switch, it would have been impossible to separate 'F' from 'CTRL+F'.
        # That's why we use imbricated if...else and a "handled" boolean.
        if ((e.key()==Qt.Key_K) and (modifiers==Qt.NoModifier)):
            self._drawGrid = not self._drawGrid
            self.updateGL()
        elif (e.key()==Qt.Key_G) and (modifiers==Qt.NoModifier):
            self._drawNurbsObject = not self._drawNurbsObject
            self.updateGL()
        else:
            QGLViewer.keyPressEvent(self,e)

    def addElement(self, selection):
        nbcolumn = self.getCtrlPoints().getColumnNb()
        minp,maxp = self.getBounds()
        diff = norm(maxp-minp)/20
        if selection >= nbcolumn:
            row = selection-nbcolumn
            newpatch = addRowToPatch(self.nurbsObject,row,Vector4(diff,0))
            selectedPoints = [(row,i) for i in range(nbcolumn)]
        else:
            newpatch = addColumnToPatch(self.nurbsObject,selection,Vector4(diff,0))
            selectedPoints = [(i,selection) for i in range(self.getCtrlPoints().getRowNb())]
        self.setNurbsObject(newpatch)
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
        
        if self._drawNurbsObject:
            self.drawNurbsObjectWire()
        if self._drawGrid:
            self.drawCtrlWire()

        # patch and control point shpere rendering
        ogl.glPolygonMode(ogl.GL_FRONT_AND_BACK,ogl.GL_FILL)
        ogl.glEnable(ogl.GL_LIGHTING)
        ogl.glLightModeli(ogl.GL_LIGHT_MODEL_TWO_SIDE, ogl.GL_TRUE)
        ogl.glEnable(ogl.GL_BLEND)
        ogl.glBlendFunc(ogl.GL_SRC_ALPHA,ogl.GL_ONE_MINUS_SRC_ALPHA)

        if self._drawNurbsObject:
            self.drawNurbsObject()
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
        self.nurbsObject.apply(self.ctrlrenderer)

    def drawNurbsObject(self):
        sc=Scene([self.nurbsShape])

        # patch rendering
        sc.apply(self.glrenderer)

    def drawNurbsObjectWire(self):
        shwire = Shape(self.nurbsObject,Material((120,255,0),0.8))
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
    

    def getNurbsObject(self):
        """ return the edited nurbs patch """
        return self.nurbsObject

    def setNurbsObject(self,nurbsObject):
        """ set the nurbs patch to edit """
        self.clear()
        self.nurbsObject=nurbsObject
        self.setNurbsObjectView()
        self.setControlPointManipulators()

    def setControlPointManipulators(self):
        import itertools
        dims = list(map(float, self.getCtrlPointsDim()))
        def color(idx):
            try:
                r = 30 + int(220*idx[0]/dims[0])
                g = 30 + int(220*idx[1]/dims[1]) if len(dims) > 1 else 250
                b = 30 + int(220*idx[2]/dims[2]) if len(dims) > 2 else 250
            except:
                r = 250
                g = 30 + int(220*idx/dims[0])
                b = 250
            return (r,g,b)

        ctrlpoints = self.getCtrlPoints()
        self.ctrlPointManipulators = {}
        dims = self.getCtrlPointsDim()
        if len(dims) == 1:
            idxiter = list(range(dims[0]))
        else:
            idxiter = itertools.product(*[list(range(d)) for d in dims])

        for pid, index in enumerate(idxiter):
            ctrlPoint = CtrlPoint(ctrlpoints[index], Pos4Setter(self.getCtrlPoints(),index, ctrlpoints[index].w),color=color(index),id=pid)
            ctrlPoint.setCallBack(self.__propagate_valuechanged__)
            self.ctrlPointManipulators[index] = ctrlPoint

        self.setSceneBoundingBox(*self.getBounds())

    def setNurbsObjectView(self):
        self.nurbsShape.geometry=self.nurbsObject
    

    def helpString(self):
        return helpstr


    def closeEvent(self,event):
        """close the windows properly"""
        helpwidget = self.helpWidget()
        if helpwidget and helpwidget.isVisible() :
            helpwidget.hide()
        QGLViewer.closeEvent(self,event)

    def setUStride(self, value):
        self.nurbsObject.ustride = value
        self.valueChanged.emit() 
        self.updateGL()

    def setWeigthToSelection(self, value):
        if self.selectionManipulator:
            try:
                if len(value) > 0:
                    for v,p in zip(value,self.selectionManipulator.selection):
                        p.position_setter.weight = v
                        p.__propagate_position_change__()
            except:
                if abs(value) < 1e-3:
                    return
                for p in self.selectionManipulator.selection:
                    p.position_setter.weight = value
                    p.__propagate_position_change__()
        self.updateGL()


class NurbsPatch2DEditorView(AbstractNurbsObjectEditorView):
    def __init__(self,parent):
        """ Constructor 
            :param parent: the parent widget
        """
        AbstractNurbsObjectEditorView.__init__(self,parent)

    def setVStride(self, value):
        self.nurbsObject.vstride = value
        self.valueChanged.emit() 
        self.updateGL()
 
    def applyScaling(self, scale):
        nurbscurve = self.getNurbsObject()
        nurbscurve.ctrlPointMatrix.scale(Vector4(scale,1))
        self.setNurbsObject(nurbscurve)

    @staticmethod
    def newDefaultNurbsObject(u = 4, v = 4):
        """ return a default nurbs patch """
        du = 1/(u-1)
        dv = 1/(v-1)
        udegree = min(u-1,3)
        vdegree = min(v-1,3)
        return NurbsPatch([[Vector4(0,i*du-0.5,j*dv-0.5,1) for j in range(v)] for i in range(u)], 
                            udegree=udegree,vdegree=vdegree,
                            ustride=10,vstride=10)


DimensionViewClass = {  2 :  NurbsPatch2DEditorView }


class NurbsPatch3DEditorView(NurbsPatch2DEditorView):
    def __init__(self,parent):
        """ Constructor 
            :param parent: the parent widget
        """
        NurbsPatch2DEditorView.__init__(self,parent)
        self.setStateFileName('.nurbspatch3deditor.xml') 
        self.patchWire = None
        self.ctrlWire = None



    def getBounds(self):
        """ Get the Bounding Box of the scene:
            return the minpos and maxpos that can be used by setSceneBoundingBox function"""
        bbx = self.nurbsObject.getBoundingBox()
        return Vec(*bbx.lowerLeftCorner),Vec(*bbx.upperRightCorner)

    @staticmethod
    def newDefaultNurbsObject(u = 4, v = 4, w = 4):
        """ return a default nurbs patch """
        from openalea.plantgl.scenegraph.nurbspatch_nd import NurbsPatch3D
        res = NurbsPatch3D.default(u,v,w)
        return res

    def setNurbsObjectView(self):
        pass

    def getCtrlPoints(self):
        return self.nurbsObject.ctrlPointMatrix

    def getCtrlPointsDim(self):
        return self.getCtrlPoints().shape

    def applyScaling(self, scale):
        nurbspatch = self.getNurbsObject()
        nurbspatch.ctrlPointMatrix = [[[Vector4(v.x*scale.x,v.y*scale.y,v.z*scale.z,v.w) for v in ci] for ci in cj ]for cj in nurbspatch.ctrlPointMatrix]
        self.setNurbsObject(nurbspatch)

    def resetDisplay(self, pid = None):
        NurbsPatch2DEditorView.resetDisplay(self, pid)
        self.patchWire = None
        self.ctrlWire = None

    def drawNurbsObject(self):
        pass

    def drawNurbsObjectWire(self):
        stride = 5
        if True : #self.patchWire is None:
            self.patchWire = Scene()

            du = (self.nurbsObject.uknotList[-1]-self.nurbsObject.uknotList[0])/self.nurbsObject.ustride
            for u in list(np.arange(self.nurbsObject.uknotList[0],
                               self.nurbsObject.uknotList[-1], 
                               du))+[self.nurbsObject.uknotList[-1]]:
                p = self.nurbsObject.getUPatch(u)

                pdu = (p.lastUKnot-p.firstUKnot)/p.vstride
                for pu in list(np.arange(p.firstUKnot,p.lastUKnot, pdu))+[p.lastUKnot]:
                    c = p.getIsoUSectionAt(pu)
                    self.patchWire.add(c)

                pdv = (p.lastVKnot-p.firstVKnot)/p.ustride
                for pv in list(np.arange(p.firstVKnot,p.lastVKnot, pdv))+[p.lastVKnot]:
                    c = p.getIsoVSectionAt(pv)
                    self.patchWire.add(c)

            dv = (self.nurbsObject.vknotList[-1]-self.nurbsObject.vknotList[0])/self.nurbsObject.vstride
            for v in list(np.arange(self.nurbsObject.vknotList[0],
                               self.nurbsObject.vknotList[-1], 
                               dv))+[self.nurbsObject.vknotList[-1]]:
                p = self.nurbsObject.getVPatch(v)

                pdv = (p.lastVKnot-p.firstVKnot)/p.vstride
                for pv in list(np.arange(p.firstVKnot,p.lastVKnot, pdv))+[p.lastVKnot]:
                    c = p.getIsoVSectionAt(pv)
                    self.patchWire.add(c)

        self.patchWire.apply(self.glrenderer)

    def drawCtrlWire(self):
        if True : #self.ctrlWire is None:
            m = Material((0,0,255),1)
            self.ctrlWire = Scene()
            opoints = self.nurbsObject.ctrlPointMatrix
            for i0,i1 in [(0,1),(1,2),(0,2)]:
                for index in itertools.product(*[list(range(d)) for d in [opoints.shape[i0],opoints.shape[i1]]]):
                    i = [slice(None,None),slice(None,None),slice(None,None)]
                    i[i0] = index[0]
                    i[i1] = index[1]
                    points = opoints[tuple(i)]
                    pid = ((i[2]*10000 if not isinstance(i[2],slice) else 0)+
                           (i[1]*100 if not isinstance(i[1],slice) else 0)+
                           (i[0] if not isinstance(i[0],slice) else 0))
                    self.ctrlWire.add(Shape(Polyline([(p.x,p.y,p.z) for p in points], width=2), m, id = pid))

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
        self.nurbsObject.wstride = value
        self.valueChanged.emit() 
        self.updateGL()

DimensionViewClass[3] = NurbsPatch3DEditorView 


class NurbsCurveEditorView(AbstractNurbsObjectEditorView):
    def __init__(self,parent):
        """ Constructor 
            :param parent: the parent widget
        """
        AbstractNurbsObjectEditorView.__init__(self,parent)
        self.setStateFileName('.nurbscurve3deditor.xml') 
        


    def getBounds(self):
        """ Get the Bounding Box of the scene:
            return the minpos and maxpos that can be used by setSceneBoundingBox function"""
        bbx = BoundingBox(self.nurbsObject)
        return Vec(*bbx.lowerLeftCorner),Vec(*bbx.upperRightCorner)

    @staticmethod
    def newDefaultNurbsObject(u = 4):
        """ return a default nurbs patch """
        from openalea.plantgl.scenegraph import NurbsCurve
        dx = 1/(u-1)
        curve = NurbsCurve([Vector4(i*dx-0.5,0,0,1) for i in range(u)], stride=5)

    def setNurbsObjectView(self):
        pass

    def getCtrlPoints(self):
        return self.nurbsObject.ctrlPointList

    def getCtrlPointsDim(self):
        return [len(self.nurbsObject.ctrlPointList)]

    def applyScaling(self, scale):
        nurbscurve = self.getNurbsObject()
        nurbscurve.ctrlPointList.scale(Vector4(scale,1))
        self.setNurbsObject(nurbscurve)

    def resetDisplay(self, pid = None):
        AbstractNurbsObjectEditorView.resetDisplay(self, pid)

    def drawNurbsObject(self):
        pass

    def drawNurbsObjectWire(self):
        self.nurbsObject.apply(self.glrenderer)

    def drawCtrlWire(self):
        Material((255,255,255),1).apply(self.glrenderer)        
        self.nurbsObject.apply(self.ctrlrenderer)

    def drawWithNames(self):
        """ draw control lines with names """


    def addElement(self, id):
        print("For now, adding control points is not implemented")
        pass

    def setUStride(self, value):
        self.nurbsObject.stride = value
        self.valueChanged.emit() 
        self.updateGL()

DimensionViewClass[1] = NurbsCurveEditorView

class NurbsObjectEditor(QtWidgets.QWidget):
    valueChanged = pyqtSignal()
    manipulated = pyqtSignal()

    def __init__(self, parent, dimension = 2):
        QtWidgets.QWidget.__init__(self, parent)
        self.gridLayout = QtWidgets.QGridLayout(self)
        self.gridLayout.setContentsMargins(2, 0, 11, 2)

        self.view = DimensionViewClass[dimension](self)
        self.view.valueChanged.connect(self.propagate_valuechanged)
        self.view.manipulated.connect(self.propagate_manipulated)

        self.stridenames = [n for n in ['u','v','w'] if hasattr(self.view,'set'+n.upper()+'Stride')]
        dim = len(self.stridenames)
        maxrow = 1+2*dim

        self.gridLayout.addWidget(self.view, 0, 0, 2, maxrow-1)
        self.view.selectionChanged.connect(self.selectionEvent)

        self.weightSpinBox = QtWidgets.QDoubleSpinBox(self)
        self.weightSpinBox.setMinimum(-100)
        self.weightSpinBox.setMaximum(100)
        self.weightSpinBox.valueChanged.connect(self.setWeigthToSelection)
        self.weightSpinBox.setEnabled(False)
        self.gridLayout.addWidget(self.weightSpinBox, 0, maxrow, 1, 1)

        self.weigthSlider = QtWidgets.QSlider(self)
        self.weigthSlider.setMinimum(-200)
        self.weigthSlider.setMaximum(200)
        self.weigthSlider.setOrientation(QtCore.Qt.Vertical)
        self.weigthSlider.setValue(0)
        self.weigthSlider.sliderPressed.connect(self.weigthSliderPressed)
        self.weigthSlider.sliderReleased.connect(self.weigthSliderReleased)
        self.weigthSlider.sliderMoved.connect(self.setWeigthRatioToSelection)
        self.weigthSlider.setEnabled(False)
        self.gridLayout.addWidget(self.weigthSlider, 1, maxrow, 1, 1)

        label = QtWidgets.QLabel(self)
        label.setText( "Discretization")
        self.gridLayout.addWidget(label, 2, 0, 1, 1)
        self.sliders = []
        for i,stridename in enumerate(self.stridenames):
            strideSlider = QtWidgets.QSlider(self)
            strideSlider.setMinimum(2)
            strideSlider.setMaximum(50)
            strideSlider.setOrientation(QtCore.Qt.Horizontal)
            strideSlider.setValue(getattr(self.view.nurbsObject,stridename+'stride'))
            strideSlider.valueChanged.connect(getattr(self.view,'set'+stridename.upper()+'Stride'))
            self.sliders.append(strideSlider)
            self.gridLayout.addWidget(strideSlider, 2, 1+2*i, 1, 1)
            spacerItem = QtWidgets.QSpacerItem(20, 20, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Minimum)
            self.gridLayout.addItem(spacerItem, 2, 1+2*i+1, 1, 1)


    def selectionEvent(self):
        active = (len(self.view.getSelection()) != 0)
        self.weightSpinBox.setEnabled(active)
        self.weigthSlider.setEnabled(active)
        if active:
            self.weightSpinBox.setValue(sum([p.position_setter.weight for p in self.view.selectionManipulator.selection])/len(self.view.selectionManipulator.selection))


    def weigthSliderPressed(self):
        if self.view.selectionManipulator:
            self.cweight = sum([p.position_setter.weight for p in self.view.selectionManipulator.selection])/len(self.view.selectionManipulator.selection)
        else:
            self.cweight = 1

    def weigthSliderReleased(self):
        self.weigthSlider.setValue(0)

    def setWeigthRatioToSelection(self, value):
        ratio = 1+(value / 100.)
        self.weightSpinBox.setValue(self.cweight*ratio)
        self.view.setWeigthToSelection(self.cweight*ratio)

    def setWeigthToSelection(self, value):
        self.view.setWeigthToSelection(value)

    def setNurbsObject(self, nurbsObject):
        self.view.setNurbsObject(nurbsObject)
        for slider, stridename in zip(self.sliders,self.stridenames):
            slider.setValue(getattr(self.view.nurbsObject,stridename+'stride'))

    def getNurbsObject(self):
        return self.view.getNurbsObject()
 
    def propagate_valuechanged(self):
        self.valueChanged.emit()

    def propagate_manipulated(self):
        self.manipulated.emit()

    def rescaleObject(self):
        """ return a default nurbs patch """
        Dialog = QtWidgets.QDialog(self)
        gridLayout = QtWidgets.QGridLayout(Dialog)
        Dialog.values = []
        for i,axis in enumerate(['X','Y','Z']):
            sectionLabel = QtWidgets.QLabel(Dialog)
            sectionLabel.setText(axis)
            gridLayout.addWidget(sectionLabel, i, 0)
            spacer = QtWidgets.QSpacerItem(1, 10)
            gridLayout.addItem(spacer, i, 1)
            valuebox = QtWidgets.QDoubleSpinBox(Dialog)
            valuebox.setDecimals(3)
            valuebox.setMinimum(0.001)
            valuebox.setMaximum(9999999999)
            valuebox.setValue(1)
            Dialog.values.append(valuebox)
            gridLayout.addWidget(valuebox, i, 2)
        buttonBox = QtWidgets.QDialogButtonBox(Dialog)
        buttonBox.setOrientation(Qt.Horizontal)
        buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel | QtWidgets.QDialogButtonBox.Ok)
        buttonBox.accepted.connect(Dialog.accept)
        buttonBox.rejected.connect(Dialog.reject)
        gridLayout.addWidget(buttonBox, 3, 0, 1, 2)
        if Dialog.exec_():
            scale = Vector3([v.value() for v in Dialog.values])
            self.view.applyScaling(scale)

    def createCurstomDefaultObject(self):
        """ return a default nurbs patch """
        import inspect
        Dialog = QtWidgets.QDialog(self)
        gridLayout = QtWidgets.QGridLayout(Dialog)
        Dialog.values = []
        pname = inspect.getfullargspec(self.view.newDefaultNurbsObject).args
        for i,axis in enumerate(pname):
            sectionLabel = QtWidgets.QLabel(Dialog)
            sectionLabel.setText(axis)
            gridLayout.addWidget(sectionLabel, i, 0)
            spacer = QtWidgets.QSpacerItem(1, 10)
            gridLayout.addItem(spacer, i, 1)
            valuebox = QtWidgets.QSpinBox(Dialog)
            valuebox.setMinimum(2)
            valuebox.setMaximum(1000)
            valuebox.setValue(4)
            Dialog.values.append(valuebox)
            gridLayout.addWidget(valuebox, i, 2)
        buttonBox = QtWidgets.QDialogButtonBox(Dialog)
        buttonBox.setOrientation(Qt.Horizontal)
        buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel | QtWidgets.QDialogButtonBox.Ok)
        buttonBox.accepted.connect(Dialog.accept)
        buttonBox.rejected.connect(Dialog.reject)
        gridLayout.addWidget(buttonBox, 3, 0, 1, 2)
        if Dialog.exec_():
            nbpoints = [v.value() for v in Dialog.values]
            self.setNurbsObject(self.view.newDefaultNurbsObject(*nbpoints))

    def createDefaultObject(self, *dims):
        self.setNurbsObject(self.view.newDefaultNurbsObject(*dims))


#########################################################

def main(dim = 2):
    """the main program, here we create a NurbsPatchEditor and make it draw itself"""
    import sys
    qapp = QApplication([])
    viewer = NurbsObjectEditor(None, dim)
    viewer.setWindowTitle("NurbsEditor")
    viewer.show()
    qapp.exec_()

if __name__ == '__main__':
    import sys
    dimension = int(sys.argv[1]) if len(sys.argv) > 1 else 2
    main(dimension)
