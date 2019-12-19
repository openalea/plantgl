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
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"MonoSpectral","Reflectance","Must be in [0,1].");
      return false;
  };

  /// Transmittance
  if (Transmittance)
    if (*Transmittance < 0 || *Transmittance > 1) {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"MonoSpectral","Transmittance","Must be in [0,1].");
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
