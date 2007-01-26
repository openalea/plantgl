/* -*-c++-*- 
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): Ch. Godin (christophe.godin@cirad.fr) 
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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


/*! \file util_enviro.h
    \brief Function relative to the environment.
*/

#ifndef __util_enviro_h__
#define __util_enviro_h__

#include <string>
#include "tools_config.h"

TOOLS_BEGIN_NAMESPACE

/// Get the Home directory.
TOOLS_API std::string getHome();

/// Get the Current Working directory.
TOOLS_API std::string getCwd();

/// Get AMAPmod directory.
TOOLS_API std::string getAMAPmodDir();

/// Get Symbol directory.
TOOLS_API std::string getSymbolDir();

/// Set AMAPmod directory.
TOOLS_API void setAMAPmodDir(const std::string&);

/// Set Symbol directory.
TOOLS_API void setSymbolDir(const std::string&);

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

TOOLS_END_NAMESPACE

#endif
