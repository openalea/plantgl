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




#ifndef __geom_matrix_h__
#define __geom_matrix_h__

/*! 
	\file util_matrix.h
    \brief Definition of Matrix2, Matrix3, Matrix4.
*/

#include "util_vector.h"

TOOLS_BEGIN_NAMESPACE

/*  --------------------------------------------------------------------- */

class Matrix2;
class Matrix3;
class Matrix4;

/**
   \class Matrix2
   \brief A 2x2 matrix stored in row-major order.

*/


class GEOM_API Matrix2
{

  friend class Matrix3;

public:

  /// The identity Matrix2.
  static const Matrix2 IDENTITY;

  /// Constructs a Matrix2 with the 2 row vectors \e c0 and \e c1.
  explicit Matrix2( const real_t& m0 = 1, const real_t& m1 = 0,
                    const real_t& m2 = 0, const real_t& m3 = 1 );

  /// Constructs a Matrix2 with the 4 values array \e matrix.
  Matrix2( const real_t * matrix );

  /** Constructs a Matrix2 with the 2 column vectors \e v0 and \e v1.
      \warning
      - \e v0 must be valid;
      - \e v1 must be valid. */
  Matrix2( const Vector2& v0, const Vector2& v1 );

  /// Destructor
  ~Matrix2();

  /// Returns a const pointer to the datas of \e self.
  const real_t * getData( ) const ;

  /// Returns a pointer to the datas of \e self.
  real_t * getData( ) ;

  /// Returns whether \e self is equal to \e v.
  bool operator==( const Matrix2& m ) const;

  /// Returns whether \e self is not equal to \e v.
  bool operator!=( const Matrix2& m ) const;

  /** Returns a const reference to the value at the \e i-th row
      and \e j-th column.
      \warning
      - \e i must belong to the range [0,2[. */
  const real_t& operator()( uchar_t i, uchar_t j ) const;

  /** Returns a reference to the value at the \e i-th row
      and \e j-th column.
      \warning
      - \e i must belong to the range [0,2[. */
  real_t& operator()( uchar_t i, uchar_t j );

  /// Sets \e self to result of the addition of itself and \e m.
  Matrix2& operator+=( const Matrix2& m );

  /// Sets \e self to result of the substraction of itself and \e m.
  Matrix2& operator-=( const Matrix2& m );

  /** Sets \e self to result of the multiplication of itself by
      the scalar \e s. */
  Matrix2& operator*=( const real_t& s );

  /** Sets \e self to result of the division of itself by the scalar \e s.
      \warning
      - \e s must be different from 0. */
  Matrix2& operator/=( const real_t& s );

  /// Returns the opposite Matrix2 of \e self.
  Matrix2 operator-( ) const;

  /** Returns the \e i-th column.
      \warning
      - \e i must belong to the range [0,1]. */
  Vector2 getColumn( uchar_t i ) const ;

  /// Returns the diagonal of \e self.
  Vector2 getDiagonal( ) const;

  /** Returns the \e i-th row.
      \warning
      - \e i must belong to the range [0,1]. */
  Vector2 getRow( uchar_t i ) const;

  /// Returns whether \e self is orthogonal.
  bool isOrthogonal( ) const;

  /// Returns whether \e self is singular.
  bool isSingular( ) const ;

  /// Returns whether \e self is valid.
  bool isValid( ) const;

  /// Sets \e self with the values \e m0, \e m1, \e m2 and \e m3.
  void set( const real_t& m0, const real_t& m1,
            const real_t& m2, const real_t& m3 );

  /// Returns the result of the addition of \e this and \e m.
  Matrix2 operator+( const Matrix2& m) const;

  /// Returns the result of the substraction of \e this and \e m.
  Matrix2 operator-( const Matrix2& m ) const;

  /// Returns the result of the multiplication of \e this by \e m.
  Matrix2 operator*( const Matrix2& m ) const;

  /// Returns the result of the multiplication of \e m by the vector \e v.
  friend GEOM_API Vector2 operator*( const Matrix2& m, const Vector2& v );

