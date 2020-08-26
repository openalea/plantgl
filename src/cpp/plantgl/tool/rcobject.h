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





#ifndef __rcobject_h__
#define __rcobject_h__

/*! \file rcobject.h
    \brief File for reference counting object and pointer.
*/

/* ----------------------------------------------------------------------- */

#include "tools_config.h"
#include "util_assert.h" // For #include <assert.h>
#include "util_types.h"  // For #include <stddef.h> and typedef long int32_t;
#include <atomic>

#ifdef RCOBJECT_DEBUG
#include <typeinfo>
#include <iostream>
#endif

#define PGL_SMARTPTR
// #define BOOST_INSTRUSIVEPTR
// #define BOOST_SHAREDPTR

#ifdef PGL_SMARTPTR
#define NO_BOOST_REFCOUNTPTR
#else
#define BOOST_REFCOUNTPTR
#endif

#define WITH_REFCOUNTLISTENER

/* ----------------------------------------------------------------------- */

/**
   \class RefCountPtr
   \brief A smart pointer to reference-counted objects.

   This implementation is taken from the book \b More \b Effective \b C++
   (Scott Meyers).
   \warning
   - T must inherit from RefCountObject..
*/

#ifdef BOOST_REFCOUNTPTR

  #ifdef BOOST_SHAREDPTR
  /// Using Boost shared_ptr
  #include <boost/shared_ptr.hpp>

  // using boost::shared_ptr;

  template<class T> struct intrusive_deleter
  {
    void operator()(T * p) { if(p) boost::intrusive_ptr_release(p); }
  };

  template<class X>
  boost::shared_ptr<X> make_shared_from_intrusive(X * p)
  {
    if(p) boost::intrusive_ptr_add_ref(p);
    boost::shared_ptr<X> px(p, intrusive_deleter<X>());
    return px;
  }

  template <class T>
  class my_shared_ptr : public boost::shared_ptr<T> {
  public:

    my_shared_ptr(): shared_ptr() { }

    template<class Y>
    explicit my_shared_ptr( Y * p ): shared_ptr<T>( p, intrusive_deleter<Y>() )
    { if(p) boost::intrusive_ptr_add_ref(p);   }

    template<class Y>
    my_shared_ptr(shared_ptr<Y> const & r): shared_ptr(r) // never throws
    {
    }

  };

  /// Tools Reference Counting Pointer
  #define RCPtr my_shared_ptr


  #else // BOOST_INSTRUSIVEPTR
    /// Using Boost intrusive_ptr
  #include <boost/intrusive_ptr.hpp>

  using boost::intrusive_ptr;
  /// Tools Reference Counting Pointer
  #define RCPtr intrusive_ptr

#endif

#else // PGL_SMARTPTR
/// Using home made smart_ptr

PGL_BEGIN_NAMESPACE

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
  RefCountPtr( const RefCountPtr<U>& ptr ) :  __ptr(ptr.get()) { if (__ptr) __ptr->addReference(); }
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
   return operator=(ptr.get());
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

#ifndef PGL_NO_DEPRECATED
  /// Cast operation
  template <class U>
  attribute_deprecated inline RefCountPtr& cast( const RefCountPtr<U>& ptr )
  {
    if (__ptr != ptr.get())
  {
    if (__ptr) __ptr->removeReference();
    __ptr = dynamic_cast<T *>(ptr.get());
    if (__ptr) __ptr->addReference();
  };
    return *this;
  }

  /// Cast operation
  template <class U>
  attribute_deprecated static RefCountPtr Cast( const RefCountPtr<U>& ptr )
  {
    RefCountPtr rptr;
    if (ptr.get() != NULL)return rptr.cast(ptr);
    return rptr;
  }
