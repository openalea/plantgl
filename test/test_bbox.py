from test_object_creation import *
from openalea.plantgl.all import *
import pytest

def bbox_application(geom,nbtest = 5):
    """ Simple test on Bounding Box Computation """
    d = Discretizer()
    b = BBoxComputer(d)
    testshape = isinstance(geom,Shape)
    for i in range(nbtest):
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
        ref = norm(refv)
        if ref  < 1e-5 : ref = 1
        dist = norm(b1.lowerLeftCorner-b2.lowerLeftCorner + b1.upperRightCorner - b2.upperRightCorner)/ref	
        if dist > 0.2 :
            if isinstance(geom, Shape):
                Scene([geom]).save('bboxerror.geom')
            else:
                Scene([Shape(geom,Material())]).save('bboxerror.geom')
            print(b1,b2,norm(b1.getSize()))
            cname = geom.__class__.__name__ if not testshape else geom.geometry.__class__.__name__
            raise Exception('Invalid BoundingBox Computation for object of type '+cname+' : '+str(dist))


@pytest.mark.parametrize('sceneobj', list(shapebenchmark_generator()))
def test_bbox_on_benchmark_objects(sceneobj):
    bbox_application(sceneobj)


def apply_bbox_on_objects():
    for t in test_bbox_on_default_object():
        pass
    for t in test_bbox_on_random_object():
        pass
    for t in test_bbox_on_random_shape():
        pass

if __name__ == '__main__':
    apply_bbox_on_objects()
