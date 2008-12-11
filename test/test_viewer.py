from openalea.plantgl.all import *
from math import *
import os

from PyQt4.QtCore import *
from PyQt4.QtGui import *

import time

class MyThread(QThread):
    def __init__(self):
        QThread.__init__(self)
        app = QApplication([])

    def test_start(self):
        Viewer.start()
  
    def test_display(self):
        Viewer.display(Sphere())
        Viewer.display(Shape(Sphere(),Material()))
        Viewer.display(Scene([Shape(Sphere(),Material())]))
        time.sleep(1)
  
    def test_add(self):
        Viewer.display(Box())
        Viewer.add(Sphere())
        assert len(Viewer.getCurrentScene()) == 2, "Size of displayed scene of the viewer is not valid. Viewer.add failed."
        Viewer.add(Shape(Sphere(),Material()))
        assert len(Viewer.getCurrentScene()) == 3, "Size of displayed scene of the viewer is not valid. Viewer.add failed."
        Viewer.add(Scene([Shape(Sphere(),Material())]))
        assert len(Viewer.getCurrentScene()) == 4, "Size of displayed scene of the viewer is not valid. Viewer.add failed."
        time.sleep(1)
  
    def test_update(self):
        s = Scene([Shape(Sphere(),Material())])
        Viewer.display(s)
        s[0].geometry = Box()
        Viewer.update()
  
    def test_selection(self):
        Viewer.display(Scene([Shape(Sphere(),Material(),1),Shape(Box(),Material(),2)]))
        assert len(Viewer.selection) == 0, "Invalid Viewer.selection"
        Viewer.selection = [1]
        assert len(Viewer.selection) == 1 and Viewer.selection[0] == 1, "Invalid Viewer.selection"
  
    def test_scene_interaction(self):
        s = Scene()
        s += Sphere()
        Viewer.display(s)
        s2 = Viewer.getCurrentScene()
        assert len(s) == len(s2), "Scene of the viewer is not equal to displayed scene"
  
    def test_camera_lookat(self):
        Viewer.display(Sphere())
        pos = Vector3(0,5,5)
        target = Vector3(0,0,1)
        Viewer.camera.lookAt(pos,target)
        resulting_pos = Viewer.camera.position
        assert norm(resulting_pos - pos) < 1e-2, "Viewer.camera.lookAt do not set the good position"
        resulting_pos, direction, up= Viewer.camera.getPosition()
        assert norm(direction^(target-pos)) < 1e-2, "Viewer.camera.lookAt do not set the good orientation"    
        assert norm((direction^up) + Vector3.OX) < 1e-2, "Viewer.camera.lookAt do not set the good orientation"
        time.sleep(1)
  
    def test_camera_set(self):
        Viewer.display(Sphere())
        pos = Vector3(5,5,5)
        az = 45
        el = 0 # 45
        Viewer.camera.set(pos,az,el)
        resulting_pos = Viewer.camera.position
        assert norm(resulting_pos - pos) < 1e-2, "Viewer.camera.set do not set the good position"
        resulting_pos, direction, up= Viewer.camera.getPosition()
        assert norm(direction^Vector3(5,5,0)) < 1e-2, "Viewer.camera.set do not set the good orientation"    
        assert norm(up^Vector3.OZ) < 1e-2, "Viewer.camera.set do not set the good orientation"
        az = 0
        el = -45
        Viewer.camera.set(pos,az,el)
        resulting_pos = Viewer.camera.position
        assert norm(resulting_pos - pos) < 1e-2, "Viewer.camera.set do not set the good position"
        resulting_pos, direction, up= Viewer.camera.getPosition()
        assert norm(direction^Vector3(5,0,5)) < 1e-2, "Viewer.camera.set do not set the good orientation"    
        assert norm((direction^up)^Vector3.OY) < 1e-2, "Viewer.camera.set do not set the good orientation"
        az = 45
        el = -45
        Viewer.camera.set(pos,az,el)
        resulting_pos = Viewer.camera.position
        assert norm(resulting_pos - pos) < 1e-2, "Viewer.camera.set do not set the good position"
        resulting_pos, direction, up= Viewer.camera.getPosition()
        assert norm(direction^Vector3(-0.5,-0.5,-1/sqrt(2))) < 1e-2, "Viewer.camera.set do not set the good orientation"    
        assert norm((direction^up)^Vector3(-1,1,0)) < 1e-2, "Viewer.camera.set do not set the good orientation"
        time.sleep(1)
  
    def test_camera_light(self):
        col = Color3(200,200,200)
        oldambient = Viewer.light.ambient
        Viewer.light.ambient = col
        assert Viewer.light.ambient == col, "Viewer.light.ambient do not set the good value"
        Viewer.light.ambient = oldambient
        olddiffuse = Viewer.light.diffuse
        Viewer.light.diffuse = col
        assert Viewer.light.diffuse == col, "Viewer.light.diffuse do not set the good value"
        Viewer.light.diffuse = olddiffuse
        oldspecular = Viewer.light.specular
        Viewer.light.specular = col
        assert Viewer.light.specular == col, "Viewer.light.specular do not set the good value"
        Viewer.light.specular = oldspecular
        Viewer.light.enabled = False
        assert Viewer.light.enabled == False, "Viewer.light.enabled do not set the good value"
        Viewer.light.enabled = True
        assert Viewer.light.enabled == True, "Viewer.light.enabled do not set the good value"
        pos = (10,10,10)
        Viewer.position = pos
        assert Viewer.position == pos, "Viewer.light.position do not set the good value"
        time.sleep(1)
  
    def test_image(self):
        w = 300
        h = 408
        Viewer.frameGL.maximize()
        Viewer.frameGL.setSize(w,w)
        fname = 'test_framegl.png'
        Viewer.frameGL.saveImage(fname,'PNG')
        Viewer.frameGL.maximize(False)
        assert os.path.exists(fname), "Viewer.frameGL.saveImage failed"
        try:
        #if True:
            from PIL import Image
            q = Image.open(fname)
            rw, rh = q.size[0], q.size[1]
            imgsizetest = (rw == w and rh == h)
            del q
        except:
            imgsizetest = True
        assert imgsizetest and "Viewer.frameGL.setSize failed %s", str((rw,rh))
        os.remove(fname)
        time.sleep(1)

    def test_state(self):
        import warnings, time    
        Viewer.start()
        assert Viewer.isRunning() == True, "Viewer not running. Viewer.start() failed."
        Viewer.stop()
        if Viewer.isRunning():      
            deb = time.time()
            t = 0
            while Viewer.isRunning() and t < 10:
                t = time.time() - deb
            if t >= 10:
                raise Exception("Viewer shutdown failed")
            else:
                warnings.warn("Viewer.stop requires %f to totally shutdown." % t)
        time.sleep(1)
        

if __name__=='__main__':
    try:
        import avoid_display
        print 'Avoid display.'
    except:
        q = MyThread()
        q.test_start()
        q.test_display()
        q.test_add()
        q.test_update()
        q.test_selection()
        q.test_scene_interaction()
        q.test_camera_lookat()
        q.test_camera_set()
        q.test_camera_light()
        q.test_image()
        q.test_state()
