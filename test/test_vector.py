from openalea.plantgl.all import *
from random import uniform
from math import pi


def test_vector():
    """ Regression test for bug #10381 """
    v = Vector3(uniform(0,1),uniform(0,1),uniform(0,1))
    v1 = v*10
    v2 = 10*v
    assert v1==v2

