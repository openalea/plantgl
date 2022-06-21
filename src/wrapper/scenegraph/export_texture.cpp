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



#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/appearance/texture.h>
#include <plantgl/scenegraph/container/pointarray.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Texture2D)
DEF_POINTEE(ImageTexture)
DEF_POINTEE(Texture2DTransformation)


#define COL3PRINT(c) "Color3(" << (int)c.getRed() << ',' << (int)c.getGreen() << ',' << (int)c.getBlue() << ')'
#define COL4PRINT(c) "Color4(" << (int)c.getRed() << ',' << (int)c.getGreen() << ',' << (int)c.getBlue() << ',' << (int)c.getAlpha() << ')'

std::string imgtex_str(ImageTexture * m){
  std::stringstream ss;
  ss << "ImageTexture(";
  if(m->isNamed()) ss << "name='" << m->getName() << "',";
  ss << "filename='''" << m->getFilename() << "'''";
  if(!m->isMipmapingToDefault())  ss << ", mipmaping=" << (m->getMipmaping()?"True":"False");
  if(!m->isRepeatSToDefault())  ss << ", repeatS=" << (m->getRepeatS()?"True":"False");
  if(!m->isRepeatTToDefault())  ss << ", repeatT=" << (m->getRepeatT()?"True":"False");
  ss << ')' ;
     ;
  return ss.str();
}


void export_ImageTexture()
{
  class_< ImageTexture, ImageTexturePtr, bases<  SceneObject >, boost::noncopyable >
    ( "ImageTexture", "The image of a textured object.",
      init< string, boost::python::optional< bool, bool, bool> >(
        (bp::arg("filename"),
         bp::arg("repeatS") = ImageTexture::DEFAULT_REPEATS,
         bp::arg("repeatT") = ImageTexture::DEFAULT_REPEATT,
         bp::arg("mipmaping") = ImageTexture::DEFAULT_MIPMAPING),
          "ImageTexture(filename [, transparency, repeatS, repeatT, mipmaping])"))
    .def(init< string,string, boost::python::optional< bool, bool, bool> >(
        (bp::arg("name"),
         bp::arg("filename"),
         bp::arg("repeatS") = ImageTexture::DEFAULT_REPEATS,
         bp::arg("repeatT") = ImageTexture::DEFAULT_REPEATT,
         bp::arg("mipmaping") = ImageTexture::DEFAULT_MIPMAPING),
          "ImageTexture(name, filename [, transparency, repeatS, repeatT, mipmaping])"))

    .DEC_BT_PROPERTY(filename,ImageTexture,Filename,std::string )
    .DEC_BT_PROPERTY_WDV(mipmaping,ImageTexture, Mipmaping,bool,DEFAULT_MIPMAPING)
    .DEC_BT_PROPERTY_WDV(repeatS,ImageTexture, RepeatS, bool, DEFAULT_REPEATS)
    .DEC_BT_PROPERTY_WDV(repeatT,ImageTexture, RepeatT, bool, DEFAULT_REPEATT)
    .def( "__str__", imgtex_str )
    .def( "__repr__", imgtex_str )
    ;

  implicitly_convertible<ImageTexturePtr, SceneObjectPtr >();
}

#define VEC2STR(vec) "(" << vec.x() << "," << vec.y() << ")"

std::string textr_str(Texture2DTransformation * m){
  std::stringstream ss;
  ss << "Texture2DTransformation(";
  if(m->isNamed()) ss << "name='" << m->getName() << "',";
  if(!m->isScaleToDefault())  ss << " scale= " << VEC2STR(m->getScale()) << ", ";
  if(!m->isTranslationToDefault())  ss << " translation= " << VEC2STR(m->getTranslation()) << ", ";
  if(!m->isRotationCenterToDefault())  ss << " rotationCenter= " << VEC2STR(m->getRotationCenter()) << ", ";
  if(!m->isRotationAngleToDefault())  ss << " rotationAngle= " << m->getRotationAngle();
  ss << ')' ;
  return ss.str();
}


