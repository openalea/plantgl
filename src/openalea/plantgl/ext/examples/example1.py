#!/usr/bin/env python
"""example1.py

Test of  ext  interactive_mode.

TODO: add assets.

:version: pon mar 19 17:29:11 CET 2007

:author:  szymon stoma
"""

#global
import openalea.plantgl.all as pgl
import openalea.plantgl.ext.all as pd

#local 
#import plantgl.all as pgl
#import plantgl.ext.all as pd

a0 = pd.AISphere()
a0.pos = pgl.Vector3(1,0,1)
a0.axis=pgl.Vector3(1,0,1)
a0.roll=30
a0.radius=0.7
a0.height=15
a0.material=pd.green
a0.visible = False
del a0

a0 = pd.AICylinder()
a0.pos = pgl.Vector3(1,0,1)
a0.axis=pgl.Vector3(1,0,1)
a0.roll=30
a0.radius=0.7
a0.height=15
a0.material=pd.green
a0.visible = False
del a0

a0 = pd.AICenterPolygon( points=[pgl.Vector3(1,1,0), pgl.Vector3(1,-1,0),pgl.Vector3(-1,-1,0),pgl.Vector3(-1,1,0)] )
a0.pos = pgl.Vector3(1,0,1)
a0.axis=pgl.Vector3(1,0,1)
a0.roll=30
a0.radius=0.7
a0.height=15
a0.material=pd.green
a0.visible = False
del a0

a0 = pd.AITriangle( points=[pgl.Vector3(1,1,0), pgl.Vector3(1,-1,0),pgl.Vector3(-1,-1,0)] )
a0.pos = pgl.Vector3(1,0,1)
a0.axis=pgl.Vector3(1,0,1)
a0.roll=30
a0.radius=0.7
a0.height=15
a0.material=pd.green
a0.visible = False
del a0

a0 = pd.AIArrow()
a0.pos = pgl.Vector3(1,0,1)
a0.axis=pgl.Vector3(1,0,1)
a0.roll=30
a0.radius=0.7
a0.height=15
a0.material=pd.green

