from openalea.plantgl.all import *
from random import uniform
from math import pi


def test_vector():
    """ Regression test for bug #10381 """
    v = Vector3(uniform(0,1),uniform(0,1),uniform(0,1))
    v1 = v*10
    v2 = 10*v
    assert v1==v2


def test_vector_creation():
    #v = Vector2(1)
    v = Vector2(1,2)
    v = Vector2((1,2))
    v = Vector2([1,2])
    v = Vector2(Vector2.Polar(1,2))
    v = Vector2(Vector2(1,2))

    #v = Vector3(1)
    v = Vector3(1,2)
    v = Vector3(1,2,3)
    v = Vector3((1,2,3))
    v = Vector3([1,2,3])
    v = Vector3(Vector3.Spherical(0,0,0))
    v = Vector3(Vector3.Cylindrical(0,0,0))
    v = Vector3(Vector2(1,2))
    v = Vector3(Vector2(1,2),1)
    v = Vector3(Vector3(1,2,3))

    v = Vector4(1)
    v = Vector4(1,2)
    v = Vector4(1,2,3)
    v = Vector4((1,2,3))
    v = Vector4([1,2,3])
    v = Vector4(Vector2(1,2))
    v = Vector4(Vector2(1,2),3,4)
    v = Vector4(Vector3(1,2,3))
    v = Vector4(Vector3(1,2,3),4)
    #v = Vector4(v)


if __name__ == '__main__':
    test_vector_creation()

