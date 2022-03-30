
:include snowman.geom



Translated tree1 {
	Translation <-5,-5,-4>
	Geometry Group {
		GeometryList [
			Cylinder {
				Height 10
				Radius 0.8
			},
			Translated {
				Translation <0,0,5>
				Geometry AsymmetricHull {
					PosXRadius 8
					PosYRadius 7
					NegXRadius 6
					NegYRadius 9
					PosXHeight 11
					PosYHeight 12
					NegXHeight 13
					NegYHeight 11
					Top <1,1,15>
					Bottom <0,0,0>
					TopShape 1.5
					BottomShape 0.5
					Slices 5
					Stacks 5
				}
			}
		]
	}
}


Translated tree2 {
	Translation <-7,3,-4>
	Geometry Group {
		GeometryList [
			Cylinder {
				Height 10
				Radius 0.4
			},
			Translated {
				Translation <0,0,5>
				Geometry AsymmetricHull {
					PosXRadius 2
					PosYRadius 2
					NegXRadius 3
					NegYRadius 3
					PosXHeight 1.25
					PosYHeight 1
					NegXHeight 1.5
					NegYHeight 1
					Top <1,0,13>
					Bottom <0,0,0>
					TopShape 1.2
					BottomShape 2
					Slices 5
					Stacks 5
				}
			}
		]
	}
}


Translated tree3 {
	Translation <5,6,-4>
	Geometry Group {
		GeometryList [
			Cylinder {
				Height 10
				Radius 0.4
			},
			Translated {
				Translation <0,0,5>
				Geometry AsymmetricHull {
					PosXRadius 2
					PosYRadius 2
					NegXRadius 3
					NegYRadius 3
					PosXHeight 1.25
					PosYHeight 1
					NegXHeight 1.5
					NegYHeight 1
					Top <0,0,14>
					Bottom <0,0,0>
					TopShape 2
					BottomShape 2
					Slices 5
					Stacks 5
				}
			}
		]
	}
}


Translated tree4 {
	Translation <0,-8,-4>
	Geometry Group {
		GeometryList [
			Cylinder {
				Height 8
				Radius 0.35
			},
			Translated {
				Translation <0,0,4>
				Geometry AsymmetricHull {
					PosXRadius 2
					PosYRadius 3
					NegXRadius 1
					NegYRadius 2
					PosXHeight 2
					PosYHeight 1
					NegXHeight 1.5
					NegYHeight 1
					Top <0,0,9>
					Bottom <0,0,0>
					TopShape 2
					BottomShape 2
					Slices 5
					Stacks 5
				}
			}
		]
	}
}