void export_Texture2DTransformation()
{
  class_< Texture2DTransformation, Texture2DTransformationPtr, bases<  SceneObject >, boost::noncopyable >
    ( "Texture2DTransformation", "The transformation of a texture 2D.",
    init< boost::python::optional<std::string,Vector2,Vector2,Vector2,real_t> >
        ((bp::arg("name")="",
          bp::arg("scale")=Texture2DTransformation::DEFAULT_SCALE,
          bp::arg("translation")=Texture2DTransformation::DEFAULT_TRANSLATION,
          bp::arg("rotationCenter")=Texture2DTransformation::DEFAULT_ROTATIONCENTER,
          bp::arg("rotationAngle")=Texture2DTransformation::DEFAULT_ROTATIONANGLE),
          "Texture2D([name,scale,translation,rotationCenter,rotationAngle])"))

    .DEC_BT_PROPERTY_WDV(scale,Texture2DTransformation,
                            Scale, Vector2,DEFAULT_SCALE)

    .DEC_BT_PROPERTY_WDV(translation,Texture2DTransformation,
                            Translation, Vector2,DEFAULT_TRANSLATION)
    .DEC_BT_PROPERTY_WDV(rotationCenter,Texture2DTransformation,
                            RotationCenter, Vector2,DEFAULT_ROTATIONCENTER)
    .DEC_BT_PROPERTY_WDV(rotationAngle,Texture2DTransformation,
                            RotationAngle, real_t,DEFAULT_ROTATIONANGLE)
    .def( "__str__", textr_str )
    .def( "__repr__", textr_str )
    .def( "transform", (Point3ArrayPtr (Texture2DTransformation::*)( const Point3ArrayPtr& points ) const)&Texture2DTransformation::transform,bp::args("points3"))
    .def( "transform", (Point2ArrayPtr (Texture2DTransformation::*)( const Point2ArrayPtr& points ) const)&Texture2DTransformation::transform,bp::args("points2"))
    .def( "transform", (Vector2 (Texture2DTransformation::*)( const Vector2& point ) const)&Texture2DTransformation::transform,bp::args("point"))
    ;

  implicitly_convertible<Texture2DTransformationPtr, SceneObjectPtr >();
}

std::string tex_str(Texture2D * m){
  std::stringstream ss;
  ss << "Texture2D(";
  if(m->isNamed()) ss << "name='" << m->getName() << "',";
  if(m->getImage())  ss << "image=" << imgtex_str(m->getImage()) << ",";
  if(!m->isTransformationToDefault())  ss << ", transformation=" << textr_str(m->getTransformation());
  if(!m->isBaseColorToDefault())  ss << ", baseColor=" << COL4PRINT(m->getBaseColor());
  ss << ')' ;
  return ss.str();
}


void export_Texture2D()
{
  class_< Texture2D, Texture2DPtr, bases<  Appearance >, boost::noncopyable >
    ( "Texture2D", "The material of a textured object.",
       init< ImageTexturePtr, boost::python::optional<Texture2DTransformationPtr, Color4> >
         ((bp::arg("image"),
           bp::arg("transformation"),
           bp::arg("baseColor")=Texture2D::DEFAULT_BASECOLOR),
          "Texture2D(image [,transformation, baseColor])"))

       .def(init< string, ImageTexturePtr, boost::python::optional<Texture2DTransformationPtr, Color4> >
         ((bp::arg("name"),
           bp::arg("image"),
           bp::arg("transformation"),
           bp::arg("baseColor")=Texture2D::DEFAULT_BASECOLOR),
          "Texture2D(name, image [,transformation, baseColor])"))
       .def(init< ImageTexturePtr, Color4 >
         ((bp::arg("image"),
           bp::arg("baseColor")=Texture2D::DEFAULT_BASECOLOR),
          "Texture2D(name, image, baseColor])"))
    .DEC_PTR_PROPERTY(image,Texture2D,Image,ImageTexturePtr )
    .DEC_PTR_PROPERTY_WDV(transformation,Texture2D, Transformation, Texture2DTransformationPtr,DEFAULT_TRANSFORMATION)
    .DEC_BT_PROPERTY_WDV(baseColor,Texture2D, BaseColor, Color4, DEFAULT_BASECOLOR)
    .def( "__str__", tex_str )
    .def( "__repr__", tex_str )
    ;

  implicitly_convertible<Texture2DPtr, AppearancePtr >();
}
