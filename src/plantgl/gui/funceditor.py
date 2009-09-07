from PyQGLViewer import *
from openalea.plantgl.scenegraph import *
from openalea.plantgl.algo import *
from openalea.plantgl.math import *
from OpenGL.GL import *
from PyQt4.Qt import *
from math import pow,log

class FuncEditor (QGLViewer):
    def __init__(self,parent):
        QGLViewer.__init__(self,parent)
        nbP = 4
        self.selection = -1
        self.points = [(float(i)/(nbP-1),0) for i in xrange(nbP)]
        self.defaultMaterial = Material()
        self.defaultMaterial = Material()
        self.curve = Shape(NurbsCurve2D(Point3Array(self.points,1),width = 2),self.defaultMaterial)
        self.createControlPointsRep()
        self.discretizer = Discretizer()
        self.renderer = GLRenderer(self.discretizer)
        self.renderer.renderingMode = GLRenderer.Dynamic
        self.ctrlrenderer = GLCtrlPointRenderer(self.discretizer)
        self.setRange(0,1)
    def setRange(self,xmin,xmax):
        self.xbounds = (xmin,xmax)
    def init(self):
        b = BoundingBox(self.ctrlpts)
        self.setSceneBoundingBox(Vec(*b.lowerLeftCorner),Vec(*b.upperRightCorner))
        self.setHandlerKeyboardModifiers(QGLViewer.CAMERA, Qt.AltModifier)
        self.setHandlerKeyboardModifiers(QGLViewer.FRAME,  Qt.NoModifier)
        self.setHandlerKeyboardModifiers(QGLViewer.CAMERA, Qt.ControlModifier)
        self.setMouseBinding(Qt.LeftButton,QGLViewer.FRAME,QGLViewer.TRANSLATE)
        self.setMouseBinding(Qt.RightButton,QGLViewer.FRAME,QGLViewer.NO_MOUSE_ACTION)
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
        QObject.connect(self.manipulator,SIGNAL('manipulated( )'),self.updatePoints)
    def draw(self):
        self.curve.apply(self.renderer)
        glColor4f(0.5,0.5,0.0,0.0)
        self.curve.apply(self.ctrlrenderer)
        self.ctrlpts.apply(self.renderer)
        self.drawGrid()
    def getCurve(self):
        return self.curve.geometry
    def setCurve(self,curve):
        assert type(curve) == NurbsCurve2D
        self.curve.geometry = curve
    def drawGrid(self):
        start = self.pointOnEditionPlane(QPoint(0,self.height()-1))
        end = self.pointOnEditionPlane(QPoint(self.width()-1,0))
        xr = end[0] - start[0]
        xy = end[1] - start[1]
        nbdigit = max(int(round(log(xr,10))),int(round(log(xy,10))))
        xdelta = pow(10,nbdigit)/10
        fxval = round(start[0]/xdelta)
        lxval = round(end[0]/xdelta)
        cxval = fxval*xdelta
        nbiter = int((lxval-fxval))
        glColor4f(0.2,0.2,0.2,0.0)
        glLineWidth(1)
        glBegin(GL_LINES)
        for i in xrange(nbiter+1):
            glVertex3f(cxval,start[1],0)
            glVertex3f(cxval,end[1],0)
            cxval += xdelta
        glEnd()
        glLineWidth(2)
        glColor4f(0.4,0.4,0.4,0.0)
        cxval = round(start[0]/(10*xdelta))*(10*xdelta)
        glBegin(GL_LINES)
        for i in xrange((nbiter/10)+1):
            glVertex3f(cxval,start[1],0)
            glVertex3f(cxval,end[1],0)
            cxval += (10*xdelta)
        glEnd()
        fyval = round(start[1]/xdelta)
        lyval = round(end[1]/xdelta)
        cyval = fyval*xdelta
        nbiter = int((lyval-fyval))
        glColor4f(0.2,0.2,0.2,0.0)
        glLineWidth(1)
        glBegin(GL_LINES)
        for i in xrange(nbiter+1):
            glVertex3f(start[0],cyval,0)
            glVertex3f(end[0],cyval,0)
            cyval += xdelta
        glEnd()
        glLineWidth(2)
        glColor4f(0.4,0.4,0.4,0.0)
        cyval = round(start[1]/(10*xdelta))*(10*xdelta)
        glBegin(GL_LINES)
        for i in xrange((nbiter/10)+1):
            glVertex3f(start[0],cyval,0)
            glVertex3f(end[0],cyval,0)
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
        #self.renderer.renderingMode = GLRenderer.Dynamic
    def mousePressEvent(self,event):
        if event.modifiers()  != Qt.ControlModifier:
            self.select(event.pos())
            self.selection = self.selectedName()
            if self.selection != -1:
                p = self.points[self.selection]
                self.manipulator.setPosition(Vec(p[0],p[1],0))
                self.setManipulatedFrame(self.manipulator)
                self.createControlPointsRep()
                self.updateGL()
            QGLViewer.mousePressEvent(self,event)
        else:
            return QGLViewer.mousePressEvent(self,event)
    def pointOnEditionPlane(self,pos):
        orig,direction = self.camera().convertClickToLine(pos)
        npoint = orig+direction*(orig[2]/(-direction[2]))
        return (npoint[0],npoint[1])        
    def mouseDoubleClickEvent(self,event):
        if event.modifiers() == Qt.NoModifier:
            if event.button()  == Qt.LeftButton:
                npoint = self.pointOnEditionPlane(event.pos())
                if self.xbounds[0] < npoint[0] < self.xbounds[1]:
                    for i,p in enumerate(self.points):
                        if p[0] > npoint[0]:
                            break
                    self.points.insert(i,npoint)
            elif event.button()  == Qt.RightButton:
                self.select(event.pos())
                selection = self.selectedName()
                if selection != -1 and len(self.points) > 4:
                    del self.points[selection]
            self.createControlPointsRep()
            self.createCurveRep()
            self.updateGL()
        else:
            QGLViewer.mouseDoubleClickEvent(self,event)
    def mouseReleaseEvent(self,event):
        self.updatePoints()
        self.selection = -1
        self.createControlPointsRep()
        self.updateGL()
        QGLViewer.mouseReleaseEvent(self,event)
    def updatePoints(self):
        sel = self.selection
        if sel != -1:
            p = self.manipulator.getPosition()
            prev = self.points[sel]
            p = (p[0],p[1])
            minbound, maxbound = self.xbounds
            if self.selection > 0:
                minbound = self.points[sel-1][0]
            else :
                p = (0,p[1])
            if self.selection < len(self.points)-1:
                maxbound = self.points[sel+1][0]
            else:
                p = (1,p[1])
            if minbound > p[0] :
                p = (minbound,p[1])
            elif p[0] > maxbound:
                p = (maxbound,p[1])                
            self.points[self.selection] = p
            self.createCurveRep()
            self.createControlPointsRep()
            self.updateGL()
    def createCurveRep(self):
        self.curve.geometry = NurbsCurve2D(Point3Array(self.points,1),width=2)
    def createControlPointsRep(self):
        sp = Sphere(radius=0.02)
        m = Material((150,30,30),1)
        self.ctrlpts = Scene([Shape(Translated(Vector3(p,0),sp),m,id=i) for i,p in enumerate(self.points)])
        m2 = Material((30,150,30),1)
        if self.selection != -1:
            self.ctrlpts[self.selection].appearance = m2
        

if __name__ == '__main__':
    qapp = QApplication([])
    mv = FuncEditor(None)
    mv.show()
    qapp.exec_()