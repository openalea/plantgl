from openalea.plantgl.all import *
import openalea.plantgl.all as pgl
import  matplotlib.pyplot as plt
from os.path import join, dirname, abspath, exists
import numpy as np
from math import *

fname = abspath(join(dirname(__file__),'data/teapot.obj'))

MT = False

def test_teapot(view = False):
    s = Scene(fname)
    t = Tesselator()
    s.apply(t)
    tr = t.result
    cam = (200,0,0)
    z = ZBufferEngine(800,800, renderingStyle=eIdBased)
    z.setPerspectiveCamera(60,1,0.1,1000)
    z.lookAt(cam,(0,0,0),(0,0,1))
    z.multithreaded = MT
    print(z.getBoundingBoxView())
    print('render')
    z.process(tr, Material((0,0,200)),2)
    print('getImage')
    i = z.getImage()

    if view:
        plt.imshow(i.to_array())
        plt.show()

def test_sphere(view = False):
    s = Scene([Sphere(0.5,32,32)])
    t = Tesselator()
    s.apply(t)
    tr = t.result
    cam = (5,0,0)
    z = ZBufferEngine(800,600)
    z.setPerspectiveCamera(60,4/3.,0.1,1000)
    z.lookAt(cam,(0,0,0),(0,0,1))
    z.multithreaded = MT
    print(z.getBoundingBoxView())
    print('render')
    z.process(tr, Material((100,50,200)),2)
    print('getImage')
    i = z.getImage()

    if view:
        plt.imshow(i.to_interlaced_array())
        plt.show()

def test_cylinder(view = False):
    s = Scene([Cylinder(0.5,32)])
    t = Tesselator()
    s.apply(t)
    tr = t.result
    cam = (50,0,1)
    z = ZBufferEngine(800,600)
    z.setPerspectiveCamera(60,1,1,1000)
    z.multithreaded = MT
    z.lookAt(cam,(0,0,-5),(0,0,1))
    print(z.getBoundingBoxView())
    print('render')
    z.process(tr, Material((100,50,200)),2)
    print('getImage')
    i = z.getImage()

    if view:
        plt.imshow(i.to_array())
        plt.show()

def test_point():
    a = (0.135299,0.135299, -0.461940)
    z = ZBufferEngine(800,800)
    z.setPerspectiveCamera(60,1,1,1000)
    cam = (300,0,0)
    z.multithreaded = MT
    z.lookAt(cam,(0,0,0),(0,0,1))
    print(z.getBoundingBoxView())
    print(z.camera().getWorldToCameraMatrix())
    print()
    print('World : \t', Vector3(a))
    b = z.camera().worldToCamera(a)    
    print('Camera : \t', b)
    c = z.camera().cameraToNDC(b)
    print('NDC :   \t', c)
    d = z.camera().NDCToRaster(c, 800, 800)    
    print('Raster : \t', d)
    e = z.camera().cameraToRaster(b, 800, 800)
    print('Raster : \t', e)

def test_tri(view = False):
    z = ZBufferEngine(800,800)
    z.setPerspectiveCamera(60,1,1,1000)
    z.multithreaded = MT
    cam = (300,0,0)
    z.lookAt(cam,(0,0,0),(0,0,1))
    print(z.camera().getWorldToCameraMatrix())
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    tr = TriangleSet(pts, [list(range(3))])
    for p in pts:
        print(z.camera().worldToCamera(p))
    for p in pts:
        print(z.camera().worldToRaster(p, 800, 800))

    print('render')
    z.process(tr, Material((0,0,200)),2)
    print('getImage')
    i = z.getImage()

    if view:
        plt.imshow(i.to_array())
        plt.show()


def test_projected_sphere(view = False):
    s = Scene([Shape(ScreenProjected(Sphere(0.5,32,32)),Material((100,50,200)),2)])
    #s = Scene([Shape(Sphere(0.5,32,32),Material((100,50,200)),2)])
    cam = (5,0,0)
    z = ZBufferEngine(800,600)
    z.setPerspectiveCamera(60,4/3.,0.1,1000)
    #z.setOrthographicCamera(-1, 1, -1, 1, 0, 10)
    z.lookAt(cam,(0,0,0),(0,0,1))
    z.multithreaded = MT
    print(z.getBoundingBoxView())
    print('render')
    z.process(s)
    print('getImage')
    i = z.getImage()

    if view:
        plt.imshow(i.to_array())
        plt.show()

def test_hemispheric_point():
    a = (0,100, 100)
    z = ZBufferEngine(800,800)
    z.setHemisphericCamera(pi,1,1000)
    cam = (0,0,0)
    z.multithreaded = MT
    z.lookAt(cam,(0,0,100),(1,0,0))
    #print(z.getBoundingBoxView())
    print(z.camera().getWorldToCameraMatrix())
    print()
    print('World : \t', Vector3(a))
    b = z.camera().worldToCamera(a)    
    print('Camera : \t', b)
    c = z.camera().cameraToNDC(b)
    print('NDC :   \t', c)
    d = z.camera().NDCToRaster(c, 800, 800)    
    print('Raster : \t', d)
    e = z.camera().cameraToRaster(b, 800, 800)
    print('Raster : \t', e)
    f = z.camera().rasterToCamera(e, 800, 800)
    print('Camera : \t', f)


def test_formfactors():
    tr = Scene('data/cube.obj')[0].geometry
    t = Tesselator()
    tr.apply(t)
    tr = t.result
    print(tr.pointList,tr.indexList)
    result = formFactors(tr.pointList,tr.indexList,ccw=True,solidangle=True)
    print(result)

def test_solidangle(view = True):
    angle = 359
    rangle = radians(angle)
    cam = SphericalCamera(angle)
    d = 200
    w, h = d, d
    img = np.array([[cam.solidAngle(x,y,w,h) if cam.isValidPixel(x,y,w,h) else 0 for x in range(w)] for y in range(h)])



    #img = np.array([[component(cam.NDCToSpherical(cam.rasterToNDC((x+1,y+1,1),w,h))) for x in range(w)] for y in range(h)])
    #img = np.array([[sa(coord(x,y),coord2(x,y),coord3(x,y)) for x in range(w)] for y in range(h)])
    print(img[d//2,d//2])
    print(sum(sum(img)) )
    p = plt.imshow(img) #,vmin=0.2,vmax=-0.2)
    plt.colorbar(p)
    plt.show()

if __name__ == '__main__':
    test_solidangle()
    #test_formfactors()
    #test_hemispheric_point()
    #test_projected_sphere(True)