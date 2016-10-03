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
#include "classinfo.h"

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
  typedef typename std::vector<element_type>::const_reverse_iterator const_reverse_iterator ;

  /// An iterator used to iterate through an Array1.
  typedef typename std::vector<element_type>::reverse_iterator reverse_iterator ;

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

  /// Returns a const iterator at the beginning of \e self.
  inline const_reverse_iterator rbegin( ) const { return __A.rbegin(); }

  /// Returns an iterator at the beginning of \e self.
  inline reverse_iterator rbegin( ) { return __A.rbegin(); }

  /// Returns a const iterator at the end of \e self.
  inline const_reverse_iterator rend( ) const { return __A.rend(); }

  /// Returns a const iterator at the end of \e self.
  inline reverse_iterator rend( ) { return __A.rend(); }

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

  template <class InputIterator>
  void push_back( InputIterator first, InputIterator last  ) { __A.push_back(first, last); }

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

TOOLS_END_NAMESPACE

#include <math.h>

TOOLS_BEGIN_NAMESPACE

template <class T>
class NumericArray1 : public Array1<T>
{

public:
  typedef typename Array1<T>::const_iterator const_iterator;

  /// Constructs an NumericArray1 
  NumericArray1( size_t size = 0 ) :
      Array1<T>( size )
      {
     }

  /// Constructs an NumericArray1 with \e size copies of \e t.
  NumericArray1( size_t size, const T& t ) :
    Array1<T>( size, t )
      {
       }

  /// Constructs an NumericArray1 with the range [\e begin, \e end).
  template <class InIterator>
  NumericArray1( InIterator begin, InIterator end) :
    Array1<T>(begin,end)
      {
   }

  /// Destructor
  virtual ~NumericArray1( ) {
  }

  /// Returns an iterator at the maximum value of \e self.
  inline const_iterator getMax( ) const {
      return std::max_element(this->__A.begin(),this->__A.end());
  }

  /// Returns an iterator at the minimum value of \e self.
  inline const_iterator getMin( ) const {
    return std::min_element(this->__A.begin(),this->__A.end());
  }

  /** Returns an iterator first at the minimum value, second at the
      maximum value of \e self. */
 inline std::pair<const_iterator,const_iterator> getMinAndMax(bool filterinf = false) const {
    const_iterator _min = this->__A.begin();
    if (filterinf) while (!isfinite(*_min) && _min != this->__A.end()) ++_min;
    const_iterator _max = _min;
    for (const_iterator _i = _min + 1; _i != this->__A.end(); _i++)
      if ((*_i < *_min) && (!filterinf || isfinite(*_i))) _min = _i;
      else if ((*_i > *_max) && (!filterinf || isfinite(*_i))) _max = _i;
    return std::pair<const_iterator,const_iterator>(_min,_max);
  }

