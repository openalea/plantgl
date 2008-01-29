/* -*-c++-*- 
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): Ch. Nouguier (christophe.nouguier@cirad.fr) 
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




#ifndef __rcobject_h__
#define __rcobject_h__

/*! \file rcobject.h
    \brief File for reference counting object and pointer.
*/

/* ----------------------------------------------------------------------- */

#include "tools_config.h"
#include "util_assert.h" // For #include <assert.h>
#include "util_types.h"  // For #include <stddef.h> and typedef long int32_t;

#ifdef RCOBJECT_DEBUG
#include <typeinfo>
#include <iostream>
#endif


/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


/**
   \class RefCountObject
   \brief A base class for reference-counted objects.

   This implementation is taken from the book \b More \b Effective \b C++ 
   (Scott Meyers).
   \warning Destructor must always be implemented even if they are pure 
   virtual and do nothing. When implemeting an object inheriting from 
   RefCountObject, you can use the macro DECLARE_REF_COUNT_OBJECT(your 
   object) in the object specification section in order to be sure to 
   declare the virtual destructor. You need then to implement it.
*/

class TOOLS_API RefCountObject
{

public:

  /// @name Constructors
  //@{

  /// Default constructor.
  RefCountObject( ) :
    _ref_count(0)
  {
  }

  /// Copy constructor.
  RefCountObject( const RefCountObject& ) :
    _ref_count(0)
  {
  }
  
  //@}

  /// @name Destructor
  //@{

  /// Destructor.
  virtual ~RefCountObject( )
  {
  }

  //@}
  
  /// @name Assignement operator
  //@{
  
  /// Assignement operator
  RefCountObject& operator=( const RefCountObject& )
  {
    return *this;
  }
  //@}


  /// @name Reference counting functions
  //@{

  /// Increments the reference counter.
  inline void addReference( )
  {
    ++_ref_count;
#ifdef RCOBJECT_DEBUG
    std::cerr << this << " ref++ => " << getReferenceCount();
    std::cerr << "\t(" << typeid(*this).name() << ")" << std::endl;
#endif
  }

  /// Returns the number of reference to \e self.
  inline size_t getReferenceCount( ) const 
  {
    return _ref_count;
  }

  /// Returns whether \e self is shared.
  inline bool isShared( ) const
  {
    return _ref_count > 1;
  }

  /// Decrements the reference counter.  
  inline void removeReference( )
  {
    --_ref_count;
#ifdef RCOBJECT_DEBUG
    std::cerr << this << " ref-- => " << getReferenceCount();
    std::cerr << "\t(" << typeid(*this).name() << ")" << std::endl;
#endif
    if (_ref_count == 0) delete this;
  }
  
  //@}

private:

  size_t _ref_count;

}; // RefCountObject


/* ----------------------------------------------------------------------- */


/**
   \class RefCountPtr
   \brief A smart pointer to reference-counted objects.

   This implementation is taken from the book \b More \b Effective \b C++ 
   (Scott Meyers).
   \warning
   - T must inherit from RefCountObject..
*/


template <class T>
class RefCountPtr
{

public:

  // For compatibility with Boost.Python.
  typedef T element_type;

  /// @name Constructors
  //@{

  /// Constructs an empty RefCountPtr.
  RefCountPtr( ) :  __ptr(0) {}

  /// Constructs an RefCountPtr from the dumb pointer \e prt.
  /* explicit */
  RefCountPtr( T * ptr ) : __ptr(ptr) { if (__ptr) __ptr->addReference(); }

  /// Copy constructor.
  RefCountPtr( const RefCountPtr& ptr ) :  __ptr(ptr.__ptr) { if (__ptr) __ptr->addReference(); }
  
#if _MSC_VER > 1200
  /// Copy constructor.
  template<class U>
  RefCountPtr( const RefCountPtr<U>& ptr ) :  __ptr(ptr.toPtr()) { if (__ptr) __ptr->addReference(); }
#endif

  //@}


  ///@name Destructor
  //@{

  /// Destructor.
  ~RefCountPtr( ){ if (__ptr)__ptr->removeReference(); }

  //@}


  /// @name Assignement operators
  //@{

  /// Assignement operator with RefCountPtr<T> \e ptr.
  RefCountPtr& operator=( const RefCountPtr& ptr )
  {
    if (__ptr != ptr.__ptr)
	{
	  if (__ptr) __ptr->removeReference();
	  __ptr = ptr.__ptr;
	  if (__ptr) __ptr->addReference();
	};
    return *this;
  }

  
#if _MSC_VER > 1200
  /// Assignement operator with RefCountPtr<T> \e ptr.
  template<class U>
  RefCountPtr& operator=( const RefCountPtr<U>& ptr )
  {
	 return operator=(ptr.toPtr());
  }
#endif

