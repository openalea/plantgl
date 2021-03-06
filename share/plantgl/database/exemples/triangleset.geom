(#
   An isocahedron represented by a TriangleSet
   C. Nouguier, 12/12/2000
#)

TriangleSet triangle_set {
	PointList [ 	<-5.2,0,8.5>, <5.2,0,8.5>, <-5.2,0,-8.5>, <5.2,0,-8.5>,
                        <0,8.5,5.2>, <0,8.5,-5.2>, <0,-8.5,5.2>, <0,-8.5,-5.2>,
                        <8.5,5.2,0>, <-8.5,5.2,0>, <8.5,-5.2,0>, <-8.5,-5.2,0>
	] 	
	IndexList [ 	[0,1,4], [0,4,9], [9,4,5], [4,8,5], [4,1,8],
              		[8,1,10], [8,10,3], [5,8,3], [5,3,2], [2,3,7],
			[7,3,10], [7,10,6], [7,6,11], [11,6,0], [0,6,1],
			[6,10,1], [9,11,0], [9,2,11], [9,5,2], [7,11,2] 
	]
	CCW True
	Solid True
}
