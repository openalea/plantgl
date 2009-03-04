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




#include "monospectral.h"
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const real_t MonoSpectral::DEFAULT_REFLECTANCE(.8);
const real_t MonoSpectral::DEFAULT_TRANSMITTANCE(0);


/* ----------------------------------------------------------------------- */


MonoSpectral::Builder::Builder( ) :
  Spectrum::Builder(),
  Reflectance(0),
  Transmittance(0) {
}


MonoSpectral::Builder::~Builder( ) {
}


SceneObjectPtr MonoSpectral::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new MonoSpectral
       (Reflectance ? *Reflectance : DEFAULT_REFLECTANCE,
	Transmittance ? *Transmittance : DEFAULT_TRANSMITTANCE));
  return SceneObjectPtr();
}


void MonoSpectral::Builder::destroy( ) {
  if (Reflectance) delete Reflectance;
  if (Transmittance) delete Transmittance;
}


bool MonoSpectral::Builder::isValid( ) const {

  // The color is always valid !!!

  /// Reflectance
  if (Reflectance)
    if (*Reflectance < 0 || *Reflectance > 1) {
      pglErrorEx(WARNINGMSG(INVALID_FIELD_VALUE_sss),"MonoSpectral","Reflectance","Must be in [0,1].");
      return false;
  };

  /// Transmittance
  if (Transmittance)
    if (*Transmittance < 0 || *Transmittance > 1) {
      pglErrorEx(WARNINGMSG(INVALID_FIELD_VALUE_sss),"MonoSpectral","Transmittance","Must be in [0,1].");
      return false;
  };

  return true;

}


/* ----------------------------------------------------------------------- */


MonoSpectral::MonoSpectral( const real_t& reflectance,
			    const real_t& transmittance ) :
  Spectrum(),
  __reflectance(reflectance),
  __transmittance(transmittance) {
  GEOM_ASSERT(isValid());
}

MonoSpectral::~MonoSpectral( ) {
}


bool MonoSpectral::isValid( ) const {
  Builder _builder;
  _builder.Reflectance = const_cast<real_t *>(&__reflectance);
  _builder.Transmittance = const_cast<real_t *>(&__transmittance);
  return _builder.isValid();
}

SceneObjectPtr MonoSpectral::copy(DeepCopier& copier) const 
{
  return SceneObjectPtr(new MonoSpectral(*this));
}

/* ----------------------------------------------------------------------- */

const real_t& MonoSpectral::getReflectance( ) const {
  return __reflectance;
}

real_t& MonoSpectral::getReflectance( ) {
  return __reflectance;
}

const real_t& MonoSpectral::getTransmittance( ) const {
  return __transmittance;
}

real_t& MonoSpectral::getTransmittance( ){
  return __transmittance;
}

bool MonoSpectral::isReflectanceToDefault( ) const {
  return __reflectance == DEFAULT_REFLECTANCE;
}

bool MonoSpectral::isTransmittanceToDefault( ) const {
  return __transmittance == DEFAULT_TRANSMITTANCE;
}

/* ----------------------------------------------------------------------- */
