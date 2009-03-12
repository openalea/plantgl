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



#include "box.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE


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
