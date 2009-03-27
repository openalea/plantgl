from openalea.plantgl.all import *
from random import uniform, randint

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
    vcoord = (randint(0,20),randint(0,20),randint(0,20))
    vid = p3grid.getVoxelIdFromCoord(vcoord)
    print vid, vcoord, p3grid.getVoxelCoordFromId(vid)
    assert tuple(p3grid.getVoxelCoordFromId(vid)) == vcoord
    assert p3grid.getVoxelCenter(vcoord) == p3grid.getVoxelCenterFromId(vid)

def test_pointgrid_access():
    p3list = [(1,1,1),(6,6,6),(10,10,10),(20,20,20)]
    r = 0
    p3grid = Point3Grid(r+1,p3list)
    assert [p3grid.query_ball_point(i,r) for i in p3list] == [[i] for i in xrange(len(p3list))]
    
#test_point3grid()
test_pointgrid_access()