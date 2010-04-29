from openalea.plantgl.all import *

def test_triangleset():
    """ Regression test for bug 10213 """
    pts = [(i,j,0) for i in range(3) for j in range(3)]
    ind = [(0,1,3), (3,4,6)]

    ts = TriangleSet(pts, ind)
    assert ts.isValid()

    ts.computeNormalList()

    assert ts.isValid()


