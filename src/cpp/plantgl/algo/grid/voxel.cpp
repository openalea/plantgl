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




#include "voxel.h"
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/scenegraph/geometry/box.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

#ifdef _WIN32
AppearancePtr Voxel::EMPTY_APPEARANCE;
AppearancePtr Voxel::FILLED_APPEARANCE;
AppearancePtr Voxel::UNDETERMINED_APPEARANCE;

#else

AppearancePtr Voxel::EMPTY_APPEARANCE(new Material("VXL_EMPTY_APPEARANCE",
                                                Color3::WHITE,
                              Material::DEFAULT_DIFFUSE,
                              Material::DEFAULT_SPECULAR,
                              Material::DEFAULT_EMISSION,
                              Material::DEFAULT_SHININESS,
                              0.2));
AppearancePtr Voxel::FILLED_APPEARANCE(new Material("VXL_FILLED_APPEARANCE",
                                                 Color3::BLUE,
                               Material::DEFAULT_DIFFUSE,
                               Material::DEFAULT_SPECULAR,
                               Material::DEFAULT_EMISSION,
                               Material::DEFAULT_SHININESS,
                               0.2));
AppearancePtr Voxel::UNDETERMINED_APPEARANCE(new Material("VXL_UNDETERMINED_APPEARANCE",
                                                       Color3::GREEN,
                                 Material::DEFAULT_DIFFUSE,
                                 Material::DEFAULT_SPECULAR,
                                 Material::DEFAULT_EMISSION,
                                 Material::DEFAULT_SHININESS,
                                 0.2));
#endif

/* ----------------------------------------------------------------------- */

Voxel::Voxel( Tile * Complex,
          const unsigned char Scale,
          const TileType Type,
          const unsigned char Num,
          const Vector3& PMin,
          const Vector3& PMax) :
    Tile(Complex, Scale,Type,Num),
    __ll(PMin),
    __ur(PMax){
}


Voxel::~Voxel(){
    /// Nothing to do.
}

bool Voxel::isValid() const {
  return
    (__ll.x() <= __ur.x()) &&
    (__ll.y() <= __ur.y()) &&
    (__ll.z() <= __ur.z());
}

/* ----------------------------------------------------------------------- */

void Voxel::setBBox(const BoundingBox& bbox){
    __ll = bbox.getLowerLeftCorner();
    __ur = bbox.getUpperRightCorner();
}

void Voxel::setBBox(const BoundingBoxPtr& bbox){
    __ll = bbox->getLowerLeftCorner();
    __ur = bbox->getUpperRightCorner();
}

/* ----------------------------------------------------------------------- */

Shape3DPtr Voxel::representation() const{

#ifdef _WIN32
    if(!EMPTY_APPEARANCE)
    EMPTY_APPEARANCE = AppearancePtr (new Material("VXL_EMPTY_APPEARANCE",
                              Color3::WHITE,
                              Material::DEFAULT_DIFFUSE,
                              Material::DEFAULT_SPECULAR,
                              Material::DEFAULT_EMISSION,
                              Material::DEFAULT_SHININESS,
                              0.2));
    if(!FILLED_APPEARANCE)
    FILLED_APPEARANCE = AppearancePtr (new Material("VXL_FILLED_APPEARANCE",
                                Color3::BLUE,
                               Material::DEFAULT_DIFFUSE,
                               Material::DEFAULT_SPECULAR,
                               Material::DEFAULT_EMISSION,
                               Material::DEFAULT_SHININESS,
                               0.2));
    if(!UNDETERMINED_APPEARANCE )
    UNDETERMINED_APPEARANCE = AppearancePtr (new Material("VXL_UNDETERMINED_APPEARANCE",
                                 Color3::GREEN,
                                 Material::DEFAULT_DIFFUSE,
                                 Material::DEFAULT_SPECULAR,
                                 Material::DEFAULT_EMISSION,
                                 Material::DEFAULT_SHININESS,
                                 0.2));
#endif
    AppearancePtr mat;
    if(__type == Empty){
      mat = EMPTY_APPEARANCE;
    }
    else if(__type == Filled /*|| !isDecomposed() */ ){
      mat = FILLED_APPEARANCE;
    }
    else  {
      mat = UNDETERMINED_APPEARANCE;
    }
    ShapePtr res(new Shape(GeometryPtr(new Translated(getCenter(),GeometryPtr(new Box(getSize())))),mat));
    res->setName("VXL_"+getName());
    if(__scale < 8){
      uint_t id = (uint_t)__num;
      uint_t base = 1;
      const Tile * comp = getComplex();
      while(comp != NULL){
    base *=  comp->getComponentsSize();
    id += base *  (uint_t)comp->getNum();
    comp = comp->getComplex();
      }
      res->getId() = id;
    }
    return Shape3DPtr(res);
}

