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

/*! \file util_array2.h
    \brief Definition of the container class Array2 and RealArray2.
*/


#ifndef __util_array2_h__
#define __util_array2_h__

/* ----------------------------------------------------------------------- */

#include "rcobject.h"
#include <vector>
#include <algorithm>

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Array2
   \brief A bi dimnensionnal array of non fixed size.
*/

/* ----------------------------------------------------------------------- */

template <class T>
class Array2 : public RefCountObject
{

public:

  /// A const iterator used to iterate through an Array2.
  typedef typename std::vector<T>::const_iterator const_iterator;

  /// An iterator used to iterate through an Array2.
  typedef typename std::vector<T>::iterator iterator;

  /// The element type.
  typedef T element_type;

  /// Constructs an Array2 with \e col columns and \e row rows
  Array2( uint32_t row = 0, uint32_t col = 0 ) :
    RefCountObject(),
      __A(col*row),
      __rowsNb(row)
      {
 }

  /// Constructs an Array2 with \e size copies of \e t.
  Array2( uint32_t row, uint32_t col, const T& t ) :
    RefCountObject(),
      __A((unsigned int)(col*row),t),
      __rowsNb(row)
      {
  }

  /// Constructs an Array2 with the range [\e begin, \e end).
  template <class InIterator>
  Array2( InIterator begin, InIterator end, uint32_t rowsNb ) :
    RefCountObject(),
      __A(begin,end),
      __rowsNb(rowsNb)
      {
  }

  /// Destructor
  virtual ~Array2( ) {
  }

  /// assignement operator.
  Array2<T>& operator=(const Array2<T>& m){
    __A = std::vector<T>(m.__A);
    __rowsNb = m.getRowsNb();
    return *this;
  }

  /// Changes the matrix dimensions.
  virtual void resize(const uint32_t nr, const uint32_t nc){
      __A = std::vector<T> (nr * nc);
      __rowsNb = nr;
  }

  /// Returns whether \e self contain \e t.
  inline bool contain( const T& t ) const {
	  return std::find(__A.begin(),__A.end(),t) != __A.end();
  }

  /** Returns the value of the \e c-th element of \e r row of \e self
      \pre
      - \e r must be strictly less than the number of rows of \e self.
      - \e c must be strictly less than the number of columns of \e self. */
  inline const T& getAt( uint32_t r, uint32_t c ) const {
    GEOM_ASSERT(__A.size() !=0 && r < __rowsNb&& c <(__A.size()/__rowsNb) );
    return __A[((r*(__A.size()/__rowsNb))+c)];
  }

  /** Returns the \e c-th element of \e r row of \e self
      \pre
      - \e r must be strictly less than the number of rows of \e self.
      - \e c must be strictly less than the number of columns of \e self. */
  inline T& getAt( uint32_t r, uint32_t c ) {
    GEOM_ASSERT(__A.size() !=0 && r < __rowsNb&& c <(__A.size()/__rowsNb) );
    return __A[((r*(__A.size()/__rowsNb))+c)];
  }

  /** Returns the \e j-th row of \e self
      \pre
      - \e j must be strictly less than the number of the rows of \e self. */
  inline std::vector<T> getRow( uint32_t r) const {
    GEOM_ASSERT( r < __rowsNb);
    return std::vector<T>(__A.begin()+(r*(__A.size()/__rowsNb)),__A.begin()+((r+1)*(__A.size()/__rowsNb)));
  }

