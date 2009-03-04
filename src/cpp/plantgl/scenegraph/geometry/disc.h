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


/*! \file geom_disc.h
    \brief Definition of the geometry class Disc.
*/


#ifndef __geom_disc_h__
#define __geom_disc_h__

/* ----------------------------------------------------------------------- */

#include "sor.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Disc
   \brief A 2D disc.
*/

/* ----------------------------------------------------------------------- */

class SG_API Disc : public SOR2D
{

public:

  /// The \b Radius field default value.
  static const real_t DEFAULT_RADIUS;


  /// A structure which helps to build a Disc when parsing. 
  struct SG_API Builder : public SOR2D::Builder {

    /// A pointer to the \b Radius field.
    real_t * Radius;

    /// Constructor.
    Builder( );    

    /// Destructor.
    ~Builder( );    

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );
    
    virtual bool isValid( ) const;

  };


  /// Defautl constructor.
  Disc( const real_t& radius = DEFAULT_RADIUS, 
	uchar_t Slices = SOR::DEFAULT_SLICES);

  /// Destructor
  virtual ~Disc( );

  PGL_OBJECT(Disc)

  /// Returns \b Radius value.
  const real_t& getRadius( ) const;

  /// Returns \b Radius field.
  real_t& getRadius( );

  virtual bool isAVolume( ) const;

  /// Returns whether \b Radius is set to its default value.
  bool isRadiusToDefault( ) const ;

  virtual bool isValid( ) const;


protected:

  /// The Radius field.
  real_t __radius;

};

/// Disc Pointer
typedef RCPtr<Disc> DiscPtr;


/* ----------------------------------------------------------------------- */

// __geom_disc_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