  /// Returns the result of the multiplication of \e m by the scalar \e s.
  friend GEOM_API Matrix2 operator*( const Matrix2& m, const real_t& s );

  /// Returns the result of the division of \e m by the scalar \e s.
  friend GEOM_API Matrix2 operator/( const Matrix2& m, const real_t& s ) ;

  /// Returns the adjoint matrix of \e m.
  friend GEOM_API Matrix2 adjoint( const Matrix2& m );

  /// Returns the determinant of \e m.
  friend GEOM_API real_t det( const Matrix2& m );

  /// Returns the det and the inverse of \e m.
  friend GEOM_API Matrix2 inverse( const Matrix2& m ) ;

  /// Returns the trace of \e m.
  friend GEOM_API real_t trace( const Matrix2& m );

  /// Returns the transpose of \e m.
  friend GEOM_API Matrix2 transpose( const Matrix2& m );

  /// Prints \e m to the output stream \e stream.
  friend GEOM_API std::ostream& operator<<( std::ostream& stream, const Matrix2& m );

protected:

  /// The matrix values.
  real_t __M[4];

}; // Matrix2

GEOM_API Vector2 operator*( const Matrix2& m, const Vector2& v );
GEOM_API Matrix2 operator*( const Matrix2& m, const real_t& s );
GEOM_API Matrix2 operator/( const Matrix2& m, const real_t& s ) ;
GEOM_API Matrix2 adjoint( const Matrix2& m );
GEOM_API real_t det( const Matrix2& m );
GEOM_API Matrix2 inverse( const Matrix2& m ) ;
GEOM_API real_t trace( const Matrix2& m );
GEOM_API Matrix2 transpose( const Matrix2& m );
GEOM_API std::ostream& operator<<( std::ostream& stream, const Matrix2& m );


/*  --------------------------------------------------------------------- */


/**
   \class Matrix3
   \brief A 3x3 matrix stored in row-major order.
*/

class Matrix4;

class GEOM_API Matrix3
{

  friend class Matrix4;

public:

  /// The identity Matrix3.
  static const Matrix3 IDENTITY;

  /** Constructs a Matrix3 with the values \e m0, \e m1, \e m2, \e m3, \e m4,
      \e m5,\e m6, \e m7, \e m8 and \e m9. */
  explicit Matrix3( const real_t& m0 = 1, const real_t& m1 = 0,
                    const real_t& m2 = 0, const real_t& m3 = 0,
                    const real_t& m4 = 1, const real_t& m5 = 0,
                    const real_t& m6 = 0, const real_t& m7 = 0,
                    const real_t& m8 = 1);

  /// Constructs a Matrix3 with the 9 values array \e matrix.
  Matrix3( const real_t * matrix );

  /// Constructs a Matrix3 with the 3 column vectors \e v0, \e v1 and \e v2.
  Matrix3( const Vector3& v0,
           const Vector3& v1,
           const Vector3& v2 );

  /** Constructs a Matrix3 with the Matrix2 \e m.
      \pre
      - \e m must be valid.
      \post
      - \e self is valid. */
  Matrix3( const Matrix2& m );

  /** Constructs a Matrix3 with the Matrix4 \e m.
      \pre
      - \e m must be valid.
      \post
      - \e self is valid. */
  Matrix3( const Matrix4& m );

  /// Destructor
  ~Matrix3();

  /// Returns whether \e self is equal to \e v.
  bool operator==( const Matrix3& m ) const;

  /// Returns whether \e self is not equal to \e v.
  bool operator!=( const Matrix3& m ) const;

  /** Returns a const reference to the value at the \e i-th row
      and \e j-th column.
      \warning
      - \e i must belong to the range [0,3[. */
  const real_t& operator()( uchar_t i, uchar_t j ) const;

  /** Returns a reference to the value at the \e i-th row
      and \e j-th column.
      \warning
      - \e i must belong to the range [0,3[. */
  real_t& operator()( uchar_t i, uchar_t j );

  /// Sets \e self to result of the addition of itself and \e m.
  Matrix3& operator+=( const Matrix3& m );

