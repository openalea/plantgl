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




#include "util_matrix.h"
#include "util_math.h"
#include "plantgl/tool/util_assert.h"
#include <iostream>
using namespace std;

TOOLS_BEGIN_NAMESPACE

/*  --------------------------------------------------------------------- */

inline real_t GEOM_DET2(real_t m0, real_t m1,real_t m2,real_t m3) 
{ return m0 * m3 - m1 * m2; }

/*#define GEOM_DET3(m0,m1,m2,m3,m4,m5,m6,m7,m8) \
  m0 * GEOM_DET2(m4,m5,m7,m8) - \
  m3 * GEOM_DET2(m1,m2,m7,m8) + \
  m6 * GEOM_DET2(m1,m2,m4,m5)*/

inline real_t GEOM_DET3(real_t m0, real_t m1,real_t m2,real_t m3,real_t m4,real_t m5,real_t m6,real_t m7,real_t m8){
return m0 * GEOM_DET2(m4,m5,m7,m8) - m1 * GEOM_DET2(m3,m5,m6,m8) + m2 * GEOM_DET2(m3,m4,m6,m7); }


/*  --------------------------------------------------------------------- */

const Matrix2 Matrix2::IDENTITY;


/*  --------------------------------------------------------------------- */

Matrix2::Matrix2( const real_t& m0 , const real_t& m1 ,
                  const real_t& m2 , const real_t& m3 ) {
  set(m0,m1,m2,m3);
}

Matrix2::Matrix2( const real_t * matrix ) {
  set(matrix[0],matrix[1],matrix[2],matrix[3]);
}

Matrix2::Matrix2( const Vector2& v0, const Vector2& v1 ) {
  set(v0.x(),v1.x(),
      v0.y(),v1.y());
}

Matrix2::~Matrix2(){
}

const real_t& Matrix2::operator()( uchar_t i, uchar_t j ) const {
  GEOM_ASSERT((i < 2) && (j < 2));
  return (__M[i * 2 + j]);
}

real_t& Matrix2::operator()( uchar_t i, uchar_t j ) {
  GEOM_ASSERT((i < 2) && (j < 2));
  return (__M[i * 2 + j]);
}

const real_t * Matrix2::getData( ) const {
  return __M;
}

real_t * Matrix2::getData( ) {
  return __M;
}

void Matrix2::set( const real_t& m0, const real_t& m1,
                   const real_t& m2, const real_t& m3 ) {
  GEOM_ASSERT(std::finite(m0) && std::finite(m1) &&
              std::finite(m2) && std::finite(m3));
  __M[0] = m0; __M[1] = m1; __M[2] = m2; __M[3] = m3;
}

/*  --------------------------------------------------------------------- */

bool Matrix2::operator==( const Matrix2& m ) const {
  return
    (fabs(__M[0] - m.__M[0]) < GEOM_TOLERANCE) &&
    (fabs(__M[1] - m.__M[1]) < GEOM_TOLERANCE) &&
    (fabs(__M[2] - m.__M[2]) < GEOM_TOLERANCE) &&
    (fabs(__M[3] - m.__M[3]) < GEOM_TOLERANCE);
}

bool Matrix2::operator!=( const Matrix2& m ) const {
	return !(operator==(m));
}

Matrix2& Matrix2::operator+=( const Matrix2& m ) {
  __M[0] += m.__M[0]; __M[1] += m.__M[1];
  __M[2] += m.__M[2]; __M[3] += m.__M[3];
  return *this;
}

Matrix2& Matrix2::operator-=( const Matrix2& m ) {
  __M[0] -= m.__M[0]; __M[1] -= m.__M[1];
  __M[2] -= m.__M[2]; __M[3] -= m.__M[3];
  return *this;
}

Matrix2& Matrix2::operator*=( const Matrix2& m ) {	
	*this = operator*(m);
    return *this ;
}
Matrix2& Matrix2::operator*=( const real_t& s ) {
  __M[0] *= s; __M[1] *= s;
  __M[2] *= s; __M[3] *= s;
  return *this;
}

Matrix2& Matrix2::operator/=( const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  __M[0] /= s; __M[1] /= s;
  __M[2] /= s; __M[3] /= s;
  return *this;
}

Matrix2 Matrix2::operator-( ) const {
  return Matrix2(-__M[0],-__M[1],
                 -__M[2],-__M[3]);
}

Matrix2 Matrix2::operator+( const Matrix2& m) const {
  return Matrix2(*this) += m;
}

Matrix2 Matrix2::operator-( const Matrix2& m ) const {
  return Matrix2(*this) -= m;
}

Matrix2 Matrix2::operator*( const Matrix2& m ) const {
  return Matrix2(__M[0] * m.__M[0] + __M[1] * m.__M[2],
                 __M[0] * m.__M[1] + __M[1] * m.__M[3],
                 __M[2] * m.__M[0] + __M[3] * m.__M[2],
                 __M[2] * m.__M[1] + __M[3] * m.__M[3]);
}
bool Matrix2::isOrthogonal( ) const {
  return getColumn(0).isOrthogonalTo(getColumn(1));
}

bool Matrix2::isValid( ) const {
  return
          finite(__M[0]) && finite(__M[1])  &&
          finite(__M[2]) && finite(__M[3]);
}

Vector2 Matrix2::getColumn( uchar_t i ) const {
  GEOM_ASSERT(i < 2);
  return Vector2(__M[i],__M[i+2]);
}

Vector2 Matrix2::getDiagonal( ) const {
  return Vector2(__M[0],__M[3]);
}

Vector2 Matrix2::getRow( uchar_t i ) const {
  GEOM_ASSERT(i < 2);
  uchar_t _offset = i * 2;
  return Vector2(__M[_offset],__M[1+_offset]);
}

bool Matrix2::isSingular( ) const {
  return fabs(det(*this)) < GEOM_TOLERANCE;
}

/*  --------------------------------------------------------------------- */


Vector2 operator*( const Matrix2& m, const Vector2& v ) {
  return Vector2(m.__M[0] * v.x() + m.__M[1] * v.y(),
                 m.__M[2] * v.x() + m.__M[3] * v.y());
}

Matrix2 operator*( const Matrix2& m, const real_t& s ) {
  return Matrix2(m) *= s;
}

Matrix2 operator/( const Matrix2& m, const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  return Matrix2(m) /= s;
}

Matrix2 adjoint( const Matrix2& m ) {
  return Matrix2(m.__M[3],-m.__M[1], -m.__M[2],m.__M[0]);
}

real_t det( const Matrix2& m ) {
  return GEOM_DET2(m.__M[0],m.__M[1],m.__M[2],m.__M[3]);
}

Matrix2 inverse( const Matrix2& m ) {
  Matrix2 _adjoint = adjoint(m);
  real_t _det = det(m);
  return _adjoint / _det;
}

real_t trace( const Matrix2& m ) {
  return m.__M[0] + m.__M[3];
}

Matrix2 transpose( const Matrix2& m ) {
  return Matrix2(m.__M[0],m.__M[2],
                 m.__M[1],m.__M[3]);
}

/*  --------------------------------------------------------------------- */

