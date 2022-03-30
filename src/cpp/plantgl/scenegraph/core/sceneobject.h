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


/*! \file scne_sceneobject.h
    \brief Definition of the scene class SceneObject.
*/

#ifndef __scne_sceneobject_h__
#define __scne_sceneobject_h__

/* ----------------------------------------------------------------------- */

#include "../sg_config.h"

#include <plantgl/scenegraph/core/action.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/util_hashmap.h>
#include "deepcopier.h"
#include "pgl_messages.h"
#include <string>
#include <iostream>


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

// Forward declaration

class SceneObject;
typedef RCPtr<SceneObject> SceneObjectPtr;
class SceneObjectSymbolTable;
class DeepCopier;

/* ----------------------------------------------------------------------- */

/**
    \class SceneObject
    \brief Abstract base class for objects of type of \c Geometry and
    \c Material.
*/

/* ----------------------------------------------------------------------- */

class SG_API SceneObject : public virtual RefCountObject
{

public:
     friend class DeepCopier;


/*     /// A stream in which builder put their comment message.
     static std::ostream * commentStream;

     /// A stream in which builder put their warning message.
     static std::ostream * warningStream;

     /// A stream in which builder put their error message.
     static std::ostream * errorStream;*/

  /*! A structure which helps to build an object of type of \c SceneObject.

      \par More about Builders.

      \b Builders are mainly used when parsing in order to construct objects
      of type of \c SceneObject. They are designed to address file syntax
      requirements:
      - Fields can be set in any order;
      - Optional fields can remain undefined;
      - Fields validity and consistency can be checked.

      \par Setting fields.
      Fields are set by allocating the corresponding pointer with the desired
      value. This way, checking whether a field has been allocated or not can
      determine whether this field has been set or not.

      \par Deleting fields.
      It can be done using the member \c destroy. It ensures that all the
      allocated fields will be destroyed.

      \par Checking fields.
      Fields validity and consistency is done using the member \c isValid. If
      the builder is not valid, information messages will be written in the
      defined \c clog stream.

      \par Building objects.
      Objects are built using the member \c build. The builder ensures that
      all optional fields which are still undefined will be replaced by their
      default values. In the case the builder is not valid, a \c null pointer
      is returned.
  */
  struct Builder {

    /// A constructor. It inits all the pointers to \c null.
    Builder( );

    /// A destructor.
    virtual ~Builder( );

    /// Destroys all the pointers \e self contains.
    virtual void destroy( ) = 0;

    /// Returns whether \e self is valid.
    virtual bool isValid( ) const = 0;

  };


  /** Default constructor.
      By default, the object is unnamed. */
  SceneObject( ) :
        RefCountObject(),
        __name() {
  }

  /** Constructor.
      The object is named \e name. */
  SceneObject(const std::string& name ) :
    RefCountObject(),
        __name(name) {
  }

  /// Destructor
  virtual ~SceneObject( );

  /// Applies the action \e action to \e self.
  virtual bool apply( Action& action ) = 0;

  /// Returns a unique id to identify \e self.
  virtual size_t getObjectId( ) const ;

  /// Returns the name of \e self.
  const std::string& getName( ) const ;

  /// Returns whether \e self is named.
  bool isNamed( ) const ;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const  = 0;

   /// Sets the name of \e self.
  void setName( const std::string& name );

   /// Sets the name of \e self to a default value.
  void setDefaultName();

  /// Deep copy of \e this.
  SceneObjectPtr deepcopy() const;

  /// Deep copy of \e this.
  SceneObjectPtr deepcopy(DeepCopier&) const;

  /// Deep copy and cast of \e this.
  template<class T>
  RCPtr<T> casted_deepcopy_using(DeepCopier&m) const{ return dynamic_pointer_cast<T>(deepcopy(m)); }

  template<class T>
  RCPtr<T> casted_deepcopy() const{ return dynamic_pointer_cast<T>(deepcopy()); }

protected:

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy(DeepCopier&) const = 0 ;


  /// Self's name
  std::string __name;

}; // class SceneObject

/// SceneObject Pointer
typedef RCPtr<SceneObject> SceneObjectPtr;

#define gerr *SceneObject::errorStream
#define gwarning *SceneObject::warningStream
#define gcomment *SceneObject::commentStream


#define PGL_OBJECT(T) \
    public: \
    virtual bool apply( Action& action ) { return action.process(this); } \
    protected: \
    virtual SceneObjectPtr copy(DeepCopier&) const; \
    public: \

#define PGL_OBJECT_PROPERTY(PROPNAME,PROPTYPE) \
    inline const PROPTYPE& get##PROPNAME() const { return __##PROPNAME; } \
    inline PROPTYPE& get##PROPNAME() { return __##PROPNAME; } \
    inline void set##PROPNAME(const PROPTYPE& value) { __##PROPNAME = value; } \
    protected: \
    PROPTYPE __##PROPNAME; \
    public:

#define PGL_OBJECT_PROPERTY_WITH_DEFAULT(PROPNAME,PROPTYPE,DEFAULTPROPVALUE) \
    PGL_OBJECT_PROPERTY(PROPNAME,PROPTYPE) \
    inline bool is##PROPNAME##ToDefault() const { return __##PROPNAME == DEFAULTPROPVALUE; }

/* ------------------------------------------------------------------------- */

PGL_END_NAMESPACE;

/* ------------------------------------------------------------------------- */

// __scne_sceneobject_h__
#endif

