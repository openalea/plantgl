from openalea.plantgl.all import * 

# list of vertices
points = [(-1,1,0),(-1,3,0),
          (1,3,0),(1,1,0),
          (3,1,0),(3,-1,0),
          (1,-1,0),(1,-3,0),
          (-1,-3,0),(-1,-1,0),
          (-3,-1,0),(-3,1,0)]
# list of indices to make the quads
indices = [(0, 1, 2, 3),
           (3, 4, 5, 6),
           (6, 7, 8, 9),
           (9, 10, 11, 0),
           (0, 3, 6, 9)]
# creating the geometry
cross = QuadSet(points,indices)
# creating the textures from files in materials
tex = ImageTexture("./textures/wood_texture.png")
tex2 = ImageTexture("./textures/leaf_tex.png")

# the coordinates of the texture to use
texCoord = [(0.33,0.66),(0.33,0.99),
            (0.66,0.99),(0.66,0.66),
            (0.99,0.66),(0.99,0.33),
            (0.66,0.33),(0.66,0),
            (0.33,0),(0.33,0.33),
            (0,0.33),(0,0.66)]
# how the coordinates are associated with the vertices
texCoordIndices = [(0, 1, 2, 3), 
                   (3, 4, 5, 6),
                   (6, 7, 8, 9),
                   (9, 10, 11, 0),
                   (0, 3, 6, 9)]

# adding the information to the geometry
cross.texCoordList = texCoord
cross.texCoordIndexList = texCoordIndices
cross2 = Translated(-7,0,0,cross)
# associating the different textures to the same geometry
sh = [Shape(cross,tex), Shape(cross2,tex2)]
Viewer.display(Scene(sh))