  /** Returns the \e i-th column of \e self
      \pre
      - \e j must be strictly less than the dimension of the columns of \e self. */
  inline std::vector<T> getColumn( uint32_t c) const {
    GEOM_ASSERT( __rowsNb!= 0 && c < (__A.size()/__rowsNb) );
    std::vector<T> _col(__rowsNb);
    const_iterator _it = __A.begin()+c;
    for ( uint32_t k = 0 ; k < __rowsNb ; k++){
        _col[k]=*_it;
        _it+=(__A.size()/__rowsNb);
    }
    return _col;
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

  /// Returns a const iterator at the beginning of the row \e row of\e self.
  inline const_iterator getBeginRow(uint32_t row ) const {
      GEOM_ASSERT(__rowsNb!= 0 );
    return (__A.begin()+(row*(__A.size()/__rowsNb)));
  }

  /// Returns an iterator at the beginning of \e self.
  inline iterator getBeginRow(uint32_t row ) {
      GEOM_ASSERT(__rowsNb!= 0 );
    return (__A.begin()+(row*(__A.size()/__rowsNb)));
  }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator getEndRow(uint32_t row ) const {
      GEOM_ASSERT(__rowsNb!= 0 );
    return (__A.begin()+((row+1)*(__A.size()/__rowsNb)));
  }

  /// Returns a const iterator at the end of \e self.
  inline iterator getEndRow(uint32_t row ) {
      GEOM_ASSERT(__rowsNb!= 0 );
    return (__A.begin()+((row+1)*(__A.size()/__rowsNb)));
  }

  /// Returns the size of \e self.
  inline uint32_t getSize( ) const {
    return __A.size();
  }

  /// Returns the size of each rows of \e self.
  inline uint32_t getRowsSize( ) const {
    if(__rowsNb != 0)return (__A.size()/__rowsNb);
    else return uint32_t(0);
  }

  /// Returns the size of each cols \e self.
  inline uint32_t getColsSize( ) const {
    return __rowsNb;
  }

  /// Returns the number of rows of \e self.
  inline uint32_t getRowsNb( ) const {
    return __rowsNb;
  }

  /// Returns the number of columns \e self.7
  inline uint32_t getColsNb( ) const {
    if(__rowsNb != 0)return (__A.size()/__rowsNb);
    else return uint32_t(0);
  }

  /* Insert \e t into \e self before the position pointed by \e it.
  iterator insert( iterator it, const T& t ) {
    return __A.insert(it,t);
  }*/

  /// Insert \e t into \e self before the position pointed by \e i.
  inline void insertColumn( uint32_t i, const std::vector<T>& t ) {
      GEOM_ASSERT( (__rowsNb ==0 && i == 0)|| i <=(__A.size()/__rowsNb));
      GEOM_ASSERT(__rowsNb ==0 || t.size() == __rowsNb);
      uint32_t _cols = (__A.size()/__rowsNb);
      iterator _pos = (__A.begin()+i);
      for(const_iterator _k = t.begin(); _k != t.end(); _k++){
          __A.insert(_pos,(*_k));
           _pos=_pos+_cols;
       };
      if(!__rowsNb)__rowsNb = t.size();
  }

  /// Insert the line \e t before the line \e i.
  inline void insertLine( uint32_t i, const std::vector<T>& t ) {
      GEOM_ASSERT( i <= __rowsNb);
      GEOM_ASSERT( (i == 0 && __rowsNb == 0) || (t.size() == (__A.size()/__rowsNb)));
      iterator _pos;
      if(!__rowsNb) _pos = this->getBegin();
      else _pos = (this->getBegin()+(i*(this->getSize()/__rowsNb)));
       const_iterator _f = (t.begin());
       const_iterator _g = (t.end());
       __A.insert(_pos,_f,_g);
      __rowsNb++;
  }

  /// Insert from \e begin to \e end into \e self before the position pointed by \e i.
  template <class InIterator>
  inline void insertColumn( uint32_t i, InIterator begin, InIterator end) {
      GEOM_ASSERT(  (i == 0 && __rowsNb == 0) || (i <=(__A.size()/__rowsNb)) );
      GEOM_ASSERT(distance(begin,end) ==  __rowsNb);
      typename std::vector<T>::iterator _pos = (__A.begin()+i);
      uint32_t _cols = (__A.size()/__rowsNb);
      for(InIterator _k = begin; _k != end; _k++){
           __A.insert(_pos,(*_k));
           _pos=_pos+_cols;
       };
  }

  /// Inserts the line \e t before the line \e j.

  template <class InIterator>
  inline void insertLine( uint32_t j,InIterator begin, InIterator end ) {
      GEOM_ASSERT( j <= __rowsNb);
      GEOM_ASSERT( __rowsNb == 0 || distance(begin,end) == (__A.size()/__rowsNb));
      typename std::vector<T>::iterator _pos;
      if(!__rowsNb)_pos = __A.begin();
      else _pos = (__A.begin()+(j*(__A.size()/__rowsNb)));
      __A.insert(_pos, begin,end);
      __rowsNb++;
  }

  /// Inserts the line \e t at the end.

  template <class InIterator>
  inline void pushRow(InIterator begin, InIterator end ) 
    { 
	if(__rowsNb != 0)
	  {
      GEOM_ASSERT( distance(begin,end) == (__A.size() /__rowsNb) );
      }
    __A.insert(__A.end(),begin,end);
    __rowsNb++;
    }

  /// Returns whether \e self is empty.
  inline bool isEmpty( ) {
    return __A.empty();
  }

  /// Returns whether \e self contain unique elements.
  bool isUnique( ) const {
    if (__A.empty()) return true;
    typename std::vector<T>::const_iterator _first = __A.begin();
    typename std::vector<T>::const_iterator _last = __A.end() - 1;
    while (_first != _last) {
      if (find(_first + 1,__A.end(),*_first) != __A.end()) return false;
      _first++;
    };
    return true;
  }

  /// Prints \e self to the output stream \e stream.
/*  std::ostream& print( std::ostream& stream,
                  char delimiter = ',',
                  char begin = '[',
                  char end = ']' ) const {
      stream << begin << ' ' << begin << ' ';
      uint32_t _size = __A.size();
      if (_size) {
          uint32_t _colsNb = _size / __rowsNb;
          stream  << __A[0];
          for (uint32_t _i = 1; _i < _size - 1; _i++){
              if((_i % _colsNb) == 0)stream << ' ' << end << ' '
                                            << delimiter << ' ' << begin;
              else stream  << ' '<< delimiter;
              stream << ' ' << __A[_i];
          }
          if(_colsNb != 1)stream  << ' '<< delimiter;
          if(_size != 1)stream << ' ' << __A[_size -1];
      };
      return (stream << ' ' << end << ' ' << end);
  }

  /// Prints \e self to the output stream \e stream.
  friend std::ostream& operator<<(std::ostream& stream, const Array2<T> a){
      return a.print(stream);
  }
*/

  /** Sets the \e (r,c)-th element of \e self to \e t.
      \pre
      - \e r must be strictly less than the number of rows of \e self.
      - \e c must be strictly less than the number of columns of \e self. */
  void setAt( uint32_t r, uint32_t c, const T& t ) {
      GEOM_ASSERT(r < __rowsNb&& c <(__A.size()/__rowsNb) );
      __A[((r*(__A.size()/__rowsNb))+c)] = t;
  }

  /** returns the matrix of size \e (nr,nc) starting at \e (rw,cl).
      \params
      - rw the index of the row
      - cl the index of the column
      - nr the number of rows() of the matrix to generate
      - nc the number of coluns of the matrix to generate
      \return
      - the matrix of size (nr,nc) starting at index (rw,cl).
      \pre
      - The matrix to return must fit inside the original matrix.
  **/
  Array2<T> get( uint32_t rw, uint32_t cl,
                   uint32_t nr, uint32_t nc) const {
    if(rw + nr > __rowsNb || cl + nc > getColsNb( ))
      return Array2<T>(0,0);
    Array2<T> result(nr,nc);
    for(uint32_t _i = 0; _i < nr ; _i++){
      typename std::vector<T>::const_iterator _it = __A.begin()+((_i+rw)*(__A.size()/__rowsNb) + cl);
      typename std::vector<T>::const_iterator _itend = _it + nc;
      uint32_t _j =0;
      for(;_it != _itend; _it++){
        result.setAt(_i,_j,*_it);
        _j++;
      }
    }
    return result;
  }

  /// returns the transpose of the matrix.
  friend Array2<T> transpose( const Array2<T>& m ) {
      Array2<T> n(m.getColsNb(),m.getRowsNb());
      uint32_t _i = 0, _j = 0;
      for(typename Array2<T>::const_iterator _it = m.getBegin();
          _it != m.getEnd();_it++){
          n.setAt(_i,_j,*_it);
          _i++;
          if(_i % n.__rowsNb == 0){ _i = 0; _j++; }
      }
      return n;
  }

  /// Set \e val to the diadonal of \e self.
  inline void setDiagonal(const T& val) {
	  uint32_t s = std::min<uint32_t>(getRowsNb(),getColsNb());
      for(uint32_t _i = 0; _i < s ; _i++)
          setAt(_i,_i,val);
  }

  /// Get the Diagonal of \e self.
  inline std::vector<T> getDiagonal() const {
	  uint32_t s = std::min<uint32_t>(getRowsNb(),getColsNb());
      std::vector<T> diag(s);
      for(uint32_t _i = 0; _i < s ; _i++)
          diag[_i] = getAt(_i,_i);
	  return diag;
  }


protected:

  /// The elements contained by \e self.
  std::vector<T> __A;

  /// The number of row of \e self.
  uint32_t __rowsNb;
};


template <class T>
class NumericArray2 : public Array2<T>
{

public:
  typedef typename Array2<T>::const_iterator const_iterator;

