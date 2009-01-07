/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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




#include "util_enviro.h"
#include "util_string.h"
#include "dirnames.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#include <winreg.h>
#include <LMCONS.H>
#define MAXPATHLEN _MAX_PATH

#else

#ifdef __GNUC__
#include <sys/param.h>
#include <sys/utsname.h>
#include <unistd.h>
#endif

#endif

using namespace std;

TOOLS_BEGIN_NAMESPACE

static string PLANTGL_DIR;
static string OPENALEA_DIR;

string getHome(){
        char * home = getenv("HOME");
#ifdef _WIN32
        if(!home)home = getenv("USERPROFILE");
        if(!home)home = "C:\\";
#else
        if(!home)home = "/";
#endif
        return string(home);
}

string getOpenAleaDir(){
		if(!OPENALEA_DIR.empty())return OPENALEA_DIR;
        char * dir = getenv("OPENALEADIR");
        if(!dir)
#ifdef _WIN32
			dir = "C:\\openalea";
#else
			dir = "/usr/local/openalea";
#endif
		OPENALEA_DIR = string(dir);
		return OPENALEA_DIR;
}

string getPlantGLDir(){
	if(!PLANTGL_DIR.empty())return PLANTGL_DIR;
    char * dir = getenv("PLANTGLDIR");
    if(!dir)PLANTGL_DIR = getOpenAleaDir();
	else PLANTGL_DIR = string(dir);
	return PLANTGL_DIR;
}

void setPlantGLDir(const std::string& dir){
	PLANTGL_DIR = dir;
}

string getUserName(){
#if defined(_WIN32)
	char lpBuffer[UNLEN];
    DWORD nSize(UNLEN);
    if(GetUserNameA(lpBuffer,&nSize)){
         return string(lpBuffer);
    }
    else{
         return string("Windows User");
    }
#elif defined(__APPLE__)
#warning username not defined
    return string("");
#elif defined(__GNUC__)
    char uname[L_cuserid]; // defined in stdio.h;
    (char*)cuserid(uname);
    return string(uname);
#else
#warning username not defined
	return string("");
#endif
}

string getOSFamily(){
#ifdef SYSTEM_IS__Linux
        return string("Linux");
#elif SYSTEM_IS__IRIX
        return string("Irix");
#elif SYSTEM_IS__CYGWIN
        return string("Cygwin");
#elif _WIN32
        return string("Windows");
#elif __APPLE__
        return string("MacOSX");
#else
  #warning OS Family not defined
		return string("");
#endif
}

string getOSName(){
#ifdef _WIN32
        string sys_name;
        LPOSVERSIONINFO lpVersionInformation = new OSVERSIONINFO;
        lpVersionInformation->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if(GetVersionEx(lpVersionInformation)){

                if(lpVersionInformation->dwPlatformId == VER_PLATFORM_WIN32s)
                        sys_name = "Windows 3.1";
                else if(lpVersionInformation->dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
                        sys_name = "Windows 9x";
                else if(lpVersionInformation->dwPlatformId == VER_PLATFORM_WIN32_NT)
                        sys_name = "Windows NT";

                sys_name += ' ' + number(lpVersionInformation->dwMajorVersion)
                                  + '.' + number(lpVersionInformation->dwMinorVersion) ;

                sys_name += " (";
                char a = lpVersionInformation->szCSDVersion[0];
                for(int i = 1 ; i < 128 && a != '\0'; i++){
                        sys_name += a;
                        a = lpVersionInformation->szCSDVersion[i];
                }
                sys_name += ")";
        }
        else sys_name = "Windows";
        return sys_name;
#elif defined (__GNUC__)
	struct utsname buf;
    uname(&buf);
    return string(buf.sysname);
#else
# warning OS Name not defined
	return string("");
#endif

}

string getOSVersion(){
#ifdef _WIN32
    LPOSVERSIONINFO lpVersionInformation = new OSVERSIONINFO;
    lpVersionInformation->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if(GetVersionEx(lpVersionInformation))
          return number(lpVersionInformation->dwBuildNumber);
    return string("");
#elif defined (__GNUC__)
    struct utsname buf;
    uname(&buf);
    return string(buf.version);
#else
	#warning OS Version not defined
    return string("");
#endif
}

string getOSRelease(){
#ifdef _WIN32
        LPOSVERSIONINFO lpVersionInformation = new OSVERSIONINFO;
        lpVersionInformation->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if(GetVersionEx(lpVersionInformation))
                return number(lpVersionInformation->dwMajorVersion)
                                + '.' + number(lpVersionInformation->dwMinorVersion) ;
        return string("");
#elif __GNUC__
    struct utsname buf;
    uname(&buf);
    return string(buf.version);
#else
#warning OS Release not defined
    return string("");
#endif
}

string getMachineName(){
#ifdef _WIN32
	char lpBuffer[MAX_COMPUTERNAME_LENGTH+1]; // buffer
    DWORD nSize(MAX_COMPUTERNAME_LENGTH+1);
    if(GetComputerNameA(lpBuffer,&nSize))
      return string(lpBuffer);
    return string("");
#elif __GNUC__
    struct utsname buf;
    uname(&buf);
    return string(buf.nodename);
#else
#warning machine name not defined
    return string("");
#endif
}

string getArchitecture(){
#ifdef _WIN32

#ifdef _M_ALPHA
    return string("DEC ALPHA");
#elif _M_IX86
    int m_i86 = _M_IX86 + 86;
    return string("i") + number(m_i86);
#elif _M_MPPC
    int m_mppc = _M_MPPC;
    return string("Mac Power PC ") + number(m_mppc);
#elif _M_PPC
      int m_ppc = _M_PPC;
      return string("Power PC ") + number(m_ppc);
#elif _M_MRX000
      int m_mrx = _M_MRX000;
      return string("R") + number(m_mrx);
#endif

#elif __GNUC__
    struct utsname buf;
    uname(&buf);
    return string(buf.machine);
#endif

        return string("");
}

string getCompilerName(){
    string c_name;
#if defined (_MSC_VER)
    c_name = "Microsoft Visual C++";
#elif __GNUC__
    c_name = "GNU C++";
#else
#warning compiler name not defined
#endif
    return c_name;
}

string getCompilerVersion(){
    string c_version;
#if defined (_MSC_VER)
    c_version = number(_MSC_VER) ;
    if(c_version == "1200") c_version = "6.0 (1200)";
#elif defined (__GNUC__)
    c_version = __VERSION__ ;
#else
#warning compiler version not defined
#endif
        return c_version;
}

string getOSLanguage(){
    string lang = "English";
#ifdef _WIN32
	  int cchData = GetLocaleInfoA(LOCALE_USER_DEFAULT,LOCALE_SENGLANGUAGE,NULL,0);
	  LPSTR lpLCData = new char[cchData];
	  cchData = GetLocaleInfoA(LOCALE_USER_DEFAULT,LOCALE_SENGLANGUAGE,lpLCData,cchData);
	  lang = string(lpLCData);
#elif __GNUC__
#warning os language not defined
#else
#warning os language not defined
#endif
    return lang;
}

string getLanguage(){
    string lang = getOSLanguage();
    return lang;
}

void setLanguage(const string& lang){
}

TOOLS_END_NAMESPACE

