from openalea.plantgl.all import *
from math import *
import numpy

s = 30
f1 = NurbsCurve2D (
  ctrlPointList= [ (0,30,1), (20,20,1), (20, 30,1), (0,40,1) ],
  stride= s,
  )

f2 = NurbsCurve2D (
  ctrlPointList= [ (0,30,1), (40,50,1), (40, 60,1), (0,40,1) ],
  stride= s
)

f3 = NurbsCurve2D (
  ctrlPointList= [ (0,30,1), (20,30,1), (20,40,1), (0,40,1) ],
  stride= s
)

f4 = NurbsCurve2D (
  ctrlPointList= [ (0,30,1), (40,40,1), (40,50,1), (0,40,1) ],
  stride= s
)

f5 = NurbsCurve2D (
  ctrlPointList= [ (0,30,1), (20,25,1), (20,35,1), (0,40,1) ],
  stride= s
)


profileList = [ f1, f5, f3,  f4,  f2,  f4,  f3,  f5,  f1 ]
angles = list(map(radians,[  0, 45, 90, 135, 180, 225, 270, 315, 360 ]))

def set_width(c,w=5) :
    c.width=w
    return c

profileList = list(map(set_width,profileList))

class NurbsSwung:
    def __init__(self, profileList, angleList, degree = 3, ccw = True, slices = 30, stride = 30):
        assert degree in [1,3]
        self.profileList = profileList
        self.angleList = angleList
        self.degree = degree
        self.ccw = ccw
        self.slices = slices
        self.stride = stride

        self.build_interpolator()

    def build_interpolator(self):
        from openalea.plantgl.scenegraph.cspline import CSpline, cspline
        if self.degree > 1:
            #cpoints = [discretize(NurbsCurve([Vector4(p.x,p.y,a,p.z) for p in s.ctrlPointList])).pointList for s,a in zip(self.profileList, self.angleList)]
            cpoints = [[Vector3(p.x,p.y,a) for p in s.ctrlPointList] for s,a in zip(self.profileList, self.angleList)]
            self.cnurbs = [cspline([cpoints[i][j] for i in range(len(cpoints))],degree=self.degree) for j in range(len(cpoints[0]))]
            cpoints = [list(n.ctrlPointList) for n in self.cnurbs]
        else:
            cpoints = [[Vector4(p.x,p.y,a,p.z) for p in s.ctrlPointList] for s,a in zip(self.profileList, self.angleList)]
        knots =  None #[self.angleList[0] for i in range(degree)]+self.angleList+[self.angleList[-1] for i in range(degree)]
        print( cpoints)
        for l in cpoints: print(len(l))
        self.profileInterpolator = NurbsPatch(cpoints, ccw=self.ccw, vstride=self.stride, ustride=self.slices, vknotList=self.cnurbs[0].knotList if self.degree > 1 else None, udegree=self.degree)
        print(self.profileInterpolator.vknotList)

    def getPointAt(self, u, v):
        result = self.profileInterpolator.getPointAt(u,v)
        return Vector3(result.x*cos(v), result.x*sin(v), result.y)
    def discretize(self):
        #return self.profileInterpolator
        mesh = discretize(self.profileInterpolator)
        mesh.pointList = [Vector3(p.x*cos(p.z),p.x*sin(p.z), p.y) for p in mesh.pointList]
        return mesh


def plot_surface(degree = 3):
    t1 = Swung (
     profileList = profileList,
     angleList   = angles,

     slices = 100,
     degree = degree,
     stride= 60,
     ccw = True
    )

    t3 = NurbsSwung (
     profileList = profileList,
     angleList   = angles,

     slices = 100,
     degree = degree,
     stride= 60,
     ccw = True if degree > 1 else False
    )
    tr = 60

    Viewer.display(Scene([t1, Translated((0,tr,0),Scaled(1,1,1,t3.discretize()))]+
        [Shape(Translated((0,tr,0),AxisRotated((1,0,0),pi/2,AxisRotated((0,1,0),a,set_width(c)))),Material((0,255,0))) for a,c in zip(t3.angleList, t3.profileList)]))


if __name__ == '__main__':
    import sys
    degree = 3
    if len(sys.argv) > 1:
        degree = int(sys.argv[1])
    plot_surface(degree)