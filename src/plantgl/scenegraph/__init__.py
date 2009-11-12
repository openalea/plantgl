from _pglsg import *

import cspline
import warnings

NurbsCurve.CSpline = staticmethod(cspline.cspline)

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

# Hack to solve problems with static_property

Shape.NOID = Shape.NOID.fget()
Color3.BLACK = Color3.BLACK.fget()
Color3.BLUE= Color3.BLUE.fget()
Color3.CYAN= Color3.CYAN.fget()
Color3.GREEN= Color3.GREEN.fget()
Color3.RED= Color3.RED.fget()
Color3.WHITE= Color3.WHITE.fget()
Color3.YELLOW= Color3.YELLOW.fget()

Color4.BLACK = Color4.BLACK.fget()
Color4.BLUE= Color4.BLUE.fget()
Color4.CYAN= Color4.CYAN.fget()
Color4.GREEN= Color4.GREEN.fget()
Color4.RED= Color4.RED.fget()
Color4.WHITE= Color4.WHITE.fget()
Color4.YELLOW= Color4.YELLOW.fget()


Material.DEFAULT_AMBIENT = Material.DEFAULT_AMBIENT.fget()
Material.DEFAULT_DIFFUSE = Material.DEFAULT_DIFFUSE.fget()
Material.DEFAULT_EMISSION = Material.DEFAULT_EMISSION.fget()
Material.DEFAULT_MATERIAL= Material.DEFAULT_MATERIAL.fget()
Material.DEFAULT_SHININESS= Material.DEFAULT_SHININESS.fget()
Material.DEFAULT_SPECULAR = Material.DEFAULT_SPECULAR.fget()
Material.DEFAULT_TRANSPARENCY = Material.DEFAULT_TRANSPARENCY.fget()

BezierPatch.DEFAULT_CCW = BezierPatch.DEFAULT_CCW.fget()
BezierPatch.DEFAULT_STRIDE = BezierPatch.DEFAULT_STRIDE.fget()

Text.DEFAULT_FONT = Text.DEFAULT_FONT.fget()
Text.DEFAULT_POSITION = Text.DEFAULT_POSITION.fget()
Text.DEFAULT_SCREEN_COORDINATES = Text.DEFAULT_SCREEN_COORDINATES.fget()

Oriented.DEFAULT_PRIMARY = Oriented.DEFAULT_PRIMARY.fget()
Oriented.DEFAULT_SECONDARY = Oriented.DEFAULT_SECONDARY.fget() 

AxisRotated.DEFAULT_ANGLE = AxisRotated.DEFAULT_ANGLE.fget()
AxisRotated.DEFAULT_AXIS = AxisRotated.DEFAULT_AXIS.fget()


