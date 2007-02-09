(#
   Creates a simple cone with a radius of 3 and a height
   of 5. The number of slices is set to 16, and the base is
   drawn.
#)


Cylinder my_trunk {
	Height 2
	Radius 0.5
	Slices 16
}

Translated my_crown {
	Translation <0,0,2>
	Geometry Cone {
		Radius 2
		Height 5
		Solid True     
		Slices 16
	}
}
