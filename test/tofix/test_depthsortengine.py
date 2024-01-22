from openalea.plantgl.all import *
import openalea.plantgl.all as pgl
import  matplotlib.pyplot as plt
from math import *

normal = lambda tr: cross(Vector3(tr[1])-tr[0],Vector3(tr[2])-tr[0])

def checkorientation(tr1, tr2):
    return dot(normal(tr1), normal(tr2)) > 0   


def process_tris_basic(pts, pts2, visual = False, translation = 2, model = None):
    cam = (-2,0,0)
    if visual : 
        Viewer.display(Shape(Translated(cam,Sphere(0.1)), Material((0,0,0))))

        tr = TriangleSet(pts, [list(range(3))])
        Viewer.add(Shape(Translated(0,0,translation,tr), Material((0,200,0))))
        tr2 = TriangleSet(pts2, [list(range(3))])
        Viewer.add(Shape(Translated(0,0,translation,tr2), Material((0,0,200))))
    
    e = DepthSortEngine()
    e.setPerspectiveCamera(60,1,1,1000)
    e.lookAt(cam,(0,0,0),(0,0,1))
    e.processTriangle(*(pts+[100]))
    e.processTriangle(*(pts2+[20000]))
    s = e.getResult(eABGR, False)
    for sh in s:
        print(sh.geometry.pointList)
    if visual : Viewer.add(s)

def process_triset_basic(listpts, visual = False, translation = 2, model = None):
    cam = (-2,0,0)
    if visual : 
        Viewer.display(Shape(Translated(cam,Sphere(0.1)), Material((0,0,0))))

        for pts2 in listpts:
            tr2 = TriangleSet(pts2, [range(3)])
            Viewer.add(Shape(Translated(0,0,translation,tr2), Material((0,0,200))))
    
    e = DepthSortEngine()
    e.setPerspectiveCamera(60,1,1,1000)
    e.lookAt(cam,(0,0,0),(0,0,1))
    for i, pts2 in enumerate(listpts):
        e.processTriangle(*(pts2+[100*i]))
    s = e.getResult(eABGR, False)
    for sh in s:
        print (sh.geometry.pointList)
    if visual : Viewer.add(s)


def process_tris_full(pts, pts2, visual = False, translation = 2, model = None):
    if visual : 
        tr = TriangleSet(pts, [list(range(3))])
        Viewer.display(Shape(tr, Material((0,200,0))))
        tr2 = TriangleSet(pts2, [list(range(3))])
        Viewer.add(Shape(tr2, Material((0,0,200))))

    for mid, alpha in enumerate(range(0,360, 30)):
      if model is None or model == mid:
        dir = Vector3(Vector3.Spherical(1,radians(alpha),pi/2))
        cam = 5*dir
        e = DepthSortEngine()
        e.setPerspectiveCamera(60,1,1,1000)
        e.lookAt(cam,(0,0,0),(0,0,1))
        cammatrix = e.camera().getWorldToCameraMatrix().inverse()
        print(cam)
        if visual : 
            vi = cammatrix.getColumn(0)
            assert vi.w == 0
            vi = Vector3(vi.x,vi.y,vi.z)
            vj = cammatrix.getColumn(1)
            assert vj.w == 0
            vj = Vector3(vj.x,vj.y,vj.z)
            vk = cammatrix.getColumn(2)
            assert vk.w == 0
            vk = Vector3(vk.x,vk.y,vk.z)

            Viewer.add(Shape(Translated(cam*2,Sphere(0.1)), Material((0,0,0))))
            Viewer.add(Shape(Translated(cam*2,Oriented(vi,vj, Cylinder(0.05,0.5))), Material((200,0,0))))
            Viewer.add(Shape(Translated(cam*2,Oriented(vk,vi, Cylinder(0.05,0.5))), Material((0,0,200))))
            Viewer.add(Shape(Translated(cam*2,Oriented(vj,vk, Cylinder(0.05,0.5))), Material((0,200,0))))

        e.processTriangle(*(pts+[100]))
        e.processTriangle(*(pts2+[20000]))
        s = e.getProjectionResult(eABGR)
        for sh in s:
            print([tuple(v) for v in sh.geometry.pointList])
        if visual : 
            Viewer.add(Scene([Shape(Translated(cam,Oriented(vi,vj,sh.geometry)), sh.appearance, sh.id) for sh in s]))

