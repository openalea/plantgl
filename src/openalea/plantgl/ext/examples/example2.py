#!/usr/bin/env python
"""example2.py

Showing the posible usage of absoute_shape and
its' usage in interactive mode.

:version: pon mar 26 11:32:04 CEST 2007

:author:  szymon stoma
"""

#global
import openalea.plantgl.all as pgl
import openalea.plantgl.ext.all as pd
import math
import random

#local 
#import plantgl.all as pgl
#import plantgl.ext.all as pd


s1 = pd.AISphere( radius=10 )

s2 = pd.AISphere( pos=pgl.Vector3(0,0,15), radius=8 )

s3 = pd.AISphere( pos=pgl.Vector3(0,0,25.8), radius=5.4 )

marchewa = pd.AIArrow( pos=pgl.Vector3(5.2,0,25.8), axis=pgl.Vector3.OX, height=4, material=pd.red, radius=0.5)

oko1 = pd.AICylinder( pos=pgl.Vector3( 4., 2, 26.8 ), axis=pgl.Vector3.OX,  material=pd.black )

oko2 = pd.AICylinder( pos=pgl.Vector3( 4., -2, 26.8 ), axis=pgl.Vector3.OX, material=pd.black )

cylinder_basis = pd.AICylinder( pos=pgl.Vector3( 0., 0., 29. ), axis=pgl.Vector3.OZ, height=0.2, radius=10,  material=pd.black )

cylinder_top = pd.AICylinder( pos=pgl.Vector3( 0., 0., 29.1 ), axis=pgl.Vector3.OZ, radius=5, height=8, material=pd.black )

button1 = pd.AISphere( pos=pgl.Vector3( 10., 0., 0. ), radius=0.5, material=pd.black )

button2 = pd.AISphere( pos=pgl.Vector3( math.sqrt(10*10-7.5*7.5), 0., 7.5 ), radius=0.5, material=pd.black )

button3 = pd.AISphere( pos=pgl.Vector3( 8., 0., 15. ), radius=0.5, material=pd.black )

broom = pd.AICylinder( pos=pgl.Vector3( 0., 7., 0. ), radius=0.5, material=pd.black, axis=pgl.Vector3(0, 1, 5), height=20)

broom_top = []
broom_top_base_center = pgl.Vector3(0, 7+math.sqrt(400./26), 5*math.sqrt(400./26))
pd.set_instant_update_visualisation_policy( policy=False )
for i in range(100):
    x = (random.random() - random.random())*0.4
    y = (random.random() - random.random())*0.4
    broom_top = pd.AICylinder( pos=broom_top_base_center, radius=0.05, material=pd.black, axis=pgl.Vector3(0+x, 1+y, 5*random.random()), height=5)
pd.set_instant_update_visualisation_policy( policy=True )

pgl.Viewer.frameGL.setBgColor(220,220,255)