std::ostream& operator<<( std::ostream& stream, const Matrix2& m ) {
  stream << "[[" << m.__M[0] << "," << m.__M[1] <<  "]," << endl;
  return stream << "[" << m.__M[2] << "," << m.__M[3]  << "]]" << endl;
}

/*  --------------------------------------------------------------------- */
Matrix2 Matrix2::rotation(const real_t angle) { 
    real_t c = cos(angle);
    real_t s = sin(angle);
    return Matrix2(c,-s,s,c);
}


Matrix2 Matrix2::linearTransformation(const Vector2& i1,
                                      const Vector2& j1,
                                      const Vector2& i2,
                                      const Vector2& j2)
{
	real_t denom = cross(i1,j1); // non colinear vectors
	real_t alpha=(i2.x()*j1.y()-j2.x()*i1.y())/denom;
	real_t beta =(j2.x()*i1.x()-i2.x()*j1.x())/denom;
	real_t gamma=(i2.y()*j1.y()-j2.y()*i1.y())/denom;
	real_t delta=(j2.y()*i1.x()-i2.y()*j1.x())/denom;
	return Matrix2(alpha,beta,gamma,delta);
}

/*  --------------------------------------------------------------------- */

const Matrix3 Matrix3::IDENTITY;

/*  --------------------------------------------------------------------- */

Matrix3::Matrix3( const real_t& m0 , const real_t& m1 ,
                  const real_t& m2 , const real_t& m3 ,
                  const real_t& m4 , const real_t& m5 ,
                  const real_t& m6 , const real_t& m7 ,
                  const real_t& m8 ) {
  set(m0,m1,m2,m3,m4,m5,m6,m7,m8);
}

Matrix3::Matrix3( const real_t * matrix ) {
  set(matrix[0],matrix[1],matrix[2],
      matrix[3],matrix[4],matrix[5],
      matrix[6],matrix[7],matrix[8]);
}

Matrix3::Matrix3( const Vector3& v0,
                  const Vector3& v1,
                  const Vector3& v2 ) {
  set(v0.x(),v1.x(),v2.x(),
      v0.y(),v1.y(),v2.y(),
      v0.z(),v1.z(),v2.z());
}

Matrix3::Matrix3( const Matrix2& m ) {
  GEOM_ASSERT(m.isValid());
  __M[0] = m.__M[0]; __M[1] = m.__M[1]; __M[2] = 0;
  __M[3] = m.__M[2]; __M[4] = m.__M[3]; __M[5]  = 0;
  __M[6] = 0; __M[7] = 0; __M[8] = 1;
  GEOM_ASSERT(isValid());
}

Matrix3::Matrix3( const Matrix4& m ) {
  GEOM_ASSERT(m.isValid());
  __M[0] = m.__M[0]; __M[1] = m.__M[1]; __M[2] = m.__M[2];
  __M[3] = m.__M[4]; __M[4] = m.__M[5]; __M[5]  = m.__M[6];
  __M[6] = m.__M[8]; __M[7] = m.__M[9]; __M[8] = m.__M[10];
  GEOM_ASSERT(isValid());
}

Matrix3::~Matrix3(){
}

const real_t&
Matrix3::operator()( uchar_t i, uchar_t j ) const {
  GEOM_ASSERT((i < 3) && (j < 3));
  return (__M[i * 3 + j]);
}

real_t&
Matrix3::operator()( uchar_t i, uchar_t j ) {
  GEOM_ASSERT((i < 3) && (j < 3));
  return (__M[i * 3 + j]);
}

const real_t * Matrix3::getData( ) const {
  return __M;
}

real_t * Matrix3::getData( ) {
  return __M;
}

void Matrix3::set( const real_t& m0, const real_t& m1, const real_t& m2,
                   const real_t& m3, const real_t& m4, const real_t& m5,
                   const real_t& m6, const real_t& m7, const real_t& m8 ) {
  GEOM_ASSERT(std::finite(m0) && std::finite(m1) && std::finite(m2) &&
              std::finite(m3) && std::finite(m4) && std::finite(m5) &&
              std::finite(m6) && std::finite(m7) && std::finite(m8));
  __M[0] = m0; __M[1] = m1; __M[2] = m2;
  __M[3] = m3; __M[4] = m4; __M[5] = m5;
  __M[6] = m6; __M[7] = m7; __M[8] = m8;
}

/*  --------------------------------------------------------------------- */

bool Matrix3::operator==( const Matrix3& m ) const {
  return
    (fabs(__M[0] - m.__M[0]) < GEOM_TOLERANCE) &&
    (fabs(__M[1] - m.__M[1]) < GEOM_TOLERANCE) &&
    (fabs(__M[2] - m.__M[2]) < GEOM_TOLERANCE) &&
    (fabs(__M[3] - m.__M[3]) < GEOM_TOLERANCE) &&
    (fabs(__M[4] - m.__M[4]) < GEOM_TOLERANCE) &&
    (fabs(__M[5] - m.__M[5]) < GEOM_TOLERANCE) &&
    (fabs(__M[6] - m.__M[6]) < GEOM_TOLERANCE) &&
    (fabs(__M[7] - m.__M[7]) < GEOM_TOLERANCE) &&
    (fabs(__M[8] - m.__M[8]) < GEOM_TOLERANCE);
}

bool Matrix3::operator!=( const Matrix3& m ) const {
	return !(operator==(m));
}

Matrix3& Matrix3::operator+=( const Matrix3& m ) {
  __M[0] += m.__M[0]; __M[1] += m.__M[1]; __M[2] += m.__M[2];
  __M[3] += m.__M[3]; __M[4] += m.__M[4]; __M[5] += m.__M[5];
  __M[6] += m.__M[6]; __M[7] += m.__M[7]; __M[8] += m.__M[8];
  return *this;
}

Matrix3& Matrix3::operator-=( const Matrix3& m ) {
  __M[0] -= m.__M[0]; __M[1] -= m.__M[1]; __M[2] -= m.__M[2];
  __M[3] -= m.__M[3]; __M[4] -= m.__M[4]; __M[5] -= m.__M[5];
  __M[6] -= m.__M[6]; __M[7] -= m.__M[7]; __M[8] -= m.__M[8];
    return *this;
}

Matrix3& Matrix3::operator*=( const Matrix3& m ) {	
	*this = operator*(m);
    return *this ;
}

Matrix3& Matrix3::operator*=( const real_t& s ) {
  __M[0] *= s; __M[1] *= s; __M[2] *= s;
  __M[3] *= s; __M[4] *= s; __M[5] *= s;
  __M[6] *= s; __M[7] *= s; __M[8] *= s;
  return *this;
}

Matrix3& Matrix3::operator/=( const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  __M[0] /= s; __M[1] /= s; __M[2] /= s;
  __M[3] /= s; __M[4] /= s; __M[5] /= s;
  __M[6] /= s; __M[7] /= s; __M[8] /= s;
  return *this;
}

Matrix3 Matrix3::operator-( ) const {
  return Matrix3(-__M[0],-__M[1],-__M[2],
                 -__M[3],-__M[4],-__M[5],
                 -__M[6],-__M[7],-__M[8]);
}

Matrix3 Matrix3::operator+( const Matrix3& m) const {
  return Matrix3(*this) += m;
}

