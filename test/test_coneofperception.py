from openalea.plantgl.all import *
from random import *
from math import *

rseed = 1
seed(rseed)

maxdist = 10
rdpoint = lambda : Vector3(uniform(-maxdist,maxdist),uniform(-maxdist,maxdist),uniform(-maxdist,maxdist))

nbPoints = 5000
pointList = [rdpoint() for i in range(nbPoints)]
grid = Point3Grid((0.5,0.5,0.5), pointList)
coneradius = 100
coneradius = coneradius/10.
alpha, beta = 77,111
coneangle = 188
cosconeangle = cos(radians(coneangle/2))
print(cosconeangle)

mdirection = Vector3(Vector3.Spherical(1, radians(alpha), radians(beta)))

inCone = lambda p : is_point_in_cone(p,(0,0,0),mdirection, coneradius,radians(coneangle))

def test_points_in_cone():
    selection = grid.query_points_in_cone((0,0,0),mdirection,coneradius,radians(coneangle))

    missed = set([])
    for i,p in enumerate(pointList):
     if inCone(p) != (i in selection):
       missed.add(i)
    print(len(selection),'are selected')
    print(len(missed),'are missing')
   
    if len(missed) > 0:
      voxels = set(grid.query_voxels_in_cone((0,0,0),mdirection, coneradius,radians(coneangle)))
      print([(p,pointList[p],grid.cellIdFromPoint(pointList[p]),grid.cellIdFromPoint(pointList[p]) in voxels) for p in missed])
    assert len(missed) == 0 and 'Some points are missed in the test'


if __name__ == '__main__':
  test_points_in_cone()
