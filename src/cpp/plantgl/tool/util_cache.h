/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
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


/*! \file util_cache.h
    \brief Definition of the container class Cache.
*/

#ifndef __util_cache_h__
#define __util_cache_h__

/* ----------------------------------------------------------------------- */

#include "util_hashmap.h"

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

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

TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __util_cache_h__
#endif
