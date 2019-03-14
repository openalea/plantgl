/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright  UMR CIRAD/INRIA/INRA DAP
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


#ifndef __geom_namespace_h__
#define __geom_namespace_h__

#include "pgl_config.h"

/*! \file geom_namespace.h
    \brief Definition of geom namespace.

   If the macro \b NO_NAMESPACE is defined, \n
   the \b GEOM Lib will be compiled without \b NAMESPACE, \n
   Else it will be compiled in a namespace named \b GEOM by default. \n
   To redefined the namespace name, you can redefined \n
   the macro \b GEOM_NAMESPACE_NAME.
*/

#ifdef NO_NAMESPACE

#ifdef PGL_NAMESPACE
#undef PGL_NAMESPACE
#endif

#ifdef PGL_NAMESPACE_NAME
#undef PGL_NAMESPACE_NAME
#endif

#else

/// Macro that enable the GEOM namespace
#define PGL_NAMESPACE

#endif

#ifdef PGL_NAMESPACE

#ifndef PGL_NAMESPACE_NAME
/// Macro that contains the GEOM namespace name
#define PGL_NAMESPACE_NAME pgl
#endif

/// Macro for beginning the GEOM namespace.
#define PGL_BEGIN_NAMESPACE namespace PGL_NAMESPACE_NAME {

/// Macro for ending the GEOM namespace.
#define PGL_END_NAMESPACE };

/// Macro for using the GEOM namespace.
#define PGL_USING_NAMESPACE using namespace PGL_NAMESPACE_NAME;

/// Macro for using an object of the GEOM namespace.
#define PGL_USING(obj) using PGL_NAMESPACE_NAME::obj;

/// Macro to use an object from the GEOM namespace.
#define PGL(obj) PGL_NAMESPACE_NAME::obj

#else

#ifdef __GNUC__
#warning namespace PGL not used
#endif

/// Macro for beginning the GEOM namespace.
#define PGL_BEGIN_NAMESPACE

/// Macro for ending the GEOM namespace.
#define PGL_END_NAMESPACE

/// Macro for using the GEOM namespace.
#define PGL_USING_NAMESPACE

/// Macro for using an object of the GEOM namespace.
#define PGL_USING(obj)

/// Macro to use an object from the GEOM namespace.
#define PGL(obj) obj

#endif


#endif // __geom_namespace_h__
