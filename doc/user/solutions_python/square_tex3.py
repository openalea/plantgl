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
# creating the texture
tex = ImageTexture("./textures/wood_texture.png")
# the texture coordinates that we will use
texCoord = [(0.25,0.25),(0.25,0.75),(0.75,0.75),(0.75,0.25)]
# associating the coordinates and the vertices
texCoordIndices = [(0,1,2,3)]
# adding the information to the geometry
carre.texCoordList = texCoord
carre.texCoordIndexList = texCoordIndices
# associating the material and the geometry
sh = Shape(carre,tex)
Viewer.display(sh)
