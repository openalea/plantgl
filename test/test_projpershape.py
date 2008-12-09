from openalea.plantgl.all import *

def test_projpershape():
    sc = Scene()
    s = Shape(Sphere())
    s.id = 100
    sc += s
    s = Shape(Translated(1,2,1,Sphere()))
    s.id = 200
    sc += s
    Viewer.display(sc)    
    pixpershape, pixsize = Viewer.frameGL.getProjectionPerShape()
    print pixpershape, pixsize
    pixpershape = dict(pixpershape)
    assert len(pixpershape) == len(sc)
    keys1 = [sh.id for sh in sc]
    keys1.sort()
    keys2 = pixpershape.keys()
    keys2.sort()
    assert keys1 == keys2

def test_projpershape_nullid():
    s = Shape(Sphere())
    s.id = 0
    Viewer.display(s)    
    pixpershape, pixsize = Viewer.frameGL.getProjectionPerShape()    
    assert len(pixpershape) == 1
    assert pixpershape[0][0] == s.id
    
if __name__ == '__main__':
    test_projpershape()
    test_projpershape_nullid()