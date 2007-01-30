/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
 *
 *       $Source$
 *       $Id$
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





#include "boundingbox.h"
#include <math/util_matrix.h>
#include <math/util_math.h>
using namespace std;
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

BoundingBox::BoundingBox( const Vector3& point ) :
  RefCountObject(),
  __ll(point),
  __ur(point) {
  GEOM_ASSERT(isValid());
}

BoundingBox::BoundingBox( const Vector3& lowLeft, const Vector3& upRight ) :
  RefCountObject(),
  __ll(lowLeft),
  __ur(upRight) {
  GEOM_ASSERT(isValid());
}

BoundingBox::~BoundingBox( ) {
}

/* ----------------------------------------------------------------------- */
void  BoundingBox::translate(const Vector3& t){
  __ll += t;
  __ur += t;
}

void  BoundingBox::scale(const Vector3& s){
  __ll.x() *= s.x();
  __ll.y() *= s.y();
  __ll.z() *= s.z();

  __ur.x() *= s.x();
  __ur.y() *= s.y();
  __ur.z() *= s.z();
}

/*
  The algorithm is taken from : Tranforming Axis-Aligned Bounding Boxes
  by James Arvo. Graphics Gems I - p 548-550.
  The only difference is that we do not have to take into account a
  translation (gloups,i've just do it).
  Meanwhile the degenerate interval used at the initialization of
  the algorithm is set to the center of the bounding box
  to be transformed.
  Note - i,j indices has to be transposed when accessing the matrix as
  we use column major order matrices.
*/
void  BoundingBox::transform(const Matrix3& matrix){
  Vector3 _minA = __ll;
  Vector3 _maxA = __ur;
  real_t _minBx = 0, _minBy = 0, _minBz = 0;
  real_t _maxBx = 0, _maxBy = 0, _maxBz = 0;
  real_t _a = matrix(0,0) * _minA.x();
  real_t _b = matrix(0,0) * _maxA.x();
  _minBx += min(_a,_b);
  _maxBx += max(_a,_b);
  _a = matrix(0,1) * _minA.y();
  _b = matrix(0,1) * _maxA.y();
  _minBx += min(_a,_b);
  _maxBx += max(_a,_b);
  _a = matrix(0,2) * _minA.z();
  _b = matrix(0,2) * _maxA.z();
  _minBx += min(_a,_b);
  _maxBx += max(_a,_b);
  _a = matrix(1,0) * _minA.x();
  _b = matrix(1,0) * _maxA.x();
  _minBy += min(_a,_b);
  _maxBy += max(_a,_b);
  _a = matrix(1,1) * _minA.y();
  _b = matrix(1,1) * _maxA.y();
  _minBy += min(_a,_b);
  _maxBy += max(_a,_b);
  _a = matrix(1,2) * _minA.z();
  _b = matrix(1,2) * _maxA.z();
  _minBy += min(_a,_b);
  _maxBy += max(_a,_b);
  _a = matrix(2,0) * _minA.x();
  _b = matrix(2,0) * _maxA.x();
  _minBz += min(_a,_b);
  _maxBz += max(_a,_b);
  _a = matrix(2,1) * _minA.y();
  _b = matrix(2,1) * _maxA.y();
  _minBz += min(_a,_b);
  _maxBz += max(_a,_b);
  _a = matrix(2,2) * _minA.z();
  _b = matrix(2,2) * _maxA.z();
  _minBz += min(_a,_b);
  _maxBz += max(_a,_b);

  __ll = Vector3(_minBx,_minBy,_minBz) ;
  __ur = Vector3(_maxBx,_maxBy,_maxBz) ;
}

