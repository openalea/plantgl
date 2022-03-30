from openalea.core import *
from . import objectgenerator

__name__  = "vplants.plantgl.objects"
__alias__ = ["PlantGL.Objects"]

__version__ = '0.0.1'
__license__ = 'CECILL-V2'
__authors__ = 'F. Boudon, C. Pradal'
__institutes__ = 'INRIA/CIRAD'
__description__ = 'PlantGL Object Generator nodes.'
__url__ =  'http://openalea.gforge.inria.fr'

__all__ = ['scene']



def generate_factory(name, **kargs):
    return Factory( name= name, 
                    description= "Create a "+name, 
                    category = "scene.PGL Object Generator", 
                    nodemodule = "objectgenerator",
                    nodeclass = name+'Node',
                    **kargs) 


scene = generate_factory("Scene")

for x in objectgenerator.PGLCLASS:
    name = x.__name__
    locals()[name] = generate_factory(name)
    __all__.append(name)

    
