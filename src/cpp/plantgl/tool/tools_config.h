/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */



#ifndef __tools_config_h__
#define __tools_config_h__
#include <string>
#include "../pgl_config.h"
#include "../pgl_namespace.h"

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
#if defined(TOOLS_NODLL)
#undef TOOLS_MAKEDLL
#undef TOOLS_DLL
#else
#ifndef TOOLS_DLL
#define TOOLS_DLL
#endif
#endif

#if defined(TOOLS_MAKEDLL)
#ifndef TOOLS_DLL
#define TOOLS_DLL
#endif
#endif

#ifdef TOOLS_DLL

#ifdef TOOLS_MAKEDLL             /* create a Geom DLL library */
#define TOOLS_API  __declspec(dllexport)
#undef TOOLS_FWDEF
#else                                                   /* use a Geom DLL library */
#define TOOLS_API  __declspec(dllimport)
#endif

#define TOOLS_TEMPLATE_API(T) template class TOOLS_API T;
#endif

#else // OS != _WIN32

#undef TOOLS_MAKEDLL             /* ignore these for other platforms */
#undef TOOLS_DLL

#endif

#ifndef TOOLS_API
#define TOOLS_API
#define TOOLS_TEMPLATE_API(T)
#endif


/* ----------------------------------------------------------------------- */

/// Macro for beginning the tools namespace.
#define TOOLS_BEGIN_NAMESPACE  PGL_BEGIN_NAMESPACE

/// Macro for ending the tools namespace.
#define TOOLS_END_NAMESPACE  PGL_END_NAMESPACE

/// Macro for using the tools namespace.
#define TOOLS_USING_NAMESPACE  PGL_USING_NAMESPACE

/// Macro for using an object of the tools namespace.
#define TOOLS_USING(obj) PGL_USING(obj)

/// Macro to use an object from the tools namespace.
#define TOOLS(obj) PGL(obj)


/* ----------------------------------------------------------------------- */

// __config_h__
#endif
