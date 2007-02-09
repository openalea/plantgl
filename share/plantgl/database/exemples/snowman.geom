(#
   A Snowman
   C. Nouguier
#)


Tapered BODY {
	BaseRadius 1.2
	TopRadius 0.8
	Primitive Sphere {
		Radius 4
		Slices 32
		Stacks 32
	}
}

Translated HEAD {
	Translation <0,0,5.25>
	Geometry Sphere {
		Radius 1.5
		Slices 32
		Stacks 32
	}
}

Translated HAT {
	Translation <0,0,6>
	Geometry Frustum {
		Radius 1.6
		Height 2
		Taper 0.25
		Slices 16
	}
}

Translated NOSE {
	Translation <1.5,0,5.25>
	Geometry AxisRotated {
		Axis <0,1,0>
		Angle 90
		Geometry Cone {
			Radius 0.25
			Slices 8
		}
	}
}

Translated EYE1 {
	Translation <1.4,-0.5,5.6>
	Geometry Sphere {
		Radius 0.2
	}
}

Translated EYE2 {
	Translation <1.4,0.5,5.6>
	Geometry Sphere {
		Radius 0.2
	}
}

Polyline SWEEP {
	PointList [ 	<0,2,2>, 
			<0,4.5,4.5>, 
			<0,5,7>,
			<0,4.5,4.5>,
			<0,6,6>,
			<0,4.5,4.5>, 
			<0,7,5>
	]
}



		
