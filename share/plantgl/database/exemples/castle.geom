(#
   A castle.
#)

Translated the_main_batiment {
	Translation <0,0,0.5>
	Geometry Box {
		Size <2,1,0.5>
	}
}

Group the_dungeon {
	GeometryList [
		Cylinder {
			Radius 0.5
			Height 2.5
			Slices 16
		},
		Translated {
			Translation <0,0,2.5>
			Geometry Cone {
				Radius 0.75
				Height 1
				Slices 16
			}
		}
	]
	Skeleton Polyline {
		PointList [ <0,0,0>, <0,0,3.5> ]
	}	
}

Translated a_first_tower {	
	Translation <-2,-1,0>	
	Geometry Scaled {
		Scale <0.8,1,0.4>
		Geometry the_dungeon
	}
}

Translated a_second_tower {
	Translation <-2,1,0>	
	Geometry Scaled {
		Scale <1.2,1,0.5>
		Geometry the_dungeon
	}
}

Translated a_third_tower {
	Translation <2,1,0>	
	Geometry Scaled {
		Scale <1.4,1.4,0.6>
		Geometry the_dungeon
	}
}

Translated a_fourth_tower {
	Translation <2,-1,0>	
	Geometry Scaled {
		Scale <0.6,0.6,0.7>
		Geometry the_dungeon
	}
}
