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

template<class Iterator>
std::ostream& print_array( Iterator beg, Iterator end,
						   std::ostream& stream,
						   char delimiter = ',',
						   char beg_sign = '[',
                           char end_sign = ']' ) {
    stream << beg_sign;
    if (beg != end) {
        stream << *beg;
        for (Iterator _i = beg+1; _i != end; _i++)
            stream << delimiter << *_i;
    }
    return stream << end_sign;
}

  /// Returns whether \e self contain unique elements.
template<class Iterator>
bool isUnique(Iterator beg, Iterator end ) {
    if (distance(beg,end) == 0) return true;
    Iterator _first = beg;
    Iterator _last  = end - 1;
    while (_first != _last) {
      if (std::find(_first + 1,end,*_first) != end) return false;
      _first++;
    };
    return true;
}

/* ----------------------------------------------------------------------- */

/**
   \class Array1
   \brief A mono dimensional array of non fixed size.
*/


/* ----------------------------------------------------------------------- */

template <class T>
class PglVector 
{

public:

  /// The element type.
  typedef T element_type;

  /// A const iterator used to iterate through an Array1.
  typedef typename std::vector<element_type>::const_iterator const_iterator;

  /// An iterator used to iterate through an Array1.
  typedef typename std::vector<element_type>::iterator iterator;

//   /// The element type.
//   typedef T element_type;

  /// Constructs an Array1 of size \e size
  PglVector( size_t size = 0 ) :
    __A(size) {
 }

  /// Constructs an Array1 with \e size copies of \e t.
  PglVector( size_t size, const T& t ) :
    __A(size,t) {
  }


  /// Constructs an Array1 with the range [\e begin, \e end).
  template <class InIterator>
  PglVector( InIterator begin, InIterator end ) :
    __A(begin,end) {
  }

  /// Destructor
  virtual ~PglVector( ) {
  }

  /// Returns whether \e self contains \e t.
  inline bool contains( const T& t ) const {
    return std::find(__A.begin(),__A.end(),t) != __A.end();
  }

  /** Returns the \e i-th element of \e self
      \pre
      - \e i must be strictly less than the size of \e self. */
  inline const T& getAt( uint_t i ) const {
    GEOM_ASSERT(i < __A.size());
    return __A[i];
  }

  /** Returns the \e i-th element of \e self
      \pre
      - \e i must be strictly less than the size of \e self. */
  inline T& getAt( uint_t i ) {
    GEOM_ASSERT(i < __A.size());
    return __A[i];
  }

  inline const T& operator[]( uint_t i ) const { return getAt(i); }
  inline T& operator[]( uint_t i ) { return getAt(i); }

  /// Returns a const iterator at the beginning of \e self.
  inline const_iterator begin( ) const { return __A.begin(); }

  /// Returns an iterator at the beginning of \e self.
  inline iterator begin( ) { return __A.begin(); }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator end( ) const { return __A.end(); }

  /// Returns a const iterator at the end of \e self.
  inline iterator end( ) { return __A.end(); }

  /// Returns the size of \e self.
  inline uint_t size( ) const { return __A.size(); }

  /// Clear \e self.
  inline void clear( ) { __A.clear();  }

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
  PglVector& operator+=( const PglVector& t ) {
    __A.insert(__A.end(),t.begin(),t.end());
	return *this;
  }

  bool operator==( const PglVector& t ) const {
    if (__A.size() != t.size()) 
      return false;
    const_iterator _it= t.begin();
    for (const_iterator _i = __A.begin() ; _i != __A.end(); _i++){
	  if(*_i != *_it) return false;
	  _it++;
	}
	return true;
  }

  bool operator!=( const PglVector& t ) const {
    if (__A.size() != t.size()) return true;
    const_iterator _it = t.begin();
    for (const_iterator _i = __A.begin() ; _i != __A.end(); _i++){
	  if(*_i != *_it) return true;
	  _it++;
	}
	return false;
  }

  /// Returns whether \e self is empty.
  inline bool empty( ) const { return __A.empty(); }

  /// Returns whether \e self contain unique elements.
  bool isUnique( ) const {
	  return TOOLS::isUnique(__A.begin(),__A.end());
  }

  /// Prints \e self to the output stream \e stream.
  std::ostream& print( std::ostream& stream,
                  char delimiter = ',',
                  char beg_sign = '[',
                  char end_sign = ']' ) const {
		return print_array(begin(),end(),stream,delimiter,beg_sign,end_sign);
  }

  /// Prints \e self to the output stream \e stream.
  friend std::ostream& operator<<(std::ostream& stream, const PglVector<T>& a){
      return a.print(stream);
  }


