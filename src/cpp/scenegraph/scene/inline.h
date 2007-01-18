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


/*! \file scne_inline.h
    \brief Definition of the scene class GeomInline.
*/


#ifndef __scne_inline_h__
#define __scne_inline_h__

/* ----------------------------------------------------------------------- */

#include "scne_shape.h"
#include "Tools/util_vector.h"
#ifndef GEOM_FWDEF
#include "scne_scene.h"
#include "geom_boundingbox.h"
#endif
/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


#ifdef GEOM_FWDEF
class Scene;
typedef RCPtr<Scene> ScenePtr;
class BoundingBox;
typedef RCPtr<BoundingBox> BoundingBoxPtr;
#endif

/* ----------------------------------------------------------------------- */

/**
   \class GeomInline
   \brief A 3D Scene represented by a list of objects
   of type of Shape which are positionned and .
*/

/* ----------------------------------------------------------------------- */

class GEOM_API GeomInline : public GeomShape3D{

public:

  /** A structure which helps to build a GeomInline when parsing.
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
	    
	    /// A pointer to the \b FileName field.
		std::string * FileName;
	    
	    /// A pointer to the \b Scene field.
	    ScenePtr * Scene;
	    
	    /// A pointer to the \b BBoxCenter field.
	    TOOLS(Vector3) * BBoxCenter;
	    TOOLS(Vector3) * Translation;
	    
	    /// A pointer to the \b BBoxSize field.
	    TOOLS(Vector3) * BBoxSize;
	    TOOLS(Vector3) * Scale;
    };

  /// Default constructor.
  GeomInline(const std::string& filename);

  /// Constructor.
  GeomInline(const std::string& filename, const TOOLS(Vector3)& bboxCenter);


  /// Constructor.
  GeomInline(const TOOLS(Vector3)& bboxSize, const std::string& filename);

  /// Constructor.
  GeomInline(const std::string& filename, const TOOLS(Vector3)& bboxCenter, const TOOLS(Vector3)& bboxSize);

  /// Constructor.
  GeomInline(const ScenePtr& scene);

  /// Constructor.
  GeomInline(const ScenePtr& scene, const TOOLS(Vector3)& bboxCenter);


  /// Constructor.
  GeomInline(const TOOLS(Vector3)& bboxSize, const ScenePtr& scene);

  /// Constructor.
  GeomInline(const ScenePtr& scene, const TOOLS(Vector3)& bboxCenter, const TOOLS(Vector3)& bboxSize);

  /// Destructor.
  virtual ~GeomInline();

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

  /// Return FileName value.
  const std::string& getFileName() const;

  /// Return BBox Center value.
  const TOOLS(Vector3) getBBoxCenter() const;

  /// Return whether BBox Center has default value.
  bool isBBoxCenterToDefault() const;

  /// Return BBox Size value.
  const TOOLS(Vector3) getBBoxSize() const;

    /// Return whether BBox Size has default value.
  bool isBBoxSizeToDefault() const ;

  /// Return Sub Scene value.
  const ScenePtr& getScene() const;

  /// Initialize Bounding Box value if none is specified.
  void initBBox();


  /// Return the Real Bouding Box of the Scene.
  const BoundingBoxPtr& getRealBBox() const;

  /// Return the translation value.
  TOOLS(Vector3) getTranslation() const;

  /// Return the Scale value.
  TOOLS(Vector3) getScale() const;
    
  /// set the translation value.
  void setTranslation(const TOOLS(Vector3)&) ;
  
  /// set the Scale value.
  void setScale(const TOOLS(Vector3)&) ;
    
protected :

  /// The file name
  std::string __filename;

  /// The center of the bouding box
  TOOLS(Vector3) __bboxCenter;

  /// The size of the bouding box
  TOOLS(Vector3) __bboxSize;

  /// If bounding box info are filled
  uchar_t __filled;

  /// The subscene.
  ScenePtr __scene;

  /// The BoundingBox if the subscene.
  BoundingBoxPtr __bbox;


};

/// GeomInline Pointer
typedef RCPtr<GeomInline> GeomInlinePtr;

/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