process_tris = process_tris_full

def process_scene_basic(sc, visual = False, translation = 1):
    if not isinstance(sc, Scene):
        sc = Scene([Shape(sc,id=100)])

    t = Tesselator() 
    sc.apply(t)
    for tr in t.result.indexList:
        print ([tuple(t.result.pointList[tid]) for tid in tr])

    cam = (-2,0,0)
    if visual : 
        Viewer.display(Shape(Translated(cam,Sphere(0.1)), Material((0,0,0))))
        Viewer.add(Scene([Shape(Translated(0,0,translation,sh.geometry), sh.appearance, sh.id) for sh in sc]) )

    e = DepthSortEngine()
    e.setPerspectiveCamera(60,1,1,1000)
    e.lookAt(cam,(0,0,0),(0,0,1))
    e.process(sc)
    s = e.getResult(eABGR, False)
    for sh in s:
        print(sh.geometry.pointList)
    if visual : Viewer.add(s)

    cam2 = (2,0,0)
    if visual : 
        Viewer.add(Shape(Translated(0,translation,0,Translated(cam2,Sphere(0.1))), Material((0,0,0))))
    e = DepthSortEngine()
    e.setPerspectiveCamera(60,1,1,1000)
    e.lookAt(cam2,(0,0,0),(0,0,1))
    e.process(sc)
    s = e.getResult(eABGR, False)
    for sh in s:
        print(sh.geometry.pointList)
    if visual : Viewer.add(Scene([Shape(Translated(0,translation,0,sh.geometry), sh.appearance, sh.id) for sh in s]))

def process_scene_full(sc, visual = False, nbView = 1):
    if not isinstance(sc, Scene):
        sc = Scene([Shape(sc,id=100)])

    if visual : 
        Viewer.display(sc)

    for alpha in range(0,360, int(360/nbView)):
        dir = Vector3(Vector3.Spherical(1,radians(alpha),pi/2))
        cam = 5*dir
        e = DepthSortEngine()
        e.setPerspectiveCamera(60,1,1,1000)
        e.lookAt(cam,(0,0,0),(0,0,1))
        cammatrix = e.camera().getWorldToCameraMatrix().inverse()
        print(cam)
        if visual : 
            vi = cammatrix.getColumn(0)
            assert vi.w == 0
            vi = Vector3(vi.x,vi.y,vi.z)
            vj = cammatrix.getColumn(1)
            assert vj.w == 0
            vj = Vector3(vj.x,vj.y,vj.z)
            vk = cammatrix.getColumn(2)
            assert vk.w == 0
            vk = Vector3(vk.x,vk.y,vk.z)

            Viewer.add(Shape(Translated(cam*2,Sphere(0.1)), Material((0,0,0))))
            Viewer.add(Shape(Translated(cam*2,Oriented(vi,vj, Cylinder(0.05,0.5))), Material((200,0,0))))
            Viewer.add(Shape(Translated(cam*2,Oriented(vk,vi, Cylinder(0.05,0.5))), Material((0,0,200))))
            Viewer.add(Shape(Translated(cam*2,Oriented(vj,vk, Cylinder(0.05,0.5))), Material((0,200,0))))

        e.process(sc)

        s = e.getResult(eABGR, False)
        for sh in s:
            print(sh.geometry.pointList)

        if visual : Viewer.add(Scene([Shape(Translated(cam,sh.geometry), sh.appearance, sh.id) for sh in s]))


process_scene = process_scene_basic


def test_tri1(visual = False, model = None):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[0.1,-0.5,0.0],[0.1,1,1],[0.1,1,0.5]]
    #print normal(pts), normal(pts2)
    #assert checkorientation(pts,pts2)
    process_tris(pts, pts2, visual, model=model)


def test_tri2(visual = False, model = None):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[0.1,0.1,0.0],[0.1,0.3,0.3],[0.1,0.3,0.1]]
    print(normal(pts), normal(pts2))
    assert checkorientation(pts,pts2)
    process_tris(pts, pts2, visual, model=model)


