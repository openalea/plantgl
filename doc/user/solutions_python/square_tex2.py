from openalea.plantgl.all import * 

# list of points
points = [(-1,-1,0),
          (1,-1,0),
          (1,1,0),
          (-1,1,0)]
# list of indices
indices = [(0, 1, 2, 3)]
# creating the geometry
carre = QuadSet(points,indices)
# creating a texture from a file
tex = ImageTexture("./textures/leaf_tex.png")
# the coordinates of the texture that we may use
texCoord = [(0,0),(0,1),(1,1),(1,0)]
# how we associate the coordinates of the texture to
# the vertices of the quad
texCoordIndices = [(0,1,2,3)]
# adding those informations to the geometry
carre.texCoordList = texCoord
carre.texCoordIndexList = texCoordIndices
# associating the texture and the geometry in a shape
sh = Shape(carre,tex)
Viewer.display(sh)
