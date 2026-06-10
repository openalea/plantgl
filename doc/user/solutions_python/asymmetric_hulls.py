from openalea.plantgl.all import *

# First hull with bottom point (0,0,-1), top point (0,0.3,3.8)
# With side points (0.5,0,0.3), (0,0.9,0), (0.6,0,0.1), (0,0.8,-0.3)
# With tangent at bottom 0.9, at top 0.8
hull = AsymmetricHull(0.5,0.9,0.6,0.8,0.3,0.,0.1,-0.3,(0,0,-1),(0,0.3,3.8),0.9,0.8)
green = Material(Color3(0,150,0))

scene = Scene()
scene += Shape(Translated(-8,0,0,hull),green)

# We do the same with a bigger tangent for top
hull = AsymmetricHull(0.5,0.9,0.6,0.8,0.3,0.,0.1,-0.3,(0,0,-1),(0,0.3,3.8),0.9,1.2)
scene += Shape(Translated(-4,0,0,hull),green)

hull = AsymmetricHull(0.5,0.9,0.6,0.8,0.3,0.,0.1,-0.3,(0,0,-1),(0,0.3,3.8),0.9,2.0)
scene += Shape(hull,green)

hull = AsymmetricHull(0.5,0.9,0.6,0.8,0.3,0.,0.1,-0.3,(0,0,-1),(0,0.3,3.8),0.9,4.0)
scene += Shape(Translated(4,0,0,hull),green)
Viewer.display(scene)

