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





#include "sor.h"
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const uchar_t SOR::DEFAULT_SLICES(8);


/* ----------------------------------------------------------------------- */


SOR::Builder::Builder() :
  ParametricModel::Builder(),
  Slices(0) {
}


SOR::Builder::~Builder() {
  // nothing to do
}


void SOR::Builder::destroy( ) {
  SORDestroy( );
}

void SOR::Builder::SORDestroy( ) {
  if (Slices) delete Slices;
}


bool SOR::Builder::isValid( ) const {
  return SORValid( );
}

bool SOR::Builder::SORValid( ) const {
  if (Slices && (*Slices < 4)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Surface Of Revolution","Slices","Must be greater than 3.");
    return false;
  };
  return true;
}


/* ----------------------------------------------------------------------- */


SOR::SOR( uchar_t slices ) :
  ParametricModel(),
  __slices(slices) {
}

SOR::~SOR( ) {
}

const uchar_t&
SOR::getSlices( ) const {
  return __slices;
}

uchar_t&
SOR::getSlices( ){
  return __slices;
}

bool
SOR::isACurve( ) const {
  return false;
}

bool
SOR::isASurface( ) const {
  return true;
}

bool
SOR::isSlicesToDefault( ) const {
  return __slices == DEFAULT_SLICES;
}

/* ----------------------------------------------------------------------- */


SOR2D::Builder::Builder() :
  PlanarModel::Builder(),
  Slices(0) {
}


SOR2D::Builder::~Builder() {
  // nothing to do
}


void SOR2D::Builder::destroy( ) {
	SOR2DDestroy( );
}

void SOR2D::Builder::SOR2DDestroy( ) {
  if (Slices) delete Slices;
}


bool SOR2D::Builder::isValid( ) const {
	return SOR2DValid( );
}

bool SOR2D::Builder::SOR2DValid( ) const {
  if (Slices && (*Slices < 4)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Surface Of Revolution 2D","Slices","Must be greater than 3.");
    return false;
  };
  return true;
}


/* ----------------------------------------------------------------------- */


SOR2D::SOR2D( uchar_t slices ) :
  PlanarModel(),
  __slices(slices) {
}

 SOR2D::~SOR2D( ) {
  }

const uchar_t&
SOR2D::getSlices( ) const {
  return __slices;
}

uchar_t&
SOR2D::getSlices( ) {
  return __slices;
}

bool
SOR2D::isACurve( ) const {
  return false;
}

bool
SOR2D::isASurface( ) const {
  return true;
}

bool
SOR2D::isSlicesToDefault( ) const {
  return __slices == SOR::DEFAULT_SLICES;
}

/* ----------------------------------------------------------------------- */