#endif

  //@}

  /// @name Comparison operators
  //@{

  /// Comparison operator with RefCountPtr<T> \e ptr.
  bool equal( const RefCountPtr& ptr ) const { return __ptr == ptr.__ptr; }

  /// Comparison operator with RefCountPtr<T> \e ptr.
  bool operator==( const RefCountPtr& ptr ) const { return __ptr == ptr.__ptr; }

  /// Comparison operator with RefCountPtr<U> \e ptr.
  template<class U>
  bool operator==( const RefCountPtr<U>& ptr ) const { return __ptr == ptr.get(); }

  /// Comparison operator with pointer \e ptr.
  bool operator==( const T * ptr ) const { return __ptr == ptr; }

  /// Comparison operator with RefCountPtr<T> \e ptr.
  bool not_equal( const RefCountPtr& ptr ) const { return __ptr != ptr.__ptr; }
  bool operator!=( const RefCountPtr& ptr ) const { return __ptr != ptr.__ptr; }

  /// Comparison operator with pointer \e ptr.
  bool operator!=( const T * ptr ) const { return __ptr != ptr; }

  /// Comparison operator with RefCountPtr<T> \e ptr.
  template<class U>
  bool operator!=( const RefCountPtr<U>& ptr ) const { return __ptr != ptr.get(); }

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

  /// Implicit conversion into normal pointer.
  operator T * ( ) const { return __ptr; }

  /// Return a conversion of \e self into T *
  T * get( ) const { return __ptr; }


#ifndef PGL_NO_DEPRECATED
  /// Return a conversion of \e self into bool
  attribute_deprecated bool toBool( ) const { return __ptr != 0; }

  /// Return a conversion of \e self into T *
  attribute_deprecated T * toPtr( ) const { return __ptr; }

  /// Return a conversion of \e self into size_t
  attribute_deprecated size_t toSizeT( ) const { return (size_t)__ptr; }

  /// Return a conversion of \e self into uint_t
  attribute_deprecated uint32_t toUint32( ) const
#if __WORDSIZE == 64
  { return (uintptr_t)__ptr; }
#else
  { return (uint32_t)__ptr; }
#endif
#endif


  //@}
  /// @name Nullness testing operators
  //@{

  /// Returns true if and only if \e self is null.
  bool operator!( ) const { return __ptr == 0; }

#ifndef PGL_NO_DEPRECATED
  /// Returns true if and only if \e self is not null.
  attribute_deprecated bool isValid( ) const { return __ptr != 0; }

  /// Returns true if and only if \e self is null.
  attribute_deprecated bool isNull( ) const { return __ptr == 0; }
#endif

  //@}

private:

  T * __ptr;

}; // RefCountPtr

/* ------------------------------------------------------------------------- */

PGL_END_NAMESPACE

/// Tools Reference Counting Pointer
#define RCPtr PGL(RefCountPtr)

/* ----------------------------------------------------------------------- */

#endif
/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

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

#ifdef WITH_REFCOUNTLISTENER
/**
    \class RefCountListener
  \brief RefCountListener are used to propagate reference count changed.
         For instance, it can be used to link with PyObject representation.
*/
class RefCountObject;
class TOOLS_API RefCountListener {
public:
  virtual ~RefCountListener() { }
  virtual void referenceAdded(RefCountObject *) = 0;
  virtual void referenceRemoved(RefCountObject *) = 0;
  virtual void objectDeleted(RefCountObject *) = 0;
};

#endif

//template<class Policy>
class TOOLS_API RefCountObject
{

public:
  /// @name Constructors
  //@{

  /// Default constructor.
  RefCountObject( ) :
    _ref_count(0)
#ifdef WITH_REFCOUNTLISTENER
  ,_ref_count_listener(0)
#endif
  {
  }

  /// Copy constructor.
  RefCountObject( const RefCountObject& ) :
    _ref_count(0)
#ifdef WITH_REFCOUNTLISTENER
  ,_ref_count_listener(0)
#endif
  {
  }

  //@}

  /// @name Destructor
  //@{

