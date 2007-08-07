/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): C. Pradal (christophe.pradal@cirad.fr)
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

#include <boost/python.hpp>

#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/appearance/appearance.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/appearance/texture.h>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Material)
DEF_POINTEE(ImageTexture)


#define COL3PRINT(c) "Color3(" << (int)c.getRed() << ',' << (int)c.getGreen() << ',' << (int)c.getBlue() << ')'

std::string mat_str(Material * m){
  std::stringstream ss;
  ss << "Material(";
  if(m->isNamed()) ss << "'" << m->getName() << "',";
  ss << COL3PRINT(m->getAmbient()) << ','
     << m->getDiffuse() << ','
	 << COL3PRINT(m->getSpecular()) << ','
	 << COL3PRINT(m->getEmission()) << ','
     << m->getShininess() << ','
     << m->getTransparency() << ')'
	  ;
  return ss.str();
}

struct mat_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Material const& m)
	{
		return make_tuple(m.getName(),m.getAmbient(),m.getDiffuse(),
						  m.getSpecular(),m.getEmission(),m.getShininess(),m.getTransparency());
	}
};


void export_Material()
{
  class_< Material, MaterialPtr, bases < Appearance >, boost::noncopyable > ("Material", init< const Material & >())
    .def(init< optional<const Color3&, const real_t&, const Color3&,const Color3&,const real_t&,const real_t&> >
	 (args("ambient","diffuse","specular","emission","shininess","transparency"),
	  "Material( ambient=Color3(160,160,160),"
	  "diffuse=1,"
	  "specular=Color3(40,40,40),"
	  "emission=Color3(0,0,0),"
	  "shininess=1,"
	  "transparency=0)"))
    .def(init< const string&, 
	 optional< const Color3&, const real_t&, 
	           const Color3&, const Color3&, const real_t&, const real_t&> >
	 (args("name","ambient","diffuse","specular","emission","shininess","transparency"),
	  "Material( name,"
	  "ambient=Color3(160,160,160),"
	  "diffuse=1,"
	  "specular=Color3(40,40,40),"
	  "emission=Color3(0,0,0),"
	  "shininess=1,"
	  "transparency=0)"))

  .def( "__str__", mat_str )
  .def( "__repr__", mat_str )
  .DEC_CT_PROPERTY_WD(ambient,Material,Ambient,Color3)
  .DEC_BT_PROPERTY_WD(diffuse,Material,Diffuse,real_t)
  .DEC_CT_PROPERTY_WD(specular,Material,Specular,Color3)
  .DEC_CT_PROPERTY_WD(emission,Material,Emission,Color3)
  .DEC_BT_PROPERTY_WD(shininess,Material,Shininess,real_t)
  .DEC_BT_PROPERTY_WD(transparency,Material,Transparency,real_t)
  .def_pickle(mat_pickle_suite());
    ;

   implicitly_convertible<MaterialPtr, AppearancePtr >();
}

void export_ImageTexture()
{
  class_< ImageTexture, ImageTexturePtr, bases<  Material >, boost::noncopyable >
    ( "ImageTexture", init< const ImageTexture & >())
	.def(init< string, optional< const Color3 &, real_t, const Color3 &, const Color3 &, real_t, real_t> >
  	 (args("filename","ambient","diffuse","specular","emission","shininess","transparency"),
	  "ImageTexture(filename [,ambient, diffuse, specular, emission, shininess, transparency])"))
      .DEC_BT_PROPERTY(filename,ImageTexture,Filename,std::string )
    ;

  implicitly_convertible<ImageTexturePtr, MaterialPtr >();
}
