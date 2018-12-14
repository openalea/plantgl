from openalea.plantgl.all import *
from openalea.vpltk.qt import qt
from PyQGLViewer import QGLViewer, Vec
from openalea.plantgl.gui.pglnqgl import *


class PglViewer (QGLViewer):
    def __init__(self, parent = None):
        QGLViewer.__init__(self,  parent)
        self.scene = None
        self.discretizer = Discretizer()
        self.glrenderer = GLRenderer(self.discretizer)
        self.bboxcomputer = BBoxComputer(self.discretizer)
        self.animationMode = eStatic
        self.forceclear = True
        self.camera().setViewDirection(Vec(-1,0,0))
        self.camera().setUpVector(Vec(0,0,1))

    def setScene(self, scene = None):
        self.scene = scene
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

    def draw(self):        
        if self.scene and self.glrenderer.beginSceneList():
            self.glrenderer.beginProcess()
            self.scene.apply(self.glrenderer)
            self.glrenderer.endProcess()
            self.glrenderer.endSceneList()

    def setAnimation(self,flag):
        self.animationMode = flag
        modemap = { eAnimatedPrimitives : GLRenderer.Dynamic , eAnimatedScene : GLRenderer.DynamicScene, eStatic : GLRenderer.Normal }
        self.glrenderer.renderingMode = modemap[flag]

    def showMessage(self,txt,timeout=0):
        self.displayMessage(txt,timeout)


if __name__ == '__main__':
    qApp = qt.QtGui.QApplication([])
    viewer = PglViewer()
    viewer.display(Scene([Sphere()]))
    viewer.show()
    qApp.exec_()