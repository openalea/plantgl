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






#include "revolution.h"
#include "curve.h"

#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


Revolution::Builder::Builder() :
  SOR::Builder(),
  Profile(0) {
}


Revolution::Builder::~Builder() {
  // nothing to do
}


SceneObjectPtr Revolution::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Revolution(*Profile,
              Slices ? *Slices : DEFAULT_SLICES));
  return SceneObjectPtr();
}


void Revolution::Builder::destroy() {
  SORDestroy();
  if (Profile) delete Profile;
}


bool Revolution::Builder::isValid( ) const {
  if (! SORValid()) return false;

  /*
    Profile field
  */
  // Initialization check
  if (! Profile) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Revolution","Profile");
    return false;
  };
  // Size check
  uint_t _pointListSize = (*Profile)->getStride();
  if (_pointListSize < 3) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Revolution","Profile","Must have more than 3 points.");
    return false;
  };
  return true;
}


/* ----------------------------------------------------------------------- */


Revolution::Revolution() :
  SOR(),
  __profile() {
}

Revolution::Revolution( const Curve2DPtr& profile,
            uchar_t slices ) :
  SOR(slices),
  __profile(profile) {
  GEOM_ASSERT(isValid());
}

Revolution::~Revolution( ) {
}


bool Revolution::isValid( ) const {
  Builder _builder;
  _builder.Profile = const_cast<Curve2DPtr *>(&__profile);
  _builder.Slices = const_cast<uchar_t *>(&__slices);
  return _builder.isValid();
}

SceneObjectPtr Revolution::copy(DeepCopier& copier) const {
  Revolution * ptr = new Revolution(*this);
  copier.copy_object_attribute(ptr->getProfile());
  return SceneObjectPtr(ptr);
}

/* ----------------------------------------------------------------------- */

const Curve2DPtr&
Revolution::getProfile( ) const {
  return __profile;
}

Curve2DPtr&
Revolution::getProfile( ) {
  return __profile;
}

bool
Revolution::isAVolume( ) const {
  if(!__profile)return false;
  return ((fabs(__profile->getPointAt(__profile->getFirstKnot()).x()) < REAL_EPSILON) &&
    (fabs(__profile->getPointAt(__profile->getLastKnot()).x()) < REAL_EPSILON));
}
