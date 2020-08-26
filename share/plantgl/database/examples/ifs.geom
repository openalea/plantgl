(# -Geom File-
 ###############################################
 #
 # IFS
 #
 # Author : C. Pradal
 #
 ###############################################
 #)
:include colors.app

Sphere ronde
  {
  Radius 1
  Slices 4
  Stacks 4
  }


IFS ifs1
  {
  Depth 1
  Geometry ronde
  TransfoList
    [
    Transfo t1 {
      Translation <.5,0,0>
      Scale <0.5,0.5,0.5> },
    Transfo t2 { Translation <-.5,0,0>
      Scale <0.5,0.5,0.5> },
    Transfo t3 { Translation <0,.5,0>
      Scale <0.5,0.5,0.5> },
    Transfo t4 { Translation <0,-.5,0>
      Scale <0.5,0.5,0.5> }
    ]
  }


#Shape{ronde,BLUE4}
#Shape{ifs1,RED7}

Box boo
  {
  Size <3/2,3/2,3/2>
  }

Sphere boule_init
  {
  Radius 10
  Slices 5
  Stacks 5
  }

Translated boule
{
Translation <10,0,0>
Geometry boule_init
}

IFS ifs2
  {
  Depth 6
  Geometry boule
  TransfoList
    [
    Transfo  {
      Translation <1,1,1>
      Scale <1/3,1/3,1/3>
      EulerRotation
        {
        Azimuth 30
        Elevation 30
        }
      },
    Transfo  {
      Translation <-1,1,1>
      Scale <1/3,1/3,1/3> },
    Transfo  {
      Translation <1,-1,1>
      Scale <1/3,1/3,1/3> },
    Transfo  {
      Translation <-1,-1,1>
      Scale <1/3,1/3,1/3> },
    Transfo  {
      Translation <1,1,-1>
      Scale <1/3,1/3,1/3>
      EulerRotation
        {
        Azimuth -30
        Roll -30
        }
      },
    Transfo  {
      Translation <-1,1,-1>
      Scale <1/3,1/3,1/3> },
    Transfo  {
      Translation <1,-1,-1>
      Scale <1/3,1/3,1/3> },
    Transfo  {
      Translation <-1,-1,-1>
      Scale <1/3,1/3,1/3> }
    ]
  }

Shape{ifs2,RED7}
Shape{boule,RED7}
