from openalea.plantgl.scenegraph import *
from openalea.plantgl.algo import *
from openalea.plantgl.math import *
from math import pow,log

from openalea.plantgl.gui.qt import QtCore, QtGui, QtWidgets, QtOpenGL
from openalea.plantgl.gui.qt.QtCore import QEvent, QObject, QPoint, Qt, Signal, qWarning
from openalea.plantgl.gui.qt.QtGui import QColor, QImage, QFont, QOpenGLTexture
#from openalea.plantgl.gui.qt.QtOpenGL import QOpenGLWidget
from openalea.plantgl.gui.qt.QtWidgets import QFileDialog, QApplication, QOpenGLWidget

class Curve2DConstraint:
    def __init__(self):
        pass
    def checkInitialCurve(self,curve):
        pass
    def movePointEvent(self,newpoint,selection,curve):
        return newpoint
    def addPointEvent(self,newpoint,curve):
        u = curve.findClosest(newpoint)
        nbPoints = curve.nbPoints()
        if u <= 0.001 : return 0,newpoint
        if u < nbPoints :
            return int(u)+1,newpoint
        else:
            return nbPoints,newpoint
    @staticmethod
    def defaultCurve(nbP=4):
        return NurbsCurve2D(Point3Array([(-0.5+float(i)/(nbP-1),0) for i in range(nbP)],1) )

class FuncConstraint:
    def __init__(self,bounds=(0,1)):
        self.bounds = bounds
    def setBounds(self,bounds):
        self.bounds = bounds
    def checkInitialCurve(self,curve):
        assert curve.getPoint(0)[0] == 0 and curve.getPoint(-1)[0] == 1
    def movePointEvent(self,newpoint,selection,curve):
        p = (newpoint[0],newpoint[1])
        nbPoints = curve.nbPoints()
        minbound, maxbound = self.bounds
        if selection > 0:
            minbound = curve.getPoint(selection-1)[0]
        else :
            p = (minbound,p[1])
        if selection < nbPoints-1:
            maxbound = curve.getPoint(selection+1)[0]
        else:
            p = (maxbound,p[1])
        if minbound > p[0] :
                p = (minbound,p[1])
        elif p[0] > maxbound:
            p = (maxbound,p[1])
        return p
    def addPointEvent(self,newpoint,curve):
        if self.bounds[0] < newpoint[0] < self.bounds[1]:
            for i,p in enumerate(curve.points()):
                if p[0] > newpoint[0]:
                    return i,newpoint
        else:
            return None
    @staticmethod
    def defaultCurve(nbP=4):
        return NurbsCurve2D(Point3Array([(float(i)/(nbP-1),0) for i in range(nbP)],1) )

class Curve2DAccessor:
    def __init__(self):
        pass
    def nbPoints(self):
        pass
    def insertPoint(self,index,npoint):
        pass
    def setPoint(self,index,npoint):
        pass
    def getPoint(self,index):
        pass
    def setPointWeight(self,index,npoint):
        pass
    def getPointWeight(self,index):
        return 1
    def delPoint(self,index):
        pass
    def findClosest(self,p):
        pass
    def bounds(self):
        pass
    def hasWeights(self):
        return False
    def hasStride(self):
        return False
    def sewPoints(self, idx0 = 0, idx1 = -1):
        pass

class Bezier2DAccessor (Curve2DAccessor):
    def __init__(self,curve):
        Curve2DAccessor.__init__(self)
        self.checkType(curve)
        self.curve = curve
    def checkType(self,curve):
        assert type(curve) == BezierCurve2D
    def points(self):
        return [(i[0],i[1]) for i in self.curve.ctrlPointList]
    def nbPoints(self):
        return len(self.curve.ctrlPointList)
    def insertPoint(self,index,npoint):
        self.curve.ctrlPointList.insert(index,Vector3(npoint,1))
        self.setKnotList()
    def setPoint(self,index,npoint):
        self.curve.ctrlPointList[index] = Vector3(npoint,self.getPointWeight(index))
    def getPoint(self,index):
        a = self.curve.ctrlPointList[index]
        return (a[0],a[1])
    def setPointWeight(self,index,nweight):
        self.curve.ctrlPointList[index].z = nweight
    def getPointWeight(self,index):
        a = self.curve.ctrlPointList[index]
        return a[2]
    def delPoint(self,index):
        nbp = self.nbPoints()
        if  nbp > self.minpointnb() and not index in [0,nbp-1]:
            del self.curve.ctrlPointList[index]
            self.setKnotList()
            self.strides = len(self.curve.ctrlPointList) * 10
    def findClosest(self,p):
        ctrlpolyline = Polyline(self.curve.ctrlPointList)
        lp,u = ctrlpolyline.findClosest(Vector3(p,1))
        return u
    def minpointnb(self):
        return 3
    def setKnotList(self):
        pass
    def bounds(self):
        minp,maxp = self.curve.ctrlPointList.getBounds()
        return minp.project(),maxp.project()
    def hasWeights(self):
        return True
    def hasStride(self):
        return True
    def sewPoints(self, idx0 = 0, idx1 = -1):
        self.curve.ctrlPointList[idx1] = self.curve.ctrlPointList[idx0]

