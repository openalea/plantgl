from openalea.plantgl.all import *
from openalea.plantgl.codec import obj

def get_filename(fname):
    import os
    return os.path.join(os.path.dirname(__file__), 'data', fname)


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
    s.read(get_filename('icosahedron.obj'))
    assert len(s) == 1, len(s)
    assert s.isValid()
    s.read(get_filename('humanoid_tri.obj'))
    assert len(s) == 2, len(s)
    assert s.isValid()
    s.read(get_filename('trumpet.obj'))
    assert s.isValid()
    
def test_write_obj():
    s = Scene()
    s.read(get_filename('icosahedron.obj'))
    s.save(get_filename('test_icosahedron.obj'))
    s.clear()
    s.read(get_filename('test_icosahedron.obj'))
    assert len(s) == 1, len(s)
    assert s.isValid()

    s.read(get_filename('humanoid_tri.obj'))
    s.save(get_filename('test_humanoid_tri.obj'))
    s.clear()
    s.read(get_filename('test_humanoid_tri.obj'))
    assert len(s) == 2, len(s)
    assert s.isValid()
    s.read(get_filename('trumpet.obj'))
    s.save(get_filename('test_trumpet.obj'))
    s.clear()
    s.read(get_filename('test_trumpet.obj'))
    assert s.isValid()
