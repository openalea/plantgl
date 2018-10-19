from openalea.plantgl.all import *
import openalea.plantgl.all as pgl
import  matplotlib.pyplot as plt


normal = lambda tr: cross(Vector3(tr[1])-tr[0],Vector3(tr[2])-tr[0])

def checkorientation(tr1, tr2):
    return dot(normal(tr1), normal(tr2)) > 0   


def process_tris(pts, pts2, visual = False):
    e = DepthSortEngine()
    e.setPerspectiveCamera(60,1,1,1000)
    cam = (-2,0,0)
    e.lookAt(cam,(0,0,0),(0,0,1))
    if visual : 
        tr = TriangleSet(pts, [range(3)])
        Viewer.display(Shape(tr, Material((0,200,0))))
        tr2 = TriangleSet(pts2, [range(3)])
        Viewer.add(Shape(tr2, Material((0,0,200))))

    e.processTriangle(*(pts+[100]))
    e.processTriangle(*(pts2+[200]))
    s = e.getResult(eABGR)
    for sh in s:
        print sh.geometry.pointList
    if visual : Viewer.add(s)

def test_tri1(visual = False):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[0.1,-0.5,0.0],[0.1,1,1],[0.1,1,0.5]]
    #print normal(pts), normal(pts2)
    #assert checkorientation(pts,pts2)
    process_tris(pts, pts2, visual)


def test_tri2(visual = False):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[0.1,0.1,0.0],[0.1,0.3,0.3],[0.1,0.3,0.1]]
    print normal(pts), normal(pts2)
    assert checkorientation(pts,pts2)
    process_tris(pts, pts2, visual)


def test_tri3(visual = False):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[-0.1,0.1,0.0],[-0.1,0.3,0.1],[-0.1,0.3,0.3]]
    process_tris(pts, pts2, visual)


def test_tri4(visual = False):
    pts = [[0,0,0.],[0,0,0.5],[0,1,0]]
    pts2 = [[-0.1,0.1,0.0],[-0.1,0.3,0.1],[0.1,0.3,0.3]]
    process_tris(pts, pts2, visual)



if __name__ == '__main__':
    import sys
    argv = list(sys.argv)    
    del argv[0]
    visual = '-v' in sys.argv
    if visual:
        del argv[argv.index('-v')]
    i = 1
    if len(argv) > 0:
        i = argv[0]
    globals()['test_tri'+str(i)](visual)