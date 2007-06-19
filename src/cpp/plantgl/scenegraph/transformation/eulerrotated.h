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


/*! \file geom_eulerrotated.h
    \brief Definition of the transformation class EulerRotation and the transformed class EulerRotated.
*/


#ifndef __geom_eulerrotated_h__
#define __geom_eulerrotated_h__

/* ----------------------------------------------------------------------- */

#include "orthotransformed.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class EulerRotation
    \brief A Rotation by the 3 Euler angles.

    The object is rotated by \b Roll around the X axis, by \b Elevation around
    the Y axis and then by \b Azimuth around the Z axis.
*/

/* ----------------------------------------------------------------------- */


class SG_API EulerRotation : public OrthonormalBasis3D
{

public:

  /// A structure which helps to build an EulerRotation when parsing.
  struct SG_API Builder : public OrthonormalBasis3D::Builder
    {
    /// A pointer to the \b Azimuth field.
    real_t * Azimuth;

    /// A pointer to the \b Elevation field.
    real_t * Elevation;

    /// A pointer to the \b Roll field.
    real_t * Roll;

    /// Constructor.
    Builder( );

    /// Desructor.
    virtual ~Builder( );

    virtual OrthonormalBasis3DPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;
    };

  /** Constructs a EulerRotation with the angles \e azimuth,
      \e elevation and a \e roll .
  */
  EulerRotation( const real_t& azimuth,
                 const real_t& elevation,
                 const real_t& roll ) ;

  /// Destructor.
  virtual ~EulerRotation( );

  virtual TOOLS(Vector3) extractEulerAngles() const;
protected:

  /// The Azimuth field.
  real_t __azimuth;

  /// The Elevation field.
  real_t __elevation;

  /// The Roll field.
  real_t __roll;
};

/// EulerRotation Pointer
typedef RCPtr<EulerRotation> EulerRotationPtr;

/* ----------------------------------------------------------------------- */

/**
    \class EulerRotated
    \brief A geometry object rotated by the 3 Euler angles.
*/


class SG_API EulerRotated : public OrthoTransformed
{

public:

  /// The default Azimuth field value.
  static const real_t DEFAULT_AZIMUTH;

  /// The default Elevation field value.
  static const real_t DEFAULT_ELEVATION;

  /// The default Roll field value.
  static const real_t DEFAULT_ROLL;


  /// A structure which helps to build a Rotated when parsing.
  struct SG_API Builder : public OrthoTransformed::Builder {

    /// A pointer to the \b Azimuth field.
    real_t * Azimuth;

    /// A pointer to the \b Elevation field.
    real_t * Elevation;

    /// A pointer to the \b Roll field.
    real_t * Roll;

    /// Constructor.
    Builder( );

    /// Destructor
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };


  /// Default Constructor. Build object is invalid.
  EulerRotated();

  /** Constructs a EulerRotated with the angles \e azimuth, \e elevation,
      \e roll and \e geometry.
      \warning
      - \e geometyr must be non null and valid. */
  EulerRotated( const real_t& azimuth,
                const real_t& elevation,
                const real_t& roll,
                const GeometryPtr& geometry );

  /// Destructor
  virtual ~EulerRotated( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns Azimuth value.
  const real_t& getAzimuth( ) const ;

  /// Returns Azimuth field.
  real_t& getAzimuth( ) ;

  /// Returns Elevation value.
  const real_t&  getElevation( ) const;

  /// Returns Elevation field.
  real_t&  getElevation( );

  /// Returns Roll value.
  const real_t& getRoll( ) const ;

  /// Returns Roll field.
  real_t& getRoll( ) ;

  virtual Transformation3DPtr getTransformation( ) const ;

  virtual OrthonormalBasis3DPtr getOrthonormalBasis( ) const ;

  /// Returns whether Azimuth is set to its default value.
  bool isAzimuthToDefault( ) const ;

  /// Returns whether Elevation is set to its default value.
  bool isElevationToDefault( ) const ;

  /// Returns whether Roll is set to its default value.
  bool isRollToDefault( ) const ;

  virtual bool isValid( ) const;

protected:

  /// The Azimuth field.
  real_t __azimuth;

  /// The Elevation field.
  real_t __elevation;

  /// The Roll field.
  real_t __roll;

};

/// EulerRotated Pointer
typedef RCPtr<EulerRotated> EulerRotatedPtr;


/* ----------------------------------------------------------------------- */

// __geom_eulerrotated_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