class Nurbs2DAccessor (Bezier2DAccessor):
    def __init__(self,curve):
        Bezier2DAccessor.__init__(self,curve)
    def checkType(self,curve):
        assert type(curve) == NurbsCurve2D
    def minpointnb(self):
        return self.curve.degree +1
    def setKnotList(self):
        self.curve.setKnotListToDefault()



class Polyline2DAccessor (Curve2DAccessor):
    def __init__(self,curve):
        assert type(curve) == Polyline2D
        self.curve = curve
    def points(self):
        return self.curve.pointList
    def nbPoints(self):
        return len(self.curve.pointList)
    def insertPoint(self,index,npoint):
        self.curve.pointList.insert(index,Vector2(npoint))
    def setPoint(self,index,npoint):
        self.curve.pointList[index] = Vector2(npoint)
    def getPoint(self,index):
        return self.curve.pointList[index]
    def delPoint(self,index):
        nbp = self.nbPoints()
        if  nbp > 2 and not index in [0,nbp-1]:
            del self.curve.pointList[index]
    def findClosest(self,p):
        ctrlpolyline = Polyline(Point3Array(self.curve.pointList,0))
        lp,u = ctrlpolyline.findClosest(Vector3(p,0))
        return u
    def bounds(self):
        return self.curve.pointList.getBounds()
    def sewPoints(self, idx0 = 0, idx1 = -1):
        print('sewPoint',idx0, idx1, self.curve.pointList[idx0])
        self.curve.pointList[idx1] = self.curve.pointList[idx0]


from PyQGLViewer import *
from OpenGL.GL import *
from OpenGL.GLU import *

BLACK_THEME = {'Curve' : (255,255,255), 'BackGround' : (51,51,51), 'Text' : (255,255,255), 'CtrlCurve' : (122,122,0), 'GridStrong' : (102,102,102), 'GridFade' : (51,51,51) , 'Points' : (250,30,30), 'FirstPoint' : (250,30,250), 'SelectedPoint' : (30,250,30), 'DisabledBackGround' : (150,150,150) }
WHITE_THEME = {'Curve' : (255,0,0), 'BackGround' : (255,255,255), 'Text' : (0,0,0), 'CtrlCurve' : (25,0,25), 'GridStrong' : (102,102,102), 'GridFade' : (153,153,153) , 'Points' : (30,250,30), 'FirstPoint' : (250,30,250), 'SelectedPoint' : (30,250,30), 'DisabledBackGround' : (150,150,150)}

