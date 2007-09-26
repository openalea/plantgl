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

	uint32_t * Size;

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

  static const uint32_t DEFAULT_SIZE;
  static const bool   DEFAULT_BOLD;
  static const bool   DEFAULT_ITALIC;

  Font(const std::string& family ="", 
	   uint32_t size = DEFAULT_SIZE,
	   bool   bold = DEFAULT_BOLD,
	   bool italic = DEFAULT_ITALIC);

  ~Font();

  virtual bool isValid( ) const;

  virtual SceneObjectPtr copy() const;

  virtual bool apply( Action& action );

  const std::string& getFamily() const
  { return __family; }

  std::string& getFamily()
  { return __family; }

  const uint32_t getSize() const
  { return __size; }

  uint32_t& getSize()
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

  uint32_t __size;

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

  static TOOLS(Vector3) DEFAULT_POSITION;

  /// A structure which helps to build a Text when parsing. 
  struct SG_API Builder : public Geometry::Builder {

    /// A pointer to the \b String field.
	std::string * String;

    /// A pointer to the \b Position field.
	TOOLS(Vector3) * Position;	

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
	    const TOOLS(Vector3)& position = DEFAULT_POSITION,
		const bool screenCoordinates = DEFAULT_SCREEN_COORDINATES,
		const FontPtr& font = DEFAULT_FONT);

  /// Destructor
  virtual ~Text( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

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

  inline const bool getScreenCoordinates() const { return __screenCoordinates; }
  inline bool& getScreenCoordinates() { return __screenCoordinates; }
  inline bool isScreenCoordinatesToDefault() { return __screenCoordinates == DEFAULT_SCREEN_COORDINATES; }

  inline const TOOLS(Vector3)& getPosition() const { return __position; }
  inline TOOLS(Vector3)& getPosition() { return __position; }
  inline bool isPositionToDefault() { return __position == DEFAULT_POSITION; }

protected:

  /// The text field.
  std::string __String;

  bool __screenCoordinates;

  TOOLS(Vector3) __position;

  FontPtr __fontStyle;
  
};

/// Text Pointer
typedef RCPtr<Text> TextPtr;

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif // __geom_text_h__

