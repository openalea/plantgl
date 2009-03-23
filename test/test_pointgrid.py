from openalea.plantgl.all import *
from random import uniform

pointrange = (0,10)

def random_point() : return Vector3(uniform(*pointrange),uniform(*pointrange),uniform(*pointrange))

def vid(id,dim):
    return id[0]*dim[1]*dim[2]+id[1]*dim[2]+id[2]

#print vid((1,2,3),(5,5,5))

def test_point3grid():
    nbpoint = 6000
    p3list = [random_point() for i in xrange(nbpoint)]
    p3grid = Point3Grid((0.5,0.5,0.5),p3list)
    center = Vector3(5,5,5)
    radius = 2
    pball = p3grid.query_ball_point(center,radius)
    print pball
    assert len(pball) == len(set(pball))
    for i,p in enumerate(p3list):
        if i in pball:
            assert(norm(p-center) <= radius)
        else :
            assert(norm(p-center) > radius)

test_point3grid()