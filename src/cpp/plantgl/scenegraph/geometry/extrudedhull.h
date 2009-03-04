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

/*! \file geom_extrudedhull.h
    \brief Definition of the geometry class ExtrudedHull.
*/


#ifndef __geom_extrudedhull_h__
#define __geom_extrudedhull_h__

/* ----------------------------------------------------------------------- */

#include "hull.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Curve2D;
typedef RCPtr<Curve2D>  Curve2DPtr;

/* ----------------------------------------------------------------------- */

/**
   \class ExtrudedHull
   \brief A hull extruded by a vertical and an horizontal profiles.
*/

class SG_API ExtrudedHull : public Hull
{

public:

  /// The \b CCW field default value.
  static const bool DEFAULT_CCW;

  /// A structure which helps to build a ExtrudedHull when parsing. 
  struct SG_API Builder : public Hull::Builder { 

    /// A pointer to the \b CCW field.
    bool * CCW;

    /// A pointer to the \b Horizontal field.
    Curve2DPtr * Horizontal;
    
    /// A pointer to the \b Orientation field.
    // real_t * Orientation;

    /// A pointer to the \b Vertical field.
    Curve2DPtr * Vertical;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  ExtrudedHull();

  /** Constructs a ExtrudedHull with the profiles \e horizontal and 
      \e vertical.
      \pre
      - \e horizontal must contain at leat 2 points;
      - \e vertival must contain at leat 2 points.
      \post
      - \e self is valid. */
  ExtrudedHull(  const Curve2DPtr& vertical,
		 const Curve2DPtr& horizontal,
		 bool ccw = DEFAULT_CCW);

  /// Destructor
  virtual ~ExtrudedHull( );

  PGL_OBJECT(ExtrudedHull)

  /// Returns \b CCW value.
  const bool getCCW( ) const;

  /// Returns \b CCW field.
  bool& getCCW( );

  /// Returns \b Horizontal value.
  const Curve2DPtr& getHorizontal( ) const;

  /// Returns \b Horizontal field.
  Curve2DPtr& getHorizontal( );

  /// Returns \b Vertical value.
  const Curve2DPtr& getVertical( ) const;

  /// Returns \b Vertical field.
  Curve2DPtr& getVertical( ) ;

  /// Returns whether \b CCW is set to its default value.
  bool isCCWToDefault( ) const;

  virtual bool isValid( ) const;

protected:
  
  /// The \b Vertical field.
  Curve2DPtr __vertical;  

  /// The \b Horizontal field.
  Curve2DPtr __horizontal;  

  /// The \b CCW field.
  bool __ccw;

}; // ExtrudedHull

/// ExtrudedHull Pointer
typedef RCPtr<ExtrudedHull> ExtrudedHullPtr;


/* ----------------------------------------------------------------------- */

// __geom_extrudedhull_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