  /// Addition of the 2 matrix.
  NumericArray1<T>& operator+=(const NumericArray1<T>&a){
      GEOM_ASSERT(a.size() == size());
      typename NumericArray1<T>::const_iterator _i1 = a.begin();
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end();
          _i2++){
          *_i2 += *_i1;
          _i1++;
      }
      return *this;
  }

  /// Addition of the 2 arrays.
  NumericArray1<T> operator+( const NumericArray1<T>& m ) const {
      NumericArray1<T> sum(*this);
      return sum+=m;
  }

  /// Minus operation of 2 arrays.
  NumericArray1<T>& operator-=(const NumericArray1<T>& a){
      GEOM_ASSERT(a.size() == getSize());
      typename NumericArray1<T>::const_iterator _i1 = a.begin();
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end(); _i2++){
          *_i2 -= *_i1;
          _i1++;
      }
      return *this;
  }

  /// Subtraction of 2 arrays.
  NumericArray1<T> operator-(const NumericArray1<T>& m) const {
      NumericArray1<T> sum(*this);
      return sum-=m;
  }

  /// Addition of an array and a value.
  NumericArray1<T>& operator+=(T val){
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end(); _i2++){
          *_i2 += val;
      }
      return *this;
  }


  /// Addition of an array and a value.
  NumericArray1<T>operator+( T val ) const {
      NumericArray1<T> sum(*this);
      return sum+=val;
  }
  /// Subtraction of an array and a value.
  NumericArray1<T>& operator -= ( T val ){
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end(); _i2++){
          *_i2 -= val;
      }
      return *this;
  }

  /// Subtraction of an array and a value.
  NumericArray1<T>operator - ( T val ) const {
      NumericArray1<T> sum(*this);
      return sum-=val;
  }

  /// Multiplication of an array and a value.
  NumericArray1<T>& operator *= (T val){
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end(); _i2++){
          *_i2 *= val;
      }
      return *this;
  }

  /// Multiplication of an array and a value.
  NumericArray1<T>operator * ( T val ) const {
      NumericArray1<T> sum(*this);
      return sum*=val;
  }

  /// Division of an array and a value.
  NumericArray1<T>& operator /= (T val){
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end(); _i2++){
          *_i2 /= val;
      }
      return *this;
  }

  /// Division of an array and a value.
  NumericArray1<T>operator /( T val ) const {
      NumericArray1<T> sum(*this);
      return sum/=val;
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
typedef NumericArray1<int16_t> Int16Array1;
/// Array of int16_t pointer
typedef RCPtr<Int16Array1> Int16Array1Ptr;
/// Array of int32_t
typedef NumericArray1<int32_t> Int32Array1;
/// Array of int32_t pointer
typedef RCPtr<Int32Array1> Int32Array1Ptr;
/// Array of int_t
typedef NumericArray1<int_t> IntArray1;
/// Array of int_t pointer
typedef RCPtr<IntArray1> IntArray1Ptr;
/// Array of uchar
typedef NumericArray1<uchar_t> UCharArray1;
/// Array of uchar_t pointer
typedef RCPtr<UCharArray1> UCharArray1Ptr;
/// Array of uint16_t
typedef NumericArray1<uint16_t> Uint16Array1;
/// Array of uint16_t pointer
typedef RCPtr<Uint16Array1> Uint16Array1Ptr;
/// Array of uint_t
typedef NumericArray1<uint32_t> Uint32Array1;
/// Array of uint_t pointer
typedef RCPtr<Uint32Array1> Uint32Array1Ptr;
/// Array of uint_t
typedef NumericArray1<uint_t> UintArray1;
/// Array of uint_t pointer
typedef RCPtr<UintArray1> UintArray1Ptr;

// typedef Array1<real_t> RealArray1;
// typedef RCPtr<RealArray1> RealArray1Ptr;
// typedef Array1<string> StringArray1;
// typedef RCPtr<StringArray1> StringArray1Ptr;


TOOLS_DECLARE_TYPE(Uint32Array1)
TOOLS_DECLARE_TYPE(Int32Array1)
TOOLS_DECLARE_TYPE(Uint16Array1)
TOOLS_DECLARE_TYPE(Int16Array1)
TOOLS_DECLARE_TYPE(UCharArray1)
TOOLS_DECLARE_TYPE(CharArray1)
TOOLS_DECLARE_TYPE(BoolArray1)

/*  --------------------------------------------------------------------- */

// GEOM_TEMPLATE_API(Array1<real_t>)

class RealArray;
/// Real Array Pointer
typedef RCPtr<RealArray> RealArrayPtr;

/**
   \class RealArray
   \brief A mono dimensional array of real of non fixed size.
*/
class TOOLS_API RealArray : public NumericArray1<real_t>
{

public:

  /// Constructs an Array1 of size \e size
  RealArray( uint_t size = 0 ) ;

  /// Constructs an Array1 with \e size copies of \e t.
  RealArray(uint_t size, const real_t& t  ) ;

  /// Constructs an Array1 with the range [\e begin, \e end).
  template <class InIterator>
  RealArray(InIterator begin, InIterator end ) :
    NumericArray1<real_t>( begin, end ) {
  }

  /// Destructor
  virtual ~RealArray( ); 

  RealArrayPtr log() const { 
      RealArrayPtr result(new RealArray(*this));
      for(iterator it = result->begin(); it != result->end(); ++it) *it = ::log(*it);
      return result;
  }

  RealArrayPtr log(real_t base ) const { 
      RealArrayPtr result(new RealArray(*this));
      float logbase = ::log(base);
      for(iterator it = result->begin(); it != result->end(); ++it) *it = ::log(*it) / logbase;
      return result;
  }

  RealArrayPtr pow(real_t exponent) const { 
      RealArrayPtr result(new RealArray(*this));
      for(iterator it = result->begin(); it != result->end(); ++it) *it = ::pow(*it,exponent);
      return result;
  }

  RealArrayPtr exp() const { 
      RealArrayPtr result(new RealArray(*this));
      for(iterator it = result->begin(); it != result->end(); ++it) *it = ::exp(*it);
      return result;
  }

  RealArrayPtr sqrt() const { 
      RealArrayPtr result(new RealArray(*this));
      for(iterator it = result->begin(); it != result->end(); ++it) *it = ::sqrt(*it);
      return result;
  }

  /// Addition of the 2 matrix.
  RealArray& operator+=(const RealArray&a){
      GEOM_ASSERT(a.size() == getSize());
      RealArray::const_iterator _i1 = a.begin();
      for(iterator  _i2 = __A.begin();  _i2 != __A.end();  _i2++){
          *_i2 += *_i1;
          _i1++;
      }
      return *this;
  }

  /// Addition of the 2 arrays.
  RealArray operator+( const RealArray& m ) const {
      RealArray sum(*this);
      return sum+=m;
  }

  /// Minus operation of 2 arrays.
  RealArray& operator-=(const RealArray& a){
      GEOM_ASSERT(a.size() == size());

      const_iterator _i1 = a.begin();
      for(iterator  _i2 = __A.begin(); _i2 != __A.end(); _i2++){
          *_i2 -= *_i1;
          _i1++;
      }
      return *this;
  }

  /// Subtraction of 2 arrays.
  RealArray operator-(const RealArray& m) const {
      RealArray sum(*this);
      return sum-=m;
  }

  /// Addition of an array and a value.
  RealArray& operator+=(real_t val){
      for(iterator  _i2 = __A.begin();  _i2 != __A.end(); _i2++){
          *_i2 += val;
      }
      return *this;
  }


  /// Addition of an array and a value.
  RealArray operator+(real_t val ) const {
      RealArray sum(*this);
      return sum+=val;
  }
  /// Subtraction of an array and a value.
  RealArray& operator -= (real_t val ){
      for(iterator  _i2 = __A.begin();  _i2 != __A.end(); _i2++){
          *_i2 -= val;
      }
      return *this;
  }

  /// Subtraction of an array and a value.
  RealArray operator - (real_t val ) const {
      RealArray sum(*this);
      return sum-=val;
  }

  /// Multiplication of an array and a value.
  RealArray& operator *= (real_t val){
      for(iterator  _i2 = __A.begin();  _i2 != __A.end(); _i2++){
          *_i2 *= val;
      }
      return *this;
  }

  /// Multiplication of an array and a value.
  RealArray operator * (real_t val ) const {
      RealArray sum(*this);
      return sum*=val;
  }

  /// Division of an array and a value.
  RealArray& operator /= (real_t val){
      for(iterator  _i2 =__A.begin(); _i2 != __A.end(); _i2++){
          *_i2 /= val;
      }
      return *this;
  }

  /// Division of an array and a value.
  RealArray operator /(real_t val ) const {
      RealArray sum(*this);
      return sum/=val;
  }
};

/// Real Array Pointer
typedef RCPtr<RealArray> RealArrayPtr;
TOOLS_DECLARE_TYPE(RealArray)

template<class NumericArray>
Uint32Array1Ptr histogram(RCPtr<NumericArray> values, uint32_t nbbins, 
                          typename NumericArray::element_type& minvalue, 
                          typename NumericArray::element_type& maxvalue, 
                          real_t& binrange)
{
    typedef typename NumericArray::element_type ValueType;
    typedef typename NumericArray::const_iterator ValueConstIterator;
    Uint32Array1Ptr result(new Uint32Array1(nbbins,0));
    std::pair<ValueConstIterator,ValueConstIterator> minmax = values->getMinAndMax();
    minvalue = *minmax.first;
    maxvalue = *minmax.second;
    binrange = (*minmax.second - minvalue) / real_t(nbbins);
    for(ValueConstIterator itVal = values->begin(); itVal != values->end(); ++itVal)
    {
        uint32_t binid = std::min(nbbins-1,uint32_t((*itVal-minvalue)/binrange));
        result->setAt(binid,result->getAt(binid)+1);
    }
    return result;


}


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

