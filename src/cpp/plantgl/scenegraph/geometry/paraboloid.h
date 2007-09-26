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


/*! \file geom_paraboloid.h
    \brief Definition of the geometry class Paraboloid.
*/


#ifndef __geom_paraboloid_h__
#define __geom_paraboloid_h__

/* ----------------------------------------------------------------------- */

#include "cone.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Paraboloid
   \brief A paraboloid.
*/

/* ----------------------------------------------------------------------- */

class SG_API Paraboloid : public Cone
{

public:

  /// The \b Shape field default value.
  static const real_t DEFAULT_SHAPE;

  /// The \b Stacks field default value.
  static const uchar_t DEFAULT_STACKS;


  /// A structure which helps to build a Paraboloid when parsing.
  struct SG_API Builder : public Cone::Builder {

    /// A pointer to the \b Shape field.
    real_t * Shape;

    /// A pointer to the \b Stacks field.
    uchar_t * Stacks;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );
    
    virtual bool isValid( ) const;

  };


  /// Defautl constructor.
  Paraboloid( const real_t& radius = DEFAULT_RADIUS,
	      const real_t& height = DEFAULT_HEIGHT,
	      const real_t& shape =  DEFAULT_SHAPE,
	      bool solid = DEFAULT_SOLID,
	      uchar_t slices = DEFAULT_SLICES,
	      uchar_t stacks = DEFAULT_STACKS );
  
  /// Destructor
  virtual ~Paraboloid( ) ;

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \b Shape value.
  const real_t& getShape( ) const ;

  /// Returns \b Shape field.
  real_t& getShape( );

  /// Returns \b Stacks value.
  const uchar_t& getStacks( ) const ;

  /// Returns \b Stacks field.
  uchar_t& getStacks( ) ;

  /// Returns whether \b Shape is set to its default value.
  bool isShapeToDefault( ) const ;

  /// Returns whether \b Stacks is set to its default value.
  bool isStacksToDefault( ) const ;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

protected:

  /// The \b Shape field.
  real_t __shape;

  /// The \b Stacks field.
  uchar_t __stacks;

};

typedef RCPtr<Paraboloid> ParaboloidPtr;


/* ----------------------------------------------------------------------- */

// __geom_paraboloid_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

