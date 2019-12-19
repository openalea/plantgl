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





#include "cone.h"
#include "disc.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_vector.h>
PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const real_t Cone::DEFAULT_RADIUS(Disc::DEFAULT_RADIUS);
const real_t Cone::DEFAULT_HEIGHT(1);
const bool Cone::DEFAULT_SOLID(true);


/* ----------------------------------------------------------------------- */


Cone::Builder::Builder() :
    SOR::Builder(),
    Height(0),
    Solid(0),
    Radius(0){
}


Cone::Builder::~Builder() {
  // nothing to do
}


SceneObjectPtr Cone::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Cone(Radius ? *Radius : Disc::DEFAULT_RADIUS,
        Height ? *Height : DEFAULT_HEIGHT,
        Solid ? *Solid : DEFAULT_SOLID,
        Slices ? *Slices : DEFAULT_SLICES));
  return SceneObjectPtr();
}


void Cone::Builder::destroy() {
    ConeDestroy();
}

void Cone::Builder::ConeDestroy() {
    SORDestroy();
    if (Height) delete Height;
    if (Solid) delete Solid;
    if (Radius) delete Radius;
}


bool Cone::Builder::isValid( ) const {
  return ConeValid( );
}

bool Cone::Builder::ConeValid( ) const {
  if (! SORValid()) return false;

  // height field.
  if (Height && (*Height < REAL_EPSILON)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Cone","Height","Must be not null.");
    return false;
  };

  // Radius field.
  if (Radius && (*Radius < REAL_EPSILON)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Cone","Radius","Must be not null.");
    return false;
  };

  // Solid field is always valid.
  return true;
}


/* ----------------------------------------------------------------------- */

Cone::Cone( const real_t& radius,
        const real_t& height,
        bool solid,
        uchar_t slices ) :
    SOR(slices),
    __height(height),
    __solid(solid),
    __radius(radius){
    GEOM_ASSERT(isValid());
}

Cone::~Cone( ) {
}

bool
Cone::isValid( ) const {
  Builder _builder;
  _builder.Radius = const_cast<real_t *>(&__radius);
  _builder.Height = const_cast<real_t *>(&__height);
  _builder.Slices = const_cast<uchar_t *>(&__slices);
  return _builder.isValid();
}

SceneObjectPtr Cone::copy(DeepCopier& copier) const {
  return SceneObjectPtr(new Cone(*this));
}

/* ----------------------------------------------------------------------- */
const bool
Cone::getSolid( ) const {
  return __solid;
}

bool&
Cone::getSolid( ) {
  return __solid;
}

const real_t&
Cone::getHeight( ) const {
  return __height;
}

real_t&
Cone::getHeight( ) {
  return __height;
}

const real_t&
Cone::getRadius( ) const {
  return __radius;
}

real_t&
Cone::getRadius( )  {
  return __radius;
}

bool
Cone::isAVolume( ) const {
  return __solid;
}

bool
Cone::isHeightToDefault( ) const {
  return fabs(__height - DEFAULT_HEIGHT) < GEOM_EPSILON;
}

bool
Cone::isSolidToDefault( ) const {
  return __solid == DEFAULT_SOLID;
}

bool
Cone::isRadiusToDefault( ) const {
  return fabs(__radius - Disc::DEFAULT_RADIUS) < GEOM_TOLERANCE;
}

/* ----------------------------------------------------------------------- */
