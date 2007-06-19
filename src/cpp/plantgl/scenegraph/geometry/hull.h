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


/*! \file geom_hull.h
    \brief Definition of the geometry class Hull.
*/


#ifndef __geom_hull_h__
#define __geom_hull_h__

/* ----------------------------------------------------------------------- */

#include "parametricmodel.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class Hull
    \brief Abstract base class for objects of type of 3D envelop.
*/

class SG_API Hull : public ParametricModel
{

public:

  /// A structure which helps to build an object of type of Hull.
  struct SG_API Builder : public ParametricModel::Builder {

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

  };


  /// Constructor.
  Hull( );

  /// Destructor
  virtual ~Hull( );

  virtual bool isACurve( ) const ;

  virtual bool isASurface( ) const ;

  virtual bool isAVolume( ) const ;

};

/// Hull Pointer
typedef RCPtr<Hull> HullPtr;
 

/* ----------------------------------------------------------------------- */

// __geom_hull_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

