(#
   A virtual stand
   C. Nouguier
#)


Translated TRUNK1 {
	Translation <-7,-7,0>
	Geometry Cylinder {
		Radius 0.5
		Height 10.5
	}
}

Translated CROWN1 {
	Translation <-7,-7,10>
	Geometry AsymmetricHull {
		NegXRadius 5
		PosXRadius 4
		NegYRadius 4.5
		PosYRadius 5
		NegXHeight 2.5
		PosXHeight 3
		NegYHeight 3.5
		PosYHeight 3
		BottomShape 2
		TopShape 0.8
		Bottom <0,0,0>
		Top <0,0,20>
	}
}

Translated TRUNK2 {
	Translation <-7,7,0>
	Geometry Cylinder {
		Radius 0.4
		Height 8.5
	}
}

Translated CROWN2 {
	Translation <-7,7,8>
	Geometry AsymmetricHull {
		NegXRadius 4
		PosXRadius 4
		NegYRadius 5
		PosYRadius 4.5
		NegXHeight 5
		PosXHeight 4.5
		NegYHeight 5
		PosYHeight 5
		BottomShape 3
		TopShape 2
		Bottom <0,0,0>
		Top <0,0,16>
	}
}

Translated TRUNK3 {
	Translation <7,7,0>
	Geometry Cylinder {
		Radius 0.3
		Height 6.5
	}
}

Translated CROWN3 {
	Translation <7,7,6>
	Geometry AsymmetricHull {
		NegXRadius 8
		PosXRadius 7
		NegYRadius 5
		PosYRadius 6
		NegXHeight 2
		PosXHeight 3
		NegYHeight 3
		PosYHeight 3
		BottomShape 0.8
		TopShape 0.8
		Bottom <0,0,0>
		Top <0,0,12>
	}
}

Translated TRUNK4 {
	Translation <7,-7,0>
	Geometry Cylinder {
		Radius 0.2
		Height 4.5
	}
}

Translated CROWN4 {
	Translation <7,-7,4>
	Geometry AsymmetricHull {
		NegXRadius 5
		PosXRadius 5
		NegYRadius 5
		PosYRadius 5
		NegXHeight 0
		PosXHeight -2
		NegYHeight -1
		PosYHeight 1
		BottomShape 1.2
		TopShape 2
		Bottom <0,0,0>
		Top <0,0,8>
	}
}