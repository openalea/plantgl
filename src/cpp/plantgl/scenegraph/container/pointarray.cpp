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



#include "pointarray.h"
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


Point2Array::Point2Array( uint_t size ) :
  Array1<Vector2>(size) {
  __A.reserve(size);
  GEOM_ASSERT(isValid());
}

Point2Array::Point2Array( uint_t size, const Vector2& t):
    Array1<Vector2>(size,t){
}

Point2Array::Point2Array( size_t size, const Vector2& firstval, const Vector2& inc):
Array1<Vector2>(range<Array1<Vector2> >(size,firstval,inc)){
}


Point2Array::Point2Array( Vector2 a, Vector2 b ) :
    Array1<Vector2>(2){
    __A[0] = a;
    __A[1] = b;
}

Point2Array::~Point2Array( ) {
}

pair<Vector2,Vector2> Point2Array::getBounds( ) const {
  Vector2 _ll, _ur;

  if (__A.size()) {
    _ll = _ur = *__A.begin();

    for (const_iterator _i = __A.begin() + 1; _i < __A.end(); _i++) {

      if (_i->x() < _ll.x()) _ll.x() = _i->x();
      else if (_i->x() > _ur.x()) _ur.x() = _i->x();

      if (_i->y() < _ll.y()) _ll.y() = _i->y();
      else if (_i->y() > _ur.y()) _ur.y() = _i->y();
    };
  };

  return pair<Vector2,Vector2>(_ll,_ur);
}


Vector2 Point2Array::getCenter( ) const {
  Vector2 _center;
  if (__A.size()) {
    _center = *__A.begin();
    for (const_iterator _i = __A.begin() + 1; _i < __A.end(); _i++)
      _center += *_i;
    _center /= (real_t)__A.size();
  };
  return _center;
}


Vector2 Point2Array::getExtent( ) const {
  Vector2 _extent;
  pair<Vector2,Vector2> _bounds = getBounds();
    _extent = _bounds.second - _bounds.first;
  return _extent;
}


real_t Point2Array::getLength( ) const {
  real_t _length = 0.0;

  Point2Array::const_iterator _pi = __A.begin();
  Point2Array::const_iterator _pj = __A.begin();
  while (_pj != __A.end()) {
    _pj++;
    Vector2 _vpi(*_pi);
    Vector2 _vpj(*_pj);
    _length += norm(_vpj-_vpi);
    _pi = _pj;
  };

  return _length;
}


Point2Array::const_iterator Point2Array::getXMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() > _result->x()) _result = _i;
  return _result;
}


Point2Array::const_iterator Point2Array::getXMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _result->x()) _result = _i;
  return _result;
}


pair<Point2Array::const_iterator,Point2Array::const_iterator>
Point2Array::getXMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _min->x()) _min = _i;
    else if (_i->x() > _max->x()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point2Array::const_iterator Point2Array::getYMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() > _result->y()) _result = _i;
  return _result;
}


Point2Array::const_iterator Point2Array::getYMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _result->y()) _result = _i;
  return _result;
}


pair<Point2Array::const_iterator,Point2Array::const_iterator>
Point2Array::getYMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _min->y()) _min = _i;
    else if (_i->y() > _max->y()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


bool Point2Array::isValid( ) const {
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (! _i->isValid()) return false;
  return true;
}


void Point2Array::normalize(){
    for (iterator _i = __A.begin(); _i != __A.end(); _i++)
        _i->normalize();
}

real_t * Point2Array::data() const {
  if(__A.empty())return NULL;
  real_t * res = new real_t[__A.size()*2];
  size_t _j = 0;
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++){
	res[_j] = _i->x(); _j++;
	res[_j] = _i->y(); _j++;
  }
  return res;
}

void Point2Array::transform(const Matrix2& m) {
  for (iterator _i = __A.begin(); _i != __A.end(); _i++){
      *_i = m* (*_i);
  }
}

/* ----------------------------------------------------------------------- */


Point3Array::Point3Array( uint_t size ) :
  Array1<Vector3>(size) {
  __A.reserve(size);
  GEOM_ASSERT(isValid());
}

