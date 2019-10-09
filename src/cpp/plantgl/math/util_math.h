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




#ifndef __util_pgl_math_h__
#define __util_pgl_math_h__

/*! \file util_math.h
    \brief File that contains some math utility.
*/

#include <cmath>     // Common math function.
#include <algorithm> // For min, max ...

/* ----------------------------------------------------------------------- */

#ifndef PGL_MIN_MAX
#define PGL_MIN_MAX

template <typename T>
inline const T &pglMin(const T &a, const T &b) { return (a < b) ? a : b; }

template <typename T>
inline const T &pglMax(const T &a, const T &b) { return (a < b) ? b : a; }


#endif

template <typename T>
inline T min3(const T &a, const T &b, const T &c)
{ return pglMin(a, pglMin(b, c)); }

template <typename T>
inline T max3(const T &a, const T &b, const T &c)
{ return pglMax(a, pglMax(b, c)); }


#ifdef PGL_USE_DOUBLE

/// value of a degree in radian
#define GEOM_RAD 0.01745329252
#define GEOM_RAD 0.01745329252
/// value of a radian in degree
#define GEOM_DEG 57.295779513
/// value of 2 pi
#define GEOM_TWO_PI 6.2831853072

#ifdef M_PI
#  define GEOM_PI M_PI
#else
/// value of pi
#  define GEOM_PI 3.14159265358979323846
/// value of pi
#  define M_PI GEOM_PI
#endif //M_PI

#ifdef M_PI_2
/// value of pi / 2
#  define GEOM_HALF_PI M_PI_2
#else
/// value of pi / 2
#  define GEOM_HALF_PI 1.5707963286
/// value of pi / 2
#  define M_PI_2 GEOM_HALF_PI
#endif //M_PI_2

#else

/// value of a degree in radian
#define GEOM_RAD 0.01745329252f
/// value of a radian in degree
#define GEOM_DEG 57.295779513f
/// value of 2 pi
#define GEOM_TWO_PI 6.2831853072f

#ifdef M_PI
#  define GEOM_PI M_PI
#else
/// value of pi
#  define GEOM_PI 3.14159265358979323846f
/// value of pi
#  define M_PI GEOM_PI
#endif

#ifdef M_PI_2
/// value of pi / 2
#  define GEOM_HALF_PI M_PI_2
#else
/// value of pi / 2
#  define GEOM_HALF_PI 1.5707963286f
/// value of pi / 2
#  define M_PI_2 GEOM_HALF_PI
#endif

#endif

/// Returns the cube value of \e v.
inline float cb( const float& v ) {
  return v * v * v;
}

/// Returns the cube value of \e v.
inline double cb( const double& v ) {
  return v * v * v;
}

/// Computes the square value of \e v.
inline float sq( const float& v ) {
  return v * v;
}

/// Computes the square value of \e v.
inline double sq( const double& v ) {
  return v * v;
}

inline bool isPowerOfTwo(int val){
   long i=0;
   while ((1<<i) < val) i++;
   if (val==(1<<i)) return true;
   return false;
}

#ifdef _MSC_VER

/// On win32, redirect finite on _finite.
#ifndef HAVE_FINITE

#define pglfinite _finite

#define HAVE_FINITE

#endif

/// On win32, redefine cubic root.
inline double cbrt(double x){
    return pow(x,1/3);
}

#if (_MSC_VER > 1500)
#define HAVE_ROUND
#define HAVE_TRUNC
#endif

#ifndef HAVE_ROUND

/// On win32, redefine round double to int.
inline int rint(double x){
    int res = (int)x;
    if(x-res>0.5)res++;
    return res;
}

/// On win32, redefine round double to int.
#define round rint
// needed to avoid to include round by python C API (%PYTHONROOT%\include\pymath.h)
#define HAVE_ROUND
#endif

#ifndef HAVE_TRUNC
#define HAVE_TRUNC
/// On win32, redefine truncate double to int.
inline int trunc(double x){
    int res = (int)x;
    if(x-res>0.5)res++;
    return res;
}
#endif

#ifndef HAVE_HYPOT

#define hypot _hypot

#define HAVE_HYPOT
#endif

#else

#define pglfinite std::isfinite

#endif


/* ----------------------------------------------------------------------- */

//__util_math_h__
#endif
