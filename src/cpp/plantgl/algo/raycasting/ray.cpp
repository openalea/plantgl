/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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

#include "ray.h"
#include <plantgl/math/util_polymath.h>
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_matrix.h>

PGL_USING_NAMESPACE


/* ----------------------------------------------------------------------- */


Ray::Ray(const Vector3& Origin,
         const Vector3& Direction) :
  __origin(Origin),
  __direction(Direction){
  __direction.normalize();
  GEOM_ASSERT(__direction.isValid() && __direction != Vector3::ORIGIN);
}

Ray::~Ray(){
  /// Nothing to do.
}


bool
Ray::isValid() const {
  return (__direction.isValid() && __direction != Vector3::ORIGIN);
}


/* ----------------------------------------------------------------------- */


bool
Ray::intersect( const Vector3& point ) const
{
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point.isValid());
  // Just test if ||AB ^ d||  < epsilon
  Vector3 AB( point - __origin);
  return  normLinf(AB ^ __direction) < GEOM_EPSILON;
}

bool
Ray::intersect( const Vector2& point ) const
{
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point.isValid());
  Vector2 d(Vector2(__origin.x(),__origin.y())-point);
  return ( fabs( d.x() / __direction.x() - d.y() / __direction.y()) < GEOM_EPSILON);
}


int Ray::intersect( const Ray& ray, Vector3& intersection, real_t& t ) const
{
    Vector3 _deltaOrig(ray.getOrigin()-__origin);
    Vector3 _crossDirs = cross(__direction,ray.getDirection());
    real_t ncd = norm(_crossDirs);
    if (ncd < GEOM_EPSILON){
        if (intersect(ray.getOrigin())) return 2;
        else return 0;
    }
    Matrix3 m(_deltaOrig,ray.getDirection(),_crossDirs);
    t = m.det() / (ncd * ncd);
    intersection = getAt(t);
    if (ray.intersect(intersection)) return 1;
    else return 0;
}

int
Ray::intersect( const Vector3& point1, const Vector3& point2, Vector3& intersection ) const
{
  Vector3 dir(point2-point1);
  real_t ndir = dir.normalize();
  real_t t;
  int res = intersect(Ray(point1,dir),intersection,t);
  if (res == 1){
      if ( t <= ndir) return 1;
      else return 0;
  }
  else return res;
 /* Vector3 _deltaOrig(point1-__origin);
  Vector3 d2 = point2 - point1;
  real_t a =  d2.y()*__direction.x() - d2.x()*__direction.y();
  if( fabs(a) < GEOM_EPSILON )
    return -1;
  real_t b = _deltaOrig.x()*__direction.y() - _deltaOrig.y()*__direction.x();
  real_t beta = b / a;
  real_t c = _deltaOrig.x()*d2.y() - _deltaOrig.y()*d2.x();
  real_t alpha = c / a;
  if( beta < GEOM_EPSILON || beta  - 1 > -GEOM_EPSILON )
    return 0;
  if( alpha < -GEOM_EPSILON )
    return 0;
  if( fabs(alpha * __direction.z() - beta * d2.z() - _deltaOrig.z()) > GEOM_EPSILON )
    return 0;
  intersection = __origin + __direction * alpha;
  return 1; */
}


int
Ray::intersect( const Vector2& point1, const Vector2& point2, Vector2& intersection ) const
{
  Vector2 _deltaOrig(point2-point1);
  Vector2 d2 = point2 - point1;
  real_t a =  d2.y()*__direction.x() - d2.x()*__direction.y();
  if( fabs(a) < GEOM_EPSILON )
    return -1;
  real_t b = _deltaOrig.x()*__direction.y() - _deltaOrig.y()*__direction.x();
  real_t beta = b / a;
  real_t c = _deltaOrig.x()*d2.y() - _deltaOrig.y()*d2.x();
  real_t alpha = c / a;
  if( beta < GEOM_EPSILON || beta  - 1> -GEOM_EPSILON )
    return 0;
  if( alpha < -GEOM_EPSILON )
    return 0;
  intersection = point1 + d2 * beta;
  return 1;
}


