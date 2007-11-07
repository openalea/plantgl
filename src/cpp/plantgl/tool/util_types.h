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

#ifndef __util_types_h__
#define __util_types_h__
/*! 
	\file util_types.h
    \brief File that contains some utility about type. define the type real_t.
*/

#include "tools_config.h"

/* -------------------------------------------------------------------------- 
 * PRECISION OPTIONS
 * --------------------------------------------------------------------------
 */

#include <cfloat>

#ifdef GEOM_USE_DOUBLE
/// type for real values. can be float or double depending to the macro GEOM_USE_DOUBLE
typedef double real_t;
/// min real value
const real_t REAL_MIN = DBL_MIN;
/// max real value
const real_t REAL_MAX = DBL_MAX;
/// real epsilon value
const real_t REAL_EPSILON = DBL_EPSILON;
#else
/// type for real values. can be float or double depending to the macro GEOM_USE_DOUBLE
typedef float real_t;
/// min real value
const real_t REAL_MIN = FLT_MIN;
/// max real value
const real_t REAL_MAX = FLT_MAX;
/// real epsilon value
const real_t REAL_EPSILON = FLT_EPSILON;
#endif

/// Geom epsilon value
#define GEOM_EPSILON 1e-5
/// Geom tolerance value
#define GEOM_TOLERANCE 1e-10
/// Geom out of range value
#define GEOM_OUT_OF_RANGE LONG_MAX;

// Basic types redefinition

#ifdef __GNUC__
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#include <stdint.h>
#if defined( __MINGW32__ )
#define __int8_t_defined
#define __uint32_t_defined
#endif // __MINGW32__
#endif

#include <climits>
/// type uchar_t (unsigned char)
typedef unsigned char uchar_t;

#ifndef __int8_t_defined

/// type int16_t (short)
typedef short int16_t;
/// int16_t min value
#ifndef INT16_MIN 
#define INT16_MIN SHRT_MIN
#endif
/// int16_t max value
#ifndef INT16_MAX
#define INT16_MAX SHRT_MAX
#endif

#endif

#ifndef __uint32_t_defined

/// type uint16_t (unsigned short)
typedef unsigned short uint16_t;
/// uint16_t min value
#ifndef UINT16_MIN 
#define UINT16_MIN USHRT_MIN
#endif
/// uint16_t max value
#ifndef UINT16_MAX
#define UINT16_MAX USHRT_MAX
#endif
/// type int32_t (long)
typedef long int32_t;
/// int32_t min value
#ifndef INT32_MIN 
#define INT32_MIN LONG_MIN
#endif
/// int32_t max value
#ifndef INT32_MAX
#define INT32_MAX LONG_MAX
#endif
/// type uint32_t (unsigned long)
typedef unsigned long uint32_t;
/// uint32_t min value
#ifndef UINT32_MIN 
#define UINT32_MIN ULONG_MIN
#endif

/// uint32_t max value
#ifndef UINT32_MAX
#define UINT32_MAX ULONG_MAX
#endif

#endif


#include <stddef.h>


/* ----------------------------------------------------------------------- */

// __util_types_h__
#endif
