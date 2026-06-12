from openalea.plantgl.all import *
from math import pi

# we create a circle
c = Polyline2D.Circle(1.,50)
m1 = Material(Color3(80,150,0))
# we create a profile
p1 = NurbsCurve2D([(0,0,1),(0.5,1,1),(1,2,1),(0.5,3,1),
                   (0,4,1),(0,4,1),(-0.7,3,1),(-1,2,1),(-0.8,1,1),(0,0,1)])
# we create a geometry extruding the circle along the profile
sh1 = Shape(Translated(-2,0,0,ExtrudedHull(p1,c)),m1)

m2 = Material(Color3(0,150,80))
# an other profile
p2 = NurbsCurve2D([(0,0,1),(1,1.6,1),(2,1.8,1),(3,2.5,1),
                   (4,0,1),(4,0,1),(3,-2.5,1),(2,-1.8,1),(1,-1.6,1),(0,0,1)])
sh2 = Shape(Translated(2,0,0,ExtrudedHull(c,p2,True)), m2)

scene = Scene([sh1,sh2])

Viewer.display(scene)
