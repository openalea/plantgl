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



#include "texture.h"
#include <plantgl/scenegraph/core/pgl_messages.h>

#include <iostream>
#include <fstream>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

bool ImageTexture::DEFAULT_MIPMAPING(true);


/* ----------------------------------------------------------------------- */


ImageTexture::Builder::Builder() :
  Material::Builder(),
  FileName(0),
  Mipmaping(0){
}


ImageTexture::Builder::~Builder() {
}


SceneObjectPtr ImageTexture::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new ImageTexture
       (*FileName,
       Mipmaping ? *Mipmaping : DEFAULT_MIPMAPING,
	    Ambient ? *Ambient : DEFAULT_AMBIENT,
        Diffuse ? *Diffuse : DEFAULT_DIFFUSE,
        Specular ? *Specular : DEFAULT_SPECULAR,
        Emission ? *Emission : DEFAULT_EMISSION,
        Shininess ? *Shininess : DEFAULT_SHININESS,
        Transparency ? *Transparency : DEFAULT_TRANSPARENCY));
  return SceneObjectPtr();
}


void ImageTexture::Builder::destroy() {
  if (Ambient) delete Ambient;
  if (Diffuse) delete Diffuse;
  if (Specular) delete Specular;
  if (Emission) delete Emission;
  if (Shininess) delete Shininess;
  if (Transparency) delete Transparency;
  if (FileName) delete FileName;
  if (Mipmaping) delete Mipmaping;
}



bool ImageTexture::Builder::isValid( ) const {

  if(!MaterialValidity())return false;

    if (! FileName) {
        pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"ImageTexture","FileName");
        return false;
    };
	ifstream _file(FileName->c_str());
    if (_file) return true;
    string _mess;
    _mess = "Cannot open " + *FileName + ".";
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"ImageTexture","FileName",_mess.c_str());
    return false;

}


/* ----------------------------------------------------------------------- */

ImageTexture::ImageTexture():
  Material() {
}
  
ImageTexture::ImageTexture(   const std::string& filename,
                    bool mipmaping,
				    const Color3& ambient,
                    const real_t& diffuse,
                    const Color3& specular,
                    const Color3& emission,
                    const real_t& shininess,
                    const real_t& transparency ) :
  Material( ambient,diffuse,specular,emission,shininess,transparency),
	__filename(filename), __mipmaping(mipmaping) {
  GEOM_ASSERT(isValid());
}

  ImageTexture::ImageTexture( const std::string& filename,
                    const std::string& name,
                    bool mipmaping,
                    const Color3& ambient,
                    const real_t& diffuse,
                    const Color3& specular,
                    const Color3& emission,
                    const real_t& shininess,
                    const real_t& transparency ) :
  Material( ambient,diffuse,specular,emission,shininess,transparency),
	__filename(filename), __mipmaping(mipmaping) {
  setName(name);
  GEOM_ASSERT(isValid());
}

ImageTexture::~ImageTexture( ) {
}


bool ImageTexture::isValid( ) const {
  Builder _builder;
  _builder.Ambient = const_cast<Color3 *>(&__ambient);
  _builder.Diffuse = const_cast<real_t *>(&__diffuse);
  _builder.Shininess = const_cast<real_t *>(&__shininess);
  _builder.Transparency = const_cast<real_t *>(&__transparency);
  _builder.FileName = const_cast<std::string *>(&__filename);
  return _builder.isValid();
}

SceneObjectPtr ImageTexture::copy(DeepCopier& copier) const
{
  return SceneObjectPtr(new ImageTexture(*this));
}
/* ----------------------------------------------------------------------- */

/// Returns \b Filename value.
const string& ImageTexture::getFilename( ) const {
  return __filename;
}

/// Returns \b Filename field.
string& ImageTexture::getFilename( ) {
  return __filename;
}


bool ImageTexture::isTexture() const {
  return true;
}
