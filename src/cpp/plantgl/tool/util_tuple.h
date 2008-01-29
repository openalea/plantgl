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


/*! 
	\file util_tuple.h
    \brief File that contains some utility about tuple.
*/


#ifndef __util_tuple_h__
#define __util_tuple_h__


#include "util_types.h"
#include "util_assert.h"

#ifdef _WIN32
#include <iostream>
#else
//class ostream;
#include <iostream>
#endif

TOOLS_BEGIN_NAMESPACE

/*  --------------------------------------------------------------------- */


/**
   \class Tuple2
   \brief A 2D tuple of numeric elements of Type T.
   \pre
   - T must be a model of \b Assignable;
   - T must be a model of \b Default \b Constructible;
   - T must be a model of \b Equality \b Comparable;
   - T must be a model of \b LessThan \b Comparable;
*/

template <class T>
class Tuple2
{

public:

  /// A const iterator used to iterate through an Tuple2.
  typedef const T * const_iterator;

  /// An iterator used to iterate through an Tuple2.
  typedef T * iterator;

  typedef T value_type;

  /// Constructs an Tuple2 with \e to and \e t1.
  inline explicit Tuple2( const T& t0 = T(), const T& t1 = T() ) {
    __T[0] = t0; __T[1] = t1;
  }

  /// Constructs an Tuple2 from the 2 elements tuple \e t2
  inline Tuple2( const T * t2 ) {
    __T[0] = t2[0]; __T[1] = t2[1];
  }

  /// Returns whether \e self is equal to \e t.
  bool operator==( const Tuple2& t ) const {
    return (__T[0] == t.__T[0]) && (__T[1] == t.__T[1]);
  }

  /// Returns whether \e self is equal to \e t.
  bool operator!=( const Tuple2& t ) const {
    return (__T[0] != t.__T[0]) || (__T[1] != t.__T[1]);
  }

  /// Returns whether \e self contain \e t.
  inline bool contain( const T& t ) const {
    return (__T[0] == t) || (__T[1] == t);
  }

  /// Return size of this
  inline uint_t getSize() const { return 2; }

  /** Returns a const reference to the \e i-th element of \e self.
      \pre
      - \e i must belong to the range [0,2). */
  inline const T& getAt( uchar_t i ) const {
    GEOM_ASSERT(i < 2);
    return __T[i];
  }

  inline const T& operator[]( uchar_t i ) const {
    return getAt( i );
  }

  /** Returns a const reference to the \e i-th element of \e self.
      \pre
      - \e i must belong to the range [0,2). */
  inline T& getAt( uchar_t i ) {
    GEOM_ASSERT(i < 2);
    return __T[i];
  }

  inline T& operator[]( uchar_t i ) {
    return getAt( i );
  }

  /// Returns a const iterator at the beginning of \e self.
  inline const_iterator getBegin( ) const {
    return __T;
  }

  /// Returns an iterator at the beginning of \e self.
  inline iterator getBegin( ) {
    return __T;
  }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator getEnd( ) const {
    return __T + 2;
  }

  /// Returns an iterator at the end of \e self.
  inline iterator getEnd( ) {
    return __T + 2;
  }
  
  /// Returns an iterator at the maximum value of \e self.
  const_iterator getMax( ) const {
    return __T[0] < __T[1] ? __T + 1 : __T;
  }

  /// Returns an iterator at the minimum value of \e self.
  const_iterator getMin( ) const {
    return __T[0] < __T[1] ? __T : __T + 1;
  }

  /// Returns whether \e self contain unique elements.
  inline bool isUnique( ) const {
    return ! (__T[0] == __T[1]);
  }

  /// Prints \e self to the output stream \e stream.
  std::ostream& print( std::ostream& stream, 
		  char delimiter = ',', 
		  char begin = '<', 
		  char end = '>' ) const {
    stream << begin << __T[0];
    stream << delimiter << __T[1];
    return stream << end;
  }

  /// Prints \e self to the output stream \e stream.
  friend std::ostream& operator<<(std::ostream& stream, const Tuple2<T> a){
      return a.print(stream);
  }

