from openalea.plantgl.scenegraph import *
from openalea.plantgl.algo import volume, surface

epsilon = 1e-5
def equal(x, y, eps=epsilon):
    return abs(x-y)<eps

def assert_geom(geom, surf, vol, eps=epsilon):
    assert geom.isValid()
    assert equal(surface(geom), surf, eps), "%f, %f"%(surface(geom),surf)
    assert equal(volume(geom), vol, eps), "%f, %f"%(volume(geom),vol)

def test_box():
    geom = Box()
    surf = 6.; vol = 1.
    assert_geom(geom, surf, vol)
    geom.size *= 2
    assert_geom(geom, surf*4,vol*8, epsilon*8) 

def test_sphere():
    geom = Sphere()
    surf = 3.141592; vol = 0.5236
    assert_geom(geom, surf, vol)
    geom.radius *= 2
    assert_geom(geom, surf*4,vol*8, epsilon*8) 

def test_cone():
    geom = Cone()
    surf=2.54160; vol=0.2618
    assert_geom(geom, surf, vol)
    geom.radius*=2; geom.height*=2
    assert_geom(geom, surf*4,vol*8, epsilon*8) 

def test_cylinder():
    geom = Cylinder()
    surf = 4.712389; vol = 0.785398
    assert_geom(geom, surf, vol)
    geom.radius*=2; geom.height*=2
    assert_geom(geom, surf*4,vol*8, epsilon*8) 
    
def test_bezcurve():
    """ test creation of bezier curve """
    b = BezierCurve([(1,1,1,1),(2,2,2,1),(3,3,3,1),(4,4,4,1)])
    #b = BezierCurve(Point3Array([(1,1,1),(2,2,2),(3,3,3),(4,4,4)]))