/* ----------------------------------------------------------------------- */

bool Voxel::intersect( const Voxel& voxel ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(voxel.isValid());
  if ( (voxel.__ll.x() > __ur.x()) || (voxel.__ur.x() < __ll.x()) ) return false;
  if ( (voxel.__ll.y() > __ur.y()) || (voxel.__ur.y() < __ll.y()) ) return false;
  if ( (voxel.__ll.z() > __ur.z()) || (voxel.__ur.z() < __ll.z()) ) return false;
  return true;
}

bool Voxel::intersect( const BoundingBox& bbox ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(bbox.isValid());
  if ( (bbox.getLowerLeftCorner().x() > __ur.x()) || (bbox.getUpperRightCorner().x() < __ll.x()) ) return false;
  if ( (bbox.getLowerLeftCorner().y() > __ur.y()) || (bbox.getUpperRightCorner().y() < __ll.y()) ) return false;
  if ( (bbox.getLowerLeftCorner().z() > __ur.z()) || (bbox.getUpperRightCorner().z() < __ll.z()) ) return false;
  return true;
}

bool Voxel::intersect( const Vector3& point ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point.isValid());
  if ( (point.x() > __ur.x()) || (point.x() < __ll.x()) ) return false;
  if ( (point.y() > __ur.y()) || (point.y() < __ll.y()) ) return false;
  if ( (point.z() > __ur.z()) || (point.z() < __ll.z()) ) return false;
  return true;
}

bool Voxel::intersect( const Vector2& point ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point.isValid());
  if ( (point.x() > __ur.x()) || (point.x() < __ll.x()) ) return false;
  if ( (point.y() > __ur.y()) || (point.y() < __ll.y()) ) return false;
  return true;
}

