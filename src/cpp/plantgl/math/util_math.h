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



#ifndef __util_math_h__
#define __util_math_h__

/*! \file util_math.h
    \brief File that contains some math utility.
*/

#include <cmath>     // Common math function.
#include <algorithm> // For min, max ...

/* ----------------------------------------------------------------------- */

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


#if defined(_MSC_VER)
#if (_MSC_VER <= 1500)

template <typename T>
inline const T &pglmin(const T &a, const T &b) { return (a < b) ? a : b; }

template <typename T>
inline const T &pglmax(const T &a, const T &b) { return (a < b) ? b : a; }


#else

#define pglmin std::min
#define pglmax std::max

#endif

#else

#define pglmin std::min
#define pglmax std::max

#endif


/* ----------------------------------------------------------------------- */

//__util_math_h__
#endif