Matrix3 Matrix3::operator-( const Matrix3& m ) const {
  return Matrix3(*this) -= m;
}

Matrix3 Matrix3::operator*( const Matrix3& m ) const {
  return Matrix3(m.__M[0] * __M[0] + m.__M[3] * __M[1] +
                 m.__M[6] * __M[2],
                 m.__M[1] * __M[0] + m.__M[4] * __M[1] +
                 m.__M[7] * __M[2],
                 m.__M[2] * __M[0] + m.__M[5] * __M[1] +
                 m.__M[8] * __M[2],
                 m.__M[0] * __M[3] + m.__M[3] * __M[4] +
                 m.__M[6] * __M[5],
                 m.__M[1] * __M[3] + m.__M[4] * __M[4] +
                 m.__M[7] * __M[5],
                 m.__M[2] * __M[3] + m.__M[5] * __M[4] +
                 m.__M[8] * __M[5],
                 m.__M[0] * __M[6] + m.__M[3] * __M[7] +
                 m.__M[6] * __M[8],
                 m.__M[1] * __M[6] + m.__M[4] * __M[7] +
                 m.__M[7] * __M[8],
                 m.__M[2] * __M[6] + m.__M[5] * __M[7] +
                 m.__M[8] * __M[8]);
}

bool Matrix3::isOrthogonal( ) const {
  return
    getColumn(0).isOrthogonalTo(getColumn(1)) &&
    getColumn(0).isOrthogonalTo(getColumn(2)) &&
    getColumn(1).isOrthogonalTo(getColumn(2));
}

bool Matrix3::isSingular( ) const {
  return fabs(det()) < GEOM_TOLERANCE;
}

bool Matrix3::isValid( ) const {
  return
    finite(__M[0]) && finite(__M[1]) &&
    finite(__M[2]) && finite(__M[3]) &&
    finite(__M[4]) && finite(__M[5]) &&
    finite(__M[6]) && finite(__M[7]) &&
    finite(__M[8]);
}

Vector3 Matrix3::getColumn( uchar_t i ) const {
  GEOM_ASSERT(i < 3);
  return Vector3(__M[i],__M[3+i],__M[6+i]);
}

Vector3 Matrix3::getDiagonal( ) const {
  return Vector3(__M[0],__M[4],__M[8]);
}

Vector3 Matrix3::getRow( uchar_t i ) const {
  GEOM_ASSERT(i < 3);
  uchar_t _offset = i * 3;
  return Vector3(__M[_offset],__M[_offset+1],__M[_offset+2]);
}


void Matrix3::setColumn( uchar_t i, const Vector3& v ) {
  GEOM_ASSERT(i < 3);
  __M[i    ] = v.x();
  __M[i + 3] = v.y();
  __M[i + 6] = v.z();
}


/*  --------------------------------------------------------------------- */

Vector3 operator*( const Matrix3& m, const Vector3& v ) {
  return Vector3(m.__M[0] * v.x() + m.__M[1] * v.y() + m.__M[2] * v.z(),
                 m.__M[3] * v.x() + m.__M[4] * v.y() + m.__M[5] * v.z(),
                 m.__M[6] * v.x() + m.__M[7] * v.y() + m.__M[8] * v.z());
}

Matrix3 operator*( const Matrix3& m, const real_t& s ) {
  return Matrix3(m) *= s;
}

Matrix3 operator/( const Matrix3& m, const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  return Matrix3(m) /= s;
}

Matrix3 adjoint( const Matrix3& m ) {
  return m.adjoint();
}


Matrix3 Matrix3::adjoint() const {
  return Matrix3( GEOM_DET2(__M[4],__M[5],__M[7],__M[8]),
                 -GEOM_DET2(__M[1],__M[2],__M[7],__M[8]),
                  GEOM_DET2(__M[1],__M[2],__M[4],__M[5]),
                 -GEOM_DET2(__M[3],__M[5],__M[6],__M[8]),
                  GEOM_DET2(__M[0],__M[2],__M[6],__M[8]),
                 -GEOM_DET2(__M[0],__M[2],__M[3],__M[5]),
                  GEOM_DET2(__M[3],__M[4],__M[6],__M[7]),
                 -GEOM_DET2(__M[0],__M[1],__M[6],__M[7]),
                  GEOM_DET2(__M[0],__M[1],__M[3],__M[4]));
}



real_t det( const Matrix3& m ) {
  return m.det();
}

real_t Matrix3::det() const {
  return GEOM_DET3(__M[0],__M[1],__M[2],
                   __M[3],__M[4],__M[5],
                   __M[6],__M[7],__M[8]);
}

Matrix3 inverse( const Matrix3& m ) {
  return m.inverse();
}

Matrix3 
Matrix3::inverse() const {
  Matrix3 _adjoint = adjoint();
  real_t _det= ( __M[0] * _adjoint.__M[0] +
                 __M[1] * _adjoint.__M[3] +
                 __M[2] * _adjoint.__M[6] );
  return _adjoint / _det;
}

Matrix3 transpose( const Matrix3& m ) {
  return m.transpose();
}

Matrix3 
Matrix3::transpose() const {
  return Matrix3(__M[0],__M[3],__M[6],
                 __M[1],__M[4],__M[7],
                 __M[2],__M[5],__M[8]);
}

ostream& operator<<( ostream& stream, const Matrix3& m ) {
  stream << "[[" << m.__M[0] << "," << m.__M[1] << "," << m.__M[2] << "]," << endl;
  stream << "[" << m.__M[3] << "," << m.__M[4] << "," << m.__M[5] << "],"<< endl;
  return stream << "[" << m.__M[6] << "," << m.__M[7] << "," << m.__M[8] << "]]";
}

real_t trace( const Matrix3& m ) {
  return m.__M[0] + m.__M[4] + m.__M[8];
}

/*  --------------------------------------------------------------------- */

Matrix3 Matrix3::scaling( const Vector3& s )
{
  GEOM_ASSERT(s.isValid());
  return Matrix3( s.x(), 0,     0,
                  0,     s.y(), 0,
                  0,     0,     s.z());
}

Matrix3 Matrix3::scaling( real_t s )
{
  GEOM_ASSERT(s.isValid());
  return Matrix3( s, 0, 0,
                  0, s, 0,
                  0, 0, s);
}

/*inline*/ Matrix3 Matrix3::axisRotation( const Vector3& axis, const real_t& angle )
{
    GEOM_ASSERT(std::finite(angle));
    GEOM_ASSERT(axis.isValid());
    register real_t _c = cos(angle);
    register real_t _s = sin(angle);
    register real_t _t = 1 - _c;
    register real_t x= axis.x();
    register real_t y= axis.y();
    register real_t z= axis.z();

    real_t _sx = _s * x;
    real_t _sy = _s * y;
    real_t _sz = _s * z;
    real_t _txx = _t * x * x;
    real_t _txy = _t * x * y;
    real_t _txz = _t * x * z;
    real_t _tyy = _t * y * y;
    real_t _tyz = _t * y * z;
    real_t _tzz = _t * z * z;

    return Matrix3(_txx + _c, _txy - _sz, _txz + _sy,
                   _txy + _sz, _tyy + _c, _tyz - _sx,
                   _txz - _sy, _tyz + _sx, _tzz + _c);
}

