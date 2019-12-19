from random import random
from openalea.plantgl.all import *


def test_polyline():
    p = Polyline2D([(2*i,random()-0.5) for i in range(10)])

    f1 = p.getArcLengthToUMapping()

    assert f1.firstx == 0.0
    assert f1.lastx == 1.0
    assert f1(f1.firstx) == 0.0
    assert f1(f1.lastx) == 9.0

    f2 = p.getUToArcLengthMapping()

    assert f2.firstx == 0.0
    assert f2.lastx == 9.0
    assert f2(f2.firstx) == 0.0
    assert f2(f2.lastx) == 1.0


if __name__=="__main__":
    test_polyline()

