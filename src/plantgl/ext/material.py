#!/usr/bin/env python
"""Contains basic material definitions.

:version: pon mar 19 16:32:21 CET 2007

:author:  szymon stoma
"""
__docformat__="restructuredtext en"

import openalea.plantgl.all as pgl

white = pgl.Material( ambient=pgl.Color3(255,255,255),
               name="White",
               diffuse=1,
               specular=pgl.Color3(40,40,40),
               emission=pgl.Color3(0,0,0),
               shininess=1,
               transparency=0  )

red = pgl.Material( ambient=pgl.Color3(255,0,0),
               name="Red",
               diffuse=1,
               specular=pgl.Color3(40,40,40),
               emission=pgl.Color3(0,0,0),
               shininess=1,
               transparency=0  )
green = pgl.Material( ambient=pgl.Color3(0,255,0),
                 name="Green",
                 diffuse=1,
                 specular=pgl.Color3(40,40,40),
                 emission=pgl.Color3(0,0,0),
                 shininess=1,
                 transparency=0  )
blue = pgl.Material( ambient=pgl.Color3(0,0,255),
                 name="Blue",
                 diffuse=1,
                 specular=pgl.Color3(40,40,40),
                 emission=pgl.Color3(0,0,0),
                 shininess=1,
                 transparency=0  )

black = pgl.Material( ambient=pgl.Color3(0,0,0),
               name="Black",
               diffuse=1,
               specular=pgl.Color3(40,40,40),
               emission=pgl.Color3(0,0,0),
               shininess=1,
               transparency=0  )
yellow = pgl.Material( ambient=pgl.Color3(255,255,0),
               name="Yellow",
               diffuse=1,
               specular=pgl.Color3(40,40,40),
               emission=pgl.Color3(0,0,0),
               shininess=1,
               transparency=0  )
