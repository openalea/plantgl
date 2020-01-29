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





#include "util_polymath.h"
#include "util_vector.h"
#include "plantgl/tool/util_assert.h"
#include <cmath>
using namespace std;

PGL_BEGIN_NAMESPACE

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

PGL_END_NAMESPACE
