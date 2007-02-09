:Ifndef __aligned_geom__
:Define __aligned_geom__

:Ifdef DEBUG
:Echo ------------------------------------------------------
:Echo aligned.geom : Definition of a GEOM object Aligned
:Echo Author : Fred Boudon
:Echo 
:Echo Definition of Aligned( name , newaxis , oldaxis, geometry )
:Echo ------------------------------------------------------
:Endif

:Define Aligned( name , newaxis , oldaxis, geometry ) \
AxisRotated name { \
	Axis :cross(newaxis,oldaxis) \
	Angle :angle(newaxis,oldaxis) \
	Geometry geometry \
}

:Endif
