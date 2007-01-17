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





#ifndef __geom_vector_h__
#define __geom_vector_h__

/*! 
	\file util_vector.h
    \brief Definition of Vector2, Vector3, Vector4.
*/

#include "util_tuple.h"

TOOLS_BEGIN_NAMESPACE

/*  --------------------------------------------------------------------- */

#ifdef GEOM_DLL
template class GEOM_API Tuple2<real_t>;
#endif

/**
   \class Vector2
   \brief A 2D vector represented by its cartesian coordinates \c x and
   \c y.
*/

class GEOM_API Vector2 : public Tuple2<real_t>
{

 public:

  /// The origin vector.
  static const Vector2 ORIGIN;

  /// The unit vector along the \c x-axis.
  static const Vector2 OX;

  /// The unit vector along the \c y-axis.
  static const Vector2 OY;


  /**
     \class Polar
     \brief A doublet representing polar coordinates \c radius and \c theta.
  */
  struct GEOM_API Polar {

    /// The \c radius coordinate
    real_t radius;

    /// The \c theta coordinate
    real_t theta;

    /** Constructs a Polar with the \e radius and \e theta coordinates.
        \post
        - \e self is valid. */
    Polar( const real_t& radius, const real_t& theta ) ;

    /** Constructs a Polar with the Vector2 \e v.
        \post
        - \e self is valid. */
    Polar( const Vector2& v ) ;

    /// Returns whether \e self is valid.
    bool isValid( ) const;

  };


  /** Constructs a Vector2 with the \e x and \e y coordinates.
      \post
      - \e self is valid. */
  explicit Vector2( const real_t& x = 0, const real_t& y = 0 );

  /** Constructs a Vector2 with the 2D array \e v2.
      \post
      - \e self is valid. */
  inline Vector2( const real_t * v2 ) ;

  /** Constructs a Vector2 with the Polar \e p.
      \pre
      - \e p must be valid.
      \post
      - \e self is valid. */
  Vector2( const Polar& p );

  /// Desructor.
  virtual ~Vector2( );

  /// set new values.
  void set(const real_t& x, const real_t& y);

  /// set new values.
  void set(const real_t * v2);

  /// set new value.
  void set(const Polar& v);

  /// set new value.
  void set(const Vector2& v);

  /// Returns a const reference to the \c x coordinate of \e self.
  const real_t& x( ) const;

  /// Returns a reference to the \c x coordinate of \e self.
  real_t& x( );

  /// Returns a const reference to the \c y coordinate of \e self.
  const real_t& y( ) const;

  /// Returns a reference to the \c y coordinate of \e self.
  real_t& y( );

  /// Returns the index of the maximum absolute coordinate
  int getMaxAbsCoord() const;

  /// Returns the index of the minimum absolute coordinate
  int getMinAbsCoord() const;

  /// Returns whether \e self is equal to \e v.
  bool operator==( const Vector2& v ) const;

  /// Returns whether \e self is equal to \e v.
  bool operator!=( const Vector2& v ) const ;

  /// Sets \e self to result of the addition of itself and \e v.
  Vector2& operator+=( const Vector2& v ) ;

  /// Sets \e self to result of the substraction of itself and \e v.
  Vector2& operator-=( const Vector2& v );

  /** Sets \e self to result of the multiplication of itself by the
      scalar \e s. */
  Vector2& operator*=( const real_t& s );

  /** Sets \e self to result of the division of itself by the scalar \e s.
      \warning
      - \e s must be different from 0. */
  Vector2& operator/=( const real_t& s ) ;

  /// Returns the opposite Vector2 of \e self.
  Vector2 operator-( ) const;

  /// Returns the result of the addition of \e this and \e v.
  Vector2 operator+( const Vector2& v) const;

  /// Returns the result of the substraction of \e this and \e v.
  Vector2 operator-( const Vector2& v ) const;

  /// Normalizes \e self and returns the norm before.
  real_t normalize( ) ;

  /// Returns whether \e self is normalized.
  bool isNormalized( ) const;

  /// Returns whether \e self is orthogonal to \e v.
  bool isOrthogonalTo( const Vector2& v ) const;

  /// Returns whether \e self is valid.
  bool isValid( ) const;