  /// Constructs an Array2 with \e col columns and \e row rows
  NumericArray2( uint32_t row = 0, uint32_t col = 0 ) :
	  Array2<T>( row, col )
      {
	 }

  /// Constructs an Array2 with \e size copies of \e t.
  NumericArray2( uint32_t row, uint32_t col, const T& t ) :
    Array2<T>( row, col, t )
      {
  }

  /// Constructs an Array2 with the range [\e begin, \e end).
  template <class InIterator>
  NumericArray2( InIterator begin, InIterator end, uint32_t rowsNb ) :
	Array2<T>(begin,end,rowsNb)
      {
   }

  /// Destructor
  virtual ~NumericArray2( ) {
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
 inline std::pair<const_iterator,const_iterator> getMinAndMax( ) const {
    const_iterator _min = this->__A.begin();
    const_iterator _max = this->__A.begin();
    for (const_iterator _i = this->__A.begin() + 1; _i != this->__A.end(); _i++)
      if (*_i < *_min) _min = _i;
      else if (*_i != *_max) _max = _i;
    return std::pair<const_iterator,const_iterator>(_min,_max);
  }

  /** returns the matrix of size \e (nr,nc) starting at \e (rw,cl).
      \params
      - rw the index of the row
      - cl the index of the column
      - nr the number of rows() of the matrix to generate
      - nc the number of coluns of the matrix to generate
      \return
      - the matrix of size (nr,nc) starting at index (rw,cl).
      \pre
      - The matrix to return must fit inside the original matrix.
  **/
  NumericArray2<T> get( uint32_t rw, uint32_t cl,
                   uint32_t nr, uint32_t nc) const {
    if(rw + nr > this->__rowsNb || cl + nc > this->getColsNb( ))
      return NumericArray2<T>(0,0);
    NumericArray2<T> result(nr,nc);
    for(uint32_t _i = 0; _i < nr ; _i++){
      typename std::vector<T>::const_iterator _it = this->__A.begin()+((_i+rw)*(this->__A.size()/this->__rowsNb) + cl);
      typename std::vector<T>::const_iterator _itend = _it + nc;
      uint32_t _j =0;
      for(;_it != _itend; _it++){
        result.setAt(_i,_j,*_it);
        _j++;
      }
    }
    return result;
  }

  /// returns the transpose of the matrix.
  friend NumericArray2<T> transpose( const NumericArray2<T>& m ) {
      NumericArray2<T> n(m.getColsNb(),m.getRowsNb());
      uint32_t _i = 0, _j = 0;
      for(typename NumericArray2<T>::const_iterator _it = m.getBegin();
          _it != m.getEnd();_it++){
          n.setAt(_i,_j,*_it);
          _i++;
          if(_i % n.__rowsNb == 0){ _i = 0; _j++; }
      }
      return n;
  }
  /// Multiplication of 2 matrix.
  NumericArray2<T> operator*( const NumericArray2<T>& m ) const {
      if(this->getColsNb() != m.getRowsNb())
          return NumericArray2<T>(0,0);
      NumericArray2<T> result(this->getRowsNb(),m.getColsNb());
      for(uint32_t _i = 0; _i < this->getRowsNb(); _i++){
          for(uint32_t _j = 0; _j < m.getColsNb(); _j++){
              typename NumericArray2<T>::const_iterator _it1 = this->getBeginRow(_i);
              typename NumericArray2<T>::const_iterator _it2 = m.getBegin() + _j;
              T sum = (*_it1)*(*_it2);
              while(_it1 != this->getEndRow(_i)-1){
                  _it1++;
                  _it2+=m.getColsNb();
                  sum += (*_it1)*(*_it2);
              };
              result.setAt(_i,_j,sum);
          }
      }
      return result;
  }
  /// Addition of the 2 matrix.
  NumericArray2<T>& operator+=(const NumericArray2<T>&a){
      GEOM_ASSERT(a.getRowsNb()!=getRowsNb() && a.getSize()!=getSize());
      typename NumericArray2<T>::const_iterator _i1 = a.getBegin();
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end();
          _i2++){
          *_i2 += *_i1;
          _i1++;
      }
      return *this;
  }

