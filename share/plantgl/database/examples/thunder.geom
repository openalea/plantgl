#VRML V2.0 utf8
# Copyright (C) 1999 John Stewart, CRC Canada.
# DISTRIBUTED WITH NO WARRANTY, EXPRESS OR IMPLIED.
# See the GNU Library General Public License (file COPYING in the distribution)
# for conditions of use and redistribution.

# first example of points, lines and faces.
# this is a lightning bolt. "VRML Sourcebook v2.0", figure 13.13
Shape {
        Appearance Material {
		Ambient <25,25,0>
	        Diffuse 10
	}
	Geometry Translated {
#		Translation <-3.0,-5.0,-10.0>
	   Geometry AxisRotated {
		Axis <0,1,1>
	        Angle 23
	        Geometry FaceSet {
		PointList [
			# lightning bolt tip
			<0.0,0.0,0.0>,
			# front perimiter
			<0.88,5.5,5.0>,
			<0.968,5.5,4.0>,
			<1.408,8.0,7.0>,
			<1.584,9.0,4.0>,
			<0.88,5.0,1.0>,
			<0.792,4.5,2.5>,
			# back perimeter
			<-0.88,5.5,5.0>,
			<-0.968,5.5,4.0>,
			<-1.408,8.0,7.0>,
			<-1.584,9.0,4.0>,
			<-0.88,5.0,1.0>,
			<-0.792,4.5,2.5>
			]
		IndexList [
		# front
#			[0, 1, 2, 3, 4, 5, 6],
			[0, 1, 2, 6],
			[2, 3, 4, 5, 6],
		# back
#			[0, 12, 11, 10, 9, 8, 7],
			[0, 12, 8 , 7],
			[12, 11, 10, 9, 8],
		# sides
			[0, 7, 1],
			[1, 7, 8, 2],
			[2, 8, 9, 3],
			[3, 9, 10, 4],
			[4, 10, 11, 5],
			[5, 11, 12, 6],
			[6, 12, 0]	
		]
		Solid True
		}
	   }		
	}
}