  /// Sets \e self to result of the substraction of itself and \e m.
  Matrix3& operator-=( const Matrix3& m );

  /** Sets \e self to result of the multiplication of itself and \e m. */
  Matrix3& operator*=( const Matrix3& m );

  /** Sets \e self to result of the multiplication of itself by
      the scalar \e s. */
  Matrix3& operator*=( const real_t& s );

  /** Sets \e self to result of the division of itself by the scalar \e s.
      \warning
      - \e s must be different from 0. */
  Matrix3& operator/=( const real_t& s ) ;

  /// Returns the opposite Matrix3 of \e self.
  Matrix3 operator-( ) const;

  /// Returns the result of the addition of \e this and \e m.
  Matrix3 operator+( const Matrix3& m) const;

  /// Returns the result of the substraction of \e this and \e m.
  Matrix3 operator-( const Matrix3& m ) const;

  /// Returns the result of the multiplication of \e this by \e m.
  Matrix3 operator*( const Matrix3& m ) const;

  /// Returns the adjoint matrix of \e this.
  Matrix3 adjoint() const;

  /// Returns the det of \e m.
  real_t det() const;

  /// Returns the inverse of \e this.
  Matrix3 inverse( ) const;

  /// Returns the transpose of \e this.
  Matrix3 transpose( ) const ;

  /// Returns the result of the multiplication of \e m by the vector \e v.
  friend GEOM_API Vector3 operator*( const Matrix3& m, const Vector3& v );

  /// Returns the result of the multiplication of \e m by the scalar \e s.
  friend GEOM_API Matrix3 operator*( const Matrix3& m, const real_t& s ) ;

  /// Returns the result of the division of \e m by the scalar \e s.
  friend GEOM_API Matrix3 operator/( const Matrix3& m, const real_t& s );

  /// Returns the adjoint matrix of \e m.
  friend GEOM_API Matrix3 adjoint( const Matrix3& m );

  /// Returns the det of \e m.
  friend GEOM_API real_t det( const Matrix3& m );

  /// Returns the det and the inverse of \e m.
  friend GEOM_API Matrix3 inverse( const Matrix3& m );

  /// Returns the transpose of \e m.
  friend GEOM_API Matrix3 transpose( const Matrix3& m ) ;

  /// Prints \e m to the output stream \e stream.
  friend GEOM_API std::ostream& operator<<( std::ostream& stream, const Matrix3& m );


  /// Returns a const pointer to the datas of \e self.
  const real_t * getData( ) const;

  /// Returns a pointer to the datas of \e self.
  real_t * getData( ) ;

  /** Returns the \e i-th column.
      \warning
      - \e i must belong to the range [0,2]. */
  Vector3 getColumn( uchar_t i ) const;

  /// Returns the diagonal of \e self.
  Vector3 getDiagonal( ) const ;

  /** Returns the \e i-th row.
      \warning
      - \e i must belong to the range [0,1]. */
  Vector3 getRow( uchar_t i ) const;


  /// Returns whether \e self is orthogonal.
  bool isOrthogonal( ) const;

  /// Returns whether \e self is singular.
  bool isSingular( ) const;

  /// Returns whether \e self is valid.
  bool isValid( ) const;

  /** Sets \e self with the values \e m0, \e m1, \e m2, \e m3, \e m4, \e m5,
      \e m6, \e m7, \e m8 and \e m9. */
  void set( const real_t& m0, const real_t& m1, const real_t& m2,
            const real_t& m3, const real_t& m4, const real_t& m5,
            const real_t& m6, const real_t& m7, const real_t& m8 );

  /** Sets the \e i-th of \e self column with the vector \e v.
      \warning
      - \e i must belong to the range [0,2];
      - \e v must be valid. */
  void setColumn( uchar_t i, const Vector3& v );

  /// Returns the trace of \e m.
  friend GEOM_API real_t trace( const Matrix3& m ) ;

