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

Color4_Color4 = Factory(name='Color4',
                authors='Thomas Cokelaer',
                description='',
                category='Unclassified',
                nodemodule='objectgenerator',
                nodeclass='Color4Node',
                inputs=[{'interface': ISequence, 'name': 'IN1', 'value': [150, 0, 0, 155], 'desc': ''}],
                outputs=[{'interface': None, 'name': 'OUT1', 'desc': ''}],
                widgetmodule=None,
                widgetclass=None,
               )
pkg.add_factory(Color4_Color4)

Color3_Color3 = Factory(name='Color3',
                authors='Thomas Cokelaer',
                description='',
                category='Unclassified',
                nodemodule='objectgenerator',
                nodeclass='Color3Node',
                inputs=[{'interface': ISequence, 'name': 'IN1', 'value': [0,150, 0], 'desc': ''}],
                outputs=[{'interface': None, 'name': 'OUT1', 'desc': ''}],
                widgetmodule=None,
                widgetclass=None,
               )
pkg.add_factory(Color3_Color3)
    
writer = package.PyPackageWriter(pkg)
fn = __file__
fn = path(fn)
fn = fn.abspath().dirname()/'__wralea__.py'
writer.write_wralea(fn)

