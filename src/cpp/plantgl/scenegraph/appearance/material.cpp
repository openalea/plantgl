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




#include "material.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <cmath>
#include <algorithm>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


const Color3 Material::DEFAULT_AMBIENT(80,80,80);
const real_t  Material::DEFAULT_DIFFUSE(2);
const Color3 Material::DEFAULT_SPECULAR(40,40,40);
const Color3 Material::DEFAULT_EMISSION(0,0,0);
const real_t Material::DEFAULT_SHININESS(1);
const real_t Material::DEFAULT_TRANSPARENCY(0);
const AppearancePtr Material::DEFAULT_MATERIAL(new Material(string("DEFAULT_MATERIAL")));

/* ----------------------------------------------------------------------- */


Material::Builder::Builder() :
  Ambient(0),
  Diffuse(0),
  Specular(0),
  Emission(0),
  Shininess(0),
  Transparency(0) {
}


Material::Builder::~Builder() {
}


SceneObjectPtr Material::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Material
       (Ambient ? *Ambient : DEFAULT_AMBIENT,
        Diffuse ? *Diffuse : DEFAULT_DIFFUSE,
        Specular ? *Specular : DEFAULT_SPECULAR,
        Emission ? *Emission : DEFAULT_EMISSION,
        Shininess ? *Shininess : DEFAULT_SHININESS,
        Transparency ? *Transparency : DEFAULT_TRANSPARENCY));
  return SceneObjectPtr();
}


void Material::Builder::destroy() {
  if (Ambient) delete Ambient;
  if (Diffuse) delete Diffuse;
  if (Specular) delete Specular;
  if (Emission) delete Emission;
  if (Shininess) delete Shininess;
  if (Transparency) delete Transparency;
}



bool Material::Builder::isValid( ) const {
  return MaterialValidity();
}

  /// Ambient is always valid
bool Material::Builder::MaterialValidity( ) const {

  /// Diffuse
  if (Diffuse && Ambient)
    if (((floor( (*Diffuse) * Ambient->getRed()) )> 255) || ((((*Diffuse) * Ambient->getRed()))<0) ||
        ((floor( (*Diffuse) * Ambient->getGreen()) )> 255) || ((((*Diffuse) * Ambient->getGreen()))<0) ||
        ((floor( (*Diffuse) * Ambient->getBlue()) )> 255) || ((((*Diffuse) * Ambient->getBlue()))<0)) {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Material","Diffuse","Don't give a valid Diffuse Color.");
      return false;
  };

  /// Specular is always valid

  /// Emission is always valid

  /// Shininess
  if (Shininess)
    if (*Shininess < 0 || *Shininess > 1) {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Material","Shininess","Must be in [0,1].");
      return false;
  };

  /// Transparency
  if (Transparency)
    if (*Transparency < 0 || *Transparency > 1) {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Material","Transparency","Must be in [0,1].");
      return false;
  };

  return true;

}


/* ----------------------------------------------------------------------- */


Material::Material( const Color3& ambient,
                    const real_t& diffuse,
                    const Color3& specular,
                    const Color3& emission,
                    const real_t& shininess,
                    const real_t& transparency ) :
  Appearance(),
  __ambient(ambient),
  __diffuse(diffuse),
  __specular(specular),
  __emission(emission),
  __shininess(shininess),
  __transparency(transparency) {
    if(__diffuse*real_t(ambient.getRed())>255)
        __diffuse = min(__diffuse,real_t(255.)/real_t(ambient.getRed()));
    if(__diffuse*real_t(ambient.getGreen())>255)
        __diffuse = min(__diffuse, real_t(255.)/real_t(ambient.getGreen()));
    if(__diffuse*real_t(ambient.getBlue())>255)
        __diffuse = min(__diffuse,real_t(255.)/real_t(ambient.getBlue()));
  GEOM_ASSERT(isValid());
}

Material::Material( const string& name,
                    const Color3& ambient,
                    const real_t& diffuse,
                    const Color3& specular,
                    const Color3& emission,
                    const real_t& shininess,
                    const real_t& transparency ) :
  Appearance(),
  __ambient(ambient),
  __diffuse(diffuse),
  __specular(specular),
  __emission(emission),
  __shininess(shininess),
  __transparency(transparency) {
    if(__diffuse*real_t(ambient.getRed())>255) __diffuse = min(__diffuse, real_t(255.)/real_t(ambient.getRed()));
    if(__diffuse*real_t(ambient.getGreen())>255) __diffuse = min(__diffuse, real_t(255.)/real_t(ambient.getGreen()));
    if(__diffuse*real_t(ambient.getBlue())>255) __diffuse = min(__diffuse,real_t(255.)/real_t(ambient.getBlue()));
  setName(name);
  GEOM_ASSERT(isValid());
}