Vector3 Matrix3::eulerAnglesZYX() const {
  real_t b = asin(- __M[6]);
  real_t a, c;
  if (b < GEOM_PI/2){
    if (b > -GEOM_PI/2){
      a = atan2(__M[3],__M[0]);
      c = atan2(__M[7],__M[8]);
    }
    else {  // not a unique solution
      a = atan2(- __M[1],__M[2]);
      c = 0.0;
      }
  }
  else { // not a unique solution
    a = - atan2(- __M[1],__M[2]);
    c = 0.0;
  }
  return Vector3(a,b,c);
}

Vector3 Matrix3::eulerAnglesXYZ() const {
  real_t c = asin( __M[3]);
  real_t a, b;
  if (c < GEOM_PI/2){
    if (c > -GEOM_PI/2){
      b = atan2(-__M[6],__M[0]);
      a = atan2(-__M[5],__M[4]);
    }
    else {  // not a unique solution
      b = atan2( __M[2],__M[1]);
      a = 0.0;
      }
  }
  else { // not a unique solution
    b = atan2( __M[2],- __M[1]);
    a = 0.0;
  }
  return Vector3(a,b,c);
}

/*inline*/ 
Matrix3 Matrix3::eulerRotationZYX( const Vector3& angle ) 
{
  GEOM_ASSERT(angle.isValid());
  real_t _CZ = cos(angle.z());
  real_t _SZ = sin(angle.z());
  real_t _CY = cos(angle.y());
  real_t _SY = sin(angle.y());
  real_t _CX = cos(angle.x());
  real_t _SX = sin(angle.x());
  real_t _CXSY = _CX * _SY;
  real_t _SXSY = _SX * _SY;

  return Matrix3( _CX*_CY,  _CXSY*_SZ - _SX*_CZ,  _CXSY*_CZ + _SX*_SZ,
                  _SX*_CY,  _CX*_CZ + _SXSY*_SZ,  _SXSY*_CZ - _CX*_SZ,
                  -_SY,     _CY*_SZ,              _CY*_CZ             );
}

Matrix3 Matrix3::eulerRotationXYZ( const Vector3& angle )
{
  GEOM_ASSERT(angle.isValid());
  real_t _CX = cos(angle.x());
  real_t _SX = sin(angle.x());
  real_t _CY = cos(angle.y());
  real_t _SY = sin(angle.y());
  real_t _CZ = cos(angle.z());
  real_t _SZ = sin(angle.z());
  real_t _CXSZ = _CX * _SZ;
  real_t _SXSY = _SX * _SY;

  return Matrix3( _CY*_CZ,   -_CXSZ*_CY + _SXSY,     _SX*_CY*_SZ + _CX*_SY,
                  _SZ,       _CZ*_CX ,               - _CZ*_SX,
                  -_SY*_CZ,  _CXSZ*_SY + _SX*_CY,    -_SXSY * _SZ + _CX*_CY );
}

/*  --------------------------------------------------------------------- */

const Matrix4 Matrix4::IDENTITY;

/*  --------------------------------------------------------------------- */

Matrix4::Matrix4( const real_t& m0 , const real_t& m1 ,
                  const real_t& m2 , const real_t& m3 ,
                  const real_t& m4 , const real_t& m5 ,
                  const real_t& m6 , const real_t& m7 ,
                  const real_t& m8 , const real_t& m9 ,
                  const real_t& m10 , const real_t& m11 ,
                  const real_t& m12 , const real_t& m13 ,
                  const real_t& m14 , const real_t& m15 ) {
  set(m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15);
}

Matrix4::Matrix4( const real_t * matrix ) {
  set(matrix[0],matrix[1],matrix[2],matrix[3],
      matrix[4],matrix[5],matrix[6],matrix[7],
      matrix[8],matrix[9],matrix[10],matrix[11],
      matrix[12],matrix[13],matrix[14],matrix[15]);
}

Matrix4::Matrix4( const Vector4& v0,
                  const Vector4& v1,
                  const Vector4& v2,
                  const Vector4& v3 ) {
  set(v0.x(),v1.x(),v2.x(),v3.x(),
      v0.y(),v1.y(),v2.y(),v3.y(),
      v0.z(),v1.z(),v2.z(),v3.z(),
      v0.w(),v1.w(),v2.w(),v3.w());
}

Matrix4::Matrix4( const Matrix3& m ) {
  GEOM_ASSERT(m.isValid());
  __M[0] = m.__M[0]; __M[1] = m.__M[1]; __M[2] = m.__M[2]; __M[3] = 0;
  __M[4] = m.__M[3]; __M[5] = m.__M[4]; __M[6] = m.__M[5]; __M[7] = 0;
  __M[8] = m.__M[6]; __M[9] = m.__M[7]; __M[10] = m.__M[8]; __M[11] = 0;
  __M[12] = 0; __M[13] = 0; __M[14] = 0; __M[15] = 1;
    GEOM_ASSERT(isValid());
}

Matrix4::~Matrix4(){
}

const real_t& Matrix4::operator()( uchar_t i, uchar_t j ) const {
  GEOM_ASSERT((i < 4) && (j < 4));
  return (__M[i * 4 + j]);
}

real_t& Matrix4::operator()( uchar_t i, uchar_t j ) {
  GEOM_ASSERT((i < 4) && (j < 4));
  return (__M[i * 4 + j]);
}

const real_t * Matrix4::getData( ) const {
  GEOM_ASSERT(isValid());
  return __M;
}

real_t * Matrix4::getData( ) {
  GEOM_ASSERT(isValid());
  return __M;
}

Vector4 Matrix4::getColumn( uchar_t i ) const {
  GEOM_ASSERT(i < 4);
  return Vector4(__M[i],__M[4+i],__M[8+i],__M[12+i]);
}

Vector4 Matrix4::getDiagonal( ) const {
  GEOM_ASSERT(isValid());
  return Vector4(__M[0],__M[5],__M[10],__M[15]);
}

Vector4 Matrix4::getRow( uchar_t i ) const {
  GEOM_ASSERT(i < 4);
  uchar_t _offset = i * 4;
  return Vector4(__M[_offset],__M[_offset+1],__M[_offset+2],__M[_offset+3]);
}

void Matrix4::set( const real_t& m0, const real_t& m1, const real_t& m2, const real_t& m3,
                   const real_t& m4, const real_t& m5, const real_t& m6, const real_t& m7,
                   const real_t& m8, const real_t& m9, const real_t& m10, const real_t& m11,
                   const real_t& m12, const real_t& m13, const real_t& m14, const real_t& m15 ) {
  GEOM_ASSERT(std::finite(m0) && std::finite(m1) && std::finite(m2) &&
              std::finite(m3) && std::finite(m4) && std::finite(m5) &&
              std::finite(m6) && std::finite(m7) && std::finite(m8) &&
              std::finite(m9) && std::finite(m10) && std::finite(m11) &&
              std::finite(m12) && std::finite(m13) && std::finite(m14) &&
              std::finite(m15));
  __M[0]  = m0;  __M[1]  = m1;  __M[2]  = m2;  __M[3]  = m3;
  __M[4]  = m4;  __M[5]  = m5;  __M[6]  = m6;  __M[7]  = m7;
  __M[8]  = m8;  __M[9]  = m9;  __M[10] = m10; __M[11] = m11;
  __M[12] = m12; __M[13] = m13; __M[14] = m14; __M[15] = m15;
}

