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




#include "util_vector.h"
#include "util_math.h"
#include "plantgl/tool/util_assert.h"
#include <iostream>
using namespace std;

TOOLS_BEGIN_NAMESPACE

/*  --------------------------------------------------------------------- */

#define __X __T[0]
#define __Y __T[1]
#define __Z __T[2]
#define __W __T[3]

/*  --------------------------------------------------------------------- */

const Vector2 Vector2::ORIGIN;
const Vector2 Vector2::OX(1,0);
const Vector2 Vector2::OY(0,1);


/*  --------------------------------------------------------------------- */

Vector2::Polar::Polar( const real_t& radius, const real_t& theta ) :
  radius(radius),
  theta(theta) {
}

Vector2::Polar::Polar( const Vector2& v ) :
  radius(hypot(v.x(),v.y())),
  theta(atan2(v.y(),v.x())) {
}

bool
Vector2::Polar::isValid( ) const {
  return finite(radius) && finite(theta);
}

/*  --------------------------------------------------------------------- */

Vector2::Vector2( const real_t& x , const real_t& y ) :
  Tuple2<real_t>(x,y) {
  GEOM_ASSERT(isValid());
}


Vector2::Vector2( const Polar& p ) :
  Tuple2<real_t>(p.radius * cos(p.theta), p.radius * sin(p.theta)) {
  GEOM_ASSERT(p.isValid());
  GEOM_ASSERT(isValid());
}

void
Vector2::set( const real_t& x , const real_t& y ) {
  __X = x;
  __Y = y;
  GEOM_ASSERT(isValid());
}

void
Vector2::set( const Vector2& v ) {
  __X = v.__X;
  __Y = v.__Y;
  GEOM_ASSERT(isValid());
}

void
Vector2::set( const real_t * v2 ) {
  __X = v2[0];
  __Y = v2[1];
  GEOM_ASSERT(isValid());
}

void
Vector2::set( const Polar& p ) {
  __X = p.radius * cos(p.theta);
  __Y = p.radius * sin(p.theta)
  GEOM_ASSERT(p.isValid());
  GEOM_ASSERT(isValid());
}

Vector2::~Vector2( ) {
}

const real_t&
Vector2::x( ) const {
  return __X;
}

real_t& 
Vector2::x( ) {
  return __X;
}

const real_t& 
Vector2::y( ) const {
  return __Y;
}

real_t& 
Vector2::y( ) {
  return __Y;
}

int 
Vector2::getMaxAbsCoord() const
{
    return ( fabs(__X) > fabs(__Y) )  ? 0 : 1;
}

int 
Vector2::getMinAbsCoord() const
{
    return ( fabs(__X) < fabs(__Y) )  ? 0 : 1;
}

/*  --------------------------------------------------------------------- */

bool 
Vector2::operator==( const Vector2& v ) const {
  return normLinf(*this - v) < GEOM_TOLERANCE;
}

bool  
Vector2::operator!=( const Vector2& v ) const {
  return normLinf(*this - v) >= GEOM_TOLERANCE;
}

Vector2& 
Vector2::operator+=( const Vector2& v ) {
  __X += v.__X; __Y += v.__Y;
  return *this;
}

Vector2& 
Vector2::operator-=( const Vector2& v ) {
  __X -= v.__X; __Y -= v.__Y;
  return *this;
}

Vector2& 
Vector2::operator*=( const real_t& s ) {
  __X *= s; __Y *= s;
  return *this;
}

Vector2& 
Vector2::operator/=( const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  __X /= s; __Y /= s;
  return *this;
}

Vector2 
Vector2::operator-( ) const {
  return Vector2(-__X, -__Y);
}

Vector2 Vector2::operator+( const Vector2& v) const {
  return Vector2(*this) += v;
}

Vector2 Vector2::operator-( const Vector2& v ) const {
  return Vector2(*this) -= v;
}

bool 
Vector2::isNormalized( ) const {
  return fabs(normSquared(*this) - 1) < GEOM_EPSILON;
}

bool 
Vector2::isOrthogonalTo( const Vector2& v ) const {
  return fabs(dot(*this,v)) < GEOM_EPSILON;
}

bool
Vector2::isValid( ) const {
  return finite(__X) && finite(__Y);
}

real_t
Vector2::normalize( ) {
  real_t _norm = norm(*this);
  if (_norm > GEOM_TOLERANCE) *this /= _norm;
  return _norm;
}

/*  --------------------------------------------------------------------- */