 /** Sets the \e i-th element of \e self with \e t.
      \pre
      - \e i must belong to the range [0,2). */
  void setAt( uchar_t i, const T& t ) {
    GEOM_ASSERT(i < 2);
    __T[i] = t;
  }  

protected:

  /// The tuple of value.
  T __T[2];

}; // Tuple2


typedef Tuple2<bool> BoolTuple2;
typedef Tuple2<char> CharTuple2;
typedef Tuple2<int16_t> Int16Tuple2;
typedef Tuple2<int32_t> Int32Tuple2;
typedef Tuple2<int_t> IntTuple2;
typedef Tuple2<real_t> RealTuple2;
typedef Tuple2<uchar_t> UCharTuple2;
typedef Tuple2<uint16_t> Uint16Tuple2;
typedef Tuple2<uint32_t> Uint32Tuple2;
typedef Tuple2<uint_t> UintTuple2;


/*  --------------------------------------------------------------------- */


/*!

  \class Tuple3

  \brief A 3D tuple of numeric elements of Type T.

  \warning
  - T must be a model of \b Assignable;
  - T must be a model of \b Default \b Constructible;
  - T must be a model of \b Equality \b Comparable;
  - T must be a model of \b LessThan \b Comparable;
*/


template <class T>
class Tuple3
{

public:

  /// A const iterator used to iterate through an Tuple3.
  typedef const T * const_iterator;

  /// An iterator used to iterate through an Tuple3.
  typedef T * iterator;

  typedef T value_type;

  /// Constructs an Tuple3 with \e to, \e t1 and \e t2.
  inline explicit Tuple3( const T& t0 = T(),
			  const T& t1 = T(),
	 		  const T& t2 = T() ) {
    __T[0] = t0; __T[1] = t1; __T[2] = t2;
  }

  /// Constructs an Tuple3 from the 3 elements tuple \e a3
  inline Tuple3( const T * a3 ) {
    __T[0] = a3[0]; __T[1] = a3[1]; __T[2] = a3[2];
  }

  /// Returns whether \e self is equal to \e t.
  bool operator==( const Tuple3& t ) const {
    return (__T[0] == t.__T[0]) && (__T[1] == t.__T[1]) && (__T[2] == t.__T[2]);
  }

  /// Returns whether \e self is equal to \e t.
  bool operator!=( const Tuple3& t ) const {
    return (__T[0] != t.__T[0]) || (__T[1] != t.__T[1]) || (__T[2] != t.__T[2]);
  }

  /// Returns whether \e self contain \e t.
  inline bool contain( const T& t ) const {
    return (__T[0] == t) || (__T[1] == t) || (__T[2] == t);
  }

  /// Return size of this
  inline uint_t getSize() const { return 3; }

  /** Returns a const reference to the \e i-th element of \e self.
      \pre
      - \e i must belong to the range [0,3). */
  inline const T& getAt( uchar_t i ) const {
    GEOM_ASSERT(i < 3);
    return __T[i];
  }

  const T& operator[]( uchar_t i ) const {
    return getAt( i );
  }

  /** Returns a reference to the \e i-th element of \e self.
      \pre
      - \e i must belong to the range [0,3). */
  inline T& getAt( uchar_t i ) {
    GEOM_ASSERT(i < 3);
    return __T[i];
  }

  T& operator[]( uchar_t i ) {
    return getAt( i );
  }

  /// Returns a const iterator at the beginning of \e self.
  inline const_iterator getBegin( ) const {
    return __T;
  }

  /// Returns an iterator at the beginning of \e self.
  inline iterator getBegin( ) {
    return __T;
  }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator getEnd( ) const {
    return __T + 3;
  }

  /// Returns an iterator at the end of \e self.
  inline iterator getEnd( ) {
    return __T + 3;
  }

  /// Returns an iterator at the maximum value of \e self.
  const_iterator getMax( ) const {
    return __T[0] < __T[1] ? 
      (__T[1] < __T[2] ? __T + 2 : __T + 1) : 
      (__T[0] < __T[2] ? __T + 2 : __T);
  }

