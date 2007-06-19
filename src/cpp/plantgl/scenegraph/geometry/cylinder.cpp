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





#include "cylinder.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include "disc.h"
PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


Cylinder::Builder::Builder( ) :
  Cone::Builder() {
}


Cylinder::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr Cylinder::Builder::build( ) const {
  if (isValid()) 
    return SceneObjectPtr
      (new Cylinder(Radius ? *Radius : DEFAULT_RADIUS,
		    Height ? *Height : DEFAULT_HEIGHT,
		    Solid ? *Solid : DEFAULT_SOLID,
		    Slices ? *Slices : DEFAULT_SLICES));
  return SceneObjectPtr();
}


/* ----------------------------------------------------------------------- */


Cylinder::Cylinder( const real_t& radius,
		    const real_t& height,
		    bool solid,
		    uchar_t slices ) :
  Cone(radius,height,solid,slices) {
  GEOM_ASSERT(isValid());
}

Cylinder::~Cylinder( ) {
#ifdef GEOM_DEBUG
    cerr <<"Cylinder " <<  __name << " destroyed" << endl;    
#endif
}

bool 
Cylinder::apply( Action& action ) {    
  return action.process(this);
}

SceneObjectPtr Cylinder::copy() const {
  return SceneObjectPtr(new Cylinder(*this));
}

/* ----------------------------------------------------------------------- */
