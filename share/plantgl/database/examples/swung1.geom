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

BezierCurve2D p1
  {
  CtrlPointList [ <2,0>, <2,2>, <5,3>, <4,4>, <5,6>, <2,8>, <2,9>, <0,10> ]
  Stride 20
  }

BezierCurve2D p2
  {
  CtrlPointList [ <4,0>, <3,2>, <4,3>, <7,8>, <5,6>, <2,7>, <2,9>, <0,10> ]
  Stride 20
  }

BezierCurve2D p3
  {
  CtrlPointList [ <2,0>, <8,2>, <1,3>, <7,8>, <20,6>, <2,7>, <5,9>, <0,10> ]
  Stride 10
  }

BezierCurve2D p31
  {
  CtrlPointList [ <2,0>, <5,2>, <1,3>, <7,6>, <12,6>, <2,7>, <2,11>, <0,10> ]
  Stride 20
  }

BezierCurve2D p4
  {
  CtrlPointList [ <2,0>, <2,2>, <5,4>, <4,10>, <0,8> ]
  Stride 10
  }

BezierCurve2D p5
  {
  CtrlPointList [ <2,0>, <0,2>, <3,4>, <4,6>, <0,8> ]
  Stride 10
  }

Polyline2D s1
  {
  PointList
    [ <0,0>,
      <0.5,0>,
      <0.5,2>,
      <5.5,2>,
      <4,3.5>,
      <2,5.5>,
      <4.75,5.5>,
      <3,6.5>,
      <1.5,8.5>,
      <2.75,8.5>,
      <1,10>,
      <0.,12> ]
  }
Polyline2D s1_2
  {
  PointList
    [ <0,0>,
      <0.5,0>,
      <0.5,2>,
      <7.5,2>,
      <4,3.5>,
      <2,5.5>,
      <5.75,5.5>,
      <3,6.5>,
      <1.5,8.5>,
      <3.75,8.5>,
      <1,10>,
      <0.,12> ]
  }

Polyline2D s2
  {
  PointList
    [ <0,0>,
      <0.5,0>,
      <0.5,2>,
      <2,2>,
      <1.8,3>,
      <1.6,4>,
      <1.4,5>,
      <1.2,6>,
      <0.8,8>,
      <0.4,10>,
      <0.2,11>,
      <0.,12> ]
  }


Swung rh1
  {
  ProfileList [ p1, p2, p31, p2, p1 ]
  AngleList [ 0, 90., 180., 270., 360. ]
  Slices 20
  Stride 10
  }

Translated trh2
  {
  Translation <10,0,0>
  Geometry Swung rh2
    {
    ProfileList [ p2 ]
    AngleList [ 0 ]
    Slices 10
    Stride 10
    }
  }

Translated trh3
  {
  Translation <-10,10,0>
  Geometry Swung rh3
    {
    ProfileList [ p4, p5, p4 ]
    AngleList [ 0,  180., 360. ]
    Slices 10
    Stride 12
    }
  }

Translated trh4
  {
  Translation <-10,-10,0>
  Geometry Swung rh4
    {
    ProfileList [ s1 ]
    AngleList [ 0 ]
    Slices 10
    }
  }

Translated trh5
  {
  Translation <10,10,0>
  Geometry Swung rh5
    {
    ProfileList [ s1, s2, s1, s2, s1_2, s2, s1, s2, s1 ]
    AngleList [ 0, 45, 90, 135, 180, 225, 270, 315, 360 ]
    Slices 32
    Degree 1
    }
  }

Shape{rh1,RED4}
Shape{trh2,BLUE4}
Shape{trh3,GREEN4}
Shape{trh4,GREEN2}
Shape{trh5,GREEN2}
Shape{s1,YELLOW5}
Shape{s1_2,GREEN5}
Shape{s2,RED5}


