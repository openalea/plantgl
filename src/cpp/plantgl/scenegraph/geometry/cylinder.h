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


/*! \file geom_cylinder.h
    \brief Definition of the geometry class Cylinder.
*/


#ifndef __geom_cylinder_h__
#define __geom_cylinder_h__

/* ----------------------------------------------------------------------- */

#include "cone.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Cylinder
   \brief A cylinder.
*/

/* ----------------------------------------------------------------------- */

class SG_API Cylinder : public Cone
{

public:

  /// A structure which helps to build a Cylinder when parsing.
  struct SG_API Builder : public Cone::Builder {

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

  };


  /// Defautl constructor.
  Cylinder( const real_t& radius = DEFAULT_RADIUS,
	    const real_t& height = DEFAULT_HEIGHT,
	    bool solid = DEFAULT_SOLID,
	    uchar_t slices = DEFAULT_SLICES );
  
  /// Destructor
  virtual ~Cylinder( ) ;

  PGL_OBJECT(Cylinder)

};

/// Cylinder Pointer
typedef RCPtr<Cylinder> CylinderPtr;


/* ----------------------------------------------------------------------- */

// __geom_cylinder_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

