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

