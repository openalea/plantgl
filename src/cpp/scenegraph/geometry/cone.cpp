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





#include "cone.h"
#include "disc.h"
#include <scenegraph/core/pgl_messages.h>
#include <math/util_math.h>
#include <math/util_vector.h>
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
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Cone","Height","Must be not null.");
    return false;
  };

  // Radius field.
  if (Radius && (*Radius < REAL_EPSILON)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Cone","Radius","Must be not null.");
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
Cone::apply( Action& action ) {    
  return action.process(this);
}

bool
Cone::isValid( ) const {
  Builder _builder;
  _builder.Radius = const_cast<real_t *>(&__radius);
  _builder.Height = const_cast<real_t *>(&__height);
  _builder.Slices = const_cast<uchar_t *>(&__slices);
  return _builder.isValid();
}
  
SceneObjectPtr Cone::copy() const {
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
