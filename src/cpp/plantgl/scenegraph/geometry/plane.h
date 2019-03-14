/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 2012-20015 UMR CIRAD/INRIA/INRA DAP 
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

/*! \file plane.h
    \brief Definition of the class Plane3.
*/



#ifndef __pgl_plane_h__
#define __pgl_plane_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/math/util_vector.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/core/sceneobject.h>
#include <plantgl/math/util_math.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE
class Matrix3;
PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Plane3;
typedef RCPtr<Plane3> Plane3Ptr;

/* ----------------------------------------------------------------------- */

/**
    \class Plane3
    \brief A plane.
*/


class SG_API Plane3 : public RefCountObject
{

public:

  /// Constructs a Plane3.
  Plane3( const Vector3& normal = TOOLS(Vector3::OZ), real_t d = 0 );

  /// Constructs a Plane3.
  Plane3( const Vector3& normal, const Vector3& point );

  /// Constructs a Plane3.
  Plane3( real_t alpha, real_t beta, real_t d = 0 );

  /// Constructs a Plane3.
  Plane3( real_t a, real_t b, real_t c, real_t d );

  PGL_OBJECT_PROPERTY(Normal,Vector3);
  PGL_OBJECT_PROPERTY(D,real_t);

  real_t getDistance(const Vector3& point) const ;

  inline real_t a() const { return __Normal.x(); }
  inline real_t b() const { return __Normal.y(); }
  inline real_t c() const { return __Normal.z(); }
  inline real_t d() const { return __D; }

  inline Vector3 base1() {
      if (fabs(a()) < GEOM_EPSILON) return Vector3(1,0,0);
      else if (fabs(b()) < GEOM_EPSILON) return Vector3(0,1,0);
      else if (fabs(c()) < GEOM_EPSILON) return Vector3(0,0,1);
      return Vector3(-b(),a(),0);
  }

  inline Vector3 base2() {
      return cross(__Normal, base1());
  }
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __pgl_plane_h__
#endif
