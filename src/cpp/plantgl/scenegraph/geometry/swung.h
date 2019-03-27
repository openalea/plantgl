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

