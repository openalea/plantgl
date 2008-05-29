from pgl_import_all import *

Viewer.start()
s = Scene([Shape(Sphere(),Material(),1),Shape(Translated(Vector3(0,0,50),Box(Vector3(0.3,0.3,0.3)*10)),Material(),2)])
Viewer.display(s)
Viewer.frameGL.setSize(600,600)
Viewer.camera.lookAt(Vector3(55,0,0),Vector3(0,0,0))
buf = Viewer.frameGL.castRays2(s)