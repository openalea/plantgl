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

/*! \file geom_box.h
    \brief Definition of the geometry class Box.
*/



#ifndef __geom_box_h__
#define __geom_box_h__

/* ----------------------------------------------------------------------- */

#include "parametricmodel.h"
#include <plantgl/math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class Box
    \brief A box 
*/

/* ----------------------------------------------------------------------- */

class SG_API Box : public ParametricModel
{

public:

  /// A structure which helps to build a Box when parsing. 
  struct SG_API Builder : public ParametricModel::Builder {

    /// A pointer to the \b Size field.
    Vector3 * Size;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );
    
    virtual bool isValid( ) const;

  };


  /// The default value fot the Size field.
  static const Vector3 DEFAULT_SIZE;


  /// Constructs a Box with \e size.
  Box( const Vector3& size = DEFAULT_SIZE );

  /// Destructor
  virtual ~Box( );

  PGL_OBJECT(Box)

  /// Returns the back lower left corner.
  Vector3 getBackLowerLeftCorner( ) const;

  /// Returns the back upper left corner.
  Vector3 getBackLowerRightCorner( ) const;

  /// Returns the normal to the back side of \e self.
  Vector3 getBackNormal( ) const;

  /// Returns the back upper left corner.
  Vector3 getBackUpperLeftCorner( ) const;

  /// Returns the back upper left corner.
  Vector3 getBackUpperRightCorner( ) const;

  /// Returns the front lower left corner.
  Vector3 getFrontLowerLeftCorner( ) const;

  /// Returns the front lower left corner.
  Vector3 getFrontLowerRightCorner( ) const;
 
  /// Returns the back upper left corner.
  Vector3 getFrontUpperLeftCorner( ) const ;

  /// Returns the back upper left corner.
  Vector3 getFrontUpperRightCorner( ) const;

  /// Returns the normal to the front side of \e self.
  Vector3 getFrontNormal( ) const;

  /// Returns the normal to the left side of \e self.
  Vector3 getLeftNormal( ) const ;

  /// Returns the normal to the lower side of \e self.
  Vector3 getLowerNormal( ) const;

  /// Returns the normal to the right side of \e self.
  Vector3 getRightNormal( ) const;

  /// Returns the normal to the upper side of \e self.
  Vector3 getUpperNormal( ) const;

  /// Returns Size value.
  const Vector3& getSize( ) const;

  /// Returns Size field.
  Vector3& getSize( );

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  /// Returns whether Size is set to its default value.
  bool isSizeToDefault( ) const ;

  virtual bool isValid( ) const; 

protected:

  /// The Size field.
  Vector3 __size;
  
};

/// Box Pointer
typedef RCPtr<Box> BoxPtr;

 
/* ----------------------------------------------------------------------- */

// __geom_box_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

