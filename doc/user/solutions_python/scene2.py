from openalea.plantgl.all import * 

s = Sphere(2)
m = [Material(Color3(150,0,0)), Material(Color3(100,150,0))]
s = [Translated(-5,0,0,s),Translated(-3,0,1,s)]
shapes = [Shape(sp,m[i]) for i,sp in enumerate(s)]
scene = Scene(shapes)
Viewer.display(scene)
