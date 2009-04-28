from random import randint
from openalea.plantgl.all import *
from math import pi;
from math import cos;
from math import sin;

shape1 = Polyline2D([(0,0),(2,2),(3,1),(4,2),(5,0),(4,-2),(3,-1),(2,-2),(0,0)])
shape2 = Polyline2D([(0,0),(1,2),(2,3),(3,4),(3.5,4),(4,3.5),(4.5,3),(5,2.5),(5.5,1.3),(7.2,0),(5.5,-1.5),(5,-2.6),(4.5,-3.2),(4,-3.3),(3.5,-3.8),(3,-4.1),(2,-2.5),(1,-2.6),(0,0)])
r1 = 2
r2 = 3
N = 18
shape3 = Polyline2D([((r1*(i%2)-r2*((i%2)-1))*cos(i*pi/(1.0*N)),(r1*(i%2)-r2*((i%2)-1))*sin(i*pi/(1.0*N))) for i in range(2*N+1)])
shape4 = Polyline2D([(r2*cos(i*pi/(0.5*N)),r2*sin(i*pi/(0.5*N))) for i in range(N+1)])
shape5 = Polyline2D([(0,0),(2,2),(3,-1),(6,-2),(7,1),(4,2),(2,-2),(0,0)])
shape6 = Polyline2D([(0,0),(1,1),(2,1),(3,0),(2,-1),(1,-1),(0,0)])

red = Material(Color3(255,0,0))
green = Material(Color3(0,255,0))
blue = Material(Color3(0,0,255))

colors = [Color4(0,0,255,255),Color4(255,0,255,255),Color4(0,255,255,255)]

geometries = [Shape(shape1,green),Shape(Translated (-10,-10,0,shape2),green), Shape(Translated(10,10,0,shape3),green),Shape(Translated(10,-10,0,shape4),green),Shape(Translated (-10,10,0,shape5),green),Shape(Translated(10,0,0,shape6),green)]

#geometries = [Shape(Translated (10,10,0,shape3),blue),Shape(Translated (10,-10,0,shape4),green)]

filter = 0.0

polylines = Skeleton.getChordalAxisTransform(shape1,filter)
triangulation = Skeleton.getDelaunayConstrained2DTriangulation(shape1)
triangulation.colorList = colors
triangulation.colorIndexList = [(0,1,2) for i in range(len(triangulation.indexList))]
geometries += [Shape(a,red) for a in polylines]
#geometries += [Shape(triangulation)]

polylines = Skeleton.getChordalAxisTransform(shape2,filter)
triangulation = Skeleton.getDelaunayConstrained2DTriangulation(shape2)
triangulation.colorList = colors
triangulation.colorIndexList = [(0,1,2) for i in range(len(triangulation.indexList))]
geometries += [Shape(Translated (-10,-10,0,a),red) for a in polylines]
#geometries += [Shape(Translated (-10,-10,0,triangulation))]

polylines = Skeleton.getChordalAxisTransform(shape3,filter)
triangulation = Skeleton.getDelaunayConstrained2DTriangulation(shape3)
triangulation.colorList = colors
triangulation.colorIndexList = [(0,1,2) for i in range(len(triangulation.indexList))]
geometries += [Shape(Translated (10,10,0,a),red) for a in polylines]
#geometries += [Shape(Translated (10,10,0,triangulation))]

polylines = Skeleton.getChordalAxisTransform(shape4,filter)
triangulation = Skeleton.getDelaunayConstrained2DTriangulation(shape4)
triangulation.colorList = [Color4(randint(0,255),randint(0,255),randint(0,255),255) for i in xrange(len(triangulation.indexList))]#colors
triangulation.colorPerVertex = False
#triangulation.colorIndexList = [(0,1,2) for i in range(len(triangulation.indexList))]
#geometries += [Shape(Translated (10,-10,0,a),red) for a in polylines]
geometries += [Shape(Translated (10,-10,0,triangulation))]

polylines = Skeleton.getChordalAxisTransform(shape5,filter)
triangulation = Skeleton.getDelaunayConstrained2DTriangulation(shape5)
triangulation.colorList = colors
triangulation.colorIndexList = [(0,1,2) for i in range(len(triangulation.indexList))]
#geometries += [Shape(Translated (-10,10,0,a),red) for a in polylines]
geometries += [Shape(Translated (-10,10,0,triangulation),blue)]

polylines = Skeleton.getChordalAxisTransform(shape6,filter)
triangulation = Skeleton.getDelaunayConstrained2DTriangulation(shape6)
triangulation.colorList = colors
triangulation.colorIndexList = [(0,1,2) for i in range(len(triangulation.indexList))]
geometries += [Shape(Translated (10,0,0,a),red) for a in polylines]
#geometries += [Shape(Translated (10,0,0,triangulation))]

scene = Scene (geometries)
Viewer.display(scene)
