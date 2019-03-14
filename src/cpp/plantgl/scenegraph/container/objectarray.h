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


/*! \file geom_objectarray.h
    \brief Definition of the container class ObjectArray.
*/

#ifndef __geom_objectarray_h__
#define __geom_objectarray_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/tool/util_array.h>
#include "../sg_config.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class ObjectArray
   \brief An array of object
*/

template <class T>
class ObjectArray : public TOOLS(Array1<T>)
{

public:

  typedef typename Array1<T>::const_iterator const_iterator;
  /** Constructs an ObjectArray of size of \e size.
      \post
      - \e self is valid. */
  ObjectArray( uint_t size = 0 ) :
    Array1<T>( size)
     { }


  /** Constructs a GeomArray with the range [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  ObjectArray( InIterator first, InIterator last ) :
    Array1<T>(first,last) {
    GEOM_ASSERT(isValid());
  }

  /// Destructor.
  virtual ~ObjectArray( ) {
  }

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const
  {
    const_iterator _i;
    for ( _i= this->begin(); _i != this->end(); _i++ )
      {
      if( ! (*_i) )
        return false;
      if( ! (*_i)->isValid() )
        return false;
      }
    return true;
    }

};


/* ----------------------------------------------------------------------- */

// __geom_geometryarray_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