  /** Computes a decomposition of the matrix into the product of 3 matrices
      Rz(a)Ry(b)Rx(c) in this order,
      where each matrix Ru(i) is a rotation about an axis u by angle i.
      \warning the decomposition is not unique.
      \warning angles are in radians. 
      \return a vector made of the 3 euler angles a, b, c in this order. */
  Vector3 eulerAnglesZYX() const;

  /** Computes a decomposition of the matrix into the product of 3 matrices
      Rx(a)Ry(b)Rz(c) in this order,
      where each matrix Ru(i) is a rotation about an axis u by angle i.
      \warning the decomposition is not unique.
      \warning angles are in radians.
      \return a vector made of the 3 euler angles a, b, c in this order. */
  Vector3 eulerAnglesXYZ() const;

  /** Returns the scaling matrix corresponding to a scaling with factor \e v.
      \pre
      - \e s must be valid. */
  static Matrix3 scaling( const Vector3& s );

  /** Returns the euler rotation matrix R(angle) where
      R( angle=(az,ay,ax) ) is the product of 3 matrices Rz(az)*Ry(ay)*Rx(ax).
      The object is rotated around the X axis, then around the Y axis,
      and then around the Z axis.
      \warning angles must be in radians.
  */
  static Matrix3 eulerRotationZYX( const Vector3& angle );

  /** Returns the euler rotation matrix R(angle) where
      R( angle=(ax,ay,az) ) is the product of 3 matrices Rx(ax)*Ry(ay)*Rz(az).
      The object is rotated around the Z axis, then around the Y axis,
      and then around the X axis.
      \warning angles must be in radians.
  */
  static Matrix3 eulerRotationXYZ( const Vector3& angle );

  /** Returns the rotation matrix corresponding to a axis rotation with axis \e v.
      and angle \e angle. 
      \warning angle must be in radians.
  */
  static Matrix3 axisRotation( const Vector3& axis, const real_t& angle );

protected:

  /// The matrix values.
  real_t __M[9];

}; // Matrix3

GEOM_API Vector3 operator*( const Matrix3& m, const Vector3& v );
GEOM_API Matrix3 operator*( const Matrix3& m, const real_t& s ) ;
GEOM_API Matrix3 operator/( const Matrix3& m, const real_t& s );
GEOM_API Matrix3 adjoint( const Matrix3& m );
GEOM_API real_t det( const Matrix3& m );
GEOM_API Matrix3 inverse( const Matrix3& m );
GEOM_API Matrix3 transpose( const Matrix3& m ) ;
GEOM_API std::ostream& operator<<( std::ostream& stream, const Matrix3& m );
GEOM_API real_t trace( const Matrix3& m ) ;

/*  --------------------------------------------------------------------- */


/*!

  \class Matrix4

  \brief A 4x4 matrix stored in row-major order.

*/

class GEOM_API Matrix4
{

public:

  friend class Matrix3;

  /// The identity Matrix4.
  static const Matrix4 IDENTITY;

  /** Constructs a Matrix4 with the values \e m0, \e m1, \e m2, \e m3,
      \e m4, \e m5,\e m6, \e m7, \e m8, \e m9, \e m10, \e m11, \e m12,
      \e m13, \e m14 and \e m15. */
  explicit Matrix4( const real_t& m0 = 1, const real_t& m1 = 0,
                    const real_t& m2 = 0, const real_t& m3 = 0,
                    const real_t& m4 = 0, const real_t& m5 = 1,
                    const real_t& m6 = 0, const real_t& m7 = 0,
                    const real_t& m8 = 0, const real_t& m9 = 0,
                    const real_t& m10 = 1, const real_t& m11 = 0,
                    const real_t& m12 = 0, const real_t& m13 = 0,
                    const real_t& m14 = 0, const real_t& m15 = 1) ;

  /// Constructs a Matrix4 with the 16 values array \e matrix.
  Matrix4( const real_t * matrix );

  /** Constructs a Matrix3 with the 4 column vectors \e v0, \e v1, \e v2
      and \e v3. */
  Matrix4( const Vector4& v0,
           const Vector4& v1,
           const Vector4& v2,
           const Vector4& v3 );

