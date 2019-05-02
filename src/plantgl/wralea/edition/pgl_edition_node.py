from openalea.core import *
from copy import deepcopy
from pickle import loads, dumps
from openalea.plantgl.all import NurbsCurve2D

class curve2D(Node):
    def __call__(self, inputs):
        """ A curve 2D """
        crv_in, ctrl_points = inputs[0], inputs[1]
        crv_out = None
        if crv_in is None:
            if ctrl_points:
                pts = loads(ctrl_points)
                crv_out = NurbsCurve2D(pts)
        crv = crv_in if crv_in else crv_out

        if crv:
            pts = dumps(crv.ctrlPointList)
            self.set_input(1,pts)

        return crv,

def nurbs(surf):
    """ A curve 2D """
    from copy import deepcopy        
    return deepcopy(surf)