int
Ray::intersect( const Vector3& point1, const Vector3& point2, const Vector3& point3, Vector3& intersection  ) const
{

//  Vector3    dir, w0, w;          // ray vectors
  float     r, a, b;             // params to calc ray-plane intersect

  // get triangle edge vectors and plane normal
  Vector3 u(point2 - point1);
  Vector3 v(point3 - point1);
  Vector3 n(cross(u,v));             // cross product

  if (n == Vector3::ORIGIN)            // triangle is degenerate
    return -1;                 // do not deal with this case

//  dir = R.P1 - R.P0;             // ray direction vector
  Vector3 w0(__origin - point1);
  a = dot(n,w0);
  b = dot(n,__direction);
  if (fabs(b) < GEOM_EPSILON) {     // ray is parallel to triangle plane
    if (a == 0)                // ray lies in triangle plane
      return 2;
    else return 0;             // ray disjoint from plane
  }

  // get intersect point of ray with triangle plane
  r = -a / b;
  if (r < 0.0)                   // ray goes away from triangle
    return 0;                  // => no intersect
  // for a segment, also test if (r > 1.0) => no intersect

  intersection = __origin +  __direction * r;           // intersect point of ray and plane

  // is I inside T?
  float    uu, uv, vv, wu, wv, D;
  uu = dot(u,u);
  uv = dot(u,v);
  vv = dot(v,v);
  Vector3 w(intersection - point1);
  wu = dot(w,u);
  wv = dot(w,v);
  D = uv * uv - uu * vv;

  // get and test parametric coords
  float s, t;
  s = (uv * wv - vv * wu) / D;
  if (s < 0.0 || s > 1.0)        // I is outside T
    return 0;
  t = (uv * wu - uu * wv) / D;
  if (t < 0.0 || (s + t) > 1.0)  // I is outside T
    return 0;

  return 1;                      // I is in T
}

int
Ray::intersect( const Vector3& quad1, const Vector3& quad2, const Vector3& quad3, const Vector3& quad4, Vector3& intersection ) const
{
  Vector3 u(quad2 - quad1);
  Vector3 v(quad3 - quad1);
  Vector3 n(cross(u,v));

  real_t dn = dot(__direction,n);

  if(!( fabs(dn) < GEOM_EPSILON ) ){

    real_t mu = dot(( quad1 - __origin),n)/dn;
    Vector3 w = __direction * mu + __origin - quad1;

    real_t uu = dot(u,u);
    real_t vv = dot(v,v);
    real_t uv = dot(u,v);
    real_t denominator = uu*vv-static_cast<real_t>(pow(uv,2));
    if( !(fabs(denominator)< GEOM_EPSILON) ) {

      real_t alpha = dot(w,u)*vv - dot(w,v)* uv;
      alpha /= denominator;

      real_t beta = dot(w,v)*uu - dot(w,u)* uv;
      beta /= denominator;

      if( alpha < -GEOM_EPSILON || alpha-1 > GEOM_EPSILON || beta < -GEOM_EPSILON || beta-1 > GEOM_EPSILON);
      else if( (alpha + beta - 1) > GEOM_EPSILON );
      else {
    intersection = w;
    return 1;
      }
    }
  }

  u = quad3 - quad1;
  v = quad4 - quad1;
  n = cross(u,v); // if planar, can be ignored.
  dn = dot(__direction,n);

  if( fabs(dn) < GEOM_EPSILON ) return 0;

  real_t mu = dot(( quad1 - __origin),n)/dn;
  if(mu < -GEOM_EPSILON || mu - 1 > GEOM_EPSILON) return 0;
  Vector3 w = __direction * mu + __origin - quad1;

  real_t uu = dot(u,u);
  real_t vv = dot(v,v);
  real_t uv = dot(u,v);
  real_t denominator = uu*vv-static_cast<real_t>(pow(uv,2));
  if(fabs(denominator)< GEOM_EPSILON) return 0;

  real_t alpha = dot(w,u)*vv - dot(w,v)* uv;
  alpha /= denominator;

  real_t beta = dot(w,v)*uu - dot(w,u)* uv;
  beta /= denominator;

  if( alpha < -GEOM_EPSILON || alpha-1 > GEOM_EPSILON || beta < -GEOM_EPSILON || beta-1 > GEOM_EPSILON) return 0;
  if( (alpha + beta - 1) > GEOM_EPSILON ) return 0;
  else {
    intersection = w;
    return 1;
  }
}

