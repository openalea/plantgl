from openalea.plantgl.all import *
import sys
import gc

if not pgl_support_extension('MAINTAIN_PYTHON_OBJECT_ID'):
    import warnings
    warnings.warn("Not supported extension. Skip tests.")
else:
  def test_consistant_sh(verbose = True):
    """ Test if the python image of a created shape stay consistant throw manipulation """
    s = Scene()
    sh = Shape(Sphere(),Material())
    if verbose:
        print(sh)
        print(sys.getrefcount(sh)-1,sh.getPglReferenceCount())
    s += sh
    if verbose:
        print(sys.getrefcount(sh)-1,sh.getPglReferenceCount(),sys.getrefcount(s[0])-1)
        print(s[0])
    assert sh is s[0] and "__getitem__ failed"
    if verbose:
        print(sys.getrefcount(sh)-1,sh.getPglReferenceCount(),sys.getrefcount(s[0])-1)
    shf = s.find(sh.id)
    if verbose:
        print(shf)
    assert sh is shf and "find failed"
    if verbose:
        print(sys.getrefcount(sh)-1,sys.getrefcount(shf)-1,sh.getPglReferenceCount())
    del shf
    if verbose:
        a,b,c = sys.getrefcount(sh)-1,sys.getrefcount(s[0])-1,sh.getPglReferenceCount()
        print(a,b,c)
    sh.toto = True
    assert s[0].toto == True and '"property transmission failed'
    del sh
    if verbose:
        a,b = sys.getrefcount(s[0])-1,s[0].getPglReferenceCount()
        print(a,b)
    assert s[0].toto == True and '"property persistence failed'
    del s

  def test_consistant_sh2():
    """ Test if the python image of a shape created in c++ stay consistant throw manipulation """
    s = Scene()
    s += Sphere()
    sh1 = s[0]
    sh2 = s[0]
    assert sh1 is sh2
    sh1.toto = True
    assert sh2.toto == True
    del sh1
    del sh2
    assert s[0].toto == True
    del s

  def test_consistant_sphere():
    """ Test if the python image of a sphere stay consistant throw manipulation """
    sp = Sphere()
    s = Scene()
    s += sp
    assert s[0] is s[0]
    assert s[0].geometry is s[0].geometry
    assert s[0].geometry is sp

  def test_consistant_extrusion():
    """ Test if the python image of a extrusion stay consistant throw manipulation """
    axis = Polyline([(0,0,0),(1,2,1),(0,5,3)])
    cross_section = Polyline2D.Circle(1,16)
    sp = Extrusion(axis,cross_section)
    assert sp.isValid()
    assert sp.axis is axis
    assert sp.crossSection is cross_section
    s = Scene()
    s += sp
    assert s[0] is s[0]
    assert s[0].geometry is s[0].geometry
    assert s[0].geometry is sp


    
  if __name__ == '__main__':
    test_consistant_sh()
    test_consistant_sh2()
    test_consistant_sphere()
    test_consistant_extrusion()    
