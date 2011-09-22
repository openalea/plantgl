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

def test_obj():
    s = Scene()
    s.read('./data/icosahedron.obj')
    assert len(s) == 2
    assert s.isValid()
    s.read('./data/humanoid_tri.obj')
    assert len(s) == 3
    assert s.isValid()
    s.read('./data/trumpet.obj')
    assert len(s) == 0
    