  /// Returns a vector with the absolute values of \e v.
  friend GEOM_API Vector2 abs( const Vector2& v ) ;

  /** Returns the direction of \e v.
      The resulting Vector2 is normalized. */
  friend GEOM_API Vector2 direction( const Vector2& v );

  /// Returns the norm of \e v.
  friend GEOM_API real_t norm( const Vector2& v );

  /// Returns the L1 (Manhattan) norm of \e v.
  friend GEOM_API real_t normL1( const Vector2& v );

  /// Returns the L-infinite norm of \e v.
  friend GEOM_API real_t normLinf( const Vector2& v );

  /// Returns the square of the norm of \e v.
  friend GEOM_API real_t normSquared( const Vector2& v );

  /// Returns the sum of all the values of \e self.
  friend GEOM_API real_t sum( const Vector2& v );

  /// Returns the result of the multiplication of \e v by the scalar \e s.
  friend GEOM_API Vector2 operator*( const Vector2& v, const real_t& s );
  friend GEOM_API Vector2 operator*( const real_t& s, const Vector2& v );

  /** Returns the result of the division of \e v by the scalar \e s.
      \warning
      - \e s must be different from 0. */
  friend GEOM_API Vector2 operator/( const Vector2& v, const real_t& s );

  // Return the result of the cross product of \e this and \e v.
  friend GEOM_API real_t cross( const Vector2& v1, const Vector2& v2);
  friend GEOM_API real_t operator^( const Vector2& v1, const Vector2& v2);

  /// Returns the dot product between \e v1 and \e v2.
  friend GEOM_API real_t dot( const Vector2& v1, const Vector2& v2 );
  friend GEOM_API real_t operator*( const Vector2& v1, const Vector2& v2 );

  /// Returns the vector with the maximum values between \e v1 and \e v2.
  friend GEOM_API Vector2 Max( const Vector2& v1, const Vector2& v2 );

  /// Returns the vector with the minimum values between \e v1 and \e v2.
  friend GEOM_API Vector2 Min( const Vector2& v1, const Vector2& v2 );

  /// Returns the value of the angle ni radian between \e v1 and \e v2.
  friend GEOM_API real_t angle( const Vector2& v1, const Vector2& v2 );

  /// Prints \e v to the output stream \e stream.
  friend GEOM_API std::ostream& operator<<( std::ostream& stream, const Vector2& v );

  /// Defines an order relation in order to use this class in a sorted container
  friend GEOM_API bool operator<(const Vector2& v1, const Vector2& v2);

}; // Vector2

GEOM_API Vector2 abs( const Vector2& v ) ;
GEOM_API Vector2 direction( const Vector2& v );
GEOM_API real_t norm( const Vector2& v );
GEOM_API real_t normL1( const Vector2& v );
GEOM_API real_t normLinf( const Vector2& v );
GEOM_API real_t normSquared( const Vector2& v );
GEOM_API real_t sum( const Vector2& v );
GEOM_API Vector2 operator*( const Vector2& v, const real_t& s );
GEOM_API Vector2 operator*( const real_t& s, const Vector2& v );
GEOM_API Vector2 operator/( const Vector2& v, const real_t& s );
GEOM_API real_t cross( const Vector2& v1, const Vector2& v2);
GEOM_API real_t operator^( const Vector2& v1, const Vector2& v2);
GEOM_API real_t dot( const Vector2& v1, const Vector2& v2 );
GEOM_API real_t operator*( const Vector2& v1, const Vector2& v2 );
GEOM_API Vector2 Max( const Vector2& v1, const Vector2& v2 );
GEOM_API Vector2 Min( const Vector2& v1, const Vector2& v2 );
GEOM_API real_t angle( const Vector2& v1, const Vector2& v2 );
GEOM_API std::ostream& operator<<( std::ostream& stream, const Vector2& v );
GEOM_API bool operator<(const Vector2& v1, const Vector2& v2);


/*  --------------------------------------------------------------------- */


#ifdef GEOM_DLL
template class GEOM_API Tuple3<real_t>;
#endif

/**
   \class Vector3
   \brief A 3D vector represented by its cartesian coordinates \c x, \c y
   and \c z.
*/

class GEOM_API Vector3 : public Tuple3<real_t>
{

 public:

