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



/*! \file util_indexarray.h
    \brief Definition of the container class Index3Array, Index4Array and IndexArray.
*/

#ifndef __util_indexarray_h__
#define __util_indexarray_h__

/* ----------------------------------------------------------------------- */

#include "../sg_config.h"
#include <plantgl/tool/util_array.h>
#include <plantgl/tool/util_tuple.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/// Index of size 3
typedef TOOLS(Uint32Tuple3) Index3;

/* ----------------------------------------------------------------------- */

/**
   \class Index3Array
   \brief An array of indices of size of 3.
*/

/* ----------------------------------------------------------------------- */

class SG_API Index3Array : public TOOLS(Array1)<Index3>
{

public:

  /** Constructs an Index3Array of size of \e size.
      \post
      - \e self is valid. */
  Index3Array( uint32_t size = 0 );

  /** Constructs a Index3Array with the range [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Index3Array( InIterator first, InIterator last ) :
    TOOLS(Array1)<Index3>(first,last) {
    GEOM_ASSERT(isValid());
  }

  /// Destructor.
  virtual ~Index3Array( );

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  uint32_t * data() const;

};

/* ----------------------------------------------------------------------- */

/// Index3Array Pointer
typedef RCPtr<Index3Array> Index3ArrayPtr;

/* ----------------------------------------------------------------------- */

/// Index of size 4
typedef TOOLS(Uint32Tuple4) Index4;

/* ----------------------------------------------------------------------- */

/**
   \class Index4Array
   \brief An array of indices of size of 4.
*/

/* ----------------------------------------------------------------------- */

class SG_API Index4Array : public TOOLS(Array1)<Index4>
{

public:

  /** Constructs an Index4Array of size of \e size.
      \post
      - \e self is valid. */
  Index4Array( uint32_t size = 0 );

  /** Constructs a Index4Array with the range [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Index4Array( InIterator first, InIterator last ) :
    TOOLS(Array1)<Index4>(first,last) {
    GEOM_ASSERT(isValid());
  }

  /// Destructor.
  virtual ~Index4Array( );

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  /** Returns the Index3Array corresponding to the tesselation of \e self.
      \pre
      - \e self must represent a convex surface.
  */
  Index3ArrayPtr triangulate( ) const;

  uint32_t * data() const;

};

/// Index4Array Pointer
typedef RCPtr<Index4Array> Index4ArrayPtr;

/* ----------------------------------------------------------------------- */

/// Index of non fixed size
typedef TOOLS(Uint32Array1) Index;

/* ----------------------------------------------------------------------- */

/**
   \class IndexArray
   \brief An array of indices of non fixed size.
*/

/* ----------------------------------------------------------------------- */

class SG_API IndexArray : public TOOLS(Array1)<Index>
{

public:

  /** Constructs an IndexArray of size of \e size.
      \post
      - \e self is valid. */
  IndexArray( uint32_t size = 0 );

  /** Constructs an IndexArray from an Index3Array.
      \post
      - \e self is valid. */
  IndexArray( const Index3Array& array );

  /** Constructs an IndexArray from an Index4Array.
      \post
      - \e self is valid. */
  IndexArray( const Index4Array& array );

  /** Constructs a IndexArray with the range [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  IndexArray( InIterator first, InIterator last ) :
    TOOLS(Array1)<Index>(first,last) {
    GEOM_ASSERT(isValid());
  }

  /// Destructor.
  virtual ~IndexArray( ) ;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  /** Returns the Index3Array corresponding to the tesselation of \e self.
      \pre
      - \e self must represent a convex surface.
  */
  Index3ArrayPtr triangulate( ) const;

  /// Sets the \b i-th element of self to \e t. 
  void setAt(uint32_t i, const Index3& t );
  
  /// Sets the \b i-th element of self to \e t. 
  void setAt(uint32_t i, const Index4& t );

  /// Sets the \b i-th element of self to \e t. 
  void setAt(uint32_t i, const Index& t ) ;

  uint32_t * data() const;

};

/// IndexArray Pointer
typedef RCPtr<IndexArray> IndexArrayPtr;


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __util_indexarray_h__
#endif

