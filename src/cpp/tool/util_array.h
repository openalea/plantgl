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



/*! \file util_array.h
    \brief Definition of the container class Array1 and RealArray.
*/


#ifndef __util_array_h__
#define __util_array_h__

/* ----------------------------------------------------------------------- */

#include <vector>
#include <algorithm>
#include <iostream>

#include "rcobject.h"
#include "tools_config.h"

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Array1
   \brief A mono dimensional array of non fixed size.
*/

/* ----------------------------------------------------------------------- */

template <class T>
class Array1 : public RefCountObject
{

public:

  /// A const iterator used to iterate through an Array1.
  typedef typename std::vector<T>::const_iterator const_iterator;

  /// An iterator used to iterate through an Array1.
  typedef typename std::vector<T>::iterator iterator;

  /// The element type.
  typedef T element_type;

  /// Constructs an Array1 of size \e size
  Array1( uint32_t size = 0 ) :
    RefCountObject(),
    __A(size) {
 }

  /// Constructs an Array1 with \e size copies of \e t.
  Array1( uint32_t size, const T& t ) :
    RefCountObject(),
    __A((unsigned int)size,t) {
  }

  /// Constructs an Array1 with the range [\e begin, \e end).
  template <class InIterator>
  Array1( InIterator begin, InIterator end ) :
    RefCountObject(),
    __A(begin,end) {
  }

  /// Destructor
  virtual ~Array1( ) {
  }

  /// Returns whether \e self contains \e t.
  inline bool contains( const T& t ) const {
    return std::find(__A.begin(),__A.end(),t) != __A.end();
  }

  /** Returns the \e i-th element of \e self
      \pre
      - \e i must be strictly less than the size of \e self. */
  inline const T& getAt( uint32_t i ) const {
    GEOM_ASSERT(i < __A.size());
    return __A[i];
  }

  /** Returns the \e i-th element of \e self
      \pre
      - \e i must be strictly less than the size of \e self. */
  inline T& getAt( uint32_t i ) {
    GEOM_ASSERT(i < __A.size());
    return __A[i];
  }

  /// Returns a const iterator at the beginning of \e self.
  inline const_iterator getBegin( ) const {
    return __A.begin();
  }

  /// Returns an iterator at the beginning of \e self.
  inline iterator getBegin( ) {
    return __A.begin();
  }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator getEnd( ) const {
    return __A.end();
  }

  /// Returns a const iterator at the end of \e self.
  inline iterator getEnd( ) {
    return __A.end();
  }

  /// Returns the size of \e self.
  inline uint32_t getSize( ) const {
    return __A.size();
  }

  /// Returns an iterator at the maximum value of \e self.
  /*const_iterator getMax( ) const {
    return std::max_element(__A.begin(),__A.end());
  }

  /// Returns an iterator at the minimum value of \e self.
  const_iterator getMin( ) const {
    return std::min_element(__A.begin(),__A.end());
  }*/

  /** Returns an iterator first at the minimum value, second at the
      maximum value of \e self. */
   /*std::pair<const_iterator,const_iterator> getMinAndMax( ) const {
    const_iterator _min = __A.begin();
    const_iterator _max = __A.begin();
    for (const_iterator _i = __A.begin() + 1; _i != __A.end(); _i++)
      if (*_i < *_min) _min = _i;
      else if (*_i != *_max) _max = _i;
    return std::pair<const_iterator,const_iterator>(_min,_max);
  }*/

  /// Inserts \e t into \e self before the position pointed by \e it.
  iterator insert( iterator it, const T& t ) {
    return __A.insert(it,t);
  }


  /// Inserts the range [first, last) before pos.
  template <class InputIterator>
      void insert(iterator pos, InputIterator f, InputIterator l){
      __A.insert(pos,f,l);
  }

  /// Inserts \e t into \e self before the position pointed by \e it.
  Array1& operator+=( const Array1& t ) {
    __A.insert(__A.end(),t.getBegin(),t.getEnd());
	return *this;
  }

  bool operator==( const Array1& t ) const {
    if (__A.size() != t.getSize()) 
      return false;
    const_iterator _it= t.getBegin();
    for (const_iterator _i = __A.begin() ; _i != __A.end(); _i++){
	  if(*_i != *_it) return false;
	  _it++;
	}
	return true;
  }

  bool operator!=( const Array1& t ) const {
    if (__A.size() != t.getSize()) return true;
    const_iterator _it = t.getBegin();
    for (const_iterator _i = __A.begin() ; _i != __A.end(); _i++){
	  if(*_i != *_it) return true;
	  _it++;
	}
	return false;
  }

  /// Returns whether \e self is empty.
  inline bool isEmpty( ) const {
    return __A.empty();
  }

