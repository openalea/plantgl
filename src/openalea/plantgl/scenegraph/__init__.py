import openalea.plantgl.math
from ._pglsg import *

from . import cspline
from . import bezier_nurbs
from .nurbspatch_nd import NurbsPatch3D
from .nurbsshape import *

import warnings
from .colormap import *

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
    newFunc.__deprecated__ = True
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

Extrusion.scale = property(__extrusion_get_scale,__extrusion_set_scale, doc='DEPRECATED')

@deprecated
def __extrusion_get_orientation(extrusion):
    return extrusion.orientationList

@deprecated
def __extrusion_set_orientation(extrusion,value):
    extrusion.orientationList = value

Extrusion.orientation = property(__extrusion_get_orientation,__extrusion_set_orientation, doc='DEPRECATED')

def __extsn_copy__(self):
    res = Extrusion(self.axis,self.crossSection, self.scaleList, self.orientationList, self.knotList, self.solid, self.ccw, self.initialNormal)
    if self.isNamed() : res.name = self.name
    return res

Extrusion.__copy__ = __extsn_copy__
del __extsn_copy__

def __extsn_deepcopy__(self,memo):
    from copy import deepcopy
    res = Extrusion(deepcopy(self.axis),deepcopy(self.crossSection), deepcopy(self.scaleList), deepcopy(self.orientationList), deepcopy(self.knotList), self.solid, self.ccw, self.initialNormal)
    if self.isNamed() : res.name = self.name
    return res

Extrusion.__deepcopy__ = __extsn_deepcopy__
del __extsn_deepcopy__


""" Copy functions for Curve and Curve2D types """
def __nbc_copy__(self):
    res = type(self)(self.ctrlPointList,self.degree, self.knotList, self.stride,self.width)
    if self.isNamed() : res.name = self.name
    return res

NurbsCurve2D.__copy__ = __nbc_copy__
NurbsCurve.__copy__ = __nbc_copy__
del __nbc_copy__

def __nbc_deepcopy__(self,memo):
    from copy import deepcopy
    res = type(self)(deepcopy(self.ctrlPointList,memo),self.degree, deepcopy(self.knotList,memo), self.stride,self.width)
    if self.isNamed() : res.name = self.name
    return res

NurbsCurve2D.__deepcopy__ = __nbc_deepcopy__
NurbsCurve.__deepcopy__ = __nbc_deepcopy__
del __nbc_deepcopy__

def __bez_copy__(self):
    res = type(self)(self.ctrlPointList, self.stride,self.width)
    if self.isNamed() : res.name = self.name
    return res

BezierCurve2D.__copy__ = __bez_copy__
BezierCurve.__copy__ = __bez_copy__
del __bez_copy__

def __bez_deepcopy__(self, memo):
    from copy import deepcopy
    res = type(self)(deepcopy(self.ctrlPointList,memo), self.stride,self.width)
    if self.isNamed() : res.name = self.name
    return res

BezierCurve2D.__deepcopy__ = __bez_deepcopy__
BezierCurve.__deepcopy__ = __bez_deepcopy__
del __bez_deepcopy__

def __pol_copy__(self):
    res = type(self)(self.pointList, self.width)
    if self.isNamed() : res.name = self.name
    return res

Polyline2D.__copy__ = __pol_copy__
Polyline.__copy__ = __pol_copy__
del __pol_copy__

def __pol_deepcopy__(self,memo):
    from copy import deepcopy
    res = type(self)(deepcopy(self.pointList,memo), self.width)
    if self.isNamed() : res.name = self.name
    return res

Polyline2D.__deepcopy__ = __pol_deepcopy__
Polyline.__deepcopy__ = __pol_deepcopy__
del __pol_deepcopy__

""" Copy functions for Patch types """
def __bzpth_copy__(self):
    res = BezierPatch(self.ctrlPointMatrix, self.ustride, self.vstride, self.ccw)
    if self.isNamed() : res.name = self.name
    return res

BezierPatch.__copy__ = __bzpth_copy__
del __bzpth_copy__