  /** Constructs a Matrix4 with the Matrix3 \e m.
      \pre
      - \e m must be valid.
      \post
      - \e self is valid. */
  Matrix4( const Matrix3& m );

  /// Destructor.
  ~Matrix4();

  /// Returns whether \e self is equal to \e v.
  bool operator==( const Matrix4& m ) const;

  /// Returns whether \e self is not equal to \e v.
  bool operator!=( const Matrix4& m ) const;

  /** Returns a const reference to the value at the \e i-th row
      and \e j-th column.
      \warning
      - \e i must belong to the range [0,4[. */
  const real_t& operator()( uchar_t i, uchar_t j ) const;

  /** Returns a reference to the value at the \e i-th row
      and \e j-th column.
      \warning
      - \e i must belong to the range [0,4[. */
  real_t& operator()( uchar_t i, uchar_t j );

  /// Sets \e self to result of the addition of itself and \e m.
  Matrix4& operator+=( const Matrix4& m );

  /// Sets \e self to result of the substraction of itself and \e m.
  Matrix4& operator-=( const Matrix4& m );

 /// Sets \e self to result of the multiplication of itself and \e m.
  Matrix4& operator*=( const Matrix4& m );

  /** Sets \e self to result of the multiplication of itself by
      the scalar \e s. */
  Matrix4& operator*=( const real_t& s );

  /** Sets \e self to result of the division of itself by the scalar \e s.
      \warning
      - \e s must be different from 0. */
  Matrix4& operator/=( const real_t& s );

  /// Returns the opposite Matrix4 of \e self.
  Matrix4 operator-( ) const ;

  /// Returns the result of the addition of \e this and \e m.
  Matrix4 operator+( const Matrix4& m) const;

  /// Returns the result of the substraction of \e this and \e m.
  Matrix4 operator-( const Matrix4& m ) const;

  /// Returns the result of the multiplication of \e this by \e m.
  Matrix4 operator*( const Matrix4& m ) const;

  /// Returns the result of the multiplication of \e m  by the vector \e v.
  friend GEOM_API Vector4 operator*( const Matrix4& m, const Vector4& v );

  /// Returns the result of the multiplication of \e m  by the point \e v.
  friend GEOM_API Vector3 operator*( const Matrix4& m, const Vector3& v );

  /// Returns the result of the multiplication of \e m by the scalar \e s.
  friend GEOM_API Matrix4 operator*( const Matrix4& m, const real_t& s );

  /// Returns the result of the division of \e m by the scalar \e s.
  friend GEOM_API Matrix4 operator/( const Matrix4& m, const real_t& s );

  /** Returns the adjoint matrix of \e m.
      \pre
      - \e m must be valid. */
  friend GEOM_API Matrix4 adjoint( const Matrix4& m );

  /** Returns the det of \e m.
      \pre
      - \e m must be valid. */
  friend GEOM_API real_t det( const Matrix4& m ) ;

  /// Prints \e m to the output stream \e stream.
  friend GEOM_API std::ostream& operator<<( std::ostream& stream, const Matrix4& m );

  /** Returns a const pointer to the datas of \e self.
      \pre
      - \e self must be valid. */
  const real_t * getData( ) const;

  /** Returns a pointer to the datas of \e self.
      \pre
      - \e self must be valid. */
  real_t * getData( ) ;

  /** Returns the \e i-th column.
      \warning
      - \e i must belong to the range [0,3]. */
  Vector4 getColumn( uchar_t i ) const;

  /** Returns the diagonal of \e self.
      \pre
      - \e self must be valid. */
  Vector4 getDiagonal( ) const;

  /** Returns the \e i-th row.
      \warning
      - \e i must belong to the range [0,3]. */
  Vector4 getRow( uchar_t i ) const;

  /** Returns the det and the inverse of \e m.
      \pre
      - \e m must be valid. */
  friend GEOM_API Matrix4 inverse( const Matrix4& m );

  /** Returns whether \e self is orthogonal.
      \pre
      - \e self must be valid. */
  bool isOrthogonal( ) const;

