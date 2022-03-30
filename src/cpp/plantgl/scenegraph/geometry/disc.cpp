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





#include "disc.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_vector.h>
PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const real_t Disc::DEFAULT_RADIUS(.5);


/* ----------------------------------------------------------------------- */


Disc::Builder::Builder() :
  SOR2D::Builder(),
  Radius(0) {
}


Disc::Builder::~Builder() {
  // nothing to do
}


SceneObjectPtr Disc::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Disc(Radius ? *Radius : DEFAULT_RADIUS,
        Slices ? *Slices : SOR::DEFAULT_SLICES));
  return SceneObjectPtr();
}


void Disc::Builder::destroy() {
  SOR2DDestroy();
  if (Radius) delete Radius;
}


bool Disc::Builder::isValid( ) const {
  if (! SOR2DValid()) return false;

  // Radius field.
  if (Radius && (*Radius < REAL_EPSILON)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Disc","Radius","Must be not null.");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */


Disc::Disc( const real_t& radius,
            uchar_t slices ) :
  SOR2D(slices),
  __radius(radius) {
  GEOM_ASSERT(isValid());
}

 Disc::~Disc( ) {
  }

const real_t&
Disc::getRadius( ) const {
  return __radius;
}

real_t&
Disc::getRadius( ) {
  return __radius;
}

bool
Disc::isAVolume( ) const {
  return false;
}

bool
Disc::isRadiusToDefault( ) const {
  return fabs(__radius - DEFAULT_RADIUS) < GEOM_TOLERANCE;
}

bool Disc::isValid( ) const {
  Builder _builder;
  _builder.Radius = const_cast<real_t *>(&__radius);
  _builder.Slices = const_cast<uchar_t *>(&__slices);
  return _builder.isValid();
}

SceneObjectPtr Disc::copy(DeepCopier& copier) const {
  return SceneObjectPtr(new Disc(*this));
}


/* ----------------------------------------------------------------------- */

