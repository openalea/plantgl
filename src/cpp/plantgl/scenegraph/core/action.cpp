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



#include "action.h"
#include <plantgl/pgl_scene.h>
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/appearance/appearance.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


Action::Action( ) {
  // nothing to do
}

Action::~Action( ) {
}

/* ----------------------------------------------------------------------- */

bool Action::beginProcess(){
  return true;
}

bool Action::endProcess(){
  return true;
}

/* ----------------------------------------------------------------------- */

bool Action::process(Shape * Shape){
    GEOM_ASSERT(Shape);
    bool b=Shape->geometry->apply(*this);
    return ( b && (Shape->appearance->apply(*this)));
}

bool Action::process(Inline * Inline){
    GEOM_ASSERT(Inline);
    if(Inline->getScene())
    return Inline->getScene()->applyGeometryFirst(*this);
    return false;
}


/* ----------------------------------------------------------------------- */
