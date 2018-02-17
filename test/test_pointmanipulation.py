from openalea.plantgl.all import *
from random import uniform, seed

pointrange = (0,100)

def random_point():
  return Vector3(uniform(*pointrange),uniform(*pointrange),uniform(*pointrange))

def test_contract_point3():
  seed(1)
  nbpoint = 500
  radius = 10
  p3list = [random_point() for i in xrange(nbpoint)]

  p3compress = contract_point3(p3list, radius)
  assert len(p3list) == len(p3compress)

def dist_to_points(p, plist):
    return sum([norm(p-pi) for pi in plist])

def brute_force_pointset_median(plist):
    dist_list = [(i,dist_to_points(pi,plist)) for i,pi in enumerate(plist)]
    dist_list.sort(lambda x,y: cmp(x[1],y[1]))
    return dist_list[0][0]


def test_median_point():
   seed(1)
   nbpoint = 100
   p3list = Point3Array([random_point() for i in xrange(nbpoint)])
   i = pointset_median(p3list)
   j = brute_force_pointset_median(p3list)
   k = approx_pointset_median(p3list,100000)
   if i != j:
       raise ValueError(i,j,dist_to_points(p3list[i],p3list),dist_to_points(p3list[j],p3list),p3list[i],p3list[j])
   if k != j:
       raise ValueError(k,j,dist_to_points(p3list[i],p3list),dist_to_points(p3list[j],p3list),p3list[i],p3list[j])



if __name__ == '__main__':
    for i in xrange(50):
        test_median_point()