  /// Returns whether \e self contain unique elements.
  bool isUnique( ) const {
    if (__A.empty()) return true;
    const_iterator _first = __A.begin();
    const_iterator _last = __A.end() - 1;
    while (_first != _last) {
      if (std::find(_first + 1,__A.end(),*_first) != __A.end()) return false;
      _first++;
    };
    return true;
  }

  /// Prints \e self to the output stream \e stream.
  std::ostream& print( std::ostream& stream,
                  char delimiter = ',',
                  char begin = '[',
                  char end = ']' ) const {
    stream << begin;
    uint32_t _size = __A.size();
    if (_size) {
        stream << __A[0];
        for (uint32_t _i = 1; _i < _size - 1; _i++)
            stream << delimiter << __A[_i];
        if(_size>1) stream <<  delimiter << __A[_size -1];
    };
    return stream << end;
  }

  /// Prints \e self to the output stream \e stream.
  friend std::ostream& operator<<(std::ostream& stream, const Array1<T> a){
      return a.print(stream);
  }


  /** Sets the \e i-th element of \e self to \e t.
      \pre
      - \e i must be strictly less than the size of \e self. */
  void setAt( uint32_t i, const T& t ) {
    GEOM_ASSERT(i < __A.size());
    __A[i] = t;
  }

  /** push back \e t to \e self.
      */
  void pushBack( const T& t ) {
    __A.push_back(t);
  }

  /** increase \e self capacity to size.
      */
  void reserve( uint32_t size ) {
    __A.reserve(size);
  }


  /** erase \e pos to \e self and return the next element.
      */
  iterator Erase(iterator pos) {
    return __A.erase(pos);
  }

  /** erase elements range \e [first,last)  of \e self and return the next element.
      */
  iterator Erase(iterator first,iterator last) {
    return __A.erase(first,last);
  }

  T * data() const {
	if(__A.empty())return NULL;
	T * res = new T[__A.size()];
	size_t _j = 0;
	for (const_iterator _i = __A.begin(); _i != __A.end(); _i++)
	  { res[_j] = *_i; _j++; }
	return res;
  }

protected:

  /// The elements contained by \e self.
  std::vector<T> __A;

};

/// Array of bool
typedef Array1<bool> BoolArray1;
/// Array of bool pointer
typedef RCPtr<BoolArray1> BoolArray1Ptr;
/// Array of char
typedef Array1<char> CharArray1;
/// Array of char pointer
typedef RCPtr<CharArray1> CharArray1Ptr;
/// Array of int16_t
typedef Array1<int16_t> Int16Array1;
/// Array of int16_t pointer
typedef RCPtr<Int16Array1> Int16Array1Ptr;
/// Array of int32_t
typedef Array1<int32_t> Int32Array1;
/// Array of int32_t pointer
typedef RCPtr<Int32Array1> Int32Array1Ptr;
/// Array of uchar
typedef Array1<uchar_t> UCharArray1;
/// Array of uchar_t pointer
typedef RCPtr<UCharArray1> UCharArray1Ptr;
/// Array of uint16_t
typedef Array1<uint16_t> Uint16Array1;
/// Array of uint16_t pointer
typedef RCPtr<Uint16Array1> Uint16Array1Ptr;
/// Array of uint32_t
typedef Array1<uint32_t> Uint32Array1;
/// Array of uint32_t pointer
typedef RCPtr<Uint32Array1> Uint32Array1Ptr;

// typedef Array1<real_t> RealArray1;
// typedef RCPtr<RealArray1> RealArray1Ptr;
// typedef Array1<string> StringArray1;
// typedef RCPtr<StringArray1> StringArray1Ptr;


/*  --------------------------------------------------------------------- */

// GEOM_TEMPLATE_API(Array1<real_t>)

/**
   \class RealArray
   \brief A mono dimensional array of real of non fixed size.
*/
class TOOLS_API RealArray : public Array1<real_t>
{

public:

  /// Constructs an Array1 of size \e size
  RealArray( uint32_t size = 0 ) ;

  /// Constructs an Array1 with \e size copies of \e t.
  RealArray(uint32_t size, const real_t& t  ) ;

  /// Constructs an Array1 with the range [\e begin, \e end).
  template <class InIterator>
  RealArray(InIterator begin, InIterator end ) :
    Array1<real_t>( begin, end ) {
  }

  /// Destructor
  virtual ~RealArray( ); 

};

/// Real Array Pointer
typedef RCPtr<RealArray> RealArrayPtr;

/*  --------------------------------------------------------------------- */

// __util_array_h__
/* ----------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

/// Write Array \b a in \b stream.
  template <class T>
  std::ostream& operator<<(std::ostream& stream, const TOOLS(Array1<T>) a){
      return a.print(stream);
  }

/* ----------------------------------------------------------------------- */
#endif

