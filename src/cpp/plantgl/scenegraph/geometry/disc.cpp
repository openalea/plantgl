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
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Disc","Radius","Must be not null.");
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

bool 
Disc::apply( Action& action ) {
  return action.process(this);
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

SceneObjectPtr Disc::copy() const {
  return SceneObjectPtr(new Disc(*this));
}


/* ----------------------------------------------------------------------- */