  /// Addition of the 2 matrix.
  NumericArray2<T> operator+( const NumericArray2<T>& m ) const {
      NumericArray2<T> sum(*this);
      return sum+=m;
  }

  /// Minus operation of 2 matrix.
  NumericArray2<T>& operator-=(const NumericArray2<T>& a){
      GEOM_ASSERT(a.getRowsNb()!=getRowsNb() && a.getSize()!=getSize());
      typename NumericArray2<T>::const_iterator _i1 = a.getBegin();
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end(); _i2++){
          *_i2 -= *_i1;
          _i1++;
      }
      return *this;
  }

  /// Subtraction of 2 matrix.
  NumericArray2<T> operator-(const NumericArray2<T>& m) const {
      NumericArray2<T> sum(*this);
      return sum-=m;
  }


  /// Addition of a matrix and a value.
  NumericArray2<T>& operator+=(T val){
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end(); _i2++){
          *_i2 -= val;
      }
      return *this;
  }


  /// Addition of a matrix and a value.
  NumericArray2<T>operator+( T val ) const {
      NumericArray2<T> sum(*this);
      return sum+=val;
  }


  /// Subtraction of a matrix and a value.
  NumericArray2<T>& operator -= ( T val ){
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end(); _i2++){
          *_i2 -= val;
      }
      return *this;
  }

  /// Subtraction of a matrix and a value.
  NumericArray2<T>operator - ( T val ) const {
      NumericArray2<T> sum(*this);
      return sum-=val;
  }

  /// Multiplication of a matrix and a value.
  NumericArray2<T>& operator *= (T val){
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end(); _i2++){
          *_i2 *= val;
      }
      return *this;
  }

  /// Multiplication of a matrix and a value.
  NumericArray2<T>operator * ( T val ) const {
      NumericArray2<T> sum(*this);
      return sum*=val;
  }

  /// Division of a matrix and a value.
  NumericArray2<T>& operator /= (T val){
      for(typename std::vector<T>::iterator  _i2 = this->__A.begin();
          _i2 != this->__A.end(); _i2++){
          *_i2 /= val;
      }
      return *this;
  }

  /// Division of a matrix and a value.
  NumericArray2<T>operator /( T val ) const {
      NumericArray2<T> sum(*this);
      return sum/=val;
  }

};

