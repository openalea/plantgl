/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) 
 *
 *       $Source$
 *       $Id: vxl_tile.cpp 1169 2003-05-07 10:45:54Z puech $
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

#include "tile.h"

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


Tile::Tile(Tile * Complex, 
	   const unsigned char Scale, 
	   const TileType Type, 
	   const unsigned char Num) :
    __num(Num),
    __scale(Scale),
    __type(Type),
    __Complex(Complex){
}


Tile::~Tile(){
}


std::string Tile::getName() const {
	std::string name( (unsigned int)(__scale+1),'0');
    const Tile * _pt = this;
    int _i = __scale;
    while( _i >= 0 && _pt != NULL){
	name[_i--]= '0' +_pt->getNum();
	_pt = _pt->getComplex();
    }
    return name;
}


/* ----------------------------------------------------------------------- */

