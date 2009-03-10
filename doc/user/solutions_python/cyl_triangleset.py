from math import cos
from math import sin
from math import pi
from openalea.plantgl.all import *

def tri_cyl(radius, height, slices = 12):
    """ tri_cyl(radius, height) makes a cylinder with triangles 
                as primitives with height height and radius radius """
    points = []
    indices = []
    for i in range(slices):
        points += [(radius*cos(2*i*pi/slices),radius*sin(2*i*pi/slices), 0),
                   (radius*cos(2*i*pi/slices),radius*sin(2*i*pi/slices), height)]
        if i is not (slices -1):
           indices += [(2*i,2*i+1,2*i+2),(2*i+1,2*i+2,2*i+3)]
        else:
           indices += [(2*i,2*i+1,0),(2*i+1,0,1)]
    cyl = TriangleSet(points,indices)
    return cyl

cyl1 = tri_cyl(1,4)
cyl2 = Translated(-4,0,0,tri_cyl(1,4,6))
cyl3 = Translated(4,0,0,tri_cyl(1,4,3))

m1 = Material(Color3(150,0,0))
m2 = Material(Color3(0,150,0))
m3 = Material(Color3(0,0,150))

shapes = [Shape(cyl1,m1), Shape(cyl2,m2), Shape(cyl3,m3)]
Viewer.display(Scene(shapes))

