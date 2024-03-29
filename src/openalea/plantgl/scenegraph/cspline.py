###############################################################################
#
# License CECILL
# Author: C. Pradal
# Copyright: CIRAD
#
###############################################################################

"""
Class Cardinal Spline.
A cardinal spline is a spline passing through a set of points.
"""

import os, math
from . import _pglsg as pgl
from openalea.plantgl.math import norm

__metaclass__ = type

epsilon= 0.001


class CSpline:
    """
    A CSpline interpolate a set of points.
    """
    def __init__(self, points, is_closed=False):
        """
        Create a CSpline from a set of 2d or 3d points.
        """
        if len(points) != 0:
            self.dim = len(points[0])
            if self.dim == 2:
                self.points = pgl.Point2Array(points)
            elif self.dim == 3:
                self.points = pgl.Point3Array(points)
            elif self.dim == 4:
                self.points = pgl.Point4Array(points)
        else:
            self.dim = 3
            self.points = points
        self.is_closed = is_closed
        if is_closed:
            if norm(points[-1]-points[0]) > epsilon:
                self.points.append(points[0])

        self.nurbs= None
        self.dist = None
        self.der = None

    def __len__(self):
        return len(self.points)

    def distances(self):
        """
        Compute the distance between the input points.
        """
        n = len(self)
        self.dist = []
        for i in range(n-1):
            p, q = self.points[i],self.points[i+1]
            self.dist.append(norm(q-p))

    def derivatives(self):
        """
        Compute the derivatives based on the knots and the distance.
        At Pi, the derivative is:
            D_i = P_(i-1)P_i / 4||.|| + P_iP_(i+1) / 4||.||
        """
        deriv = self._derivative1

        n = len(self.points)
        d0 = (self.points[1]-self.points[0])/ (2.)
        dn = (self.points[-1]-self.points[-2])/ (2.)
        if self.is_closed:
            d0 = dn = deriv(self.points[-1], self.points[0], self.points[1], self.dist[-1], self.dist[0])

        self.der = [d0]

        for i in range(1,n-1):
            p, q, r = self.points[i-1], self.points[i], self.points[i+1]
            dx1, dx2 = self.dist[i-1], self.dist[i]
            der = self._derivative1(p,q,r,dx1,dx2)
            self.der.append(der)

        self.der.append(dn)

    def _derivative1( self, p, q, r, d0, d1):
        return (r-p)/2.

    def _derivative2( self, p, q, r, d0, d1):
        d= d0+d1
        d0 /= d
        d1 /= d
        der = (q-p)/(2*d0)+(r-q)/(2*d1)
        return der

    def bezier_cp(self, degree = 3):
        """
        Compute bezier control points from the input points.
        """
        if degree == 1:
            self.ctrl_pts = self.points
        elif degree == 2:
            a = 1./2

            n = len(self)
            self.ctrl_pts = [0]*(3*n-2)
            for i in range(0,n-1):
                p, q = self.points[i:i+2]
                dp, dq = self.der[i:i+2]
                self.ctrl_pts[i*3]   = p
                self.ctrl_pts[i*3+1] = p+dp*a
                self.ctrl_pts[i*3+2] = q-dq*a
            # last point
            self.ctrl_pts[-1]=self.points[-1]
            if self.is_closed:
                self.ctrl_pts[1] = self.ctrl_pts[2]
                self.ctrl_pts[-2] = self.ctrl_pts[-3]

        elif degree == 3:
            a = 1./3

            n = len(self)
            self.ctrl_pts = [0]*(3*n-2)
            for i in range(0,n-1):
                p, q = self.points[i:i+2]
                dp, dq = self.der[i:i+2]
                self.ctrl_pts[i*3]   = p
                self.ctrl_pts[i*3+1] = p+dp*a
                self.ctrl_pts[i*3+2] = q-dq*a
            # last point
            self.ctrl_pts[-1]=self.points[-1]
            if self.is_closed:
                self.ctrl_pts[1] = self.ctrl_pts[2]
                self.ctrl_pts[-2] = self.ctrl_pts[-3]

    def bezier_kv(self, is_linear=False, degree = 3):
        """
        Compute a nurbs knot vector from Bezier control points.
        bezier_kv(linear=False) -> knot_vector

        :param: is_linear indicate if the parametrization is linear or
        pseudo curvilinear abscisse.
        """
        nb_pts = len(self.ctrl_pts )
        nb_arc = len(self)-1
        assert nb_arc == (nb_pts-1) / degree

        nb_knots = degree + nb_pts
        p = 0.
        param = [p]

        if is_linear:
            param = list(map(float,list(range(nb_arc+1))))
            self.kv= [ param[ 0 ] ]*(degree+1)
            step = (param[-1]-param[0]) / float(nb_arc)
            for i in range(1,nb_arc):
                self.kv.extend([param[0]+i*step]*degree)
            self.kv.extend( [param[ -1 ]]*(degree+1) )
        else:
            dist = self.dist
            assert len(dist) == nb_arc
            for d in dist:
                p += d
                param.append( p )

                self.kv= [ param[ 0 ] ]
                for p in param:
                    for j in range(degree):
                        self.kv.append(p)
                self.kv.append(param[-1])

    def add_point( self, pt ):
        if self.is_closed:
            raise Exception("Unable to add a point to a closed curve")

        self.points.append(pt)
        # TODO: Compute incrementally the curve.
        self.nurbs = None

    def move_point( self, i, pt ):
        if i >= len(self):
            raise Exception("Index %d out of range [0,%d[" % (i, len(self)))

        self.points[i] = pt
        # TODO: Compute incrementally the curve.
        self.nurbs = None

    def curve(self, is_linear= False, degree = 3, stride_factor=10, distances=None):
        """
        Return the equivalent PlantGL nurbs curve which interpol the points.
        :param: stride_factor is the number of points to draw an arc of the curve.
        """
        if not self.nurbs:
            if distances is None:
                self.distances()
            else:
                self.dist = distances
            self.derivatives()
            self.bezier_cp()
            self.bezier_kv(is_linear, degree)

        pts = self.points
        kv = pgl.RealArray(self.kv)

        stride = (len(pts)-1) * stride_factor
        if self.dim == 3:
            ctrl_pts = pgl.Point4Array(self.ctrl_pts,1)
            self.nurbs = pgl.NurbsCurve( ctrl_pts, kv, 3, stride)
        elif self.dim == 2:
            ctrl_pts = pgl.Point3Array(self.ctrl_pts,1)
            self.nurbs = pgl.NurbsCurve2D( ctrl_pts, kv, 3, stride)
        else:
            raise Exception("Unable to build a spline curve from points of dimension %d"% (self.dim,))

        return self.nurbs

###############################################################################

# factory function
def cspline(pts, is_closed=False,is_linear=False,distances=None, degree = 3):
    """
    Build a nurbs curve by interpolate (C1) the points pts.
    The resulting curve can be closed.
    """
    spline = CSpline(pts,is_closed)
    return spline.curve(is_linear, degree=degree, distances=distances)


###############################################################################

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

