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


/*! \file geom_cone.h
    \brief Definition of the geometry class Cone.
*/


#ifndef __geom_cone_h__
#define __geom_cone_h__

/* ----------------------------------------------------------------------- */

#include "geom_sor.h"

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Cone
   \brief A cone.
*/

class GEOM_API Cone : public SOR
{

public:

  /// The \b Radius field default value.
  static const real_t DEFAULT_RADIUS;

  /// The \b Height field default value.
  static const real_t DEFAULT_HEIGHT;

  /// The \b Solid field default value.
  static const bool DEFAULT_SOLID;  


  /// A structure which helps to build a Cone when parsing. 
  struct Builder : public SOR::Builder {

    /// A pointer to the \b Height field.
    real_t * Height;

    /// A pointer to the \b Solid field.
    bool * Solid;

    /// A pointer to the \b Radius field.
    real_t * Radius;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );
    
    virtual bool isValid( ) const;

    void ConeDestroy( );
    
    bool ConeValid( ) const;
    
  };


  /// Defautl constructor.
  Cone( const real_t& radius = DEFAULT_RADIUS,
	const real_t& height = DEFAULT_HEIGHT,
	bool solid = DEFAULT_SOLID,
	uchar_t slices = DEFAULT_SLICES );

  /// Destructor
  virtual ~Cone( ) ;

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \b solid value.
  const bool getSolid( ) const;

  /// Returns \b solid field.
  bool& getSolid( );

  /// Returns \b Height value.
  const real_t& getHeight( ) const;
	
  /// Returns \b Height field.
  real_t& getHeight( );
	
  /// Returns \b Radius value.
  const real_t& getRadius( ) const ;

  /// Returns \b Radius field.
  real_t& getRadius( ) ;

  virtual bool isAVolume( ) const ;

  /// Returns whether \b Height is set to its default value.
  bool isHeightToDefault( ) const;

  /// Returns whether \b Solid is set to its default value.
  bool isSolidToDefault( ) const;

  /// Returns whether \b Radius is set to its default value.
  bool isRadiusToDefault( ) const ;

  virtual bool isValid( ) const;
  
protected:

  /// The \b Height field.
  real_t __height;

  /// The \b Solid field.
  bool __solid;

  /// The Radius field.
  real_t __radius;
};

/// Cone Pointer
typedef RCPtr<Cone> ConePtr;


/* ----------------------------------------------------------------------- */

// __geome_cone_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

