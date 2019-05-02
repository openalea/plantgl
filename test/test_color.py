from openalea.plantgl.all import *
from random import randint

def test_color3_to_uint():
    """ Test Color3 conversion from and to int """
    c = Color3(randint(0,255),randint(0,255),randint(0,255))
    i = int(c)
    c2 = Color3.fromUint(i)
    print(c,i,c2)
    assert c == c2 and "conversion of Color3 to/from uint failed"


def test_color4_to_uint():
    """ Test Color4 conversion from and to int """
    c = Color4(randint(0,255),randint(0,255),randint(0,255),randint(0,255))
    i = int(c)
    c2 = Color4.fromUint(i)
    print(c,i,c2)
    assert c == c2 and "conversion of Color4 to/from uint failed"