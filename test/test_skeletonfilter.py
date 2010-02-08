from openalea.plantgl.all import *
from random import *


def resample(self,nb = 100):
    fk = self.firstKnot
    nbm1 = nb - 1 
    deltau = self.lastKnot - fk
    self.pointList = [self.getPointAt(fk + deltau*i/nbm1) for i in range(nb)]

def test_triangulation(visual = False):
    # error with 3rd point as 0.5,0.5 (infinite point for cgal). use 0.4999,0.5 instead
    p = Polyline2D([(0,0),(1,0),(0.5,0.5),(1,1),(0,2),(-1,1.5),(-0.9,1.4),(-0.85,1.3),(-0.8,1.2),(-0.7,1.3),(0,1.3),(0,0)])
    tr = Skeleton.getDelaunayConstrained2DTriangulation(p)
    print tr.pointList,tr.indexList
    if visual:
      Viewer.display(p)
      Viewer.add(tr)

def test_filter_one_point_branch(visual = False):
    if not Overlay.supportCGAL():
        print 'Not compiled with CGAL. Test not performed'
        return

    p = Polyline2D([(1,1),(0,2),(-1,1.5),(-0.9,1.4),(-0.85,1.3),(-0.8,1.2),(-0.7,1.3),(0,1.3)])
    skel = Skeleton.getChordalAxisTransform(p,0.0005)
    if visual:
      Viewer.display(p)
      Viewer.add(Scene([Shape(i,Material((randint(0,255),randint(0,255),randint(0,255)))) for i in skel]))
    assert skel[len(skel)-1].isValid()
    assert len(skel[len(skel)-1].pointList) > 1

def test_filter_branch(visual = False):
    p = Polyline2D([(0,0),(0.5,0.3),(1,0),(0.7,0.5),(1,1),(0,2),(-1,2),(0,1.75),(-1,1.5),(0,1.3),(0.3,0.5),(0,0)])
    skel = Skeleton.getChordalAxisTransform(p,0.1)
    if visual:
      Viewer.display(p)
      Viewer.add(Scene([Shape(i,Material((randint(0,255),randint(0,255),randint(0,255)))) for i in skel]))
    print len(skel)
    print skel

def test_filter_information(visual = False):
    p = Polyline2D([(0,0),(0.5,0.3),(1,0),(0.7,0.5),(1,1),(0,2),(-1,2),(0,1.75),(-1,1.5),(0,1.3),(0.3,0.5),(0,0)])
    resample(p,50)
    res = Skeleton.getSkeletonInformation(p,0.5)
    if visual:
      Viewer.display(p)
      Viewer.add(Scene([Shape(i,Material((randint(0,255),randint(0,255),randint(0,255)))) for i in res[0]]))
      Viewer.add(Scene([Shape(Translated(Vector3(i),Sphere(0.02)),Material((randint(0,255),randint(0,255),randint(0,255)))) for i in res[1]]))
    print len(res[0])    

def test_filter(visual = False):
    p = Polyline2D([(0,0),(0.5,0.3),(1,0),(0.7,0.5),(1,1),(0,2),(-1,2),(0,1.75),(-1,1.5),(0,1.3),(0.3,0.5),(0,0)])
    resample(p,50)
    skel = Skeleton.getChordalAxisTransform(p,0.0)
    if visual:
      Viewer.display(p)
      Viewer.add(Scene([Shape(i,Material((randint(0,255),randint(0,255),randint(0,255)))) for i in skel]))
    print len(skel)
    print skel

    
if __name__ == '__main__':
    #test_triangulation()
    test_filter_one_point_branch()
    #test_filter_information()
