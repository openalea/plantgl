/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
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


#ifndef __util_assert_h__
#define __util_assert_h__

/*! \file util_assert.h
    \brief Definition of the assert depending on debug mode is or off.
*/

#include "tools_config.h"
#include <assert.h>

/* --------------------------------------------------------------------------
 * DEBUG OPTIONS
 * --------------------------------------------------------------------------
 */

#ifdef GEOM_DEBUG
/// GEOM Assertion : Test on debug mode else do nothing
#  define GEOM_ASSERT(expr) \
   if (! (expr)) { \
     std::cerr << "*** GEOM : " << __FILE__ << ":" << __LINE__ << " " << std::endl; \
     std::cerr << "--> Assertion failure : " << #expr << std::endl; \
     abort(); \
   }
/// GEOM Trace : Test and trace on debug mode else do nothing
#  define GEOM_TRACE(expr) { \
     std::cerr << "*** GEOM : " << __FILE__ << ":" << __LINE__ << " " << std::endl; \
     std::cerr << "--> " << expr << std::endl; \
   }
#else
/// GEOM Assertion : Test on debug mode else do nothing
#  define GEOM_ASSERT(expr)
/// GEOM Trace : Test and trace on debug mode else do nothing
#  define GEOM_TRACE(expr)
#endif


/* ----------------------------------------------------------------------- */

// __config_h__
#endif
