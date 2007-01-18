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


/*! \file geom_asymmetrichull.h
    \brief Definition of the geometry class AsymmetricHull.
*/


#ifndef __geom_asymmetrichull_h__
#define __geom_asymmetrichull_h__

/* ----------------------------------------------------------------------- */

#include "geom_hull.h"
#include "../Tools/util_vector.h"


/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class AsymmetricHull
   \brief An asymmetric hull defined by 6 morphological points.
*/

class GEOM_API AsymmetricHull : public Hull
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
  static const TOOLS(Vector3) DEFAULT_BOTTOM;

  /// The \b Top field default value.
  static const TOOLS(Vector3) DEFAULT_TOP;

  /// The \b TopShape field default value.
  static const real_t DEFAULT_TOP_SHAPE;

  /// The \b BottomShape field default value.
  static const real_t DEFAULT_BOTTOM_SHAPE;

  /// The \b Slices field default value.
  static const uchar_t DEFAULT_SLICES;

  /// The \b Stacks field default value.
  static const uchar_t DEFAULT_STACKS;


  /// A structure which helps to build a AsymmetricHull when parsing. 
  struct Builder : public Hull::Builder { 

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
    TOOLS(Vector3) * Bottom;

    /// A pointer to the \b Top field.
    TOOLS(Vector3) * Top;

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
		  const TOOLS(Vector3)& bottom  = DEFAULT_BOTTOM,
		  const TOOLS(Vector3)& top     = DEFAULT_TOP,
		  const real_t& bottomShape= DEFAULT_BOTTOM_SHAPE,
		  const real_t& topShape   = DEFAULT_TOP_SHAPE,
		  uchar_t slices           = DEFAULT_SLICES,
		  uchar_t stacks           = DEFAULT_STACKS);

  /// Destructor
  virtual ~AsymmetricHull( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \b Bottom value.
  const TOOLS(Vector3)& getBottom( ) const;

  /// Returns \b Bottom field.
  TOOLS(Vector3)& getBottom( );

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
  TOOLS(Vector3) getNegXPoint( ) const ;
  
  /// Returns \b NegYHeight value.
  const real_t& getNegYHeight( ) const;

  /// Returns \b NegYHeight field.
  real_t& getNegYHeight( );

  /// Returns \b NegYRadius value.
  const real_t& getNegYRadius( ) const;

  /// Returns \b NegYRadius field.
  real_t& getNegYRadius( );

  /// Return \b NegY point.
  TOOLS(Vector3) getNegYPoint( ) const ;

  /// Returns \b PosXHeight value.
  const real_t& getPosXHeight( ) const;

  /// Returns \b PosXHeight field.
  real_t& getPosXHeight( );

  /// Returns \b PosXRadius value.
  const real_t& getPosXRadius( ) const;

  /// Returns \b PosXRadius field.
  real_t& getPosXRadius( ) ;

  /// Return \b PosX point.
  TOOLS(Vector3) getPosXPoint( ) const ;

  /// Returns \b PosYHeight value.
  const real_t& getPosYHeight( ) const;

  /// Returns \b PosYHeight field.
  real_t& getPosYHeight( );

  /// Returns \b PosYRadius value.
  const real_t& getPosYRadius( ) const;

  /// Returns \b PosYRadius field.
  real_t& getPosYRadius( );

  /// Return \b PosY point.
  TOOLS(Vector3) getPosYPoint( ) const ;

  /// Returns \b Top value.
  const TOOLS(Vector3)& getTop( ) const;

  /// Returns \b Top field.
  TOOLS(Vector3)& getTop( ) ;

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
  TOOLS(Vector2) __negXShape;

  /// The \b NegYHeight field.
  real_t __negYHeight;

  /// The \b NegYRadius field.
  real_t __negYRadius;

  /// The \b NegYShape field.
  TOOLS(Vector2) __negYShape;

  /// The \b PosXHeight field.
  real_t __posXHeight;

  /// The \b PosXRadius field.
  real_t __posXRadius;

  /// The \b PosXShape field.
  TOOLS(Vector2) __posXShape;

  /// The \b PosYHeight field.
  real_t __posYHeight;

  /// The \b PosYRadius field.
  real_t __posYRadius;

  /// The \b PosYShape field.
  TOOLS(Vector2) __posYShape;

  /// The \b Top field.
  TOOLS(Vector3) __top;

  /// The \b Bottom field.
  TOOLS(Vector3) __bottom;

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

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

