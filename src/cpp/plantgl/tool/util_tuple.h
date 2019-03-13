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
   \class Tuple
   \brief A N-D tuple of numeric elements of Type T.
   \pre
   - T must be a model of \b Assignable;
   - T must be a model of \b Default \b Constructible;
   - T must be a model of \b Equality \b Comparable;
   - T must be a model of \b LessThan \b Comparable;
*/

template <class T, int N>
class Tuple
{

public:

  /// A const iterator used to iterate through an Tuple.
  typedef const T * const_iterator;

  /// An iterator used to iterate through an Tuple.
  typedef T * iterator;

  /// A const iterator used to iterate through an Tuple.
  typedef const T * const_reverse_iterator;

  /// An iterator used to iterate through an Tuple.
  typedef T * reverse_iterator;

  // The value type
  typedef T value_type;

  static const size_t SIZE = N;

  inline Tuple( ) { }

  /// Constructs an Tuple with each values initialized to \e v .
  inline explicit Tuple( const T& v ) {
      for (iterator it = begin(); it != end(); ++it)
            *it = v;
  }

  /// Constructs an Tuple from the elements in \e t
  inline Tuple( const T * t ) {
      for (iterator it = begin(); it != end(); ++it,++t)
            *it = *t;
  }

  /// Constructs an Tuple from value between beg and end
  template<class Iterator>
  inline Tuple( Iterator beg_source, Iterator end_source ) {
      assert(std::distance(beg_source, end_source) == N);
          Iterator itsrc = beg_source;
      for (iterator it = begin(); it != end(); ++it,++itsrc)
            *it = *itsrc;
  }

  /// Constructs an Tuple with each values initialized to \e v .
  inline explicit Tuple( const T& v0, const T& v1 ) {
      assert(N >= 2);
      __data[0] = v0; __data[1] = v1;
      for (iterator it = begin()+2; it != end(); ++it) *it = T();
  }

  /// Constructs an Tuple with each values initialized to \e v .
  inline explicit Tuple( const T& v0, const T& v1, const T& v2  ) {
      assert(N >= 3);
      __data[0] = v0; __data[1] = v1; __data[2] = v2;
      for (iterator it = begin()+3; it != end(); ++it) *it = T();
  }

  /// Constructs an Tuple with each values initialized to \e v .
  inline explicit Tuple( const T& v0, const T& v1, const T& v2, const T& v3  ) {
      assert(N >= 4);
      __data[0] = v0; __data[1] = v1; __data[2] = v2; ; __data[3] = v3;
      for (iterator it = begin()+4; it != end(); ++it) *it = T();
  }

  /// Constructs a Tuple from the elements
/*  inline Tuple( const T& first, ... ) {
    iterator it = begin();
    va_list values;
    *it = first; ++it;
    va_start( values );     // Initialize variable arguments.
    for (; it != end(); ++it)
    {  *it = va_arg( values, T );  }
    va_end( values );              // Reset variable arguments.
  }*/

  inline const T * data() const { return __data; }
  inline T * data() { return __data; }

  /// Returns whether \e self is equal to \e t.
  bool operator==( const Tuple& t ) const {
    const_iterator it2 = t.begin();
    for (const_iterator it = begin(); it != end() ; ++it, ++it2)
        if( *it != *it2) return false;
    return true;
  }

  /// Returns whether \e self is equal to \e t.
  bool operator!=( const Tuple& t ) const {
    const_iterator it2 = t.begin();
    for (const_iterator it = begin(); it != end() ; ++it, ++it2)
        if( *it != *it2) return true;
    return false;
  }

  /// Returns whether \e self contain \e t.
  inline bool contains( const T& t ) const {
    for (const_iterator it = begin(); it != end() ; ++it)
        if (*it == t) return true;
    return false;
  }

  /// Return size of this
  static inline size_t size() { return SIZE; }

  /** Returns a const reference to the \e i-th element of \e self.
      \pre
      - \e i must belong to the range [0,2). */
  inline const T& getAt( uchar_t i ) const {
    GEOM_ASSERT(i < SIZE);
    return __data[i];
  }

  inline const T& operator[]( uchar_t i ) const { return getAt( i ); }

  /** Returns a const reference to the \e i-th element of \e self.
      \pre
      - \e i must belong to the range [0,2). */
  inline T& getAt( uchar_t i ) {
    GEOM_ASSERT(i < SIZE);
    return __data[i];
  }

  inline T& operator[]( uchar_t i ) { return getAt( i ); }

  /// Returns a const iterator at the beginning of \e self.
  inline const_iterator begin( ) const { return __data; }

