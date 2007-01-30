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


/*! \file geom_geometryarray.h
    \brief Definition of the container class GeomArray.
*/


#ifndef __geom_geometryarray_h__
#define __geom_geometryarray_h__

/* ----------------------------------------------------------------------- */

#include "objectarray.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Action;

/* ----------------------------------------------------------------------- */

/**
   \class GeomArray
   \brief An array of geometry object
*/
template <class T>
class GeomArray : public ObjectArray<T>
{

public:

  /** Constructs an GeomArray of size of \e size.
      \post
      - \e self is valid. */
  GeomArray( uint32_t size = 0 ) :
    ObjectArray<T>( size)
     { }


  /** Constructs a GeomArray with the range [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  GeomArray( InIterator first, InIterator last ) :
    ObjectArray<T>(first,last)
     { }

  /// Destructor.
  virtual ~GeomArray( ) {
  }

  /// Applies the action to each Geometry object contained within \e self.
  bool apply( Action& action ) {
	  for (iterator _i = this->getBegin(); _i != this->getEnd(); _i++)
          (*_i)->apply(action);
      return true;
  }

};


/* ----------------------------------------------------------------------- */

// __geom_geometryarray_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

