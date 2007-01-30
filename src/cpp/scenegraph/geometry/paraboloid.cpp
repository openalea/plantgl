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





#include "paraboloid.h"
#include <scenegraph/core/pgl_messages.h>

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
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Paraboloid","Shape","Must be not null.");
    return false;
  };

  // stacks field
  if (Stacks && (*Stacks < 4)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Paraboloid","Stacks","Must be greater than 3.");
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

bool 
Paraboloid::apply( Action& action ) {
  return action.process(this);
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

SceneObjectPtr Paraboloid::copy() const {
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
