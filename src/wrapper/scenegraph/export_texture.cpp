/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/appearance/texture.h>
#include <plantgl/scenegraph/container/pointarray.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Texture2D)
DEF_POINTEE(ImageTexture)
DEF_POINTEE(Texture2DTransformation)


#define COL3PRINT(c) "Color3(" << (int)c.getRed() << ',' << (int)c.getGreen() << ',' << (int)c.getBlue() << ')'

std::string imgtex_str(ImageTexture * m){
  std::stringstream ss;
  ss << "ImageTexture(";
  if(m->isNamed()) ss << "name='" << m->getName() << "',";
  ss << "filename='''" << m->getFilename() << "'''";
  if(!m->isMipmapingToDefault())  ss << ", mipmaping=" << (m->getMipmaping()?"True":"False");
  if(!m->isRepeatSToDefault())  ss << ", repeatS=" << (m->getRepeatS()?"True":"False");
  if(!m->isRepeatTToDefault())  ss << ", repeatT=" << (m->getRepeatT()?"True":"False");
  if(!m->isTransparencyToDefault()) ss << ", transparency=" << m->getTransparency() ;
  ss << ')' ;
     ;
  return ss.str();
}


void export_ImageTexture()
{
  class_< ImageTexture, ImageTexturePtr, bases<  SceneObject >, boost::noncopyable >
    ( "ImageTexture", "The image of a textured object.", 
	  init< string, optional< real_t, bool, bool, bool> >(
		(bp::arg("filename"),
		 bp::arg("transparency") = ImageTexture::DEFAULT_TRANSPARENCY,
		 bp::arg("repeatS") = ImageTexture::DEFAULT_REPEATS,
		 bp::arg("repeatT") = ImageTexture::DEFAULT_REPEATT,
		 bp::arg("mipmaping") = ImageTexture::DEFAULT_MIPMAPING),
          "ImageTexture(filename [, transparency, repeatS, repeatT, mipmaping])"))
    .def(init< string,string, optional< real_t, bool, bool, bool> >(
		(bp::arg("name"),
		 bp::arg("filename"),
		 bp::arg("transparency") = ImageTexture::DEFAULT_TRANSPARENCY,
		 bp::arg("repeatS") = ImageTexture::DEFAULT_REPEATS,
		 bp::arg("repeatT") = ImageTexture::DEFAULT_REPEATT,
		 bp::arg("mipmaping") = ImageTexture::DEFAULT_MIPMAPING),
          "ImageTexture(name, filename [, transparency, repeatS, repeatT, mipmaping])"))

    .DEC_BT_PROPERTY(filename,ImageTexture,Filename,std::string )
    .DEC_BT_PROPERTY_WDV(mipmaping,ImageTexture, Mipmaping,bool,DEFAULT_MIPMAPING)
    .DEC_BT_PROPERTY_WDV(repeatS,ImageTexture, RepeatS, bool, DEFAULT_REPEATS)
    .DEC_BT_PROPERTY_WDV(repeatT,ImageTexture, RepeatT, bool, DEFAULT_REPEATT)
    .DEC_BT_PROPERTY_WDV(transparency,ImageTexture, Transparency, real_t, DEFAULT_TRANSPARENCY)
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
	init< optional<std::string,Vector2,Vector2,Vector2,real_t> >
		((bp::arg("name"),
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
  ss << ')' ;
  return ss.str();
}


void export_Texture2D()
{
  class_< Texture2D, Texture2DPtr, bases<  Appearance >, boost::noncopyable >
    ( "Texture2D", "The material of a textured object.", 
	   init< ImageTexturePtr, optional<Texture2DTransformationPtr> >
         (args("image","transformation"),
          "Texture2D(image [,transformation])"))
       .def(init< string, ImageTexturePtr, optional<Texture2DTransformationPtr> >
         (args("name","image","transformation"),
          "Texture2D(name, image [,transformation])"))
    .DEC_PTR_PROPERTY(image,Texture2D,Image,ImageTexturePtr )
    .DEC_PTR_PROPERTY_WDV(transformation,Texture2D, Transformation, Texture2DTransformationPtr,DEFAULT_TRANSFORMATION)
    .def( "__str__", tex_str )
    .def( "__repr__", tex_str )
    ;

  implicitly_convertible<Texture2DPtr, AppearancePtr >();
}
