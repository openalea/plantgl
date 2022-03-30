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




#include "texture.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_matrix.h>
#include <plantgl/scenegraph/container/pointarray.h>


#include <iostream>
#include <fstream>

PGL_USING_NAMESPACE
TOOLS_USING(Vector2)
TOOLS_USING(Vector3)
TOOLS_USING(Matrix2)
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
    if (!pglfinite(*RotationAngle) ){
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),
                 "Texture2DTransformation","RotationAngle","Must be valid.");
      return false;
    }
  }
  return true;

}

/* ----------------------------------------------------------------------- */

Texture2DTransformation::Texture2DTransformation(const Vector2& scaling,
                                             const Vector2& translation,
                                             const Vector2& rotationCenter,
                                             real_t rotationAngle):
    SceneObject(),
    __Scale(scaling),
    __Translation(translation),
    __RotationCenter(rotationCenter),
    __RotationAngle(rotationAngle)  {
}

Texture2DTransformation::Texture2DTransformation(const std::string& name,
                                             const Vector2& scaling,
                                             const Vector2& translation,
                                             const Vector2& rotationCenter,
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

Point3ArrayPtr Texture2DTransformation::transform( const Point3ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->size()));
  Point3Array::iterator _ti = _tPoints->begin();
  Matrix2 rotationOp = Matrix2::rotation(__RotationAngle);
  for (Point3Array::const_iterator _i = points->begin();
       _i != points->end();
       _i++){
    Vector2 res(_i->x() * __Scale.x(), _i->y() * __Scale.y()) ;
    res -= __RotationCenter;
    res = rotationOp * res;
    res += __RotationCenter;
    res += __Translation;
    *_ti++ = Vector3(res.x() , res.y() , _i->z());
  }
  return _tPoints;
}


Point2ArrayPtr Texture2DTransformation::transform( const Point2ArrayPtr& points ) const {
  GEOM_ASSERT(points);
  Point2ArrayPtr _tPoints(new Point2Array(points->size()));
  Point2Array::iterator _ti = _tPoints->begin();
  Matrix2 rotationOp = Matrix2::rotation(__RotationAngle);
  for (Point2Array::const_iterator _i = points->begin();
       _i != points->end();
         _i++){
    Vector2 res(_i->x() * __Scale.x(), _i->y() * __Scale.y()) ;
    res -= __RotationCenter;
    res = rotationOp * res;
    res += __RotationCenter;
    res += __Translation;
    *_ti++ = res;
  }
  return _tPoints;
}

Vector2 Texture2DTransformation::transform( const Vector2& point ) const
{
    Vector2 res(point.x() * __Scale.x(), point.y() * __Scale.y()) ;
    res -= __RotationCenter;
    res =  Matrix2::rotation(__RotationAngle) * res;
    res += __RotationCenter;
    res += __Translation;
    return res;
}

/* ----------------------------------------------------------------------- */

const bool ImageTexture::DEFAULT_MIPMAPING(true);
const bool ImageTexture::DEFAULT_REPEATS(true);
const bool ImageTexture::DEFAULT_REPEATT(true);

/* ----------------------------------------------------------------------- */

ImageTexture::Builder::Builder() :
  SceneObject::Builder(),
  FileName(0),
  RepeatT(0),
  RepeatS(0),
  Mipmaping(0){
}


ImageTexture::Builder::~Builder() {
}


SceneObjectPtr ImageTexture::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new ImageTexture
       (*FileName,
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
}



bool ImageTexture::Builder::isValid( ) const {

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
    __RepeatT(DEFAULT_REPEATT)  {
}

ImageTexture::ImageTexture(   const std::string& filename,
                              bool repeatS,
                              bool repeatT,
                              bool mipmaping) :
    SceneObject(),
    __Filename(filename),
    __Mipmaping(mipmaping),
    __RepeatS(repeatS),
    __RepeatT(repeatT) {
  GEOM_ASSERT(isValid());
}

  ImageTexture::ImageTexture( const std::string& name,
                              const std::string& filename,
                              bool repeatS,
                              bool repeatT,
                              bool mipmaping ) :
    SceneObject(name),
    __Filename(filename),
    __Mipmaping(mipmaping),
    __RepeatS(repeatS),
    __RepeatT(repeatT) {
  GEOM_ASSERT(isValid());
}

ImageTexture::~ImageTexture( ) {
}


bool ImageTexture::isValid( ) const {
  Builder _builder;
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
const Color4 Texture2D::DEFAULT_BASECOLOR(255,255,255,0);


/* ----------------------------------------------------------------------- */

Texture2D::Builder::Builder() :
  Appearance::Builder(),
  Image(0),
  Transformation(0),
  BaseColor(0){
}


Texture2D::Builder::~Builder() {
}


SceneObjectPtr Texture2D::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Texture2D
       (*Image,
         Transformation ? *Transformation : DEFAULT_TRANSFORMATION,
         BaseColor ? *BaseColor : DEFAULT_BASECOLOR));
  return SceneObjectPtr();
}


void Texture2D::Builder::destroy() {
  if (Image) delete Image;
  if (Transformation) delete Transformation;
  if (BaseColor) delete BaseColor;
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
    __Transformation(DEFAULT_TRANSFORMATION),
    __BaseColor(DEFAULT_BASECOLOR) {
}

Texture2D::Texture2D(const ImageTexturePtr& image,
                     const Texture2DTransformationPtr& transformation,
                     const Color4& basecolor):
    Appearance(),
    __Image(image),
    __Transformation(transformation),
    __BaseColor(basecolor) {
}

Texture2D::Texture2D(const ImageTexturePtr& image,
                     const Color4& basecolor):
    Appearance(),
    __Image(image),
    __Transformation(DEFAULT_TRANSFORMATION),
    __BaseColor(basecolor) {
}


Texture2D::Texture2D(const std::string& name,
                     const ImageTexturePtr& image,
                     const Texture2DTransformationPtr& transformation,
                     const Color4& basecolor):
    Appearance(name),
    __Image(image),
    __Transformation(transformation),
    __BaseColor(basecolor) {
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