class Curve2DEditorView (QGLViewer):

    valueChanged = Signal()
    nbCtrlPointsChanged = Signal(int)
    selectionChanged = Signal()

    def __init__(self,parent,constraints=Curve2DConstraint()):
        QGLViewer.__init__(self,parent)
        self.selection = -1
        self.lastselection = -1
        self.closedCurve = False
        self.setStateFileName('.curveeditor.xml')
        
        self.sphere = Sphere(radius=0.02)
        self.curveshape = Shape()
        self.setTheme() #Color theme
        
        self.pointsConstraints = constraints
        self.accessorType = { NurbsCurve2D : Nurbs2DAccessor,
                              BezierCurve2D : Bezier2DAccessor,
                              Polyline2D : Polyline2DAccessor,}
        self.setCurve(self.newDefaultCurve())
        self.discretizer = Discretizer()
        self.renderer = GLRenderer(self.discretizer)
        self.renderer.renderingMode = GLRenderer.Dynamic
        self.ctrlrenderer = GLCtrlPointRenderer(self.discretizer)
        self.bgimage = False
        self.font = QFont()
        if not hasattr(self,'updateGL'):
            self.updateGL = self.update
        
    def setTheme(self,theme = BLACK_THEME):
        self.curveMaterial = Material(theme['Curve'],1)
        self.defaultColor = QColor(*theme['BackGround'])
        self.disabledBGColor = QColor(*theme['DisabledBackGround'])
        self.textColor = [v/255. for v in theme['Text']]+[1.0]
        self.ctrlCurveColor = [v/255. for v in theme['CtrlCurve']]+[0.0]
        self.gridColor = [v/255. for v in theme['GridStrong']]+[0.0]
        self.gridColor2 = [v/255. for v in theme['GridFade']]+[0.0]
        self.pointColor = Material(theme['Points'],1)
        self.firstPointColor = Material(theme['FirstPoint'],1)
        self.selectedPointColor = Material(theme['SelectedPoint'],1)
        self.curveshape.appearance = self.curveMaterial

    def applyTheme(self,theme = BLACK_THEME):
        self.setTheme(theme)
        if self.isVisible(): 
            self.createControlPointsRep()
            self.update()

    def newDefaultCurve(self):
        return self.pointsConstraints.defaultCurve()

    def hasSelection(self):
        return self.selection != -1

    def hasLastSelection(self):
        return self.lastselection != -1

    def closeCurveEnabled(self):
        return self.closedCurve

    def toggleCurveClosing(self):
        self.enableCurveClosing(not self.closedCurve)

    def enableCurveClosing(self, enabled):
        self.closedCurve = enabled
        if enabled:
            self.curveAccessor.sewPoints()
            self.createControlPointsRep()
            self.update()
            self.valueChanged.emit()
            self.curveshape.geometry.closed = enabled

    def init(self):
        self.updateSceneDimension()
        #self.setHandlerKeyboardModifiers(QGLViewer.CAMERA, Qt.AltModifier)
        #self.setHandlerKeyboardModifiers(QGLViewer.FRAME,  Qt.NoModifier)
        #self.setHandlerKeyboardModifiers(QGLViewer.CAMERA, Qt.ControlModifier)
        self.setMouseBinding(Qt.ControlModifier,Qt.LeftButton,QGLViewer.FRAME,QGLViewer.TRANSLATE)
        self.setMouseBinding(Qt.ControlModifier,Qt.RightButton,QGLViewer.FRAME,QGLViewer.NO_MOUSE_ACTION)
        self.setMouseBinding(Qt.NoModifier,Qt.LeftButton,QGLViewer.CAMERA,QGLViewer.TRANSLATE)
        self.setMouseBinding(Qt.NoModifier,Qt.RightButton,QGLViewer.CAMERA,QGLViewer.NO_MOUSE_ACTION)
        self.camera().setUpVector(Vec(0,1,0))
        self.camera().setType(Camera.ORTHOGRAPHIC)
        self.camConstraint = WorldConstraint()
        self.camConstraint.setRotationConstraintType(AxisPlaneConstraint.FORBIDDEN)
        self.camera().frame().setConstraint(self.camConstraint)
        self.showEntireScene()
        self.setSelectRegionWidth(5)
        self.setSelectRegionHeight(5)
        self.manipulator = ManipulatedFrame()
        self.setManipulatedFrame(self.manipulator)
        self.manipulator.manipulated.connect(self.updatePoints) # QObject.connect(self.manipulator,SIGNAL('manipulated()'),self.updatePoints)
        #self.setBackgroundImage("test.png") #self.openImage() # QQQ 
        
    def getCurve(self):
        """ Get the edited curve """
        return self.curveshape.geometry

    def updateSceneDimension(self):
        minp,maxp = self.curveAccessor.bounds()
        minp,maxp = Vector3(minp,0),Vector3(maxp,0)
        self.setSceneBoundingBox(Vec(*minp),Vec(*maxp))

    def setCurve(self, curve, closed = None):
        """ Set the edited curve """
        self.selection = -1
        self.lastselection = -1
        if hasattr(curve,'closed'):
            print(curve.closed)
        self.closedCurve = curve.closed if hasattr(curve,'closed') else (closed if not closed is None else False)
        self.curveshape.geometry = curve
        curve.width=2
        self.curveAccessor = self.accessorType[type(curve)](curve)
        self.pointsConstraints.checkInitialCurve(self.curveAccessor)
        self.createControlPointsRep()
        self.updateSceneDimension()
        if self.isVisible() : self.showEntireScene()
        
    def openImage(self):
        name, selection = QFileDialog.getOpenFileName(self, "Select an image", ".", "Images (*.png *.xpm *.jpg)");

        # In case of Cancel
        if not name :  return
        
        self.setBackgroundImage(name)
    
        return
        
    def closeImage(self):
        self.bgimage = False

    def setBackgroundImage(self, imagefilename):
        # code taken from the backgroundImage.cpp file that is part of the QGLViewer library
        
        self.u_max = 1.0
        self.v_max = 1.0
         
        # load image
        img = QImage(imagefilename)
        
        self.imageWidth = float(img.width())
        self.imageHeight = float(img.height())

        if img.isNull():
            qWarning("Unable to load file, unsupported file format")
            return

        # qWarning("Loading " + imagefilename + " " + str(img.width()) + "x" + str(img.height()) +" pixels")

        self.qgl_texture = QOpenGLTexture(img)
        self.qgl_texture.setWrapMode(QOpenGLTexture.DirectionS, QOpenGLTexture.ClampToEdge)
        self.qgl_texture.setWrapMode(QOpenGLTexture.DirectionT, QOpenGLTexture.ClampToEdge)
        self.qgl_texture.setMinMagFilters(QOpenGLTexture.LinearMipMapNearest, QOpenGLTexture.LinearMipMapNearest)
        self.qgl_texture.generateMipMaps()
        self.qgl_texture.create()
        self.qgl_texture.bind()

        # QQQ qWarning(str(self.imageWidth) + " " + str(self.imageHeight))

        # 1E-3 needed. Just try with width=128 and see !
        #newWidth  = 1<<(int)(1+log(img.width() -1+1E-3) / log(2.0))
        #newHeight = 1<<(int)(1+log(img.height()-1+1E-3) / log(2.0))
        
        #self.u_max = img.width()  / float(newWidth)
        #self.v_max = img.height() / float(newHeight)

        #if ((img.width()!=newWidth) or (img.height()!=newHeight)):
            #qWarning("Image size set to " + str(newWidth) + "x" + str(newHeight) + " pixels")
        #    img = img.copy(0, 0, newWidth, newHeight)

        #glImg = QImage(QOpenGLWidget.convertToGLFormat(img)) # flipped 32bit RGBA

        # Bind the img texture...
        #glTexImage2D(GL_TEXTURE_2D, 0, 4, glImg.width(), glImg.height(), 0,
        #   GL_RGBA, GL_UNSIGNED_BYTE, glImg.bits().asstring(glImg.sizeInBytes()))
        # Another way to bind img texture:
        # if self.textureId:
            # self.deleteTexture(self.textureId)
            # self.textureId = None
        # self.textureId = self.bindTexture(img,GL_TEXTURE_2D,GL_RGBA)
        # if self.textureId :
            # glBindTexture(GL_TEXTURE_2D, self.textureId)
               
        self.bgimage = True
        
    def drawBackground(self):
        # code taken from the backgroundImage.cpp file that is part of the QGLViewer library

        # set texturing for background image
        #glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        #glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        # Nice texture coordinate interpolation
        #glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

        glDisable(GL_LIGHTING)
        glEnable(GL_TEXTURE_2D)
        glColor3f(1,1,1)
        
        # to draw texture in entire window:
        # self.startScreenCoordinatesSystem(True)
        # # Draws the background quad
        # glNormal3f(0.0, 0.0, 1.0)
        # glBegin(GL_QUADS)
        # glTexCoord2f(0.0, 1.0-self.v_max)
        # glVertex2i(0,0)
        # glTexCoord2f(0.0, 1.0)
        # glVertex2i(0,self.height())
        # glTexCoord2f(self.u_max, 1.0)
        # glVertex2i(self.width(),self.height())
        # glTexCoord2f(self.u_max, 1.0-self.v_max)
        # glVertex2i(self.width(),0)
        # glEnd()
        # self.stopScreenCoordinatesSystem()
        
        # Draws the quad in (0,0) to (1.0,imageHeight/imageWidth)
        self.startScreenCoordinatesSystem(True)
        self.qgl_texture.bind()
        centre_x = self.width() * 0.5 - self.imageWidth*0.5
        centre_y = self.height() * 0.5 - self.imageHeight*0.5

        glNormal3f(0.0, 0.0, 1.0)
        glBegin(GL_QUADS)

        glTexCoord2f(0.0, 1.0)
        glVertex2f(centre_x,centre_y)

        glTexCoord2f(0.0, 1.0-self.v_max)
        glVertex2f(centre_x,centre_y+self.imageHeight)

        glTexCoord2f(self.u_max, 1.0-self.v_max)
        glVertex2f(centre_x+self.imageWidth,centre_y+self.imageHeight)
        
        glTexCoord2f(self.u_max, 1.0)
        glVertex2f(centre_x+self.imageWidth,centre_y)
        glEnd()
        self.stopScreenCoordinatesSystem()
        
        # Depth clear is not absolutely needed. An other option would have been to draw the
        # QUAD with a 0.999 z value (z ranges in [0, 1[ with startScreenCoordinatesSystem()).
        glClear(GL_DEPTH_BUFFER_BIT)
        glDisable(GL_TEXTURE_2D)
        glEnable(GL_LIGHTING)
        
    def draw(self):
        if self.isEnabled():
            if self.bgimage == False:
                self.setBackgroundColor(self.defaultColor)
            else:
                self.drawBackground()
        else:
            self.setBackgroundColor(self.disabledBGColor)
        self.start = self.pointOnEditionPlane(QPoint(0,self.height()-1))
        self.end = self.pointOnEditionPlane(QPoint(self.width()-1,0))
        self.sphere.radius = (self.end[0]-self.start[0])/80
        self.discretizer.clear()
        self.drawGrid()
        self.curveshape.apply(self.renderer)
        try:    glColor4fv(self.ctrlCurveColor)
        except: pass
        self.curveshape.apply(self.ctrlrenderer)
        self.ctrlpts.apply(self.renderer)

    def mRenderText(self, x, y, text):
        #error = glGetError()
        if self.font is None:
            self.font = QFont()
        self.renderText(x,y, 0, text)
        #self.drawText(x,y, text)
        #error = glGetError()
        #if error :  print gluErrorString(error)

    def drawVLine(self, x, y1, y2):
        glVertex3f(x,y1,0)
        glVertex3f(x,y2,0)

    def drawHLine(self, x1, x2, y):
        glVertex3f(x1,y,0)
        glVertex3f(x2,y,0)

    def drawGrid(self):
        xr = self.end[0] - self.start[0]
        xy = self.end[1] - self.start[1]
        if xr <= 0 or xy <= 0:
            return
        nbdigit = max(int(round(log(xr,10))),int(round(log(xy,10))))
        xdelta = pow(10,nbdigit)/10.

        fxval = round(self.start[0]/xdelta)
        lxval = round(self.end[0]/xdelta)
        cxval = fxval*xdelta
        nbiter = int((lxval-fxval))
        glColor4fv(self.gridColor2)
        glLineWidth(1)
        glBegin(GL_LINES)
        for i in range(nbiter+1):
            self.drawVLine(cxval,self.start[1],self.end[1])
            cxval += xdelta
        glEnd()
        cxval = fxval*xdelta
        glColor4fv(self.textColor)
        for i in range(nbiter+1):
            self.mRenderText(cxval,self.start[1],'%.1f' % cxval)
            cxval += xdelta
        glLineWidth(2)
        glColor4fv(self.gridColor)
        cxval = round(self.start[0]/(10*xdelta))*(10*xdelta)
        glBegin(GL_LINES)
        for i in range(int(nbiter/10)+1):
            self.drawVLine(cxval,self.start[1],self.end[1])
            cxval += (10*xdelta)
        glEnd()

        fyval = round(self.start[1]/xdelta)
        lyval = round(self.end[1]/xdelta)
        firstcyval = fyval*xdelta
        cyval = firstcyval
        nbiter = int((lyval-fyval))
        glColor4fv(self.gridColor2)
        glLineWidth(1)
        glBegin(GL_LINES)
        for i in range(nbiter+1):
            self.drawHLine(self.start[0],self.end[0], cyval)
            cyval += xdelta
        glEnd()
        cyval = firstcyval
        glColor4fv(self.textColor)
        for i in range(nbiter+1):
            glVertex3f(self.end[0],cyval,0)
            self.mRenderText(self.start[0],cyval,'%.1f' % cyval)
            cyval += xdelta
        glLineWidth(2)
        glColor4fv(self.gridColor)
        cyval = round(self.start[1]/(10*xdelta))*(10*xdelta)
        glBegin(GL_LINES)
        for i in range(int(nbiter/10)+1):
            self.drawHLine(self.start[0],self.end[0], cyval)
            cyval += (10*xdelta)
        glEnd()

    def drawWithNames(self):
        self.renderer.clear()
        for sh in self.ctrlpts:
          glPushName(sh.id)
          sh.apply(self.renderer)
          glPopName()
        # self.renderer.renderingMode = GLRenderer.Selection
        # self.renderer.selectionMode = GLRenderer.ShapeId
        # self.renderer.beginProcess()
        # self.ctrlpts.apply(self.renderer)
        # self.renderer.endProcess()
        # self.renderer.renderingMode = GLRenderer.Dynamic

    def pointOnEditionPlane(self,pos):
        orig,direction = self.camera().convertClickToLine(pos)
        npoint = orig+direction*(orig[2]/(-direction[2]))
        return (npoint[0],npoint[1])

    def setInteractionMode(self,frame=True):
        if frame:
                self.setMouseBinding(Qt.NoModifier,Qt.LeftButton,QGLViewer.FRAME,QGLViewer.TRANSLATE)
                self.setMouseBinding(Qt.NoModifier,Qt.RightButton,QGLViewer.FRAME,QGLViewer.NO_MOUSE_ACTION)
                self.setMouseBinding(Qt.ControlModifier,Qt.LeftButton,QGLViewer.CAMERA,QGLViewer.TRANSLATE)
                self.setMouseBinding(Qt.ControlModifier,Qt.RightButton,QGLViewer.CAMERA,QGLViewer.NO_MOUSE_ACTION)
        else:
                self.setMouseBinding(Qt.ControlModifier,Qt.LeftButton,QGLViewer.FRAME,QGLViewer.TRANSLATE)
                self.setMouseBinding(Qt.ControlModifier,Qt.RightButton,QGLViewer.FRAME,QGLViewer.NO_MOUSE_ACTION)
                self.setMouseBinding(Qt.NoModifier,Qt.LeftButton,QGLViewer.CAMERA,QGLViewer.TRANSLATE)
                self.setMouseBinding(Qt.NoModifier,Qt.RightButton,QGLViewer.CAMERA,QGLViewer.NO_MOUSE_ACTION)

    def mousePressEvent(self,event):
        if event.modifiers()  != Qt.ControlModifier:
            self.select(event.pos())
            self.selection = self.selectedName()
            self.lastselection = self.selection
            if self.selection != -1:
                p = self.curveAccessor.getPoint(self.selection)
                self.manipulator.setPosition(Vec(p[0],p[1],0))
                self.setManipulatedFrame(self.manipulator)
                self.createControlPointsRep()
                self.setInteractionMode(True)
                self.updateGL()
                self.selectionChanged.emit()
                QGLViewer.mousePressEvent(self,event)
            else:
                QGLViewer.mousePressEvent(self,event)
                self.selectionChanged.emit()
        else:
            return QGLViewer.mousePressEvent(self,event)

    def mouseDoubleClickEvent(self,event):
        if event.modifiers() == Qt.NoModifier:
            if event.button()  == Qt.LeftButton:
                self.select(event.pos())
                self.selection = self.selectedName()
                if self.selection == -1:
                    npoint = self.pointOnEditionPlane(event.pos())
                    res = self.pointsConstraints.addPointEvent(npoint,self.curveAccessor)
                    if res:
                        index,npoint = res
                        self.curveAccessor.insertPoint(index,npoint)
                        self.lastselection = index
                        self.nbCtrlPointsChanged.emit(self.curveAccessor.nbPoints())
                        self.valueChanged.emit()
                self.selectionChanged.emit()
            elif event.button()  == Qt.RightButton:
                self.select(event.pos())
                self.selection = self.selectedName()
                if self.selection != -1 :
                    self.curveAccessor.delPoint(self.selection)
                    self.nbCtrlPointsChanged.emit(self.curveAccessor.nbPoints())
                    self.valueChanged.emit()
                    self.selection = -1
                    self.lastselection = -1
                self.selectionChanged.emit()
            self.createControlPointsRep()
            self.updateGL()
        elif event.modifiers() == Qt.ShiftModifier:
            self.select(event.pos())
            self.selection = self.selectedName()
            if self.selection != -1 :
                self.curveAccessor.delPoint(self.selection)
                self.nbCtrlPointsChanged.emit(self.curveAccessor.nbPoints())
                self.valueChanged.emit()
                self.createControlPointsRep()
                self.updateGL()
                self.selection = -1
                self.lastselection = -1
            self.selectionChanged.emit()
        else:
            QGLViewer.mouseDoubleClickEvent(self,event)

    def mouseReleaseEvent(self,event):
        if self.selection != -1:
            self.selection = -1
            self.setInteractionMode(False)
        self.createControlPointsRep()
        self.updateSceneDimension()
        self.updateGL()
        self.selectionChanged.emit()
        QGLViewer.mouseReleaseEvent(self,event)

    def changeEvent(self,event):
        if event.type() == QEvent.EnabledChange:
            if self.isEnabled():
                self.pointColor = Material((250,30,30),1)
            else:
                self.pointColor = Material((30,30,30),1)
            self.createControlPointsRep()
        QGLViewer.changeEvent(self,event)

    def updatePoints(self):
        sel = self.selection
        if sel != -1:
            p = self.manipulator.getPosition()
            p = (p[0],p[1])
            p = self.pointsConstraints.movePointEvent(p,self.selection,self.curveAccessor)
            self.curveAccessor.setPoint(self.selection,p)
            if self.closedCurve :
                lastpoint = self.curveAccessor.nbPoints()-1
                if self.selection == 0 :
                    self.curveAccessor.sewPoints(0,lastpoint)
                elif self.selection == lastpoint :
                    self.curveAccessor.sewPoints(lastpoint,0)
            self.createControlPointsRep()
            self.update()
            self.valueChanged.emit()

    def createControlPointsRep(self):
        idpoints = list(reversed(list(enumerate(self.curveAccessor.points()))))
        self.ctrlpts = Scene([Shape(Translated(Vector3(p[0],p[1],0),self.sphere),self.pointColor,id=i) for i,p in idpoints])
        nbpoints = len(idpoints)
        if self.selection != -1:
            self.ctrlpts[nbpoints-self.selection-1].appearance = self.selectedPointColor
        if self.selection != 0:
            self.ctrlpts[nbpoints-1].appearance = self.firstPointColor

    def setStride(self, value):
        self.curveshape.geometry.stride = value
        self.valueChanged.emit() 
        self.update()

