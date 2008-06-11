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


#ifndef __sg_config_h__
#define __sg_config_h__

#include "../pgl_namespace.h"

/* ----------------------------------------------------------------------- */


/*! \def SG_NODLL
    \brief Not creating dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
/*! \def SG_DLL
    \brief Using lib SceneGraph as a dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
/*! \def SG_MAKEDLL
    \brief Creating SceneGraph dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
#if defined(_WIN32)
#if defined(SG_NODLL)
#undef SG_MAKEDLL
#undef SG_DLL
#else
#ifndef SG_DLL
#define SG_DLL
#endif
#endif

#if defined(SG_MAKEDLL)
#ifndef SG_DLL
#define SG_DLL
#endif
#endif

#ifdef SG_DLL

#ifdef SG_MAKEDLL             /* create a Geom DLL library */
#define SG_API  __declspec(dllexport)
#undef SG_FWDEF
#else                                                   /* use a Geom DLL library */
#define SG_API  __declspec(dllimport)
#endif

#define SG_TEMPLATE_API(T) template class SG_API T;
#endif

#else // OS != _WIN32

#undef SG_MAKEDLL             /* ignore these for other platforms */
#undef SG_DLL

#endif

#ifndef SG_API
#define SG_API
#define SG_TEMPLATE_API(T) 
#endif


/* ----------------------------------------------------------------------- */

// __config_h__
#endif
