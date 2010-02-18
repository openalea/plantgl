from PyQGLViewer import *
from openalea.plantgl.scenegraph import *
from openalea.plantgl.algo import *
from openalea.plantgl.math import *
from OpenGL.GL import *
from PyQt4.Qt import *
from math import pow,log


class CurveConstraint:
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
    def defaultCurve(self,nbP=4):
        return NurbsCurve2D(Point3Array([(-0.5+float(i)/(nbP-1),0) for i in xrange(nbP)],1) )

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
    def defaultCurve(self,nbP=4):
        return NurbsCurve2D(Point3Array([(float(i)/(nbP-1),0) for i in xrange(nbP)],1) )
        
class CurveAccessor:
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
    def delPoint(self,index):
        pass
    def findClosest(self,p):
        pass
    def bounds(self):
        pass

class BezierAccessor (CurveAccessor):
    def __init__(self,curve):
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
        self.curve.ctrlPointList[index] = Vector3(npoint,1)
    def getPoint(self,index):
        a = self.curve.ctrlPointList[index]
        return (a[0],a[1])
    def delPoint(self,index):
        nbp = self.nbPoints()
        if  nbp > self.minpointnb() and not index in [0,nbp-1]:
            del self.curve.ctrlPointList[index]
            self.setKnotList()
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

class NurbsAccessor (BezierAccessor):
    def __init__(self,curve):
        BezierAccessor.__init__(self,curve)
    def checkType(self,curve):
        assert type(curve) == NurbsCurve2D
    def minpointnb(self):
        return self.curve.degree +1
    def setKnotList(self):
        self.curve.setKnotListToDefault()
        
