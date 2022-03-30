ExtrudedHull hull {
	Vertical Polyline2D a {
		PointList [ <0,-3.5>,
		   <-1,-3>,
		   <-3,-1>, 
                   <-3,1>, 
                   <-1,3.2>,
		   <1,3.2>,
		   <3,1>,
		   <3,-1>,
		   <1,-3>,
		   <1,-3.5>,
		   <0.5,-4>,
		   <0,-5>
		]
             }
	Horizontal  Polyline2D b {
		PointList [ 
		<-3.8,0>,
		<-4,-1>,
		<-2,-3>,
		<0,-4>,
		<0,-4>,
		<1,-3>,
		<3,-3>,
		<5,-2>,
		<5,0>,
		<4,1>,
		<3,1>,
		<2,2>,
		<2,3>,
		<1,4>,
		<-1,4>,
		<-2,3.75>,
		<-3,2.8>,
		<-3,1>
		]
	}
}

Material red {
	Ambient Red
}

Material green {
	Ambient <0,100,0>
}

Shape {
	Geometry Oriented {
		Primary <1,0,0>	
		Secondary <0,0,1>
		Geometry a
	}
	Appearance red
}
Shape {b,red}

Shape {hull,green}