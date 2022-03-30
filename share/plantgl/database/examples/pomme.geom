 # -Geom File-
(#--------------------------------------------------
 #  
 #  File : D:/Fred/Doc/UserManual/Examples/pomme3.geom
 #  
 #  A GEOM file generated with GEOM library
 #  Published under the GNU General Public Licence. 
 #  
 # --------------------------------------------------
 #)

Revolution pom { 
    Profile BezierCurve2D c { 
        Degree 6
        CtrlPointList [ 
            <0,0.2>, 
            <0,0>, 
            <0.3,0>, 
            <0.7,0.5>, 
            <0.5,1>, 
            <0.2,1>, 
            <0,0.7>
        ]
    }
    Slices 32
}

Material r { 
    Ambient <60,0,0>
    Diffuse 4
}

Shape pomme { 
    Geometry  pom
    Appearance  r
}

Translated p { 
    Translation <0,0,0.5>
    Geometry Cylinder { 
        Radius 0.01
        Height 0.45
    }
}

Material brown { 
    Ambient <60,30,0>
    Diffuse 3
}

Shape petiole { 
    Geometry  p
    Appearance  brown
}

