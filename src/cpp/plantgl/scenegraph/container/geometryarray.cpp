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





#include "geometryarray2.h"
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/geometry/lineicmodel.h>
#include <plantgl/scenegraph/geometry/curve.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

GeometryArray::GeometryArray (uint32_t size ) :
  GeomArray<GeometryPtr>( size){ 
}

GeometryArray::~GeometryArray()
{
}


/* ----------------------------------------------------------------------- */
Curve2DArray::Curve2DArray (uint32_t size ) :
  GeomArray<Curve2DPtr>( size){ 
}

Curve2DArray::~Curve2DArray (){
}

/* ----------------------------------------------------------------------- */

CurveArray::CurveArray (uint32_t size ) :
  GeomArray<LineicModelPtr>( size){ 
}

CurveArray::~CurveArray ()
{
}

/* ----------------------------------------------------------------------- */

/*
GeometryArray<T>::GeometryArray<T>( uint32_t size = 0 ) :
  Array1<T>(size) {
}


bool GeometryArray<T>::isValid( ) const {
  for (const_iterator _i = __A.begin(); _i != __A.end(); _i++) {
    if (! (*_i)) return false;
    if (! (*_i)->isValid()) return false;
  };
  return true;
}
*/

/* ----------------------------------------------------------------------- */