/// Array of dim 2 of bool
typedef NumericArray2<bool> BoolArray2;
/// Pointer on Array of dim 2 of bool
typedef RCPtr<BoolArray2> BoolArray2Ptr;
/// Array of dim 2 of char
typedef NumericArray2<char> CharArray2;
/// Pointer on Array of dim 2 of char
typedef RCPtr<CharArray2> CharArray2Ptr;
/// Array of dim 2 of int16_t
typedef NumericArray2<int16_t> Int16Array2;
/// Pointer on Array of dim 2 of int16_t
typedef RCPtr<Int16Array2> Int16Array2Ptr;
/// Array of dim 2 of int32_t
typedef NumericArray2<int32_t> Int32Array2;
/// Pointer on Array of dim 2 of int32_t
typedef RCPtr<Int32Array2> Int32Array2Ptr;
/// Array of dim 2 of uchar
typedef NumericArray2<uchar_t> UCharArray2;
/// Pointer on Array of dim 2 of uchar
typedef RCPtr<UCharArray2> UCharArray2Ptr;
/// Array of dim 2 of uint16_t
typedef NumericArray2<uint16_t> Uint16Array2;
/// Pointer on Array of dim 2 of uint16_t
typedef RCPtr<Uint16Array2> Uint16Array2Ptr;
/// Array of dim 2 of uint32_t
typedef NumericArray2<uint32_t> Uint32Array2;
/// Pointer on Array of dim 2 of uint32_t
typedef RCPtr<Uint32Array2> Uint32Array2Ptr;