Vector2 operator*( const Vector2& v, const real_t& s ) {
  return Vector2(v) *= s;
}

Vector2 operator*( const real_t& s, const Vector2& v ) {
  return Vector2(v) *= s;
}

ostream& operator<<( ostream& stream, const Vector2& v ) {
  return stream << "<" << v.__X << "," << v.__Y << ">";
}

bool operator<(const Vector2& v1, const Vector2& v2)
{
  if(v1.__X < v2.__X)
    {
      return true;
    }
  if(v1.__X > v2.__X)
    {
      return false;
    }
  return v1.__Y < v2.__Y;
}

Vector2 operator/( const Vector2& v, const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  return Vector2(v) /= s;
}

Vector2 abs( const Vector2& v ) {
  return Vector2(fabs(v.__X),fabs(v.__Y));
}

Vector2 direction( const Vector2& v ) {
  return v / norm(v);
}

real_t cross( const Vector2& v1,const Vector2& v2) 
{
  return v1.__X*v2.__Y - v1.__Y*v2.__X;
}

real_t operator^( const Vector2& v1,const Vector2& v2)
{
  return v1.__X*v2.__Y - v1.__Y*v2.__X;
}

real_t dot( const Vector2& v1, const Vector2& v2 ) {
  return (v1.__X * v2.__X + v1.__Y * v2.__Y);
}

real_t operator*( const Vector2& v1, const Vector2& v2 ) {
  return (v1.__X * v2.__X + v1.__Y * v2.__Y);
}

Vector2 Max( const Vector2& v1, const Vector2& v2 ) {
  return Vector2(max(v1.__X,v2.__X),max(v1.__Y,v2.__Y));
}

Vector2 Min( const Vector2& v1, const Vector2& v2 ) {
  return Vector2(min(v1.__X,v2.__X),min(v1.__Y,v2.__Y));
}

real_t norm( const Vector2& v ) {
  return sqrt(normSquared(v));
}

real_t normL1( const Vector2& v ) {
  return sum(abs(v));
}

real_t normLinf( const Vector2& v ) {
  return *(abs(v).getMax());
}

real_t normSquared( const Vector2& v ) {
  return dot(v,v);
}

real_t sum( const Vector2& v ) {
  return v.__X + v.__Y;
}

/*
real_t angle( const Vector2& v1 , const Vector2& v2 ) {
	real_t n = norm(v1)*norm(v2);
	if(n < GEOM_EPSILON)return 0;
	return acos(dot(v1,v2)/n);
*/
real_t angle( const Vector2& v1 , const Vector2& v2 ) {
  real_t cosinus = v1*v2;
  real_t sinus = v1^v2;
  return atan2( sinus, cosinus );
}

/*  --------------------------------------------------------------------- */

const Vector3 Vector3::ORIGIN;
const Vector3 Vector3::OX(1,0,0);
const Vector3 Vector3::OY(0,1,0);
const Vector3 Vector3::OZ(0,0,1);


/*  --------------------------------------------------------------------- */

Vector3::Cylindrical::Cylindrical( const real_t& radius, const real_t& theta, const real_t& z ) :
  radius(radius),
  theta(theta),
  z(z) {
  GEOM_ASSERT(isValid());
}

Vector3::Cylindrical::Cylindrical( const Vector3& v ) :
  radius(hypot(v.x(),v.y())),
  theta(atan2(v.y(),v.x())),
  z(v.z()) {
  GEOM_ASSERT(isValid());
}

bool
Vector3::Cylindrical::isValid( ) const {
  return
    finite(radius) &&
    finite(theta) &&
    finite(z);
}

/*  --------------------------------------------------------------------- */

Vector3::Spherical::Spherical( const real_t& radius, const real_t& theta, const real_t& phi ) :
  radius(radius),
  theta(theta),
  phi(phi) {
  GEOM_ASSERT(isValid());
}

Vector3::Spherical::Spherical( const Vector3& v ) :
  radius(v.x() * v.x() + v.y() * v.y()),
  theta(atan2(v.y(),v.x())),
  phi(0) {
  phi = atan2(sqrt(radius),v.z());
  radius = sqrt(radius + v.z() * v.z());
}

bool
Vector3::Spherical::isValid( ) const {
  return
    finite(radius) &&
    finite(theta) &&
    finite(phi);
}

/*  --------------------------------------------------------------------- */

