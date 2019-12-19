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


/*! \file geom_asymmetrichull.h
    \brief Definition of the geometry class AsymmetricHull.
*/


#ifndef __geom_asymmetrichull_h__
#define __geom_asymmetrichull_h__

/* ----------------------------------------------------------------------- */

#include "hull.h"
#include <plantgl/math/util_vector.h>


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class AsymmetricHull
   \brief An asymmetric hull defined by 6 morphological points.
*/

class SG_API AsymmetricHull : public Hull
{

public:

  /// The \b NegXHeight field default value.
  static const real_t DEFAULT_NEG_X_HEIGHT;

  /// The \b NegXRadius field default value.
  static const real_t DEFAULT_NEG_X_RADIUS;

  /// The \b NegYHeight field default value.
  static const real_t DEFAULT_NEG_Y_HEIGHT;

  /// The \b NegYRadius field default value.
  static const real_t DEFAULT_NEG_Y_RADIUS;

  /// The \b PosXHeight field default value.
  static const real_t DEFAULT_POS_X_HEIGHT;

  /// The \b PosXRadius field default value.
  static const real_t DEFAULT_POS_X_RADIUS;

  /// The \b PosYHeight field default value.
  static const real_t DEFAULT_POS_Y_HEIGHT;

  /// The \b PosYRadius field default value.
  static const real_t DEFAULT_POS_Y_RADIUS;

  /// The \b Bottom field default value.
  static const Vector3 DEFAULT_BOTTOM;

  /// The \b Top field default value.
  static const Vector3 DEFAULT_TOP;

  /// The \b TopShape field default value.
  static const real_t DEFAULT_TOP_SHAPE;

  /// The \b BottomShape field default value.
  static const real_t DEFAULT_BOTTOM_SHAPE;

  /// The \b Slices field default value.
  static const uchar_t DEFAULT_SLICES;

  /// The \b Stacks field default value.
  static const uchar_t DEFAULT_STACKS;


  /// A structure which helps to build a AsymmetricHull when parsing.
  struct SG_API Builder : public Hull::Builder {

    /// A pointer to the \b NegXHeight field.
    real_t * NegXHeight;

    /// A pointer to the \b NegXRadius field.
    real_t * NegXRadius;

    /// A pointer to the \b NegYHeight field.
    real_t * NegYHeight;

    /// A pointer to the \b NegYRadius field.
    real_t * NegYRadius;

    /// A pointer to the \b PosXHeight field.
    real_t * PosXHeight;

    /// A pointer to the \b PosXRadius field.
    real_t * PosXRadius;

    /// A pointer to the \b PosYHeight field.
    real_t * PosYHeight;

    /// A pointer to the \b PosYRadius field.
    real_t * PosYRadius;

    /// A pointer to the \b Bottom field.
    Vector3 * Bottom;

    /// A pointer to the \b Top field.
    Vector3 * Top;

    /// A pointer to the \b BottomShape field.
    real_t * BottomShape;

    /// A pointer to the \b TopShape field.
    real_t * TopShape;

    /// A pointer to the \b Slices field.
    uchar_t * Slices;

    /// A pointer to the \b Stacks field.
    uchar_t * Stacks;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };


  /** Constructs a AsymmetricHull
      \pre

      \post
      - \e self is valid. */
  AsymmetricHull( const real_t& negXRadius = DEFAULT_NEG_X_RADIUS,
          const real_t& posXRadius = DEFAULT_POS_X_RADIUS,
          const real_t& negYRadius = DEFAULT_NEG_Y_RADIUS,
          const real_t& posYRadius = DEFAULT_POS_Y_RADIUS,
          const real_t& negXHeight = DEFAULT_NEG_X_HEIGHT,
          const real_t& posXHeight = DEFAULT_POS_X_HEIGHT,
          const real_t& negYHeight = DEFAULT_NEG_Y_HEIGHT,
          const real_t& posYHeight = DEFAULT_POS_Y_HEIGHT,
          const Vector3& bottom  = DEFAULT_BOTTOM,
          const Vector3& top     = DEFAULT_TOP,
          const real_t& bottomShape= DEFAULT_BOTTOM_SHAPE,
          const real_t& topShape   = DEFAULT_TOP_SHAPE,
          uchar_t slices           = DEFAULT_SLICES,
          uchar_t stacks           = DEFAULT_STACKS);

  /// Destructor
  virtual ~AsymmetricHull( );

  PGL_OBJECT(AsymmetricHull)

  /// Returns \b Bottom value.
  const Vector3& getBottom( ) const;

  /// Returns \b Bottom field.
  Vector3& getBottom( );

  /// Returns \b BottomShape value.
  const real_t& getBottomShape( ) const;

  /// Returns \b BottomShape field.
  real_t& getBottomShape( );

  /// Returns \b NegXHeight value.
  const real_t& getNegXHeight( ) const ;

  /// Returns \b NegXHeight field.
  real_t& getNegXHeight( );

  /// Returns \b NegXRadius value.
  const real_t& getNegXRadius( ) const;

