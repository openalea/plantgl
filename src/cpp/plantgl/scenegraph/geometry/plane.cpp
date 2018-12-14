/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
 *
 *  ----------------------------------------------------------------------------
 *
 *                      GNU General Public Licence
 *
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */





#include "plane.h"
#include <plantgl/math/util_matrix.h>
#include <plantgl/math/util_math.h>
using namespace std;
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

  /// Constructs a Plane3.
Plane3::Plane3( const TOOLS(Vector3)& _normal, const TOOLS(Vector3)& _point):
      __Normal(direction(_normal)), __D() { __D = dot(__Normal,_point);  GEOM_ASSERT(__Normal.isNormalized()); }

  /// Constructs a Plane3.
Plane3::Plane3( const TOOLS(Vector3)& _normal , real_t _d ):
      __Normal(direction(_normal)), __D(_d) { GEOM_ASSERT(__Normal.isNormalized()); }

Plane3::Plane3( real_t alpha, real_t beta, real_t _d ):
      __Normal(Vector3::Spherical(1.0,alpha,beta)),__D(_d) { }


Plane3::Plane3( real_t a, real_t b, real_t c, real_t d ):
      __Normal(Vector3(a,b,c)),__D(d) { }

real_t Plane3::getDistance(const TOOLS(Vector3)& point) const {
    return dot(__Normal,point) - __D;
}