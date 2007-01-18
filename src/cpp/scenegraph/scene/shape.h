/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (boudon@cirad.fr) nouguier
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


/*! \file scne_shape.h
    \brief Definition of the scene class GeomShape3D and GeomShape.
*/


#ifndef __scne_shape_h__
#define __scne_shape_h__

/* ----------------------------------------------------------------------- */

#include "scne_sceneobject.h"

#ifndef GEOM_FWDEF
#include "geom_geometry.h"
#endif
#include "appe_material.h"

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

#ifdef GEOM_FWDEF
class Geometry;
typedef RCPtr<Geometry> GeometryPtr;
#endif

/* ------------------------------------------------------------------------- */

/**
    \class GeomShape3D
    \brief A GeomShape is composed of a Geometry object and an Appearance object.
      There is an optional id to identy the shape. We use it when
      building a Scene from AMAPmod to store the id of the corresponding
      Vertex.
*/

/* ------------------------------------------------------------------------- */

class GEOM_API GeomShape3D : public SceneObject
{
public:

  /** A structure which helps to build a GeomShape3D when parsing.
      Fields are normally allocated while parsing and are free when the
      object is contructed using the member \c destroy. */

    struct Builder : public SceneObject::Builder {

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
  GeomShape3D( ) ;

  /// Destructor
  virtual ~GeomShape3D() ;


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
//    friend ostream& operator<<( ostream& stream, GeomShape3D& s );

}; // class Shape3D

/// GeomShape3D Pointer
typedef RCPtr<GeomShape3D> GeomShape3DPtr;

/* ------------------------------------------------------------------------- */

/**
    \class GeomShape
    \brief A GeomShape is composed of a Geometry object and an Appearance object.
      There is an optional id to identy the shape. We use it when
      building a Scene from AMAPmod to store the id of the corresponding
      Vertex.
*/
class GEOM_API GeomShape : public GeomShape3D
{

public:

  /** A structure which helps to build a Geometry when parsing.
      Fields are normally allocated while parsing and are free when the
      object is contructed using the member \c destroy. */

    struct Builder : public GeomShape3D::Builder {

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
            uint32_t * Id;
    };

  /// The undef value for the Id field.
  static const uint32_t NOID;

  /// Default constructor.
  GeomShape( );

  /** Constructs a Shape with the Geometry \e _geom, the Appearance
      \e _app and the id \e _id. */
  GeomShape( const GeometryPtr& _geom,
             const AppearancePtr& _app = Material::DEFAULT_MATERIAL,
             uint32_t _id=NOID);

  /** Constructs a Shape with the Geometry \e _geom, the Appearance
      \e _app and the id \e _id. */
  GeomShape( const std::string&   name,
		 const GeometryPtr&   geom,
	     const AppearancePtr& app = Material::DEFAULT_MATERIAL,
         uint32_t id = NOID );

  /// Destructor
  virtual ~GeomShape();

  /// Set computed names to \e self , geometry and apperance.
  void setComputedName();

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

    /// Applies the action \e action to \e self.
  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

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
  virtual uint32_t getId() const;

  /// Return the Id of \e self.
  uint32_t& getId();

  /// The appearance of \e self.
  AppearancePtr appearance;

  /// The geometry of \e self.
  GeometryPtr geometry;

  /// The identification id of \e self.
  uint32_t id;

}; // class GeomShape

/// GeomShape Pointer
typedef RCPtr<GeomShape> GeomShapePtr;


/* ------------------------------------------------------------------------- */

// __geom_geometry_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/// Write the GeomShape3D \b s in \b stream.
GEOM_API std::ostream& operator<<( std::ostream& stream, GEOM(GeomShape3D&) s );

/* ----------------------------------------------------------------------- */
#endif