Point3Array::Point3Array( uint_t size, const Vector3& t):
    Array1<Vector3>(size,t){
}

Point3Array::Point3Array( size_t size, const Vector3& firstval, const Vector3& inc):
Array1<Vector3>(range<Array1<Vector3> >(size,firstval,inc)){
}


Point3Array::Point3Array( const Vector3& a, const Vector3& b ) :
    Array1<Vector3>(2){
    __A[0] = a;
    __A[1] = b;
}

Point3Array::Point3Array( const Point2Array& a, real_t z ) :
   Array1<Vector3>(a.size()){
   GEOM_ASSERT(a.isValid());
   Point3Array::iterator it3 = begin();
   for(Point2Array::const_iterator it2 = a.begin();it2 != a.end();++it2,++it3)
        *it3 = Vector3(*it2,z);
   GEOM_ASSERT(isValid());
}

Point3Array::Point3Array( const Point2ArrayPtr& a, real_t z ) :
   Array1<Vector3>(){
   if(!a) return;
   GEOM_ASSERT(a->isValid());
   size_t size = a->size();
   __A.reserve(size);
   __A.resize(size);
   Point3Array::iterator it3 = begin();
   for(Point2Array::const_iterator it2 = a->begin();it2 != a->end();++it2,++it3)
        *it3 = Vector3(*it2,z);
   GEOM_ASSERT(isValid());
}


Point3Array::~Point3Array( ) {
}

pair<Vector3,Vector3> Point3Array::getBounds( ) const {
  Vector3 _ll, _ur;

  if (__A.size()) {
    _ll = _ur = *__A.begin();

    for (const_iterator _i = __A.begin() + 1; _i < __A.end(); _i++) {
      if (_i->x() < _ll.x()) _ll.x() = _i->x();
      else if (_i->x() > _ur.x()) _ur.x() = _i->x();

      if (_i->y() < _ll.y()) _ll.y() = _i->y();
      else if (_i->y() > _ur.y()) _ur.y() = _i->y();

      if (_i->z() < _ll.z()) _ll.z() = _i->z();
      else if (_i->z() > _ur.z()) _ur.z() = _i->z();
    };
  };

  return pair<Vector3,Vector3>(_ll,_ur);
}


Vector3 Point3Array::getCenter( ) const {
  Vector3 _center;
  if (__A.size()) {
    _center = *__A.begin();
    for (const_iterator _i = __A.begin() + 1; _i < __A.end(); _i++)
      _center += *_i;
    _center /= (real_t)__A.size();
  };
  return _center;
}


Vector3 Point3Array::getExtent( ) const {
  Vector3 _extent;
  pair<Vector3,Vector3> _bounds = getBounds();
  _extent = _bounds.second - _bounds.first;
  return _extent;
}


real_t Point3Array::getLength( ) const {
  real_t _length = 0.0;

  const_iterator _pi = __A.begin();
  const_iterator _pj = __A.begin();
  while (_pj != __A.end()) {
    _pj++;
    _length += norm(*_pi - *_pj);
    _pi = _pj;
  };

  return _length;
}


Point3Array::const_iterator Point3Array::getXMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() > _result->x()) _result = _i;
  return _result;
}


Point3Array::const_iterator Point3Array::getXMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _result->x()) _result = _i;
  return _result;
}


pair<Point3Array::const_iterator,Point3Array::const_iterator>
Point3Array::getXMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _min->x()) _min = _i;
    else if (_i->x() > _max->x()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point3Array::const_iterator Point3Array::getYMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() > _result->y()) _result = _i;
  return _result;
}


Point3Array::const_iterator Point3Array::getYMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _result->y()) _result = _i;
  return _result;
}


pair<Point3Array::const_iterator,Point3Array::const_iterator>
Point3Array::getYMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _min->y()) _min = _i;
    else if (_i->y() > _max->y()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point3Array::const_iterator Point3Array::getZMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->z() > _result->z()) _result = _i;
  return _result;
}


Point3Array::const_iterator Point3Array::getZMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _result->x()) _result = _i;
  return _result;
}


