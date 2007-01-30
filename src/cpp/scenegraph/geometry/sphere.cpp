/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier 
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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





#include "sphere.h"
#include "disc.h"

#include <scenegraph/core/pgl_messages.h>
#include <math/util_math.h>

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
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Sphere","Stacks","Must be greater than 3.");
    return false;
  };
  
  // Radius field.
  if (Radius && (*Radius < REAL_EPSILON)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Sphere","Radius","Must be not null.");
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

bool 
Sphere::apply( Action& action ) {    
  return action.process(this);
}

bool Sphere::isValid( ) const {
  Builder _builder;
  _builder.Radius = const_cast<real_t *>(&__radius);
  _builder.Slices = const_cast<uchar_t *>(&__slices);
  _builder.Stacks = const_cast<uchar_t *>(&__stacks);
  return _builder.isValid();
}

SceneObjectPtr Sphere::copy() const {
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
