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






#include "paraboloid.h"
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const real_t Paraboloid::DEFAULT_SHAPE(2);
const uchar_t Paraboloid::DEFAULT_STACKS(8);


/* ----------------------------------------------------------------------- */


Paraboloid::Builder::Builder() :
  Cone::Builder(),
  Shape(0),
  Stacks(0) {
}


Paraboloid::Builder::~Builder() {
  // nothing to do
}


SceneObjectPtr Paraboloid::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Paraboloid(Radius ? *Radius : DEFAULT_RADIUS,
              Height ? *Height : DEFAULT_HEIGHT,
              Shape ? *Shape : DEFAULT_SHAPE,
              Solid ? *Solid : DEFAULT_SOLID,
              Slices ? *Slices : DEFAULT_SLICES,
              Stacks ? *Stacks : DEFAULT_STACKS));
  return SceneObjectPtr();
}


void Paraboloid::Builder::destroy() {
  ConeDestroy();
  if (Shape) delete Shape;
  if (Stacks) delete Stacks;
}


bool Paraboloid::Builder::isValid( ) const {
  if (! ConeValid()) return false;

  // shape field.
  if (Shape && (*Shape < GEOM_EPSILON)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Paraboloid","Shape","Must be not null.");
    return false;
  };

  // stacks field
  if (Stacks && (*Stacks < 4)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Paraboloid","Stacks","Must be greater than 3.");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */


Paraboloid::Paraboloid( const real_t& radius,
            const real_t& height,
            const real_t& shape,
            bool solid,
            uchar_t slices,
            uchar_t stacks ) :
  Cone(radius,height,solid,slices),
  __shape(shape),
  __stacks(stacks) {
  GEOM_ASSERT(isValid());
}

Paraboloid::~Paraboloid( ) {
}

bool Paraboloid::isValid( ) const {
  Builder _builder;
  _builder.Radius = const_cast<real_t *>(&__radius);
  _builder.Height = const_cast<real_t *>(&__height);
  _builder.Shape = const_cast<real_t *>(&__shape);
  _builder.Slices = const_cast<uchar_t *>(&__slices);
  _builder.Stacks = const_cast<uchar_t *>(&__stacks);
  return _builder.isValid();
}

SceneObjectPtr Paraboloid::copy(DeepCopier& copier) const {
  return SceneObjectPtr(new Paraboloid(*this));
}


/* ----------------------------------------------------------------------- */

const real_t&
Paraboloid::getShape( ) const {
  return __shape;
}

real_t&
Paraboloid::getShape( ){
  return __shape;
}

const uchar_t&
Paraboloid::getStacks( ) const {
  return __stacks;
}

uchar_t&
Paraboloid::getStacks( ) {
  return __stacks;
}

bool
Paraboloid::isShapeToDefault( ) const {
  return __shape == DEFAULT_SHAPE;
}

bool
Paraboloid::isStacksToDefault( ) const {
  return __stacks == DEFAULT_STACKS;
}

/* ----------------------------------------------------------------------- */