/* ----------------------------------------------------------------------- */

int
Ray::intersect( const Vector3& origin,const real_t& r,
            Vector3& intersection1, Vector3& intersection2   ) const {
  Vector3 delta(__origin - origin);
  real_t f = sq(__direction.x()) + sq(__direction.y()) + sq(__direction.z());
  real_t s = (__direction.x() + delta.x()) + (__direction.y() + delta.y()) + (__direction.z() + delta.z());
  s *= 2;
  real_t t = sq(delta.x()) + sq(delta.y()) + sq(delta.z()) - sq(r);
  real_t x1, x2;
  if(!solve(f,s,t,x1,x2)) return 0;
  else if(x1 == x2){
    intersection1 = intersection2 = __origin + __direction * x1;
    return 1;
  }
  else {
    intersection1 = __origin + __direction * x1;
    intersection2 = __origin + __direction * x2;
    return 2;
  }
}



int
Ray::intersect( const Vector3& origin,const real_t& a, const real_t& b, const real_t& c,
            Vector3& intersection1, Vector3& intersection2   ) const {
  real_t aa = sq(a);
  real_t bb = sq(b);
  real_t cc = sq(c);
  Vector3 delta(__origin - origin);
  real_t f = (sq(__direction.x()) / aa) + (sq(__direction.y()) / bb) + (sq(__direction.z()) / cc);
  real_t s = (__direction.x() + delta.x())/ aa + (__direction.y() + delta.y())/ bb + (__direction.z() + delta.z()) / cc;
  s *= 2;
  real_t t = (sq(delta.x()) / aa) + (sq(delta.y()) / bb) + (sq(delta.z()) / cc) - 1;
  real_t x1, x2;
  if(!solve(f,s,t,x1,x2)) return 0;
  else if(x1 == x2){
    intersection1 = intersection2 = __origin + __direction * x1;
    return 1;
  }
  else {
    intersection1 = __origin + __direction * x1;
    intersection2 = __origin + __direction * x2;
    return 2;
  }
}


/* ----------------------------------------------------------------------- */