  /// Returns an iterator at the beginning of \e self.
  inline iterator begin( ) { return __data; }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator end( ) const { return __data + N; }

  /// Returns an iterator at the end of \e self.
  inline iterator end( ) { return __data + N; }

  /// Returns a const iterator at the beginning of \e self.
  inline const_reverse_iterator rbegin( ) const { return __data+(N-1); }

  /// Returns an iterator at the beginning of \e self.
  inline reverse_iterator rbegin( ) { return __data+(N-1); }

  /// Returns a const iterator at the end of \e self.
  inline const_reverse_iterator rend( ) const { return __data - 1; }

  /// Returns an iterator at the end of \e self.
  inline reverse_iterator rend( ) { return __data - 1; }

  /// Returns an iterator at the maximum value of \e self.
  const_iterator getMax( ) const {
    const_iterator mvalue = __data;
    for (const_iterator it = begin()+1; it != end() ; ++it)
        if (*it > *mvalue) mvalue = it;
    return mvalue;
  }

  /// Returns an iterator at the minimum value of \e self.
  const_iterator getMin( ) const {
    const_iterator mvalue = __data;
    for (const_iterator it = begin()+1; it != end() ; ++it)
        if (*it < *mvalue) mvalue = it;
    return mvalue;
  }

  /// Returns whether \e self contain unique elements.
  inline bool isUnique( ) const {
    for (const_iterator it = begin(); it != end() ; ++it)
        for (const_iterator it2 = it+1; it2 != end() ; ++it2)
            if (*it == *it2) return false;
    return true;
  }

  /// Prints \e self to the output stream \e stream.
  std::ostream& print( std::ostream& stream,
          char delimiter = ',',
          char beg_sign = '<',
          char end_sign = '>' ) const {
    stream << beg_sign << __data[0];
    for (const_iterator it = begin()+1; it != end() ; ++it)
        stream << delimiter << *it;
    return stream  << end_sign ;
  }

  /// Prints \e self to the output stream \e stream.
  friend std::ostream& operator<<(std::ostream& stream, const Tuple<T,N> a){
      return a.print(stream);
  }

 /** Sets the \e i-th element of \e self with \e t.
      \pre
      - \e i must belong to the range [0,N). */
  inline void setAt( uchar_t i, const T& t ) {
    GEOM_ASSERT(i < SIZE);
    __data[i] = t;
  }

 /** Sets the \e i-th element of \e self with \e t.
      \pre
      - \e i must belong to the range [0,N). */
  inline void setAll( const T& t ) {
    GEOM_ASSERT(i < SIZE);
    for (iterator it = begin(); it != end() ; ++it)
       *it = t;
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

  inline attribute_deprecated bool contain( const T& t ) const { return contains(t); }

  /// Return size of this
  static inline attribute_deprecated uint_t getSize() { return size(); }
#endif

protected:

  /// The tuple of value.
  T __data[N];

}; // Tuple

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


// template <class T> class Tuple<T,2> { };

template <class T>
class Tuple2 : public Tuple<T,2>
{
  typedef Tuple<T,2> Base;
public:
  /// A const iterator used to iterate through an Tuple.
  typedef typename Base::const_iterator const_iterator;

  /// An iterator used to iterate through an Tuple.
  typedef typename Base::iterator iterator;

  /// A const iterator used to iterate through an Tuple.
  typedef typename Base::const_reverse_iterator const_reverse_iterator;

  /// An iterator used to iterate through an Tuple.
  typedef typename Base::reverse_iterator reverse_iterator;

  /// Constructs an Tuple2 with \e to and \e t1.
  inline explicit Tuple2( const T& t0 = T(), const T& t1 = T() ) {
    Base::getAt(0) = t0; Base::getAt(1) = t1;
  }

  /// Constructs an Tuple2 from the 2 elements tuple \e t2
  inline Tuple2( const T * t2 ) {
    Base::getAt(0) = t2[0]; Base::getAt(1) = t2[1];
  }

  /// Returns whether \e self is equal to \e t.
  bool operator==( const Tuple2& t ) const {
    return (Base::getAt(0) == t[0]) && (Base::getAt(1) == t[1]);
  }

  /// Returns whether \e self is equal to \e t.
  bool operator!=( const Tuple2& t ) const {
    return (Base::getAt(0) != t[0]) || (Base::getAt(1) != t[1]);
  }

  /// Returns whether \e self contain \e t.
  inline bool contains( const T& t ) const {
    return (Base::getAt(0) == t) || (Base::getAt(1) == t);
  }

  /// Returns an iterator at the maximum value of \e self.
  const_iterator getMax( ) const {
    return Base::getAt(0) < Base::getAt(1) ? Base::__data + 1 : Base::__data;
  }

