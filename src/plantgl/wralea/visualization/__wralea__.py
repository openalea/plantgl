from openalea.core import *


__name__ = "vplants.plantgl.visualization"
__alias__ = ["PlantGL.Visualization"]

__version__ = '0.0.2'
__license__ = 'CECILL-V2'
__authors__ = 'F. Boudon'
__institutes__ = 'INRIA/CIRAD'
__description__ = 'PlantGL Object Generator nodes.'
__url__ =  'http://openalea.gforge.inria.fr'

__all__ = ['plot3d']
        
plot3d = Factory( name= "plot3D", 
                  description= "Viewer Display", 
                  category = "Visualisation, plot", 
                  nodemodule = "viewernode",
                  nodeclass = "Plot3D",
                  #lazy = False
                  )


