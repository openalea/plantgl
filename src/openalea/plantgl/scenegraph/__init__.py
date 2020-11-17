import openalea.plantgl.math
from ._pglsg import *

from . import cspline
from . import bezier_nurbs
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


""" Copy functions for Curve2D types """
def __nbc2D_copy__(self):
    res = NurbsCurve2D(self.ctrlPointList,self.degree, self.knotList, self.stride,self.width)
    if self.isNamed() : res.name = self.name
    return res

NurbsCurve2D.__copy__ = __nbc2D_copy__
del __nbc2D_copy__

def __nbc2D_deepcopy__(self,memo):
    from copy import deepcopy
    res = NurbsCurve2D(deepcopy(self.ctrlPointList,memo),self.degree, deepcopy(self.knotList,memo), self.stride,self.width)
    if self.isNamed() : res.name = self.name
    return res

NurbsCurve2D.__deepcopy__ = __nbc2D_deepcopy__
del __nbc2D_deepcopy__

def __bez2D_copy__(self):
    res = BezierCurve2D(self.ctrlPointList, self.stride,self.width)
    if self.isNamed() : res.name = self.name
    return res

BezierCurve2D.__copy__ = __bez2D_copy__
del __bez2D_copy__

def __bez2D_deepcopy__(self, memo):
    from copy import deepcopy
    res = BezierCurve2D(deepcopy(self.ctrlPointList,memo), self.stride,self.width)
    if self.isNamed() : res.name = self.name
    return res

BezierCurve2D.__deepcopy__ = __bez2D_deepcopy__
del __bez2D_deepcopy__

def __pol2D_copy__(self):
    res = Polyline2D(self.pointList, self.width)
    if self.isNamed() : res.name = self.name
    return res

Polyline2D.__copy__ = __pol2D_copy__
del __pol2D_copy__

def __pol2D_deepcopy__(self,memo):
    from copy import deepcopy
    res = Polyline2D(deepcopy(self.pointList,memo), self.width)
    if self.isNamed() : res.name = self.name
    return res

Polyline2D.__deepcopy__ = __pol2D_deepcopy__
del __pol2D_deepcopy__

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
    if PGL_QT_VERSION == 5:
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

from .editablequantisedfunction import *

from . import __docufy
