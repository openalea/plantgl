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




#include "box.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
PGL_USING_NAMESPACE


/* ----------------------------------------------------------------------- */

const Vector3 Box::DEFAULT_SIZE(.5,.5,.5);


/* ----------------------------------------------------------------------- */

Box::Builder::Builder() :
  ParametricModel::Builder(),
  Size(0) {
}


Box::Builder::~Builder() {
  // nothing to do
}


SceneObjectPtr Box::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Box(Size ? *Size : Box::DEFAULT_SIZE));
  return SceneObjectPtr();
}


void Box::Builder::destroy( ) {
  if (Size) delete Size;
}


bool Box::Builder::isValid( ) const {
  if (Size) {
    if ((Size->x() < GEOM_EPSILON) ||
    (Size->y() < GEOM_EPSILON) ||
    (Size->z() < GEOM_EPSILON)) {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Box","Size","The three values of size must not be null");
      return false;
    };
  };

  return true;
}


/* ----------------------------------------------------------------------- */


Box::Box( const Vector3& size ) :
  ParametricModel(),
  __size(size) {
  GEOM_ASSERT(isValid());
}

Box::~Box( ) {
}

bool Box::isValid( ) const {
  Builder _builder;
  _builder.Size = const_cast<Vector3 *>(&__size);
  return _builder.isValid();
}

SceneObjectPtr Box::copy(DeepCopier& copier) const
{
  return SceneObjectPtr(new Box(*this));
}

/* ----------------------------------------------------------------------- */

Vector3
Box::getBackLowerLeftCorner( ) const {
  return Vector3(-__size);
}

Vector3
Box::getBackLowerRightCorner( ) const {
  return Vector3(-__size.x(),__size.y(),-__size.z());
}

Vector3
Box::getBackNormal( ) const {
  return Vector3(-1,0,0);
}

Vector3
Box::getBackUpperLeftCorner( ) const {
  GEOM_ASSERT(isValid());
  return Vector3(-__size.x(),-__size.y(),__size.z());
}

Vector3
Box::getBackUpperRightCorner( ) const {
  return Vector3(-__size.x(),__size.y(),__size.z());
}

Vector3
Box::getFrontLowerLeftCorner( ) const {
  return Vector3(__size.x(),-__size.y(),-__size.z());
}

Vector3
Box::getFrontLowerRightCorner( ) const {
  return Vector3(__size.x(),__size.y(),-__size.z());
}

Vector3
Box::getFrontUpperLeftCorner( ) const {
  return Vector3(__size.x(),-__size.y(),__size.z());
}

Vector3
Box::getFrontUpperRightCorner( ) const {
  return Vector3(__size);
}

Vector3
Box::getFrontNormal( ) const {
  return Vector3(1,0,0);
}

Vector3
Box::getLeftNormal( ) const {
  return Vector3(0,-1,0);
}

Vector3
Box::getLowerNormal( ) const {
  return Vector3(0,0,-1);
}

Vector3
Box::getRightNormal( ) const {
  return Vector3(0,1,0);
}

Vector3
Box::getUpperNormal( ) const {
  return Vector3(0,0,1);
}

const Vector3&
Box::getSize( ) const {
  return __size;
}

Vector3&
Box::getSize( ){
  return __size;
}

bool
Box::isSizeToDefault( ) const {
  return __size == DEFAULT_SIZE;
}

/* ----------------------------------------------------------------------- */

bool
Box::isACurve( ) const {
  return false;
}

bool
Box::isASurface( ) const {
  return true;
}

bool
Box::isAVolume( ) const {
  return true;
}

/* ----------------------------------------------------------------------- */
