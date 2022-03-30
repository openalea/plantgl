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


/*! \file pgl_version.h
    \brief File for accessing to PGL version.
*/


#ifndef __pgl_version_h__
#define __pgl_version_h__

#include "../version.h"

#include "sg_config.h"
#include <string>
#include <vector>

/// PGL Version
extern SG_API float getPGLVersion();
extern SG_API int getPGLRevision();

/// PGL Version String
extern SG_API std::string getPGLVersionString();
extern SG_API int getPGLVersionNumber();

#define PGL_LIB_VERSION_CHECK \
     assert( PGL_VERSION == getPGLVersionNumber() && \
             "PlantGL version of the loaded library is different from the one at linking.");

extern SG_API const std::vector<std::string>& get_pgl_supported_extensions();
extern SG_API bool pgl_support_extension(const std::string& ext);

extern SG_API int getPGLQtVersion();
extern SG_API std::string getPGLQtVersionString();

#endif