  /// Returns an iterator at the minimum value of \e self.
  const_iterator getMin( ) const {
    return Base::getAt(0) < Base::getAt(1) ? Base::__data : Base::__data + 1;
  }

  /// Returns whether \e self contain unique elements.
  inline bool isUnique( ) const {
    return ! (Base::getAt(0) == Base::getAt(1));
  }


 /** Sets the elements of \e self. */
  void set( const T& t1, const T& t2  ) {
    Base::getAt(0) = t1;
    Base::getAt(1) = t2;
  }

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
class Tuple3 : public Tuple<T,3>
{
  typedef Tuple<T,3> Base;

public:
  /// A const iterator used to iterate through an Tuple.
  typedef typename Base::const_iterator const_iterator;

  /// An iterator used to iterate through an Tuple.
  typedef typename Base::iterator iterator;

  /// A const iterator used to iterate through an Tuple.
  typedef typename Base::const_reverse_iterator const_reverse_iterator;

  /// An iterator used to iterate through an Tuple.
  typedef typename Base::reverse_iterator reverse_iterator;

  /// Constructs an Tuple3 with \e to, \e t1 and \e t2.
  inline explicit Tuple3( const T& t0 = T(),
               const T& t1 = T(),
               const T& t2 = T() ) {
    Base::getAt(0) = t0; Base::getAt(1) = t1; Base::getAt(2) = t2;
  }

  /// Constructs an Tuple3 from the 3 elements tuple \e a3
  inline Tuple3( const T * a3 ) {
    Base::getAt(0) = a3[0]; Base::getAt(1) = a3[1]; Base::getAt(2) = a3[2];
  }

  /// Returns whether \e self is equal to \e t.
  bool operator==( const Tuple3& t ) const {
    return (Base::getAt(0) == t[0]) && (Base::getAt(1) == t[1]) && (Base::getAt(2) == t[2]);
  }

  /// Returns whether \e self is equal to \e t.
  bool operator!=( const Tuple3& t ) const {
    return (Base::getAt(0) != t[0]) || (Base::getAt(1) != t[1]) || (Base::getAt(2) != t[2]);
  }

  /// Returns whether \e self contain \e t.
  inline bool contains( const T& t ) const {
    return (Base::getAt(0) == t) || (Base::getAt(1) == t) || (Base::getAt(2) == t);
  }

  /// Returns an iterator at the maximum value of \e self.
  const_iterator getMax( ) const {
    return Base::getAt(0) < Base::getAt(1) ?
      (Base::getAt(1) < Base::getAt(2) ? Base::__data + 2 : Base::__data + 1) :
      (Base::getAt(0) < Base::getAt(2) ? Base::__data + 2 : Base::__data);
  }

  /// Returns an iterator at the minimum value of \e self.
  const_iterator getMin( ) const {
    return Base::getAt(0) < Base::getAt(1) ?
      (Base::getAt(0) < Base::getAt(2) ? Base::__data : Base::__data + 2) :
      (Base::getAt(1) < Base::getAt(2) ? Base::__data + 1 : Base::__data + 2);
  }

  /// Returns whether \e self contain unique elements.
  bool isUnique( ) const {
    return ! ((Base::getAt(0) == Base::getAt(1)) || (Base::getAt(0) == Base::getAt(2)) || (Base::getAt(1) == Base::getAt(2)));
  }

 /** Sets the elements of \e self. */
  void set( const T& t1, const T& t2, const T& t3  ) {
    Base::getAt(0) = t1;
    Base::getAt(1) = t2;
    Base::getAt(2) = t3;
  }

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
class Tuple4 : public Tuple<T,4>
{
  typedef Tuple<T,4> Base;

public:
  /// A const iterator used to iterate through an Tuple.
  typedef typename Base::const_iterator const_iterator;

  /// An iterator used to iterate through an Tuple.
  typedef typename Base::iterator iterator;

  /// A const iterator used to iterate through an Tuple.
  typedef typename Base::const_reverse_iterator const_reverse_iterator;

  /// An iterator used to iterate through an Tuple.
  typedef typename Base::reverse_iterator reverse_iterator;

  /// Constructs a Tuple4 with \e to, \e t1, \e t2 and \e t3.
  inline explicit Tuple4( const T& t0 = T(),
              const T& t1 = T(),
              const T& t2 = T(),
              const T& t3 = T() ) {
    Base::getAt(0) = t0; Base::getAt(1) = t1; Base::getAt(2) = t2; Base::getAt(3) = t3;
  }

