from openalea.core import *

__name__ = "vplants.plantgl.operator"
__alias__= ["Geometric Operator"]

__version__ = '0.0.1',
__license__ = 'CECILL-V2',
__authors__ = 'C. Pradal',
__institutes__ = 'INRIA/CIRAD',
__description__ = 'PlantGL Object Generator nodes.',
__url__ =  'http://openalea.gforge.inria.fr'

__all__ = ['random_sample', 'regular_sample','planter', 'import_scene']

random_sample = Factory( name= "random sample", 
                         description= "Create random distribution", 
                         category = "Stand,scene", 
                         nodemodule = "frog",
                         nodeclass = "RandomSample",
                         )

regular_sample = Factory( name= "regular sample", 
                          description= "Create regular distribution", 
                          category = "Stand,scene", 
                          nodemodule = "frog",
                          nodeclass = "RegularSample",
                          )

planter = Factory( name= "planter", 
                   description= "Apply a set of transformation to a shape", 
                   category = "Visualization,scene", 
                   nodemodule = "frog",
                   nodeclass = "Planter",
                   )
    
import_scene = Factory( name= "import scene", 
                        description= "Import GEOM scene", 
                        category = "Visualization,scene,IO", 
                        nodemodule = "frog",
                        nodeclass = "ImportScene",
                        )