/*  --------------------------------------------------------------------- */

bool Matrix4::operator==( const Matrix4& m ) const {
  return
    (fabs(__M[0] - m.__M[0]) < GEOM_TOLERANCE) &&
    (fabs(__M[1] - m.__M[1]) < GEOM_TOLERANCE) &&
    (fabs(__M[2] - m.__M[2]) < GEOM_TOLERANCE) &&
    (fabs(__M[3] - m.__M[3]) < GEOM_TOLERANCE) &&
    (fabs(__M[4] - m.__M[4]) < GEOM_TOLERANCE) &&
    (fabs(__M[5] - m.__M[5]) < GEOM_TOLERANCE) &&
    (fabs(__M[6] - m.__M[6]) < GEOM_TOLERANCE) &&
    (fabs(__M[7] - m.__M[7]) < GEOM_TOLERANCE) &&
    (fabs(__M[8] - m.__M[8]) < GEOM_TOLERANCE) &&
    (fabs(__M[9] - m.__M[9]) < GEOM_TOLERANCE) &&
    (fabs(__M[10] - m.__M[10]) < GEOM_TOLERANCE) &&
    (fabs(__M[11] - m.__M[11]) < GEOM_TOLERANCE) &&
    (fabs(__M[12] - m.__M[12]) < GEOM_TOLERANCE) &&
    (fabs(__M[13] - m.__M[13]) < GEOM_TOLERANCE) &&
    (fabs(__M[14] - m.__M[14]) < GEOM_TOLERANCE) &&
    (fabs(__M[15] - m.__M[15]) < GEOM_TOLERANCE);
}

bool Matrix4::operator!=( const Matrix4& m ) const {
	return !(operator==(m));
}

Matrix4& Matrix4::operator+=( const Matrix4& m ) {
  __M[0] += m.__M[0]; __M[1] += m.__M[1];
  __M[2] += m.__M[2]; __M[3] += m.__M[3];
  __M[4] += m.__M[4]; __M[5] += m.__M[5];
  __M[6] += m.__M[6]; __M[7] += m.__M[7];
  __M[8] += m.__M[8]; __M[9] += m.__M[9];
  __M[10] += m.__M[10]; __M[11] += m.__M[11];
  __M[12] += m.__M[12]; __M[13] += m.__M[13];
  __M[14] += m.__M[14]; //__M[15] += m.__M[15];
  return *this;
}

Matrix4& Matrix4::operator-=( const Matrix4& m ) {
  __M[0] -= m.__M[0]; __M[1] -= m.__M[1];
  __M[2] -= m.__M[2]; __M[3] -= m.__M[3];
  __M[4] -= m.__M[4]; __M[5] -= m.__M[5];
  __M[6] -= m.__M[6]; __M[7] -= m.__M[7];
  __M[8] -= m.__M[8]; __M[9] -= m.__M[9];
  __M[10] -= m.__M[10]; __M[11] -= m.__M[11];
  __M[12] -= m.__M[12]; __M[13] -= m.__M[13];
  __M[14] -= m.__M[14]; //__M[15] -= m.__M[15];
  return *this;
}

Matrix4& Matrix4::operator*=( const Matrix4& m ) {
  *this = *this * m;
  return *this;
}

Matrix4& Matrix4::operator*=( const real_t& s ) {
  __M[0] *= s; __M[1] *= s; __M[2] *= s; __M[3] *= s;
  __M[4] *= s; __M[5] *= s; __M[6] *= s; __M[7] *= s;
  __M[8] *= s; __M[9] *= s; __M[10] *= s; __M[11] *= s;
  __M[12] *= s; __M[13] *= s; __M[14] *= s; __M[15] *= s;
  return *this;
}

Matrix4& Matrix4::operator/=( const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  __M[0] /= s; __M[1] /= s; __M[2] /= s; __M[3] /= s;
  __M[4] /= s; __M[5] /= s; __M[6] /= s; __M[7] /= s;
  __M[8] /= s; __M[9] /= s; __M[10] /= s; __M[11] /= s;
  __M[12] /= s; __M[13] /= s; __M[14] /= s; __M[15] /= s;
    return *this;
}

Matrix4 Matrix4::operator-( ) const {
  return Matrix4(-__M[0],-__M[1],-__M[2],-__M[3],
                 -__M[4],-__M[5],-__M[6],-__M[7],
                 -__M[8],-__M[9],-__M[10],-__M[11],
                 -__M[12],-__M[13],-__M[14],-__M[15]);
}

Matrix4 Matrix4::operator+( const Matrix4& m) const {
  return Matrix4(*this) += m;
}

Matrix4 Matrix4::operator-( const Matrix4& m ) const {
  return Matrix4(*this) -= m;
}

Matrix4 Matrix4::operator*( const Matrix4& m ) const {
  return Matrix4(m.__M[0]  * __M[0] + m.__M[4]  * __M[1] +
                 m.__M[8]  * __M[2] + m.__M[12] * __M[3],
                 m.__M[1]  * __M[0] + m.__M[5]  * __M[1] +
                 m.__M[9]  * __M[2] + m.__M[13] * __M[3],
                 m.__M[2]  * __M[0] + m.__M[6]  * __M[1] +
                 m.__M[10] * __M[2] + m.__M[14] * __M[3],
                 m.__M[3]  * __M[0] + m.__M[7]  * __M[1] +
                 m.__M[11] * __M[2] + m.__M[15] * __M[3],

                 m.__M[0]  * __M[4] + m.__M[4]  * __M[5] +
                 m.__M[8]  * __M[6] + m.__M[12] * __M[7],
                 m.__M[1]  * __M[4] + m.__M[5]  * __M[5] +
                 m.__M[9]  * __M[6] + m.__M[13] * __M[7],
                 m.__M[2]  * __M[4] + m.__M[6]  * __M[5] +
                 m.__M[10] * __M[6] + m.__M[14] * __M[7],
                 m.__M[3]  * __M[4] + m.__M[7]  * __M[5] +
                 m.__M[11] * __M[6] + m.__M[15] * __M[7],

                 m.__M[0]  * __M[8]  + m.__M[4]  * __M[9] +
                 m.__M[8]  * __M[10] + m.__M[12] * __M[11],
                 m.__M[1]  * __M[8]  + m.__M[5]  * __M[9] +
                 m.__M[9]  * __M[10] + m.__M[13] * __M[11],
                 m.__M[2]  * __M[8]  + m.__M[6]  * __M[9] +
                 m.__M[10] * __M[10] + m.__M[14] * __M[11],
                 m.__M[3]  * __M[8]  + m.__M[7]  * __M[9] +
                 m.__M[11] * __M[10] + m.__M[15] * __M[11],

                 m.__M[0]  * __M[12] + m.__M[4]  * __M[13] +
                 m.__M[8]  * __M[14] + m.__M[12] * __M[15],
                 m.__M[1]  * __M[12] + m.__M[5]  * __M[13] +
                 m.__M[9]  * __M[14] + m.__M[13] * __M[15],
                 m.__M[2]  * __M[12] + m.__M[6]  * __M[13] +
                 m.__M[10] * __M[14] + m.__M[14] * __M[15],
                 m.__M[3]  * __M[12] + m.__M[7]  * __M[13] +
                 m.__M[11] * __M[14] + m.__M[15] * __M[15]);
}

