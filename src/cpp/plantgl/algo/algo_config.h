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
