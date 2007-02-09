Group tree1 {
	GeometryList [
		Cylinder {
			Radius 0.5
			Height 8
			Slices 24
		},
		Translated {
			Translation <0,0,5>
			Geometry AsymmetricHull {	
				PosXRadius 6
				NegXRadius 8
				PosYRadius 6
				NegYRadius 5
				PosXHeight 8.5
				NegXHeight 4.5
				PosYHeight 6.5
				NegYHeight 7
				Bottom <0,0,0>
				Top <1,0.5,14>
				BottomShape 0.8
				TopShape 2
				Slices 6
				Stacks 6
			}
		}
	]
}
