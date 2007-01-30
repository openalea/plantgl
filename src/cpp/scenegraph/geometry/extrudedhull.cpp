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




#include "extrudedhull.h"
#include <scenegraph/core/pgl_messages.h>
#include "curve.h"
PGL_USING_NAMESPACE


/* ----------------------------------------------------------------------- */


const bool ExtrudedHull::DEFAULT_CCW(true);

/* ----------------------------------------------------------------------- */


ExtrudedHull::Builder::Builder( ) :
  Hull::Builder(),
  CCW(0),
  Horizontal(0),
  Vertical(0) {
}


ExtrudedHull::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr ExtrudedHull::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new ExtrudedHull(*Vertical,
			*Horizontal,
//			Orientation ? *Orientation : DEFAULT_ORIENTATION,
			CCW ? *CCW : DEFAULT_CCW));
  // Returns null as self is not valid.
  return SceneObjectPtr();
}


void ExtrudedHull::Builder::destroy( ) {
  if (CCW) delete CCW;
  if (Horizontal) delete Horizontal;
//  if (Orientation) delete Orientation;
  if (Vertical) delete Vertical;
}


bool ExtrudedHull::Builder::isValid( ) const {

  // Vertical
  if (! Vertical) {
      genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Extruded Hull","Vertical");
      return false;
    };

  // Vertical
  if (! (*Vertical)->isValid()) {
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Extruded Hull","Vertical","Must be a valid Object");
      return false;
    };

  // Horizontal
  if (! Horizontal) {
      genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Extruded Hull","Horizontal");
      return false;
    };

  // Horizontal
  if (! (*Horizontal)->isValid()) {
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Extruded Hull","Horizontal","Must be a valid Object");
      return false;
    };

  return true;
}


/* ----------------------------------------------------------------------- */


ExtrudedHull::ExtrudedHull() :
  Hull(),
  __vertical(0),
  __horizontal(0),
  __ccw(DEFAULT_CCW) {
}

ExtrudedHull::ExtrudedHull( const Curve2DPtr& vertical,
			    const Curve2DPtr& horizontal,
			    bool ccw ) :
  Hull(),
  __vertical(vertical),
  __horizontal(horizontal),
  __ccw(ccw) {
  GEOM_ASSERT(isValid());
}

ExtrudedHull::~ExtrudedHull( ) {
}

bool ExtrudedHull::apply( Action& action ) {    
  return action.process(this);
}

bool ExtrudedHull::isValid( ) const {
  Builder _builder;
  _builder.Horizontal = const_cast<Curve2DPtr *>(&__horizontal);
  _builder.Vertical = const_cast<Curve2DPtr *>(&__vertical);
  return _builder.isValid();
}

SceneObjectPtr ExtrudedHull::copy() const 
{
  ExtrudedHull * ptr = new ExtrudedHull(*this);
  if(__horizontal)ptr->getHorizontal().cast(__horizontal->copy());
  if(__vertical)ptr->getVertical().cast(__vertical->copy());
  return SceneObjectPtr(ptr);
}

/* ----------------------------------------------------------------------- */

const bool
ExtrudedHull::getCCW( ) const {
  return __ccw;
}

bool&
ExtrudedHull::getCCW( ){
  return __ccw;
}

const Curve2DPtr&
ExtrudedHull::getHorizontal( ) const {
  return __horizontal;
}

Curve2DPtr& 
ExtrudedHull::getHorizontal( ) {
  return __horizontal;
}

const Curve2DPtr& 
ExtrudedHull::getVertical( ) const {
  return __vertical;
}

Curve2DPtr&
ExtrudedHull::getVertical( ) {
  return __vertical;
}

bool
ExtrudedHull::isCCWToDefault( ) const {
  return __ccw == DEFAULT_CCW;
}
/* ----------------------------------------------------------------------- */

