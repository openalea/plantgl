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
#include <plantgl/math/util_math.h>

#include <iostream>
#include <fstream>

PGL_USING_NAMESPACE
TOOLS_USING(Vector2)
using namespace std;

/* ----------------------------------------------------------------------- */

const Vector2 Texture2DTransformation::DEFAULT_SCALE(1,1);
const Vector2 Texture2DTransformation::DEFAULT_TRANSLATION(0,0);
const Vector2 Texture2DTransformation::DEFAULT_ROTATIONCENTER(0.5,0.5);
const real_t  Texture2DTransformation::DEFAULT_ROTATIONANGLE(0);

/* ----------------------------------------------------------------------- */


Texture2DTransformation::Builder::Builder() :
  SceneObject::Builder(),
  Scale(0),
  Translation(0),
  RotationCenter(0),
  RotationAngle(0){
}


Texture2DTransformation::Builder::~Builder() {
}


SceneObjectPtr Texture2DTransformation::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Texture2DTransformation
       (Scale ? *Scale : DEFAULT_SCALE,
         Translation ? *Translation : DEFAULT_TRANSLATION,
         RotationCenter ? *RotationCenter : DEFAULT_ROTATIONCENTER,
         RotationAngle ? *RotationAngle : DEFAULT_ROTATIONANGLE));
  return SceneObjectPtr();
}


void Texture2DTransformation::Builder::destroy() {
  if (Scale) delete Scale;
  if (Translation) delete Translation;
  if (RotationCenter) delete RotationCenter;
  if (RotationAngle) delete RotationAngle;
}



bool Texture2DTransformation::Builder::isValid( ) const {

  /// Scaling
  if (Scale){
    if (!Scale->isValid()){
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),
				  "Texture2DTransformation","Scale","Must be valid.");
      return false;
	}
    if (!(fabs(Scale->x()) > GEOM_EPSILON) || !(fabs(Scale->y()) > GEOM_EPSILON)){
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),
				 "Texture2DTransformation","Scale","Must be positive.");
      return false;
	}
  }

  /// Translation
  if (Translation){
    if (!Translation->isValid()){
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),
				 "Texture2DTransformation","Translation","Must be valid.");
      return false;
	}
  }

  /// RotationCenter
  if (RotationCenter){
    if (!RotationCenter->isValid()){
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),
				 "Texture2DTransformation","RotationCenter","Must be valid.");
      return false;
	}
  }

  /// RotationAngle
  if (RotationAngle){
    if (!finite(*RotationAngle) ){
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),
				 "Texture2DTransformation","RotationAngle","Must be valid.");
      return false;
	}
  }
  return true;

}

/* ----------------------------------------------------------------------- */

Texture2DTransformation::Texture2DTransformation(const TOOLS(Vector2)& scaling, 
											 const TOOLS(Vector2)& translation, 
											 const TOOLS(Vector2)& rotationCenter, 
											 real_t rotationAngle):
	SceneObject(),
	__Scale(scaling), 
	__Translation(translation),
	__RotationCenter(rotationCenter),
	__RotationAngle(rotationAngle)  {
}
  
Texture2DTransformation::Texture2DTransformation(const std::string& name,
										     const TOOLS(Vector2)& scaling, 
											 const TOOLS(Vector2)& translation, 
											 const TOOLS(Vector2)& rotationCenter, 
											 real_t rotationAngle):
	SceneObject(name),
	__Scale(scaling), 
	__Translation(translation),
	__RotationCenter(rotationCenter),
	__RotationAngle(rotationAngle)  {
}


Texture2DTransformation::~Texture2DTransformation( ) {
}


bool Texture2DTransformation::isValid( ) const {
  Builder _builder;
  _builder.Scale = const_cast<Vector2 *>(&__Scale);
  _builder.Translation = const_cast<Vector2 *>(&__Translation);
  _builder.RotationCenter = const_cast<Vector2 *>(&__RotationCenter);
  _builder.RotationAngle = const_cast<real_t *>(&__RotationAngle);
  return _builder.isValid();
}

SceneObjectPtr Texture2DTransformation::copy(DeepCopier& copier) const
{
  return SceneObjectPtr(new Texture2DTransformation(*this));
}
/* ----------------------------------------------------------------------- */

const bool ImageTexture::DEFAULT_MIPMAPING(true);
const bool ImageTexture::DEFAULT_REPEATS(true);
const bool ImageTexture::DEFAULT_REPEATT(true);
const real_t ImageTexture::DEFAULT_TRANSPARENCY(0);

/* ----------------------------------------------------------------------- */

ImageTexture::Builder::Builder() :
  SceneObject::Builder(),
  FileName(0),
  RepeatT(0),
  RepeatS(0),
  Mipmaping(0),
  Transparency(0){
}


ImageTexture::Builder::~Builder() {
}


SceneObjectPtr ImageTexture::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new ImageTexture
       (*FileName,
         Transparency ? *Transparency : DEFAULT_TRANSPARENCY,
         RepeatS ? *RepeatS : DEFAULT_REPEATS,
         RepeatT ? *RepeatT : DEFAULT_REPEATT,
         Mipmaping ? *Mipmaping : DEFAULT_MIPMAPING));
  return SceneObjectPtr();
}


