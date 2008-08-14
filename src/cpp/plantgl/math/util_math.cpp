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




#include "util_polymath.h"
#include "util_vector.h"
#include "plantgl/tool/util_assert.h"
#include <cmath>
using namespace std;

TOOLS_BEGIN_NAMESPACE

/*
  Compute the value of ith Bernstein polynomial for a fixed u.
  (see the Nurbs Book p20)
*/

real_t bernstein( uint_t i, uint_t n, const real_t& u ) {
  vector<real_t> temp(n+1);
  real_t u1=1.0-u;
  for (uint_t j = 0; j <= n; j++)
    temp[j]=0.0;
  temp[n-i] = 1.0;
  for (uint_t k = 1; k <= n; k++)
    for (uint_t j = n; j >= k; j--)
      temp[j] = (u1*temp[j]) + (u*temp[j-1]);
  return temp[n];
}

/*
  Compute the n+1 nth-degree Bernstein polynomials for a fixed u.
  (see the Nurbs Book p21)
*/
vector<real_t> all_bernstein( uint_t n, const real_t& u ) {
  vector<real_t> B(n+1);
  B[0]=1.0;
  real_t u1=1.0-u;
  real_t saved, temp;  
  for(uint_t j=1; j<=n; j++){
      saved =0.0;
      for(uint_t k=0; k<j; k++){
	  temp = B[k];
	  B[k] = saved+u1*temp;
	  saved = u*temp;
      }
      B[j] = saved;
  }
  return B;
}


bool solve( const real_t& a, const real_t& b, const real_t& c, real_t& x1, real_t& x2 ) {
  GEOM_ASSERT(a);

  real_t d = b * b - 4 * a * c;
  if (d < 0) 
    return false;
  else
    if (d == 0)
      x1 = x2 = -b / ( 2 * a);
    else
      {
	x1 = x2 = 1 / (2 * a);
	real_t sqrt_d = sqrt(d);
	x1 *= -b - sqrt_d;
	x2 *= -b + sqrt_d;
      }
  return true;
}

TOOLS_END_NAMESPACE
