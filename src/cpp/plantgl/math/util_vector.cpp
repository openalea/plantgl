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





#include "util_vector.h"
#include "util_math.h"
#include "plantgl/tool/util_assert.h"
#include <iostream>
using namespace std;

PGL_BEGIN_NAMESPACE

/*  --------------------------------------------------------------------- */

#define __X __data[0]
#define __Y __data[1]
#define __Z __data[2]
#define __W __data[3]

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
  return pglfinite(radius) && pglfinite(theta);
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

Vector2 Vector2::operator%( const Vector2& v ) const {
  return Vector2(__X*v.__X, __Y*v.__Y);
}

Vector2 Vector2::cwiseProduct( const Vector2& v ) const {
  return Vector2(__X*v.__X, __Y*v.__Y);
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
  return pglfinite(__X) && pglfinite(__Y);
}

real_t
Vector2::normalize( ) {
  real_t _norm = norm(*this);
  if (_norm > GEOM_TOLERANCE) *this /= _norm;
  return _norm;
}

Vector2 Vector2::normed( ) const {
  return direction(*this);
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
  if (v1.__X < (v2.__X - GEOM_TOLERANCE)) return true;
  if (v1.__X > (v2.__X + GEOM_TOLERANCE)) return false;
  return v1.__Y < (v2.__Y- GEOM_TOLERANCE);
}

bool strictly_eq( const Vector2& v1, const Vector2& v2 ) {
  return normLinf(v1 - v2) ==0;
}

bool strictly_inf(const Vector2& v1, const Vector2& v2)
{
  if (v1.__X < v2.__X) return true;
  if (v1.__X > v2.__X) return false;
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
  real_t n = norm(v);
  if (n < GEOM_EPSILON) return v;
  return v / n;
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
  real_t cosinus = v1*v2; //dot(v1,v2)
  real_t sinus = v1^v2;   // cross(v1,v2)
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
    pglfinite(radius) &&
    pglfinite(theta) &&
    pglfinite(z);
}

/*  --------------------------------------------------------------------- */

real_t spherical_distance(real_t theta1, real_t phi1, real_t theta2, real_t phi2, real_t radius)
{
    // return radius * acos(sin(theta1)sin(theta2) + cos(theta1)cos(theta2)cos(phi2-phi1));

    real_t costheta1 = cos(theta1);
    real_t costheta2 = cos(theta2);

    real_t sintheta1 = sin(theta1);
    real_t sintheta2 = sin(theta2);

    real_t deltaphi = phi2-phi1;
    real_t cosdeltaphi = cos(deltaphi);


    real_t a = (costheta2*sin(deltaphi));
    real_t b = (costheta1*sintheta2 - sintheta1*costheta2*cosdeltaphi);
    real_t c = sqrt(a*a+b*b);
    real_t d = sintheta1 * sintheta2 + costheta1 * costheta2 * cosdeltaphi;

    return radius * atan2(c,d);
}

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
    pglfinite(radius) &&
    pglfinite(theta) &&
    pglfinite(phi);
}

