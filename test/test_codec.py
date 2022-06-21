from openalea.plantgl.all import *
from openalea.plantgl.codec import obj
import os
from test_object_creation import *
import pytest

def get_filename(fname):
    import os
    return os.path.join(os.path.dirname(__file__), 'data', fname)


class DummyCodec (SceneCodec):
    def __init__(self):
        SceneCodec.__init__(self,"Test",SceneCodec.Mode.Read)
    def test(self, fname, format):
        return True
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
    fname = 'toto.test'
    with open(fname,'w') as f:
        pass
    s.read(fname,'Test')
    assert type(s[0].geometry) == Sphere
    s = Scene(fname)
    assert type(s[0].geometry) == Sphere
    os.remove(fname)

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

    
def test_init_from_obj():
    s = Scene(get_filename('teapot.obj'))
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

def test_bgeom():
    g = Scene([Group([Sphere(),Box()])])
    g2 = frombinarystring(tobinarystring(g))
    assert g2.isValid() and len(g) == len(g2)

    g = Scene([Group([Group([Sphere()]),Box()])])
    g2 = frombinarystring(tobinarystring(g))
    assert g2.isValid() and len(g) == len(g2)

def binary_str_benchmark(sceneobj):
    print(sceneobj)
    sceneobj2 = frombinarystring(tobinarystring(Scene([sceneobj])))
    assert sceneobj2.isValid() 

@pytest.mark.parametrize('sceneobj', list(shapebenchmark_generator()))
def test_binary_str_benchmark(sceneobj):
    binary_str_benchmark(sceneobj)



if __name__ == '__main__':
    for t in list(shapebenchmark_generator()):
        print(t)
        binary_str_benchmark(t)
