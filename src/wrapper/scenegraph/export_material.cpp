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
#include <plantgl/scenegraph/appearance/appearance.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/appearance/texture.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Material)
DEF_POINTEE(ImageTexture)


#define COL3PRINT(c) "Color3(" << (int)c.getRed() << ',' << (int)c.getGreen() << ',' << (int)c.getBlue() << ')'

std::string mat_str(Material * m){
  std::stringstream ss;
  ss << "Material(";
  if(m->isNamed()) ss << "name='" << m->getName() << "',";
  if(!m->isAmbientToDefault())  ss << "ambient=" << COL3PRINT(m->getAmbient()) << ',';
  if(!m->isDiffuseToDefault())  ss << "diffuse=" << m->getDiffuse() << ',';
  if(!m->isSpecularToDefault()) ss << "specular=" << COL3PRINT(m->getSpecular()) << ',';
  if(!m->isEmissionToDefault()) ss << "emission=" << COL3PRINT(m->getEmission()) << ',';
  if(!m->isShininessToDefault()) ss << "shininess=" << m->getShininess() << ',';
  if(!m->isTransparencyToDefault()) ss << "transparency=" << m->getTransparency() ;
  ss << ')' ;
  return ss.str();
}

struct mat_pickle_suite : boost::python::pickle_suite
{
  static boost::python::tuple getinitargs(Material const& m)
    {
      return boost::python::make_tuple(m.getName(),m.getAmbient(),m.getDiffuse(),
                        m.getSpecular(),m.getEmission(),m.getShininess(),m.getTransparency());
    }
};

MaterialPtr getDefaultMaterial() { return dynamic_pointer_cast<Material>(Material::DEFAULT_MATERIAL); }

MaterialPtr py_interpolate(Material * m1, Material * m2, real_t t ){
    return interpolate(MaterialPtr(m1),MaterialPtr(m2),t);
}


void export_Material()
{
  class_< Material, MaterialPtr, bases < Appearance >, boost::noncopyable > 
        ("Material", 
         "The material of an object. It determine the way light reflect off an object to create color.", 
         init< const Material & >())
    .def(init< optional<const Color3&, const real_t&, const Color3&,const Color3&,const real_t&,const real_t&> >
         ("Material( ambient=(160,160,160),diffuse=1,specular=(40,40,40),emission=(0,0,0),shininess=1,transparency=0)",
          (bp::arg("ambient")=Material::DEFAULT_AMBIENT,
           bp::arg("diffuse")=Material::DEFAULT_DIFFUSE,
           bp::arg("specular")=Material::DEFAULT_SPECULAR,
           bp::arg("emission")=Material::DEFAULT_EMISSION,
           bp::arg("shininess")=Material::DEFAULT_SHININESS,
           bp::arg("transparency")=Material::DEFAULT_TRANSPARENCY)
         ))
    .def(init< const string&, 
         optional< const Color3&, const real_t&, 
         const Color3&, const Color3&, const real_t&, const real_t&> >
         ("Material( name, ambient=(160,160,160), diffuse=1, specular=(40,40,40), emission=(0,0,0), shininess=1, transparency=0)",
          (bp::arg("name"),
           bp::arg("ambient")=Material::DEFAULT_AMBIENT,
           bp::arg("diffuse")=Material::DEFAULT_DIFFUSE,
           bp::arg("specular")=Material::DEFAULT_SPECULAR,
           bp::arg("emission")=Material::DEFAULT_EMISSION,
           bp::arg("shininess")=Material::DEFAULT_SHININESS,
           bp::arg("transparency")=Material::DEFAULT_TRANSPARENCY)
         ))
    .DEF_PGLBASE(Material)
    .def( "__str__", mat_str )
    .def( "__repr__", mat_str )
    .def( "isSimilar", &Material::isSimilar)
    .def( "interpolate", &py_interpolate, (bp::arg("m"),bp::arg("alpha")=0.5))
    .DEC_CT_PROPERTY_WDV(ambient,Material,Ambient,Color3,DEFAULT_AMBIENT)
    .DEC_BT_PROPERTY_WDV(diffuse,Material,Diffuse,real_t,DEFAULT_DIFFUSE)
    .DEC_CT_PROPERTY_WDV(specular,Material,Specular,Color3,DEFAULT_SPECULAR)
    .DEC_CT_PROPERTY_WDV(emission,Material,Emission,Color3,DEFAULT_EMISSION)
    .DEC_BT_PROPERTY_WDV(shininess,Material,Shininess,real_t,DEFAULT_SHININESS)
    .DEC_BT_PROPERTY_WDV(transparency,Material,Transparency,real_t,DEFAULT_TRANSPARENCY)
    .def( "diffuseColor", &Material::getDiffuseColor)
    .add_static_property("DEFAULT_MATERIAL",&getDefaultMaterial)
    .def_pickle(mat_pickle_suite());
  ;

  implicitly_convertible<MaterialPtr, AppearancePtr >();
}