void  BoundingBox::transform(const Matrix4& matrix){
  Vector3 translation(matrix(0,3),matrix(1,3),matrix(2,3));
  Vector3 _minA = __ll;
  Vector3 _maxA = __ur;
  real_t _minBx = 0, _minBy = 0, _minBz = 0;
  real_t _maxBx = 0, _maxBy = 0, _maxBz = 0;
  real_t _a = matrix(0,0) * _minA.x();
  real_t _b = matrix(0,0) * _maxA.x();
  _minBx += min(_a,_b);
  _maxBx += max(_a,_b);
  _a = matrix(0,1) * _minA.y();
  _b = matrix(0,1) * _maxA.y();
  _minBx += min(_a,_b);
  _maxBx += max(_a,_b);
  _a = matrix(0,2) * _minA.z();
  _b = matrix(0,2) * _maxA.z();
  _minBx += min(_a,_b);
  _maxBx += max(_a,_b);
  _a = matrix(1,0) * _minA.x();
  _b = matrix(1,0) * _maxA.x();
  _minBy += min(_a,_b);
  _maxBy += max(_a,_b);
  _a = matrix(1,1) * _minA.y();
  _b = matrix(1,1) * _maxA.y();
  _minBy += min(_a,_b);
  _maxBy += max(_a,_b);
  _a = matrix(1,2) * _minA.z();
  _b = matrix(1,2) * _maxA.z();
  _minBy += min(_a,_b);
  _maxBy += max(_a,_b);
  _a = matrix(2,0) * _minA.x();
  _b = matrix(2,0) * _maxA.x();
  _minBz += min(_a,_b);
  _maxBz += max(_a,_b);
  _a = matrix(2,1) * _minA.y();
  _b = matrix(2,1) * _maxA.y();
  _minBz += min(_a,_b);
  _maxBz += max(_a,_b);
  _a = matrix(2,2) * _minA.z();
  _b = matrix(2,2) * _maxA.z();
  _minBz += min(_a,_b);
  _maxBz += max(_a,_b);

  __ll = Vector3(_minBx,_minBy,_minBz) + translation;
  __ur = Vector3(_maxBx,_maxBy,_maxBz) + translation;
}


BoundingBox& BoundingBox::extend( const BoundingBoxPtr& bbox ) {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT((bbox) && (bbox->isValid()));
  if (bbox->__ll.x() < __ll.x()) __ll.x() = bbox->__ll.x();
  if (bbox->__ur.x() > __ur.x()) __ur.x() = bbox->__ur.x();
  if (bbox->__ll.y() < __ll.y()) __ll.y() = bbox->__ll.y();
  if (bbox->__ur.y() > __ur.y()) __ur.y() = bbox->__ur.y();
  if (bbox->__ll.z() < __ll.z()) __ll.z() = bbox->__ll.z();
  if (bbox->__ur.z() > __ur.z()) __ur.z() = bbox->__ur.z();
  return *this;
}

BoundingBox& BoundingBox::operator+=( const BoundingBox& bbox ) {
  return extend(bbox);
}

BoundingBox& BoundingBox::operator|=( const BoundingBox& bbox ) {
  return extend(bbox);
}

BoundingBox& BoundingBox::operator&=( const BoundingBox& bbox ) {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT((bbox) && (bbox.isValid()));
  if (bbox.__ll.x() > __ll.x()) __ll.x() = bbox.__ll.x();
  if (bbox.__ur.x() < __ur.x()) __ur.x() = bbox.__ur.x();
  if (bbox.__ll.y() > __ll.y()) __ll.y() = bbox.__ll.y();
  if (bbox.__ur.y() < __ur.y()) __ur.y() = bbox.__ur.y();
  if (bbox.__ll.z() > __ll.z()) __ll.z() = bbox.__ll.z();
  if (bbox.__ur.z() < __ur.z()) __ur.z() = bbox.__ur.z();
  return *this;
}

BoundingBox& BoundingBox::extend( const BoundingBox& bbox ) {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT((bbox) && (bbox.isValid()));
  if (bbox.__ll.x() < __ll.x()) __ll.x() = bbox.__ll.x();
  if (bbox.__ur.x() > __ur.x()) __ur.x() = bbox.__ur.x();
  if (bbox.__ll.y() < __ll.y()) __ll.y() = bbox.__ll.y();
  if (bbox.__ur.y() > __ur.y()) __ur.y() = bbox.__ur.y();
  if (bbox.__ll.z() < __ll.z()) __ll.z() = bbox.__ll.z();
  if (bbox.__ur.z() > __ur.z()) __ur.z() = bbox.__ur.z();
  return *this;
}

BoundingBox& BoundingBox::operator+=( const Vector3& point ) {
  return extend(point);
}

BoundingBox& BoundingBox::operator|=( const Vector3& point ) {
  return extend(point);
}

