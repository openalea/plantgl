/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) 
 *
 *       $Source$
 *       $Id: beam_ray.h 1697 2006-06-16 09:18:30Z barbier $
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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
  Ray(const TOOLS(Vector3)& Origin = TOOLS(Vector3)::ORIGIN,
	  const TOOLS(Vector3)& Direction = TOOLS(Vector3)::OX);
    
    
  /// Destructor
  virtual ~Ray();
  
  
  /// Validity check.
  virtual bool isValid() const ;
  
  /// Return origin of \e self.
  inline const TOOLS(Vector3)& getOrigin() const {
    return __origin ;
  }
    
  /// Return origin of \e self.
  inline TOOLS(Vector3)& getOrigin() {
    return __origin ;
  }
    
  /// Return direction of \e self.
  inline const TOOLS(Vector3)& getDirection() const {
    return __direction ;
  }
    
  /// Return direction of \e self.
  inline TOOLS(Vector3)& getDirection() {
    return __direction ;
  }
    
  /// Return point \e t on the ray.
  inline TOOLS(Vector3) getAt(const real_t t) const {
    return __origin + __direction * t ;
  }
        
  /*! Intersection between \e self and \e point.
    \pre
    - \e self must be valid;
    - \e point must be valid. */
  bool intersect( const TOOLS(Vector3)& point ) const;

  /*! Intersection between projection of \e self on z=0 and \e point.
    \pre
    - \e self must be valid;
    - \e point must be valid. */
  bool intersect( const TOOLS(Vector2)& point ) const;

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
  int intersect( const Ray& ray, TOOLS(Vector3)& intersection, real_t& t ) const;

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
  int intersect( const TOOLS(Vector3)& point1, const TOOLS(Vector3)& point2, TOOLS(Vector3)& intersection ) const;
  
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
  int intersect( const TOOLS(Vector2)& point1, const TOOLS(Vector2)& point2, TOOLS(Vector2)& intersection ) const;
  
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
  int intersect( const TOOLS(Vector3)& triangle1, const TOOLS(Vector3)& triangle2, const TOOLS(Vector3)& triangle3, TOOLS(Vector3)& intersection   ) const;

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
  int intersect( const TOOLS(Vector3)& quad1, const TOOLS(Vector3)& quad2, const TOOLS(Vector3)& quad3, const TOOLS(Vector3)& quad4, TOOLS(Vector3)& intersection   ) const;



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
  int intersect( const TOOLS(Vector3)& center,const real_t& r, 
		 TOOLS(Vector3)& intersection1, TOOLS(Vector3)& intersection2   ) const;


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
  int intersect( const TOOLS(Vector3)& center,const real_t& a, const real_t& b, const real_t& c, 
		 TOOLS(Vector3)& intersection1, TOOLS(Vector3)& intersection2   ) const;


  /// Intersection with a bounding box
  bool intersect(const BoundingBox& bbox) const ;

  /// Intersection with a bounding sphere
  bool intersect(const BoundingSphere& bsphere) const ;

  /// Intersection with a bounding box
  bool intersect(const BoundingBoxPtr& bbox) const ;

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
  TOOLS(Vector3) __origin;
  
  /// direction of \e self.
  TOOLS(Vector3) __direction;
  
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