bool Matrix4::isOrthogonal( ) const {
  return
    getColumn(0).isOrthogonalTo(getColumn(1)) &&
    getColumn(0).isOrthogonalTo(getColumn(2)) &&
    getColumn(0).isOrthogonalTo(getColumn(3)) &&
    getColumn(1).isOrthogonalTo(getColumn(2)) &&
    getColumn(1).isOrthogonalTo(getColumn(3)) &&
    getColumn(2).isOrthogonalTo(getColumn(3));
}

bool Matrix4::isSingular( ) const {
  GEOM_ASSERT(isValid());
  return fabs(det(*this)) < GEOM_TOLERANCE;
}

bool Matrix4::isValid( ) const {
  return
    finite(__M[0]) && finite(__M[1]) &&
    finite(__M[2]) && finite(__M[3]) &&
    finite(__M[4]) && finite(__M[5]) &&
    finite(__M[6]) && finite(__M[7]) &&
    finite(__M[8]) && finite(__M[9]) &&
    finite(__M[10]) && finite(__M[11]) &&
    finite(__M[12]) && finite(__M[13]) &&
    finite(__M[14]) && finite(__M[15]);
}


/*  --------------------------------------------------------------------- */

Vector4 operator*( const Matrix4& m, const Vector4& v ) {
  return Vector4(m.__M[0] * v.x() + m.__M[1] * v.y() +
                 m.__M[2] * v.z() + m.__M[3] * v.w(),
                 m.__M[4] * v.x() + m.__M[5] * v.y() +
                 m.__M[6] * v.z() + m.__M[7] * v.w(),
                 m.__M[8] * v.x() + m.__M[9] * v.y() +
                 m.__M[10] * v.z() + m.__M[11] * v.w(),
                 m.__M[12] * v.x() + m.__M[13] * v.y() +
                 m.__M[14] * v.z() + m.__M[15] * v.w());
}

Vector3 operator*( const Matrix4& m, const Vector3& v )
{
  register double h= m(3,0) * v.x()
                    +m(3,1) * v.y()
                    +m(3,2) * v.z()
                    +m(3,3);
  h= 1. / h;
  register real_t x= v.x(), y= v.y(), z= v.z();
  return
    Vector3( ( m(0,0) * x + m(0,1) * y + m(0,2) * z + m(0,3) ) * h,
             ( m(1,0) * x + m(1,1) * y + m(1,2) * z + m(1,3) ) * h,
             ( m(2,0) * x + m(2,1) * y + m(2,2) * z + m(2,3) ) * h );
}

Matrix4 operator*( const Matrix4& m, const real_t& s ) {
  return Matrix4(m) *= s;
}

Matrix4 operator/( const Matrix4& m, const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  return Matrix4(m) /= s;
}

Matrix4 adjoint( const Matrix4& m ) {
  GEOM_ASSERT(m.isValid());
  return Matrix4(GEOM_DET3(m.__M[5],m.__M[6],m.__M[7],
                           m.__M[9],m.__M[10],m.__M[11],
                           m.__M[13],m.__M[14],m.__M[15]),
                 -GEOM_DET3(m.__M[1],m.__M[2],m.__M[3],
                            m.__M[9],m.__M[10],m.__M[11],
                            m.__M[13],m.__M[14],m.__M[15]),
                 GEOM_DET3(m.__M[1],m.__M[2],m.__M[3],
                           m.__M[5],m.__M[6],m.__M[7],
                           m.__M[13],m.__M[14],m.__M[15]),

                 -GEOM_DET3(m.__M[1],m.__M[2],m.__M[3],
                         m.__M[5],m.__M[6],m.__M[7],
                         m.__M[9],m.__M[10],m.__M[11]),

                 -GEOM_DET3(m.__M[4],m.__M[6],m.__M[7],
                           m.__M[8],m.__M[10],m.__M[11],
                           m.__M[12],m.__M[14],m.__M[15]),
                 GEOM_DET3(m.__M[0],m.__M[2],m.__M[3],
                            m.__M[8],m.__M[10],m.__M[11],
                            m.__M[12],m.__M[14],m.__M[15]),
                 -GEOM_DET3(m.__M[0],m.__M[2],m.__M[3],
                           m.__M[4],m.__M[6],m.__M[7],
                           m.__M[12],m.__M[14],m.__M[15]),
                 GEOM_DET3(m.__M[0],m.__M[2],m.__M[3],
                            m.__M[4],m.__M[6],m.__M[7],
                            m.__M[8],m.__M[10],m.__M[11]),

                 GEOM_DET3(m.__M[4],m.__M[5],m.__M[7],
                           m.__M[8],m.__M[9],m.__M[11],
                           m.__M[12],m.__M[13],m.__M[15]),
                 -GEOM_DET3(m.__M[0],m.__M[1],m.__M[3],
                            m.__M[8],m.__M[9],m.__M[11],
                            m.__M[12],m.__M[13],m.__M[15]),
                 GEOM_DET3(m.__M[0],m.__M[1],m.__M[3],
                           m.__M[4],m.__M[5],m.__M[7],
                           m.__M[12],m.__M[13],m.__M[15]),
                 -GEOM_DET3(m.__M[0],m.__M[1],m.__M[3],
                            m.__M[4],m.__M[5],m.__M[7],
                            m.__M[8],m.__M[9],m.__M[11]),

                 -GEOM_DET3(m.__M[4],m.__M[5],m.__M[6],
                           m.__M[8],m.__M[9],m.__M[10],
                           m.__M[12],m.__M[13],m.__M[14]),
                 GEOM_DET3(m.__M[0],m.__M[1],m.__M[2],
                            m.__M[8],m.__M[9],m.__M[10],
                            m.__M[12],m.__M[13],m.__M[14]),
                 -GEOM_DET3(m.__M[0],m.__M[1],m.__M[2],
                           m.__M[4],m.__M[5],m.__M[6],
                           m.__M[12],m.__M[13],m.__M[14]),
                 GEOM_DET3(m.__M[0],m.__M[1],m.__M[2],
                            m.__M[4],m.__M[5],m.__M[6],
                            m.__M[8],m.__M[9],m.__M[10]));
}

real_t det( const Matrix4& m ) {
  GEOM_ASSERT(m.isValid());
  return (m.__M[0] * GEOM_DET3(m.__M[5],m.__M[6],m.__M[7],
                               m.__M[9],m.__M[10],m.__M[11],
                               m.__M[13],m.__M[14],m.__M[15]) -
          m.__M[1] * GEOM_DET3(m.__M[4],m.__M[6],m.__M[7],
                               m.__M[8],m.__M[10],m.__M[11],
                               m.__M[12],m.__M[14],m.__M[15]) +
          m.__M[2] * GEOM_DET3(m.__M[4],m.__M[5],m.__M[7],
                               m.__M[8],m.__M[9],m.__M[11],
                               m.__M[12],m.__M[13],m.__M[15]) -
          m.__M[3] * GEOM_DET3(m.__M[4],m.__M[5],m.__M[6],
                               m.__M[8],m.__M[9],m.__M[10],
                               m.__M[12],m.__M[13],m.__M[14]));
}