  /** Sets the \e i-th element of \e self to \e t.
      \pre
      - \e i must be strictly less than the size of \e self. */
  void setAt( uint_t i, const T& t ) {
    GEOM_ASSERT(i < __A.size());
    __A[i] = t;
  }

  /** push back \e t to \e self. */
  void push_back( const T& t ) { __A.push_back(t); }

  /** increase \e self capacity to size.
      */
  void reserve( uint_t size ) {
    __A.reserve(size);
  }


  /** erase \e pos to \e self and return the next element. */
  iterator erase(iterator pos) { return __A.erase(pos); }

  /** erase elements range \e [first,last)  of \e self and return the next element. */
  iterator erase(iterator first,iterator last) { return __A.erase(first,last); }

  T * data() const {
	if(__A.empty())return NULL;
	T * res = new T[__A.size()];
	size_t _j = 0;
	for (const_iterator _i = __A.begin(); _i != __A.end(); _i++)
	  { res[_j] = *_i; _j++; }
	return res;
  }
  void reverse( ) {
      std::reverse(__A.begin(),__A.end());
  }

#ifndef PGL_NO_DEPRECATED
  /// Returns a const iterator at the beginning of \e self.
  inline attribute_deprecated const_iterator getBegin( ) const { return begin(); }

  /// Returns an iterator at the beginning of \e self.
  inline attribute_deprecated iterator getBegin( ) { return begin(); }

  /// Returns a const iterator at the end of \e self.
  inline attribute_deprecated const_iterator getEnd( ) const { return end(); }

  /// Returns an iterator at the end of \e self.
  inline attribute_deprecated iterator getEnd( ) { return end(); }

  /// Return size of this
  inline attribute_deprecated uint_t getSize() const { return size(); }

  inline attribute_deprecated bool isEmpty() const { return empty(); }

  inline attribute_deprecated void pushBack( const T& t ) { push_back(t); }

  inline attribute_deprecated iterator Erase(iterator pos) { return erase(pos); }

  inline attribute_deprecated iterator Erase(iterator first,iterator last) { return erase(first,last); }

#endif

protected:

  /// The elements contained by \e self.
  std::vector<T> __A;

};

/* ----------------------------------------------------------------------- */

template <class T>
class Array1 : public RefCountObject, public PglVector<T>
{
public:
  /// Constructs an Array1 of size \e size
  Array1( size_t size = 0 ) :
    RefCountObject(),
    PglVector<T>(size) {
 }

  /// Constructs an Array1 with \e size copies of \e t.
  Array1( size_t size, const T& t ) :
    RefCountObject(),
    PglVector<T>(size,t) {
  }


  /// Constructs an Array1 with the range [\e begin, \e end).
  template <class InIterator>
  Array1( InIterator begin, InIterator end ) :
    RefCountObject(),
    PglVector<T>(begin,end) {
  }

  /// Inserts \e t into \e self before the position pointed by \e it.
  Array1& operator+=( const Array1& t ) {
    PglVector<T>::__A.insert(PglVector<T>::end(),t.begin(),t.end());
	return *this;
  }
};

/// Constructs an Array1 with \e size copies of \e t.
template<class U, class T = typename U::element_type>
struct range {
    const T& firstvalue;
    const T& increment;
    size_t size;

    range(size_t _size, const T& _firstvalue,  const T& _increment) :
        size(_size),
        firstvalue(_firstvalue),
        increment(_increment) {}

    operator U () const {
        U result(size,firstvalue);
        T value = firstvalue + increment;
        for (size_t i = 1; i < size; ++i, value += increment)
            result[i] = value;
        return result;
    }
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
/// Array of int_t
typedef Array1<int_t> IntArray1;
/// Array of int_t pointer
typedef RCPtr<IntArray1> IntArray1Ptr;
/// Array of uchar
typedef Array1<uchar_t> UCharArray1;
/// Array of uchar_t pointer
typedef RCPtr<UCharArray1> UCharArray1Ptr;
/// Array of uint16_t
typedef Array1<uint16_t> Uint16Array1;
/// Array of uint16_t pointer
typedef RCPtr<Uint16Array1> Uint16Array1Ptr;
/// Array of uint_t
typedef Array1<uint32_t> Uint32Array1;
/// Array of uint_t pointer
typedef RCPtr<Uint32Array1> Uint32Array1Ptr;
/// Array of uint_t
typedef Array1<uint_t> UintArray1;
/// Array of uint_t pointer
typedef RCPtr<UintArray1> UintArray1Ptr;

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
  RealArray( uint_t size = 0 ) ;

  /// Constructs an Array1 with \e size copies of \e t.
  RealArray(uint_t size, const real_t& t  ) ;

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
  std::ostream& operator<<(std::ostream& stream, const TOOLS(Array1<T>)& a){
      return a.print(stream);
  }

/* ----------------------------------------------------------------------- */
#endif