Vector3::Vector3( const real_t& x,
		  const real_t& y,
		  const real_t& z ) :
  Tuple3<real_t>(x,y,z) {
  GEOM_ASSERT(isValid());
}


Vector3::Vector3( const Vector2& v, const real_t& z ) :
  Tuple3<real_t>(v.x(),v.y(),z) {
  GEOM_ASSERT(v.isValid());
  GEOM_ASSERT(isValid());
}

Vector3::Vector3( const Cylindrical& c ) :
  Tuple3<real_t>(c.radius * cos(c.theta),
	       c.radius * sin(c.theta),
	       c.z) {
  GEOM_ASSERT(c.isValid());
  GEOM_ASSERT(isValid());
}

Vector3::Vector3( const Spherical& s ) :
  Tuple3<real_t>(cos(s.theta),
	       sin(s.theta),
	       s.radius * cos(s.phi)) {
  real_t _tmp = s.radius * sin(s.phi);
  __X *= _tmp;
  __Y *= _tmp;
  GEOM_ASSERT(s.isValid());
  GEOM_ASSERT(isValid());
}

void
Vector3::set( const real_t& x,
		  const real_t& y,
		  const real_t& z )  {
	__X = x;
	__Y = y;
	__Z = z;
  GEOM_ASSERT(isValid());
}

void
Vector3::set( const real_t * v3 ) {
	__X = v3[0];
	__Y = v3[1];
	__Z = v3[2];
  GEOM_ASSERT(isValid());
}

void
Vector3::set( const Vector2& v, const real_t& z ) {
  GEOM_ASSERT(v.isValid());
	__X = v.x();
	__Y = v.y();
	__Z = z;
  GEOM_ASSERT(isValid());
}

void
Vector3::set( const Cylindrical& c ) {
  GEOM_ASSERT(c.isValid());
	__X = c.radius * cos(c.theta);
	__Y = c.radius * sin(c.theta);
	__Z = c.z;
  GEOM_ASSERT(isValid());
}

void
Vector3::set( const Spherical& s ) {
  GEOM_ASSERT(s.isValid());
  real_t _tmp = s.radius * sin(s.phi);
  __X = cos(s.theta) * _tmp;
  __Y = sin(s.theta) * _tmp;
  __Z = s.radius * cos(s.phi);
  GEOM_ASSERT(isValid());
}

void
Vector3::set( const Vector3& v )  {
	__X = v.__X;
	__Y = v.__Y;
	__Z = v.__Z;
  GEOM_ASSERT(isValid());
}

Vector3::~Vector3( ) {
}

const real_t& 
Vector3::x( ) const {
  return __X;
}

real_t&
Vector3::x( ) {
  return __X;
}

const real_t&
Vector3::y( ) const {
  return __Y;
}

real_t&
Vector3::y( ) {
  return __Y;
}

const real_t& 
Vector3::z( ) const {
  return __Z;
}

real_t&
Vector3::z( ) {
  return __Z;
}

/*  --------------------------------------------------------------------- */

bool 
Vector3::operator==( const Vector3& v ) const {
  return normLinf(*this - v) < GEOM_TOLERANCE;
}

bool 
Vector3::operator!=( const Vector3& v ) const {
  return normLinf(*this - v) >= GEOM_TOLERANCE;
}

Vector3& 
Vector3::operator+=( const Vector3& v ) {
  __X += v.__X; __Y += v.__Y; __Z += v.__Z;
  return *this;
}
Vector3& 
Vector3::operator-=( const Vector3& v ) {
  __X -= v.__X; __Y -= v.__Y; __Z -= v.__Z;
  return *this;
}
Vector3& 
Vector3::operator*=( const real_t& s ) {
  __X *= s; __Y *= s; __Z *= s;
  return *this;
}

Vector3& 
Vector3::operator/=( const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  __X /= s; __Y /= s; __Z /= s;
  return *this;
}

Vector3 
Vector3::operator-( ) const {
  return Vector3(-__X, -__Y, -__Z);
}

Vector3 Vector3::operator+( const Vector3& v) const {
  return Vector3(*this) += v;
}

Vector3 Vector3::operator-( const Vector3& v ) const {
  return Vector3(*this) -= v;
}

bool 
Vector3::isNormalized( ) const {
  return fabs(normSquared(*this) - 1) < GEOM_EPSILON;
}

bool 
Vector3::isOrthogonalTo( const Vector3& v ) const {
  return fabs(dot(*this,v)) < GEOM_EPSILON;
}

