from openalea.plantgl.all import *
from math import pi


# primitive to use as initiator
disc=Disc(1,50)

def create_transformation(translation,scaling,angles):
    # retrieve the rotation angles
    elevation,azimuth,roll = angles
    matrix = Matrix4() # Identity matrix
    matrix.setTransformation(scaling,Vector3(azimuth,elevation,roll),translation) # set the transformation needed
    return Transform4(matrix) # Store the matrix into a Transform4 object needed for IFS
    
transformations=[create_transformation(t,s,a) for t,s,a in
  # list of transformation values
  [((0,0,2),(0.33,0.33,0.33),(0,0,0)),
   ((0,0.5,1.5),(0.33,0.33,0.33),(pi/4,pi/2,0)),
   ((0,-0.5,1.5),(0.33,0.33,0.33),(pi/4,3*pi/2,0)),
   ((0.5,0,1),(0.33,0.33,0.33),(pi/4,0,0)),
   ((-0.5,0,1),(0.33,0.33,0.33),(pi/4,pi,0)),
   ((0,1,0.5),(0.33,0.33,0.33),(pi/4,pi/2,0)),
   ((0,-1,0.5),(0.33,0.33,0.33),(pi/4,3*pi/2,0)),
   ((1,0,0),(0.33,0.33,0.33),(pi/4,0,0)),
   ((-1,0,0),(0.33,0.33,0.33),(pi/4,pi,0))]]

# creating the ifs object. 4 is the recursion level.
ifs=IFS(4,transformations,disc)

# display the ifs with green material
Viewer.display(Shape(ifs,Material((5,108,8))))


