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

/*! \file geom_parametricmodel.h
    \brief Definition of the geometry class ParametricModel.
*/


#ifndef __geom_parametricmodel_h__
#define __geom_parametricmodel_h__

/* ------------------------------------------------------------------------- */

#include "primitive.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ------------------------------------------------------------------------- */

/**
    \class ParametricModel
    \brief Abstract base class for all  parametric primitives.
*/

/* ----------------------------------------------------------------------- */

class SG_API ParametricModel : public virtual Primitive
{

public:

  /// A structure which helps to build an object of type of ParametricModel.
  struct SG_API Builder : public Primitive::Builder {

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

  };


  /// Constructs a ParametricModel.
  ParametricModel( );

  /// Destructor.
  virtual ~ParametricModel( );

  virtual bool isExplicit( ) const;

}; // ParametricModel

/// ParametricModel Pointer
typedef RCPtr<ParametricModel> ParametricModelPtr;


/* ------------------------------------------------------------------------- */

// __geom_parametricmodel_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