  /// Returns \b NegXRadius field.
  real_t& getNegXRadius( );

  /// Return \b NegX point.
  Vector3 getNegXPoint( ) const ;

  /// Returns \b NegYHeight value.
  const real_t& getNegYHeight( ) const;

  /// Returns \b NegYHeight field.
  real_t& getNegYHeight( );

  /// Returns \b NegYRadius value.
  const real_t& getNegYRadius( ) const;

  /// Returns \b NegYRadius field.
  real_t& getNegYRadius( );

  /// Return \b NegY point.
  Vector3 getNegYPoint( ) const ;

  /// Returns \b PosXHeight value.
  const real_t& getPosXHeight( ) const;

  /// Returns \b PosXHeight field.
  real_t& getPosXHeight( );

  /// Returns \b PosXRadius value.
  const real_t& getPosXRadius( ) const;

  /// Returns \b PosXRadius field.
  real_t& getPosXRadius( ) ;

  /// Return \b PosX point.
  Vector3 getPosXPoint( ) const ;

  /// Returns \b PosYHeight value.
  const real_t& getPosYHeight( ) const;

  /// Returns \b PosYHeight field.
  real_t& getPosYHeight( );

  /// Returns \b PosYRadius value.
  const real_t& getPosYRadius( ) const;

  /// Returns \b PosYRadius field.
  real_t& getPosYRadius( );

  /// Return \b PosY point.
  Vector3 getPosYPoint( ) const ;

  /// Returns \b Top value.
  const Vector3& getTop( ) const;

  /// Returns \b Top field.
  Vector3& getTop( ) ;

  /// Returns \b TopShape value.
  const real_t& getTopShape( ) const;

  /// Returns \b TopShape field.
  real_t& getTopShape( );

  /// Returns \b Slices value.
  const uchar_t& getSlices( ) const;

  /// Returns \b Slices field.
  uchar_t& getSlices( );

  /// Returns \b Stacks value.
  const uchar_t& getStacks( ) const;

  /// Returns \b Stacks field.
  uchar_t& getStacks( );

  /// Returns whether \b Bottom is set to its default value.
  bool isBottomToDefault( ) const;

  /// Returns whether \b BottomShape is set to its default value.
  bool isBottomShapeToDefault( ) const;

  /// Returns whether \b NegXHeight is set to its default value.
  bool isNegXHeightToDefault( ) const;

  /// Returns whether \b NegXRadius is set to its default value.
  bool isNegXRadiusToDefault( ) const;

  /// Returns whether \b NegYHeight is set to its default value.
  bool isNegYHeightToDefault( ) const;

  /// Returns whether \b NegYRadius is set to its default value.
  bool isNegYRadiusToDefault( ) const;

  /// Returns whether \b PosXHeight is set to its default value.
  bool isPosXHeightToDefault( ) const;

  /// Returns whether \b PosXRadius is set to its default value.
  bool isPosXRadiusToDefault( ) const;

  /// Returns whether \b PosYHeight is set to its default value.
  bool isPosYHeightToDefault( ) const;

  /// Returns whether \b PosYRadius is set to its default value.
  bool isPosYRadiusToDefault( ) const;

  /// Returns whether \b Top is set to its default value.
  bool isTopToDefault( ) const ;

  /// Returns whether \b TopShape is set to its default value.
  bool isTopShapeToDefault( ) const ;

  /// Returns whether \b Slices  is set to its default value.
  bool isSlicesToDefault( ) const;

  /// Returns whether \b Stacks value.
  bool isStacksToDefault( ) const;

  virtual bool isValid( ) const;

protected:

  /// The \b NegXHeight field.
  real_t __negXHeight;

  /// The \b NegXRadius field.
  real_t __negXRadius;

  /// The \b NegXShape field.
  Vector2 __negXShape;

  /// The \b NegYHeight field.
  real_t __negYHeight;

  /// The \b NegYRadius field.
  real_t __negYRadius;

  /// The \b NegYShape field.
  Vector2 __negYShape;

  /// The \b PosXHeight field.
  real_t __posXHeight;

  /// The \b PosXRadius field.
  real_t __posXRadius;

  /// The \b PosXShape field.
  Vector2 __posXShape;

  /// The \b PosYHeight field.
  real_t __posYHeight;

  /// The \b PosYRadius field.
  real_t __posYRadius;

  /// The \b PosYShape field.
  Vector2 __posYShape;

  /// The \b Top field.
  Vector3 __top;

  /// The \b Bottom field.
  Vector3 __bottom;

  /// The \b BottomShape field.
  real_t __topShape;

  /// The \b TopShape field.
  real_t __bottomShape;

  /// The \b Slices field.
  uchar_t __slices;

  /// The \b Stacks field.
  uchar_t __stacks;

}; // AsymmetricHull

/// AsymmetricHull Pointer
typedef RCPtr<AsymmetricHull> AsymmetricHullPtr;


/* ----------------------------------------------------------------------- */

// __geom_asymmetrichull_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

