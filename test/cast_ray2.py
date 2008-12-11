"""
Test code on castRays2()
"""
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from openalea.plantgl.all import Scene, Viewer, Shape, Sphere, Material, Translated, Vector3, Box
import code


class MyThread(QThread):
    def __init__(self):
        QThread.__init__(self)
    
    def run(self):
        code.interact("Sub thread interpretation",local=globals())
        QApplication.quit()

    def doit(self):
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
        app.exec_()

if __name__ == '__main__':
	m = MyThread()
	m.doit()

