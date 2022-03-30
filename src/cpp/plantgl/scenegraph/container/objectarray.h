/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
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

