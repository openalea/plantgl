BezierCurve2D c {
	CtrlPointList [ <0,0.2>,<0,0>,<0.3,0>,<0.7,0.5>,<0.5,1>,<0.2,1>,<0,0.7>]
}

Revolution pom {
	PointList c
	Slices 32
}

Material r {
	Ambient <60,0,0>
	Diffuse 4
}

Shape pomme { pom, r }

Translated p {
	Translation <0,0,0.5>
	Geometry Cylinder  {
		Radius 0.01
		Height 0.45
	}
}

Shape {p}