def test_tri3(visual = False, model = None):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[-0.1,0.1,0.0],[-0.1,0.3,0.1],[-0.1,0.3,0.3]]
    process_tris(pts, pts2, visual, model=model)


def test_tri4(visual = False, model = None):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[-0.1,0.1,0.0],[-0.1,0.3,0.1],[0.1,0.3,0.3]]
    process_tris(pts, pts2, visual, model=model)

def test_tri5(visual = False, model = None):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[0.1,0.1,0.0],[0.1,0.3,0.1],[-0.1,0.3,0.3]]
    process_tris(pts, pts2, visual, model=model)

def test_tri6(visual = False, model = None):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[0,0,0.0],[0,0,0.5],[0,-1,0]]
    process_tris(pts, pts2, visual, model=model)

def test_tri7(visual = False, model = None):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[0,0,0.0],[0,0,0.5],[-1,0,0]]
    process_tris(pts, pts2, visual, model=model)

def test_triset(visual = False, model = None):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[0.1,1,0],[0.1,1,0.5],[0.1,0,0]]
    pts3 = [[0.2,1,0.5],[0.2,1,0],[0.2,0,0.5]]
    pts4 = [[0.3,0,0.5],[0.3,0,0],[0.3,1,0.5]]
    pts5 = [[-0.1,0,0.5],[-0.1,0,0],[-0.1,1,0.5]]
    pts5 = [[-0.2,0.5,0.5],[-0.2,0,0],[-0.2,1,0]]
    process_triset_basic([pts, pts2, pts3, pts4, pts5], visual, model=model)


def test_triset2(visual = False, model = None):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[0.2,1,0],[0.1,1,0.5],[0.3,0,0]]
    pts3 = [[0.1,1,0.5],[0.3,1,0],[0.2,0,0.5]]
    pts4 = [[0.3,0,0.5],[0.0,0,0],[-0.3,1,0.5]]
    pts5 = [[-0.1,0,0.5],[0.2,0,0],[-0.2,1,0.5]]
    pts5 = [[0,0.5,0.5],[0.3,0,0],[-0.2,1,0]]
    process_triset_basic([pts, pts2, pts3, pts4, pts5], visual, model=model)

def test_triset3(visual = False, model = None):
    pts = [[0,0,0.],    [0,0,0.5],[0,1,0]]
    pts2 = [[0.0,1,0],  [0.0,1,0.5],[0.0,0,0]]
    pts3 = [[0.0,1,0.5],[0.0,1,0],[0.0,0,0.5]]
    pts4 = [[0.0,0,0.5],[0.0,0,0],[0.0,1,0.5]]
    pts5 = [[0.0,0,0.5],[0.0,0,0],[0.0,1,0.5]]
    pts5 = [[0,0.5,0.5],[0.0,0,0],[0.0,1,0]]
    process_triset_basic([pts, pts2, pts3, pts4, pts5]*10, visual, model=model)

def test_triset4(visual = False, model = None):
    t = Tesselator() 
    Sphere().apply(t)
    triangles = [[tuple(t.result.pointList[tid]) for tid in tr]for tr in t.result.indexList]
    triangles = triangles[:30]
    process_triset_basic(triangles, visual, model=model)



def test_sphere(visual = False):
    process_scene(Sphere(), visual)

def test_box(visual = False):
    process_scene(Box(), visual)

def test_cylinder(visual = False):
    process_scene(Cylinder(), visual)

def test_cone(visual = False):
    process_scene(Cone(), visual, nbView=4)

def test_paraboloid(visual = False):
    process_scene(Paraboloid(), visual)


if __name__ == '__main__':
    import sys
    argv = list(sys.argv)    
    del argv[0]
    visual = '-v' in sys.argv
    print(visual)
    if visual:
        del argv[argv.index('-v')]
    i = 1
    if len(argv) > 0:
        i = argv[0]
        try:
            i = int(i)
        except:
            pass
    mid = None
    if len(argv) > 1 :
        mid = int(argv[1])
        print ('Model', mid)
    if type(i) == int:
        globals()['test_tri'+str(i)](visual, model = mid)
    else:
        globals()['test_'+str(i)](visual)
