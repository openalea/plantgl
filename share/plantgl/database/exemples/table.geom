
:Define LENGTH 8
:Define HEIGHT 10
:Define WIDTH_Z 0.5
:Define WIDTH_R 4

Material col {
	Ambient <22,18,9>
	Diffuse 4.7
	Specular <44,44,17>
}

Cylinder foot {
	Radius 0.5
	Height HEIGHT
	Solid False
}

Translated s_foot {
	Translation <0,LENGTH,0>
	Geometry foot
}

Translated t_foot {
	Translation <LENGTH,0,0>
	Geometry foot
}

Translated f_foot {
	Translation <LENGTH,LENGTH,0>
	Geometry foot
}

Group feet {
	GeometryList [ foot , s_foot , t_foot , f_foot ]
}
 
Shape { feet, col }

Translated tray {
	Translation <LENGTH/2,LENGTH/2,HEIGHT+WIDTH_Z>
	Geometry Box {
		Size <(LENGTH/2)+WIDTH_R,(LENGTH/2)+WIDTH_R,WIDTH_Z>
	}
}

Shape { tray, col }

Translated cover {
	Translation <LENGTH/2,LENGTH/2,HEIGHT+2*WIDTH_Z+0.1>
	Geometry Box {
		Size <(LENGTH/2)+(WIDTH_R/2),(LENGTH/2)+(WIDTH_R/2),0.1>
	}
}
Material col_cover {
	Ambient <150,150,0>
}

Shape { cover, col_cover }

Translated cover_b {
	Translation <LENGTH/2,LENGTH/2,HEIGHT-0.1>
	Geometry Box {
		Size <(LENGTH/2)+(WIDTH_R/2),(LENGTH/2)+(WIDTH_R/2),0.1>
	}
}

Shape { cover_b, col_cover }

Scaled chair_feet {
	Scale <0.6,0.6,0.6>
	Geometry feet
}

Translated chair_tray {
	Translation <LENGTH*0.6/2,LENGTH*0.6/2,HEIGHT*0.6>
	Geometry Scaled {
		Scale <0.6,0.6,0.6>
		Geometry Box {
			Size <(LENGTH/2),(LENGTH/2),WIDTH_Z/2>
		}
	}
}

Group chair_base {
	GeometryList [chair_feet,chair_tray]
}

Scaled  chair_top {
	Scale <0.6,0.6,0.6>
	Geometry Translated  {
		Translation <0,0,HEIGHT>
		Geometry Group {
			GeometryList [
				foot,
				t_foot,
				Translated {
					Translation <(LENGTH/2),0,(HEIGHT/2)>
					Geometry Box {
					Size <(LENGTH/2),WIDTH_Z/2,(HEIGHT/2)*0.7>
					}
				}
			]
		}
	}
}

	
Group chair {
	GeometryList [ chair_base, chair_top ]
}

Translated f_chair {
	Translation <LENGTH*0.2,-WIDTH_R-4,0>
	Geometry chair
}

Shape { f_chair, col }

Translated s_chair {
	Translation <LENGTH*0.8,LENGTH+WIDTH_R+4,0>
	Geometry AxisRotated {
		Axis OZ
		Angle Pi
		Geometry chair
	}
}

Shape { s_chair, col }

Translated t_chair {
	Translation <LENGTH+WIDTH_R+4,LENGTH*0.2,0>
	Geometry AxisRotated {
		Axis OZ
		Angle Pi/2
		Geometry chair
	}
}

Shape { t_chair, col }

Translated forst_chair {
	Translation <-WIDTH_R-4,LENGTH*0.8,0>
	Geometry AxisRotated {
		Axis OZ
		Angle -Pi/2
		Geometry chair
	}
}

Shape { forst_chair, col }

:include teapot.geom

Translated teap {
	Translation <LENGTH/2-1.5,LENGTH/2,HEIGHT+WIDTH_Z>
	Geometry Scaled {
		Scale <2.5,2.5,2.5>
		Geometry teapot
	}
}

Material col_t {
	Ambient <22,22,22>
	Diffuse 11
	Specular White
}

Shape { teap, col_t }


Translated plate_cup {
	Translation <0,0,HEIGHT+(WIDTH_Z*2)+0.2>	
	Geometry Revolution {
		PointList BezierCurve2D {
			CtrlPointList [<0,0,1>,<1.2,0,1>,<1.5,0.5,1> ]
		}
	}
}

Translated cup {
	Translation <0,0,HEIGHT+WIDTH_Z*2+0.2>
	Geometry Revolution {
		PointList BezierCurve2D {
			CtrlPointList [<0,0,1>,<0.7,0.2,1>,<0.8,0.8,1>,<0.8,1.0,1>,<1.0,1.2,1> ]
		}
	}
}
Group plate {
	GeometryList [plate_cup,cup]
}

Translated f_plate {
	Translation <0,LENGTH/2,0>
	Geometry plate 
}

Shape { f_plate , col_t }

Translated s_plate {
	Translation <LENGTH,LENGTH/2,0>
	Geometry plate
}

Shape { s_plate , col_t }

Translated t_plate {
	Translation <LENGTH/2,0,0>
	Geometry plate
}

Shape { t_plate , col_t }

Translated fo_plate {
	Translation <LENGTH/2,LENGTH,0>
	Geometry plate
}

Shape { fo_plate , col_t }

Translated f_furniture {
	Translation <-WIDTH_R-LENGTH-8,-LENGTH*0.3,0>
	Geometry Scaled  furniture {
		Scale <1,3,1>
		Geometry Group {
			GeometryList [
				chair_base,
				Translated {
					Translation <0,0,HEIGHT*0.6>
					Geometry chair_base
				},
				Translated {
					Translation <0,0,HEIGHT*1.2>
					Geometry chair_base
				},
				Translated {
					Translation <0,0,HEIGHT*1.8>
					Geometry chair_base
				}
			]
		}
	}
}

Shape { f_furniture, col }

Translated s_furniture {
	Translation <-WIDTH_R-LENGTH-8,-LENGTH*2.3,0>
	Geometry furniture
}
Shape { s_furniture, col }

Translated t_furniture {
	Translation <-WIDTH_R-LENGTH-8,LENGTH*1.7,0>
	Geometry furniture
}
Shape { t_furniture, col }

Translated fo_furniture {
	Translation <LENGTH*1.3,-WIDTH_R-LENGTH-8,0>
	Geometry AxisRotated {
		Axis OZ
		Angle Pi/2
		Geometry furniture
	}
}
Shape { fo_furniture, col }
