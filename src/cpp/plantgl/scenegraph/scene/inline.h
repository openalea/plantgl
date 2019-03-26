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


/*! \file scne_inline.h
    \brief Definition of the scene class Inline.
*/


#ifndef __scne_inline_h__
#define __scne_inline_h__

/* ----------------------------------------------------------------------- */

#include "shape.h"
#include <plantgl/math/util_vector.h>
#include <string>

#ifndef GEOM_FWDEF
#include "scene.h"
#endif
/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


#ifdef GEOM_FWDEF
class Scene;
typedef RCPtr<Scene> ScenePtr;
#endif

/* ----------------------------------------------------------------------- */

/**
   \class Inline
   \brief A 3D Scene represented by a list of objects
   of type of Shape which are positionned and .
*/

/* ----------------------------------------------------------------------- */

class SG_API Inline : public Shape3D {

public:

    // The default translation value
    static const Vector3 DEFAULT_TRANSLATION;

    // The default scaling value
    static const Vector3 DEFAULT_SCALE;

  /** A structure which helps to build a Inline when parsing.
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

        /// A pointer to the \b FileName field.
        std::string * FileName;

        /// A pointer to the \b Scene field.
        ScenePtr * Scene;

        /// A pointer to the \b Translation field.
        Vector3 * Translation;

        /// A pointer to the \b Scale field.
        Vector3 * Scale;
    };

  /// Constructor.
  Inline(const std::string& filename,
         const Vector3& translation = DEFAULT_TRANSLATION,
         const Vector3& size = DEFAULT_SCALE);

  /// Destructor.
  virtual ~Inline();

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  PGL_OBJECT(Inline)

  virtual bool applyGeometryFirst( Action& action );

  virtual bool applyGeometryOnly( Action& action );

  virtual bool applyAppearanceFirst( Action& action );

  virtual bool applyAppearanceOnly( Action& action );

  /// Return FileName value.
  const std::string& getFileName() const;

  /// Return Sub Scene value.
  inline const ScenePtr& getScene() const { return __scene; }

  /// Return the translation value.
  inline const Vector3& getTranslation() const { return __translation; }

  /// Return the Scale value.
  inline const Vector3& getScale() const { return __scale; }

  /// Return the translation value.
  inline Vector3& getTranslation() { return __translation; }

  /// Return the Scale value.
  inline Vector3& getScale() { return __scale; }

  /// Tell whether Translation value is to default
  inline bool isTranslationToDefault() const { return __translation == DEFAULT_TRANSLATION; }

  /// Tell whether Scale value is to default
  inline bool isScaleToDefault() const { return __scale == DEFAULT_SCALE; }

  /// set a default name to \e this
  void setDefaultName();

  /// Return whether self should be rendered dynamically
  virtual bool hasDynamicRendering() const ;

protected :

  /// The file name
  std::string __filename;

  /// The translation
  Vector3 __translation;

  /// The scale
  Vector3 __scale;

  /// The subscene.
  ScenePtr __scene;

};

/// Inline Pointer
typedef RCPtr<Inline> InlinePtr;

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

