

#spec with only ctrlPoints

#Translated my_head {
#	Translation <0,0,-5>
#	Geometry NurbsCurve  {
#	CtrlPointList [ <-4,-4,0>, <-2,4,0>, <2,-4,0>, <4,4,0>, <6,4,0>, <4,-4,0>, <2,-8,0>, <-2,-8,0>,<-4,-4,0> ]
#	}
#}

#spec with  ctrlPoints and degree
NurbsCurve heart {
	Degree 15
	Stride 100
	CtrlPointList [ <0,0,1>, <0,0,4>,<-3 ,0,3.5>, <-4,0,2>, <-2,0,-1.5> ,<0,0,-4>,<0,0,-4>,<0,0,-4>,<0,0,-4>,<0,0,-4>,<0,0,-4>, <2,0,-1.5>, <4,0,2>, <3,0,3.5>, <0,0,4>,  <0,0,1>]
}

#spec with  ctrlPoints and degree : error with degree
#NurbsCurve third {
#	Degree 5
#	CtrlPointList [ <1,-4,0>, <3,4,0>, <7,-4,0>, <9,4,0> ]
#}


#complete spec 
#NurbsCurve four {
#	Degree 2
#	CtrlPointList [ <6,-4,0>, <-2,4,0>, <2,-4,0>, <4,4,0> ]
#	KnotList [0.0, 0.0, 0.0, 0.5 , 1.0 , 1.0, 1.0 ]
#}

#spec without degree -> degree 2 ? 
#NurbsCurve five {
#	CtrlPointList [ <11,-4,0>, <3,4,0>, <7,-4,0>, <9,4,0> ]
#	KnotList [0.0, 0.0, 0.0, 0.5 , 1.0 , 1.0, 1.0 ]
#}


#complete spec : fields coherence error
#NurbsCurve six {
#	Degree 3
#	CtrlPointList [ <6,-4,0>, <-2,4,0>, <2,-4,0>, <4,4,0> ]
#	KnotList [0.0, 0.0, 0.0, 0.5 , 1.0 , 1.0, 1.0 ]
#}

#complete spec : fields coherence error
#NurbsCurve seven {
#	Degree 3
#	CtrlPointList [ <6,-4,0>, <-2,4,0>, <2,-4,0> ]
#	KnotList [0.0, 0.0, 0.0, 0.5 , 1.0 , 1.0, 1.0 ]
#}