  /// Destructor.
  virtual ~RefCountObject( )
  {
#ifdef WITH_REFCOUNTLISTENER
    if(_ref_count_listener) {
      _ref_count_listener->objectDeleted(this);
      if(_ref_count_listener) { delete _ref_count_listener; _ref_count_listener = NULL; }
    }
#endif
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
#ifdef WITH_REFCOUNTLISTENER
  if(_ref_count_listener) _ref_count_listener->referenceAdded(this);
#endif
  }

  /// Returns the number of reference to \e self.
  inline size_t use_count( ) const
  {
    return _ref_count;
  }


  /// Returns whether \e self is shared.
  inline bool unique( ) const
  {
    return _ref_count == 1;
  }

#ifndef PGL_NO_DEPRECATED
  /// Returns the number of reference to \e self.
  attribute_deprecated inline size_t getReferenceCount( ) const
  {
    return _ref_count;
  }

  /// Returns whether \e self is shared.
  attribute_deprecated inline bool isShared( ) const
  {
    return _ref_count > 1;
  }
#endif

  /// Decrements the reference counter.
  inline void removeReference( )
  {
    size_t refcount = --_ref_count;
#ifdef RCOBJECT_DEBUG
    std::cerr << this << " ref-- => " << getReferenceCount();
    std::cerr << "\t(" << typeid(*this).name() << ")" << std::endl;
#endif
#ifdef WITH_REFCOUNTLISTENER
  if(_ref_count_listener) _ref_count_listener->referenceRemoved(this);
#endif
    if (refcount == 0) delete this;
  }

  //@}

  /// Returns unique id.
  inline size_t uid( ) const { return (size_t)this; }


#ifdef WITH_REFCOUNTLISTENER
  inline void setRefCountListener(RefCountListener * r)
  {  _ref_count_listener = r;   }

  inline RefCountListener * getRefCountListener() const
  { return _ref_count_listener; }
#endif

private:

  std::atomic<size_t> _ref_count;

#ifdef WITH_REFCOUNTLISTENER
  RefCountListener * _ref_count_listener;
#endif

}; // RefCountObject

typedef RefCountPtr<RefCountObject> RefCountObjectPtr;

PGL_END_NAMESPACE

#ifdef BOOST_REFCOUNTPTR
    // Use of intrusive_ptr or shared_ptr
  namespace boost
  {
    inline void intrusive_ptr_add_ref(RefCountObject * obj) { obj->addReference(); }
    inline void intrusive_ptr_release(RefCountObject * obj) { obj->removeReference(); }
  };
#endif


/// dynamic_pointer_cast
#ifdef BOOST_REFCOUNTPTR
using boost::dynamic_pointer_cast;
using boost::static_pointer_cast;
using boost::const_pointer_cast;
#else
template <class T, class U>
RCPtr<T> dynamic_pointer_cast(const RCPtr<U>& r) {  return RCPtr<T>(dynamic_cast<T *>(r.get())); }

template <class T, class U>
RCPtr<T> static_pointer_cast(const RCPtr<U>& r) {  return RCPtr<T>(static_cast<T *>(r.get())); }

template <class T, class U>
RCPtr<T> const_pointer_cast(const RCPtr<U>& r) {  return RCPtr<T>(const_cast<T *>(r.get())); }
#endif


/// Return a conversion of \e ptr into size_t
template <class U>
inline size_t ptr_to_size_t( const RCPtr<U>& p ) { return (size_t)p.get(); }

/// Return a conversion of \e ptr into uint_t
template <class U>
inline uint32_t ptr_to_uint32( const RCPtr<U>& p )
  { return (uintptr_t)p.get(); }

/// Returns true if and only if \e ptr is not null.
template <class U>
inline bool is_valid_ptr( const RCPtr<U>& p ) { return p.get() != 0; }

/// Returns true if and only if \e ptr is null.
template <class U>
inline bool is_null_ptr( const RCPtr<U>& p ) { return p.get() == 0; }

/* ----------------------------------------------------------------------- */

// __rcobject_h__
#endif