BoundingBox& BoundingBox::extend( const Vector3& point ) {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point.isValid());
  if (point.x() < __ll.x()) __ll.x() = point.x();
  else if (point.x() > __ur.x()) __ur.x() = point.x();
  if (point.y() < __ll.y()) __ll.y() = point.y();
  else if (point.y() > __ur.y()) __ur.y() = point.y();
  if (point.z() < __ll.z()) __ll.z() = point.z();
  else if (point.z() > __ur.z()) __ur.z() = point.z();
  return *this;
}

bool BoundingBox::intersect( const BoundingBoxPtr& bbox ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT((bbox) && (bbox->isValid()));
  if ( (bbox->__ll.x() > __ur.x()) || (bbox->__ur.x() < __ll.x()) ) return false;
  if ( (bbox->__ll.y() > __ur.y()) || (bbox->__ur.y() < __ll.y()) ) return false;
  if ( (bbox->__ll.z() > __ur.z()) || (bbox->__ur.z() < __ll.z()) ) return false;
  return true;
}

bool BoundingBox::intersect( const BoundingBox& bbox ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT((bbox) && (bbox.isValid()));
  if ( (bbox.__ll.x() > __ur.x()) || (bbox.__ur.x() < __ll.x()) ) return false;
  if ( (bbox.__ll.y() > __ur.y()) || (bbox.__ur.y() < __ll.y()) ) return false;
  if ( (bbox.__ll.z() > __ur.z()) || (bbox.__ur.z() < __ll.z()) ) return false;
  return true;
}

bool BoundingBox::intersect( const Vector3& point ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point.isValid());
  if ( (point.x() > __ur.x()) || (point.x() < __ll.x()) ) return false;
  if ( (point.y() > __ur.y()) || (point.y() < __ll.y()) ) return false;
  if ( (point.z() > __ur.z()) || (point.z() < __ll.z()) ) return false;
  return true;
}

bool BoundingBox::intersect( const Vector3& point1, const Vector3& point2 ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT(point1.isValid());
  GEOM_ASSERT(point2.isValid());


  if ( fabs(point1.x()-point2.x())< GEOM_EPSILON ){
      if ( (point1.x() > __ur.x() )|| (point1.x() < __ll.x()) ) return false;
  }
  else {

      /// First facet.
      real_t m = ( __ll.z() - point1.z() ) / (point2.z() - point1.z());
      real_t x0 = point1.x() + m * (point1.x() - point2.x());
      real_t y0 = point1.y() + m * (point1.y() - point2.y());
      if( x0 >= __ll.x() && x0 <= __ur.x() && y0 >= __ll.y() && y0 <= __ur.y()) return true;

      /// Second facet.
      m = ( __ur.z() - point1.z() ) / (point2.z() - point1.z());
      x0 = point1.x() + m * (point1.x() - point2.x());
      y0 = point1.y() + m * (point1.y() - point2.y());
      if( x0 >= __ll.x() && x0 <= __ur.x() && y0 >= __ll.y() && y0 <= __ur.y()) return true;

  }

  if ( fabs(point1.y()-point2.y())< GEOM_EPSILON ){
      if ( (point1.y() > __ur.y() )|| (point1.y() < __ll.y()) ) return false;
  }
  else {

      /// Third facet.
      real_t m = ( __ll.y() - point1.y() ) / (point2.y() - point1.y());
      real_t x0 = point1.x() + m * (point1.x() - point2.x());
      real_t z0 = point1.z() + m * (point1.z() - point2.z());
      if( x0 >= __ll.x() && x0 <= __ur.x() && z0 >= __ll.z() && z0 <= __ur.z()) return true;

      /// Forth facet.
      m = ( __ur.y() - point1.y() ) / (point2.y() - point1.y());
      x0 = point1.x() + m * (point1.x() - point2.x());
      z0 = point1.z() + m * (point1.z() - point2.z());
      if( x0 >= __ll.x() && x0 <= __ur.x() && z0 >= __ll.z() && z0 <= __ur.z()) return true;

  }

  if ( fabs(point1.z()-point2.z())< GEOM_EPSILON ){
      if ( (point1.z() > __ur.z() )|| (point1.z() < __ll.z()) ) return false;
  }
  else {

      /// Fifth facet.
      real_t m = ( __ll.x() - point1.x() ) / (point2.x() - point1.x());
      real_t y0 = point1.y() + m * (point1.y() - point2.y());
      real_t z0 = point1.z() + m * (point1.z() - point2.z());
      if( y0 >= __ll.y() && y0 <= __ur.y() && z0 >= __ll.z() && z0 <= __ur.z()) return true;

      /// sixth facet.
      m = ( __ur.x() - point1.x() ) / (point2.x() - point1.x());
      y0 = point1.y() + m * (point1.y() - point2.y());
      z0 = point1.z() + m * (point1.z() - point2.z());
      if( y0 >= __ll.y() && y0 <= __ur.y() && z0 >= __ll.z() && z0 <= __ur.z()) return true;

  }

  return false;
}

