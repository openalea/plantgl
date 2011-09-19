from openalea.plantgl.all import *
from random import *
from math import *

def test(visual = False):
    maxheigth = 50
    radius = 5
    src = Vector3(Vector3.Cylindrical(radius,pi*uniform(0,2),uniform(maxheigth*0.1,maxheigth*0.9)))
    a, b = 1, 1/(2*pi)
    #sab = a+b
    #a /= sab
    #b /= sab
    seta = []
    setb = []
    setc = []
    setd = []
    for i in xrange(5000):
        p = Vector3(Vector3.Cylindrical(radius,pi*uniform(0,2),uniform(0,maxheigth)))
        if (p-src).radialAnisotropicNorm(Vector3.OZ,a,b) < radius:
            if norm(p-src) < radius: setc.append(p)
            else: seta.append(p)
        else: 
            if norm(p-src) < radius: setd.append(p)
            else: setb.append(p)
    if visual:
        width = 3
        s = Scene([Shape(PointSet([src,src],width=width),Material((255,0,0)))])
        if len(seta) > 0: s += Shape(PointSet(seta,width=width),Material((0,255,0)))
        if len(setb) > 0: s += Shape(PointSet(setb,width=width),Material((0,0,255)))
        if len(setc) > 0: s += Shape(PointSet(setc,width=width),Material((255,100,0)))
        if len(setd) > 0: s += Shape(PointSet(setd,width=width),Material((0,255,255)))
        Viewer.display(s)
        Viewer.dialog.question('result','check results')

if __name__ == '__main__':
    test(True)