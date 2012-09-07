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

/*! \file util_array2.h
    \brief Definition of the container class Array2 and RealArray2.
*/


#ifndef __util_array2_h__
#define __util_array2_h__

/* ----------------------------------------------------------------------- */

#include "rcobject.h"
#include "tools_config.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "classinfo.h"
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
  Array2( uint_t row = 0, uint_t col = 0 ) :
    RefCountObject(),
      __A(col*row),
      __rowSize(col)
      {
 }

  /// Constructs an Array2 with \e size copies of \e t.
  Array2( uint_t row, uint_t col, const T& t ) :
    RefCountObject(),
      __A((unsigned int)(col*row),t),
      __rowSize(col)
      {
  }

  /// Constructs an Array2 with the range [\e begin, \e end).
  template <class InIterator>
  Array2( InIterator begin, InIterator end, uint_t rowsSize ) :
    RefCountObject(),
      __A(begin,end),
      __rowSize(rowsSize)
      {
  }

  /// Destructor
  virtual ~Array2( ) {
  }

  /// assignement operator.
  Array2<T>& operator=(const Array2<T>& m){
    __A = std::vector<T>(m.__A);
    __rowSize = m.getRowSize();
    return *this;
  }

  /// Changes the matrix dimensions.
  virtual void resize(const uint_t nr, const uint_t nc){
      __A = std::vector<T> (nr * nc);
      __rowSize = nc;
  }

  /// Changes the matrix dimensions.
  virtual void reshape(const uint_t nr, const uint_t nc){
      assert(__A.size() == nr*nc);
      __rowSize = nc;
  }

  /// Returns whether \e self contain \e t.
  inline bool contains( const T& t ) const {
      return std::find(__A.begin(),__A.end(),t) != __A.end();
  }

  /** Returns the value of the \e c-th element of \e r row of \e self
      \pre
      - \e r must be strictly less than the number of rows of \e self.
      - \e c must be strictly less than the number of columns of \e self. */
  inline const T& getAt( uint_t r, uint_t c ) const {
    GEOM_ASSERT(__A.size() !=0 && r < getRowNb() && c < getColumnNb() );
    return __A[(r*getRowSize())+c];
  }

  /** Returns the \e c-th element of \e r row of \e self
      \pre
      - \e r must be strictly less than the number of rows of \e self.
      - \e c must be strictly less than the number of columns of \e self. */
  inline T& getAt( uint_t r, uint_t c ) {
    GEOM_ASSERT(__A.size() !=0 && r < getRowNb() && c < getColumnNb() );
    return __A[(r*getRowSize())+c];
  }

  /** Returns the \e j-th row of \e self
      \pre
      - \e j must be strictly less than the number of the rows of \e self. */
  inline std::vector<T> getRow( uint_t r) const {
    GEOM_ASSERT( r < getRowNb());
    return std::vector<T>(__A.begin()+(r*getRowSize()),__A.begin()+((r+1)*getRowSize()));
  }

  /** Returns the \e i-th column of \e self
      \pre
      - \e j must be strictly less than the dimension of the columns of \e self. */
  inline std::vector<T> getColumn( uint_t c) const {
    GEOM_ASSERT( c < getColumnNb() );
    std::vector<T> _col(getRowNb());
    if (getRowNb() > 0) {
        const_iterator _it = __A.begin()+c;
        _col[0]=*_it;
        for ( uint_t k = 1 ; k < getRowNb() ; k++){
            _it+= getRowSize();
            _col[k]=*_it;
        }
    }
    return _col;
  }

  /// Returns the size of \e self.
  inline uint_t size( ) const { return __A.size(); }

  /// Returns whether \e self is empty.
  inline bool empty( ) const { return __A.empty(); }

  /// Clear \e self.
  inline void clear( ) { __A.clear(); __rowSize = 0; }

  /// Returns a const iterator at the beginning of \e self.
  inline const_iterator begin( ) const { return __A.begin(); }

  /// Returns an iterator at the beginning of \e self.
  inline iterator begin( ) { return __A.begin(); }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator end( ) const { return __A.end(); }

  /// Returns a const iterator at the end of \e self.
  inline iterator end( ) { return __A.end(); }

  /// Returns a const iterator at the beginning of the row \e row of\e self.
  inline const_iterator beginRow(uint_t row ) const {
      return (__A.begin()+(row*getRowSize())); 
  }

  /// Returns an iterator at the beginning of \e self.
  inline iterator beginRow(uint_t row ) {
      return (__A.begin()+(row*getRowSize()));
  }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator endRow(uint_t row ) const {
      return (__A.begin()+((row+1)*getRowSize()));
  }

  /// Returns a const iterator at the end of \e self.
  inline iterator endRow( uint_t row ) {
      return (__A.begin()+((row+1)*getRowSize()));
  }

  /// Returns the size of each rows of \e self.
  inline attribute_deprecated uint_t getRowsSize( ) const { return getRowSize(); }
  inline uint_t getRowSize( ) const { return __rowSize;  }

  /// Returns the size of each cols \e self.
  inline attribute_deprecated uint_t getColsSize( ) const { return getColumnSize(); }
  inline uint_t getColumnSize( ) const {
    if(__rowSize != 0) return (__A.size() / __rowSize);
    else return uint_t(0);
  }

  /// Returns the number of rows of \e self.
  inline attribute_deprecated uint_t getRowsNb( ) const { return getRowNb(); }
  inline uint_t getRowNb( ) const { return getColumnSize(); }

  /// Returns the number of columns \e self.
  inline attribute_deprecated uint_t getColsNb( ) const { return getColumnNb(); }
  inline uint_t getColumnNb( ) const { return __rowSize;  }


  /// Insert the line \e t before the line \e i.
  inline void insertRow( uint_t i, const std::vector<T>& t ) {
      insertRow(i,t.begin(),t.end());
  }

  /// Inserts the line \e t before the line \e j.
  template <class InIterator>
  inline void insertRow( uint_t j,InIterator begin, InIterator end ) {
      GEOM_ASSERT( j <= getRowNb());
      size_t nrsize = distance(begin,end);
      GEOM_ASSERT((j == 0 && __rowSize == 0) || (nrsize == getRowSize()));
      iterator _pos;
      if(!__rowSize) { _pos = __A.begin(); __rowSize = nrsize; }
      else _pos = beginRow(j); 
      __A.insert(_pos, begin,end);
  }

  /// Insert \e t into \e self before the position pointed by \e i.
  inline void insertColumn( uint_t i, const std::vector<T>& t ) {
      insertColumn(i,t.begin(),t.end());
  }

  /// Insert from \e begin to \e end into \e self before the position pointed by \e i.
  template <class InIterator>
  inline void insertColumn( uint_t i, InIterator begin, InIterator end) {
      GEOM_ASSERT(  (i == 0 && getRowSize() == 0) || (i  <= getColumnNb()) );
      GEOM_ASSERT(  getColumnSize() == 0 || distance(begin,end) ==  getColumnSize());
      if (__rowSize == 0) {
          __A.insert(__A.begin(),begin,end);
          __rowSize = 1;
      }
      else {
          __rowSize += 1;
          iterator _pos = __A.begin()+i;
          for(InIterator _k = begin; _k != end; _k++,_pos+=__rowSize){
               _pos = __A.insert(_pos,*_k);
          };
      }
  }

  /// Inserts a row \e t at the end.
  template <class InIterator>
  inline void pushRow(InIterator begin, InIterator end ){
    GEOM_ASSERT( __rowSize == 0 || distance(begin,end) == getRowSize() );
    __A.insert(__A.end(),begin,end);
    if (__rowSize == 0) __rowSize = __A.size();
   }

  /// Inserts a row \e t at the end.
  template <class InIterator>
  inline void pushRow(std::vector<T>& t ){ pushRow(t.begin(),t.end()); }

  /// Inserts a row \e t at the end.
  template <class InIterator>
  inline void pushColumn(InIterator begin, InIterator end ){
      insertColumn(getColumnNb(),begin,end);
   }

  /// Inserts a row \e t at the end.
  template <class InIterator>
  inline void pushColumn(std::vector<T>& t ){ pushColumn(t.begin(),t.end()); }


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

  /** Sets the \e (r,c)-th element of \e self to \e t.
      \pre
      - \e r must be strictly less than the number of rows of \e self.
      - \e c must be strictly less than the number of columns of \e self. */
  void setAt( uint_t r, uint_t c, const T& t ) {
      GEOM_ASSERT(r < getRowNb() && c < getColumnNb() );
      __A[((r*getRowSize())+c)] = t;
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
  Array2<T> get( uint_t rw, uint_t cl,
                   uint_t nr, uint_t nc) const {
    if(rw + nr > getRowNb() || cl + nc > getColumnNb( ))
            return Array2<T>(0,0);
    Array2<T> result(nr,nc);
    for(uint_t _i = 0; _i < nr ; _i++){
      typename std::vector<T>::const_iterator _it = beginRow(_i+rw) + cl;
      typename std::vector<T>::const_iterator _itend = _it + nc;
      uint_t _j =0;
      for(;_it != _itend; _it++){
        result.setAt(_i,_j,*_it);
        _j++;
      }
    }
    return result;
  }

  /// returns the transpose of the matrix.
  friend Array2<T> transpose( const Array2<T>& m ) {
      size_t rownb = m.getRowNb();
      Array2<T> n(m.getColumnNb(),rownb);
      uint_t _i = 0, _j = 0;
      for(typename Array2<T>::const_iterator _it = m.begin();
          _it != m.end();_it++){
          n.setAt(_i,_j,*_it);
          _i++;
          if(_i % rownb == 0){ _i = 0; _j++; }
      }
      return n;
  }

  /// Set \e val to the diagonal of \e self.
  inline void setDiagonal(const T& val) {
      uint_t s = std::min<uint_t>(getRowNb(),getColumnNb());
      for(uint_t _i = 0; _i < s ; _i++)
          setAt(_i,_i,val);
  }

  /// Get the Diagonal of \e self.
  inline std::vector<T> getDiagonal() const {
      uint_t s = std::min<uint_t>(getRowNb(),getColumnNb());
      std::vector<T> diag(s);
      for(uint_t _i = 0; _i < s ; _i++)
          diag[_i] = getAt(_i,_i);
      return diag;
  }

#ifndef PGL_NO_DEPRECATED
  /// Returns a const iterator at the beginning of \e self.
  inline attribute_deprecated const_iterator getBegin( ) const { return begin(); }

  /// Returns an iterator at the beginning of \e self.
  inline attribute_deprecated iterator getBegin( ) { return begin(); }

  /// Returns a const iterator at the beginning of \e self.
  inline attribute_deprecated const_iterator getBeginRow(uint_t row  ) const { return beginRow(row); }

  /// Returns an iterator at the beginning of \e self.
  inline attribute_deprecated iterator getBeginRow(uint_t row  ) { return beginRow(row); }

  /// Returns a const iterator at the end of \e self.
  inline attribute_deprecated const_iterator getEnd( ) const { return end(); }

  /// Returns an iterator at the end of \e self.
  inline attribute_deprecated iterator getEnd( ) { return end(); }

  /// Returns a const iterator at the end of \e self.
  inline attribute_deprecated const_iterator getEndRow(uint_t row ) const { return endRow(row); }

  /// Returns an iterator at the end of \e self.
  inline attribute_deprecated iterator getEndRow(uint_t row ) { return endRow(row); }

  /// Return size of this
  inline attribute_deprecated uint_t getSize() const { return size(); }

  inline attribute_deprecated bool isEmpty() const { return empty(); }

#endif

protected:

  /// The elements contained by \e self.
  std::vector<T> __A;

  /// The number of row of \e self.
  uint_t __rowSize;
};