bool BoundingBox::include( const BoundingBox& bbox ) const {
  GEOM_ASSERT(isValid());
  GEOM_ASSERT((bbox) && (bbox.isValid()));
  if ( (bbox.__ll.x() < __ll.x()) || (bbox.__ur.x() > __ur.x()) ) return false;
  if ( (bbox.__ll.y() < __ll.y()) || (bbox.__ur.y() > __ur.y()) ) return false;
  if ( (bbox.__ll.z() < __ll.z()) || (bbox.__ur.z() > __ur.z()) ) return false;
  return true;
}

/* ----------------------------------------------------------------------- */
void BoundingBox::set( const Vector3& lowLeft, const Vector3& upRight ){
  __ll = lowLeft;
  __ur = upRight;
  GEOM_ASSERT(isValid());
}

void BoundingBox::change( const Vector3& center, const Vector3& size ){
  __ll = center - size/2;
  __ur = center + size/2;
  GEOM_ASSERT(isValid());
}

Vector3 BoundingBox::getCenter( ) const {
  GEOM_ASSERT(isValid());
  return (__ll + __ur)/2;
}

const Vector3& BoundingBox::getLowerLeftCorner( ) const {
  GEOM_ASSERT(isValid());
  return __ll;
}

Vector3& BoundingBox::getLowerLeftCorner( ) {
  GEOM_ASSERT(isValid());
  return __ll;
}

const Vector3& BoundingBox::getUpperRightCorner( ) const {
  GEOM_ASSERT(isValid());
  return __ur;
}

Vector3& BoundingBox::getUpperRightCorner( ) {
  GEOM_ASSERT(isValid());
  return __ur;
}

Vector3 BoundingBox::getSize( ) const {
  GEOM_ASSERT(isValid());
  return (__ur - __ll)/2;
}

bool BoundingBox::isValid( ) const {
  return
    (__ll.x() <= __ur.x()) &&
    (__ll.y() <= __ur.y()) &&
    (__ll.z() <= __ur.z());
}

real_t 
BoundingBox::getXRange() const { return __ur.x()- __ll.x(); }

real_t 
BoundingBox::getYRange() const { return __ur.y()- __ll.y(); }

real_t 
BoundingBox::getZRange() const { return __ur.z()- __ll.z(); }

real_t 
BoundingBox::getXMin() const { return __ll.x(); }

real_t 
BoundingBox::getYMin() const { return __ll.y(); }

real_t 
BoundingBox::getZMin() const { return __ll.z(); }

real_t 
BoundingBox::getXMax() const { return __ur.x(); }

real_t 
BoundingBox::getYMax() const { return __ur.y(); }

real_t 
BoundingBox::getZMax() const { return __ur.z(); }

/* ----------------------------------------------------------------------- */

BoundingBox PGL(operator+)( const BoundingBox& b1, const BoundingBox& b2) {
  return BoundingBox(b1).extend(b2);
}

BoundingBox PGL(operator|)( const BoundingBox& b1, const BoundingBox& b2) {
  return BoundingBox(b1).extend(b2);
}

BoundingBox PGL(operator&)( const BoundingBox& b1, const BoundingBox& b2) {
  return (BoundingBox(b1) &= (b2));
}

BoundingBox PGL(operator+)( const BoundingBox& b1, const Vector3& v2) {
  return BoundingBox(b1).extend(v2);
}

BoundingBox PGL(operator|)( const BoundingBox& b1, const Vector3& v2) {
  return BoundingBox(b1).extend(v2);
}

bool PGL(intersection)( const BoundingBox& b1, const BoundingBox& b2) {
  return b1.intersect(b2);
}

bool PGL(intersection)( const BoundingBox& b1, const Vector3& v2) {
  return b1.intersect(v2);
}

ostream& operator<<( ostream& stream, BoundingBox& b ) {
  return stream << "[" << b.getLowerLeftCorner() << "," << b.getUpperRightCorner() << "]" << endl;
}
