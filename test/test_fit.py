from openalea.plantgl.all import *
from random import uniform, randint


pointrange = (0,10)
def random_point2(pointrange = pointrange) : return Vector2(uniform(*pointrange),uniform(*pointrange))

if not pgl_support_extension('CGAL'):
    import warnings
    warnings.warn("Not supported CGAL extension. Skip overlay tests.")
else:
  def test_fit_circle(verbose = False):
    nbpoint = 2
    p2list = Point2Array([random_point2() for i in xrange(nbpoint)])
    res = Fit.boundingCircle(p2list)
    assert not res is None
    center,radius = res
    if verbose:
        print res
        Viewer.display(Scene([Translated(Vector3(center,0),Polyline2D.Circle(radius,128)),Shape(PointSet2D(p2list),Material((250,0,0)))]))
    for p in p2list:
        if  norm(p-center) > (radius+1e-5):
            print p,center,norm(p-center),radius
            assert False, "Bounding circle do not encapsulates all points"
        

  if __name__ == '__main__':
    test_fit_circle(True)
