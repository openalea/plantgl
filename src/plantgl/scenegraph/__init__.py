from _pglsg import *
import cspline

NurbsCurve.CSpline = staticmethod(cspline.cspline)