from _pglsg import *

import cspline
import bezier_nurbs
import warnings

NurbsCurve.CSpline = staticmethod(cspline.cspline)
NurbsCurve2D.CBezier = staticmethod(bezier_nurbs.cubic_bezier2D)
NurbsCurve.CBezier = staticmethod(bezier_nurbs.cubic_bezier3D)

def deprecated(func):
    """This is a decorator which can be used to mark functions
    as deprecated. It will result in a warning being emmitted
    when the function is used."""
    def newFunc(*args, **kwargs):
        warnings.warn("Call to deprecated function %s." % func.__name__,
                      category=DeprecationWarning)
        return func(*args, **kwargs)
    newFunc.__name__ = func.__name__
    newFunc.__doc__ = func.__doc__
    newFunc.__dict__.update(func.__dict__)
    return newFunc

@deprecated
def __extrusion_get_scale(extrusion):
    return extrusion.scaleList

@deprecated
def __extrusion_set_scale(extrusion,value):
    extrusion.scaleList = value

Extrusion.scale = property(__extrusion_get_scale,__extrusion_set_scale)

@deprecated
def __extrusion_get_orientation(extrusion):
    return extrusion.orientationList

@deprecated
def __extrusion_set_orientation(extrusion,value):
    extrusion.orientationList = value

Extrusion.orientation = property(__extrusion_get_orientation,__extrusion_set_orientation)


""" Pickling functions for Curve2D types """
def __nbc2D_copy__(self):
    return NurbsCurve2D(self.ctrlPointList,self.degree, self.knotList, self.stride,self.width)

NurbsCurve2D.__copy__ = __nbc2D_copy__
del __nbc2D_copy__

def __nbc2D_deepcopy__(self,memo):
    from copy import deepcopy
    return NurbsCurve2D(deepcopy(self.ctrlPointList,memo),self.degree, deepcopy(self.knotList,memo), self.stride,self.width)

NurbsCurve2D.__deepcopy__ = __nbc2D_deepcopy__
del __nbc2D_deepcopy__

def __bez2D_copy__(self):
    return BezierCurve2D(self.ctrlPointList, self.stride,self.width)

BezierCurve2D.__copy__ = __bez2D_copy__
del __bez2D_copy__

def __bez2D_deepcopy__(self, memo):
    from copy import deepcopy
    return BezierCurve2D(deepcopy(self.ctrlPointList,memo), self.stride,self.width)

BezierCurve2D.__deepcopy__ = __bez2D_deepcopy__
del __bez2D_deepcopy__

def __pol2D_copy__(self):
    return Polyline2D(self.pointList, self.width)

Polyline2D.__copy__ = __pol2D_copy__
del __pol2D_copy__

def __pol2D_deepcopy__(self):
    from copy import deepcopy
    return Polyline2D(deepcopy(self.pointList,memo), self.width)

Polyline2D.__deepcopy__ = __pol2D_deepcopy__
del __pol2D_deepcopy__
