from openalea.plantgl.all import *
from math import sqrt

def nurbsEllipsoid(xradius = 1, yradius = 1, zradius = 1):
    uknots = [0,0,0,0,1,1,1,1]
    vknots = [0,0,0,0,1/2,1/2,1/2,1,1,1,1]
    top = (0,0,1)
    bot = (0,0,-1)
    def p(point,w) :
        return [pi*r for pi,r in zip(point,[xradius,yradius,zradius])]+[w]

    ctrlPoints = [ 
        [ p(top,1), p(top,1/3), p(top,1/3), p(top,1), p(top,1/3), p(top,1/3), p(top,1) ],
        [ p((2,0,1),1/3), p((2,4,1),1/9), p((-2,4,1),1/9), p((-2,0,1),1/3), p((-2,-4,1),1/9), p((2,-4,1),1/9), p((2,0,1),1/3) ],
        [ p((2,0,-1),1/3), p((2,4,-1),1/9), p((-2,4,-1),1/9), p((-2,0,-1),1/3), p((-2,-4,-1),1/9), p((2,-4,-1),1/9), p((2,0,-1),1/3) ],
        [ p(bot,1), p(bot,1/3), p(bot,1/3), p(bot,1), p(bot,1/3), p(bot,1/3), p(bot,1) ] 
    ]

    return NurbsPatch(ctrlPoints, uknots, vknots)

def nurbsHalfEllipsoid(xradius = 1, yradius = 1, zradius = 1):
    uknots = [0,0,0,0,1,1,1,1]
    vknots = [0,0,0,0,1,1,1,1]
    top = (0,0,1)
    bot = (0,0,-1)
    def p(point,w) :
        return [pi*r for pi,r in zip(point,[xradius,yradius,zradius])]+[w]

    ctrlPoints = [ 
        [ p(top,1), p(top,1/3), p(top,1/3), p(top,1) ],
        [ p((2,0,1),1/3), p((2,4,1),1/9), p((-2,4,1),1/9), p((-2,0,1),1/3)],
        [ p((2,0,-1),1/3), p((2,4,-1),1/9), p((-2,4,-1),1/9), p((-2,0,-1),1/3)],
        [ p(bot,1), p(bot,1/3), p(bot,1/3), p(bot,1) ] 
    ]

    return NurbsPatch(ctrlPoints, uknots, vknots)


def nurbsSphere(radius = 1):
    return nurbsEllipsoid(radius, radius, radius)

def nurbsHalfSphere(radius = 1):
    return nurbsHalfEllipsoid(radius, radius, radius)


def nurbEllipse(xradius = 1, yradius = 1):
    knots = [0,0,0,0,1/2,1/2,1/2,1,1,1,1]

    def p(point,w) :
        return [pi*r for pi,r in zip(point,[xradius,yradius])]+[w]

    ctrlPoints =  [ p((1,0),1), p((1,2),1/3), p((-1,2),1/3), p((-1,0),1), p((-1,-2),1/3), p((1,-2),1/3), p((1,0),1) ]

    return NurbsCurve2D(ctrlPoints, knots)


def nurbHalfEllipse(xradius = 1, yradius = 1):
    knots = [0,0,0,0,1,1,1,1]

    def p(point,w) :
        return [pi*r for pi,r in zip(point,[xradius,yradius])]+[w]

    ctrlPoints =  [ p((1,0),1), p((1,2),1/3), p((-1,2),1/3), p((-1,0),1)]

    return NurbsCurve2D(ctrlPoints, knots)

def nurbEllipseQuarter(xradius = 1, yradius = 1):
    knots = [0,0,0,1,1,1]

    def p(point,w) :
        return [pi*r for pi,r in zip(point,[xradius,yradius])]+[w]

    ctrlPoints =  [ p((1,0),1), p((1,1),sqrt(1/2)), p((0,1),1)]

    return NurbsCurve2D(ctrlPoints, knots, degree=2) 

def nurbCircle(radius = 1):
    return nurbEllipse(radius, radius)

def nurbHalfCircle(radius = 1):
    return nurbHalfEllipse(radius, radius)