  /// Constructs an Tuple4 from the 4 elements tuple \e a4
  inline Tuple4( const T * a4 ) {
    Base::getAt(0) = a4[0];
    Base::getAt(1) = a4[1];
    Base::getAt(2) = a4[2];
    Base::getAt(2) = a4[2];
  }

  /// Returns whether \e self is equal to \e t.
  bool operator==( const Tuple4& t ) const {
    return
      (Base::getAt(0) == t[0]) && (Base::getAt(1) == t[1]) &&
      (Base::getAt(2) == t[2]) && (Base::getAt(3) == t[3]);
  }

  /// Returns whether \e self is equal to \e t.
  bool operator!=( const Tuple4& t ) const {
    return
      (Base::getAt(0) != t[0]) || (Base::getAt(1) != t[1]) ||
      (Base::getAt(2) != t[2]) || (Base::getAt(3) != t[3]);
  }

  /// Returns whether \e self contain \e t.
  inline bool contains( const T& t ) const {
    return (Base::getAt(0) == t) || (Base::getAt(1) == t) || (Base::getAt(2) == t) || (Base::getAt(3) == t);
  }

  /// Returns an iterator at the maximum value of \e self.
  const_iterator getMax( ) const {
    return Base::getAt(0) < Base::getAt(1) ?
      (Base::getAt(1) < Base::getAt(2) ?
       (Base::getAt(2) < Base::getAt(3) ? Base::__data + 3 : Base::__data + 2) :
       (Base::getAt(1) < Base::getAt(3) ? Base::__data + 3 : Base::__data + 1)) :
      (Base::getAt(0) < Base::getAt(2) ?
       (Base::getAt(2) < Base::getAt(3) ? Base::__data + 3 : Base::__data + 2) :
       (Base::getAt(0) < Base::getAt(3) ? Base::__data + 3 : Base::__data));
  }

  /// Returns an iterator at the minimum value of \e self.
  const_iterator getMin( ) const {
    return Base::getAt(0) < Base::getAt(1) ?
      (Base::getAt(0) < Base::getAt(2) ?
       (Base::getAt(0) < Base::getAt(3) ? Base::__data : Base::__data + 3) :
       (Base::getAt(2) < Base::getAt(3) ? Base::__data + 2 : Base::__data + 3)) :
      (Base::getAt(1) < Base::getAt(2) ?
       (Base::getAt(1) < Base::getAt(3) ? Base::__data + 1 : Base::__data + 3) :
       (Base::getAt(2) < Base::getAt(3) ? Base::__data + 2 : Base::__data + 3));
  }

  /// Returns whether \e self contain unique elements.
  bool isUnique( ) const {
    return ! ((Base::getAt(0) == Base::getAt(1)) || (Base::getAt(0) == Base::getAt(2)) || (Base::getAt(0) == Base::getAt(3)) ||
          (Base::getAt(1) == Base::getAt(2)) || (Base::getAt(1) == Base::getAt(3)) || (Base::getAt(2) == Base::getAt(3)));
  }

 /** Sets the elements of \e self. */
  void set( const T& t1, const T& t2, const T& t3, const T& t4  ) {
    Base::getAt(0) = t1;
    Base::getAt(1) = t2;
    Base::getAt(2) = t3;
    Base::getAt(3) = t4;
  }

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

template<class T, int I>
inline T reduce(const Tuple<T,I>& t, T (* functor )(const T&, const T&))
{
    T res = t[0];
    for (typename Tuple<T,I>::const_iterator it = t.begin()+1; it != t.end(); ++it)
        res = (*functor)(res,*it);
    return res;
}

namespace internal {
  template <class U> inline U mult(const U& a, const U& b) { return a*b; }
  template <class T> inline T add(const T& a, const T& b) { return a+b; }
}

template<class T, int I>
inline T product(const Tuple<T,I>& t) {
    return reduce<T,I>(t,&internal::mult<T>);
}

template<class T, int I>
inline T sum(const Tuple<T,I>& t) {

    return reduce<T,I>(t,&internal::add<T>);
}

/*  ---------------------------------------------------------------------- */

template<class Tuple>
inline Tuple add(const Tuple& t1, const Tuple& t2) {
    Tuple res(t1);
    typename Tuple::const_iterator it2 = t2.begin();
    for (typename Tuple::iterator it = res.begin(); it != res.end(); ++it,++it2 )
        *it += *it2;
    return res;
}

template<class Tuple>
inline Tuple sub(const Tuple& t1, const Tuple& t2) {
    Tuple res(t1);
    typename Tuple::const_iterator it2 = t2.begin();
    for (typename Tuple::iterator it = res.begin(); it != res.end(); ++it,++it2 )
        *it -= *it2;
    return res;
}

/*  ---------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

// __util_tuple_h__
#endif
