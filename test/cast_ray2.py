"""
Test code on castRays2()
"""
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from openalea.plantgl.all import Scene, Viewer, Shape, Sphere, Material, Translated, Vector3, Box



def test_castRays2():
    m = MyThread()
    m.test_doit()
    return


class MyThread(QThread):
    def __init__(self):
        QThread.__init__(self)
    
    def test_doit(self):
        app = QApplication([])
        Viewer.start()
        s = Scene([ Shape(Sphere(),Material(),1),
                    Shape(  Translated(Vector3(0,0,50),
                            Box(Vector3(0.3,0.3,0.3)*10)),
                            Material((0,0,0)),2)])
        Viewer.display(s)
        Viewer.frameGL.setSize(600,600)
        Viewer.camera.lookAt(Vector3(55,0,0),Vector3(0,0,0))
        buf = Viewer.frameGL.castRays2(s)
        self.start()
        #app.exec_()


if __name__ == '__main__':
    test_castRays2()
