/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */


#include "util_array2.h"
#include "util_array.h"

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

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
