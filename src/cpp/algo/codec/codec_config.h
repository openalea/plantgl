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


#ifndef __codec_config_h__
#define __codec_config_h__

#include "../pgl_namespace.h"

/* ----------------------------------------------------------------------- */


/*! \def CODEC_NODLL
    \brief Not creating dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
/*! \def CODEC_DLL
    \brief Using lib ACTN as a dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
/*! \def CODEC_MAKEDLL
    \brief Creating ACTN dll

    Uncomment to use this functionnality
        Do nothing on other platform than windows
*/
#if defined(_WIN32)
#if defined(CODEC_NODLL)
#undef CODEC_MAKEDLL
#undef CODEC_DLL
#else
#ifndef CODEC_DLL
#define CODEC_DLL
#endif
#endif

#if defined(CODEC_MAKEDLL)
#ifndef CODEC_DLL
#define CODEC_DLL
#endif
#endif

#ifdef CODEC_DLL

#ifdef CODEC_MAKEDLL             /* create a ACTN DLL library */
#define CODEC_API  __declspec(dllexport)
#undef CODEC_FWDEF
#else                                                   /* use a ACTN DLL library */
#define CODEC_API  __declspec(dllimport)
#endif

#define CODEC_TEMPLATE_API(T) template class CODEC_API T;
#endif

#else // OS != _WIN32

#undef CODEC_MAKEDLL             /* ignore these for other platforms */
#undef CODEC_DLL

#endif

#ifndef CODEC_API
#define CODEC_API
#define CODEC_TEMPLATE_API(T) 
#endif


/* ----------------------------------------------------------------------- */

// __config_h__
#endif
