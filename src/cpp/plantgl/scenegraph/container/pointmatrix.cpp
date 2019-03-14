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


#include "pointmatrix.h"
PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


Point2Matrix::Point2Matrix( uint_t rows , uint_t cols ) :
  Array2<Vector2>(rows,cols) {
  // __A.reserve(rows*cols);
  GEOM_ASSERT(isValid());
}

Point2Matrix::Point2Matrix( uint_t rows , uint_t cols, const Vector2& value ) :
  Array2<Vector2>(rows,cols,value) {
  GEOM_ASSERT(isValid());
}

Point2Matrix::~Point2Matrix( ) {
}

pair<Vector2,Vector2> Point2Matrix::getBounds( ) const {
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


Vector2 Point2Matrix::getCenter( ) const {
  Vector2 _center;
  if (__A.size()) {
    _center = *__A.begin();
    for (const_iterator _i = __A.begin() + 1; _i < __A.end(); _i++)
      _center += *_i;
    _center /= (real_t)__A.size();
  };
  return _center;
}


Vector2 Point2Matrix::getExtent( ) const {
  Vector2 _extent;
  pair<Vector2,Vector2> _bounds = getBounds();
    _extent = _bounds.second - _bounds.first;
  return _extent;
}


Point2Matrix::const_iterator Point2Matrix::getXMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() > _result->x()) _result = _i;
  return _result;
}


Point2Matrix::const_iterator Point2Matrix::getXMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _result->x()) _result = _i;
  return _result;
}


pair<Point2Matrix::const_iterator,Point2Matrix::const_iterator>
Point2Matrix::getXMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _min->x()) _min = _i;
    else if (_i->x() > _max->x()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point2Matrix::const_iterator Point2Matrix::getYMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() > _result->y()) _result = _i;
  return _result;
}


Point2Matrix::const_iterator Point2Matrix::getYMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _result->y()) _result = _i;
  return _result;
}


pair<Point2Matrix::const_iterator,Point2Matrix::const_iterator>
Point2Matrix::getYMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _min->y()) _min = _i;
    else if (_i->y() > _max->y()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


bool Point2Matrix::isValid( ) const {
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (! _i->isValid()) return false;
  return true;
}



void Point2Matrix::normalize(){
    for (iterator _i = __A.begin(); _i != __A.end(); _i++)
        _i->normalize();
}

/* ----------------------------------------------------------------------- */


Point3Matrix::Point3Matrix( uint_t rows, uint_t cols ) :
  Array2<Vector3>(rows,cols) {
//  __A.reserve(rows*cols);
  GEOM_ASSERT(isValid());
}

Point3Matrix::Point3Matrix( uint_t rows, uint_t cols, const Vector3& value ) :
  Array2<Vector3>(rows,cols, value) {
//  __A.reserve(rows*cols);
  GEOM_ASSERT(isValid());
}

  
Point3Matrix::Point3Matrix( const Point2MatrixPtr& points2, const real_t& z ) :
  Array2<Vector3>() {
  if(!points2) return;
  resize(points2->getRowNb(),points2->getColumnNb());
  Point2Matrix::const_iterator _i2 = points2->begin();
  for (iterator _i3 = __A.begin(); _i3 != __A.end(); _i3++)
    *_i3 = Vector3(*_i2++,z);
  GEOM_ASSERT(isValid());
}

Point3Matrix::Point3Matrix( const Point2Matrix& points2, const real_t& z ) :
  Array2<Vector3>(points2.getRowNb(),points2.getColumnNb()) {
  GEOM_ASSERT(points2);
  Point2Matrix::const_iterator _i2 = points2.begin();
  for (iterator _i3 = __A.begin(); _i3 != __A.end(); _i3++)
    *_i3 = Vector3(*_i2++,z);
  GEOM_ASSERT(isValid());
}

Point3Matrix::~Point3Matrix( ) {
}

pair<Vector3,Vector3> Point3Matrix::getBounds( ) const {
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


Vector3 Point3Matrix::getCenter( ) const {
  Vector3 _center;
  if (__A.size()) {
    _center = *__A.begin();
    for (const_iterator _i = __A.begin() + 1; _i < __A.end(); _i++)
      _center += *_i;
    _center /= (real_t)__A.size();
  };
  return _center;
}


Vector3 Point3Matrix::getExtent( ) const {
  Vector3 _extent;
  pair<Vector3,Vector3> _bounds = getBounds();
  _extent = _bounds.second - _bounds.first;
  return _extent;
}


Point3Matrix::const_iterator Point3Matrix::getXMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() > _result->x()) _result = _i;
  return _result;
}


Point3Matrix::const_iterator Point3Matrix::getXMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _result->x()) _result = _i;
  return _result;
}


pair<Point3Matrix::const_iterator,Point3Matrix::const_iterator>
Point3Matrix::getXMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _min->x()) _min = _i;
    else if (_i->x() > _max->x()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point3Matrix::const_iterator Point3Matrix::getYMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() > _result->y()) _result = _i;
  return _result;
}


Point3Matrix::const_iterator Point3Matrix::getYMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _result->y()) _result = _i;
  return _result;
}


pair<Point3Matrix::const_iterator,Point3Matrix::const_iterator>
Point3Matrix::getYMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _min->y()) _min = _i;
    else if (_i->y() > _max->y()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point3Matrix::const_iterator Point3Matrix::getZMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->z() > _result->z()) _result = _i;
  return _result;
}


Point3Matrix::const_iterator Point3Matrix::getZMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _result->x()) _result = _i;
  return _result;
}