template <class T>
class NumericArray2 : public Array2<T>
{

public:
  typedef typename Array2<T>::const_iterator const_iterator;

  /// Constructs an Array2 with \e col columns and \e row rows
  NumericArray2( uint_t row = 0, uint_t col = 0 ) :
      Array2<T>( row, col )
      {
     }

  /// Constructs an Array2 with \e size copies of \e t.
  NumericArray2( uint_t row, uint_t col, const T& t ) :
    Array2<T>( row, col, t )
      {
  }

  /// Constructs an Array2 with the range [\e begin, \e end).
  template <class InIterator>
  NumericArray2( InIterator begin, InIterator end, uint_t rowsSize ) :
    Array2<T>(begin,end,rowsSize)
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
      else if (*_i > *_max) _max = _i;
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
  NumericArray2<T> get( uint_t rw, uint_t cl,
                   uint_t nr, uint_t nc) const {
    if(rw + nr > this->getRowNb() || cl + nc > this->getColumnNb( ))
              return NumericArray2<T>(0,0);
    NumericArray2<T> result(nr,nc);
    for(uint_t _i = 0; _i < nr ; _i++){
      typename std::vector<T>::const_iterator _it = this->beginRow(_i+rw) + cl;
      typename std::vector<T>::const_iterator _itend = _it + nc;
      uint_t _j =0;
      for(;_it != _itend; _it++){
        result.setAt(_i,_j,*_it);
        _j++;
      }
    }
    return result;
  }

