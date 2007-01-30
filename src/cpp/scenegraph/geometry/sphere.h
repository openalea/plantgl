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


/*! \file geom_sphere.h
    \brief Definition of the geometry class Sphere.
*/


#ifndef __geom_sphere_h__
#define __geom_sphere_h__

/* ----------------------------------------------------------------------- */

#include "sor.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Sphere
   \brief A sphere.
*/

/* ----------------------------------------------------------------------- */

class SG_API Sphere : public SOR
{

public:

  /// The \b Stacks field default value.
  static const uchar_t DEFAULT_STACKS;

  /// The \b Radius field default value.
  static const real_t DEFAULT_RADIUS;

  /// A structure which helps to build a Sphere when parsing. 
  struct SG_API Builder : public SOR::Builder {
    
    /// A pointer to the Stacks field.
    uchar_t * Stacks;

    /// A pointer to the \b Radius field.
    real_t * Radius;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );
    
    virtual bool isValid( ) const;

  };


  /// Default constructor.
  Sphere( const real_t& radius = DEFAULT_RADIUS,
	  uchar_t Slices = DEFAULT_SLICES,
	  uchar_t Stacks = DEFAULT_STACKS);

  /// Destructor
  virtual ~Sphere( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \b Stacks value.
  const uchar_t& getStacks( ) const;

  /// Returns \b Stacks field.
  uchar_t& getStacks( );

  /// Returns \b Radius value.
  const real_t& getRadius( ) const;

  /// Returns \b Radius field.
  real_t& getRadius( );

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  /// Returns whether \b Stacks is set to its default value.
  bool isStacksToDefault( ) const;

  /// Returns whether \b Radius is set to its default value.
  bool isRadiusToDefault( ) const;

  virtual bool isValid( ) const;

protected:

  /// The \b Stackes field.
  uchar_t __stacks;


  /// The Radius field.
  real_t __radius;
};

/// Sphere Pointer
typedef RCPtr<Sphere> SpherePtr;


/* ----------------------------------------------------------------------- */

// __geom_sphere_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

