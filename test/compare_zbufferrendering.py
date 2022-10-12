from openalea.plantgl.config import PGL_QT_VERSION
if PGL_QT_VERSION == 6:
    from PySide6.QtCore import *
    from PySide6.QtGui import *
    from PySide6.QtWidgets import *
elif PGL_QT_VERSION == 5:
    from PyQt5.QtCore import *
    from PyQt5.QtGui import *
    from PyQt5.QtWidgets import *
else:
    from PyQt4.QtCore import *
    from PyQt4.QtGui import *

from PyQGLViewer import *
import OpenGL.GL as ogl
from openalea.plantgl.all import *
from openalea.plantgl.gui.pglnqgl import *
from math import *
import numpy as np
from collections import OrderedDict

toCol3 = lambda v : Color3(int(v[0]*255),int(v[1]*255),int(v[2]*255))
toV3 = lambda v : Vector3(float(v[0]),float(v[1]),float(v[2]))

projections = OrderedDict([(ePerspective , Camera.PERSPECTIVE), (eOrthographic , Camera.ORTHOGRAPHIC),
                           (eHemispheric , Camera.PERSPECTIVE), (eCylindrical , Camera.ORTHOGRAPHIC)])

header = '''
#ifndef (__camera_definition__)
#declare __camera_definition__ = true;

camera {{
   {projection}
   angle {angle}
    location <{xpos},{ypos},{zpos}>
    right <{rightx},{righty},{rightz}>
    up <{upx},{upy},{upz}>
    direction <{directionx},{directiony},{directionz}>
}}

light_source {{
     <{lightposx},{lightposy},{lightposz}>
    color rgb 1
}}

light_source {{
     <{xpos},{ypos},{zpos}>
    color rgb 1
}}

background {{ color rgb <{bgcolorR},{bgcolorG},{bgcolorB}> }}

#end // __camera_definition__

'''
povray_exe = None

def read_povray_exe(fname = 'config.cfg'):
    from sys import platform
    import os
    if os.path.exists(fname):
        return open(fname,'r').read().strip()
    else:
        if platform == "linux" or platform == "linux2" or platform == 'darwin':
            return '/opt/local/bin/povray'
        elif platform == 'win32':
            return 'C:/Program Files/povray/v3.7/bin/pvengine.exe'
        else:
            return 'povray'

def get_povray_exe():
    import os.path
    global povray_exe
    if povray_exe is None:
        povray_exe = read_povray_exe()
    assert os.path.exists(povray_exe)
    return povray_exe