  /// Returns an iterator at the minimum value of \e self.
  const_iterator getMin( ) const {
    return __T[0] < __T[1] ? 
      (__T[0] < __T[2] ? __T : __T + 2) : 
      (__T[1] < __T[2] ? __T + 1 : __T + 2);
  }

  /// Returns whether \e self contain unique elements.
  bool isUnique( ) const {
    return ! ((__T[0] == __T[1]) || (__T[0] == __T[2]) || (__T[1] == __T[2]));
  }

  /// Prints \e self to the output stream \e stream.
  std::ostream& print( std::ostream& stream, 
		  char delimiter = ',', 
		  char begin = '<', 
		  char end = '>' ) const {
    stream << begin << __T[0];
    stream << delimiter << __T[1];
    stream << delimiter << __T[2];
    return stream << end;
  }

  /// Prints \e self to the output stream \e stream.
  friend std::ostream& operator<<(std::ostream& stream, const Tuple3<T> a){
      return a.print(stream);
  }

  /** Sets the \e i-th element of \e self with \e t.
      \pre
      - \e i must belong to the range [0,3). */
  void setAt( uchar_t i, const T& t ) {
    GEOM_ASSERT(i < 3);
    __T[i] = t;
  }  

protected:

  /// The 3D value tuple.
  T __T[3];

}; // Tuple3


typedef Tuple3<bool> BoolTuple3;
typedef Tuple3<char> CharTuple3;
typedef Tuple3<int16_t> Int16Tuple3;
typedef Tuple3<int32_t> Int32Tuple3;
typedef Tuple3<int_t> IntTuple3;
typedef Tuple3<real_t> RealTuple3;
typedef Tuple3<uchar_t> UCharTuple3;
typedef Tuple3<uint16_t> Uint16Tuple3;
typedef Tuple3<uint32_t> Uint32Tuple3;
typedef Tuple3<uint_t> UintTuple3;


/*  --------------------------------------------------------------------- */


/*!

  \class Tuple4

  \brief A 4D tuple of numeric elements of Type T.

  \pre
  - T must be a model of \b Assignable;
  - T must be a model of \b Default \b Constructible;
  - T must be a model of \b Equality b Comparable;
  - T must be a model of \b LessThan b Comparable;
*/

template <class T>
class Tuple4
{

public:
  
  /// A const iterator used to iterate through an Tuple4.
  typedef const T * const_iterator;

  /// An iterator used to iterate through an Tuple4.
  typedef T * iterator;

  typedef T value_type;

  /// Constructs a Tuple4 with \e to, \e t1, \e t2 and \e t3.
  inline explicit Tuple4( const T& t0 = T(), 
			  const T& t1 = T(),
			  const T& t2 = T(),
			  const T& t3 = T() ) {
    __T[0] = t0; __T[1] = t1; __T[2] = t2; __T[3] = t3;
  }

  /// Constructs an Tuple4 from the 4 elements tuple \e a4
  inline Tuple4( const T * a4 ) {
    __T[0] = a4[0]; __T[1] = a4[1]; __T[2] = a4[2]; __T[2] = a4[2];
  }

  /// Returns whether \e self is equal to \e t.
  bool operator==( const Tuple4& t ) const {
    return 
      (__T[0] == t.__T[0]) && (__T[1] == t.__T[1]) && 
      (__T[2] == t.__T[2]) && (__T[3] == t.__T[3]);
  }

  /// Returns whether \e self is equal to \e t.
  bool operator!=( const Tuple4& t ) const {
    return 
      (__T[0] != t.__T[0]) || (__T[1] != t.__T[1]) || 
      (__T[2] != t.__T[2]) || (__T[3] != t.__T[3]);
  }

  /// Returns whether \e self contain \e t.
  inline bool contain( const T& t ) const {
    return (__T[0] == t) || (__T[1] == t) || (__T[2] == t) || (__T[3] == t);
  }

  /// Return size of this
  inline uint_t getSize() const { return 4; }