real_t Vector3::Spherical::spherical_distance(real_t theta2, real_t phi2) const {
    return PGL(spherical_distance)(theta,phi, theta2,phi2, radius);
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

Vector3 Vector3::operator%( const Vector3& v ) const {
  return Vector3(__X*v.__X, __Y*v.__Y, __Z*v.__Z);
}

Vector3 Vector3::cwiseProduct( const Vector3& v ) const {
  return Vector3(__X*v.__X, __Y*v.__Y, __Z*v.__Z);
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
  return pglfinite(__X) && pglfinite(__Y) && pglfinite(__Z);
}

real_t
Vector3::normalize( )  {
  real_t _norm = norm(*this);
  if (_norm > GEOM_TOLERANCE) *this /= _norm;
  return _norm;
}

Vector3 Vector3::normed( ) const {
  return direction(*this);
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


Vector3 Vector3::anOrthogonalVector() const
{
      if (fabs(__X) < GEOM_EPSILON) return Vector3(1,0,0);
      else if (fabs(__Y) < GEOM_EPSILON) return Vector3(0,1,0);
      else if (fabs(__Z) < GEOM_EPSILON) return Vector3(0,0,1);
      real_t _norm = norm(*this);
      if (_norm < GEOM_TOLERANCE) return Vector3(0,0,0);;
      return Vector3(-__Y/_norm,__X/_norm,0);
}


Vector3 Vector3::reflect(const Vector3& v) const
{
    return v - 2. * dot(normed(), v) * v;
}

Vector3 Vector3::refract(const Vector3& v, real_t eta) const
{
    Vector3 self = normed();

    real_t k = 1.0 - eta * eta * (1.0 - dot(self, v) * dot(self, v));
    if (k < 0.0) return Vector3(0.0,0.0,0.0);
    else return eta * v - (eta * dot(self, v) + sqrt(k)) * self;
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
  if (v1.__X < (v2.__X - GEOM_TOLERANCE)) return true;
  if (v1.__X > (v2.__X + GEOM_TOLERANCE)) return false;
  if (v1.__Y < (v2.__Y - GEOM_TOLERANCE)) return true;
  if (v1.__Y > (v2.__Y + GEOM_TOLERANCE)) return false;
  return v1.__Z < (v2.__Z - GEOM_TOLERANCE);
}

bool strictly_eq( const Vector3& v1, const Vector3& v2 )  {
  return normLinf(v1 - v2) == 0;
}

bool strictly_inf(const Vector3& v1, const Vector3& v2)
{
  if (v1.__X < v2.__X) return true;
  if (v1.__X > v2.__X) return false;
  if (v1.__Y < v2.__Y) return true;
  if (v1.__Y > v2.__Y) return false;
  return v1.__Z < v2.__Z ;
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
  real_t n = norm(v);
  if (n < GEOM_EPSILON) return v;
  return v / n;
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

real_t radialAnisotropicNorm( const Vector3& v, const Vector3& t, real_t alpha, real_t beta ){
    real_t a = dot(v,t);
    Vector3 b = v - a*t;
    return sqrt(a*a*alpha+beta*normSquared(b));
}

real_t anisotropicNorm( const Vector3& v, const Vector3& factors  ) {
    return sqrt(factors.x()*v.x()*v.x()+factors.y()*v.y()*v.y()+factors.z()*v.z()*v.z());
}

real_t angle( const Vector3& v1 , const Vector3& v2 ) {
  double cosinus = dot( v1,v2 );
  Vector3 vy = cross( v1, v2 );
  double sinus = norm( vy );
  return atan2( sinus, cosinus );}

real_t angle( const Vector3& v1, const Vector3& v2, const Vector3& axis )
{
  double cosinus = dot( v1,v2 );
  Vector3 vy = cross( v1, v2 );
  double sinus = norm( vy );
  if( dot( vy, axis ) < 0 ){
    return atan2( -sinus, cosinus );
  }
  return atan2( sinus, cosinus );
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

Vector4 Vector4::operator%( const Vector4& v ) const {
  return Vector4(__X*v.__X, __Y*v.__Y, __Z*v.__Z, __W*v.__W);
}

Vector4 Vector4::cwiseProduct( const Vector4& v ) const {
  return Vector4(__X*v.__X, __Y*v.__Y, __Z*v.__Z, __W*v.__W);
}


bool Vector4::isNormalized( ) const {
  return fabs(normSquared(*this) - 1) < GEOM_EPSILON;
}

bool Vector4::isOrthogonalTo( const Vector4& v ) const {
  return fabs(dot(*this,v)) < GEOM_EPSILON;
}

bool Vector4::isValid( ) const {
  return
    pglfinite(__X) && pglfinite(__Y) &&
    pglfinite(__Z) && pglfinite(__W);
}

real_t Vector4::normalize( ) {
  real_t _norm = norm(*this);
  if (_norm > GEOM_TOLERANCE) *this /= _norm;
  return _norm;
}

Vector4 Vector4::normed( ) const {
  return direction(*this);
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
  if (v1.__X < (v2.__X - GEOM_TOLERANCE)) return true;
  if (v1.__X > (v2.__X + GEOM_TOLERANCE)) return false;
  if (v1.__Y < (v2.__Y - GEOM_TOLERANCE)) return true;
  if (v1.__Y > (v2.__Y + GEOM_TOLERANCE)) return false;
  if (v1.__Z < (v2.__Z - GEOM_TOLERANCE)) return true;
  if (v1.__Z > (v2.__Z + GEOM_TOLERANCE)) return false;
  return v1.__W < (v2.__W - GEOM_TOLERANCE);
}

bool strictly_eq( const Vector4& v1, const Vector4& v2 )  {
  return normLinf(v1 - v2) == 0;
}

bool strictly_inf(const Vector4& v1, const Vector4& v2)
{
  if (v1.__X < v2.__X) return true;
  if (v1.__X > v2.__X) return false;
  if (v1.__Y < v2.__Y) return true;
  if (v1.__Y > v2.__Y) return false;
  if (v1.__Z < v2.__Z) return true;
  if (v1.__Z > v2.__Z) return false;
  return v1.__W < v2.__W ;
}

Vector4 abs( const Vector4& v ) {
  return Vector4(fabs(v.__X),fabs(v.__Y),
         fabs(v.__Z),fabs(v.__W));
}

Vector4 direction( const Vector4& v ) {
  real_t n = norm(v);
  if (n < GEOM_EPSILON) return v;
  return v / n;
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

PGL_END_NAMESPACE

/*  --------------------------------------------------------------------- */
