from openalea.plantgl.all import *
from math import pi

def line_prof():
    """ create a list of profiles with polyline """
    return [Polyline2D([(0,0),(1.5,0.1),(0.75,2),(1.1,2.2),(0.55,3),(0.8,3.1),(0,4),(0,4)]),
            Polyline2D([(0,0),(1.2,0.1),(0.7,2),(1.0,2.3),(0.5,3.1),(0.8,3.1),(0,4),(0,4)]),
            Polyline2D([(0,0),(1.4,0.1),(0.8,2),(1.1,2.1),(0.6,3),(0.85,3.0),(0,4),(0,4)]),
            Polyline2D([(0,0),(1.6,0.1),(0.8,2),(1.2,2.2),(0.4,3),(0.7,3.2),(0,4),(0,4)]),
            Polyline2D([(0,0),(1.5,0.1),(0.75,2),(1.1,2.2),(0.55,3),(0.8,3.1),(0,4),(0,4)])]

def bezier_prof():
    """ create a list of profiles with curves of Bezier """
    return [BezierCurve2D([(0,0,1),(1.5,1,1),(2.4,2,6),(0.9,3,2),(0,4,1),(0,4,1)]),
            BezierCurve2D([(0,0,1),(1.6,1,1),(2.5,2,6),(1.2,3,2),(0,4,1),(0,4,1)]),
            BezierCurve2D([(0,0,1),(1.6,1,1),(2.0,2,6),(0.8,3,2),(0,4,1),(0,4,1)]),
            BezierCurve2D([(0,0,1),(1.1,1,1),(2.5,2,6),(0.7,3,2),(0,4,1),(0,4,1)]),
            BezierCurve2D([(0,0,1),(1.3,1,1),(2.2,2,6),(1.2,3,2),(0,4,1),(0,4,1)])]

def nurbs_prof():
    """ create a list of profiles with nurbs curves """
    return [NurbsCurve2D([(0,0,1),(0.5,1,1),(1,2,1),(0.5,3,1),(0,4,1),(0,4,1)]),
            NurbsCurve2D([(0,0,1),(0.7,1,1),(1,2.1,1),(0.5,3.2,1),(0,4,1),(0,4,1)]),
            NurbsCurve2D([(0,0,1),(0.4,1,1),(0.8,2.2,1),(0.4,2.9,1),(0,4,1),(0,4,1)]),
            NurbsCurve2D([(0,0,1),(0.6,1,1),(1.2,2,1),(0.3,3,1),(0,4,1),(0,4,1)]),
            NurbsCurve2D([(0,0,1),(0.7,1,1),(1.2,2.1,2),(0.6,3.1,1),(0,4,1),(0,4,1)])]

def mixed_prof():
    """ create a list of profiles of different types """
    return [Polyline2D([(0,0),(1.5,0.1),(0.75,2),(1.1,2.2),(0.55,3),(0.8,3.1),(0,4),(0,4)]),
            BezierCurve2D([(0,0,1),(1.5,1,1),(2.4,2,6),(0.9,3,2),(0,4,1),(0,4,1)]),
            NurbsCurve2D([(0,0,1),(0.5,1,1),(1,2,1),(0.5,3,1),(0,4,1),(0,4,1)]),
            BezierCurve2D([(0,0,1),(1.6,1,1),(2.0,2,6),(0.8,3,2),(0,4,1),(0,4,1)]),
            Polyline2D([(0,0),(1.5,0.1),(0.75,2),(1.1,2.2),(0.55,3),(0.8,3.1),(0,4),(0,4)])]

scene = Scene()
# the angles to associate to profiles
angles = [0,pi/2.,pi,3.*pi/2.,2.*pi]
col = Material(Color3(0,100,50))
# create a swung interpolating the profiles associated to the angles
scene += Shape(Translated(-5,0,0,Swung(line_prof(),angles)),col)
scene += Shape(Translated(5,0,0,Swung(nurbs_prof(),angles)),col)
scene += Shape(Translated(5,5,0,Swung(mixed_prof(),angles)),col)
scene += Shape(Swung(bezier_prof(),angles),col)

Viewer.display(scene)
