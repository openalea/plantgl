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




 
#include "geom_deformed.h"
#include "util_messages.h"
#include "geom_primitive.h"

GEOM_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

Deformation::Deformation( ) :
  Transformation3D() {
}

Deformation::~Deformation( ) {
}

/* ----------------------------------------------------------------------- */


/* ----------------------------------------------------------------------- */


Deformed::Builder::Builder() :
  Transformed::Builder(),
  Primitive(0) {
}


void Deformed::Builder::destroy() {
	DefDestroy();
}

void Deformed::Builder::DefDestroy() {
  if (Primitive) delete Primitive;
}


bool Deformed::Builder::isValid( ) const {
	return DefValid( ) ;
}

bool Deformed::Builder::DefValid( ) const {
  if (! Primitive) {
    genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Deformed","Primitive");
    return false;
  };
  if (! (*Primitive)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Deformed","Primitive","Must be a valid Primitive.");
    return false;
  };
  return true;
}


/* ----------------------------------------------------------------------- */


Deformed::Deformed() :
  Transformed(),
  __primitive(0) {
}

Deformed::Deformed( const PrimitivePtr& primitive ) :
  Transformed(),
  __primitive(primitive) {
}

Deformed::~Deformed( ) {
}

const GeometryPtr 
Deformed::getGeometry( ) const {
  return GeometryPtr(__primitive);
}

const PrimitivePtr& 
Deformed::getPrimitive( ) const {
  return __primitive;
}

PrimitivePtr& 
Deformed::getPrimitive( ){
  return __primitive;
}

bool 
Deformed::isACurve( ) const {
  return __primitive->isACurve();
}

bool 
Deformed::isASurface( ) const {
  return __primitive->isASurface();
}

bool 
Deformed::isAVolume( ) const {
  return __primitive->isAVolume();
}

bool 
Deformed::isExplicit( ) const {
  return __primitive->isExplicit();
}

/* ----------------------------------------------------------------------- */

