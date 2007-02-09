(#
   A petal represented by a Scaled BezierPatch
   C. Nouguier, 11/12/2000
#)

Scaled a_petal {
	Scale <0.5,0.8,0.2>
	Geometry BezierPatch {
		CtrlPointMatrix [	 [ <0,0,0>, <0,0,0> ],
					 [ <2,-1,0.8>, <2,1,1.2> ],
					 [ <4,-2,2.1>, <4,2,1.8> ],
					 [ <6,-1,0.3>, <6,1,0.5> ], 
                	                 [ <7,0,-1>, <7,0,-1> ]
		]
		UStride 10
		VStride 10
	}
}
	
