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
#include <plantgl/scenegraph/appearance/appearance.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/appearance/texture.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
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
    .def(init< boost::python::optional<const Color3&, const real_t&, const Color3&,const Color3&,const real_t&,const real_t&> >
         ("Material( ambient=(160,160,160),diffuse=1,specular=(40,40,40),emission=(0,0,0),shininess=1,transparency=0)",
          (bp::arg("ambient")=Material::DEFAULT_AMBIENT,
           bp::arg("diffuse")=Material::DEFAULT_DIFFUSE,
           bp::arg("specular")=Material::DEFAULT_SPECULAR,
           bp::arg("emission")=Material::DEFAULT_EMISSION,
           bp::arg("shininess")=Material::DEFAULT_SHININESS,
           bp::arg("transparency")=Material::DEFAULT_TRANSPARENCY)
         ))
    .def(init< const string&,
         boost::python::optional< const Color3&, const real_t&,
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

