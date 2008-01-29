/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 2000-2007 UMR CIRAD/INRIA/INRA DAP 
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


#ifndef __util_polymath_h__
#define __util_polymath_h__

/*! \file util_polymath.h
    \brief File that contains some utility about polynome.
*/

#include <vector>
#include "math_config.h"
#include "plantgl/tool/util_types.h"

TOOLS_BEGIN_NAMESPACE


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

TOOLS_END_NAMESPACE

//__util_polymath_h__
#endif 