  /// The origin vector.
  static const Vector3 ORIGIN;

  /// The unit vector along the \c x-axis.
  static const Vector3 OX;

  /// The unit vector along the \c y-axis.
  static const Vector3 OY;

  /// The unit vector along the \c z-axis.
  static const Vector3 OZ;


  /**
     \class Cylindrical
     \brief A triplet representing cylindrical coordinates \c radius,
     \c theta and \c z coordinate.
  */
  struct GEOM_API Cylindrical {

    /// The \c radius coordinate
    real_t radius;

    /// The \c theta coordinate
    real_t theta;

    /// The \c z coordinate
    real_t z;

    /** Constructs a Cylindrical with the \e radius, \e theta and \e z
        coordinates.
        \post
        - \e self is valid. */
    Cylindrical( const real_t& radius, const real_t& theta, const real_t& z );

    /** Constructs a Cylindrical from the Vector3 \e v.
        \post
        - \e self is valid. */
    Cylindrical( const Vector3& v );

    /// Returns whether \e self is valid.
    bool isValid( ) const;

  };


  /**
     \class Spherical
     \brief A triplet representing cylindrical coordinates \c radius,
     \c theta and \c phi coordinate.
  */
  struct GEOM_API Spherical {

    /// The \c radius coordinate
    real_t radius;

    /// The \c theta coordinate
    real_t theta;

    /// The \c phi coordinate
    real_t phi;

    /** Constructs a Spherical with the \e radius, \e theta and \e phi
        coordinates.
        \post
        - \e self is valid. */
    Spherical( const real_t& radius, const real_t& theta, const real_t& phi );

    /** Constructs a Spherical from the Vector3 \e v.
        \post
        - \e self is valid. */
    Spherical( const Vector3& v );

    /// Returns whether \e self is valid.
    bool isValid( ) const;

  };


  /** Constructs a Vector3 with the \e x, \e y and \e z coordinates.
      \post
      - \e self is valid. */
  explicit Vector3( const real_t& x = 0,
                    const real_t& y = 0,
                    const real_t& z = 0 );

  /** Constructs a Vector3 with the 3D array \e v3.
      \post
      - \e self is valid. */
  inline Vector3( const real_t * v3 );

  /** Constructs a Vector3 with the Vector2 \e v and \e z.
      \pre
      - \v must be valid.
      \post
      - \e self is valid. */
  Vector3( const Vector2& v, const real_t& z );

  /** Constructs a Vector3 with the Cylindrical \e c.
      \pre
      - \c must be valid.
      \post
      - \e self is valid. */
  Vector3( const Cylindrical& c );

  /** Constructs a Vector3 with the Spherical \e s.
      \pre
      - \s must be valid.
      \post
      - \e self is valid. */
  Vector3( const Spherical& s ) ;

  /// Destructor
  virtual ~Vector3( );

  /// set new values.
  void set(const real_t& x, const real_t& y, const real_t& z);

  /// set new values.
  void set(const real_t * v3);

  /// set new values.
  void set(const Vector2& v, const real_t& z);

  /// set new value.
  void set(const Cylindrical& c);

  /// set new value.
  void set(const Spherical& c);

  /// set new value.
  void set(const Vector3& v);

  /// Returns a const reference to the \c x coordinate of \e self.
  const real_t& x( ) const ;

  /// Returns a reference to the \c x coordinate of \e self.
  real_t& x( ) ;

  /// Returns a const reference to the \c y coordinate of \e self.
  const real_t& y( ) const ;

  /// Returns a reference to the \c y coordinate of \e self.
  real_t& y( ) ;

  /// Returns a const reference to the \c z coordinate of \e self.
  const real_t& z( ) const;

  /// Returns a reference to the \c z coordinate of \e self.
  real_t& z( ) ;

  /// Returns the index of the maximum absolute coordinate
  int getMaxAbsCoord() const;

  /// Returns the index of the minimum absolute coordinate
  int getMinAbsCoord() const;

  /// Returns whether \e self is equal to \e v.
  bool operator==( const Vector3& v ) const;

  /// Returns whether \e self is equal to \e v.
  bool operator!=( const Vector3& v ) const;

  /// Sets \e self to result of the addition of itself and \e v.
  Vector3& operator+=( const Vector3& v );

