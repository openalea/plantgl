(#
   A glass represented by a Revolution  
#)

:Echo A glass represented by a Revolution 

Scaled my_vase {
	Scale <2,1,2>
	Geometry Tapered {
		BaseRadius 3
		TopRadius 0.5
		Primitive Revolution {
			PointList [
				<0.000, 0.000>,
				<0.118, 0.000>,
				<0.620, 0.540>,
				<0.210, 0.827>,
				<0.194, 0.962>,
				<0.287, 1.000>,
				<0.470, 1.034>
			]
			Slices 200
		}
	}
}
