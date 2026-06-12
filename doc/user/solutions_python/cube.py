from openalea.plantgl.all import * 

# list of points
points = [(-1,-1,-1), # coordinates of bottom right corner
          (1,-1,-1),
          (1,1,-1),
          (-1,1,-1),
          (-1,-1,1),
          (1,-1,1),
          (1,1,1),    # coordinates of top left corner
          (-1,1,1)]

# list of indices to make the quads from the points
indices = [(0, 1, 2, 3), # indices for bottom face
           (4, 5, 6, 7), # indices for top face
           (0, 1, 5, 4), #
           (1, 2, 6, 5), # indices for 
           (2, 3, 7, 6), # side faces
           (3, 0, 4, 7)] #

# list of colors
colors = [Color4(150,0,0,155),
          Color4(150,150,0,155),
          Color4(0,150,0,155),
          Color4(0,150,150,155),
          Color4(0,0,150,155),
          Color4(150,0,150,155)]

# construction of the geometry
cube = QuadSet(points,indices)
# adding information to the geometry
cube.colorList = colors

# list of indices to associate a vertex in a face to a color
# here each face will be associated to only one color of the list 
cube.colorIndexList = [(0,0,0,0), 
                       (1,1,1,1),
                       (2,2,2,2),
                       (3,3,3,3),
                       (4,4,4,4),
                       (5,5,5,5)]
Viewer.display(cube)