class PolylineAccessor (CurveAccessor):
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

        
class CurveEditor (QGLViewer):
    def __init__(self,parent,constraints=CurveConstraint()):
        QGLViewer.__init__(self,parent)
        self.selection = -1
        self.defaultMaterial = Material((255,255,255),1)
        self.sphere = Sphere(radius=0.02)
        self.curveshape = Shape()
        self.curveshape.appearance = self.defaultMaterial
        self.pointsConstraints = constraints
        self.accessorType = { NurbsCurve2D : NurbsAccessor, BezierCurve2D : BezierAccessor, Polyline2D : PolylineAccessor }
        self.setCurve(self.newDefaultCurve())
        self.discretizer = Discretizer()
        self.renderer = GLRenderer(self.discretizer)
        self.renderer.renderingMode = GLRenderer.Dynamic
        self.ctrlrenderer = GLCtrlPointRenderer(self.discretizer)
        self.setStateFileName('.curveeditor.xml')
    def newDefaultCurve(self):
        return self.pointsConstraints.defaultCurve()
    def init(self):
        self.updateSceneDimension()
        self.setHandlerKeyboardModifiers(QGLViewer.CAMERA, Qt.AltModifier)
        self.setHandlerKeyboardModifiers(QGLViewer.FRAME,  Qt.NoModifier)
        self.setHandlerKeyboardModifiers(QGLViewer.CAMERA, Qt.ControlModifier)
        self.setMouseBinding(Qt.LeftButton,QGLViewer.FRAME,QGLViewer.TRANSLATE)
        self.setMouseBinding(Qt.RightButton,QGLViewer.FRAME,QGLViewer.NO_MOUSE_ACTION)
        self.setMouseBinding(Qt.ControlModifier + Qt.LeftButton,QGLViewer.CAMERA,QGLViewer.TRANSLATE)
        self.setMouseBinding(Qt.ControlModifier + Qt.RightButton,QGLViewer.CAMERA,QGLViewer.NO_MOUSE_ACTION)
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
        QObject.connect(self.manipulator,SIGNAL('manipulated()'),self.updatePoints)
    def getCurve(self):
        """ Get the edited curve """
        return self.curveshape.geometry
    def updateSceneDimension(self):
        minp,maxp = self.curveAccessor.bounds()
        minp,maxp = Vector3(minp,0),Vector3(maxp,0)
        self.setSceneBoundingBox(Vec(*minp),Vec(*maxp))
    def setCurve(self,curve):
        """ Set the edited curve """
        self.curveshape.geometry = curve
        curve.width=2
        self.curveAccessor = self.accessorType[type(curve)](curve)
        self.pointsConstraints.checkInitialCurve(self.curveAccessor)
        self.createControlPointsRep()
        self.updateSceneDimension()
        self.showEntireScene()
    def draw(self):
        self.start = self.pointOnEditionPlane(QPoint(0,self.height()-1))
        self.end = self.pointOnEditionPlane(QPoint(self.width()-1,0))
        self.sphere.radius = (self.end[0]-self.start[0])/80
        self.discretizer.clear()
        self.curveshape.apply(self.renderer)
        glColor4f(0.5,0.5,0.0,0.0)
        self.curveshape.apply(self.ctrlrenderer)
        self.ctrlpts.apply(self.renderer)
        self.drawGrid()
    def drawGrid(self):
        xr = self.end[0] - self.start[0]
        xy = self.end[1] - self.start[1]
        nbdigit = max(int(round(log(xr,10))),int(round(log(xy,10))))
        xdelta = pow(10,nbdigit)/10
        fxval = round(self.start[0]/xdelta)
        lxval = round(self.end[0]/xdelta)
        cxval = fxval*xdelta
        nbiter = int((lxval-fxval))
        glColor4f(0.2,0.2,0.2,0.0)
        glLineWidth(1)
        glBegin(GL_LINES)
        for i in xrange(nbiter+1):
            glVertex3f(cxval,self.start[1],0)
            glVertex3f(cxval,self.end[1],0)
            cxval += xdelta
        glEnd()
        cxval = fxval*xdelta
        glColor4f(1.0,1.0,1.0,1.0)
        for i in xrange(nbiter+1):
            self.renderText(cxval,self.start[1],0,'%.1f' % cxval)
            cxval += xdelta
        glLineWidth(2)
        glColor4f(0.4,0.4,0.4,0.0)
        cxval = round(self.start[0]/(10*xdelta))*(10*xdelta)
        glBegin(GL_LINES)
        for i in xrange((nbiter/10)+1):
            glVertex3f(cxval,self.start[1],0)
            glVertex3f(cxval,self.end[1],0)
            cxval += (10*xdelta)
        glEnd()
        fyval = round(self.start[1]/xdelta)
        lyval = round(self.end[1]/xdelta)
        firstcyval = fyval*xdelta
        cyval = firstcyval
        nbiter = int((lyval-fyval))
        glColor4f(0.2,0.2,0.2,0.0)
        glLineWidth(1)
        glBegin(GL_LINES)
        for i in xrange(nbiter+1):
            glVertex3f(self.start[0],cyval,0)
            glVertex3f(self.end[0],cyval,0)
            cyval += xdelta
        glEnd()
        cyval = firstcyval
        glColor4f(1.0,1.0,1.0,1.0)
        for i in xrange(nbiter+1):
            glVertex3f(self.end[0],cyval,0)
            self.renderText(self.start[0],cyval,0,'%.1f' % cyval)
            cyval += xdelta
        glLineWidth(2)
        glColor4f(0.4,0.4,0.4,0.0)
        cyval = round(self.start[1]/(10*xdelta))*(10*xdelta)
        glBegin(GL_LINES)
        for i in xrange((nbiter/10)+1):
            glVertex3f(self.start[0],cyval,0)
            glVertex3f(self.end[0],cyval,0)
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
    def mousePressEvent(self,event):
        if event.modifiers()  != Qt.ControlModifier:
            self.select(event.pos())
            self.selection = self.selectedName()
            if self.selection != -1:
                p = self.curveAccessor.getPoint(self.selection)
                self.manipulator.setPosition(Vec(p[0],p[1],0))
                self.setManipulatedFrame(self.manipulator)
                self.createControlPointsRep()
                self.updateGL()
            QGLViewer.mousePressEvent(self,event)
        else:
            return QGLViewer.mousePressEvent(self,event)
    def mouseDoubleClickEvent(self,event):
        if event.modifiers() == Qt.NoModifier:
            if event.button()  == Qt.LeftButton:
                self.select(event.pos())
                selection = self.selectedName()
                if selection == -1:
                    npoint = self.pointOnEditionPlane(event.pos())
                    res = self.pointsConstraints.addPointEvent(npoint,self.curveAccessor)
                    if res:
                        index,npoint = res
                        self.curveAccessor.insertPoint(index,npoint)
                        self.emit(SIGNAL('valueChanged()'))
            elif event.button()  == Qt.RightButton:
                self.select(event.pos())
                selection = self.selectedName()
                if selection != -1 :
                    self.curveAccessor.delPoint(selection)
                    self.emit(SIGNAL('valueChanged()'))
            self.createControlPointsRep()
            self.updateGL()
        elif event.modifiers() == Qt.ShiftModifier:
            self.select(event.pos())
            selection = self.selectedName()
            if selection != -1 :
                self.curveAccessor.delPoint(selection)
                self.emit(SIGNAL('valueChanged()'))
                self.createControlPointsRep()
                self.updateGL()
        else:
            QGLViewer.mouseDoubleClickEvent(self,event)
    def mouseReleaseEvent(self,event):
        self.selection = -1
        self.createControlPointsRep()
        self.updateSceneDimension()
        self.updateGL()
        QGLViewer.mouseReleaseEvent(self,event)
    def updatePoints(self):
        sel = self.selection
        if sel != -1:
            p = self.manipulator.getPosition()
            p = (p[0],p[1])
            p = self.pointsConstraints.movePointEvent(p,self.selection,self.curveAccessor)
            self.curveAccessor.setPoint(self.selection,p)
            self.createControlPointsRep()
            self.updateGL()
            self.emit(SIGNAL('valueChanged()'))
    def createControlPointsRep(self):
        m = Material((250,30,30),1)
        self.ctrlpts = Scene([Shape(Translated(Vector3(p[0],p[1],0),self.sphere),m,id=i) for i,p in enumerate(self.curveAccessor.points())])
        m2 = Material((30,250,30),1)
        if self.selection != -1:
            self.ctrlpts[self.selection].appearance = m2
        

if __name__ == '__main__':
    qapp = QApplication([])
    mv = CurveEditor(None,FuncConstraint())
    #mv.setCurve(Polyline2D([(0,0),(1,1)]))
    mv.show()
    qapp.exec_()