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





#include "sphere.h"
#include "disc.h"

#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const uchar_t Sphere::DEFAULT_STACKS(8);
const real_t Sphere::DEFAULT_RADIUS(Disc::DEFAULT_RADIUS);


/* ----------------------------------------------------------------------- */


Sphere::Builder::Builder() :
  SOR::Builder(),
  Stacks(0),
  Radius(0) {
}


Sphere::Builder::~Builder() {
  // nothing to do
}


SceneObjectPtr Sphere::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Sphere(Radius ? *Radius : DEFAULT_RADIUS,
          Slices ? *Slices : DEFAULT_SLICES,
          Stacks ? *Stacks : DEFAULT_STACKS));
  return SceneObjectPtr();
}


void Sphere::Builder::destroy() {
  SORDestroy();
  if (Stacks) delete Stacks;
  if (Radius) delete Radius;
}


bool Sphere::Builder::isValid( ) const {
  if (! SORValid()) return false;

  // Stacks field.
  if (Stacks && (*Stacks < 4)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Sphere","Stacks","Must be greater than 3.");
    return false;
  };

  // Radius field.
  if (Radius && (*Radius < REAL_EPSILON)) {
    pglErrorEx(PGLERRORMSG(INVALID_FIELD_VALUE_sss),"Sphere","Radius","Must be positive non null.");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */


Sphere::Sphere( const real_t& radius,
        uchar_t slices,
        uchar_t stacks ) :
  SOR(slices),
    __stacks(stacks),
    __radius(radius) {
  GEOM_ASSERT(isValid());
}

Sphere::~Sphere( ) {
}

bool Sphere::isValid( ) const {
  Builder _builder;
  _builder.Radius = const_cast<real_t *>(&__radius);
  _builder.Slices = const_cast<uchar_t *>(&__slices);
  _builder.Stacks = const_cast<uchar_t *>(&__stacks);
  return _builder.isValid();
}

SceneObjectPtr Sphere::copy(DeepCopier& copier) const {
  return SceneObjectPtr(new Sphere(*this));
}



/* ----------------------------------------------------------------------- */

const uchar_t& Sphere::getStacks( ) const {
  return __stacks;
}

uchar_t& Sphere::getStacks( ) {
  return __stacks;
}

const real_t& Sphere::getRadius( ) const {
  return __radius;
}

real_t& Sphere::getRadius( ) {
  return __radius;
}

bool Sphere::isASurface( ) const {
  return true;
}

bool Sphere::isAVolume( ) const {
  return true;
}

bool Sphere::isStacksToDefault( ) const {
  return __stacks == DEFAULT_STACKS;
}

bool Sphere::isRadiusToDefault( ) const {
  return fabs(__radius - DEFAULT_RADIUS) < GEOM_TOLERANCE;
}
