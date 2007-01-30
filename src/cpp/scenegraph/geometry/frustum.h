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


/*! \file geom_frustum.h
    \brief Definition of the geometry class Frustum.
*/


#ifndef __geom_frustum_h__
#define __geom_frustum_h__

/* ----------------------------------------------------------------------- */

#include "cone.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Frustum
   \brief A frustum.
*/

/* ----------------------------------------------------------------------- */


class SG_API Frustum : public Cone
{

public:

  /// The \b Taper field default value.
  static const real_t DEFAULT_TAPER;


  /// A structure which helps to build a Frustum when parsing.
  struct Builder : public Cone::Builder {

    /// A pointer to the \b Taper field.
    real_t * Taper;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );
    
    virtual bool isValid( ) const;

  };


  /// Defautl constructor.
  Frustum( const real_t& radius = DEFAULT_RADIUS,
	   const real_t& height = DEFAULT_HEIGHT,
	   const real_t& taper = DEFAULT_TAPER,
	   bool solid = DEFAULT_SOLID,
	   uchar_t Slices = DEFAULT_SLICES );
  
  /// Destructor
  virtual ~Frustum( ) ;

  virtual bool apply( Action& action );
  
  /// Returns \b Taper value.
  const real_t& getTaper( ) const ;

  /// Returns \b Taper field.
  real_t& getTaper( );

  /// Returns whether \b Taper is set to its default value.
  bool isTaperToDefault( ) const;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

protected:

  /// The \b Taper field.
  real_t __taper;

};

/// Frustum Pointer
typedef RCPtr<Frustum> FrustumPtr;

/* ----------------------------------------------------------------------- */

// __geom_frustum_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

