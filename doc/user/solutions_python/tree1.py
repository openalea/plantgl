from openalea.plantgl.all import *
from math import pi
from math import sqrt

# This is the function for a tree with leaves 
# along the trunk on both sides
def tree1():
    
    # We compute the trunk of the tree with an extruded circle
    cyl = Extrusion(Polyline([(0,0,0),(0,0,6)]),Polyline2D.Circle(1.0,50))
    cyl.solid = True
    tex = ImageTexture("./textures/wood_texture.png")
    trunk = Shape(cyl,tex)
    
    # Now we compute a leaf. We will use a transparent texture over a square
    points = [(0,-1,-1),
              (0,1,-1),
              (0,1,1),
              (0,-1,1)]
    indices = [(0, 1, 2, 3)]
    square = QuadSet(points,indices)
    tex = ImageTexture("./textures/palm_leaf.png")
    texCoord = [(0,0),(0,1),(1,1),(1,0)]
    texCoordIndices = [(0,1,2,3)]
    square.texCoordList = texCoord
    square.texCoordIndexList = texCoordIndices
    square = EulerRotated(0,pi/2.,0,square)
    square = AxisRotated((1,0,0),pi/4., square)
    square = Translated(0,2.0,1.1,square)    
    # scale factor
    sf = 0.6

    leaves = [Shape(Translated(0,0,1.5*i,square),tex) for i in range(4)]
    square = AxisRotated((0,0,1),pi,square)
    leaves += [Shape(Translated(0,-0,1.5*i,square),tex) for i in range(4)]

    return leaves+[trunk]

scene = Scene(tree1())
Viewer.display(scene)
