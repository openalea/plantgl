/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
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
