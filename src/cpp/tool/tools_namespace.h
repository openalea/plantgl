/* -*-c++-*-
 * ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) boudon
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
 *
 * ----------------------------------------------------------------------------
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
 * ----------------------------------------------------------------------------
 */


#ifndef __tools_namespace_h__
#define __tools_namespace_h__

/*! \file tools_namespace.h
    \brief Definition of tools namespace.

  If the macro \b NAMESPACE is defined, \n
  the \b TOOLS Lib will be compiled without \b NAMESPACE, \n
  Else it will be compiled in a namespace named \b TOOLS by default. \n
  To redefined the namespace name, you can redefined \n
  the macro \b TOOLS_NAMESPACE_NAME.
*/

#include "config.h"


#ifdef NO_NAMESPACE

#ifdef TOOLS_NAMESPACE
#undef TOOLS_NAMESPACE
#endif

#ifdef TOOLS_NAMESPACE_NAME
#undef TOOLS_NAMESPACE_NAME
#endif

#else

/// Macro that enable the tools namespace
#define TOOLS_NAMESPACE

#endif

#ifdef TOOLS_NAMESPACE

#ifndef TOOLS_NAMESPACE_NAME

/// Macro that contains the tools namespace name
#define TOOLS_NAMESPACE_NAME TOOLS
#endif

/// Macro for beginning the tools namespace.
#define TOOLS_BEGIN_NAMESPACE namespace TOOLS_NAMESPACE_NAME {

/// Macro for ending the tools namespace.
#define TOOLS_END_NAMESPACE };

/// Macro for using the tools namespace.
#define TOOLS_USING_NAMESPACE using namespace TOOLS_NAMESPACE_NAME;

/// Macro for using an object of the tools namespace.
#define TOOLS_USING(obj) using TOOLS_NAMESPACE_NAME::obj;

/// Macro to use an object from the tools namespace.
#define TOOLS(obj) TOOLS_NAMESPACE_NAME::obj

#else

#ifdef __GNUC__
#warning namespace TOOLS not used
#endif

/// Macro for beginning the tools namespace.
#define TOOLS_BEGIN_NAMESPACE  

/// Macro for ending the tools namespace.
#define TOOLS_END_NAMESPACE  

/// Macro for using the tools namespace.
#define TOOLS_USING_NAMESPACE  

/// Macro for using an object of the tools namespace.
#define TOOLS_USING(obj)

/// Macro to use an object from the tools namespace.
#define TOOLS(obj) obj

#endif


#endif // __tools_namespace_h__
