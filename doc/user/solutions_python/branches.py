from openalea.plantgl.all import *

cir = Polyline2D.Circle(1,50)
prof1 = NurbsCurve([(0.0, 0.0, 0, 1),
                   (0.8, 1.0, 3, 1),
                   (0.4, 1.1, 6, 2),
                   (-0.2, 0.3, 8, 2),
                   (-0.4, 0.0, 9, 2)])
scales1 = [(0.9, 0.7),
          (0.7, 0.6),
          (0.5, 0.4),
          (0.2, 0.2),
          (0.0, 0.0)]
prof2 = NurbsCurve([(0.8, 1.0, 3, 1),
                   (1.5, 1.8, 4, 1),
                   (1.7, 2.1, 5, 1),
                   (1.2, 1.7, 6, 1),
                   (1.0, 1.5, 7, 1)])
scales2 = [(0.5, 0.5),
           (0.37, 0.35),
           (0.24, 0.21),
           (0.11, 0.08),
           (0.0, 0.0)]


br1 = Extrusion(prof1, cir, scales1)
br2 = Extrusion(prof2, cir, scales2)
col = Material(Color3(127,72,0))
shb = [Shape(br1,col), Shape(br2,col)]
scene = Scene(shb)
Viewer.display(scene)
