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

  /// Return whether self should be rendered dynamically
  virtual bool hasDynamicRendering() const  = 0;


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

  /// Return whether self should be rendered dynamically
  bool hasDynamicRendering() const
  { return (is_null_ptr(geometry)?false:geometry->hasDynamicRendering()); }

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

