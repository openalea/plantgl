:Include circle.geom
:Include aligned.geom

:List
:PrintDef

Shape{ Geometry Circle( mycircle , 5 )
	Appearance Black }

Shape{ Geometry Circle3D( mycircle2 ,12)
	Appearance Black }

Shape { Geometry Extrusion a { Axis Circle3D ( c1 , 12 ) CrossSection Circle ( c2, 5 ) } Appearance Blue }

Shape { Geometry Aligned( "", (OX) , OZ, a ) Appearance Green }

Sphere tata {
	Radius (2*5)/3
}

(#
#)