Material::~Material( ) {
}


bool Material::isValid( ) const {
  Builder _builder;
  _builder.Ambient = const_cast<Color3 *>(&__ambient);
  _builder.Diffuse = const_cast<real_t *>(&__diffuse);
  _builder.Shininess = const_cast<real_t *>(&__shininess);
  _builder.Transparency = const_cast<real_t *>(&__transparency);
  return _builder.isValid();
}

SceneObjectPtr Material::copy(DeepCopier& copier) const
{
  return SceneObjectPtr(new Material(*this));
}
/* ----------------------------------------------------------------------- */

const Color3& Material::getAmbient( ) const {
  return __ambient;
}

/// Returns \b Ambient field.
Color3& Material::getAmbient( ) {
  return __ambient;
}

/// Returns \b Diffuse value.
const real_t& Material::getDiffuse( ) const {
  return __diffuse;
}

/// Returns \b Diffuse field.
real_t& Material::getDiffuse( ) {
  return __diffuse;
}

Color3 Material::getDiffuseColor( ) const {
  return Color3(uchar_t( floor( __ambient.getRed()*__diffuse ) ),
                uchar_t( floor( __ambient.getGreen()*__diffuse ) ),
                uchar_t( floor( __ambient.getBlue()*__diffuse) ) );
}

/// Returns \b Specular value.
const Color3& Material::getSpecular( ) const {
  return __specular;
}

/// Returns \b Specular field.
Color3& Material::getSpecular( ){
  return __specular;
}

/// Returns \b Emission value.
const Color3& Material::getEmission( ) const {
  return __emission;
}

/// Returns \b Emission field.
Color3& Material::getEmission( ) {
  return __emission;
}

/// Returns \b Shininess value.
const real_t& Material::getShininess( ) const {
  return __shininess;
}

/// Returns \b Shininess field.
real_t& Material::getShininess( ){
  return __shininess;
}

/// Returns \b Transparency value.
const real_t& Material::getTransparency( ) const {
  return __transparency;
}

/// Returns \b Transparency field.
real_t& Material::getTransparency( ) {
  return __transparency;
}

/// Returns whether \b Ambient field is set to its default value.
bool Material::isAmbientToDefault( ) const {
  return __ambient == DEFAULT_AMBIENT;
}

/// Returns whether \b Diffuse field is set to its default value.
bool Material::isDiffuseToDefault( ) const {
  return __diffuse == DEFAULT_DIFFUSE;
}

/// Returns whether \b Specular field is set to its default value.
bool Material::isSpecularToDefault( ) const {
  return __specular == DEFAULT_SPECULAR;
}

/// Returns whether \b Emission field is set to its default value.
bool Material::isEmissionToDefault( ) const {
  return __emission == DEFAULT_EMISSION;
}


bool Material::isShininessToDefault( ) const {
  return fabs(__shininess - DEFAULT_SHININESS) < GEOM_EPSILON;
}

bool Material::isTransparencyToDefault( ) const {
  return fabs(__transparency - DEFAULT_TRANSPARENCY) < GEOM_EPSILON;
}

bool Material::isSimilar(const Material& other) const
{
    return isTexture() == other.isTexture() &&
           __ambient == other.__ambient     &&
           __diffuse == other.__diffuse     &&
           __specular == other.__specular   &&
           __emission == other.__emission   &&
           __shininess == other.__shininess &&
           __transparency == other.__transparency;
}

MaterialPtr PGL(interpolate)(const MaterialPtr m1, const MaterialPtr m2, real_t t) {
    GEOM_ASSERT(is_valid_prt(m1) && is_valid_ptr(m2));
    if (t < 0) t = 0.;
    else if(t >= 1) t = 1.;
    real_t oneminust = 1- t;

    return MaterialPtr(new Material(
                        Color3::interpolate(m1->getAmbient(),m2->getAmbient(),t),
                        m1->getDiffuse()*oneminust+m2->getDiffuse()*t,
                        Color3::interpolate(m1->getSpecular(),m2->getSpecular(),t),
                        Color3::interpolate(m1->getEmission(),m2->getEmission(),t),
                        m1->getShininess()*oneminust+m2->getShininess()*t,
                        m1->getTransparency()*oneminust+m2->getTransparency()*t));
}
