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


/*! \file geom_text.h
    \brief Definition of the geometry class Text.
*/



#ifndef __geom_text_h__
#define __geom_text_h__

/* ----------------------------------------------------------------------- */

#include "geometry.h"
#include <plantgl/math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class Text
    \brief Some text with font
*/

/* ----------------------------------------------------------------------- */

class SG_API Font : public SceneObject
{
  public:
  /// A structure which helps to build a Text when parsing.
  struct SG_API Builder : public SceneObject::Builder {

    /// A pointer to the \b String field.
    std::string * Family;

    uint_t * Size;

    bool * Bold;

    bool * Italic;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  static const uint_t DEFAULT_SIZE;
  static const bool   DEFAULT_BOLD;
  static const bool   DEFAULT_ITALIC;

  Font(const std::string& family ="",
       uint_t size = DEFAULT_SIZE,
       bool   bold = DEFAULT_BOLD,
       bool italic = DEFAULT_ITALIC);

  ~Font();

  virtual bool isValid( ) const;

  PGL_OBJECT(Font)

  const std::string& getFamily() const
  { return __family; }

  std::string& getFamily()
  { return __family; }

  const uint_t getSize() const
  { return __size; }

  uint_t& getSize()
  { return __size; }

  const bool getBold() const
  { return __bold; }

  bool& getBold()
  { return __bold; }

  const bool getItalic() const
  { return __italic; }

  bool& getItalic()
  { return __italic; }

  bool isSizeToDefault( ) const
  { return __size == DEFAULT_SIZE; }

  bool isBoldToDefault( ) const
  { return __bold == DEFAULT_BOLD; }

  bool isItalicToDefault( ) const
  { return __italic == DEFAULT_ITALIC; }

protected:

  std::string __family;

  uint_t __size;

  bool __bold;

  bool __italic;

};

/// Font Pointer
typedef RCPtr<Font> FontPtr;

/* ----------------------------------------------------------------------- */

class SG_API Text : public Geometry
{

public:

  static bool DEFAULT_SCREEN_COORDINATES;

  static Vector3 DEFAULT_POSITION;

  /// A structure which helps to build a Text when parsing.
  struct SG_API Builder : public Geometry::Builder {

    /// A pointer to the \b String field.
    std::string * String;

    /// A pointer to the \b Position field.
    Vector3 * Position;

    /// A pointer to the \b ScreenCoordinates field.
    bool * ScreenCoordinates;

    /// A pointer to the \b FontStyle field.
    FontPtr * FontStyle;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };


  static const FontPtr DEFAULT_FONT;

  /// Constructs a Box with \e size.
  Text( const std::string& s = "",
        const Vector3& position = DEFAULT_POSITION,
        const bool screenCoordinates = DEFAULT_SCREEN_COORDINATES,
        const FontPtr& font = DEFAULT_FONT);

  /// Destructor
  virtual ~Text( );

  PGL_OBJECT(Text)

  virtual bool isValid( ) const;

  const std::string& getString() const
  { return __String; }

  std::string& getString()
  { return __String; }

  const FontPtr& getFontStyle() const
  { return __fontStyle; }

  FontPtr& getFontStyle()
  { return __fontStyle; }

  bool isFontStyleToDefault() const
  { return __fontStyle == DEFAULT_FONT; }

  bool isACurve() const
  { return false; }

  bool isASurface() const
  { return false; }

  bool isAVolume() const
  { return false; }

  bool isExplicit() const
  { return false; }

  bool hasDynamicRendering() const { return true; }

  inline const bool getScreenCoordinates() const { return __screenCoordinates; }
  inline bool& getScreenCoordinates() { return __screenCoordinates; }
  inline bool isScreenCoordinatesToDefault() { return __screenCoordinates == DEFAULT_SCREEN_COORDINATES; }

  inline const Vector3& getPosition() const { return __position; }
  inline Vector3& getPosition() { return __position; }
  inline bool isPositionToDefault() { return __position == DEFAULT_POSITION; }

protected:

  /// The text field.
  std::string __String;

  bool __screenCoordinates;

  Vector3 __position;

  FontPtr __fontStyle;

};

/// Text Pointer
typedef RCPtr<Text> TextPtr;

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif // __geom_text_h__

