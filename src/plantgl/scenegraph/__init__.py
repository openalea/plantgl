from _pglsg import *

import cspline
import bezier_nurbs
import warnings

NurbsCurve.CSpline = staticmethod(cspline.cspline)
NurbsCurve2D.CBezier = staticmethod(bezier_nurbs.cubic_bezier)

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


