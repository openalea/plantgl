from openalea.core import *
from pgl_interface import *
from pgl_interface_widget import *

__name__ = "vplants.plantgl.edition"

__version__ = '0.0.2'
__license__ = 'CECILL-V2'
__authors__ = 'F. Boudon'
__institutes__ = 'INRIA/CIRAD'
__description__ = 'PlantGL Edition nodes.'
__url__ =  'http://openalea.gforge.inria.fr'

__all__ = ['curve2d']

curve2d = Factory( name= "Curve2D", 
                  description= "Display and edition of a curve 2D", 
                  category = "Visualisation, Edition", 
                  nodemodule = "pgl_edition_node",
                  nodeclass = "curve2D",
                  widgetmodule = "pgl_interface_widget",
                  widgetclass = "Curve2DWidget",
                  inputs=(dict(name="curve", interface=ICurve2D,),),
                  outputs=(dict(name="curve", interface=ICurve2D,),),
                  lazy = False
                  )





