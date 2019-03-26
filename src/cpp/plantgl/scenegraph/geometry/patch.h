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


/*! \file geom_patch.h
    \brief Definition of the geometry class Patch.
*/


#ifndef __geom_patch_h__
#define __geom_patch_h__

/* ----------------------------------------------------------------------- */

#include "parametricmodel.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Point4Matrix;
typedef RCPtr<Point4Matrix> Point4MatrixPtr;

/* ----------------------------------------------------------------------- */

/**
    \class Patch
    \brief Abstract base class for objects of type of 3D parametric patch.
*/

/* ----------------------------------------------------------------------- */

class SG_API Patch : public ParametricModel
{

public:

  static const bool DEFAULT_CCW;

  /// A structure which helps to build an object of type of Patch.
  struct SG_API Builder : public ParametricModel::Builder {

    /// A pointer to the \b CCW field.
    bool * CCW;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

  };


  /// Constructor.
  Patch(bool ccw = DEFAULT_CCW);

  /// Destructor
  virtual ~Patch( ) ;

  /// Returns \e Control Points value.
  virtual Point4MatrixPtr getCtrlPoints( ) const = 0 ;


  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  /// Returns \e CCW value.
  const bool getCCW( ) const;

  /// Returns \e CCW field.
  bool& getCCW( );

  /// Returns whether \e CCW is set to its default value.
  bool isCCWToDefault( ) const;

protected:

  /// the ccw field
  bool __ccw;

};

/// Patch Pointer
typedef RCPtr<Patch> PatchPtr;


/* ----------------------------------------------------------------------- */

// __geom_patch_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