  /// returns the transpose of the matrix.
  friend NumericArray2<T> transpose( const NumericArray2<T>& m ) {
      size_t rownb = m.getRowNb();
      NumericArray2<T> n(m.getColumnNb(),rownb);
      uint_t _i = 0, _j = 0;
      for(typename NumericArray2<T>::const_iterator _it = m.begin();
          _it != m.end();_it++){
          n.setAt(_i,_j,*_it);
          _i++;
          if(_i % rownb == 0){ _i = 0; _j++; }
      }
      return n;
  }
  /// Multiplication of 2 matrix.
  NumericArray2<T> operator*( const NumericArray2<T>& m ) const {
      if(this->getColumnNb() != m.getRowNb()) return NumericArray2<T>(0,0);
      NumericArray2<T> result(this->getRowNb(),m.getColumnNb());
      for(uint_t _i = 0; _i < this->getRowNb(); _i++){
          for(uint_t _j = 0; _j < m.getColumnNb(); _j++){
              typename NumericArray2<T>::const_iterator _it1 = this->beginRow(_i);
              typename NumericArray2<T>::const_iterator _it2 = m.begin() + _j;
              T sum = (*_it1)*(*_it2);
              while(_it1 != this->endRow(_i)-1){
                  _it1++;
                  _it2+=m.getRowSize();
                  sum += (*_it1)*(*_it2);
              };
              result.setAt(_i,_j,sum);
          }
      }
      return result;
  }
  /// Addition of the 2 matrix.
  NumericArray2<T>& operator+=(const NumericArray2<T>&a){
      GEOM_ASSERT(a.getRowSize()!=getRowSize() && a.size()!=getSize());
      typename NumericArray2<T>::const_iterator _i1 = a.begin();
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
      GEOM_ASSERT(a.getRowSize()!=getRowSize() && a.size()!=getSize());
      typename NumericArray2<T>::const_iterator _i1 = a.begin();
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
/// Array of dim 2 of int_t
typedef NumericArray2<int_t> IntArray2;
/// Pointer on Array of dim 2 of int32_t
typedef RCPtr<IntArray2> IntArray2Ptr;
/// Array of dim 2 of uchar
typedef NumericArray2<uchar_t> UCharArray2;
/// Pointer on Array of dim 2 of uchar
typedef RCPtr<UCharArray2> UCharArray2Ptr;
/// Array of dim 2 of uint16_t
typedef NumericArray2<uint16_t> Uint16Array2;
/// Pointer on Array of dim 2 of uint16_t
typedef RCPtr<Uint16Array2> Uint16Array2Ptr;
/// Array of dim 2 of uint_t
typedef NumericArray2<uint32_t> Uint32Array2;
/// Pointer on Array of dim 2 of uint_t
typedef RCPtr<Uint32Array2> Uint32Array2Ptr;
/// Array of dim 2 of uint_t
typedef NumericArray2<uint_t> UintArray2;
/// Pointer on Array of dim 2 of uint_t
typedef RCPtr<UintArray2> UintArray2Ptr;

// typedef Array2<string> StringArray2;
// typedef RCPtr<StringArray2> StringArray2Ptr;

/// Array of dim 2 of double
typedef NumericArray2<double> DoubleArray2;
/// Pointer on Array of dim 2 of double
typedef RCPtr<DoubleArray2> DoubleArray2Ptr;
/// Array of dim 2 of float
typedef NumericArray2<float> FloatArray2;
/// Pointer on Array of dim 2 of uint_t
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
  RealArray2( uint_t row = 0, uint_t col = 0 );

  /// Constructs an Array2 with \e size copies of \e t.
  RealArray2( uint_t row, uint_t col, const real_t& t ) ;

  /// Constructs an Array2 with the range [\e begin, \e end).
  template <class InIterator>
  RealArray2( InIterator begin, InIterator end, uint_t rowsSize ) :
   NumericArray2<real_t>( begin, end, rowsSize ) {
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

