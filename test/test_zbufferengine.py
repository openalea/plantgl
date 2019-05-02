from openalea.plantgl.all import *
import openalea.plantgl.all as pgl
import  matplotlib.pyplot as plt



def test_teapot():
    s = Scene('data/teapot.obj')
    t = Tesselator()
    s.apply(t)
    tr = t.result
    cam = (1000,0,0)
    z = ZBufferEngine(800,800)
    z.setPerspectiveCamera(60,1,0.1,1000)
    z.lookAt(cam,(0,0,0),(0,0,1))
    print(z.getBoundingBoxView())
    print('render')
    z.process(tr, Material((0,0,200)),2)
    print('getImage')
    i = z.getImage()

    plt.imshow(i.to_array())
    plt.show()

def test_sphere():
    s = Scene([Sphere(0.5,32,32)])
    t = Tesselator()
    s.apply(t)
    tr = t.result
    cam = (5,0,0)
    z = ZBufferEngine(800,600)
    z.setPerspectiveCamera(60,4/3.,0.1,1000)
    z.lookAt(cam,(0,0,0),(0,0,1))
    print(z.getBoundingBoxView())
    print('render')
    z.process(tr, Material((100,50,200)),2)
    print('getImage')
    i = z.getImage()

    plt.imshow(i.to_interlaced_array())
    plt.show()

def test_cylinder():
    s = Scene([Cylinder(0.5,32)])
    t = Tesselator()
    s.apply(t)
    tr = t.result
    cam = (50,0,1)
    z = ZBufferEngine(800,600)
    z.setPerspectiveCamera(60,1,1,1000)
    z.process(cam,(0,0,-5),(0,0,1))
    print(z.getBoundingBoxView())
    print('render')
    z.render(tr, Material((100,50,200)),2)
    print('getImage')
    i = z.getImage()

    plt.imshow(i.to_array())
    plt.show()

def test_point():
    a = (0.135299,0.135299, -0.461940)
    z = ZBufferEngine(800,800)
    z.setPerspectiveCamera(60,1,1,1000)
    cam = (300,0,0)
    z.lookAt(cam,(0,0,0),(0,0,1))
    print(z.getBoundingBoxView())
    if hasattr(z, 'getWorldToCameraMatrix'):
        print(z.getWorldToCameraMatrix())
    else:
        # CGAL was missing during the build of the libraries.
        # We abort this test
        return
    print()
    print('World : \t', Vector3(a))
    b = z.worldToCamera(a)    
    print('Camera : \t', b)
    c = z.cameraToNDC(b)
    print('NDC :   \t', c)
    d = z.ndcToRaster(c)    
    print('Raster : \t', d)
    e = z.cameraToRaster(b)
    print('Raster : \t', e)

def test_tri():
    z = ZBufferEngine(800,800)
    z.setPerspectiveCamera(60,1,1,1000)
    cam = (300,0,0)
    z.lookAt(cam,(0,0,0),(0,0,1))
    if hasattr(z, 'getWorldToCameraMatrix'):
        print(z.getWorldToCameraMatrix())
    else:
        # CGAL was missing during the build of the libraries.
        # We abort this test
        return
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    tr = TriangleSet(pts, [list(range(3))])
    for p in pts:
        print(z.worldToCamera(p))
    for p in pts:
        print(z.worldToRaster(p))

    print('render')
    z.process(tr, Material((0,0,200)),2)
    print('getImage')
    i = z.getImage()

    plt.imshow(i.to_array())
    plt.show()


if __name__ == '__main__':
    test_teapot()