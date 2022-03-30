(#
   
#)

Sphere my_sphere {
	Radius 0.5
	Slices 16
	Stacks 16
}	


Scaled my_ellipsoid {
	Scale <0.5,0.25,3>
	Geometry my_sphere
}