  /// Sets \e self to result of the substraction of itself and \e v.
  Vector3& operator-=( const Vector3& v );

  /** Sets \e self to result of the multiplication of itself by the
      scalar \e s. */
  Vector3& operator*=( const real_t& s );

  /** Sets \e self to result of the division of itself by the scalar \e s.
      \warning
      - \e s must be different from 0. */
  Vector3& operator/=( const real_t& s );

  /// Returns the opposite Vector3 of \e self.
  Vector3 operator-( ) const;

  /// Returns the result of the addition of \e this and \e v.
  Vector3 operator+( const Vector3& v) const ;

  /// Returns the result of the substraction of \e this and \e v.
  Vector3 operator-( const Vector3& v ) const;

  /** Returns the Vector2 corresponding to the projection of \e self.
      The \c x and \c y  coordinates are divided by the \c z coordinate. */
  Vector2 project( ) const ;

  /// Normalizes \e self and returns the norm before.
  real_t normalize( );

  /// Returns whether \e self is normalized.
  bool isNormalized( ) const;

  /// Returns whether \e self is orthogonal to \e v.
  bool isOrthogonalTo( const Vector3& v ) const;

  /// Returns whether \e self is valid.
  bool isValid( ) const ;

  /// Returns the result of the multiplication of \e v by the scalar \e s.
  friend GEOM_API Vector3 operator*( const Vector3& v, const real_t& s );
  friend GEOM_API Vector3 operator*( const real_t& s, const Vector3& v );

  /** Returns the result of the division of \e v by the scalar \e s.
      \warning
      - \e s must be different from 0. */
  friend GEOM_API Vector3 operator/( const Vector3& v, const real_t& s );

  /** Returns the direction of \e v.
      The resulting Vector3 is normalized. */
  friend GEOM_API Vector3 direction( const Vector3& v );

  /// Returns the abs value of \e v.
  friend GEOM_API Vector3 abs( const Vector3& v );

  /// Returns the norm of \e v.
  friend GEOM_API real_t norm( const Vector3& v );

  /// Returns the L1 (Manhattan) norm of \e v.
  friend GEOM_API real_t normL1( const Vector3& v );

  /// Returns the L-infinite norm of \e v.
  friend GEOM_API real_t normLinf( const Vector3& v );

  /// Returns the square of the norm of \e v.
  friend GEOM_API real_t normSquared( const Vector3& v );

  /// Returns the sum of all the values of \e self.
  friend GEOM_API real_t sum( const Vector3& v );

  ///  Returns the result of the cross product between \e v1 and \e v2.
  friend GEOM_API Vector3 cross( const Vector3& v1, const Vector3& v2 );
  friend GEOM_API Vector3 operator^( const Vector3& v1, const Vector3& v2 );

  /// Returns the dot product between \e v1 and \e v2.
  friend GEOM_API real_t dot( const Vector3& v1, const Vector3& v2 );
  friend GEOM_API real_t operator*( const Vector3& v1, const Vector3& v2 );

  /// Returns the vector with the maximum values between \e v1 and \e v2.
  friend GEOM_API Vector3 Max( const Vector3& v1, const Vector3& v2 );

  /// Returns the vector with the minimum values between \e v1 and \e v2.
  friend GEOM_API Vector3 Min( const Vector3& v1, const Vector3& v2 );

  /// Returns the value of the angle between \e v1 and \e v2.
  friend GEOM_API real_t angle( const Vector3& v1, const Vector3& v2 );
  friend GEOM_API real_t angle( const Vector3& v1, const Vector3& v2, const Vector3& axis );

  /// Prints \e v to the output stream \e stream.
  friend GEOM_API std::ostream& operator<<( std::ostream& stream, const Vector3& v );

  /// Defines an order relation in order to use this class in a sorted container
  friend GEOM_API bool operator<(const Vector3& v1, const Vector3& v2);

}; // Vector3

