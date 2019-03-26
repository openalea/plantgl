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

#include "util_intersection.h"
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

bool PGL(intersectSegment)(const Vector3& seg1, const Vector3& seg2 ,
              const Vector3& pt1){
  Vector3 AB( pt1 - seg1);
  Vector3 dir( seg2 - seg1);
  if (normLinf(AB ^ dir) < GEOM_EPSILON) return false;
  real_t u = norm(AB) / norm(dir);
  return ( (u > - GEOM_EPSILON) && (u < 1 - GEOM_EPSILON));
}

bool PGL(intersectSegment)(const Vector3& seg1, const Vector3& seg2 ,
           const Vector3& segb1,const Vector3& segb2){
  Vector3 _deltaOrig(segb1-seg1);
  Vector3 d1 = seg2 - seg1;
  Vector3 d2 = segb2 - segb1;
  real_t a =  d2.y()*d1.x() - d2.x()*d1.y();
  if(fabs(a) < GEOM_EPSILON) return false;
  real_t b = _deltaOrig.x()*d1.y() - _deltaOrig.y()*d1.x();
  real_t beta = b / a;
  real_t c = _deltaOrig.x()*d2.y() - _deltaOrig.y()*d2.x();
  real_t alpha = c / a;
  if(beta < GEOM_EPSILON || beta  - 1 > -GEOM_EPSILON)return false;
  if(alpha < -GEOM_EPSILON || alpha - 1 > GEOM_EPSILON)return false;
  if(fabs(alpha * d1.z() - beta * d2.z() - _deltaOrig.z()) > GEOM_EPSILON) return false;
  return true;
}

bool PGL(intersectSegment)(const Vector2& seg1, const Vector2& seg2 ,
           const Vector2& segb1,const Vector2& segb2){
  Vector2 _deltaOrig(segb1-seg1);
  Vector2 d1 = seg2 - seg1;
  Vector2 d2 = segb2 - segb1;
  real_t a =  d2.y()*d1.x() - d2.x()*d1.y();
  if(fabs(a) < GEOM_EPSILON) return false;
  real_t b = _deltaOrig.x()*d1.y() - _deltaOrig.y()*d1.x();
  real_t beta = b / a;
  real_t c = _deltaOrig.x()*d2.y() - _deltaOrig.y()*d2.x();
  real_t alpha = c / a;
  if(beta < GEOM_EPSILON || beta  - 1> -GEOM_EPSILON)return false;
  if(alpha < -GEOM_EPSILON || alpha  - 1 > GEOM_EPSILON)return false;
  return true;
}

bool PGL(intersectSegment)(const Vector3& seg1, const Vector3& seg2 ,
           const Vector3& triangle1,const Vector3& triangle2, const Vector3& triangle3 ){

  Vector3 u(triangle2 - triangle1);
  Vector3 v(triangle3 - triangle1);
  Vector3 n(cross(u,v));

  Vector3 d(seg2-seg1);
  Vector3 Pline(seg1);
  real_t dn = dot(d,n);

  if( fabs(dn) < GEOM_EPSILON ) return false;

  real_t mu = dot(( triangle1 - Pline),n)/dn;
  if(mu < -GEOM_EPSILON || mu - 1 > GEOM_EPSILON) return false;

//      Vector3 Pi = Pline + mu * d;
  Vector3 w = d * mu + Pline - triangle1;

  real_t uu = dot(u,u);
  real_t vv = dot(v,v);
  real_t uv = dot(u,v);
  real_t denominator = uu*vv-pow(uv,2);
  if(fabs(denominator)< GEOM_EPSILON) return false;

  real_t alpha = dot(w,u)*vv - dot(w,v)* uv;
  alpha /= denominator;

  real_t beta = dot(w,v)*uu - dot(w,u)* uv;
  beta /= denominator;

  if( alpha < -GEOM_EPSILON || alpha-1 > GEOM_EPSILON || beta < -GEOM_EPSILON || beta-1 > GEOM_EPSILON) return false;
  if( (alpha + beta - 1) > GEOM_EPSILON ) return false;
  else return true;

/*
  if( alpha < -GEOM_EPSILON || alpha-1 > GEOM_EPSILON || beta < -GEOM_EPSILON || beta-1 > GEOM_EPSILON) return false;
  if( (alpha + beta - 1) > GEOM_EPSILON ) return false;
  if( (alpha >= GEOM_EPSILON) && (alpha-1 <= -GEOM_EPSILON) &&
      (beta >= GEOM_EPSILON) && (beta-1 <= -GEOM_EPSILON) &&
      ((alpha + beta - 1) <= -GEOM_EPSILON) )
      return true; /// inside triangle.
  else if(alpha < GEOM_EPSILON) { // along edge t1->t2
      if(beta < GEOM_EPSILON) return true; // at vertex t1.
      else if(beta-1 > -GEOM_EPSILON) return true; // at vertex t2.
      else true; // on edge not near any vertex.
  }
  else if(beta < GEOM_EPSILON) { // along edge t1->t3
      if(alpha < GEOM_EPSILON) return true; // at vertex t1.
      else if(alpha-1 > -GEOM_EPSILON) return true; // at vertex t3.
      else true; // on edge not near any vertex.
  }
  else if((alpha + beta -1) > GEOM_EPSILON) return true; // on edge t2->t3
  else return false;
*/

}