bool
Ray::intersect(const BoundingBoxPtr& bbox, real_t& ta, real_t& tb) const {
    GEOM_ASSERT(bbox);
    return intersect(*bbox,ta,tb);
}
/*
bool
Ray::intersect(const BoundingBox& bbox) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(bbox.isValid());
  const Vector3& __ll = bbox.getLowerLeftCorner();
  const Vector3& __ur = bbox.getUpperRightCorner();

  if ( fabs(__direction.x())< GEOM_EPSILON ){
      if ( (__origin.x() > __ur.x() )|| (__origin.x() < __ll.x()) ) return false;
  }
  else {

      /// First facet.
      real_t m = ( __ll.z() - __origin.z() ) / __direction.z();
      real_t x0 = __origin.x() - m * (__direction.x());
      real_t y0 = __origin.y() - m * (__direction.y());
      if( x0 >= __ll.x() && x0 <= __ur.x() && y0 >= __ll.y() && y0 <= __ur.y()) return true;

      /// Second facet.
      m = ( __ur.z() - __origin.z() ) / (__direction.z());
      x0 = __origin.x() - m * __direction.x();
      y0 = __origin.y() - m * __direction.y();
      if( x0 >= __ll.x() && x0 <= __ur.x() && y0 >= __ll.y() && y0 <= __ur.y()) return true;

  }

  if ( fabs(__direction.y())< GEOM_EPSILON ){
      if ( (__origin.y() > __ur.y() )|| (__origin.y() < __ll.y()) ) return false;
  }
  else {

      /// Third facet.
      real_t m = ( __ll.y() - __origin.y() ) / __direction.y();
      real_t x0 = __origin.x() - m * __direction.x();
      real_t z0 = __origin.z() - m * __direction.z();
      if( x0 >= __ll.x() && x0 <= __ur.x() && z0 >= __ll.z() && z0 <= __ur.z()) return true;

      /// Forth facet.
      m = ( __ur.y() - __origin.y() ) / __direction.y();
      x0 = __origin.x() - m * __direction.x();
      z0 = __origin.z() - m * __direction.z();
      if( x0 >= __ll.x() && x0 <= __ur.x() && z0 >= __ll.z() && z0 <= __ur.z()) return true;

  }

  if ( fabs(__direction.z())< GEOM_EPSILON ){
      if ( (__origin.z() > __ur.z() )|| (__origin.z() < __ll.z()) ) return false;
  }
  else {

      /// Fifth facet.
      real_t m = ( __ll.x() - __origin.x() ) / __direction.x();
      real_t y0 = __origin.y() - m * __direction.y();
      real_t z0 = __origin.z() - m * __direction.z();
      if( y0 >= __ll.y() && y0 <= __ur.y() && z0 >= __ll.z() && z0 <= __ur.z()) return true;

      /// sixth facet.
      m = ( __ur.x() - __origin.x() ) / __direction.x();
      y0 = __origin.y() + m * __direction.y();
      z0 = __origin.z() + m * __direction.z();
      if( y0 >= __ll.y() && y0 <= __ur.y() && z0 >= __ll.z() && z0 <= __ur.z()) return true;

  }

  return false;
}
*/
bool
Ray::intersect(const BoundingBox& bbox, real_t& ta, real_t& tb) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(bbox.isValid());
  real_t tnear = -REAL_MAX;
  real_t tfar = REAL_MAX;

  const Vector3& ll = bbox.getLowerLeftCorner();
  const Vector3& ur = bbox.getUpperRightCorner();

  for (uchar_t i = 0; i < 3; ++i){
      if ( fabs(__direction[i]) < GEOM_EPSILON ){
          if ( (__origin[i] > ur[i] )|| (__origin[i] < ll[i]) ) return false;
      }
      else {
          real_t t1 = ( ll[i] - __origin[i] ) / __direction[i];
          real_t t2 = ( ur[i] - __origin[i] ) / __direction[i];
          if (t1 > t2) {
              // swap(t1,t2)
              real_t ti = t2; t2 = t1; t1 = ti;
          }
          if (t1 > tnear) tnear = t1;
          if (t2 < tfar)  tfar = t2;
              if (tnear > tfar) return false; // box is missed
          if (tfar < 0)  return false; // box is behind ray
      }
  }
  ta = tnear;
  tb = tfar;
  return true;
}

bool
Ray::intersect(const BoundingSpherePtr& bsphere) const {
    GEOM_ASSERT(bsphere);
    return intersect(*bsphere);
}

bool
Ray::intersect(const BoundingSphere& bsphere) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(bsphere.isValid());

  real_t a = normSquared(__direction);
  if(a < GEOM_EPSILON)return bsphere.intersect(__origin);

  Vector3 o = __origin - bsphere.getCenter();
  real_t c = normSquared(o) - sq(bsphere.getRadius());
  real_t b = __direction.x()*o.x() + __direction.y()*o.y() + __direction.z()*o.z();

  real_t x1,x2;
  if(!solve(a,b,c,x1,x2))return false;
  if(x1 > - GEOM_EPSILON && x1 < 1 + GEOM_EPSILON)return true;
  if(x2 > - GEOM_EPSILON && x2 < 1 + GEOM_EPSILON)return true;

  return false;
}

bool intersection(const Ray& ray, const BoundingBox& bbox){
    real_t t1,t2;
    return ray.intersect(bbox,t1,t2);
}

bool intersection(const Ray& ray, const BoundingSphere& bsphere){
    return ray.intersect(bsphere);
}

bool intersection(const Ray& ray, const BoundingBoxPtr& bbox){
    GEOM_ASSERT(bbox);
    real_t t1,t2;
    return ray.intersect(*bbox,t1,t2);
}

bool intersection(const Ray& ray, const BoundingSpherePtr& bsphere){
    GEOM_ASSERT(bsphere);
    return ray.intersect(*bsphere);
}