GEOM_API Vector3 operator*( const Vector3& v, const real_t& s );
GEOM_API Vector3 operator*( const real_t& s, const Vector3& v );
GEOM_API Vector3 operator/( const Vector3& v, const real_t& s );
GEOM_API Vector3 direction( const Vector3& v );
GEOM_API Vector3 abs( const Vector3& v );
GEOM_API real_t norm( const Vector3& v );
GEOM_API real_t normL1( const Vector3& v );
GEOM_API real_t normLinf( const Vector3& v );
GEOM_API real_t normSquared( const Vector3& v );
GEOM_API real_t sum( const Vector3& v );
GEOM_API Vector3 cross( const Vector3& v1, const Vector3& v2 );
GEOM_API Vector3 operator^( const Vector3& v1, const Vector3& v2 );
GEOM_API real_t dot( const Vector3& v1, const Vector3& v2 );
GEOM_API real_t operator*( const Vector3& v1, const Vector3& v2 );
GEOM_API Vector3 Max( const Vector3& v1, const Vector3& v2 );
GEOM_API Vector3 Min( const Vector3& v1, const Vector3& v2 );
GEOM_API real_t angle( const Vector3& v1, const Vector3& v2 );
GEOM_API real_t angle( const Vector3& v1, const Vector3& v2, const Vector3& axis );
GEOM_API std::ostream& operator<<( std::ostream& stream, const Vector3& v );
GEOM_API bool operator<(const Vector3& v1, const Vector3& v2);



/*  --------------------------------------------------------------------- */

#ifdef GEOM_DLL
template class GEOM_API Tuple4<real_t>;
#endif

/**
   \class Vector4
   \brief A 4D vector represented by its cartesian coordinates \c x, \c y,
   \e z and \c w.
*/

class GEOM_API Vector4 : public Tuple4<real_t>
{

 public:

  /// The origin vector.
  static const Vector4 ORIGIN;

  /// The unit vector along the \c x-axis.
  static const Vector4 OX;

  /// The unit vector along the \c y-axis.
  static const Vector4 OY;

  /// The unit vector along the \c z-axis.
  static const Vector4 OZ;

  /// The unit vector along the \c w-axis.
  static const Vector4 OW;

  /** Constructs a Vector4 with the \e x, \e y, \e z and \e w coordinates.
      \post
      - \e self is valid. */
  explicit Vector4( const real_t& x = 0, const real_t& y = 0,
                    const real_t& z = 0, const real_t& w = 0 );

  /** Constructs a Vector4 with the 4D array \e v4.
      \post
      - \e self is valid. */
  inline Vector4( const real_t * v4 );

  /** Constructs a Vector4 with a Vector3 \e v and \e w.
      \pre
      - \e v must be valid.
      \post
      - \e self is valid. */
  Vector4( const Vector3& v, const real_t& w );

  /// Destructor
  virtual ~Vector4( );

  /// set new values.
  void set(const real_t& x, const real_t& y, const real_t& z , const real_t& w);

  /// set new values.
  void set(const real_t * v4);

  /// set new values.
  void set(const Vector3& v, const real_t& w);

  /// set new value.
  void set(const Vector4& v);

  /// Returns a const reference to the \c x coordinate of \e self.
  const real_t& x( ) const ;

  /// Returns a reference to the \c x coordinate of \e self.
  real_t& x( );

  /// Returns a const reference to the \c y coordinate of \e self.
  const real_t& y( ) const;

  /// Returns a reference to the \c y coordinate of \e self.
  real_t& y( );

  /// Returns a const reference to the \c z coordinate of \e self.
  const real_t& z( ) const;

  /// Returns a reference to the \c z coordinate of \e self.
  real_t& z( );

  /// Returns a const reference to \c w coordinate of \e self.
  const real_t& w( ) const;

  /// Returns a reference to the \c w coordinate of \e self.
  real_t& w( );

  /// Returns whether \e self is equal to \e v.
  bool operator==( const Vector4& v ) const;

  /// Returns whether \e self is equal to \e v.
  bool operator!=( const Vector4& v ) const ;

  /// Sets \e self to result of the addition of itself and \e v.
  Vector4& operator+=( const Vector4& v );

  /// Sets \e self to result of the substraction of itself and \e v.
  Vector4& operator-=( const Vector4& v );

  /** Sets \e self to result of the multiplication of itself by the
      scalar \e s. */
  Vector4& operator*=( const real_t& s );

  /** Sets \e self to result of the division of itself by the scalar \e s.
      \warning
      - \e s must be different from 0. */
  Vector4& operator/=( const real_t& s );

