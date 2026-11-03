from openalea.plantgl.all import * 

s1 = Sphere(3)
m = Material("red", Color3(150,0,0))
Shape1 = Shape(s1,m)
Viewer.display(Shape1)
