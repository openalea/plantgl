/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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


#ifndef __algo_config_h__
#define __algo_config_h__

#include "../pgl_namespace.h"

/* ----------------------------------------------------------------------- */


/*! \def ALGO_NODLL
    \brief Not creating dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
/*! \def ALGO_DLL
    \brief Using lib SceneGraph as a dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
/*! \def ALGO_MAKEDLL
    \brief Creating SceneGraph dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
#if defined(_WIN32)
#if defined(ALGO_NODLL)
#undef ALGO_MAKEDLL
#undef ALGO_DLL
#else
#ifndef ALGO_DLL
#define ALGO_DLL
#endif
#endif

#if defined(ALGO_MAKEDLL)
#ifndef ALGO_DLL
#define ALGO_DLL
#endif
#endif

#ifdef ALGO_DLL

#ifdef ALGO_MAKEDLL             /* create a Geom DLL library */
#define ALGO_API  __declspec(dllexport)
#undef ALGO_FWDEF
#else                                                   /* use a Geom DLL library */
#define ALGO_API  __declspec(dllimport)
#endif

#define ALGO_TEMPLATE_API(T) template class ALGO_API T;
#endif

#else // OS != _WIN32

#undef ALGO_MAKEDLL             /* ignore these for other platforms */
#undef ALGO_DLL

#endif

#ifndef ALGO_API
#define ALGO_API
#define ALGO_TEMPLATE_API(T) 
#endif


/* ----------------------------------------------------------------------- */

// __config_h__
#endif
