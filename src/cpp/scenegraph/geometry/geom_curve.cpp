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





#include "geom_curve.h"
GEOM_USING_NAMESPACE


/* ----------------------------------------------------------------------- */

/*
Curve::Builder::Builder() :
  ParametricModel::Builder() {
}


Curve::Builder::~Builder() {
  // nothing to do
}
*/

/* ----------------------------------------------------------------------- */
/*

Curve::Curve( ) :
  ParametricModel() {
}

*/
/* ----------------------------------------------------------------------- */


Curve2D::Builder::Builder() :
    PlanarModel::Builder() {
}


Curve2D::Builder::~Builder() {
  // nothing to do
}


/* ----------------------------------------------------------------------- */


Curve2D::Curve2D( ) :
    PlanarModel () {
}

Curve2D::~Curve2D( ) {
}

bool Curve2D::isACurve( ) const {
  return true;
}

bool Curve2D::isASurface( ) const {
  return false;
} 

bool Curve2D::isAVolume( ) const {
  return false;
} 

/* ----------------------------------------------------------------------- */

