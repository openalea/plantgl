import openalea.plantgl.all as pgl
import openalea.plantgl.algo.jsonrep as jr
from test_object_creation import shapebenchmark_generator,randtransform,randint
import pytest


def jsonconversion(sceneobj, verbose = False):
  assert sceneobj.isValid()
  if verbose:
    print(sceneobj)
  res = jr.to_json_rep(sceneobj)
  if verbose:
      print(res)
  assert type(res) == dict
  sceneobj2 = jr.from_json_rep(res)
  assert type(sceneobj2) == type(sceneobj)

@pytest.mark.parametrize('sceneobj', list(shapebenchmark_generator()))
def test_jsonconversion_on_benchmark_objects(sceneobj):
    jsonconversion(sceneobj)

def test_matrix():
    t = pgl.Matrix4()
    jsonconversion(t)

def test_extrusion():
    bc = pgl.BezierCurve([(0,0,0,1),(.5,1,0,1),(1.,1,0,1),(1.5,0,0,1)])
    circle = pgl.Polyline2D.Circle(1,20)
    t = pgl.Extrusion(bc,circle)
    jsonconversion(t)

def test_tapered():
    ds = pgl.Sphere()
    t = pgl.Tapered(1.,1.,ds)    
    jsonconversion(t)

def test_scene():
    ds = pgl.Sphere()
    t = pgl.Tapered(1.,1.,ds)    
    t2 = pgl.Translated(0.,0.,1., ds)    
    sc = pgl.Scene([pgl.Shape(t, pgl.Material((255,0,0))),pgl.Shape(t2, pgl.Material((0,255,0)))])
    jsonconversion(sc)

def test_texture():
    fname = '../share/plantgl/pixmap/geomviewer.png'
    t = pgl.ImageTexture(fname)
    t.baseColor = (255,0,0,0)
    sh = pgl.Shape(pgl.Cylinder(),t, 2)
    jsonconversion(sh)


def te_st_jsonrep():
    jc = jr.JsonRepConverter()
    print(jc.conversion_map.keys())

if __name__ == '__main__':
    import traceback as tb
    test_func = [ (n,v) for n,v in list(globals().items()) if 'test_' in n]
    test_func.sort(key=lambda x : x[1].__code__.co_firstlineno)
    for tfn,tf in test_func:
        print(tfn)
        if hasattr(tf, 'pytestmark'):
            args = tf.pytestmark[0].args[1]
            for arg in args:
                try:
                    tf(arg)
                except:
                    tb.print_exc()
        else:
            try:
                tf()       
            except:
                tb.print_exc()        