bool PGL(intersectSegment)(const Vector3& seg1, const Vector3& seg2 ,
           const Vector3& quad1,const Vector3& quad2, const Vector3& quad3 , const Vector3& quad4 ){

  Vector3 u(quad2 - quad1);
  Vector3 v(quad3 - quad1);
  Vector3 n(cross(u,v));

  Vector3 d(seg2-seg1);
  Vector3 Pline(seg1);
  real_t dn = dot(d,n);

  if(!( fabs(dn) < GEOM_EPSILON ) ){

      real_t mu = dot(( quad1 - Pline),n)/dn;
      if(!(mu < -GEOM_EPSILON || mu - 1 > GEOM_EPSILON)){
      Vector3 w = d * mu + Pline - quad1;

      real_t uu = dot(u,u);
      real_t vv = dot(v,v);
      real_t uv = dot(u,v);
      real_t denominator = uu*vv-pow(uv,2);
      if( !(fabs(denominator)< GEOM_EPSILON) ) {

          real_t alpha = dot(w,u)*vv - dot(w,v)* uv;
          alpha /= denominator;

          real_t beta = dot(w,v)*uu - dot(w,u)* uv;
          beta /= denominator;

          if( alpha < -GEOM_EPSILON || alpha-1 > GEOM_EPSILON || beta < -GEOM_EPSILON || beta-1 > GEOM_EPSILON);
          else if( (alpha + beta - 1) > GEOM_EPSILON );
          else return true;
      }
      }
  }

  u = quad3 - quad1;
  v = quad4 - quad1;
  n = cross(u,v); // if planar, can be ignored.
  dn = dot(d,n);

  if( fabs(dn) < GEOM_EPSILON ) return false;

  real_t mu = dot(( quad1 - Pline),n)/dn;
  if(mu < -GEOM_EPSILON || mu - 1 > GEOM_EPSILON) return false;
  Vector3 w = d * mu + Pline - quad1;

  real_t uu = dot(u,u);
  real_t vv = dot(v,v);
  real_t uv = dot(u,v);
  real_t denominator = uu*vv-pow(uv,2);
  if(fabs(denominator)< GEOM_EPSILON) return false;

  real_t alpha = dot(w,u)*vv - dot(w,v)* uv;
  alpha /= denominator;

  real_t beta = dot(w,v)*uu - dot(w,u)* uv;
  beta /= denominator;

  if( alpha < -GEOM_EPSILON || alpha-1 > GEOM_EPSILON || beta < -GEOM_EPSILON || beta-1 > GEOM_EPSILON) return false;
  if( (alpha + beta - 1) > GEOM_EPSILON ) return false;
  else return true;
}
