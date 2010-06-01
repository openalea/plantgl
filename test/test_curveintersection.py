from openalea.plantgl.all import *

def check_curve_redundancy(curve):
    lp = curve.pointList[0]
    for i,p in enumerate(curve.pointList):
        if i == 0: continue
        if norm(p-lp) < 1e-5:
            print 'Point',i-1,'and',i,'are similars',p,lp
        lp = p
    
def test_intersection():    
    line = Polyline2D(Point2Array([Vector2(-2.33948e-07,7.62939e-06),Vector2(-2.33948e-07,10.3269),Vector2(-0.364881,26.1375),Vector2(-0.364866,37.844),Vector2(-0.364881,50.9286),Vector2(-0.364866,64.701),Vector2(-0.364881,77.7856),Vector2(-0.364881,90.1812),Vector2(-0.364881,102.577),Vector2(-0.608216,114.283),Vector2(-0.608216,125.304),Vector2(-0.608231,135.635),Vector2(-0.608216,144.588),Vector2(-0.608231,154.92),Vector2(-0.608231,154.92),Vector2(-0.678083,159.971),Vector2(-0.812783,165.122),Vector2(-1.00971,170.337),Vector2(-1.26621,175.576),Vector2(-1.57967,180.802),Vector2(-1.94744,185.978),Vector2(-2.36691,191.066),Vector2(-2.83542,196.027),Vector2(-3.35037,200.824),Vector2(-3.90908,205.419),Vector2(-4.50897,209.775),Vector2(-5.14739,213.853),Vector2(-5.82169,217.617),Vector2(-6.52924,221.027),Vector2(-7.26744,224.046),Vector2(-8.03361,226.637),Vector2(-8.82516,228.761),Vector2(-9.63943,230.382),Vector2(-10.4738,231.46),Vector2(-11.3256,231.959)]))
    check_curve_redundancy(line)
    print CurveIntersection.compute([line])