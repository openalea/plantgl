/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       $Source: /usr/cvsmaster/AMAPmod/src/Tools/config.h,v $
 *       $Id: config.h,v 1.34 2006/08/21 12:12:46 fboudon Exp $
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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


#ifndef __math_config_h__
#define __math_config_h__

#include "../tool/tools_config.h"

/* ----------------------------------------------------------------------- */


/*! \def GEOM_NODLL
    \brief Not creating dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
/*! \def GEOM_DLL
    \brief Using lib GEOM as a dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
/*! \def GEOM_MAKEDLL
    \brief Creating GEOM dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
#if defined(_WIN32)
#if defined(PGLMATH_NODLL)
#undef PGLMATH_MAKEDLL
#undef PGLMATH_DLL
#else
#ifndef PGLMATH_DLL
#define PGLMATH_DLL
#endif
#endif

#if defined(PGLMATH_MAKEDLL)
#ifndef PGLMATH_DLL
#define PGLMATH_DLL
#endif
#endif

#ifdef PGLMATH_DLL

#ifdef PGLMATH_MAKEDLL             /* create a Geom DLL library */
#define PGLMATH_API  __declspec(dllexport)
#undef PGLMATH_FWDEF
#else                                                   /* use a Geom DLL library */
#define PGLMATH_API  __declspec(dllimport)
#endif

#define PGLMATH_TEMPLATE_API(T) template class PGLMATH_API T;
#endif

#else // OS != _WIN32

#undef PGLMATH_MAKEDLL             /* ignore these for other platforms */
#undef PGLMATH_DLL

#endif

#ifndef PGLMATH_API
#define PGLMATH_API
#define PGLMATH_TEMPLATE_API(T) 
#endif



/* ----------------------------------------------------------------------- */

// __config_h__
#endif