bool Voxel::intersect( const Vector3& point1, const Vector3& point2 ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point1.isValid());
  GEOM_ASSERT(point2.isValid());

  Vector3 deltaP = point2 - point1;

  if ( fabs(deltaP.z())< GEOM_EPSILON ){
      if ( (point1.z() > __ur.z() )|| (point1.z() < __ll.z()) ) return false;
  }
  else {


      /// First facet.
      real_t m = ( __ll.z() - point1.z() ) / (deltaP.z());
      if(m > 0 && m <1.0){
    real_t x0 = point1.x() + m * (deltaP.x());
    real_t y0 = point1.y() + m * (deltaP.y());
    if( x0 >= __ll.x() && x0 <= __ur.x() && y0 >= __ll.y() && y0 <= __ur.y()){
#ifdef GEOM_DEBUG
      cerr << "Voxel :" << __ur << " , " << __ll << endl;
      cerr << "Point1 : " << point1 << " , Point2 : " << point2 << " , deltaP : " << deltaP << endl;
      cerr << "Intersect First facet on <" << x0 << ',' << y0 << ',' << __ll.z() << ">. (m=" << m << ')' << endl;
#endif
      return true;
    }
      }

      /// Second facet.
      m = ( __ur.z() - point1.z() ) / (deltaP.z());
      if(m > 0 && m <1.0){
    real_t x0 = point1.x() + m * (deltaP.x());
    real_t y0 = point1.y() + m * (deltaP.y());
    if( x0 >= __ll.x() && x0 <= __ur.x() && y0 >= __ll.y() && y0 <= __ur.y()){
#ifdef GEOM_DEBUG
      cerr << "Voxel :" << __ur << " , " << __ll << endl;
      cerr << "Point1 : " << point1 << " , Point2 : " << point2 << " , deltaP : " << deltaP << endl;
      cerr << "Intersect Second facet on <" << x0 << ',' << y0 << ',' << __ur.z() << ">. (m=" << m << ')' << endl;
#endif
      return true;
    }
      }
  }

  if ( fabs(deltaP.y())< GEOM_EPSILON ){
      if ( (point1.y() > __ur.y() )|| (point1.y() < __ll.y()) ) return false;
  }
  else {

      /// Third facet.
      real_t m = ( __ll.y() - point1.y() ) / (deltaP.y());
      if(m > 0 && m <1.0){
    real_t x0 = point1.x() + m * (deltaP.x());
    real_t z0 = point1.z() + m * (deltaP.z());
    if( x0 >= __ll.x() && x0 <= __ur.x() && z0 >= __ll.z() && z0 <= __ur.z()){
#ifdef GEOM_DEBUG
      cerr << "Voxel :" << __ur << " , " << __ll << endl;
      cerr << "Point1 : " << point1 << " , Point2 : " << point2 << " , deltaP : " << deltaP << endl;
      cerr << "Intersect Third facet on <" << x0 << ',' << __ll.y() << ',' << z0 << ">. (m=" << m << ')' << endl;
#endif
      return true;
    }
      }

      /// Forth facet.
      m = ( __ur.y() - point1.y() ) / (deltaP.y());
      if(m > 0 && m <1.0){
    real_t x0 = point1.x() + m * (deltaP.x());
    real_t z0 = point1.z() + m * (deltaP.z());
    if( x0 >= __ll.x() && x0 <= __ur.x() && z0 >= __ll.z() && z0 <= __ur.z()){
#ifdef GEOM_DEBUG
      cerr << "Voxel :" << __ur << " , " << __ll << endl;
      cerr << "Point1 : " << point1 << " , Point2 : " << point2 << " , deltaP : " << deltaP << endl;
      cerr << "Intersect Forth facet on <" << x0 << ',' << __ur.y() << ',' << z0 << ">. (m=" << m << ')' << endl;
#endif
      return true;
    }
      }
  }

  if ( fabs(deltaP.x())< GEOM_EPSILON ){
      if ( (point1.x() > __ur.x() )|| (point1.x() < __ll.x()) ) return false;
  }
  else {

      /// Fifth facet.
      real_t m = ( __ll.x() - point1.x() ) / (deltaP.x());
      if(m > 0 && m <1.0){
    real_t y0 = point1.y() + m * (deltaP.y());
    real_t z0 = point1.z() + m * (deltaP.z());
    if( y0 >= __ll.y() && y0 <= __ur.y() && z0 >= __ll.z() && z0 <= __ur.z()){
#ifdef GEOM_DEBUG
      cerr << "Voxel :" << __ur << " , " << __ll << endl;
      cerr << "Point1 : " << point1 << " , Point2 : " << point2 << " , deltaP : " << deltaP << endl;
      cerr << "Intersect Fifth facet on <" << __ll.x() << ',' << y0 << ',' << z0 << ">. (m=" << m << ')' << endl;
#endif
      return true;
    }
      }

      /// Sixth facet.
      m = ( __ur.x() - point1.x() ) / (deltaP.x());
      if(m > 0 && m <1.0){
    real_t y0 = point1.y() + m * (deltaP.y());
    real_t z0 = point1.z() + m * (deltaP.z());
    if( y0 >= __ll.y() && y0 <= __ur.y() && z0 >= __ll.z() && z0 <= __ur.z()){
#ifdef GEOM_DEBUG
      cerr << "Voxel :" << __ur << " , " << __ll << endl;
      cerr << "Point1 : " << point1 << " , Point2 : " << point2 << " , deltaP : " << deltaP << endl;
      cerr << "Intersect Sixth facet on <" << __ur.x() << ',' << y0 << ',' << z0 << ">. (m=" << m << ')' << endl;
#endif
      return true;
    }
      }
  }
  return false;
}


