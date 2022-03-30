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


/*! \file ray.h
    \brief Definition of Ray.
*/



#ifndef __ray_h__
#define __ray_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/math/util_vector.h>
#include "../algo_config.h"
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/scenegraph/geometry/boundingsphere.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class Ray
    \brief A Ray is a point 3D and a direction.
*/

/* ----------------------------------------------------------------------- */

class ALGO_API Ray  {

    public :

  /*! Default constructor.
    \pre
    - \e Direction must not be = Vector3(0,0,0);
  */
  Ray(const Vector3& Origin = Vector3::ORIGIN,
      const Vector3& Direction = Vector3::OX);


  /// Destructor
  virtual ~Ray();


  /// Validity check.
  virtual bool isValid() const ;

  /// Return origin of \e self.
  inline const Vector3& getOrigin() const {
    return __origin ;
  }

  /// Return origin of \e self.
  inline Vector3& getOrigin() {
    return __origin ;
  }

  /// Return direction of \e self.
  inline const Vector3& getDirection() const {
    return __direction ;
  }

  /// Return direction of \e self.
  inline Vector3& getDirection() {
    return __direction ;
  }

  /// Return point \e t on the ray.
  inline Vector3 getAt(const real_t t) const {
    return __origin + __direction * t ;
  }

  /*! Intersection between \e self and \e point.
    \pre
    - \e self must be valid;
    - \e point must be valid. */
  bool intersect( const Vector3& point ) const;

  /*! Intersection between projection of \e self on z=0 and \e point.
    \pre
    - \e self must be valid;
    - \e point must be valid. */
  bool intersect( const Vector2& point ) const;

  /*! Intersection between  \e self and a segment defined by [ \e point1 , \e point2 ].
    \pre
    - \e self must be valid;
    - \e point1 and \e point2 must be valid.
    \post
    - \e intersection contains the intersection if returned value is 1
    \return  type of intersection.
    \retval -1 ray is degenerate
    \retval 0 disjoint (no intersect)
    \retval 1 intersect in unique point \e intersection.
    \retval 2 are equivalent
  */
  int intersect( const Ray& ray, Vector3& intersection, real_t& t ) const;

  /*! Intersection between  \e self and a segment defined by [ \e point1 , \e point2 ].
    \pre
    - \e self must be valid;
    - \e point1 and \e point2 must be valid.
    \post
    - \e intersection contains the intersection if returned value is 1
    \return  type of intersection.
    \retval -1 segment is degenerate
    \retval 0 disjoint (no intersect)
    \retval 1 intersect in unique point \e intersection.
    \retval 2 segment is in the ray
  */
  int intersect( const Vector3& point1, const Vector3& point2, Vector3& intersection ) const;

  /*! Intersection between projection of \e self on z=0 and a segment defined by [ \e point1 , \e point2 ].
    \pre
    - \e self must be valid;
    - \e point1 and \e point2 must be valid.
    \post
    - \e intersection contains the intersection if returned value is 1
    \return  type of intersection.
    \retval -1 segment is degenerate
    \retval 0 disjoint (no intersect)
    \retval 1 intersect in unique point \e intersection
    \retval 2 are in the same plane
  */
  int intersect( const Vector2& point1, const Vector2& point2, Vector2& intersection ) const;

  /*! Intersection between \e self and a triangle defined by [ \e triangle1 , \e triangle2 , \e triangle3 ].
    \pre
    - \e self must be valid;
    - \e triangle1, \e triangle2  and \e triangle3 must be valid.
    \post
    - \e intersection contains the intersection if returned value is 1
    \return  type of intersection.
    \retval -1 triangle is degenerate (a segment or point)
    \retval 0 disjoint (no intersect)
    \retval 1 intersect in unique point \e intersection
    \retval 2 are in the same plane
  */
  int intersect( const Vector3& triangle1, const Vector3& triangle2, const Vector3& triangle3, Vector3& intersection   ) const;

  /*! Intersection between \e self and a quad facet defined by [ \e quad1 , \e quad2 , \e quad3 , \e quad4 ].
    \pre
    - \e self must be valid;
    - \e facet must be planar and convex;
    - \e quad1, \e quad2, \e quad3 and \e quad4 must be valid.
    \post
    - \e intersection contains the intersection if returned value is 1
    \return  type of intersection.
    \retval -1 triangle is degenerate (a segment or point)
    \retval 0 disjoint (no intersect)
    \retval 1 intersect in unique point \e intersection
    \retval 2 are in the same plane
  */
  int intersect( const Vector3& quad1, const Vector3& quad2, const Vector3& quad3, const Vector3& quad4, Vector3& intersection   ) const;



  /*! Intersection between \e self and a sphere defined by \e center and rayon \e r.
    \pre
    - \e self must be valid;
    - \e facet must be planar and convex;
    - \e center, \e r must be valid.
    \post
    - \e intersection contains the intersection if returned value is > 0
    \return  type of intersection.
    \retval 0 disjoint (no intersect)
    \retval 1 intersect in unique point \e intersection
    \retval 2 are in 2 points.
  */
  int intersect( const Vector3& center,const real_t& r,
         Vector3& intersection1, Vector3& intersection2   ) const;


  /*! Intersection between \e self and a ellipsoid defined by \e center and [ \e a , \e b , \e c ].
    \pre
    - \e self must be valid;
    - \e facet must be planar and convex;
    - \e center, \e a, \e b and \e c must be valid.
    \post
    - \e intersection contains the intersection if returned value is > 0
    \return  type of intersection.
    \retval 0 disjoint (no intersect)
    \retval 1 intersect in unique point \e intersection
    \retval 2 are in 2 points.
  */
  int intersect( const Vector3& center,const real_t& a, const real_t& b, const real_t& c,
         Vector3& intersection1, Vector3& intersection2   ) const;


  /// Intersection with a bounding box
  bool intersect(const BoundingBox& bbox, real_t& tnear, real_t& tfar) const ;

  /// Intersection with a bounding sphere
  bool intersect(const BoundingSphere& bsphere) const ;

  /// Intersection with a bounding box
  bool intersect(const BoundingBoxPtr& bbox, real_t& tnear, real_t& tfar) const ;

  /// Intersection with a bounding sphere
  bool intersect(const BoundingSpherePtr& bsphere) const ;

  /// Intersection of a ray with a bounding box
  friend ALGO_API bool intersection(const Ray& ray, const BoundingBox& bbox);

  /// Intersection of a ray with a bounding sphere
  friend ALGO_API bool intersection(const Ray& ray, const BoundingSphere& bsphere);

  /// Intersection of a ray with a bounding box
  friend ALGO_API bool intersection(const Ray& ray, const BoundingBoxPtr& bbox);

  /// Intersection of a ray with a bounding sphere
  friend ALGO_API bool intersection(const Ray& ray, const BoundingSpherePtr& bsphere);

protected :

  /// origin of \e self.
  Vector3 __origin;

  /// direction of \e self.
  Vector3 __direction;

};

ALGO_API bool intersection(const Ray& ray, const BoundingBox& bbox);
ALGO_API bool intersection(const Ray& ray, const BoundingSphere& bsphere);
ALGO_API bool intersection(const Ray& ray, const BoundingBoxPtr& bbox);
ALGO_API bool intersection(const Ray& ray, const BoundingSpherePtr& bsphere);

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