bool 
Vector3::isValid( ) const {
  return finite(__X) && finite(__Y) && finite(__Z);
}

real_t 
Vector3::normalize( )  {
  real_t _norm = norm(*this);
  if (_norm > GEOM_TOLERANCE) *this /= _norm;
  return _norm;
}

Vector2 
Vector3::project( ) const {
  return Vector2(__X / __Z,__Y / __Z);
}

int 
Vector3::getMaxAbsCoord() const
{
  if( fabs(__X) > fabs(__Y) )
    return ( fabs(__X) > fabs(__Z) )  ? 0 : 2;
  else
    return ( fabs(__Y) > fabs(__Z) ) ? 1 : 2;
}

int 
Vector3::getMinAbsCoord() const
{
  if( fabs(__X) < fabs(__Y) )
    return ( fabs(__X) < fabs(__Z) )  ? 0 : 2;
  else
    return ( fabs(__Y) < fabs(__Z) ) ? 1 : 2;
}

/*  --------------------------------------------------------------------- */

Vector3 operator*( const Vector3& v, const real_t& s ) {
  return Vector3(v) *= s;
}

Vector3 operator*( const real_t& s, const Vector3& v ) {
  return Vector3(v) *= s;
}

Vector3 operator/( const Vector3& v, const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  return Vector3(v) /= s;
}

ostream& operator<<( ostream& stream, const Vector3& v ) {
  return stream << "<" << v.__X << "," << v.__Y << "," << v.__Z << ">";
}

bool operator<(const Vector3& v1, const Vector3& v2)
{
  if(v1.__X < v2.__X)
    {
      return true;
    }
  if(v1.__X > v2.__X)
    {
      return false;
    }
  if(v1.__Y < v2.__Y)
    {
      return true;
    }
  if(v1.__Y > v2.__Y)
    {
      return false;
    }
  return v1.__Z < v2.__Z;
}

Vector3 abs( const Vector3& v ) {
  return Vector3(fabs(v.__X),fabs(v.__Y),fabs(v.__Z));
}

Vector3 cross( const Vector3& v1, const Vector3& v2 ) {
  return Vector3(v1.__Y * v2.__Z - v1.__Z * v2.__Y,
		 v1.__Z * v2.__X - v1.__X * v2.__Z,
		 v1.__X * v2.__Y - v1.__Y * v2.__X);
}

Vector3 operator^( const Vector3& v1, const Vector3& v2 ) {
  return Vector3(v1.__Y * v2.__Z - v1.__Z * v2.__Y,
		 v1.__Z * v2.__X - v1.__X * v2.__Z,
		 v1.__X * v2.__Y - v1.__Y * v2.__X);
}

Vector3 direction( const Vector3& v ) {
  return v / norm(v);
}

real_t dot( const Vector3& v1, const Vector3& v2 ) {
  return (v1.__X * v2.__X + v1.__Y * v2.__Y + v1.__Z * v2.__Z);
}

real_t operator*( const Vector3& v1, const Vector3& v2 ) {
  return (v1.__X * v2.__X + v1.__Y * v2.__Y + v1.__Z * v2.__Z);
}

Vector3 Max( const Vector3& v1, const Vector3& v2 ) {
  return Vector3(max(v1.__X,v2.__X),
		 max(v1.__Y,v2.__Y),
		 max(v1.__Z,v2.__Z));
}

Vector3 Min( const Vector3& v1, const Vector3& v2 ) {
  return Vector3(min(v1.__X,v2.__X),
		 min(v1.__Y,v2.__Y),
		 min(v1.__Z,v2.__Z));
}

real_t norm( const Vector3& v ) {
  return sqrt(normSquared(v));
}

real_t normL1( const Vector3& v ) {
  return sum(abs(v));
}

real_t normLinf( const Vector3& v ) {
  return *(abs(v).getMax());
}

real_t normSquared( const Vector3& v ) {
  return dot(v,v);
}

real_t sum( const Vector3& v ) {
  return v.__X + v.__Y + v.__Z;
}

real_t angle( const Vector3& v1 , const Vector3& v2 ) {
	real_t n = norm(v1)*norm(v2);
	if(n < GEOM_EPSILON)return 0;
	return acos(dot(v1,v2)/n);
}

real_t angle( const Vector3& v1, const Vector3& v2, const Vector3& axis )
{
  double x,y;
  Vector3 vy;
  x = dot( v1,v2 );
  vy = cross( v1, v2 );
  y = norm( vy );
  if( dot( vy, axis ) < 0 )
    {
    return atan2( -y, x );
    }
  return atan2( y, x );
}


