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



#ifndef __util_polymath_h__
#define __util_polymath_h__

/*! \file util_polymath.h
    \brief File that contains some utility about polynome.
*/

#include <vector>
#include "math_config.h"
#include "plantgl/tool/util_types.h"

PGL_BEGIN_NAMESPACE


/*! \fn std::vector<real_t> all_bernstein( uint_t n, const real_t& u );
   Computes the \e n + 1 th degree Bernstein polynomials for a fixed \e u.
   (see the Nurbs Book p21)
   usefull for Bezier Curve calcul.
*/
PGLMATH_API extern std::vector<real_t> all_bernstein( uint_t n, const real_t& u );


/*! \fn real_t bernstein( uint_t i, uint_t n, const real_t& u );
   Computes the value of \e i-th Bernstein polynomial for a fixed \e u.
   (see the Nurbs Book p20)
   usefull for Bezier Curve calcul.
*/
PGLMATH_API extern real_t bernstein( uint_t i, uint_t n, const real_t& u );



/*! \fn bool solve( const real_t& a, const real_t& b, const real_t& c, real_t& x1, real_t& x2 );
   Solves the equation ax2 + bx + c = 0 and returns whether the equation
   has some solutions \e x1 and \e x2.

   \pre
   - \e a must be different from 0.
*/
PGLMATH_API extern bool solve( const real_t& a,
           const real_t& b,
           const real_t& c,
           real_t& x1,
           real_t& x2 );

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

//__util_polymath_h__
#endif
