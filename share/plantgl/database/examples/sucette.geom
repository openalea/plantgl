 # -Geom File-
(#--------------------------------------------------
 #  
 #  File : D:/Fred/Mes documents/Develop/AMAPmod/databases/MyGEOM/sucette2.geom
 #  
 #  A GEOM file generated with GEOM library
 #  Published under the GNU General Public Licence. 
 #  
 # --------------------------------------------------
 #)

AxisRotated a { 
    Axis <1,0,0>
    Angle -15
    Geometry Cylinder { 
        Radius 0.5
        Height 20
    }
}

Material DEFAULT_MATERIAL { 
    Ambient 81
    Diffuse 2.65432
}

Shape SHAPE_17799964 { 
    Geometry  a
    Appearance  DEFAULT_MATERIAL
}

AxisRotated b { 
    Axis <1,0,0>
    Angle -15
    Geometry Translated { 
        Translation <-1,0,20>
        Geometry AxisRotated { 
            Axis <0,1,0>
            Angle 90
            Geometry Cylinder { 
                Radius 4
                Height 2.2
                Slices 20
            }
        }
    }
}

Material APP_174563360 { 
    Ambient <37,8,13>
    Diffuse 3.89189
}

Shape SHAPE_17800036 { 
    Geometry  b
    Appearance  APP_174563360
}

AxisRotated c { 
    Axis <1,0,0>
    Angle -15
    Geometry Translated { 
        Translation <0.11,0,20>
        Geometry Box { 
            Size <1.2,1,4>
        }
    }
}

Material APP_17484320 { 
    Ambient <33,14,37>
    Diffuse 4.62162
}

Shape SHAPE_17800780 { 
    Geometry  c
    Appearance  APP_17484320
}

AxisRotated d { 
    Axis <1,0,0>
    Angle -15
    Geometry Translated { 
        Translation <0,0,24>
        Geometry Scaled boucle { 
            Scale <2,2,1>
            Geometry NurbsPatch { 
                UDegree 2
                CtrlPointMatrix [
                     [ <0.1,0,0,1>, <1,4,0,1>, <1.5,4,5,1>, <0.1,0,0,1> ],
                     [ <0,0,0,1>, <0.1,4,-1,1>, <0.2,4,6,1>, <0,0,0,1> ],
                     [ <-0.1,0,0,1>, <-1,4,0,1>, <-1.5,4,5,1>, <-0.1,0,0,1> ]
                ]
            }
        }
    }
}

Shape SHAPE_17799812 { 
    Geometry  d
    Appearance  APP_17484320
}

AxisRotated e { 
    Axis <1,0,0>
    Angle -15
    Geometry Translated { 
        Translation <0,0,24>
        Geometry AxisRotated { 
            Angle 180
            Geometry boucle

        }
    }
}

Shape SHAPE_18084756 { 
    Geometry  e
    Appearance  APP_17484320
}

