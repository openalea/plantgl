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


/*! \file geom_revolution.h
    \brief Definition of the geometry class Revolution.
*/


#ifndef __geom_revolution_h__
#define __geom_revolution_h__

/* ----------------------------------------------------------------------- */

#include "sor.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Curve2D;
typedef RCPtr<Curve2D> Curve2DPtr;

/* ----------------------------------------------------------------------- */

/**
   \class Revolution
   \brief A revolution represented by a profile.
*/

/* ----------------------------------------------------------------------- */

class SG_API Revolution : public SOR
{

public:

  /// A structure which helps to build a Revolution when parsing. 
  struct SG_API Builder : public SOR::Builder {
    
    /// A pointer to the PointList field.
    Curve2DPtr * Profile;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  Revolution();

  /// Constructs a Revolution from the 2D polyline \e points.
  Revolution( const Curve2DPtr& profile,
	      uchar_t slices = DEFAULT_SLICES  );

  /// Destructor
  virtual ~Revolution( );

  PGL_OBJECT(Revolution)

  /// Returns PointList values.
  const Curve2DPtr& getProfile( ) const ;

  /// Returns PointList field.
  Curve2DPtr& getProfile( );

  virtual bool isAVolume( ) const;

  virtual bool isValid( ) const;

protected:

  /// The PointList field.
  Curve2DPtr __profile;
  
}; // Revolution

/// Revolution Pointer
typedef RCPtr<Revolution> RevolutionPtr;


/* ----------------------------------------------------------------------- */

// __geom_revolution_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

