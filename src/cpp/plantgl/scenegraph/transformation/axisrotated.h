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


/*! \file geom_axisrotated.h
    \brief Definition of the transformation class AxisRotation and the transformed class AxisRotated.
*/


#ifndef __geom_axisrotated_h__
#define __geom_axisrotated_h__

/* ----------------------------------------------------------------------- */

#include "orthotransformed.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class AxisRotation
    \brief A Rotation by an angle around an arbitrary axis
*/

class SG_API AxisRotation : public OrthonormalBasis3D
{

public:

  /// A structure which helps to build an AxisRotation when parsing.
  struct SG_API Builder : public OrthonormalBasis3D::Builder
    {

    /// A pointer to the \b Axis field.
    Vector3 * Axis;

     /// A pointer to the \b Angle field.
    real_t * Angle;

    /// Constructor.
    Builder( );

    /// Desructor.
    virtual ~Builder( );

    virtual OrthonormalBasis3DPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;
    };

  /** Constructs an AxisRotation
      \pre
      - \e axis have to be normalized
      \post
      - \e self is valid. */
  AxisRotation( const Vector3& axis,
                const real_t& angle ) ;

  /// Destructor.
  virtual ~AxisRotation( );

protected:

  /// The Axis field.
  Vector3 __axis;

  /// The Angle field.
  real_t __angle;
};

/// AxisRotation Pointer
typedef RCPtr<AxisRotation> AxisRotationPtr;

/* ----------------------------------------------------------------------- */

/**
    \class AxisRotated
    \brief A geometry object rotated by an angle about an arbitrary axis.
*/


class SG_API AxisRotated : public OrthoTransformed
{

public:

  /// The default Axis field value.
  static const Vector3 DEFAULT_AXIS;

  /// The default Angle field value.
  static const real_t DEFAULT_ANGLE;

  /// A structure which helps to build a Translated when parsing.
  struct SG_API Builder : public OrthoTransformed::Builder {

    /// A pointer to the \b Axis field.
    Vector3 * Axis;

     /// A pointer to the \b Angle field.
    real_t * Angle;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  AxisRotated();

  /// Constructor
  AxisRotated( const Vector3& axis,
               const real_t& angle,
               const GeometryPtr& geometry );

  /// Destructor
  virtual ~AxisRotated( );

  PGL_OBJECT(AxisRotated)

  /// Returns Angle value.
  const real_t getAngle( ) const;

  /// Returns Angle field.
  real_t& getAngle( );

  /// Returns Axis value.
  const Vector3& getAxis( ) const ;

  /// Returns Axis field.
  Vector3& getAxis( );

  virtual Transformation3DPtr getTransformation( ) const;

  virtual OrthonormalBasis3DPtr getOrthonormalBasis( ) const ;

  /// Returns whether Angle is set to its default value.
  bool isAngleToDefault( ) const;

  /// Returns whether Axis is set to its default value.
  bool isAxisToDefault( ) const;

  virtual bool isValid( ) const;

protected:

  /// The Axis field.
  Vector3 __axis;

  /// The Angle field.
  real_t __angle;

};

/// AxisRotated Pointer
typedef RCPtr<AxisRotated> AxisRotatedPtr;


/* ----------------------------------------------------------------------- */

// __geom_axisrotated_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