/*  --------------------------------------------------------------------- */

const Vector4 Vector4::ORIGIN;
const Vector4 Vector4::OX(1,0,0,0);
const Vector4 Vector4::OY(0,1,0,0);
const Vector4 Vector4::OZ(0,0,1,0);
const Vector4 Vector4::OW(0,0,0,1);


/*  --------------------------------------------------------------------- */

Vector4::Vector4( const real_t& x , const real_t& y ,
		  const real_t& z , const real_t& w ) :
    Tuple4<real_t>(x,y,z,w) {
  GEOM_ASSERT(isValid());
}

Vector4::Vector4( const Vector3& v, const real_t& w ) :
  Tuple4<real_t>(v.x(),v.y(),v.z(),w) {
  GEOM_ASSERT(v.isValid());
  GEOM_ASSERT(isValid());
}

Vector4::Vector4( const Vector2& v, const real_t& z , const real_t& w ) :
  Tuple4<real_t>(v.x(),v.y(),z,w) {
  GEOM_ASSERT(v.isValid());
  GEOM_ASSERT(isValid());
}

Vector4::~Vector4( ) {
}

void
Vector4::set( const real_t& x , const real_t& y ,
			  const real_t& z , const real_t& w ) {
	  __X = x;
	  __Y = y;
	  __Z = z;
	  __W = w;
  GEOM_ASSERT(isValid());
}

void
Vector4::set( const real_t * v4 ) {
	  __X = v4[0];
	  __Y = v4[1];
	  __Z = v4[2];
	  __W = v4[3];
  GEOM_ASSERT(isValid());
}

void
Vector4::set( const Vector3& v, const real_t& w ) {
  GEOM_ASSERT(v.isValid());
	  __X = v.x();
	  __Y = v.y();
	  __Z = v.z();
	  __W = w;
  GEOM_ASSERT(isValid());
}

void
Vector4::set( const Vector4& v) {
  GEOM_ASSERT(v.isValid());
	  __X = v.__X;
	  __Y = v.__Y;
	  __Z = v.__Z;
	  __W = v.__W;
  GEOM_ASSERT(isValid());
}

const real_t& Vector4::x( ) const {
  return __X;
}

real_t& Vector4::x( ) {
  return __X;
}

const real_t& Vector4::y( ) const {
  return __Y;
}

real_t& Vector4::y( ) {
  return __Y;
}

const real_t& Vector4::z( ) const {
  return __Z;
}

real_t& Vector4::z( ) {
  return __Z;
}

const real_t& Vector4::w( ) const {
  return __W;
}

real_t& Vector4::w( ) {
  return __W;
}

int 
Vector4::getMaxAbsCoord() const
{
    if ( fabs(__X) > fabs(__Y) ){
        if( fabs(__X) > fabs(__Z) )
            return ( fabs(__X) > fabs(__W) ) ? 0 : 3;
        else
            return ( fabs(__Z) > fabs(__W) ) ? 2 : 3;
    }
    else {
        if( fabs(__Y) > fabs(__Z) )
            return ( fabs(__Y) > fabs(__W) ) ? 1 : 3;
        else
            return ( fabs(__Z) > fabs(__W) ) ? 2 : 3;
    }
}

int 
Vector4::getMinAbsCoord() const
{
    if ( fabs(__X) < fabs(__Y) ){
        if( fabs(__X) < fabs(__Z) )
            return ( fabs(__X) < fabs(__W) ) ? 0 : 3;
        else
            return ( fabs(__Z) < fabs(__W) ) ? 2 : 3;
    }
    else {
        if( fabs(__Y) < fabs(__Z) )
            return ( fabs(__Y) < fabs(__W) ) ? 1 : 3;
        else
            return ( fabs(__Z) < fabs(__W) ) ? 2 : 3;
    }
}

/*  --------------------------------------------------------------------- */

bool Vector4::operator==( const Vector4& v ) const {
  return normLinf(*this - v) < GEOM_TOLERANCE;
}

bool Vector4::operator!=( const Vector4& v ) const {
  return normLinf(*this - v) >= GEOM_TOLERANCE;
}

Vector4& Vector4::operator+=( const Vector4& v ) {
  __X += v.__X; __Y += v.__Y; __Z += v.__Z; __W += v.__W;
  return *this;
}

