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




#include "boundingsphere.h"
#include <plantgl/math/util_matrix.h>
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_polymath.h>
using namespace std;
PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

BoundingSphere::BoundingSphere( const Vector3& point ) :
  RefCountObject(),
  __center(point),
  __radius(0.0) {
  GEOM_ASSERT(isValid());
}

BoundingSphere::BoundingSphere( const Vector2& point ) :
  RefCountObject(),
  __center(Vector3(point,0)),
  __radius(0.0) {
  GEOM_ASSERT(isValid());
}

BoundingSphere::BoundingSphere( const Vector3& center, const real_t& radius) :
  RefCountObject(),
  __center(center),
  __radius(radius) {
  GEOM_ASSERT(isValid());
}

BoundingSphere::BoundingSphere( const Vector3& point1, const Vector3& point2 ) :
  RefCountObject(),
  __center((point1+point2)/2),
  __radius(norm(point1-point2)/2) {
  GEOM_ASSERT(isValid());
}

BoundingSphere::~BoundingSphere( ) {
}

/* ----------------------------------------------------------------------- */

void  BoundingSphere::transform(const Matrix4& matrix){
  Vector3 translation, rotation, scaling;
  matrix.getTransformation(scaling,rotation,translation);
  __center += translation;
  __radius *= *(scaling.getMax());
}


BoundingSphere& BoundingSphere::extend( const BoundingSpherePtr& bsphere ) {
  GEOM_ASSERT(bsphere);
  return extend(*bsphere);
}

BoundingSphere& BoundingSphere::extend( const BoundingSphere& bsphere ) {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(bsphere.isValid());
  Vector3 center = (__center*__radius + bsphere.__center*bsphere.__radius)/(__radius+bsphere.__radius);
  real_t radius = max(norm(__center-center)+__radius,norm(bsphere.__center-center)+bsphere.__radius);
  change(center,radius);
  return *this;
}


BoundingSphere& BoundingSphere::extend( const Vector3& point ) {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point.isValid());
  real_t dist = norm(__center - point);
  __radius = max(dist,__radius);
  return *this;
}

BoundingSphere& BoundingSphere::extend( const Vector2& point ) {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point.isValid());
  real_t dist = norm(__center - Vector3(point,0));
  __radius = max(dist,__radius);
  return *this;
}

BoundingSphere&
BoundingSphere::operator+=( const BoundingSpherePtr& bsphere ){
  GEOM_ASSERT(bsphere);
  return extend(*bsphere);
}

BoundingSphere&
BoundingSphere::operator+=( const BoundingSphere& bsphere ){
  return extend(bsphere);
}

BoundingSphere&
BoundingSphere::operator+=( const Vector3& point ){
  return extend(point);
}

BoundingSphere&
BoundingSphere::operator+=( const Vector2& point ){
  return extend(point);
}

bool BoundingSphere::intersect( const BoundingSpherePtr& bsphere ) const {
  GEOM_ASSERT(bsphere);
  return intersect(*bsphere);
}

bool BoundingSphere::intersect( const BoundingSphere& bsphere ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(bsphere.isValid());
  if(normSquared(__center-bsphere.__center) - sq(__radius+bsphere.__radius) < GEOM_EPSILON) return true;
  return false;
}

bool BoundingSphere::intersect( const Vector3& point ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point.isValid());
  if ( normSquared(__center - point) - sq(__radius) < GEOM_EPSILON) return true;
  return true;
}

bool BoundingSphere::intersect( const Vector3& point1, const Vector3& point2 ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point1.isValid());
  GEOM_ASSERT(point2.isValid());

  Vector3 d = point2 - point1;
  real_t a = normSquared(d);
  if(a < GEOM_EPSILON)return intersect(point1);

  Vector3 o = point1 - __center;
  real_t c = normSquared(o) - sq(__radius);
  real_t b = d.x()*o.x() + d.y()*o.y() + d.z()*o.z();

  real_t x1,x2;
  if(!solve(a,b,c,x1,x2))return false;
  if(x1 > - GEOM_EPSILON && x1 < 1 + GEOM_EPSILON)return true;
  if(x2 > - GEOM_EPSILON && x2 < 1 + GEOM_EPSILON)return true;

  return false;
}


/* ----------------------------------------------------------------------- */

void BoundingSphere::change( const Vector3& center, const real_t& radius){
  __radius = radius;
  __center = center;
  GEOM_ASSERT(isValid());
}

const Vector3& BoundingSphere::getCenter( ) const {
  GEOM_ASSERT(isValid());
  return __center;
}

Vector3& BoundingSphere::getCenter( ) {
  GEOM_ASSERT(isValid());
  return __center;
}

const real_t& BoundingSphere::getRadius( ) const {
  GEOM_ASSERT(isValid());
  return __radius;
}

real_t& BoundingSphere::getRadius( ) {
  GEOM_ASSERT(isValid());
  return __radius;
}

bool BoundingSphere::isValid( ) const {
  return ( __radius > GEOM_EPSILON ) ;
}

/* ----------------------------------------------------------------------- */

BoundingSphere operator+( const BoundingSphere& b1, const BoundingSphere& b2) {
  return BoundingSphere(b1).extend(b2);
}

BoundingSphere operator+( const BoundingSphere& b1, const Vector3& v2) {
  return BoundingSphere(b1).extend(v2);
}

bool intersection( const BoundingSphere& b1, const BoundingSphere& b2) {
  return b1.intersect(b2);
}

bool intersection( const BoundingSphere& b1, const Vector3& v2) {
  return b1.intersect(v2);
}

ostream& operator<<( ostream& stream, BoundingSphere& b ) {
  return stream << "[" << b.getCenter() << "," << b.getRadius() << "]" << endl;
}