// typedef Array2<string> StringArray2;
// typedef RCPtr<StringArray2> StringArray2Ptr;

/// Array of dim 2 of double
typedef NumericArray2<double> DoubleArray2;
/// Pointer on Array of dim 2 of double
typedef RCPtr<DoubleArray2> DoubleArray2Ptr;
/// Array of dim 2 of float
typedef NumericArray2<float> FloatArray2;
/// Pointer on Array of dim 2 of uint32_t
typedef RCPtr<FloatArray2> FloatArray2Ptr;

/*  --------------------------------------------------------------------- */

/**
   \class RealArray2
   \brief A bi dimnensionnal array of real of non fixed size.
*/

class TOOLS_API RealArray2 : public NumericArray2<real_t>
{

public:

  /// Constructs an Array2 with \e col columns and \e row rows
  RealArray2( uint32_t row = 0, uint32_t col = 0 );

  /// Constructs an Array2 with \e size copies of \e t.
  RealArray2( uint32_t row, uint32_t col, const real_t& t ) ;

  /// Constructs an Array2 with the range [\e begin, \e end).
  template <class InIterator>
  RealArray2( InIterator begin, InIterator end, uint32_t rowsNb ) :
   NumericArray2<real_t>( begin, end, rowsNb ) {
  }

  /// Destructor
  virtual ~RealArray2( ) ;

};

// typedef Array2<real_t> RealArray2;
/// Pointer on array of dim 2 of real
typedef RCPtr<RealArray2> RealArray2Ptr;

/*  --------------------------------------------------------------------- */
// __util_array2_h__
/* ----------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

/// Write Array2 \b a on \b stream
  template< class T >
  std::ostream& operator<<(std::ostream& stream, const TOOLS(Array2<T>) a){
      return a.print(stream);
  }

/* ----------------------------------------------------------------------- */
#endif

