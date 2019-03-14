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

#include "text.h"
#include <plantgl/scenegraph/core/pgl_messages.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

const uint_t Font::DEFAULT_SIZE(10);
const bool   Font::DEFAULT_BOLD(false);
const bool   Font::DEFAULT_ITALIC(false);

const FontPtr Text::DEFAULT_FONT;

/* ----------------------------------------------------------------------- */

/// Constructor.
Font::Builder::Builder( ):
	  SceneObject::Builder( ),
	  Family(0),
	  Size(0),
	  Bold(0),
	  Italic(0){
}

    /// Destructor.
Font::Builder::~Builder( ){
}

SceneObjectPtr Font::Builder::build( ) const{
  if (isValid()) 
    return SceneObjectPtr
	(new Font((Family ? *Family : ""),
			  (Size ? *Size : Font::DEFAULT_SIZE),
			  (Bold ? *Bold : Font::DEFAULT_BOLD),
			  (Italic ? *Italic : Font::DEFAULT_ITALIC)));
  return SceneObjectPtr();
}

void Font::Builder::destroy( ){
  if(Family)    delete Family;
  if(Size)      delete Size;
  if(Bold)      delete Bold;
  if(Italic)    delete Italic;
}
    
bool Font::Builder::isValid( ) const
{
  if(Size && *Size < 6){
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Text","Size","Font Size should be greater than 6. ");
	return false;
  }
  return true;
}

/* ----------------------------------------------------------------------- */

Font::Font(const std::string& family, 
	 uint_t size,
	 bool bold,
	 bool italic):
  SceneObject(),
__family(family),
__size(size),
__bold(bold),
__italic(italic){
}

Font::~Font(){}

bool  Font::isValid( ) const{
  Builder _builder;
  _builder.Family = const_cast<std::string *>(&__family);
  _builder.Size = const_cast<uint_t *>(&__size);
  _builder.Bold = const_cast<bool *>(&__bold);
  _builder.Italic = const_cast<bool *>(&__italic);
  return _builder.isValid();
}

SceneObjectPtr Font::copy(DeepCopier& copier) const {
  return SceneObjectPtr(new Font(*this));
}

/* ----------------------------------------------------------------------- */

bool Text::DEFAULT_SCREEN_COORDINATES(false);

Vector3 Text::DEFAULT_POSITION(0,0,0);

/* ----------------------------------------------------------------------- */

/// Constructor.
Text::Builder::Builder( ):
  Geometry::Builder(),
  String(0),
  Position(0),
  ScreenCoordinates(0),
  FontStyle(0)
{}

    /// Destructor.
Text::Builder::~Builder( ){}

SceneObjectPtr 
Text::Builder::build( ) const {
  if(isValid()) 
    return SceneObjectPtr
	(new Text((String ? *String : ""),
			  (Position ? *Position : DEFAULT_POSITION),
			  (ScreenCoordinates ? *ScreenCoordinates : DEFAULT_SCREEN_COORDINATES),
			  (FontStyle ? *FontStyle : Text::DEFAULT_FONT)));
  return SceneObjectPtr();
}

void 
Text::Builder::destroy( ){
  if(String) delete String;
  if(FontStyle) delete FontStyle;
}
    
bool Text::Builder::isValid( ) const{
    if (!String) {
        pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Text","String");
        return false;
    };
    if (String->empty()) {
        pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Text","String");
        return false;
    };
	if(FontStyle && *FontStyle && !(*FontStyle)->isValid()){
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Text","FontStyle","");
	  return false;
	}
	return true;
}



  /// Constructs a Box with \e size.
Text::Text( const std::string& s,
			const Vector3& position,
			const bool screenCoordinates,
		    const FontPtr& font):
  Geometry(),
__String(s),
__screenCoordinates(screenCoordinates),
__position(position),
__fontStyle(font){}

  /// Destructor
Text::~Text( ) {}


SceneObjectPtr Text::copy(DeepCopier& copier) const {
  Text * t = new Text(*this);
  copier.copy_object_attribute(t->getFontStyle());
  return SceneObjectPtr(t);
}

bool Text::isValid( ) const{
  Builder _builder;
  _builder.String = const_cast<std::string *>(&__String);
  _builder.FontStyle = const_cast<FontPtr *>(&__fontStyle);
  return _builder.isValid();
}


/* ----------------------------------------------------------------------- */
