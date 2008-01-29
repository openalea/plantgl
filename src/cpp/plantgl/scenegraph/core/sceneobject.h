/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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

/*! \file scne_sceneobject.h
    \brief Definition of the scene class SceneObject.
*/

#ifndef __scne_sceneobject_h__
#define __scne_sceneobject_h__

/* ----------------------------------------------------------------------- */

#include "../sg_config.h"

#include <plantgl/scenegraph/core/action.h>
#include <plantgl/tool/rcobject.h>
#include <string>
#include <iostream>


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

// Forward declaration

class SceneObject;
typedef RCPtr<SceneObject> SceneObjectPtr;
class SceneObjectSymbolTable;

/* ----------------------------------------------------------------------- */

/**
    \class SceneObject
    \brief Abstract base class for objects of type of \c Geometry and
    \c Material.
*/

/* ----------------------------------------------------------------------- */

class SG_API SceneObject : public virtual TOOLS(RefCountObject)
{

public:

     /// A stream in which builder put their comment message.
     static std::ostream * commentStream;

     /// A stream in which builder put their warning message.
     static std::ostream * warningStream;

     /// A stream in which builder put their error message.
     static std::ostream * errorStream;

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
  virtual size_t getId( ) const ;

  /// Returns the name of \e self.
  const std::string& getName( ) const ;

  /// Returns whether \e self is named.
  bool isNamed( ) const ;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const  = 0;

   /// Sets the name of \e self.
  void setName( const std::string& name );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const = 0 ;

protected:

  /// Self's name
  std::string __name;

}; // class SceneObject

/// SceneObject Pointer
typedef RCPtr<SceneObject> SceneObjectPtr;

#define gerr *SceneObject::errorStream
#define gwarning *SceneObject::warningStream
#define gcomment *SceneObject::commentStream

/* ------------------------------------------------------------------------- */

PGL_END_NAMESPACE;

/* ------------------------------------------------------------------------- */

// __scne_sceneobject_h__
#endif

