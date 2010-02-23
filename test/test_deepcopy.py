from openalea.plantgl.all import *
import warnings

def test_deepcopy():
    s = Sphere()
    s2 = s.deepcopy()
    assert type(s2) == type(s)
    Scaled(1,s)
    Scaled(1,s2)
    sc = Scene()
    sc += s
    sc += s2

def test_deepcopy_polyline():
    s = Polyline([(0,0,0),(1,0,0)])
    s2 = s.deepcopy()
    assert type(s2) == type(s)
    Group([s],s)
    try:
        Group([s2],s2).isValid()
    except:
        warnings.warn( "DeepCopy mixup type")
    sc = Scene()
    sc += s
    try:
        sc += s2
    except:
        warnings.warn( "DeepCopy mixup type")

def test_polyline_as_result():
    s = Polyline2D([(0,0),(1,0)])
    d = Discretizer()
    s.apply(d)
    s2 = d.result
    Group([s],skeleton=Polyline([(0,0,0),(1,0,0)]))
    try:
        Group([s],s2).isValid()
    except:
        warnings.warn( "Discretizer return value type mixup")
    sc = Scene()
    sc += s
    sc += s2
    
if __name__ == '__main__':
    import traceback as tb
    test_func = [ (n,v) for n,v in globals().items() if 'test' in n]
    test_func.sort(lambda x,y : cmp(x[1].func_code.co_firstlineno,y[1].func_code.co_firstlineno))
    for tfn,tf in test_func:
        print tfn
        try:
            tf()
        except:
            tb.print_exc()