bool Voxel::intersect( const Vector2& point1, const Vector2& point2 ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point1.isValid());
  GEOM_ASSERT(point2.isValid());

  Vector2 deltaP = point2 - point1;

  if ( fabs(point1.x()-point2.x())< GEOM_EPSILON ){
      if ( (point1.x() > __ur.x() )|| (point1.x() < __ll.x()) ) return false;
  }
  else {

      /// First facet.
      real_t m = ( __ll.x() - point1.x() ) / (deltaP.x());
      if(m > 0 && m <1.0){
    real_t y0 = point1.y() + m * (deltaP.y());
    if( y0 >= __ll.y() && y0 <= __ur.y()) return true;
      }

      /// Second facet.
      m = ( __ur.x() - point1.x() ) / (deltaP.x());
      if(m > 0 && m <1.0){
    real_t y0 = point1.y() + m * (deltaP.y());
    if( y0 >= __ll.y() && y0 <= __ur.y()) return true;
      }

  }

  if ( fabs(point1.y()-point2.y())< GEOM_EPSILON ){
      if ( (point1.y() > __ur.y() )|| (point1.y() < __ll.y()) ) return false;
  }
  else {

      /// Third facet.
      real_t m = ( __ll.y() - point1.y() ) / (deltaP.y());
      if(m > 0 && m <1.0){
    real_t x0 = point1.x() + m * (deltaP.x());
    if( x0 >= __ll.x() && x0 <= __ur.x()) return true;
      }

      /// Forth facet.
      m = ( __ur.y() - point1.y() ) / (deltaP.y());
      if(m > 0 && m <1.0){
    real_t x0 = point1.x() + m * (deltaP.x());
    if( x0 >= __ll.x() && x0 <= __ur.x()) return true;
      }

  }
  return false;
}



#define TRIANGLE_INTERSECTION_TEST \
          mu = dot((point1 - Pline),n)/dn; \
      if(mu > -GEOM_EPSILON && (mu - 1) < GEOM_EPSILON){ \
          Vector3 w = d * mu + Pline - point1; \
           \
          real_t alpha = dot(w,u)*vv - dot(w,v)* uv; \
          alpha /= denominator; \
           \
          real_t beta = dot(w,v)*uu - dot(w,u)* uv; \
          beta /= denominator; \
           \
          if( alpha < -GEOM_EPSILON || alpha-1 > GEOM_EPSILON || beta < -GEOM_EPSILON || beta-1 > GEOM_EPSILON); \
          else if( (alpha + beta - 1) > GEOM_EPSILON ); \
          else return true; \
      } \



bool Voxel::intersect( const Vector3& point1, const Vector3& point2 , const Vector3& point3) const {

/* Test if an edge of the voxel cross the triangle */

  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point1.isValid());
  GEOM_ASSERT(point2.isValid());
  GEOM_ASSERT(point3.isValid());

  if(point1 == point2) return false;
  if(point1 == point3) return false;
  if(point2 == point3) return false;

  Vector3 u(point2 - point1);
  Vector3 v(point3 - point1);
  Vector3 n(cross(u,v));

  real_t uu = dot(u,u);
  real_t vv = dot(v,v);
  real_t uv = dot(u,v);
  real_t denominator = uu*vv-pow(uv,2);
  real_t mu;

  if(fabs(denominator)> GEOM_EPSILON){
      /// Test of edge align on z-axis.
      Vector3 d(0,0,__ur.z()-__ll.z());
      Vector3 Pline(__ll);
      real_t dn = dot(d,n);

      if( fabs(dn) > GEOM_EPSILON ){

    /// Edge align on z-axis 0-2
    TRIANGLE_INTERSECTION_TEST;

    /// Edge align on z-axis 1-3
    Pline.x() = __ur.x();
    TRIANGLE_INTERSECTION_TEST;

    /// Edge align on z-axis 5-7
    Pline.y() = __ur.y();
    TRIANGLE_INTERSECTION_TEST;

    /// Edge align on z-axis 4-6
    Pline.x() = __ll.x();
    TRIANGLE_INTERSECTION_TEST;
      }

      /// Test on edge align on y-axis.
      d = Vector3(0,__ur.y()-__ll.y(),0);
      Pline.y() = __ll.y(); /// Reinit P_line at __ll.
      dn = dot(d,n);

      if( fabs(dn) > GEOM_EPSILON ){

      /// Edge align on y-axis 0-4
      TRIANGLE_INTERSECTION_TEST;

      /// Edge align on y-axis 1-5
      Pline.x() = __ur.x();
      TRIANGLE_INTERSECTION_TEST;

      /// Edge align on y-axis 3-7
      Pline.z() = __ur.z();
      TRIANGLE_INTERSECTION_TEST;

      /// Edge align on y-axis 2-6
      Pline.x() = __ll.x();
      TRIANGLE_INTERSECTION_TEST;
      }

      /// Test on edge align on x-axis.
      d = Vector3(__ur.x()-__ll.x(),0,0);
      Pline.z() = __ll.z(); /// Reinit P_line at __ll.
      dn = dot(d,n);

      if( fabs(dn) > GEOM_EPSILON ){

      /// Edge align on x-axis 0-1
      TRIANGLE_INTERSECTION_TEST;

      /// Edge align on x-axis 4-5
      Pline.y() = __ur.y();
      TRIANGLE_INTERSECTION_TEST;

      /// Edge align on x-axis 6-7
      Pline.z() = __ur.z();
      TRIANGLE_INTERSECTION_TEST;

      /// Edge align on x-axis 2-3
      Pline.y() = __ll.y();
      TRIANGLE_INTERSECTION_TEST;

      }
  }
  return false;
}

