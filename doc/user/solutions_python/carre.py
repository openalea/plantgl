from openalea.plantgl.all import * 

# A list of the coordinates of the square
points = [(-1,-1,0),(1,-1,0),(1,1,0),(-1,1,0)]
# A list of colors
colors = [Color4(150,0,0,155),
          Color4(150,150,0,155),
          Color4(0,150,0,155),
          Color4(0,150,150,155)]
# A list of directions for the normals
normals = [(0,0,1) for i in range(4)]
# A list of indices that set the indices for the quads
indices = [(0, 1, 2, 3)]
# Creation of the quadset
carre = QuadSet(points,indices,normals,indices,colors)
Viewer.display(carre)
