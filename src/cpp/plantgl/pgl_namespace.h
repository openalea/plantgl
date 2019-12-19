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