class Curve2DEditor(QtWidgets.QWidget):
    valueChanged = Signal()

    BLACK_THEME = {'Curve' : (255,255,255), 'BackGround' : (51,51,51), 'Text' : (255,255,255), 'CtrlCurve' : (122,122,0), 'GridStrong' : (102,102,102), 'GridFade' : (51,51,51) , 'Points' : (250,30,30), 'FirstPoint' : (250,30,250), 'SelectedPoint' : (30,250,30), 'DisabledBackGround' : (150,150,150) }
    WHITE_THEME = {'Curve' : (255,0,0), 'BackGround' : (255,255,255), 'Text' : (0,0,0), 'CtrlCurve' : (25,0,25), 'GridStrong' : (102,102,102), 'GridFade' : (153,153,153) , 'Points' : (30,250,30), 'FirstPoint' : (250,30,250), 'SelectedPoint' : (30,250,30), 'DisabledBackGround' : (150,150,150)}

    def __init__(self, parent,constraints=Curve2DConstraint()):
        QtWidgets.QWidget.__init__(self, parent)
        self.gridLayout = QtWidgets.QGridLayout(self)
        self.gridLayout.setContentsMargins(2, 0, 11, 2)

        self.view = Curve2DEditorView(self,constraints)
        self.view.valueChanged.connect(self.propagate_valuechanged)
        self.view.nbCtrlPointsChanged.connect(self.adaptStrideSelection)

        self.gridLayout.addWidget(self.view, 0, 0, 2, 2)
        self.view.selectionChanged.connect(self.selectionEvent)

        self.weightSpinBox = QtWidgets.QDoubleSpinBox(self)
        self.weightSpinBox.setMinimum(-100)
        self.weightSpinBox.setMaximum(100)
        self.weightSpinBox.valueChanged.connect(self.setWeigthToSelection)
        self.weightSpinBox.setEnabled(False)
        self.gridLayout.addWidget(self.weightSpinBox, 0, 2, 1, 1)

        self.weigthSlider = QtWidgets.QSlider(self)
        self.weigthSlider.setMinimum(-200)
        self.weigthSlider.setMaximum(200)
        self.weigthSlider.setOrientation(QtCore.Qt.Vertical)
        self.weigthSlider.setValue(0)
        self.weigthSlider.sliderPressed.connect(self.weigthSliderPressed)
        self.weigthSlider.sliderReleased.connect(self.weigthSliderReleased)
        self.weigthSlider.sliderMoved.connect(self.setWeigthRatioToSelection)
        self.weigthSlider.setEnabled(False)
        self.gridLayout.addWidget(self.weigthSlider, 1, 2, 1, 1)

        self.discrlabel = QtWidgets.QLabel(self)
        self.discrlabel.setText( "Discretization")
        self.gridLayout.addWidget(self.discrlabel, 2, 0, 1, 1)
        self.sliders = []

        self.strideSlider = QtWidgets.QSlider(self)
        self.strideSlider.setMinimum(2)
        self.strideSlider.setMaximum(50)
        self.strideSlider.setOrientation(QtCore.Qt.Horizontal)
        self.strideSlider.setValue(self.view.getCurve().stride)
        self.strideSlider.valueChanged.connect(self.view.setStride)
        self.gridLayout.addWidget(self.strideSlider, 2, 1, 1, 1)
        spacerItem = QtWidgets.QSpacerItem(20, 20, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem, 2, 2, 1, 1)


    def propagate_valuechanged(self):
        self.valueChanged.emit()

    def selectionEvent(self):
        active = self.view.hasLastSelection() 
        self.weightSpinBox.setEnabled(active)
        self.weigthSlider.setEnabled(active)
        if active:
            self.weigthSlider.setValue(0)
            self.weightSpinBox.setValue(self.view.curveAccessor.getPointWeight(self.view.lastselection))


    def weigthSliderPressed(self):
        if self.view.hasLastSelection():
            self.cweight = self.view.curveAccessor.getPointWeight(self.view.lastselection)
        else:
            self.cweight = 1

    def weigthSliderReleased(self):
        self.weigthSlider.setValue(0)

    def setWeigthRatioToSelection(self, value):
        ratio = 1+(value / 100.)
        self.weightSpinBox.setValue(self.cweight*ratio)
        self.setWeigthToSelection(self.cweight*ratio)

    def setWeigthToSelection(self, value):
        self.view.curveAccessor.setPointWeight(self.view.lastselection,value)
        self.view.update()

    def setCurve(self, nurbsObject, closed = False):
        self.view.setCurve(nurbsObject, closed)
        if self.view.curveAccessor.hasWeights():
            self.weightSpinBox.show()
            self.weigthSlider.show()
            self.selectionEvent()
        else:
            self.weightSpinBox.hide()
            self.weigthSlider.hide()
        if self.view.curveAccessor.hasStride():
            self.strideSlider.setValue(self.view.getCurve().stride)
            self.adaptStrideSelection()
            self.discrlabel.show()
            self.strideSlider.show()
        else:
            self.discrlabel.hide()
            self.strideSlider.hide()

    def adaptStrideSelection(self, nbPoints = None):
        if self.view.curveAccessor.hasStride():
            strideValue = self.view.getCurve().stride
            if nbPoints is None:
                nbPoints = self.view.curveAccessor.nbPoints()
                newmax = max(50,strideValue,nbPoints*10)
            else:
                ratio = float(strideValue) / self.strideSlider.maximum()
                newmax = max(50,strideValue,nbPoints*10)
                strideValue = int(ratio * newmax)
            self.strideSlider.setMaximum(newmax)
            self.strideSlider.setValue(strideValue)

    def setStride(self, value):
        self.view.setStride(value)
        self.strideSlider.setValue(value)

    def getCurve(self):
        return self.view.getCurve()

    def newDefaultCurve(self):
        self.setCurve(self.view.newDefaultCurve())

    def closeCurveEnabled(self):
        return self.view.closeCurveEnabled()

    def toggleCurveClosing(self):
        self.view.toggleCurveClosing()

    def enableCurveClosing(self, enabled):
        self.view.enableCurveClosing(enabled)

    def openImage(self):
        self.view.openImage()

    def closeImage(self):
        self.view.closeImage()

    def setBackgroundImage(self, imagefilename):
        self.view.setBackgroundImage(imagefilename)

    def applyTheme(self,theme = BLACK_THEME):
        self.view.applyTheme(theme)


def main():
    qapp = QApplication([])
    mv = Curve2DEditor(None) #,FuncConstraint())
    mv.setEnabled(True)
    mv.setCurve(Polyline2D([(0,0),(1,1)]))
    mv.show()
    qapp.exec_()

if __name__ == '__main__':
    main()