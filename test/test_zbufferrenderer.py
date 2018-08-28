from openalea.plantgl.all import *
import openalea.plantgl.all as pgl
import  matplotlib.pyplot as plt



def test_teapot():
    s = Scene('data/teapot.obj')
    t = Tesselator()
    s.apply(t)
    tr = t.result
    cam = (300,0,0)
    z = ZBufferRenderer(800,800)
    z.setPerspectiveCamera(60,1,1,1000)
    z.lookAt(cam,(0,0,0),(0,0,1))
    print z.getBoundingBoxView()
    print 'render'
    z.render(tr, Material((0,0,200)),2)
    print 'getImage'
    i = z.getImage()

    plt.imshow(i.to_array())
    plt.show()

def test_sphere():
    s = Scene([Sphere()])
    t = Tesselator()
    s.apply(t)
    tr = t.result
    cam = (300,0,0)
    z = ZBufferRenderer(800,800)
    z.setPerspectiveCamera(60,1,1,1000)
    z.lookAt(cam,(0,0,0),(0,0,1))
    print z.getBoundingBoxView()
    print 'render'
    z.render(tr, Material((0,0,200)),2)
    print 'getImage'
    i = z.getImage()

    plt.imshow(i.to_array())
    plt.show()

def test_point():
    a = (0.135299,0.135299, -0.461940)
    z = ZBufferRenderer(800,800)
    z.setPerspectiveCamera(60,1,1,1000)
    cam = (300,0,0)
    z.lookAt(cam,(0,0,0),(0,0,1))
    print z.getBoundingBoxView()
    print z.getWorldToCameraMatrix()
    print
    print 'World : \t', Vector3(a)
    b = z.worldToCamera(a)    
    print 'Camera : \t', b
    c = z.cameraToNDC(b)
    print 'NDC :   \t', c
    d = z.ndcToRaster(c)    
    print 'Raster : \t', d
    e = z.cameraToRaster(b)
    print 'Raster : \t', e

def test_tri():
    z = ZBufferRenderer(800,800)
    z.setPerspectiveCamera(60,1,1,1000)
    cam = (300,0,0)
    z.lookAt(cam,(0,0,0),(0,0,1))
    print z.getWorldToCameraMatrix()
    pts = [[0,0,0.],[0,0,0.5],[5,0,0]]
    tr = TriangleSet(pts, [range(3)])
    for p in pts:
        print z.worldToRaster(p)

    print 'render'
    z.render(tr, Material((0,0,200)),2)
    print 'getImage'
    i = z.getImage()

    #plt.imshow(i.to_array())
    #plt.show()


if __name__ == '__main__':
    test_tri()