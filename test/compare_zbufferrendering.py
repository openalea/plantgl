from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQGLViewer import *
import OpenGL.GL as ogl
from openalea.plantgl.all import *
from openalea.plantgl.gui.pglnqgl import *
from math import *
import numpy as np

toCol3 = lambda v : Color3(int(v[0]*255),int(v[1]*255),int(v[2]*255))
toV3 = lambda v : Vector3(float(v[0]),float(v[1]),float(v[2]))


class MLabel(QLabel):
    def __init__(self, parent, viewer = None):
        QLabel.__init__(self, parent)
        self.viewer = viewer

    def mousePressEvent(self, event):
        if self.viewer:
            self.viewer.notimer = True
            return self.viewer.mousePressEvent(event)
        else :
            return QLabel.mousePressEvent(self, event)

    def mouseReleaseEvent(self, event):
        if self.viewer:
            self.viewer.notimer = False
            return self.viewer.mouseReleaseEvent(event)
        else :
            return QLabel.mouseReleaseEvent(self, event)

    def mouseMoveEvent(self, event):
        if self.viewer:
            return self.viewer.mouseMoveEvent(event)
        else :
            return QLabel.mouseMoveEvent(self, event)

    def wheelEvent(self, event):
        if self.viewer:
            self.viewer.notimer = True
            res = self.viewer.wheelEvent(event)
            self.viewer.notimer = False
            return res
        else :
            return QLabel.wheelEvent(self, event)