Matrix4 inverse( const Matrix4& m ) {
  GEOM_ASSERT(m.isValid());
  Matrix4 _adjoint = adjoint(m);
  real_t _det = (m.__M[0] * _adjoint.__M[0] +
               m.__M[1] * _adjoint.__M[4] +
               m.__M[2] * _adjoint.__M[8] +
               m.__M[3] * _adjoint.__M[12]);
  return _adjoint / _det;
}

real_t trace( const Matrix4& m ) {
  GEOM_ASSERT(m.isValid());
  return m.__M[0] + m.__M[5] + m.__M[10] + m.__M[15];
}

Matrix4 transpose( const Matrix4& m ) {
  GEOM_ASSERT(m.isValid());
  return Matrix4(m.__M[0], m.__M[4], m.__M[8],  m.__M[12],
                 m.__M[1], m.__M[5], m.__M[9],  m.__M[13],
                 m.__M[2], m.__M[6], m.__M[10], m.__M[14],
                 m.__M[3], m.__M[7], m.__M[11], m.__M[15]);
}

ostream& operator<<( ostream& stream, const Matrix4& m ) {
  stream << "[[" << m.__M[0] << "," << m.__M[1] << "," << m.__M[2] << "," << m.__M[3] << "]," << endl;
  stream << "[" << m.__M[4] << "," << m.__M[5] << "," << m.__M[6] << "," << m.__M[7] << "],"<< endl;
  stream << "[" << m.__M[8] << "," << m.__M[9] << "," << m.__M[10] << "," << m.__M[11] << "],"<< endl;
  return stream << "[" << m.__M[12] << "," << m.__M[13] << "," << m.__M[14]  << "," << m.__M[15] << "]]";
}

/*  --------------------------------------------------------------------- */

