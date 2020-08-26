(# -Geom File-
 ###############################################
 #
 # Swung
 #
 # Author : C. Pradal
 #
 ###############################################
 #)
:include colors.app

#Pin maritime

Polyline2D Tronc
  {
  PointList [<0,0>,<0.5,0>,<.5,32>,<0,32>]
  }

BezierCurve2D f1
  {
  CtrlPointList [ <0,30>, <20,20>, <20, 30>, <0,40> ]
  Stride 10
  }

BezierCurve2D f2
  {
  CtrlPointList [ <0,30>, <40,50>, <40, 60>, <0,40> ]
  Stride 10
  }

BezierCurve2D f3
  {
  CtrlPointList [ <0,30>, <20,30>, <20,40>, <0,40> ]
  Stride 10
  }

BezierCurve2D f4
  {
  CtrlPointList [ <0,30>, <40,45>, <40,55>, <0,40> ]
  Stride 10
  }

BezierCurve2D f5
  {
  CtrlPointList [ <0,30>, <20,25>, <20,35>, <0,40> ]
  Stride 10
  }

Swung t1
  {
  ProfileList [ Tronc ]
  AngleList [ 0 ]
  Slices 10
  }

Swung t2
  {
  ProfileList [ f1, f5, f3,  f4,  f2,  f4,  f3,  f5,  f1 ]
  AngleList   [  0, 45, 90, 160, 180, 200, 270, 315, 360 ]
#  ProfileList [ f1, f3,  f2,  f3, f1 ]
#  AngleList   [  0, 90, 180, 270, 360 ]
#  ProfileList [ f1, f2, f1 ]
#  AngleList   [  0, 180, 360 ]
  Slices 30
  Degree 3
  Stride 6
  }

Group pin
  {
  GeometryList [ t1, t2 ]
  }

Shape{pin,GREEN2}
Shape{f1,BLACK}
Shape{f2,GREEN5}
Shape{f3,RED5}
#Shape{f4,BLUE5}
#Shape{f5,MAGENTA5}