pair<Point3Array::const_iterator,Point3Array::const_iterator>
Point3Array::getZMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->z() < _min->z()) _min = _i;
    else if (_i->z() > _max->z()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


bool Point3Array::isValid( ) const {
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (! _i->isValid()) return false;
  return true;
}


Point2ArrayPtr Point3Array::project( ) const {
  Point2ArrayPtr _points2(new Point2Array(__A.size()));
  Point2Array::iterator _i2 = _points2->begin();
  for (const_iterator _i3 = __A.begin(); _i3 != __A.end(); _i3++)
    *_i2++ = Vector2(_i3->project());
  return _points2;
}

void Point3Array::normalize(){
    for (iterator _i = __A.begin(); _i != __A.end(); _i++)
        _i->normalize();
}

real_t * Point3Array::data() const{
  if(__A.empty())return NULL;
  real_t * res = new real_t[__A.size()*3];
  size_t _j = 0;
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++){
	res[_j] = _i->x(); _j++;
	res[_j] = _i->y(); _j++;
	res[_j] = _i->z(); _j++;
  }
  return res;
}
void Point3Array::transform(const Matrix3& m) {
  for (iterator _i = __A.begin(); _i != __A.end(); _i++){
      *_i = m* (*_i);
  }
}

void Point3Array::transform(const Matrix4& m) {
  for (iterator _i = __A.begin(); _i != __A.end(); _i++){
      *_i = m* (*_i);
  }
}

/* ----------------------------------------------------------------------- */


Point4Array::Point4Array( uint_t size ) :
  Array1<Vector4>(size) {
  __A.reserve(size);
  GEOM_ASSERT(isValid());
}

Point4Array::Point4Array( const Point2Array& a, real_t z, real_t w  ) :
    Array1<Vector4>(a.size()){
        Point4Array::iterator it4 = begin();
        for(Point2Array::const_iterator it2 = a.begin();it2 != a.end();++it2,++it4)
                *it4 = Vector4(*it2,z,w);
  GEOM_ASSERT(isValid());
}

Point4Array::Point4Array( const Point2ArrayPtr& a, real_t z, real_t w  ) :
    Array1<Vector4>(){
	if (!a) return;
	__A.reserve(size());
	__A.resize(size());
    Point4Array::iterator it4 = begin();
    for(Point2Array::const_iterator it2 = a->begin();it2 != a->end();++it2,++it4)
        *it4 = Vector4(*it2,z,w);
    GEOM_ASSERT(isValid());
}

Point4Array::Point4Array( const Point3Array& a, real_t w  ) :
    Array1<Vector4>(a.size()){
        Point4Array::iterator it4 = begin();
        for(Point3Array::const_iterator it3 = a.begin();it3 != a.end();++it3,++it4)
                *it4 = Vector4(*it3,w);
  GEOM_ASSERT(isValid());
}

Point4Array::Point4Array( const Point3ArrayPtr& a, real_t w  ) :
    Array1<Vector4>(){
	if (!a) return;
	__A.reserve(size());
	__A.resize(size());
    Point4Array::iterator it4 = begin();
    for(Point3Array::const_iterator it2 = a->begin();it2 != a->end();++it2,++it4)
        *it4 = Vector4(*it2,w);
    GEOM_ASSERT(isValid());
}

Point4Array::Point4Array( size_t size, const Vector4& firstval, const Vector4& inc):
Array1<Vector4>(range<Array1<Vector4> >(size,firstval,inc)){
}


Point4Array::~Point4Array( ) {
}

pair<Vector4,Vector4> Point4Array::getBounds( ) const {
  Vector4 _ll, _ur;

  if (__A.size()) {
    _ll = _ur = *__A.begin();

    for (const_iterator _i = __A.begin() + 1; _i < __A.end(); _i++) {
      if (_i->x() < _ll.x()) _ll.x() = _i->x();
      else if (_i->x() > _ur.x()) _ur.x() = _i->x();

      if (_i->y() < _ll.y()) _ll.y() = _i->y();
      else if (_i->y() > _ur.y()) _ur.y() = _i->y();

      if (_i->z() < _ll.z()) _ll.z() = _i->z();
      else if (_i->z() > _ur.z()) _ur.z() = _i->z();

      if (_i->w() < _ll.w()) _ll.w() = _i->w();
      else if (_i->w() > _ur.w()) _ur.w() = _i->w();
    };
  };

  return pair<Vector4,Vector4>(_ll,_ur);
}


