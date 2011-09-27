from openalea.plantgl.all import *
from openalea.plantgl.codec import obj

class DummyCodec (SceneCodec):
    def __init__(self):
        SceneCodec.__init__(self,"Test",SceneCodec.Mode.Read)
    def formats(self):
        return [SceneFormat('test',['test'],'A test codec')]
    def read(self,fname):
        s = Scene()
        s += Sphere()
        return s
    def write(self,fname,scene):
        pass

def test_codec():        
    t = DummyCodec()
    SceneFactory.get().registerCodec(t)
    s = Scene()
    s.read('toto.test','Test')
    assert type(s[0].geometry) == Sphere

def test_read_obj():
    s = Scene()
    s.read('./data/icosahedron.obj')
    assert len(s) == 1, len(s)
    assert s.isValid()
    s.read('./data/humanoid_tri.obj')
    assert len(s) == 2, len(s)
    assert s.isValid()
    s.read('./data/trumpet.obj')
    assert s.isValid()
    
def test_write_obj():
    s = Scene()
    s.read('./data/icosahedron.obj')
    s.save('./data/test_icosahedron.obj')
    s.clear()
    s.read('./data/test_icosahedron.obj')
    assert len(s) == 1, len(s)
    assert s.isValid()

    s.read('./data/humanoid_tri.obj')
    s.save('./data/test_humanoid_tri.obj')
    s.clear()
    s.read('./data/test_humanoid_tri.obj')
    assert len(s) == 2, len(s)
    assert s.isValid()
    s.read('./data/trumpet.obj')
    s.save('./data/test_trumpet.obj')
    s.clear()
    s.read('./data/test_trumpet.obj')
    assert s.isValid()
