from openalea.plantgl.all import *
from math import pi
from math import sqrt


def tree3():
    
    # We compute the trunk of the tree with an extruded circle
    cyl = Extrusion(Polyline([(0,0,0),(0,0,3),(0,2,5)]),Polyline2D.Circle(1.0,50))
    cyl.solid = True
    tex = ImageTexture("./textures/bois_texture.png")
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
    square = Translated(0,1.8,1.1,square)
    
    leaves = [Shape(Translated(0,0,1.6*i,square),tex) for i in range(2)]
    square = AxisRotated((0,0,1),pi,square)
    leaves += [Shape(Translated(0,0,1.6*i,square),tex) for i in range(2)]

    leaves += [Shape(Translated(0,1.6*i*1./sqrt(2),1.6*(2+i*1./sqrt(2)),AxisRotated((1,0,0),-pi/4.,square)),tex) for i in range(2)]
    leaves += [Shape(Translated(0,1.6*i*1./sqrt(2),1.6*(2+i*1./sqrt(2)),AxisRotated((0,2,2),pi,AxisRotated((1,0,0),-pi/4.,square))),tex) for i in range(2)]

    return leaves+[trunk]


scene = Scene(tree3())
Viewer.display(scene)
