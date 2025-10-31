from openalea.plantgl.all import *

def test_scene():
    scene = Scene()
    assert scene.isValid()


def test_default_material():
    mat = Material()
    assert mat.isValid()
    assert mat != mat.DEFAULT_MATERIAL
    assert mat.name == ''
    assert not mat.isNamed() 
    material_methods = """
ambient
diffuse
emission
specular
shininess
specular
transparency
"""
    methods = material_methods.split()
    for s in methods:
        expr = 'mat.%s == mat.DEFAULT_%s'%(s,s.upper())
        assert eval(expr), expr

def test_default_material1():
    # TODO: These materials are the same
    mat = Material()
    mat.name = 'DEFAULT_MATERIAL'
    assert mat != mat.DEFAULT_MATERIAL, 'mat =%s\nDEFAULT=%s'%( mat, mat.DEFAULT_MATERIAL)

def test_scene1():
    scene = Scene()
    sphere = Sphere()
    scene.add(sphere)
    
def test_scene2():
    scene = Scene()
    sphere = Sphere()
    scene += sphere
    assert scene.isValid()

def test_scene3():
    scene = Scene()
    sphere = Sphere()
    mat = Material()
    shape = Shape(sphere,mat)
    scene.add(shape)
    assert scene.isValid()

    