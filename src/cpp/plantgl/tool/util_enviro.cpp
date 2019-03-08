/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
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

#include "util_array.h"
#include "util_array2.h"

/* ----------------------------------------------------------------------- */

TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

/// Constructs an Array1 of size \e size
RealArray::RealArray( uint_t size  ) :
    NumericArray1<real_t>( size ) {
}

/// Constructs an Array1 with \e size copies of \e t.
RealArray::RealArray(uint_t size, const real_t& t ) :
    NumericArray1<real_t>( size, t ){
}

/// Destructor
RealArray::~RealArray( ) { }

/* ----------------------------------------------------------------------- */

RealArray2::RealArray2( uint_t row , uint_t col ) :
    NumericArray2<real_t>( row, col ) {
  }

RealArray2::RealArray2( uint_t row, uint_t col, const real_t& t ) :
    NumericArray2<real_t>( row, col, t ) {
  }

RealArray2::~RealArray2( ) {
}

/* ----------------------------------------------------------------------- */
