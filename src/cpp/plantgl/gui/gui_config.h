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


#ifndef __gui_config_h__
#define __gui_config_h__

#include "../pgl_namespace.h"

/* ----------------------------------------------------------------------- */


/*! \def VIEW_NODLL
    \brief Not creating dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
/*! \def VIEW_DLL
    \brief Using lib SceneGraph as a dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
/*! \def VIEW_MAKEDLL
    \brief Creating SceneGraph dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
#if defined(_WIN32)
#if defined(VIEW_NODLL)
#undef VIEW_MAKEDLL
#undef VIEW_DLL
#else
#ifndef VIEW_DLL
#define VIEW_DLL
#endif
#endif

#if defined(VIEW_MAKEDLL)
#ifndef VIEW_DLL
#define VIEW_DLL
#endif
#endif

#ifdef VIEW_DLL

#ifdef VIEW_MAKEDLL             /* create a Geom DLL library */
#define VIEW_API  __declspec(dllexport)
#undef VIEW_FWDEF
#else                                                   /* use a Geom DLL library */
#define VIEW_API  __declspec(dllimport)
#endif

#define VIEW_TEMPLATE_API(T) template class VIEW_API T;
#endif

#else // OS != _WIN32

#undef VIEW_MAKEDLL             /* ignore these for other platforms */
#undef VIEW_DLL

#endif

#ifndef VIEW_API
#define VIEW_API
#define VIEW_TEMPLATE_API(T) 
#endif


/* ----------------------------------------------------------------------- */

// __config_h__
#endif
