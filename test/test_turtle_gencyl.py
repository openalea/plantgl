from openalea.plantgl.all import *

def test_turtle_gc_gen():
    p = PglTurtle()
    p.startGC()
    p.F(10)
    p.push()
    p.left(10)
    p.F(10)
    p.pop()
    p.push()
    p.right(10)
    p.pop()
    p.F(10)
    p.stopGC()
    assert len(p.getScene()) == 2

if __name__ == '__main__':
    test_turtle_gc_gen()
