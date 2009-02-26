from openalea.plantgl.all import *
from random import uniform
from math import pi

epsilon = 1e-4

def test_toaxisangle():
    v = Vector3(uniform(0,1),uniform(0,1),uniform(0,1))
    v.normalize()
    r = uniform(-pi,pi)
    m = Matrix3.axisRotation(v,r)
    v2,r2 = m.toAxisAngle()
    print v,r,v2,r2
    assert (norm(v-v2) < epsilon and r-r2 < epsilon) or (norm(v+v2) < epsilon and r+r2 < epsilon)

def test_eulerrotatedZYX():
    v = Vector3(uniform(-pi,pi),uniform(-pi,pi),uniform(-pi,pi))
    m = Matrix3.eulerRotationZYX(v)
    v2 = m.eulerAnglesZYX()
    v3 = (pi+v2.x,-v2.y-pi,v2.z-pi)
    print v,v2
    assert v.x-v2.x < epsilon or v.x- pi-v2.x < epsilon
    assert v.y-v2.y < epsilon or v.y- pi-v2.y < epsilon
    assert v.z-v2.z < epsilon or v.z- pi-v2.z < epsilon
    #norm(v-v2) < epsilon or norm(v-v3) < epsilon

def test_eulerrotatedXYZ():
    v = Vector3(uniform(-pi,pi),uniform(-pi,pi),uniform(-pi,pi))
    m = Matrix3.eulerRotationXYZ(v)
    v2 = m.eulerAnglesXYZ()
    print v,v2
    assert v.x-v2.x < epsilon or v.x- pi-v2.x < epsilon
    assert v.y-v2.y < epsilon or v.y- pi-v2.y < epsilon
    assert v.z-v2.z < epsilon or v.z- pi-v2.z < epsilon
    #assert norm(v-v2) < epsilon