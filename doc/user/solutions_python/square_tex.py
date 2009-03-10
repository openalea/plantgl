from openalea.plantgl.all import * 

# list of points
points =  [(-1,-1,0),
           (1,-1,0),
           (1,1,0),
           (-1,1,0)]
# list of indices
indices = [(0, 1, 2, 3)]
# creating the geometry
carre = QuadSet(points,indices)
# creating the texture in a material
tex = ImageTexture("./textures/bois_texture.png")
# the texture coordinates that we will use
texCoord = [(0,0),(0,1),(1,1),(1,0)]
# how texture coordinates are associated to vertices
texCoordIndices = [(0,1,2,3)]
# adding information to the geometry
carre.texCoordList = texCoord
carre.texCoordIndexList = texCoordIndices
# associating the geometry and the material in a shape
sh = Shape(carre,tex)
Viewer.display(sh)
