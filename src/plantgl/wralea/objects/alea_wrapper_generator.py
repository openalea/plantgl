from openalea.core import *
import objectgenerator
from openalea.core.path import path


name  = "vplants.plantgl.objects"

metainfo = dict(
alias = ["PlantGL.Objects"],
version = '0.0.2',
license = 'CECILL-V2',
authors = 'F. Boudon, C. Pradal',
institutes = 'INRIA/CIRAD',
description = 'PlantGL Object Generator nodes.',
url =  'http://openalea.gforge.inria.fr',
)

pkg = package.Package(name, metainfo)


#__all__ = ['scene']

def generate_factory(name, **kargs):
    return Factory( name= name, 
                    description= "Create a "+name, 
                    category = "scene.PGL Object Generator", 
                    nodemodule = "objectgenerator",
                    nodeclass = name+"Node",
                    **kargs) 


pkg.add_factory(generate_factory("Scene"))

for x in objectgenerator.PGLCLASS:
    name = x.__name__
    pkg.add_factory(generate_factory(name))


writer = package.PyPackageWriter(pkg)
fn = __file__
fn = path(fn)
fn = fn.abspath().dirname()/'__wralea__.py'
writer.write_wralea(fn)