def generate_from_representation(scene, projection = 0, size = 400, angle = 180, campos = (0,0,0), direction = (-1,0,0), up = (0,0,1), right = (0,1,0), lightpos = (0,0,10), bgcolor = QColor(255,255,255), antialiasing = False, debug = False):
    from sys import platform
    import numpy as np
    import pandas as pd
    from math import degrees
    import os
    import shutil
    proj = OrderedDict([(ePerspective, 'perspective'), (eOrthographic, 'orthographic'), 
                                  (eHemispheric, 'fisheye') , (eCylindrical, 'cylinder 1')])

    povray_exe = get_povray_exe()
    if os.path.exists('povray'):
        shutil.rmtree('povray')
    os.makedirs('povray')
    os.chdir('povray')
    if debug:
        print(os.getcwd())
    try:
        tmpfile = 'hemispheric_view'
        if len(scene) > 0:
            scene.save('scene.pov')
        povstream = open(tmpfile+'.pov','w')
        povstream.write(header.format(projection = proj[projection], angle=angle,xpos=campos[0], ypos=campos[1], zpos=campos[2],
                                      directionx=direction[0], directiony=direction[1], directionz=direction[2],
                                      upx=up[0], upy=up[1],upz=up[2],
                                      rightx=right[0], righty=right[1], rightz=right[2],
                                      lightposx=lightpos[0], lightposy=lightpos[1], lightposz=lightpos[2],
                                      bgcolorR=bgcolor.redF(), bgcolorG=bgcolor.greenF(), bgcolorB=bgcolor.blueF()  ))
        if len(scene) > 0:
            povstream.write('#include "scene.pov"\n\n\n')
        povstream.close()
        cmd = povray_exe
        if platform == "win32":
            cmd +=" /EXIT /RENDER "
        else:
            cmd +=" -I"
        if projection == 2:
            size = (min(size),min(size))
        cmd += tmpfile+".pov -O"+tmpfile+".png +H"+str(size[1])+" +W"+str(size[0])+" +FN -GA "
        if antialiasing:
            cmd += '+A'
        else:
            cmd += '-A'            
        if (platform == "linux" or platform == "linux2" or platform == 'darwin') and debug == False:
            cmd += " &> rendering.log"
        from time import time
        t = time()
        if debug:
            print(cmd)
        os.system(cmd)
        if debug:
            print('Done in',time()-t, 'sec.')
        from PIL import Image
        img = np.asarray(Image.open(tmpfile+".png"))
    except Exception as e:
        os.chdir(os.pardir)
        raise e        
    os.chdir(os.pardir)
    #if not debug:
    #s    shutil.rmtree('povray')
    return img

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
        self.cameratype = ePerspective
        self.timer = QTimer()
        self.timer.setSingleShot(True)
        self.timer.timeout.connect(self.drawCPU)
        self.label = label
        self.timeout  = int(1000)
        self.notimer = False
        self.lightManip = True
        self.lighManipulator = ManipulatedFrame()
        self.setManipulatedFrame(self.lighManipulator)
        self.setAnimation(eAnimatedPrimitives)
        self.bufferDuplication = False
        self.renderingStyle = eColorBased
        self.renderingMultithreaded = False
        self.hemisphericangle = 180
        if hasattr(self,'updateGL'):
            self.update = self.updateGL

    def init(self):
        self.showEntireScene()
        self.drawCPU()
        self.setKeyDescription(Qt.Key_E, 'Toogle Camera Type (Perspective or Orthographic)')
        self.setKeyDescription(Qt.Key_L, 'Toogle Light Manipulation')
        self.setKeyDescription(Qt.Key_P, 'Periodize Buffer')
        self.setKeyDescription(Qt.Key_D, 'Render depths')
        self.setKeyDescription(Qt.Key_I, 'Render Ids')
        self.setKeyDescription(Qt.Key_M, 'Toogle Multithread')
        self.setKeyDescription(Qt.Key_F, 'Export to Povray')
        pos = ogl.glGetLightfv(ogl.GL_LIGHT0, ogl.GL_POSITION)
        self.lighManipulator.setPosition(pos[0], pos[1], pos[2])
        self.lighManipulator.manipulated.connect(self.setLightPosition)
        self.setBackgroundColor(QColor(255,255,255))
        #self.restoreStateFromFile()

    def setLightPosition(self):
        position = list(self.lighManipulator.position())+[0]
        print('Set Light Position to', position)
        ogl.glLightfv(ogl.GL_LIGHT0, ogl.GL_POSITION, position)


    def keyPressEvent(self, event):
        if event.key() == Qt.Key_E:
            self.toogleCameraType()
            self.update()
        elif event.key() == Qt.Key_L:
            self.toogleLightManipulation()
            self.update()
        elif event.key() == Qt.Key_P:
            self.bufferDuplication = not self.bufferDuplication
            self.update()
        elif event.key() == Qt.Key_D:
            if self.renderingStyle != eDepthOnly:
                self.renderingStyle = eDepthOnly
            else:
                self.renderingStyle = eColorBased                
            self.update()
        elif event.key() == Qt.Key_I:
            if self.renderingStyle != eIdBased:
                self.renderingStyle = eIdBased
            else:
                self.renderingStyle = eColorBased                
            self.update()
        elif event.key() == Qt.Key_F:
            self.exportToPov()
        elif event.key() == Qt.Key_M:
            self.renderingMultithreaded = not self.renderingMultithreaded
            print('Multithread',self.renderingMultithreaded)
            self.update()
        else:
            QGLViewer.keyPressEvent(self, event)

    def setScene(self, scene = None):
        self.bbx = BoundingBox(scene)
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
            else: print('error computing bbox')

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
        projs = list(projections.keys())
        self.cameratype = projs[(projs.index(self.cameratype) + 1) % len(projections)]
        camera.setType(projections[self.cameratype])

    def toogleLightManipulation(self):
        self.lightManip = not self.lightManip
        if self.lightManip:
            print('Set Light Manipulation')
            self.setManipulatedFrame(self.lighManipulator)
        else:
            print('Set Camera Manipulation')
            self.setManipulatedFrame(self.camera().frame())

    def import_light_config(self, z, lightId = ogl.GL_LIGHT0):
        try:
            #self.determine_light_config()
            z.setLight(
                toV3(ogl.glGetLightfv(lightId, ogl.GL_POSITION)),
                toCol3(ogl.glGetLightfv(lightId, ogl.GL_AMBIENT)),
                toCol3(ogl.glGetLightfv(lightId, ogl.GL_DIFFUSE)),
                toCol3(ogl.glGetLightfv(lightId, ogl.GL_SPECULAR)), True)
        except Exception as ie:
            print(ie)
            pass

    def determine_light_config(self):
        for i in range(ogl.GL_MAX_LIGHTS):
            lightId = ogl.__dict__.get('GL_LIGHT'+str(i))
            if lightId:
                print('Light', i, '(',lightId,') :', 'Enabled' if ogl.glIsEnabled(lightId) else 'Disabled')
                if ogl.glIsEnabled(lightId):
                    print('Position', ogl.glGetLightfv(lightId, ogl.GL_POSITION))
                    print('Ambient', ogl.glGetLightfv(lightId, ogl.GL_AMBIENT))
                    print('Diffuse', ogl.glGetLightfv(lightId, ogl.GL_DIFFUSE))
                    print('Specular', ogl.glGetLightfv(lightId, ogl.GL_SPECULAR))

    def drawCPU(self):
        print('drawCPU', self.width(),self.height())
        import time
        t = time.time()  
        camera = self.camera()
        #if self.renderingStyle == eIdBased:
        #    z = ZBufferEngine(self.width(),self.height(), toC4(self.backgroundColor()).toUint(eABGR), eABGR)
        #else:
        z = ZBufferEngine(self.width(),self.height(), self.renderingStyle, toC3(self.backgroundColor()))
        z.multithreaded = self.renderingMultithreaded
        znear, zfar = camera.zNear(), camera.zFar()
        #if self.camera().type() == Camera.PERSPECTIVE:
        if self.cameratype == ePerspective:
            z.setPerspectiveCamera(degrees(camera.fieldOfView()),self.width()/float(self.height()),znear,zfar)
        elif self.cameratype == eOrthographic:
            halfWidth, halfHeight = camera.getOrthoWidthHeight()
            z.setOrthographicCamera(-halfWidth, halfWidth, -halfHeight, halfHeight, znear,zfar)
        elif self.cameratype == eHemispheric:
            z.setHemisphericCamera(self.hemisphericangle)
        elif self.cameratype == eCylindrical:
            #halfHeight = znear * tan(camera.fieldOfView()/2.);
            halfWidth, halfHeight = camera.getOrthoWidthHeight()
            z.setCylindricalCamera(self.hemisphericangle, -halfHeight, halfHeight)
        z.lookAt(camera.position(),camera.position()+camera.viewDirection(),camera.upVector())
        self.import_light_config(z)
        #print z.getBoundingBoxView()
        #print camera.position(),camera.position()+camera.viewDirection(),camera.upVector()
        #if self.idBasedRendering:
        #    z.setIdRendering()
        #z.setLight((0,0,100),(255,255,255))
        print('process')
        z.process(self.scene)
        if self.bufferDuplication:
            z.periodizeBuffer(self.bbx.getCenter(), self.bbx.getCenter()+Vector3(self.bbx.getSize().x*2,0,0)) #, False)
            z.periodizeBuffer(self.bbx.getCenter(), self.bbx.getCenter()+Vector3(0,self.bbx.getSize().y*2,0))
            z.periodizeBuffer(self.bbx.getCenter(), self.bbx.getCenter()+Vector3(self.bbx.getSize().x*2,0,0))
            z.periodizeBuffer(self.bbx.getCenter(), self.bbx.getCenter()+Vector3(0,self.bbx.getSize().y*2,0))

        if self.renderingStyle == eDepthOnly:
            print('draw depth img')
            import qimage2ndarray as qn
            minz, maxz = znear, zfar
            db = z.getDepthBuffer()
            db.threshold_max_values(maxz)
            db.threshold_min_values(minz)
            img = qn.gray2qimage(db.to_array().T, (minz,maxz))
            self.label.setPixmap(QPixmap(img))
        elif self.renderingStyle == eColorBased:
            self.label.setPixmap(QPixmap(z.getImage().toQImage()))
        elif self.renderingStyle == eIdBased:
            self.label.setPixmap(QPixmap(z.getIdBufferAsImage(eABGR).toQImage()))
        dtime = time.time()  - t
        print('done in', dtime,'sec.')
        self.timeout = int(dtime * 1000)



    def setAnimation(self,flag):
        self.animationMode = flag
        modemap = { eAnimatedPrimitives : GLRenderer.Dynamic , eAnimatedScene : GLRenderer.DynamicScene, eStatic : GLRenderer.Normal }
        self.glrenderer.renderingMode = modemap[flag]

    def showMessage(self,txt,timeout=0):
        self.displayMessage(txt,timeout)

    def exportToPov(self):
        camera = self.camera()
        angle = self.hemisphericangle if self.cameratype in [eHemispheric, eCylindrical] else degrees(camera.horizontalFieldOfView())
        up=camera.upVector()
        right=camera.rightVector()
        if self.cameratype == eOrthographic:
            halfWidth, halfHeight = camera.getOrthoWidthHeight()
            up *= halfHeight
            right *= halfWidth
        elif self.cameratype == eCylindrical:
            halfWidth, halfHeight = camera.getOrthoWidthHeight()
            #up *= halfHeight
            #right *= halfWidth
        img = generate_from_representation(scene=self.scene, projection=self.cameratype,
                                           angle=angle,size=(self.label.width(),self.label.height()), campos=camera.position(), 
                                           direction=camera.viewDirection(),
                                           up=up,
                                           right=right,
                                           lightpos=ogl.glGetLightfv(ogl.GL_LIGHT0, ogl.GL_POSITION),
                                           bgcolor=self.backgroundColor())
        #from matplotlib.pyplot import imshow, show
        #imshow(img)
        #show()
        if not hasattr(self,'povlabel'):
            self.povlabel = QLabel(self.parent())
            self.povlabel.setMinimumSize(self.minimumSize())
            self.povlabel.setSizePolicy(QSizePolicy(QSizePolicy.MinimumExpanding, QSizePolicy.MinimumExpanding))
            #self.povlabel.setSizePolicy(QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed))
        import qimage2ndarray as qn
        qimage = qn.array2qimage(img)                                                                                                                                                                 
        self.povlabel.setPixmap(QPixmap(qimage))
        self.povlabel.show()


