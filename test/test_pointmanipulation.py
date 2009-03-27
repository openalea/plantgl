from openalea.plantgl.all import *
from random import uniform

pointrange = (0,100)

def random_point() : return Vector3(uniform(*pointrange),uniform(*pointrange),uniform(*pointrange))

def test_compress_point3():
  nbpoint = 500
  radius = 10
  p3list = [random_point() for i in xrange(nbpoint)]
  
  p3compress = compress_point3(p3list, radius)
  assert len(p3list) == len(p3compress)
  
test_compress_point3()