  /** Returns whether \e self is singular.
      \pre
      - \e self must be valid. */
  bool isSingular( ) const ;

  /// Returns whether \e self is valid.
  bool isValid( ) const;

  /** Sets \e self with the values \e m0, \e m1, \e m2, \e m3, \e m4, \e m5,
      \e m6, \e m7, \e m8, \e m9, \e m10, \e m11, \e m12, \e m13, \e m14 and
      \e m15. */
  void set( const real_t& m0, const real_t& m1, const real_t& m2, const real_t& m3,
            const real_t& m4, const real_t& m5, const real_t& m6, const real_t& m7,
            const real_t& m8, const real_t& m9, const real_t& m10, const real_t& m11,
            const real_t& m12, const real_t& m13, const real_t& m14, const real_t& m15 );

  /** Returns the trace of \e m.
      \pre
      - \e m must be valid. */
  friend GEOM_API real_t trace( const Matrix4& m );

  /** Returns the translation matrix corresponding to the translation of
      vector \e v.
      \pre
      - \e v must be valid. */
  static Matrix4 translation( const Vector3& v );

  /** Returns the transpose of \e m.
      \pre
      - \e m must be valid. */
  friend GEOM_API Matrix4 transpose( const Matrix4& m );

  /** Set *this= R(angle) * S(scale) + T(translate) where
      R( angle=(az,ay,ax) ) is the product of 3 matrices Rz(az)Ry(ay)Rx(ax).
      The object is rotated around the X axis, then around the Y axis,
      and then around the Z axis.
      \warning angles must be in radians.
  */
  void setTransformation( const Vector3& scale,
                          const Vector3& angle,
                          const Vector3& translate );

  /** Computes a decomposition of the matrix into
      R(rotate) * S(scale) + T(translate) where
      R( rotate=(az,ay,ax) ) is the product of 3 matrices Rz(az)Ry(ay)Rx(ax). 
      \warning angles are in radians.
  */
  void getTransformation( Vector3& scale,
                          Vector3& rotate,
                          Vector3& translate ) const;

  /** Set *this= S(scale) * R(angle) + T(translate) where
      R( angle=(az,ay,ax) ) is the product of 3 matrices Rz(az)Ry(ay)Rx(ax).
      The object is rotated around the X axis, then around the Y axis,
      and then around the Z axis.
      \warning angles must be in radians.
  */
  void setTransformation2( const Vector3& scale,
                          const Vector3& angle,
                          const Vector3& translate );

  /** Computes a decomposition of the matrix into
      S(scale) * R(rotate) + T(translate) where
      R( rotate=(az,ay,ax) ) is the product of 3 matrices Rz(az)Ry(ay)Rx(ax). 
      \warning angles are in radians.
  */
  void getTransformation2( Vector3& scale,
                          Vector3& rotate,
                          Vector3& translate ) const;

  /** Computes a decomposition of the matrix into
      R(rotate) * S(scale) + T(translate) where
      R( rotate=(az,ay,ax) ) is the product of 3 matrices Rz(az)Ry(ay)Rx(ax). 
      \warning angles are in radians.
  */
  void getTransformationB( Vector3& scale,
                          Vector3& rotate,
                          Vector3& translate ) const;

protected:

  /// The matrix values.
  real_t __M[16];

}; // Matrix4

GEOM_API Vector4 operator*( const Matrix4& m, const Vector4& v );
GEOM_API Vector3 operator*( const Matrix4& m, const Vector3& v );
GEOM_API Matrix4 operator*( const Matrix4& m, const real_t& s );
GEOM_API Matrix4 operator/( const Matrix4& m, const real_t& s );
GEOM_API Matrix4 adjoint( const Matrix4& m );
GEOM_API real_t det( const Matrix4& m ) ;
GEOM_API std::ostream& operator<<( std::ostream& stream, const Matrix4& m );
GEOM_API Matrix4 inverse( const Matrix4& m );
GEOM_API real_t trace( const Matrix4& m );
GEOM_API Matrix4 transpose( const Matrix4& m );

/*  --------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

// __geom_matrix_h__
#endif
