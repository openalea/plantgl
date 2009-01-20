from test_object_creation import create_random_objects
from openalea.plantgl.all import *

def test_bbox_object(nbtest = 5):
    """ Simple test on Bounding Box Computation """
    d = Discretizer()
    b = BBoxComputer(d)
    for i in xrange(nbtest):
      for geom in create_random_objects():
       if not isinstance(geom,Text):
        #b.clear() # a cache pb may occur sometimes.
        assert geom.apply(b)
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
            raise Exception('Invalid BoundingBox Computation for object of type '+geom.__class__.__name__+' : '+str(dist))

if __name__ == '__main__':
    test_bbox_object()