Matrix4 Matrix4::translation( const Vector3& v ) {
  GEOM_ASSERT(v.isValid());
  return Matrix4( 1, 0, 0, v.x(),
                  0, 1, 0, v.y(),
                  0, 0, 1, v.z(),
                  0, 0, 0, 1);
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4::setTransformation( const Vector3& scale,
                                 const Vector3& rotate,
                                 const Vector3& translate )
{
#define m(i,j) __M[j+4*i]

  m(3,0)= m(3,1)= m(3,2)= 0.;
  m(3,3)= 1.;
  m(0,3)= translate.x();
  m(1,3)= translate.y();
  m(2,3)= translate.z();
  double c1= cos(rotate.z()), s1= sin(rotate.z());
  double c2= cos(rotate.y()), s2= sin(rotate.y());
  double c3= cos(rotate.x()), s3= sin(rotate.x());
  double sx= scale.x(), sy= scale.y(), sz= scale.z();

  m(0,0)= sx*c2*c3;
  m(1,0)= sx*c2*s3;
  m(2,0)=-sx*s2;
  m(0,1)= sy*(s1*s2*c3-c1*s3);
  m(1,1)= sy*(s1*s2*s3+c1*c3);
  m(2,1)= sy*s1*c2;
  m(0,2)= sz*(c1*s2*c3+s1*s3);
  m(1,2)= sz*(c1*s2*s3-s1*c3);
  m(2,2)= sz*c1*c2;

#undef m
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4::getTransformation( Vector3& scale,
                                 Vector3& rotate,
                                 Vector3& translate ) const
{
#define m(i,j) __M[4*i+j]
  translate.x()= m(0,3);
  translate.y()= m(1,3);
  translate.z()= m(2,3);

  real_t a1, a2, a3;
  real_t c1, c2, c3, s1, s2, s3;
  if( fabs(m(1,0)) < GEOM_EPSILON && fabs(m(0,0)) < GEOM_EPSILON )
    {
    c2= 0.;
    s2= ( m(2,0) > 0 ) ? -1. : 1.;
    a2= ( m(2,0) > 0 ) ? -GEOM_PI/2 : GEOM_PI/2;

    // not a unique solution
    a1= 0.;
    c1= 1., s1= 0.;

    if( fabs(m(1,1)) < GEOM_EPSILON )
      a3= ( m(0,1) > 0 ) ? -GEOM_PI/2 : GEOM_PI/2;
    else
    if( a2 < 0. )
      a3= -atan2(-m(0,1), m(1,1));
    else
      a3= atan2(-m(0,1), m(1,1));
    c3= cos(a3), s3= sin(a3);
    }
  else
    {
    // be careful, angle are choose with a potential error.
    // Sign is not representative

    a3= atan2(m(1,0),m(0,0));
    c3= cos(a3), s3= sin(a3);

    if( fabs(c3) > fabs(s3) ){
      a2= atan2( -m(2,0) * c3, m(0,0) );
	}
    else
      a2= atan2( -m(2,0) * s3, m(1,0) );
    c2= cos(a2), s2= sin(a2);

    // angle verification
    if( ( m(2,0) * s2 > 0. ) ||
        ( ( fabs(s2) < GEOM_EPSILON ) &&
          ( (c2*c3*m(0,0) < 0.) || ( c2*s3*m(1,0) < 0. ) ) ) )
      {
      a2+= ( a2 < 0. ) ? GEOM_PI : -GEOM_PI;
      c2= -c2, s2= -s2;
      if( c2 < 0. )
        {
        a3+= ( a3 < 0. ) ? GEOM_PI : -GEOM_PI;
        c3= -c3, s3= -s3;
        }
      }

    if( fabs(c3) > fabs(s3) )
      {
      a1= atan2( m(2,1) * c3, m(1,1) * c2 - m(2,1) * s2 * s3);
     // angle verification
      if( c2*c3 < 0 )
        a1+= ( a1 < 0. ) ? GEOM_PI : -GEOM_PI;
      }
    else
      {
      a1= atan2( m(2,1) * s3, - m(0,1) * c2 + m(2,1) * s2 * c3);
      // angle verification
      if( c2*s3 < 0 )
        a1+= ( a1 < 0. ) ? GEOM_PI : -GEOM_PI;
      }
    c1= cos(a1), s1= sin(a1);
    }

  double sx= 1., sy= 1., sz= 1.;

  switch( Vector3(m(0,0),m(1,0),m(2,0)).getMaxAbsCoord() )
    {
    case 0: sx= m(0,0) / (c2*c3); break;
    case 1: sx= m(1,0) / (c2*s3); break;
    case 2: sx= m(2,0) / -s2; break;
    }
  switch( Vector3(m(0,1),m(1,1),m(2,1)).getMaxAbsCoord() )
    {
    case 0: sy= m(0,1) / (s1*s2*c3-c1*s3); break; //
    case 1: sy= m(1,1) / (s1*s2*s3+c1*c3); break;
    case 2: sy= m(2,1) / (s1*c2); break;
    }
  switch( Vector3(m(0,2),m(1,2),m(2,2)).getMaxAbsCoord() )
    {
    case 0: sz= m(0,2) / (c1*s2*c3+s1*s3); break;
    case 1: sz= m(1,2) / (c1*s2*s3-s1*c3); break;
    case 2: sz= m(2,2) / (c1*c2); break;
    }

  assert(sx > 0.);
  assert(sy > 0.);
  assert(sz > 0.);

  scale= Vector3(sx,sy,sz);
  rotate= Vector3(a3,a2,a1);

#undef m
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4::setTransformation2( const Vector3& scale,
                                 const Vector3& rotate,
                                 const Vector3& translate )
{
#define m(i,j) __M[j+4*i]

  m(3,0)= m(3,1)= m(3,2)= 0.;
  m(3,3)= 1.;
  m(0,3)= translate.x();
  m(1,3)= translate.y();
  m(2,3)= translate.z();
  double c1= cos(rotate.z()), s1= sin(rotate.z());
  double c2= cos(rotate.y()), s2= sin(rotate.y());
  double c3= cos(rotate.x()), s3= sin(rotate.x());
  double sx= scale.x(), sy= scale.y(), sz= scale.z();

  m(0,0)= sx*c2*c3;
  m(1,0)= sy*c2*s3;
  m(2,0)=-sz*s2;
  m(0,1)= sx*(s1*s2*c3-c1*s3);
  m(1,1)= sy*(s1*s2*s3+c1*c3);
  m(2,1)= sz*s1*c2;
  m(0,2)= sx*(c1*s2*c3+s1*s3);
  m(1,2)= sy*(c1*s2*s3-s1*c3);
  m(2,2)= sz*c1*c2;

#undef m
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4::getTransformation2( Vector3& scale,
                                  Vector3& rotate,
                                  Vector3& translate ) const
{
#define m(i,j) __M[4*i+j]
  translate.x()= m(0,3);
  translate.y()= m(1,3);
  translate.z()= m(2,3);

  real_t a1, a2, a3;
  real_t c1, c2, c3, s1, s2, s3;
  if( fabs(m(1,0)) < GEOM_EPSILON && fabs(m(0,0)) < GEOM_EPSILON )
    {
    c2= 0.;
    s2= ( m(2,0) > 0 ) ? -1. : 1.;
    a2= ( m(2,0) > 0 ) ? -GEOM_PI/2 : GEOM_PI/2;

    // not a unique solution
    a1= 0.;
    c1= 1., s1= 0.;

    if( fabs(m(1,1)) < GEOM_EPSILON )
      a3= ( m(0,1) > 0 ) ? -GEOM_PI/2 : GEOM_PI/2;
    else
    if( a2 < 0. )
      a3= atan2(-m(0,1), -m(0,2));
    else
      a3= atan2(-m(0,1), m(0,2));
    c3= cos(a3), s3= sin(a3);
    }
  else 
    {
    // be careful, angle are choose with a potential error.
    // Sign is not representative

    a1= atan2(m(2,1),m(2,2));
    c1= cos(a1), s1= sin(a1);

    if( fabs(c1) > fabs(s1) )
      a2= atan2( -m(2,0) * c1, m(2,2) );
     else
      a2= atan2( -m(2,0) * s1, m(2,1) );
    c2= cos(a2), s2= sin(a2);

    // angle verification
    if( ( m(2,0) * s2 > 0. ) ||
        ( ( fabs(s2) < GEOM_EPSILON ) &&
          ( (c2*s1*m(2,1) < 0.) || ( c2*c1*m(2,2) < 0. ) ) ) )
      {
      a2+= ( a2 < 0. ) ? GEOM_PI : -GEOM_PI;
      c2= -c2, s2= -s2;
      if( c2 < 0. )
        {
        a1+= ( a1 < 0. ) ? GEOM_PI : -GEOM_PI;
        c1= -c1, s1= -s1;
        }
      }

    if( fabs(c1) > fabs(s1) )
      {
      a3 = atan2( m(1,0) * c1, m(1,1) * c2 - m(1,0) * s2 * s1);
      // angle verification
      if( c2*c1 < 0 )
        a3+= ( a3 < 0. ) ? GEOM_PI : -GEOM_PI;
      }
    else
      {
      a3= atan2( m(1,0) * s1, - m(1,2) * c2 + m(1,0) * s2 * c1);
      // angle verification
      if( c2*s1 < 0 )
        a3+= ( a3 < 0. ) ? GEOM_PI : -GEOM_PI;
      }
    c3= cos(a3), s3= sin(a3);
    }

  double sx= 1., sy= 1., sz= 1.;

  switch( Vector3(m(0,0),m(0,1),m(0,2)).getMaxAbsCoord() )
    {
    case 0: sx= m(0,0) / (c2*c3); break;
    case 1: sx= m(0,1) / (s1*s2*c3-c1*s3); break;
    case 2: sx= m(0,2) / (c1*s2*c3+s1*s3); break;
    }
  switch( Vector3(m(1,0),m(1,1),m(1,2)).getMaxAbsCoord() )
    {
    case 0: sy= m(1,0) / (c2*s3); break;
    case 1: sy= m(1,1) / (s1*s2*s3+c1*c3); break;
    case 2: sy= m(1,2) / (c1*s2*s3-s1*c3); break;
    }
  switch( Vector3(m(2,0),m(2,1),m(2,2)).getMaxAbsCoord() )
    {
    case 0: sz= m(2,0) / -s2; break;
    case 1: sz= m(2,1) / (s1*c2); break;
    case 2: sz= m(2,2) / (c1*c2); break;
    }

  assert(sx > 0.);
  assert(sy > 0.);
  assert(sz > 0.);

  scale= Vector3(sx,sy,sz);
  rotate= Vector3(a3,a2,a1);

#undef m
}

void Matrix4::getTransformationB( Vector3& scale,
                                  Vector3& rotate,
                                  Vector3& translate ) const
{
#define m(i,j) __M[4*i+j]
  translate.x()= m(0,3);
  translate.y()= m(1,3);
  translate.z()= m(2,3);

  Vector3 a(m(0,0),m(1,0),m(2,0));
  Vector3 b(m(0,1),m(1,1),m(2,1));
  Vector3 c(m(0,2),m(1,2),m(2,2));
  scale.x()= a.normalize();
  scale.y()= b.normalize();
  scale.z()= c.normalize();

  real_t thetaX,thetaY,thetaZ;
  thetaY = asin(-a.z());
  if(thetaY < GEOM_HALF_PI-GEOM_EPSILON){
	if(thetaY > -GEOM_HALF_PI+GEOM_EPSILON){
		thetaZ = atan2(a.y(),a.x());
		thetaX = atan2(b.z(),c.z());
	}
	else {
		// Not a unique solution
		thetaZ = -atan2(-b.x(),c.x());
		thetaX = 0;
	}
  }
  else {
	  // Not a unique solution
	  thetaZ = atan2(-b.x(),c.x());
	  thetaX = 0;
  }
  rotate.x() = thetaZ;
  rotate.y() = thetaY;
  rotate.z() = thetaX;
}

/*  --------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