  /// Assignement operator with pointer \e ptr.
  RefCountPtr& operator=( T * ptr )
  {
    if (__ptr != ptr)
	{
	  if (__ptr) __ptr->removeReference();
	  __ptr = ptr;
	  if (__ptr) __ptr->addReference();
	};
    return *this;
  }

  //@}

  /// @name Cast operation
  //@{

  /// Cast operation
  template <class U>
  inline RefCountPtr& cast( const RefCountPtr<U>& ptr )
  {
    if (__ptr != ptr.toPtr())
	{
	  if (__ptr) __ptr->removeReference();
	  __ptr = dynamic_cast<T *>(ptr.toPtr());
	  if (__ptr) __ptr->addReference();
	};
    return *this;
  }

  /// Cast operation
  template <class U>
  static RefCountPtr Cast( const RefCountPtr<U>& ptr )
  {
    RefCountPtr rptr;
    if (ptr.toPtr() != NULL)return rptr.cast(ptr);
    return rptr;
  }


  //@}

  /// @name Comparison operators
  //@{

  /// Comparison operator with RefCountPtr<T> \e ptr.
  bool equal( const RefCountPtr& ptr ) const { return __ptr == ptr.__ptr; }
  /// Comparison operator with RefCountPtr<T> \e ptr.
  bool operator==( const RefCountPtr& ptr ) const { return __ptr == ptr.__ptr; }

  /// Comparison operator with pointer \e ptr.
  bool operator==( const T * ptr ) const { return __ptr == ptr; }

  /// Comparison operator with RefCountPtr<T> \e ptr.
  bool not_equal( const RefCountPtr& ptr ) const { return __ptr != ptr.__ptr; }
  bool operator!=( const RefCountPtr& ptr ) const { return __ptr != ptr.__ptr; }

  /// Comparison operator with pointer \e ptr.
  bool operator!=( const T * ptr ) const { return __ptr != ptr; }

  /// Comparison operator with RefCountPtr<T> \e ptr.
  bool inf( const RefCountPtr& ptr ) const { return __ptr < ptr.__ptr; }
  /// Comparison operator with RefCountPtr<T> \e ptr.
  bool operator<( const RefCountPtr& ptr ) const { return __ptr < ptr.__ptr; }

  /// Comparison operator with pointer \e ptr.
  bool operator<( const T * ptr ) const { return __ptr < ptr; }

  //@}


  /// @name Dereferencing operators
  //@{

  /// Deferences \e self to get a pointer to the object \s self points on.
  T * operator->( ) const { return __ptr; }

  /** Dereferences \e self to get a reference to the object \e self points on.
      \warning
     - \e self must be non null. */
  T& operator*( ) const { assert(__ptr != 0); return *__ptr; }

  //@}

  /// @name Conversion operators
  //@{

  /// Returns true if and only if \e self is not null.
  //  operator bool( ) const { return __ptr != 0; }
  
  /// Return a conversion of \e self into bool
  bool toBool( ) const { return __ptr != 0; }

  /// Implicit conversion into normal pointer.
  operator T * ( ) const { return __ptr; }
  
  /// Return a conversion of \e self into T *
  T * toPtr( ) const { return __ptr; }

  /// Return a conversion of \e self into size_t
  size_t toSizeT( ) const { return (size_t)__ptr; }
  
  /// Return a conversion of \e self into uint_t
  uint32_t toUint32( ) const 
#if __WORDSIZE == 64
  { return (uintptr_t)__ptr; }
#else
  { return (uint32_t)__ptr; }
#endif
  
  
  //@}
  /// @name Nullness testing operators
  //@{

  /// Returns true if and only if \e self is not null.
  bool isValid( ) const { return __ptr != 0; }

  /// Returns true if and only if \e self is null.
  bool operator!( ) const { return __ptr == 0; }

  /// Returns true if and only if \e self is null.
  bool isNull( ) const { return __ptr == 0; }

  //@}

private:

  T * __ptr;

}; // RefCountPtr

/* ------------------------------------------------------------------------- */

// For compatibility with Boost.Python.
template<class T>
T* get_pointer(const RefCountPtr<T>& p)
{
	return p.toPtr();
}

/*
 * helper macros section
 *
 */

/// To declare a Reference Counting Pointer on Type : TypePtr
#define DECLARE_PTR(Type) \
  typedef RCPtr<Type> Type##Ptr; \

/// To make a forward declaration of a Reference Counting Pointer on Type : TypePtr
#define DECLARE_FWD_PTR(Type) \
  class Type; \
  typedef RCPtr<Type> Type##Ptr; \

/* ----------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

/// Tools Reference Counting Pointer 
#define RCPtr TOOLS(RefCountPtr)

/* ----------------------------------------------------------------------- */

// __rcobject_h__
#endif