#define QUAD_INTERSECTION_TEST \
      mu = dot(( point1 - Pline),n)/dn; \
      if(!(mu < -GEOM_EPSILON || mu - 1 > GEOM_EPSILON)){ \
          Vector3 w = d * mu + Pline - point1; \
          \
          real_t alpha = dot(w,u)*vv - dot(w,v)* uv; \
          alpha /= denominator; \
          \
          real_t beta = dot(w,v)*uu - dot(w,u)* uv; \
          beta /= denominator; \
          \
          if( alpha < -GEOM_EPSILON || alpha-1 > GEOM_EPSILON || beta < -GEOM_EPSILON || beta-1 > GEOM_EPSILON); \
          else if( (alpha + beta - 1) > GEOM_EPSILON ); \
          else return true; \
          \
          alpha = dot(w,u_)*vv_ - dot(w,v_)* uv_; \
          alpha /= denominator_; \
          \
          beta = dot(w,v_)*uu_ - dot(w,u_)* uv_; \
          beta /= denominator_; \
          \
          if( alpha < -GEOM_EPSILON || alpha-1 > GEOM_EPSILON || beta < -GEOM_EPSILON || beta-1 > GEOM_EPSILON); \
          else if( (alpha + beta - 1) > GEOM_EPSILON ); \
          else return true; \
      } \



