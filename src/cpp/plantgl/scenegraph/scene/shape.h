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


/*! \file scne_shape.h
    \brief Definition of the scene class Shape3D and Shape.
*/


#ifndef __scne_shape_h__
#define __scne_shape_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/scenegraph/core/sceneobject.h>

#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/appearance/texture.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class Shape3D
    \brief Abstract base class for shape that can be stored into a scene. 
*/

/* ------------------------------------------------------------------------- */

class SG_API Shape3D : public SceneObject
{
public:

  /** A structure which helps to build a Shape3D when parsing.
      Fields are normally allocated while parsing and are free when the
      object is contructed using the member \c destroy. */

    struct SG_API Builder : public SceneObject::Builder {

            /// A constructor. It inits all the pointers to null.
            Builder( );

            /// A destructor.
            virtual ~Builder( );

            /** Returns a pointer to a SceneObject constructed using the values
                contained within \e self. If \e self is not valid, it returns a
                null pointer. */
            virtual SceneObjectPtr build( ) = 0;
    };

  /// Default constructor.
  Shape3D( ) ;

  /// Destructor
  virtual ~Shape3D() ;


  /** Applies the action \e action to \e self.
      For each shape within \e self, it applies the action first to the
      Geometry part and second to the Appearance part. */
    virtual bool applyGeometryFirst( Action& action ) = 0 ;

  /** Applies the action \e action to \e self.
      It applies the action to the Geometry part only. The Appearance part
      is skipped. */
    virtual bool applyGeometryOnly( Action& action ) = 0 ;

   /** Applies the action \e action to \e self.
      It applies the action first to the Appearance part and second to the
      Geometry part. */
   virtual bool applyAppearanceFirst( Action& action ) = 0 ;

  /** Applies the action \e action to \e self.
      It applies the action to the Appearance part only. The Geometry
      part is skipped. */
    virtual bool applyAppearanceOnly( Action& action ) = 0;

    /// Prints \e s to the output stream \e stream.
//    friend ostream& operator<<( ostream& stream, Shape3D& s );

}; // class Shape3D

/// Shape3D Pointer
typedef RCPtr<Shape3D> Shape3DPtr;

/* ------------------------------------------------------------------------- */

/**
    \class Shape
    \brief A Shape is composed of a Geometry object and an Appearance object.
      There is an optional id to identy the shape and parent id to store shape relationship.
*/
class SG_API Shape : public Shape3D
{

public:

  /** A structure which helps to build a Geometry when parsing.
      Fields are normally allocated while parsing and are free when the
      object is contructed using the member \c destroy. */

    struct SG_API Builder : public Shape3D::Builder {

            /// A constructor. It inits all the pointers to null.
            Builder( );

            /// A destructor.
            virtual ~Builder( );

            /** Returns a pointer to a Geometry constructed using the values
                contained within \e self. If \e self is not valid, it returns a
                null pointer. */
            virtual SceneObjectPtr build( );

            /// Destroys all the pointers self contains.
            virtual void destroy( );

            /// Returns whether self is valid.
            virtual bool isValid( ) const;

            /// A pointer to the \b appearance field.
            AppearancePtr * Appearance;

            /// A pointer to the \b geometry field.
            GeometryPtr * Geometry;

            /// A pointer to the \b identification field.
            uint_t * Id;

            /// A pointer to the \b parent identification field.
            uint_t * ParentId;
    };

  /// The undef value for the Id field.
  static const uint_t NOID;

  /// Default constructor.
  Shape( );

  /** Constructs a Shape with the Geometry \e geom, the Appearance
      \e app and the id \e id. */
  Shape( const GeometryPtr&  geom,
         const AppearancePtr& app = Material::DEFAULT_MATERIAL,
         uint_t id = NOID,
         uint_t parent_id = NOID);

  /** Constructs a Shape with the Geometry \e geom, the Appearance
      \e app and the id \e id. */
  Shape( const std::string&   name,
		 const GeometryPtr&   geom,
	     const AppearancePtr& app = Material::DEFAULT_MATERIAL,
         uint_t id = NOID,
         uint_t parent_id = NOID );

  /** Constructs a Shape with the Geometry \e geom, the Appearance
      \e app and the id \e id. */
  Shape( const GeometryPtr&  geom,
         const ImageTexturePtr& app,
         uint_t id = NOID,
         uint_t parent_id = NOID);

  /** Constructs a Shape with the Geometry \e geom, the Appearance
      \e app and the id \e id. */
  Shape( const std::string&   name,
		 const GeometryPtr&   geom,
	     const ImageTexturePtr& app,
         uint_t id = NOID,
         uint_t parent_id = NOID );

  /// Destructor
  virtual ~Shape();

  /// Set computed names to \e self , geometry and apperance.
  void setComputedName();

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  PGL_OBJECT(Shape)

  virtual bool applyGeometryFirst( Action& action );

  virtual bool applyGeometryOnly( Action& action );

  virtual bool applyAppearanceFirst( Action& action );

  virtual bool applyAppearanceOnly( Action& action );

  /// Return the Geometry of \e self.
  const GeometryPtr& getGeometry() const;

  /// Return the Geometry of \e self.
  GeometryPtr& getGeometry();

  /// Return the Appearance of \e self.
  const AppearancePtr& getAppearance() const;

  /// Return the Appearance of \e self.
  AppearancePtr& getAppearance();

  /// Return the Id of \e self.
  virtual size_t getId() const;

  /// Return the Id of \e self.
  uint_t& getId();

  /// Return the SceneObject Id of \e self.
  size_t getSceneObjectId() const;

  /// Return the ParentId of \e self.
  uint_t getParentId() const { return parentId; }

  /// Return the ParentId of \e self.
  uint_t& getParentId() { return parentId; }

  /// Return if ParentId is to default.
  bool isParentIdToDefault() { return parentId == NOID; }

  /// The appearance of \e self.
  AppearancePtr appearance;

  /// The geometry of \e self.
  GeometryPtr geometry;

  /// The identification id of \e self.
  uint_t id;

  /// The parent identification id of \e self.
  uint_t parentId;

}; // class Shape

/// Shape Pointer
typedef RCPtr<Shape> ShapePtr;


/* ------------------------------------------------------------------------- */

// __geom_geometry_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