void ImageTexture::Builder::destroy() {
  if (FileName) delete FileName;
  if (Mipmaping) delete Mipmaping;
  if (RepeatS) delete RepeatS;
  if (RepeatT) delete RepeatT;
  if (Transparency) delete Transparency;
}



bool ImageTexture::Builder::isValid( ) const {
  /// Transparency
  if (Transparency)
    if (*Transparency < 0 || *Transparency > 1) {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Material","Transparency","Must be in [0,1].");
      return false;
	};

  /// Filename
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
	SceneObject(),
	__Filename(), 
	__Mipmaping(DEFAULT_MIPMAPING), 
	__RepeatS(DEFAULT_REPEATS),
	__RepeatT(DEFAULT_REPEATT),
	__Transparency(DEFAULT_TRANSPARENCY)  {
}
  
ImageTexture::ImageTexture(   const std::string& filename,
						      const real_t& transparency,
							  bool repeatS,
							  bool repeatT,
							  bool mipmaping) :
	SceneObject(),
	__Filename(filename), 
	__Mipmaping(mipmaping), 
	__RepeatS(repeatS),
	__RepeatT(repeatT),
	__Transparency(transparency) {
  GEOM_ASSERT(isValid());
}

  ImageTexture::ImageTexture( const std::string& name,
							  const std::string& filename,
						      const real_t& transparency,
							  bool repeatS,
							  bool repeatT,
							  bool mipmaping ) :
	SceneObject(name),
	__Filename(filename), 
	__Mipmaping(mipmaping), 
	__RepeatS(repeatS),
	__RepeatT(repeatT),
	__Transparency(transparency) {
  GEOM_ASSERT(isValid());
}

ImageTexture::~ImageTexture( ) {
}


bool ImageTexture::isValid( ) const {
  Builder _builder;
  _builder.Transparency = const_cast<real_t *>(&__Transparency);
  _builder.FileName = const_cast<std::string *>(&__Filename);
  return _builder.isValid();
}

SceneObjectPtr ImageTexture::copy(DeepCopier& copier) const
{
  return SceneObjectPtr(new ImageTexture(*this));
}
/* ----------------------------------------------------------------------- */

/*
bool ImageTexture::isSimilar(const Material& other) const
{
	if (!other.isTexture()) return false;
	if (!Material::isSimilar(other)) return false;
	ImageTexture * texture = (ImageTexture *)&other;
	if (__filename != texture->__filename) return false;
	if (__mipmaping != texture->__mipmaping) return false;
	return true;
}
*/

/* ----------------------------------------------------------------------- */

const Texture2DTransformationPtr Texture2D::DEFAULT_TRANSFORMATION(0);

/* ----------------------------------------------------------------------- */

Texture2D::Builder::Builder() :
  Appearance::Builder(),
  Image(0),
  Transformation(0){
}


Texture2D::Builder::~Builder() {
}


SceneObjectPtr Texture2D::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Texture2D
       (*Image,
         Transformation ? *Transformation : DEFAULT_TRANSFORMATION));
  return SceneObjectPtr();
}


void Texture2D::Builder::destroy() {
  if (Image) delete Image;
  if (Transformation) delete Transformation;
}



bool Texture2D::Builder::isValid( ) const {
  /// Image
	if (Image && (*Image)){
    if (!(*Image)->isValid()) {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Texture2D","Image","Must be valid.");
      return false;
	}
  }
  else {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Texture2D","Image","Must be defined.");
      return false;
  }

  /// Transformation
  if (Transformation && (*Transformation))
    if (!(*Transformation)->isValid()) {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Texture2D","Transformation","Must be valid.");
      return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */

Texture2D::Texture2D():
	Appearance(),
	__Image(), 
	__Transformation(DEFAULT_TRANSFORMATION) {
}

Texture2D::Texture2D(const ImageTexturePtr& image, 
					 const Texture2DTransformationPtr& transformation):
	Appearance(),
	__Image(image), 
	__Transformation(transformation) {
}


Texture2D::Texture2D(const std::string& name,
					 const ImageTexturePtr& image, 
					 const Texture2DTransformationPtr& transformation):
	Appearance(name),
	__Image(image), 
	__Transformation(transformation) {
}


Texture2D::~Texture2D( ) {
}


bool Texture2D::isValid( ) const {
  Builder _builder;
  if(__Image)_builder.Image = const_cast<ImageTexturePtr *>(&__Image);
  if(__Transformation)_builder.Transformation = const_cast<Texture2DTransformationPtr *>(&__Transformation);
  return _builder.isValid();
}

SceneObjectPtr Texture2D::copy(DeepCopier& copier) const
{
  Texture2D * ptr = new Texture2D(*this);
  copier.copy_object_attribute(ptr->getImage());
  copier.copy_object_attribute(ptr->getTransformation());
  return SceneObjectPtr(ptr);
}
/* ----------------------------------------------------------------------- */
