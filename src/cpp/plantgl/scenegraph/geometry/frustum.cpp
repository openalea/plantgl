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






#include "frustum.h"
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const real_t Frustum::DEFAULT_TAPER(.5);


/* ----------------------------------------------------------------------- */


Frustum::Builder::Builder() :
  Cone::Builder(),
  Taper(0) {
}


Frustum::Builder::~Builder() {
  // nothing to do
}


SceneObjectPtr Frustum::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Frustum(Radius ? *Radius : DEFAULT_RADIUS,
           Height ? *Height : DEFAULT_HEIGHT,
           Taper ? *Taper : DEFAULT_TAPER,
           Solid ? *Solid : DEFAULT_SOLID,
           Slices ? *Slices : DEFAULT_SLICES));
  return SceneObjectPtr();
}


void Frustum::Builder::destroy() {
  ConeDestroy();
  if (Taper) delete Taper;
}


bool Frustum::Builder::isValid( ) const {
  if (! ConeValid()) return false;

  // taper field.
  if (Taper && (*Taper < REAL_EPSILON)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Frustum","Taper","Must be not null.");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */


Frustum::Frustum( const real_t& radius,
          const real_t& height,
          const real_t& taper,
          bool solid,
          uchar_t slices ) :
  Cone(radius,height,solid,slices),
  __taper(taper) {
  GEOM_ASSERT(isValid());
}

Frustum::~Frustum( ) {
}



bool Frustum::isValid( ) const {
  Builder _builder;
  _builder.Radius = const_cast<real_t *>(&__radius);
  _builder.Height = const_cast<real_t *>(&__height);
  _builder.Taper = const_cast<real_t *>(&__taper);
  _builder.Slices = const_cast<uchar_t *>(&__slices);
  return _builder.isValid();
}

SceneObjectPtr Frustum::copy(DeepCopier& copier) const {
  return SceneObjectPtr(new Frustum(*this));
}

/* ----------------------------------------------------------------------- */

const real_t&
Frustum::getTaper( ) const {
  return __taper;
}

real_t&
Frustum::getTaper( ) {
  return __taper;
}

bool
Frustum::isTaperToDefault( ) const {
  return __taper == DEFAULT_TAPER;
}

/* ----------------------------------------------------------------------- */
