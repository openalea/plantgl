#!BPY
""" 
Name: 'PlantGL multi shp'
Blender: 244
Group: 'Import'
Tooltip: 'Import from PlantGL scene (.geom)'
"""

# -*- coding: utf-8 -*-
# -*- python -*-
#
#       Vplants.PlantGL
#
#       Copyright 2008-2009 INRIA - CIRAD - INRA
#
#       File author(s): Jerome Chopard <jerome.chopard.at.sophia.inria.fr>
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
#
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
###############################################################################

'''
This module import a PlantGL scene into blender
'''

__docformat__ = "restructuredtext"
__license__ = "Cecill-C"
__revision__ = " $Id: $ "


import Blender
from Blender import Window
from openalea.plantgl.scenegraph import Scene as PGLScene
from openalea.plantgl.codec.blender import sc_to_blender

def load_scene (filename) :
    sc = PGLScene()
    sc.read(filename)
    print("loaded",len(sc))
    
    return sc_to_blender(sc)

if __name__ == '__main__' :
    Window.FileSelector(load_scene, 'Import scene', '*.geom,*.bgeom')