Vector4 Point4Array::getCenter( ) const {
  Vector4 _center;
  if (__A.size()) {
    _center = *__A.begin();
    for (const_iterator _i = __A.begin() + 1; _i < __A.end(); _i++)
      _center += *_i;
    _center /= (real_t)__A.size();
  };
  return _center;
}


Vector4 Point4Array::getExtent( ) const {
  Vector4 _extent;
  pair<Vector4,Vector4> _bounds = getBounds();
  _extent = _bounds.second - _bounds.first;
  return _extent;
}


real_t Point4Array::getLength( ) const {
  real_t _length = 0.0;

  const_iterator _pi = __A.begin();
  const_iterator _pj = __A.begin();
  while (_pj != __A.end()) {
    _pj++;
    _length += norm(*_pi - *_pj);
    _pi = _pj;
  };

  return _length;
}


Point4Array::const_iterator Point4Array::getXMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() > _result->x()) _result = _i;
  return _result;
}


Point4Array::const_iterator Point4Array::getXMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _result->x()) _result = _i;
  return _result;
}


pair<Point4Array::const_iterator,Point4Array::const_iterator>
Point4Array::getXMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _min->x()) _min = _i;
    else if (_i->x() > _max->x()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point4Array::const_iterator Point4Array::getYMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() > _result->y()) _result = _i;
  return _result;
}


Point4Array::const_iterator Point4Array::getYMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _result->y()) _result = _i;
  return _result;
}


pair<Point4Array::const_iterator,Point4Array::const_iterator>
Point4Array::getYMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _min->y()) _min = _i;
    else if (_i->y() > _max->y()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point4Array::const_iterator Point4Array::getZMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->z() > _result->z()) _result = _i;
  return _result;
}


Point4Array::const_iterator Point4Array::getZMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _result->x()) _result = _i;
  return _result;
}


pair<Point4Array::const_iterator,Point4Array::const_iterator>
Point4Array::getZMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->z() < _min->z()) _min = _i;
    else if (_i->z() > _max->z()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point4Array::const_iterator Point4Array::getWMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->w() > _result->w()) _result = _i;
  return _result;
}


Point4Array::const_iterator Point4Array::getWMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->w() < _result->w()) _result = _i;
  return _result;
}


pair<Point4Array::const_iterator,Point4Array::const_iterator>
Point4Array::getWMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->w() < _min->w()) _min = _i;
    else if (_i->w() > _max->w()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


bool Point4Array::isValid( ) const {
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++)
    if (! _i->isValid()) return false;
  return true;
}


Point3ArrayPtr Point4Array::project( ) const {
  Point3ArrayPtr _points3(new Point3Array(__A.size()));
  Point3Array::iterator _i3 = _points3->begin();
  for (const_iterator _i4 = __A.begin(); _i4 != __A.end(); _i4++)
    *_i3++ = Vector3(_i4->project());
  return _points3;
}


void Point4Array::normalize(){
    for (iterator _i = __A.begin() ; _i != __A.end(); _i++)
        _i->normalize();
}


real_t * Point4Array::data() const {
  if(__A.empty())return NULL;
  real_t * res = new real_t[__A.size()*4];
  size_t _j = 0;
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++){
	res[_j] = _i->x(); _j++;
	res[_j] = _i->y(); _j++;
	res[_j] = _i->z(); _j++;
	res[_j] = _i->w(); _j++;
  }
  return res;
}

void Point4Array::transform(const Matrix4& m) {
  for (iterator _i = __A.begin(); _i != __A.end(); _i++){
      *_i = m* (*_i);
  }
}


/* ----------------------------------------------------------------------- */