def __bzpth_deepcopy__(self,memo):
    from copy import deepcopy
    res = BezierPatch(deepcopy(self.ctrlPointMatrix,memo), self.ustride, self.vstride, self.ccw)
    if self.isNamed() : res.name = self.name
    return res

BezierPatch.__deepcopy__ = __bzpth_deepcopy__
del __bzpth_deepcopy__

def __nbpth_copy__(self):
    res = NurbsPatch(self.ctrlPointMatrix,self.udegree, self.vdegree, self.uknotList, self.vknotList, self.ustride, self.vstride, self.ccw)
    if self.isNamed() : res.name = self.name
    return res

NurbsPatch.__copy__ = __nbpth_copy__
del __nbpth_copy__

def __nbpth_deepcopy__(self,memo):
    from copy import deepcopy
    res = NurbsPatch(deepcopy(self.ctrlPointMatrix,memo),self.udegree, self.vdegree, deepcopy(self.uknotList,memo),deepcopy(self.vknotList,memo), self.ustride,self.vstride, self.ccw)
    if self.isNamed() : res.name = self.name
    return res

NurbsPatch.__deepcopy__ = __nbpth_deepcopy__
del __nbpth_deepcopy__

def __group_copy__(self):
    from copy import copy
    res = Group([copy(geom) for geom in self.geometryList])
    if self.skeleton  : res.skeleton = copy(self.skeleton)
    if self.isNamed() : res.name = self.name
    return res

Group.__copy__ = __group_copy__
del __group_copy__

def __group_deepcopy__(self,memo):
    from copy import deepcopy
    res = Group([deepcopy(geom,memo) for geom in self.geometryList])
    if self.skeleton  : res.skeleton = deepcopy(self.skeleton,memo)
    if self.isNamed() : res.name = self.name
    return res

Group.__deepcopy__ = __group_deepcopy__
del __group_deepcopy__

def __group_getinitargs__(self):
    if not self.skeleton: return list(self.geometryList),
    return list(self.geometryList), self.skeleton

Group.__getinitargs__ = __group_getinitargs__
del __group_getinitargs__

def __trans_getinitargs__(self):
    return self.translation, self.geometry

Translated.__getinitargs__ = __trans_getinitargs__
del __trans_getinitargs__


def __pol_getinitargs__(self):
    return list(self.pointList), self.width

Polyline2D.__getinitargs__ = __pol_getinitargs__
del __pol_getinitargs__


def _toQImage(self):
    """ Convert self into a QImage """
    from openalea.plantgl.config import PGL_QT_VERSION
    if PGL_QT_VERSION == 6:
        from PySide6.QtGui import QImage
    elif PGL_QT_VERSION == 5:
        from PyQt5.QtGui import QImage
    else:
        from PyQt4.QtGui import QImage
    npi = self.to_interlaced_array()
    width, height,  nbchannel = npi.shape
    bytesPerLine = nbchannel * width
    iformat = QImage.Format_RGB888
    if nbchannel == 4 : iformat = QImage.Format_RGBX8888
    return QImage(npi.data, width, height,  bytesPerLine, iformat)

Image.toQImage = _toQImage
del _toQImage

def _img_plot(self):
    """ plot self using maplotlib """
    import matplotlib.pyplot as plot
    plot.imshow(self.to_array())
    plot.show()

Image.plot = _img_plot
del _img_plot

def _col3iter(col):
    for v in (col.red, col.green, col.blue):
        yield v

Color3.__iter__ = _col3iter
del _col3iter

def _col4titer(col):
    for v in (col.red, col.green, col.blue, col.alpha):
        yield v

Color4.__iter__ = _col4titer
del _col4titer

from .editablequantisedfunction import *

def bbx_corners(bbx):
    from itertools import product
    from openalea.plantgl.math import Vector3
    
    lc = bbx.lowerLeftCorner
    uc = bbx.upperRightCorner
    return list(map(Vector3,product(*[(lc[i],uc[i]) for i in range(3)] )))

BoundingBox.corners = bbx_corners
del bbx_corners

from . import __docufy

