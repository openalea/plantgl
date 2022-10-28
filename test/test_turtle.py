import openalea.plantgl.all as pgl

def test_turtle_gc_gen():
    p = pgl.PglTurtle()
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

def retrieve_primitive(sh):
    while hasattr(sh,'geometry'):
        sh = sh.geometry
    while hasattr(sh,'primitive'):
        sh = sh.primitive
    return sh

def test_turtle_sphere():
    p = pgl.PglTurtle()
    p.f(10)
    p.sphere(3)
    assert len(p.getScene()) == 1
    assert isinstance(retrieve_primitive(p.getScene()[0]), pgl.Sphere) 

def test_turtle_cylinder():
    p = pgl.PglTurtle()
    p.f(20)
    p.F(3)
    assert len(p.getScene()) == 1
    assert isinstance(retrieve_primitive(p.getScene()[0]), pgl.Cylinder) 

def test_turtle_frustum():
    p = pgl.PglTurtle()
    p.f(20)
    p.F(3,0.3)
    assert len(p.getScene()) == 1
    assert isinstance(retrieve_primitive(p.getScene()[0]), pgl.Frustum) 

def test_turtle_box():
    p = pgl.PglTurtle()
    p.f(20)
    p.box(3,0.3)
    assert len(p.getScene()) == 1
    assert isinstance(retrieve_primitive(p.getScene()[0]), pgl.Box) 

def test_turtle_polygon():
    p = pgl.PglTurtle()
    p.startPolygon()
    p.polygonPoint()
    p.f(10)
    p.polygonPoint()
    p.right(120)
    p.f(10)
    p.polygonPoint()
    p.stopPolygon()
    assert len(p.getScene()) == 1
    tr = retrieve_primitive(p.getScene()[0])
    assert isinstance(tr, pgl.TriangleSet) 
    assert len(tr.indexList) == 1
    assert len(tr.pointList) == 3

if __name__ == '__main__':
    import traceback as tb
    test_func = [ (n,v) for n,v in list(globals().items()) if ('test' in n) and hasattr(v,'__code__')]
    test_func.sort(key = lambda x : x[1].__code__.co_firstlineno)
    for tfn,tf in test_func:
        print(tfn)
        try:
            tf()
        except:
            tb.print_exc()