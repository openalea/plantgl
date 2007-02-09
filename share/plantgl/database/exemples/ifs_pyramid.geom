(#
   An Iterative Function system on a pyramide
#)
Box box{ Size <1.0, 1.0, 1.0> }
Sphere ronde
{
  Radius 1
  Slices 4
  Stacks 4
}

Tapered pyramid
{
  BaseRadius 1.0
  TopRadius 0
  Primitive Box
    {
    Size <1.0, 1.0, 1.0>
    }
}


IFS _ifs
{
  Depth 7
  Geometry pyramid
  TransfoList
    [
    Transfo {
      Translation <0,0,0.5>
      Scale <1/2,1/2,1/2>
      EulerRotation
        {
        Azimuth 45
        }
      },
    Transfo {
      Translation <.7,.7,-0.5>
      Scale <1/2,1/2,1/2>
      EulerRotation
        {
        Elevation 90.
        Roll -45.
        }
      },
    Transfo { Translation <-0.7,0.7,-0.5>
      Scale <1/2,1/2,1/2>
      EulerRotation
        {
        Elevation -90.
        Roll -45.
        }
      },
    Transfo {
      Translation <0.7,-0.7,-0.5>
      Scale <1/2,1/2,1/2>
      EulerRotation
        {
        Elevation 90.
        Roll 45.
        }
      },
    Transfo {
      Translation <-0.7,-0.7,-0.5>
      Scale <1/2,1/2,1/2>
      EulerRotation
        {
        Elevation -90.
        Roll 45.
        }
      }
    ]
}

Shape{_ifs}
