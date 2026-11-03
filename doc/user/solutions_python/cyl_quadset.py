from math import cos
from math import sin
from math import pi
from openalea.plantgl.all import *

def quad_cyl(radius, height, slices = 12):
    """ quad_cyl(radius, height) makes a cylinder with quads as primitives 
                             with height height and radius radius """
    # we create the empty lists of points and indices 
    points = []
    indices = []
    for i in range(slices):
        # we had 2 points at each iteration, the top and bottom
        # of a slice inn the cylinder, after a rotation of 2*pi
        # over the number of slices
        points += [(radius*cos(2*i*pi/slices),radius*sin(2*i*pi/slices), 0),
                   (radius*cos(2*i*pi/slices),radius*sin(2*i*pi/slices), height)]
        # we set the indices, each quad joining two slices
        if i is not (slices -1):
           indices += [(2*i,2*i+1,2*i+3,2*i+2)]
        else:
           indices += [(2*i,2*i+1,1,0)]
    # we create the geometry
    cyl = QuadSet(points,indices)
    return cyl

# The same cylinder with 3 different discretizations
cyl1 = quad_cyl(1,4)
cyl2 = Translated(-4,0,0,quad_cyl(1,4,6))
cyl3 = Translated(4,0,0,quad_cyl(1,4,3))

m1 = Material(Color3(150,0,0))
m2 = Material(Color3(0,150,0))
m3 = Material(Color3(0,0,150))

shapes = [Shape(cyl1,m1), Shape(cyl2,m2), Shape(cyl3,m3)]
Viewer.display(Scene(shapes))

