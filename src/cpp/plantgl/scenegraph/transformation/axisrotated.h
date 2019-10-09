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

