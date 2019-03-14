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


/*! \file geom_swung.h
    \brief Definition of the geometry class Swung.
*/


#ifndef __geom_swung_h__
#define __geom_swung_h__

/* ----------------------------------------------------------------------- */

#include "sor.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE
class RealArray;
typedef RCPtr<RealArray> RealArrayPtr;
PGL_END_NAMESPACE

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Curve2DArray;
typedef RCPtr<Curve2DArray> Curve2DArrayPtr;
class ProfileInterpolation;
typedef RCPtr<ProfileInterpolation> ProfileInterpolationPtr;

/* ----------------------------------------------------------------------- */

/**
   \class Swung
   \brief A surface defined by the revolution and interpolation of several 2D profiles along Z axis.
*/

/* ----------------------------------------------------------------------- */

class SG_API Swung : public SOR
{

public:

  /// The \b CCW field default value.
  static const bool DEFAULT_CCW;

  /// The \b DEGREE field default value.
  static const uint_t DEFAULT_DEGREE;

  /// The \b STRIDE field default value.
  static const uint_t DEFAULT_STRIDE;

  /// A structure which helps to build a Swung when parsing.
  struct SG_API Builder : public SOR::Builder
    {
    /// A pointer to the \b CCW field.
    bool * CCW;

    /// A pointer to the \b Profile List field.
    Curve2DArrayPtr * ProfileList;

    /// A pointer to the \b Angle List field. An angle is associated to each Profile.
    RealArrayPtr * AngleList;

    /// A pointer to the \b Degree field.
    uint_t * Degree;

    /// A pointer to the \b Stride field.
    uint_t * Stride;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;
    };

  /// Default Constructor. Build object is invalid.
  Swung();

  /** Constructs a Swung with a \b Profile interpolation object
      \pre
      - \e degree must be in [1,3];
      - \e slices and \e stride must be superior to 2
      \post
      - \e self is valid. */
  Swung( const ProfileInterpolationPtr& profiles,
         uchar_t slices = DEFAULT_SLICES,
         bool ccw = DEFAULT_CCW,
         uint_t degree = DEFAULT_DEGREE,
         uint_t stride = DEFAULT_STRIDE );

  /** Constructs a Swung with a \b Profile List and an
      \b Angle List
      \pre
      - \e degree must be in [1,3];
      - \e slices and \e stride must be superior to 2
      \post
      - \e self is valid. */
  Swung( const Curve2DArrayPtr& profileList,
         const RealArrayPtr& angleList,
         uchar_t slices = DEFAULT_SLICES,
         bool ccw = DEFAULT_CCW,
         uint_t degree = DEFAULT_DEGREE,
         uint_t stride = DEFAULT_STRIDE );

  /// Destructor
  virtual ~Swung( );

  PGL_OBJECT(Swung)

  /// Returns \b CCW value.
  const bool getCCW( ) const;

  /// Returns \b CCW field.
  bool& getCCW( );

  /// Returns \b Profile Interpolation value.
  const ProfileInterpolationPtr& getProfileInterpolation( ) const;

  /// Returns \b Profile Interpolation field.
  ProfileInterpolationPtr& getProfileInterpolation( );

  /// Returns \b Profile List value.
  const Curve2DArrayPtr& getProfileList( ) const;

  /// Returns \b Profile List field.
  Curve2DArrayPtr& getProfileList( );

  /// Returns \b Angle List value.
  const RealArrayPtr& getAngleList( ) const;

  /// Returns \b Angle List field.
  RealArrayPtr& getAngleList( );

  /// Returns \b Degree value.
  const uint_t& getDegree() const;

  /// Returns \b Degree field.
  uint_t& getDegree();

  const uint_t& getStride() const;

  /// Returns \b Degree field.
  uint_t& getStride();

  /// Returns whether \b CCW is set to its default value.
  bool isCCWToDefault( ) const;

  /// Returns whether \b Degree is set to its default value.
  bool isDegreeToDefault( ) const;

  /// Returns whether \b Stride is set to its default value.
  bool isStrideToDefault( ) const;

  virtual bool isValid( ) const;

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

protected:

  /// The \b Profile List field.
  ProfileInterpolationPtr __profiles;

  /// The \b CCW field.
  bool __ccw;

  /// The \b Degree field.
  uint_t __degree;

  /// The \b Stride field.
  uint_t __stride;

}; // Swung

/// Swung Pointer
typedef RCPtr<Swung> SwungPtr;


/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
 // __geom_swung_h__

