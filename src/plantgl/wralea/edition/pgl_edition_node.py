from openalea.core import *
from copy import deepcopy

class curve2D(Node):
    def __call__(self, inputs):
        """ A curve 2D """
        print 'eval'
        crv_in, crv_out = inputs[0], inputs[1]
        crv = crv_in if crv_in else crv_out
        crv = deepcopy(crv)
        self.set_input(1,crv)
        return crv,

def nurbs(surf):
    """ A curve 2D """
    from copy import deepcopy        
    return deepcopy(surf)

