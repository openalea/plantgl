:Ifndef __circle_geom__
:Define __circle_geom__

:include vector.geom

:Ifdef DEBUG
:Echo ------------------------------------------------------
:Echo circle.geom : Definition of a GEOM object circle
:Echo Author : Fred Boudon
:Echo 
:Echo Definition of Circle( name , radius )
:Echo Definition of Circle3D( name , radius )
:Echo ------------------------------------------------------
:Endif


:Define Circle( name , radius ) \
Polyline2D name { \
	PointList [ Polar(radius,0),     Polar(radius,Pi/8), \
		    Polar(radius,Pi/4),  Polar(radius,3*Pi/8), \
		    Polar(radius,Pi/2),  Polar(radius,5*Pi/8), \
		    Polar(radius,3*Pi/4),Polar(radius,7*Pi/8), \
		    Polar(radius,Pi),    Polar(radius,9*Pi/8), \
		    Polar(radius,5*Pi/4),Polar(radius,11*Pi/8), \
		    Polar(radius,3*Pi/2),Polar(radius,13*Pi/8), \
		    Polar(radius,7*Pi/4),Polar(radius,15*Pi/8), \
		    Polar(radius,2*Pi)] \
}


:Define Circle3D( name , radius ) \
Polyline name { \
	PointList [ Cylindrical(radius,0,0),     Cylindrical(radius,Pi/8,0), \
		    Cylindrical(radius,Pi/4,0),  Cylindrical(radius,3*Pi/8,0), \
		    Cylindrical(radius,Pi/2,0),  Cylindrical(radius,5*Pi/8,0), \
		    Cylindrical(radius,3*Pi/4,0),Cylindrical(radius,7*Pi/8,0), \
		    Cylindrical(radius,Pi,0),    Cylindrical(radius,9*Pi/8,0), \
		    Cylindrical(radius,5*Pi/4,0),Cylindrical(radius,11*Pi/8,0), \
		    Cylindrical(radius,3*Pi/2,0),Cylindrical(radius,13*Pi/8,0), \
		    Cylindrical(radius,7*Pi/4,0),Cylindrical(radius,15*Pi/8,0), \
		    Cylindrical(radius,2*Pi,0)] \
}

:Else
:Echo circle.geom already included
:Endif

