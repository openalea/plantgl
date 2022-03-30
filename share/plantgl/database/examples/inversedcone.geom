(#
   Creates a simple Cone.
#)


:Define InversedCone(name,radius,height,solid,slices) \
Translated name { \
	Translation <0,0,height> \
	Geometry AxisRotated { \
		Axis OY \
		Angle Pi \
		Geometry Cone { \
			Radius radius \
			Height height \
			Solid solid \
			Slices slices \
		} \
	} \
}


InversedCone(my_cone2,2,5,True,16)

Translated my_cone {
	Translation <8,8,8>
	Geometry
	Cone  {
		Radius 2
		Height 5
		Solid True     
		Slices 16
	}
}