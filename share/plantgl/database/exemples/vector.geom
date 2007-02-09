:Ifndef __vector_geom__
:Define __vector_geom__

:Ifdef DEBUG
:Echo ------------------------------------------------------
:Echo vector.geom : Vector related macros
:Echo Author : Fred Boudon
:Echo 
:Echo Definition of Vector2 in Polar Coordinates : 
:Echo Polar( radius , theta )
:Endif
:Define Polar( radius , theta ) <radius*:cos(theta), \
				 radius*:sin(theta) > 

:Ifdef DEBUG
:Echo Definition of Vector3 in Cylindrical Coordinates :
:Echo Cylindrical( radius , theta , z )
:Endif
:Define Cylindrical(radius,theta,z) <radius*:cos(theta), \
				     radius*:sin(theta), \
				     z>

:Ifdef DEBUG
:Echo Definition of Vector3 in Spherical Coordinates :
:Echo Spherical( radius , theta , phi )
:Echo ------------------------------------------------------
:Endif
:Define Spherical(radius,theta,phi) <radius*:cos(theta)*:sin(phi), \
				     radius*:sin(theta)*:sin(phi), \
				     radius*:cos(phi)> 

:Else
:Echo vector.geom already included
:Endif