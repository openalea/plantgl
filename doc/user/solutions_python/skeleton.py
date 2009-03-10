from openalea.plantgl.all import *


shape = Polyline2D([(0,0),(1,1),(2,2),(3,1),(4,2),(5,0),(4,-2),(3,-1),(2,-2),(1,-1),(0,0)])

hull = [shape]
polylines = Skeleton.getChordalAxisTransform(shape)
for a in polylines:
   print("point :"+a[0]+" , "+a[1] +"\n");

geometries = [shape]
geometries += polylines

scene = Scene (geometries)
Viewer.display(scene)