bool Voxel::intersect( const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector3& point4) const {

/* Test if an edge of the voxel cross the facet */

  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point1.isValid());
  GEOM_ASSERT(point2.isValid());
  GEOM_ASSERT(point3.isValid());
  GEOM_ASSERT(point4.isValid());
  if(point1 == point2){
    if(point1 == point3) return false;
    if(point1 == point4) return false;
    if(point3 == point4) return false;
    return intersect(point1,point3,point4);
  }
  else {
    // point1 != point2
    if(point1 == point3){
      if(point1 == point4) return false;
      if(point2 == point4) return false;
      return intersect(point1,point2,point4);
    }
    // point1 != point3
    if(point1 == point4){
      if(point2 == point3) return false;
      return intersect(point1,point2,point3);
    }
    // point1 != point4
    if(point2 == point3){
      if(point2 == point4) return false;
      return intersect(point1,point2,point4);
    }
    // point2 != point3.
    if(point2 == point4){
      return intersect(point1,point2,point3);
    }
    // point2 != point4.
    if(point3 == point4){
      return intersect(point1,point2,point3);
    }
  }

  Vector3 u(point2 - point1);
  Vector3 v(point3 - point1);
  Vector3 u_(point3 - point1);
  Vector3 v_(point4 - point1);
  Vector3 n(cross(u,v));

  real_t uu = dot(u,u);
  real_t vv = dot(v,v);
  real_t uv = dot(u,v);
  real_t denominator = uu*vv-pow(uv,2);

  real_t uu_ = vv;
  real_t vv_ = dot(v_,v_);
  real_t uv_ = dot(v,v_);
  real_t denominator_ = uu_*vv_-pow(uv_,2);

  real_t mu;

  if(fabs(denominator)> GEOM_EPSILON || fabs(denominator_)> GEOM_EPSILON ){

      /// Test onf edge align on z-axis.
      Vector3 d(0,0,__ur.z()-__ll.z());
      Vector3 Pline(__ll);
      real_t dn = dot(d,n);

      if(!( fabs(dn) < GEOM_EPSILON) ){

      /// Edge align on z-axis 0-2
      QUAD_INTERSECTION_TEST;

      /// Edge align on z-axis 1-3
      Pline.x() = __ur.x();
      QUAD_INTERSECTION_TEST;

      /// Edge align on z-axis 5-7
      Pline.y() = __ur.y();
      QUAD_INTERSECTION_TEST;

      /// Edge align on z-axis 4-6
      Pline.x() = __ll.x();
      QUAD_INTERSECTION_TEST;

      }

      /// Test on edge align on y-axis.
      d = Vector3(0,__ur.y()-__ll.y(),0);
      Pline.y() = __ll.y(); /// Reinit P_line at __ll.
      dn = dot(d,n);

      if(!( fabs(dn) < GEOM_EPSILON) ){

      /// Edge align on y-axis 0-4
      QUAD_INTERSECTION_TEST;

      /// Edge align on y-axis 1-5
      Pline.x() = __ur.x();
      QUAD_INTERSECTION_TEST;

      /// Edge align on y-axis 3-7
      Pline.z() = __ur.z();
      QUAD_INTERSECTION_TEST;

      /// Edge align on y-axis 2-6
      Pline.x() = __ll.x();
      QUAD_INTERSECTION_TEST;

      }

      /// Test on edge align on x-axis.
      d = Vector3(__ur.x()-__ll.x(),0,0);
      Pline.z() = __ll.z(); /// Reinit P_line at __ll.
      dn = dot(d,n);

      if(!( fabs(dn) < GEOM_EPSILON) ){

      /// Edge align on x-axis 0-1
      QUAD_INTERSECTION_TEST;

      /// Edge align on x-axis 4-5
      Pline.y() = __ur.y();
      QUAD_INTERSECTION_TEST;

      /// Edge align on x-axis 6-7
      Pline.z() = __ur.z();
      QUAD_INTERSECTION_TEST;

      /// Edge align on x-axis 2-3
      Pline.y() = __ll.y();
      QUAD_INTERSECTION_TEST;

      }
  }
  return false;
}

/* ----------------------------------------------------------------------- */

bool PGL(intersection)( const Voxel& v1, const Voxel& v2) {
    return v1.intersect(v2);
}

bool PGL(intersection)( const Voxel& v1, const BoundingBox& b2) {
    return v1.intersect(b2);
}

bool PGL(intersection)( const Voxel& v1, const BoundingBoxPtr& b2) {
    return v1.intersect(*b2);
}

bool PGL(intersection)( const Voxel& v1, const Vector3& v2) {
    return v1.intersect(v2);
}

bool PGL(intersection)( const Voxel& v1, const Vector3& seg1, const Vector3& seg2) {
    if(v1.intersect(seg1))return true;
    if(v1.intersect(seg2))return true;
    return v1.intersect(seg1,seg2);
}

bool PGL(intersection)( const Voxel& v1, const Vector3& triangle1, const Vector3& triangle2, const Vector3& triangle3) {
    if(v1.intersect(triangle1))return true;
    if(v1.intersect(triangle2))return true;
    if(v1.intersect(triangle3))return true;
    if(v1.intersect(triangle1,triangle2))return true;
    if(v1.intersect(triangle1,triangle3))return true;
    if(v1.intersect(triangle2,triangle3))return true;
    return v1.intersect(triangle1,triangle2,triangle3);
}

bool PGL(intersection)( const Voxel& v1, const Vector3& quad1, const Vector3& quad2, const Vector3& quad3, const Vector3& quad4) {
    if(v1.intersect(quad1))return true;
    if(v1.intersect(quad2))return true;
    if(v1.intersect(quad3))return true;
    if(v1.intersect(quad4))return true;
    if(v1.intersect(quad1,quad2))return true;
    if(v1.intersect(quad2,quad3))return true;
    if(v1.intersect(quad3,quad4))return true;
    if(v1.intersect(quad4,quad1))return true;
    return v1.intersect(quad1,quad2,quad3,quad4);
}

/* ----------------------------------------------------------------------- */
