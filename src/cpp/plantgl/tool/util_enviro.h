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
				


/*! \file util_enviro.h
    \brief Function relative to the environment.
*/

#ifndef __util_enviro_h__
#define __util_enviro_h__

#include <string>
#include "tools_config.h"

PGL_BEGIN_NAMESPACE

/// Get the Home directory.
TOOLS_API std::string getHome();

/// Get OpenAlea directory.
TOOLS_API std::string getOpenAleaDir();

/// Get OpenAlea directory.
TOOLS_API std::string getPlantGLDir();

/// Set PlantGL directory.
TOOLS_API void setPlantGLDir(const std::string&);

/// Get User name
TOOLS_API std::string getUserName();

/// Get Family Name of the Operating System [ex:Windows,Linux].
TOOLS_API std::string getOSFamily();

/// Get Name of the Operating System [ex:Windows NT].
TOOLS_API std::string getOSName();

/// Get Release of the Operating System [ex:5.1].
TOOLS_API std::string getOSRelease();

/// Get Version of the Operating System [ex:2840].
TOOLS_API std::string getOSVersion();

/// Get the machine name.
TOOLS_API std::string getMachineName();

/// Get the architecture of the machine.
TOOLS_API std::string getArchitecture();

/// Get the compiler name.
TOOLS_API std::string getCompilerName();

/// Get the compiler version.
TOOLS_API std::string getCompilerVersion();

/// Get the language.
TOOLS_API std::string getOSLanguage();
TOOLS_API std::string getLanguage();
TOOLS_API void setLanguage(const std::string&);

PGL_END_NAMESPACE

#endif
