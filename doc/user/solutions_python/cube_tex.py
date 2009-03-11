from openalea.plantgl.all import * 

# list of points
points = [(-1,-1,-1),
          (1,-1,-1),
          (1,1,-1),
          (-1,1,-1),
          (-1,-1,1),
          (1,-1,1),
          (1,1,1),
          (-1,1,1)]
# list of indices 
indices = [(0, 1, 2, 3),
           (4, 5, 6, 7),
           (0, 1, 5, 4),
           (1, 2, 6, 5),
           (2, 3, 7, 6),
           (3, 0, 4, 7)]
# creation of the geometry
cube = QuadSet(points,indices)
# creation of a texture from a file
tex = ImageTexture("./textures/wood_texture.png")
# the texture coordinates that we will use
texCoord = [(0,0),(0,1),(1,1),(1,0)]
# how we associate texture coordinates to vertices in a quad
texCoordIndices = [(0,1,2,3) for i in range(len(indices))]
# adding those informations to the geometry
cube.texCoordList = texCoord
cube.texCoordIndexList = texCoordIndices
# associating the geometry and the material in a Shape
sh = Shape(cube,tex)
Viewer.display(sh)
