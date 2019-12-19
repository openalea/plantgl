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
typedef UintTuple3 Index3;

/* ----------------------------------------------------------------------- */

/**
   \class Index3Array
   \brief An array of indices of size of 3.
*/

/* ----------------------------------------------------------------------- */

class SG_API Index3Array : public Array1<Index3>
{

public:

  /** Constructs an Index3Array of size of \e size.
      \post
      - \e self is valid. */
  Index3Array( uint_t size = 0, const Index3& defaultvalue = Index3());

  /** Constructs a Index3Array with the range [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Index3Array( InIterator first, InIterator last ) :
    Array1<Index3>(first,last) {
    GEOM_ASSERT(isValid());
  }

  /// Destructor.
  virtual ~Index3Array( );

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  uint_t * data() const;

  inline uint_t getIndexSizeAt(uint_t) const { return 3; }

};

/* ----------------------------------------------------------------------- */

/// Index3Array Pointer
typedef RCPtr<Index3Array> Index3ArrayPtr;
PGL_DECLARE_TYPE(Index3Array)

/* ----------------------------------------------------------------------- */

/// Index of size 4
typedef UintTuple4 Index4;

/* ----------------------------------------------------------------------- */

/**
   \class Index4Array
   \brief An array of indices of size of 4.
*/

/* ----------------------------------------------------------------------- */

class SG_API Index4Array : public Array1<Index4>
{

public:
  /** Constructs an Index4Array of size of \e size.
      \post
      - \e self is valid. */
  Index4Array( uint_t size = 0, const Index4& defaultvalue = Index4() );

  /** Constructs a Index4Array with the range [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Index4Array( InIterator first, InIterator last ) :
    Array1<Index4>(first,last) {
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

  uint_t * data() const;

  inline uint_t getIndexSizeAt(uint_t) const { return 4; }

};

/// Index4Array Pointer
typedef RCPtr<Index4Array> Index4ArrayPtr;
PGL_DECLARE_TYPE(Index4Array)
/* ----------------------------------------------------------------------- */

/// Index of non fixed size
// typedef UintArray1 Index;
typedef TOOLS(PglVector<uint_t>) Index;

/* ----------------------------------------------------------------------- */

/**
   \class IndexArray
   \brief An array of indices of non fixed size.
*/

/* ----------------------------------------------------------------------- */

class SG_API IndexArray : public Array1<Index>
{

public:

  /** Constructs an IndexArray of size of \e size.
      \post
      - \e self is valid. */
  IndexArray( uint_t size = 0, const Index& defaultvalue = Index() );

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
    Array1<Index>(first,last) {
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
  void setAt(uint_t i, const Index3& t );

  /// Sets the \b i-th element of self to \e t.
  void setAt(uint_t i, const Index4& t );

  /// Sets the \b i-th element of self to \e t.
  void setAt(uint_t i, const Index& t ) ;

  uint_t * data() const;

  inline uint_t getIndexSizeAt(uint_t t) const { return getAt(t).size(); }

};

/// IndexArray Pointer
typedef RCPtr<IndexArray> IndexArrayPtr;
PGL_DECLARE_TYPE(IndexArray)

template<class IndexArrayType>
void shift_all_indices(RCPtr<IndexArrayType> indices, int shift)
{
    typedef typename IndexArrayType::iterator ArrayIterator;
    typedef typename IndexArrayType::element_type Index;
    typedef typename Index::iterator IndexIterator;
    for (ArrayIterator it = indices->begin(); it != indices->end(); ++it){
        for(IndexIterator itv = it->begin(); itv != it->end(); ++itv){
            *itv += shift;
        }
    }
}

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __util_indexarray_h__
#endif