pair<Point3Matrix::const_iterator,Point3Matrix::const_iterator>
Point3Matrix::getZMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->z() < _min->z()) _min = _i;
    else if (_i->z() > _max->z()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


bool Point3Matrix::isValid( ) const {
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (! _i->isValid()) return false;
  return true;
}


Point2MatrixPtr Point3Matrix::project( ) const {
  Point2MatrixPtr _points2(new Point2Matrix(getRowNb(),getColumnNb()));
  Point2Matrix::iterator _i2 = _points2->begin();
  for (const_iterator _i3 = __A.begin(); _i3 != __A.end(); _i3++)
    *_i2++ = Vector2(_i3->project());
  return _points2;
}


void Point3Matrix::normalize(){
    for (iterator _i = __A.begin(); _i != __A.end(); _i++)
        _i->normalize();
}

/* ----------------------------------------------------------------------- */


Point4Matrix::Point4Matrix( uint_t rows,uint_t cols ) :
  Array2<Vector4>(rows,cols) {
  // __A.reserve(rows*cols);
  GEOM_ASSERT(isValid());
}


Point4Matrix::Point4Matrix( uint_t rows, uint_t cols, const Vector4& value ) :
  Array2<Vector4>(rows,cols, value) {
  // __A.reserve(rows*cols);
  GEOM_ASSERT(isValid());
}


Point4Matrix::Point4Matrix( const Point3MatrixPtr& points3, const real_t& z ) :
  Array2<Vector4>() {
  if(!points3) return;
  GEOM_ASSERT(points3);
  resize(points3->getRowNb(),points3->getColumnNb());
  Point3Matrix::const_iterator _i3 = points3->begin();
  for (iterator _i4 = __A.begin(); _i4 != __A.end(); _i4++)
    *_i4 = Vector4(*_i3++,z);
  GEOM_ASSERT(isValid());
}

Point4Matrix::Point4Matrix( const Point3Matrix& points3, const real_t& z ) :
  Array2<Vector4>(points3.getRowNb(),points3.getColumnNb()) {
  GEOM_ASSERT(points3);
  Point3Matrix::const_iterator _i3 = points3.begin();
  for (iterator _i4 = __A.begin(); _i4 != __A.end(); _i4++)
    *_i4 = Vector4(*_i3++,z);
  GEOM_ASSERT(isValid());
}

Point4Matrix::~Point4Matrix( ) {
}

pair<Vector4,Vector4> Point4Matrix::getBounds( ) const {
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


Vector4 Point4Matrix::getCenter( ) const {
  Vector4 _center;
  if (__A.size()) {
    _center = *__A.begin();
    for (const_iterator _i = __A.begin() + 1; _i < __A.end(); _i++)
      _center += *_i;
    _center /= (real_t)__A.size();
  };
  return _center;
}


Vector4 Point4Matrix::getExtent( ) const {
  Vector4 _extent;
  pair<Vector4,Vector4> _bounds = getBounds();
  _extent = _bounds.second - _bounds.first;
  return _extent;
}


Point4Matrix::const_iterator Point4Matrix::getXMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() > _result->x()) _result = _i;
  return _result;
}


Point4Matrix::const_iterator Point4Matrix::getXMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _result->x()) _result = _i;
  return _result;
}


pair<Point4Matrix::const_iterator,Point4Matrix::const_iterator>
Point4Matrix::getXMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _min->x()) _min = _i;
    else if (_i->x() > _max->x()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point4Matrix::const_iterator Point4Matrix::getYMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() > _result->y()) _result = _i;
  return _result;
}


Point4Matrix::const_iterator Point4Matrix::getYMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _result->y()) _result = _i;
  return _result;
}


pair<Point4Matrix::const_iterator,Point4Matrix::const_iterator>
Point4Matrix::getYMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->y() < _min->y()) _min = _i;
    else if (_i->y() > _max->y()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point4Matrix::const_iterator Point4Matrix::getZMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->z() > _result->z()) _result = _i;
  return _result;
}


Point4Matrix::const_iterator Point4Matrix::getZMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->x() < _result->x()) _result = _i;
  return _result;
}


pair<Point4Matrix::const_iterator,Point4Matrix::const_iterator>
Point4Matrix::getZMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->z() < _min->z()) _min = _i;
    else if (_i->z() > _max->z()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


Point4Matrix::const_iterator Point4Matrix::getWMax( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->w() > _result->w()) _result = _i;
  return _result;
}


Point4Matrix::const_iterator Point4Matrix::getWMin( ) const {
  const_iterator _result = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->w() < _result->w()) _result = _i;
  return _result;
}


pair<Point4Matrix::const_iterator,Point4Matrix::const_iterator>
Point4Matrix::getWMinAndMax( ) const {
  const_iterator _min = __A.begin();
  const_iterator _max = __A.begin();
  for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
    if (_i->w() < _min->w()) _min = _i;
    else if (_i->w() > _max->w()) _max = _i;
  return pair<const_iterator,const_iterator>(_min,_max);
}


bool Point4Matrix::isValid( ) const {
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++)
    if (! _i->isValid()) return false;
  return true;
}


Point3MatrixPtr Point4Matrix::project( ) const {
  Point3MatrixPtr _points3(new Point3Matrix(getRowNb(),getColumnNb()));
  Point3Matrix::iterator _i3 = _points3->begin();
  for (const_iterator _i4 = __A.begin(); _i4 != __A.end(); _i4++)
    *_i3++ = Vector3(_i4->project());
  return _points3;
}


void Point4Matrix::normalize(){
    for (iterator _i = __A.begin(); _i != __A.end(); _i++)
        _i->normalize();
}

/* ----------------------------------------------------------------------- */
