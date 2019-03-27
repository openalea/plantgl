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



/*! \file util_cache.h
    \brief Definition of the container class Cache.
*/

#ifndef __util_cache_h__
#define __util_cache_h__

/* ----------------------------------------------------------------------- */

#include "util_hashmap.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Cache
   \brief A cache
*/

/* ----------------------------------------------------------------------- */

template <class T>
class Cache {

public:

  typedef typename pgl_hash_map<size_t,T> maptype;

  /// A const iterator used to iterate through the cache.
  typedef typename maptype::const_iterator const_Iterator;

  /// An iterator used to iterate through the cache.
  typedef typename maptype::iterator Iterator;

  /// Constructs an empty Cache.
  Cache( ) :
    __cache() {
  }

  /// Destructor.
  ~Cache( ) {
    clear();
  }

  /// Returns an iterator at the beginning of \e self.
  inline Iterator begin( ) {
    return __cache.begin();
  }

  /// Returns an iterator at the beginning of \e self.
  inline const_Iterator begin( ) const {
    return __cache.begin();
  }

  /// Clears the cache.
  inline void clear( ) {
    __cache.clear();
  }

  /// Returns a const iterator at the beginning of \e self.
  inline Iterator end( ) {
    return __cache.end();
  }

  /// Returns a const iterator at the beginning of \e self.
  inline const_Iterator end( ) const {
    return __cache.end();
  }

  /// Returns an iterator to the object identified with \e id.
  inline Iterator find( size_t id ) {
    return __cache.find(id);
  }

  /** Inserts into \e self the element \e t associated to the object
      identified with \e id. */
  inline Iterator insert( size_t id, const T& t ) {
    return __cache.insert(std::pair<size_t,T>(id,t)).first;
  }

  inline void remove( size_t id ) {
    Iterator _it = find(id);
    if(_it != end()) __cache.erase(_it);
  }

  /// Returns whether \e self is empty.
  inline bool isEmpty( ) const {
    return __cache.empty();
  }

protected:

  /// The elements contained by \e self.
  maptype __cache;
};



/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __util_cache_h__
#endif
