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

  virtual Vector3 extractEulerAngles() const;
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

  PGL_OBJECT(EulerRotated)

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

