# -*- python -*-
#
#       OpenAlea.PlantGL.ext Library: PlantGL external Library module
#
#       Copyright 2006 INRIA - CIRAD - INRA  
#
#       File author(s): BOUDON Frederic <frederic.boudon@cirad.fr>
#                       Da SILVA David <david.da_silva@cirad.fr>
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
# 
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#


from openalea.core import *

def register_packages(pkg_manager):
    
    metainfo={ 'version' : '0.0.1',
               'license' : 'CECILL-C',
               'authors' : 'F. Boudon and D. Da Silva',
               'institutes' : 'INRIA/CIRAD',
               'description' : 'Spatial distribution module.',
               'url' : 'http://www.scipy.org'
               }
    
    
    package = Package("PlantGL.Dresser", metainfo)
    
    
###### begin nodes definitions #############

    nf = Factory( name="PGL Dresser",
                  description="Add Pgl geometry an object",
                  category="Modelling",
                  nodemodule="dresser",
                  nodeclass="dresser",
                  inputs= ( dict( name = "obj", interface=None ),
                            dict( name = "type", interface=IEnumStr(['AsymetricSwung', 'ChupaChups']), value = 'AsymetricSwung', showwidget=True ),
                            dict( name = "args", interface=IDict ),
                          ),
                  outputs=(dict(name="dresser function", interface = None),
                          ),
                  )

    package.add_factory( nf )

###### end nodes definitions ###############

    pkg_manager.add_package(package)