  /// Returns the opposite Vector4 of \e self.
  Vector4 operator-( ) const ;

  /// Returns the result of the addition of \e this and \e v.
  Vector4 operator+( const Vector4& v) const;

  /// Returns the result of the substraction of \e this and \e v.
  Vector4 operator-( const Vector4& v ) const;

  /// Returns the abs value of \e v.
  friend GEOM_API Vector4 abs( const Vector4& v );

  /** Returns the direction of \e v.
      The resulting Vector4 is normalized. */
  friend GEOM_API Vector4 direction( const Vector4& v );

  /// Returns the dot product between \e v1 and \e v2.
  friend GEOM_API real_t dot( const Vector4& v1, const Vector4& v2 );
  friend GEOM_API real_t operator*( const Vector4& v1, const Vector4& v2 );

  /// Returns whether \e self is normalized.
  bool isNormalized( ) const;

  /// Returns whether \e self is orthogonal to \e v.
  bool isOrthogonalTo( const Vector4& v ) const;

  /// Returns whether \e self is valid.
  bool isValid( ) const ;

  /// Normalizes \e self and returns the norm before.
  real_t normalize( );

  /** Returns the Vector3 corresponding to the projection of \e self.
      The \c x, \c y and \c z  coordinates are divided by the \c w
      coordinate. */
  Vector3 project( ) const;

  /// Returns the result of the multiplication of \e v by the scalar \e s.
  friend GEOM_API Vector4 operator*( const Vector4& v, const real_t& s );
  friend GEOM_API Vector4 operator*( const real_t& s, const Vector4& v );

  /** Returns the result of the division of \e v by the scalar \e s.
      \warning
      - \e s must be different from 0. */
  friend GEOM_API Vector4 operator/( const Vector4& v, const real_t& s );

  /// Returns the vector with the maximum values between \e v1 and \e v2.
  friend GEOM_API Vector4 Max( const Vector4& v1, const Vector4& v2 ) ;

  /// Returns the vector with the minimum values between \e v1 and \e v2.
  friend GEOM_API Vector4 Min( const Vector4& v1, const Vector4& v2 );

  /// Returns the norm of \e v.
  friend GEOM_API real_t norm( const Vector4& v );

  /// Returns the L1 (Manhattan) norm of \e v.
  friend GEOM_API real_t normL1( const Vector4& v );

  /// Returns the L-infinite norm of \e v.
  friend GEOM_API real_t normLinf( const Vector4& v );

  /// Returns the square of the norm of \e v.
  friend GEOM_API real_t normSquared( const Vector4& v ) ;

  /// Returns the sum of all the values of \e self.
  friend GEOM_API real_t sum( const Vector4& v );

    /// Prints \e v to the output stream \e stream.
  friend GEOM_API std::ostream& operator<<( std::ostream& stream, const Vector4& v );

  /// Defines an order relation in order to use this class in a sorted container
  friend GEOM_API bool operator<(const Vector4& v1, const Vector4& v2);

}; // Vector4


GEOM_API Vector4 abs( const Vector4& v );
GEOM_API Vector4 direction( const Vector4& v );
GEOM_API real_t dot( const Vector4& v1, const Vector4& v2 );
GEOM_API real_t operator*( const Vector4& v1, const Vector4& v2 );
GEOM_API Vector4 operator*( const Vector4& v, const real_t& s );
GEOM_API Vector4 operator*( const real_t& s, const Vector4& v );
GEOM_API Vector4 operator/( const Vector4& v, const real_t& s );
GEOM_API Vector4 Max( const Vector4& v1, const Vector4& v2 ) ;
GEOM_API Vector4 Min( const Vector4& v1, const Vector4& v2 );
GEOM_API real_t norm( const Vector4& v );
GEOM_API real_t normL1( const Vector4& v );
GEOM_API real_t normLinf( const Vector4& v );
GEOM_API real_t normSquared( const Vector4& v ) ;
GEOM_API real_t sum( const Vector4& v );
GEOM_API std::ostream& operator<<( std::ostream& stream, const Vector4& v );
GEOM_API bool operator<(const Vector4& v1, const Vector4& v2);


/*  --------------------------------------------------------------------- */
TOOLS_END_NAMESPACE

// __geom_vector_h__
#endif
