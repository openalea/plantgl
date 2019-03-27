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