class PglViewer (QGLViewer):
    def __init__(self, parent = None, label = None):
        QGLViewer.__init__(self,  parent)
        self.scene = None
        self.discretizer = Tesselator()
        self.glrenderer = GLRenderer(self.discretizer)
        self.bboxcomputer = BBoxComputer(self.discretizer)
        self.animationMode = eStatic
        self.forceclear = True
        self.camera().setViewDirection(Vec(-1,0,0))
        self.camera().setUpVector(Vec(0,0,1))
        self.timer = QTimer()
        self.timer.setSingleShot(True)
        self.timer.timeout.connect(self.drawCPU)
        self.label = label
        self.timeout  = 1000
        self.notimer = False
        self.lightManip = True
        self.lighManipulator = ManipulatedFrame()
        self.setManipulatedFrame(self.lighManipulator)
        self.setAnimation(eAnimatedPrimitives)


    def init(self):
        self.drawCPU()
        self.setKeyDescription(Qt.Key_E, 'Toogle Camera Type (Perspective or Orthographic)')
        self.setKeyDescription(Qt.Key_L, 'Toogle Light Manipulation')
        pos = ogl.glGetLightfv(ogl.GL_LIGHT0, ogl.GL_POSITION)
        self.lighManipulator.setPosition(pos[0], pos[1], pos[2])
        self.lighManipulator.manipulated.connect(self.setLightPosition)

    def setLightPosition(self):
        position = list(self.lighManipulator.position())+[0]
        print 'Set Light Position to', position
        ogl.glLightfv(ogl.GL_LIGHT0, ogl.GL_POSITION, position)

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_E:
            self.toogleCameraType()
        if event.key() == Qt.Key_L:
            self.toogleLightManipulation()
        else:
            QGLViewer.keyPressEvent(self, event)

    def setScene(self, scene = None):
        self.scene = scene
        radius = 1
        self.lightPoint = Point3Array([(0,0,0)])
        self.scene.add(Shape(PointSet(self.lightPoint, width=10), Material((255,255,0))))
        if self.animationMode != eAnimatedScene or self.forceclear:
            self.glrenderer.clear()
            self.discretizer.clear()
            self.bboxcomputer.clear()
        if not scene is None:
            self.bboxcomputer.process(self.scene)
            bbx = self.bboxcomputer.result                
            if bbx : 
                self.camera().setSceneBoundingBox(*bbx2qgl(bbx))
            else: print 'error computing bbox'

    def display(self,scene = None):
        self.setScene(scene)
        if self.animationMode == eStatic:
            self.showEntireScene()
        self.updateGL()

    def updateLight(self):
        pos = ogl.glGetLightfv(ogl.GL_LIGHT0, ogl.GL_POSITION)
        self.lightPoint[0] = toV3(pos)

    def draw(self): 
        self.updateLight()
        if self.scene and self.glrenderer.beginSceneList():
            self.glrenderer.beginProcess()
            self.scene.apply(self.glrenderer)
            self.glrenderer.endProcess()
            self.glrenderer.endSceneList()

        if self.notimer:
            self.drawCPU()
        else:
            self.timer.start(self.timeout)

    def fastDraw(self):
        self.updateLight()
        if self.scene and self.glrenderer.beginSceneList():
            self.glrenderer.beginProcess()
            self.scene.apply(self.glrenderer)
            self.glrenderer.endProcess()
            self.glrenderer.endSceneList()

        if self.notimer:
            self.drawCPU()

    def toogleCameraType(self):
        camera = self.camera()
        if camera.type() ==  Camera.ORTHOGRAPHIC:
            camera.setType(Camera.PERSPECTIVE)
        else:
            camera.setType(Camera.ORTHOGRAPHIC)

    def toogleLightManipulation(self):
        self.lightManip = not self.lightManip
        if self.lightManip:
            print 'Set Light Manipulation'
            self.setManipulatedFrame(self.lighManipulator)
        else:
            print 'Set Camera Manipulation'
            self.setManipulatedFrame(self.camera().frame())

    def import_light_config(self, z, lightId = ogl.GL_LIGHT0):
        try:
            #self.determine_light_config()
            z.setLight(
                toV3(ogl.glGetLightfv(lightId, ogl.GL_POSITION)),
                toCol3(ogl.glGetLightfv(lightId, ogl.GL_AMBIENT)),
                toCol3(ogl.glGetLightfv(lightId, ogl.GL_DIFFUSE)),
                toCol3(ogl.glGetLightfv(lightId, ogl.GL_SPECULAR)))
        except Exception, ie:
            print ie
            pass

    def determine_light_config(self):
        for i in range(ogl.GL_MAX_LIGHTS):
            lightId = ogl.__dict__.get('GL_LIGHT'+str(i))
            if lightId:
                print 'Light', i, '(',lightId,') :', 'Enabled' if ogl.glIsEnabled(lightId) else 'Disabled'
                if ogl.glIsEnabled(lightId):
                    print 'Position', ogl.glGetLightfv(lightId, ogl.GL_POSITION)
                    print 'Ambient', ogl.glGetLightfv(lightId, ogl.GL_AMBIENT)
                    print 'Diffuse', ogl.glGetLightfv(lightId, ogl.GL_DIFFUSE)
                    print 'Specular', ogl.glGetLightfv(lightId, ogl.GL_SPECULAR)

    def drawCPU(self):
        print 'drawCPU', self.width(),self.height()
        import time
        t = time.time()  
        camera = self.camera()
        z = ZBufferEngine(self.width(),self.height(), toC3(self.backgroundColor()))
        if self.camera().type() == Camera.PERSPECTIVE:
            z.setPerspectiveCamera(degrees(camera.fieldOfView()),self.width()/float(self.height()),camera.zNear(),camera.zFar())
        else:
            halfWidth, halfHeight = camera.getOrthoWidthHeight()
            z.setOrthographicCamera(-halfWidth, halfWidth, -halfHeight, halfHeight, camera.zNear(), camera.zFar())
        z.lookAt(camera.position(),camera.position()+camera.viewDirection(),camera.upVector())
        self.import_light_config(z)
        #z.setLight((0,0,100),(255,255,255))
        z.render(self.scene)
        # for sh in self.scene:
        #     if isinstance(sh.geometry,PointSet) :
        #         z.render(PointSet(sh.geometry.pointList, sh.geometry.colorList, sh.geometry.width),Material(sh.appearance),sh.id)
        #     elif isinstance(sh.geometry,Polyline):
        #         z.render(Polyline(sh.geometry),Material(sh.appearance),sh.id)
        #     else:
        #         sh.apply(self.discretizer)
        #         z.render(self.discretizer.result,Material(sh.appearance),sh.id)
        self.label.setPixmap(QPixmap(z.getImage().toQImage()))
        dtime = time.time()  - t
        print 'done in', dtime,'sec.'
        self.timeout = dtime * 1000



    def setAnimation(self,flag):
        self.animationMode = flag
        modemap = { eAnimatedPrimitives : GLRenderer.Dynamic , eAnimatedScene : GLRenderer.DynamicScene, eStatic : GLRenderer.Normal }
        self.glrenderer.renderingMode = modemap[flag]

    def showMessage(self,txt,timeout=0):
        self.displayMessage(txt,timeout)

def main():
    qapp = QApplication([])
    hSplit  = QSplitter(Qt.Vertical)
    
    #scene = Scene([Shape(Sphere(10),Material((200,50,100),2))])
    #scene = Scene([Shape(Cylinder(1,10),Material((100,25,50),4))])
    #scene = Scene([Shape(TriangleSet([(0,0,0),(0,20,0),(0,0,10)], [range(3)], colorList=[(255,0,0,0),(0,255,0,0),(0,0,255,0)],colorPerVertex=True))])
    scene = Scene('data/cow.obj')
    h = 600/2
    w = 800/2


    # Instantiate the viewers.
    right  = PglViewer(hSplit)
    right.setMinimumSize(QSize(w,h))
    right.setSizePolicy(QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed))
    right.setScene(scene)

    left   = MLabel(hSplit, right)
    left.setMinimumSize(QSize(w,h))
    left.setSizePolicy(QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed))

    right.label = left
    
    hSplit.setWindowTitle("multiView")    
    hSplit.show()
    #right.show()
    qapp.exec_()

if __name__ == '__main__':
    main()
