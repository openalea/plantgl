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





#include "patch.h"
#include "mesh.h"

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

const bool Patch::DEFAULT_CCW(Mesh::DEFAULT_CCW);

/* ----------------------------------------------------------------------- */


Patch::Builder::Builder() :
    ParametricModel::Builder(),
    CCW(0){
}


Patch::Builder::~Builder() {
  // nothing to do
}


/* ----------------------------------------------------------------------- */


Patch::Patch(bool ccw) :
  ParametricModel(),
    __ccw(ccw){
}

Patch::~Patch( ) {
}

/* ----------------------------------------------------------------------- */

bool 
Patch::isACurve( ) const {
  return false;
}

bool
Patch::isASurface( ) const {
  return true;
} 

bool
Patch::isAVolume( ) const {
  return false;
}

const bool 
Patch::getCCW( ) const {
  return __ccw;
}

bool& 
Patch::getCCW( ) {
  return __ccw;
}

bool 
Patch::isCCWToDefault( ) const {
  return (__ccw==DEFAULT_CCW);
}