  /** Returns a const reference to the \e i-th element of \e self.
      \pre
      - \e i must belong to the range [0,4). */
  inline const T& getAt( uchar_t i ) const {
    GEOM_ASSERT(i < 4);
    return __T[i];
  }

  const T& operator[]( uchar_t i ) const {
    return getAt( i );
  }

  /** Returns a reference to the \e i-th element of \e self.
      \pre
      - \e i must belong to the range [0,4). */
  inline T& getAt( uchar_t i ) {
    GEOM_ASSERT(i < 4);
    return __T[i];
  }

  T& operator[]( uchar_t i ) {
    return getAt( i );
  }

  /// Returns a const iterator at the beginning of \e self.
  inline const_iterator getBegin( ) const {
    return __T;
  }

  /// Returns an iterator at the beginning of \e self.
  inline iterator getBegin( ) {
    return __T;
  }


  /// Returns a const iterator at the end of \e self.
  inline const_iterator getEnd( ) const {
    return __T + 4;
  }

  /// Returns an iterator at the end of \e self.
  inline iterator getEnd( ) {
    return __T + 4;
  }

  /// Returns an iterator at the maximum value of \e self.
  const_iterator getMax( ) const {
    return __T[0] < __T[1] ?
      (__T[1] < __T[2] ?
       (__T[2] < __T[3] ? __T + 3 : __T + 2) : 
       (__T[1] < __T[3] ? __T + 3 : __T + 1)) :
      (__T[0] < __T[2] ?
       (__T[2] < __T[3] ? __T + 3 : __T + 2) : 
       (__T[0] < __T[3] ? __T + 3 : __T));
  }

  /// Returns an iterator at the minimum value of \e self.
  const_iterator getMin( ) const {
    return __T[0] < __T[1] ?
      (__T[0] < __T[2] ?
       (__T[0] < __T[3] ? __T : __T + 3) : 
       (__T[2] < __T[3] ? __T + 2 : __T + 3)) :
      (__T[1] < __T[2] ?
       (__T[1] < __T[3] ? __T + 1 : __T + 3) : 
       (__T[2] < __T[3] ? __T + 2 : __T + 3));
  }

  /// Returns whether \e self contain unique elements.
  bool isUnique( ) const {
    return ! ((__T[0] == __T[1]) || (__T[0] == __T[2]) || (__T[0] == __T[3]) ||
	      (__T[1] == __T[2]) || (__T[1] == __T[3]) || (__T[2] == __T[3]));
  }

  /// Prints \e self to the output stream \e stream.
  std::ostream& print( std::ostream& stream, 
		  char delimiter = ',', 
		  char begin = '<', 
		  char end = '>' ) const {
    stream << begin << __T[0];
    stream << delimiter << __T[1];
    stream << delimiter << __T[2];
    stream << delimiter << __T[3];
    return stream << end;
  }

  /// Prints \e self to the output stream \e stream.
  friend std::ostream& operator<<(std::ostream& stream, const Tuple4<T> a){
      return a.print(stream);
  }

 /** Sets the \e i-th element of \e self with \e t.
      \pre
      - \e i must belong to the range [0,4). */
  void setAt( uchar_t i, const T& t ) {
    GEOM_ASSERT(i < 4);
    __T[i] = t;
  }  

protected:

  /// The 4D value tuple.
  T __T[4];

}; // Tuple4


typedef Tuple4<bool> BoolTuple4;
typedef Tuple4<char> CharTuple4;
typedef Tuple4<int16_t> Int16Tuple4;
typedef Tuple4<int32_t> Int32Tuple4;
typedef Tuple4<int_t> IntTuple4;
typedef Tuple4<real_t> RealTuple4;
typedef Tuple4<uchar_t> UCharTuple4;
typedef Tuple4<uint16_t> Uint16Tuple4;
typedef Tuple4<uint32_t> Uint32Tuple4;
typedef Tuple4<uint_t> UintTuple4;


/*  ---------------------------------------------------------------------- */

// typedef Tuple2<string> StringTuple2;
// typedef Tuple3<string> StringTuple3;
// typedef Tuple4<string> StringTuple4;

/*  ---------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

// __util_tuple_h__
#endif