def main():
    qapp = QApplication([])
    hSplit  = QSplitter(Qt.Horizontal)
    
    #scene = Scene([Shape(Sphere(10),Material((200,50,100),2))])
    #scene = Scene([Shape(Cylinder(1,10),Material((100,25,50),4))])
    points = [(0,0,0),(0,20,0),(0,0,-10)]
    scene = Scene([Shape(TriangleSet(points, [range(3)], colorList=[(255,0,0,0),(0,255,0,0),(0,0,255,0)],colorPerVertex=True),id=3), Shape(PointSet(points, width=10),id=100)])
    #scene = Scene([Shape(Polyline([(0,0,0),(1,0,1)], width=3),Material((200,50,100),2))])
    #scene = Scene('data/cow.obj')
    #scene = Scene('../share/plantgl/database/advancedgraphics/tulipa.geom')
    #scene = Scene('../share/plantgl/database/advancedgraphics/mango.bgeom')
    #scene = Scene('../share/plantgl/database/examples/snowmanshape.geom')
    #scene = Scene('/Users/fboudon/Dropbox/mtpellier_training/project/benchmark_data/GDR_12_r1.txt')
    #scene[0].geometry.geometry.width=10
    h = int(600/2)
    w = int(800/2)


    # Instantiate the viewers.
    right  = PglViewer(hSplit)
    right.setMinimumSize(QSize(w,h))
    right.setSizePolicy(QSizePolicy(QSizePolicy.MinimumExpanding, QSizePolicy.MinimumExpanding))
    #right.setSizePolicy(QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed))
    right.setScene(scene)

    left   = MLabel(hSplit, right)
    left.setMinimumSize(QSize(w,h))
    left.setSizePolicy(QSizePolicy(QSizePolicy.MinimumExpanding, QSizePolicy.MinimumExpanding))
    #left.setSizePolicy(QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed))

    right.label = left
    
    hSplit.setWindowTitle("multiView")    
    hSplit.show()
    #right.show()
    qapp.exec_()

if __name__ == '__main__':
    main()
