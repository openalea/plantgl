from openalea.plantgl.all import *
from random import uniform, randint

pointrange = (0,10)

def random_point() : return Vector3(uniform(*pointrange),uniform(*pointrange),uniform(*pointrange))

def vid(id,dim):
    return id[0]*dim[1]*dim[2]+id[1]*dim[2]+id[2]

ID = 1

#print vid((1,2,3),(5,5,5))
def test_point3grid_construct1():
    nbpoint = 100
    p3list = [random_point() for i in range(nbpoint)]
    p3grid = Point3Grid((0.5,0.5,0.5),p3list)

def test_point3grid_construct2():
    nbpoint = 100
    p3list = [random_point() for i in range(nbpoint)]
    p3grid = Point3Grid(0.5*ID,p3list)

def test_point3grid_construct3():
    nbpoint = 100
    p3list = [random_point() for i in range(nbpoint)]
    p3grid = Point3Grid((0.5,0.5,0.5),(0,0,0),(10,10,10),p3list)

def test_point3grid_ball():
    nbpoint = 6000
    p3list = [random_point() for i in range(nbpoint)]
    p3grid = Point3Grid((0.5,0.5,0.5),p3list)
    center = Vector3(5.5,5.5,5.5)
    radius = 2
    pball = p3grid.query_ball_point(center,radius)
    print(pball)
    assert len(pball) == len(set(pball))
    for i,p in enumerate(p3list):
        if i in pball:
            assert(norm(p-center) <= radius)
        else :
            assert(norm(p-center) > radius)
    vcoord = (randint(0,19),randint(0,19),randint(0,19))
    vid = p3grid.cellId(vcoord)
    print(vid, vcoord, p3grid.index(vid))
    assert p3grid.index(vid) == vcoord
    assert p3grid.getVoxelCenter(vcoord) == p3grid.getVoxelCenterFromId(vid)

def test_pointgrid_access():
    p3list = [(1,1,1),(6,6,6),(10,10,10),(20,20,20)]
    r = 0
    p3grid = Point3Grid((r+1)*ID,p3list)
    print([p3grid.query_ball_point(i,r) for i in p3list])
    print([[i] for i in range(len(p3list))])
    assert [p3grid.query_ball_point(i,r) for i in p3list] == [[i] for i in range(len(p3list))]
    
def test_pointgrid_corners(nbpoint = 10):
    p3list = [(0,0,0),(10,10,10)]+[random_point() for i in range(nbpoint)]
    p3grid = Point3Grid(1*ID,p3list)
    corners = p3grid.getCorners()
    #print corners
    assert len(corners) == 8

def manual_closest(pt,plist):
        d = 100
        res = None
        for i,p in enumerate(plist):
            ld = norm(pt-p)
            if ld < d:
                d = ld
                res = i
        return res
    
def test_pointgrid_closest(nbtest = 10, nbpoint = 10):
    p3list = [(0,0,0),(10,10,10)]+[random_point() for i in range(nbpoint)]
    p3grid = Point3Grid(2*ID,p3list)
    #print p3list
    #print p3grid.size(),p3grid.nbFilledVoxels()
    for i in range(nbtest):
        randompoint = random_point()
        mc = manual_closest(randompoint,p3list)
        ac = p3grid.closest_point(randompoint)
        #print mc , randompoint, p3list[mc],  norm(randompoint-p3list[mc])
        #print ac , randompoint, p3list[ac],  norm(randompoint-p3list[ac])
        assert ac == mc

def closest_point(p3grid,p3list,pt,target=None):
    #print p3list
    #print p3grid.size(),p3grid.nbFilledVoxels()
    if target:
        pass #print 'target voxel',p3grid.indexFromPoint(p3list[target])
    mc = manual_closest(pt,p3list)
    ac = p3grid.closest_point(pt)
    #print mc , pt, p3list[mc],  norm(pt-p3list[mc])
    #print ac , pt, p3list[ac],  norm(pt-p3list[ac])
    assert ac == mc

def test_pointgrid_closest_dist1():
    p3list = [(0,0,0),(10,10,10)]+[(2.5,2.5,2.5),(5,5,5)]
    p3grid = Point3Grid(1*ID,p3list)
    closest_point(p3grid,p3list,Vector3(2.5,3.5,2.5),2)

def test_pointgrid_closest_dist2():
    p3list = [(0,0,0),(10,10,10)]+[(2.5,2.5,2.5),(5,5,5)]
    p3grid = Point3Grid(1*ID,p3list)
    closest_point(p3grid,p3list,Vector3(2.5,4.5,2.5),2)

def test_pointgrid_closest_dist3():
    p3list = [(0,0,0),(10,10,10)]+[(1.9,2.9,5),(3.1,1.1,5)]
    p3grid = Point3Grid(1*ID,p3list)
    closest_point(p3grid,p3list,Vector3(1.9,1.1,5),3)
    
if __name__ == '__main__':
    test_pointgrid_corners()
    test_pointgrid_closest_dist1()
    test_pointgrid_closest_dist2()
    test_pointgrid_closest_dist3()
    test_pointgrid_closest(100)
    test_pointgrid_closest(100,100)
    test_pointgrid_closest(100,1000)
#test_pointgrid_access()