Vector4& Vector4::operator-=( const Vector4& v ) {
  __X -= v.__X; __Y -= v.__Y; __Z -= v.__Z; __W -= v.__W;
  return *this;
}

Vector4& Vector4::operator*=( const real_t& s ) {
  __X *= s; __Y *= s; __Z *= s; __W *= s;
  return *this;
}

Vector4& Vector4::operator/=( const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
  __X /= s; __Y /= s; __Z /= s; __W /= s;
  return *this;
}

Vector4 Vector4::operator-( ) const {
  return Vector4(-__X, -__Y, -__Z,-__W);
}

Vector4 Vector4::operator+( const Vector4& v) const {
  return Vector4(*this) += v;
}

Vector4 Vector4::operator-( const Vector4& v ) const {
  return Vector4(*this) -= v;
}

bool Vector4::isNormalized( ) const {
  return fabs(normSquared(*this) - 1) < GEOM_EPSILON;
}

bool Vector4::isOrthogonalTo( const Vector4& v ) const {
  return fabs(dot(*this,v)) < GEOM_EPSILON;
}

bool Vector4::isValid( ) const {
  return
    finite(__X) && finite(__Y) &&
    finite(__Z) && finite(__W);
}

real_t Vector4::normalize( ) {
  real_t _norm = norm(*this);
  if (_norm > GEOM_TOLERANCE) *this /= _norm;
  return _norm;
}

Vector3 Vector4::project( ) const {
  GEOM_ASSERT(fabs(__W) > GEOM_TOLERANCE);
  return Vector3(__X / __W, __Y / __W, __Z / __W);
}

/*  --------------------------------------------------------------------- */

Vector4 operator*( const Vector4& v, const real_t& s ) {
  return Vector4(v) *= s;
}

Vector4 operator*( const real_t& s, const Vector4& v ) {
  return Vector4(v) *= s;
}

Vector4 operator/( const Vector4& v, const real_t& s ) {
  GEOM_ASSERT(fabs(s) > GEOM_TOLERANCE);
    return Vector4(v) /= s;
}

ostream& operator<<( ostream& stream, const Vector4& v ) {
  return stream << "<" << v.__X << "," << v.__Y << "," << v.__Z << "," << v.__W << ">";
}

bool operator<(const Vector4& v1, const Vector4& v2)
{
  if(v1.__X < v2.__X)
    {
      return true;
    }
  if(v1.__X > v2.__X)
    {
      return false;
    }
  if(v1.__Y < v2.__Y)
    {
      return true;
    }
  if(v1.__Y > v2.__Y)
    {
      return false;
    }
  if(v1.__Z < v2.__Z)
    {
      return true;
    }
  if(v1.__Z > v2.__Z)
    {
      return false;
    }
  return v1.__W < v2.__W;
}

Vector4 abs( const Vector4& v ) {
  return Vector4(fabs(v.__X),fabs(v.__Y),
		 fabs(v.__Z),fabs(v.__W));
}

Vector4 direction( const Vector4& v ) {
  return v / norm(v);
}

real_t dot( const Vector4& v1, const Vector4& v2 ) {
  return
    v1.__X * v2.__X + v1.__Y * v2.__Y +
    v1.__Z * v2.__Z + v1.__W * v2.__W;
}

real_t operator*( const Vector4& v1, const Vector4& v2 ) {
  return
    v1.__X * v2.__X + v1.__Y * v2.__Y +
    v1.__Z * v2.__Z + v1.__W * v2.__W;
}

Vector4 Max( const Vector4& v1, const Vector4& v2 ) {
  return Vector4(max(v1.__X,v2.__X),max(v1.__Y,v2.__Y),
		 max(v1.__Z,v2.__Z),max(v1.__W,v2.__W));
}

Vector4 Min( const Vector4& v1, const Vector4& v2 ) {
  return Vector4(min(v1.__X,v2.__X),min(v1.__Y,v2.__Y),
		 min(v1.__Z,v2.__Z),min(v1.__W,v2.__W));
}

real_t norm( const Vector4& v ) {
  return sqrt(normSquared(v));
}

real_t normL1( const Vector4& v ) {
  return sum(abs(v));
}

real_t normLinf( const Vector4& v ) {
  return *(abs(v).getMax());
}

real_t normSquared( const Vector4& v ) {
  return dot(v,v);
}

real_t sum( const Vector4& v ) {
  return v.__X + v.__Y + v.__Z + v.__W;
}

/*  --------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

/*  --------------------------------------------------------------------- */

