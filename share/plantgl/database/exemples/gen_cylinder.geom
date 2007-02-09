(# -Geom File-
 ###############################################
 #
 # Generalized Cylinders.
 #
 # Author : Fred Boudon.
 #
 ###############################################
 #)
:include colors.app

BezierCurve2D _crossSection {
			CtrlPointList [ <-2,0>, <-2,-2>, <2,-2>, <2,2>,
					<-2,2>, <-2,0> ]
			Stride 10		
}

	
BezierCurve _axis {
		CtrlPointList [ <0,0,0>, <3,3,3>, <0,0,6>, <-3,-3,9>, <0,0,12>]
		Stride 10
		}


NurbsCurve _axis2 {
		CtrlPointList [ <0,0,0>, <3,3,3>, <-3,-3,9>, <0,0,12>]
		Stride 30
		}

Polyline _axis3 {
		PointList [ <0,0,0>, <3,3,3>, <-3,-3,9>, <0,0,12>]
		}

Extrusion _mycylinder {
	Axis  _axis
	CrossSection _crossSection
		Scale [ <2,2>,<0.2,0.2> ]
#	Scale [ <1,1>,<2,2>,<0.0,0.0> ]
#	KnotList [ 0, 0.2 ,1 ]
}


Translated _mysecondcylinder {
	Translation <10,0,0>
	Geometry Extrusion second {
 		Orientation [ 0, 90 ]
		Scale [ <2,2>,<0.2,0.2> ]
		KnotList [ 0, 1 ]
		Axis  _axis2
		CrossSection _crossSection
		Solid True
	}
}

Translated _mythirdcylinder {
	Translation <-10,0,0>
	Geometry Extrusion third {
		Axis  _axis3
 		Orientation [ 0, 90 ]
		CrossSection _crossSection
		Scale [ <2,2>,<0.2,0.2> ]
	}
}



Shape{_mycylinder,RED5} 
Shape{_mysecondcylinder,BLUE5}
#Shape{	
#	Geometry Translated {
#	Translation <20,0,0>
#	Geometry _axis
#	}
#	Appearance BLUE5
#}
Shape{_mythirdcylinder,GREEN5}
