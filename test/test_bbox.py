from test_object_creation import *
from openalea.plantgl.all import *

def bbox_application(geom,nbtest = 5,testshape = False):
    """ Simple test on Bounding Box Computation """
    d = Discretizer()
    b = BBoxComputer(d)
    for i in xrange(nbtest):
       if not isinstance(geom,Text) and not ((testshape and isinstance(geom.geometry,Text))):
        #b.clear() # a cache pb may occur sometimes.
        if not geom.apply(b):
            Scene([geom]).save('bboxerror.bgeom')
            assert False and "Application of BBoxComputer failed."
        b1 = b.result
        geom.apply(d)
        assert d.result.apply(b)
        b2 = b.result
        refv = b1.getSize()
        ref = 1
        if refv.x != 0: ref *= refv.x
        if refv.y != 0: ref *= refv.y
        if refv.z != 0: ref *= refv.z
        dist = norm(b1.lowerLeftCorner-b2.lowerLeftCorner + b1.upperRightCorner - b2.upperRightCorner)/ref	
        if dist > 0.1 :
            Scene([geom]).save('bboxerror.bgeom')
            print b1,b2,norm(b1.getSize())
            cname = geom.__class__.__name__ if not testshape else geom.geometry.__class__.__name__
            raise Exception('Invalid BoundingBox Computation for object of type '+cname+' : '+str(dist))

def test_bbox_on_default_object():
    for v in defaultobj_func_generator(bbox_application):
        yield v

def test_bbox_on_random_object():
    for v in randomobj_func_generator(bbox_application):
        yield v

def test_bbox_on_random_shape():
    for v in randomshape_func_generator(lambda x : bbox_application(x,testshape = True)):
        yield v

def apply_bbox_on_objects():
    for t in test_bbox_on_default_object():
        pass
    for t in test_bbox_on_random_object():
        pass
    for t in test_bbox_on_random_shape():
        pass

if __name__ == '__main__':
    apply_bbox_on_objects()
