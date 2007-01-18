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
#endif

#ifdef __GNUC__
#include <sys/param.h>
#include <sys/utsname.h>
#include <unistd.h>
#endif

#ifdef SYSTEM_IS__Linux
// AML
// Apparament, il y a un probleme avec la fonction cuserid,
// qui pourtant est bien defini dans stdio.h
// Je ne sais pas faire mieux que de la definir "a la main".
extern "C" char *cuserid(char *);
#endif

using namespace std;
TOOLS_BEGIN_NAMESPACE

static string SYMBOL_DIR;
static string AMAPMOD_DIR;

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

string getCwd(){
  char cwd_path[MAXPATHLEN];
  if (getcwd(cwd_path, MAXPATHLEN) == NULL) { // stores the current cwd
    cwd_path[0] = '\0'; // empty string denotes an error
    perror("getcwd");
  }
  return cwd_path;
}

#ifdef _WIN32
string predefKey(HKEY key){
	if(key == HKEY_CURRENT_USER)return "HKEY_CURRENT_USER";
	else if(key == HKEY_LOCAL_MACHINE)return "HKEY_LOCAL_MACHINE";
	else return "";
}

#ifdef UNICODE
bool setRegValue(HKEY key,
				 const std::string& SubKey,
				 const std::string& ValueName,
				 const std::string& ValueData) {
	bool result = true;

	// address of name of subkey to open
	LPTSTR lpSubKey = new WORD[SubKey.size()+1];
	for(int i = 0 ; i < SubKey.size() ; i++){
		lpSubKey[i] = SubKey[i];
	}
	lpSubKey[SubKey.size()] = '\0';

	// address of handle to open key
	PHKEY phkResult = new HKEY;
	
	if(RegCreateKeyEx(key,lpSubKey,0,NULL, 
					  REG_OPTION_NON_VOLATILE, 
					  KEY_ALL_ACCESS,
					  NULL, 
					  phkResult, 
					  NULL) == ERROR_SUCCESS){
		
		// address of name of value to query
		LPTSTR lpValueName = new WORD[ValueName.size()+1];
		for(int i = 0 ; i < ValueName.size() ; i++){
			lpValueName[i] = ValueName[i];
		}

		lpValueName[ValueName.size()] = '\0';
		DWORD lpType   = REG_SZ; // value type

		// address of data buffer
		BYTE* lpData    = (BYTE*)new WORD[ValueData.size()+1];
		LPTSTR p = (LPTSTR)lpData;
		for(int j = 0 ; j < ValueData.size() ; j++){
			p[j] = ValueData[j];
		}
		p[ValueData.size()] = '\0';

		// data buffer size
		DWORD lpcbData = DWORD((ValueData.size()+1)*sizeof(WORD));  
		
		if(RegSetValueEx(*phkResult, lpValueName , 
					     NULL, lpType, 
						 lpData, lpcbData ) == ERROR_SUCCESS){
		}
		else {
			result = false;
			printf("Error when writing registry value '%s' in key %s\\%s\n",
				    ValueName.c_str(),predefKey(key).c_str(),SubKey.c_str());
		}
		if(RegCloseKey(*phkResult) != ERROR_SUCCESS){
				printf("Warning ! Error when closing key %s\\%s\n",
					    predefKey(key).c_str(),SubKey.c_str());
			result = false;
		}
		delete [] lpValueName;
		delete [] lpData;
	}
	else {
		printf("Error when opening registry key %s\\%s\n",
			   predefKey(key).c_str(),SubKey.c_str());
		result = false;
	}
	delete phkResult;
	return result;
}


bool getRegValue(HKEY key,
				 const std::string& SubKey,
				 const std::string& ValueName,
				 std::string& ValueData) {

	bool result = false;

	// address of name of subkey to open
	LPTSTR lpSubKey = new WORD[SubKey.size()+1];
	for(int i = 0 ; i < SubKey.size() ; i++){
		lpSubKey[i] = SubKey[i];
	}
	lpSubKey[SubKey.size()] = '\0';

	// address of handle to open key
	PHKEY phkResult = new HKEY;
	
	if(RegOpenKeyEx(key,lpSubKey,
					0,KEY_QUERY_VALUE, 
					phkResult) == ERROR_SUCCESS){
		
		// address of name of value to query
		LPTSTR lpValueName = new WORD[ValueName.size()+1];
		for(int i = 0 ; i < ValueName.size() ; i++){
			lpValueName[i] = ValueName[i];
		}
		lpValueName[ValueName.size()] = '\0';

		// address of buffer for value type
		LPDWORD lpType   = new DWORD(REG_SZ);
		// address of data buffer
		LPBYTE lpData    = (LPBYTE)new WORD[MAXPATHLEN];
		// address of data buffer size
		LPDWORD lpcbData = new DWORD(MAXPATHLEN*sizeof(WORD));
		
		if(RegQueryValueEx(*phkResult, lpValueName , 
						   NULL, lpType, 
						   lpData, lpcbData ) == ERROR_SUCCESS 
			&& *lpcbData <= (MAXPATHLEN*sizeof(WORD))){
			if(*lpType != REG_SZ){
			printf("Warning ! Error of type value '%s' of key %s\\%s\n",
				    ValueName.c_str(),predefKey(key).c_str(),SubKey.c_str());

			}
			else {				
				string res;
				LPTSTR p = (LPTSTR)lpData;
//				printf("Value %s data size : %i / %i = %i\n",
//					ValueName.c_str(),
//					(*lpcbData),sizeof(WORD),
//					((*lpcbData)/sizeof(WORD)));
				for(int i = 0 ; i < ((*lpcbData)/sizeof(WORD)) ; i++){
					char c = p[i];
					res += c;
				}
//				printf("Value %s data : '%s'\n",
//					ValueName.c_str(),
//					res);
				if(res.size() != 0 && res[0]!='\0'){
					ValueData = res;
					ValueData += '\0';
					result = true;
				}
			}
		}
		else {
//			printf("Warning ! Error when querying value '%s' of key %s\\%s\n",
//				    ValueName.c_str(),predefKey(key).c_str(),SubKey.c_str());
		}
		if(RegCloseKey(*phkResult) != ERROR_SUCCESS){
//				printf("Warning ! Error when closing key %s\\%s\n",
//					    predefKey(key).c_str(),SubKey.c_str());
		}
		delete [] lpValueName;
		delete lpType;
		delete [] lpData;
		delete lpcbData;
	}
	else {
//		printf("Error when opening registry key %s\\%s\n",
//			   predefKey(key).c_str(),SubKey.c_str());
	}
	delete phkResult;
	return result;
}
#endif

bool setRegValueA(HKEY key,
				 const std::string& SubKey,
				 const std::string& ValueName,
				 const std::string& ValueData) {
	bool result = true;

	// address of handle to open key
	HKEY phkResult;
	
	if(RegCreateKeyExA(key,SubKey.c_str(),0,NULL, 
					  REG_OPTION_NON_VOLATILE, 
					  KEY_ALL_ACCESS,
					  NULL, 
					  &phkResult, 
					  NULL) == ERROR_SUCCESS){
		
		DWORD lpType   = REG_SZ; // value type

     	// data buffer size
		DWORD lpcbData((ValueData.size()+1));  
		
		if(RegSetValueExA(phkResult, ValueName.c_str() , 
					     NULL, lpType, 
						 (BYTE*)ValueData.c_str(), lpcbData ) == ERROR_SUCCESS){
		}
		else {
			result = false;
			printf("Error when writing registry value '%s' in key %s\\%s\n",
				    ValueName.c_str(),predefKey(key).c_str(),SubKey.c_str());
		}
		if(RegCloseKey(phkResult) != ERROR_SUCCESS){
				printf("Warning ! Error when closing key %s\\%s\n",
					    predefKey(key).c_str(),SubKey.c_str());
			result = false;
		}
	}
	else {
		printf("Error when opening registry key %s\\%s\n",
			   predefKey(key).c_str(),SubKey.c_str());
		result = false;
	}
	return result;
}

bool getRegValueA(HKEY key,
				 const std::string& SubKey,
				 const std::string& ValueName,
				 std::string& ValueData) {

	bool result = false;

	// address of handle to open key
	HKEY phkResult;
	
	if(RegOpenKeyExA(key,SubKey.c_str(),
					0,KEY_QUERY_VALUE, 
					&phkResult) == ERROR_SUCCESS){
		

		// address of buffer for value type
		DWORD lpType   = REG_SZ;
		// address of data buffer
		LPBYTE lpData    = (LPBYTE)new char[MAXPATHLEN];
		// address of data buffer size
		DWORD lpcbData(MAXPATHLEN);
		
		if(RegQueryValueExA(phkResult, ValueName.c_str() , 
						   NULL, &lpType, 
						   lpData, &lpcbData ) == ERROR_SUCCESS 
			&& lpcbData <= MAXPATHLEN){
			if(lpType != REG_SZ){
			printf("Warning ! Error of type value '%s' of key %s\\%s\n",
				    ValueName.c_str(),predefKey(key).c_str(),SubKey.c_str());

			}
			else {				
				string res = (char *)lpData;
				if(res.size() != 0 && res[0]!='\0'){
					ValueData = res;
					result = true;
				}
			}
		}
		else {
//			printf("Warning ! Error when querying value '%s' of key %s\\%s\n",
//				    ValueName.c_str(),predefKey(key).c_str(),SubKey.c_str());
		}
		if(RegCloseKey(phkResult) != ERROR_SUCCESS){
//				printf("Warning ! Error when closing key %s\\%s\n",
//					    predefKey(key).c_str(),SubKey.c_str());
		}
		delete [] lpData;
	}
	else {
//		printf("Error when opening registry key %s\\%s\n",
//			   predefKey(key).c_str(),SubKey.c_str());
	}
	return result;
}

#endif

void setAMAPmodDir(const std::string& dir){
	AMAPMOD_DIR = dir;
#ifdef _WIN32
	setRegValueA(HKEY_CURRENT_USER,"Software\\AMAP\\AMAPmod","InstallPath",dir);
#endif
}

void setSymbolDir(const std::string& dir){
	SYMBOL_DIR = dir;
#ifdef _WIN32
	setRegValueA(HKEY_CURRENT_USER,"Software\\AMAP\\AMAPmod","SMBPath",dir);
#endif
}

string getAMAPmodDir(){
		if(!AMAPMOD_DIR.empty())return AMAPMOD_DIR;
#ifdef _WIN32
        char * dir = getenv("AMAPMOD_DIR");
        if(dir){
			AMAPMOD_DIR = short_dirname(string(dir));
			return AMAPMOD_DIR;
		}
//		printf("getRegValueA(HKEY_CURRENT_USER,\"Software\\AMAP\\AMAPmod\",\"InstallPath\",AMAPMOD_DIR);\n");
		if(!getRegValueA(HKEY_CURRENT_USER,"Software\\AMAP\\AMAPmod",
			"InstallPath",AMAPMOD_DIR)){
//		    printf("getRegValueA(HKEY_LOCAL_MACHINE,\"Software\\AMAP\\AMAPmod\",\"InstallPath\",AMAPMOD_DIR);\n");
			if(!getRegValueA(HKEY_LOCAL_MACHINE,"Software\\AMAP\\AMAPmod",
			"InstallPath",AMAPMOD_DIR)){
				printf("WARNING : AMAPMOD_DIR is not set.\n");
			}
		}
//		printf("AMAPMOD_DIR = '%s'\n",AMAPMOD_DIR.c_str());
		if(AMAPMOD_DIR.empty())AMAPMOD_DIR = string("C:\\");
		else AMAPMOD_DIR = short_dirname(AMAPMOD_DIR);
#else
        char * dir = getenv("AMAPMOD_DIR");
        if(!dir)dir = "/usr/local/AMAPmod";
		AMAPMOD_DIR = string(dir);
#endif
		return AMAPMOD_DIR;
}

string getSymbolDir(){
		if(!SYMBOL_DIR.empty())return SYMBOL_DIR;
#ifdef _WIN32
        char * dir = getenv("SYMBOL_DIR");
        if(dir){
			SYMBOL_DIR = string(dir);
			return SYMBOL_DIR;
		}

		if(!getRegValueA(HKEY_CURRENT_USER,"Software\\AMAP\\AMAPmod",
			"SMBPath",SYMBOL_DIR)){
			getRegValueA(HKEY_LOCAL_MACHINE,"Software\\AMAP\\AMAPmod",
			"SMBPath",SYMBOL_DIR);
		}
		if(SYMBOL_DIR.empty())
			SYMBOL_DIR = getAMAPmodDir() + "/databases/SMBFiles";
#else
        char * dir = getenv("SYMBOL_DIR");
        if(!dir)dir = "/usr/local/AMAPmod/databases/SMBFiles";
		SYMBOL_DIR = string(dir);
        return SYMBOL_DIR;
#endif
	return SYMBOL_DIR;
}


string getUserName(){
#ifdef _WIN32
/*#ifdef UNICODE
		LPTSTR lpBuffer = new WORD[UNLEN];
        LPDWORD nSize = new DWORD(UNLEN);
        if(GetUserNameW(lpBuffer,nSize)){
                string uname;
                for(DWORD i = 0 ; i < *nSize ; i++){
                char c = lpBuffer[i];
                uname += c;
                }
                delete nSize;
                delete lpBuffer;
                return uname;
        }
        else{
                delete nSize;
                delete lpBuffer;
                return string("Windows User");
        }
#else*/
		char lpBuffer[UNLEN];
        DWORD nSize(UNLEN);
        if(GetUserNameA(lpBuffer,&nSize)){
           return string(lpBuffer);
        }
        else{
           return string("Windows User");
        }

// #endif
#else
        char uname[L_cuserid]; // defined in stdio.h;
    (char*)cuserid(uname);
    return string(uname);
#endif
}

string getOSFamily(){
#ifdef SYSTEM_IS__Linux
        return string("Linux");
#endif
#ifdef SYSTEM_IS__IRIX
        return string("Irix");
#endif
#ifdef SYSTEM_IS__CYGWIN
        return string("Cygwin");
#endif
#ifdef _WIN32
        return string("Windows");
#endif
        return string("");
}

string getOSName(){
#ifdef __GNUC__
    struct utsname buf;
    uname(&buf);
        return string(buf.sysname);
#endif
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
#endif
        return string("");
}

string getOSVersion(){
#ifdef __GNUC__
    struct utsname buf;
    uname(&buf);
    return string(buf.version);
#endif
#ifdef _WIN32
        LPOSVERSIONINFO lpVersionInformation = new OSVERSIONINFO;
        lpVersionInformation->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if(GetVersionEx(lpVersionInformation)){
                return number(lpVersionInformation->dwBuildNumber);
        }
#endif
        return string("");
}

string getOSRelease(){
#ifdef __GNUC__
    struct utsname buf;
    uname(&buf);
    return string(buf.version);
#endif
#ifdef _WIN32
        LPOSVERSIONINFO lpVersionInformation = new OSVERSIONINFO;
        lpVersionInformation->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if(GetVersionEx(lpVersionInformation)){
                return number(lpVersionInformation->dwMajorVersion)
                                + '.' + number(lpVersionInformation->dwMinorVersion) ;
        }
#endif
        return string("");
}

string getMachineName(){
#ifdef __GNUC__
    struct utsname buf;
    uname(&buf);
    return string(buf.nodename);
#endif
#ifdef _WIN32
/*#ifdef UNICODE
		LPTSTR lpBuffer = new WORD[MAX_COMPUTERNAME_LENGTH+1]; // address of name buffer
        LPDWORD nSize = new DWORD(MAX_COMPUTERNAME_LENGTH+1);
        if(GetComputerName(lpBuffer,nSize)){
                string machine_name;
                for(int i = 0 ; i < *nSize ; i++){
                        char c = lpBuffer[i];
                        machine_name += c;
                }
                return machine_name;
        }
#else*/
		char lpBuffer[MAX_COMPUTERNAME_LENGTH+1]; // buffer
        DWORD nSize(MAX_COMPUTERNAME_LENGTH+1);
        if(GetComputerNameA(lpBuffer,&nSize)){
                return string(lpBuffer);
        }

// #endif
#endif
        return string("");
}

string getArchitecture(){
#ifdef __GNUC__
    struct utsname buf;
    uname(&buf);
    return string(buf.machine);
#endif
#ifdef _WIN32

#ifdef _M_ALPHA
    return string("DEC ALPHA");
#endif
#ifdef _M_IX86
    int m_i86 = _M_IX86 + 86;
    return string("i") + number(m_i86);
#endif
#ifdef _M_MPPC
    int m_mppc = _M_MPPC;
    return string("Mac Power PC ") + number(m_mppc);
#endif
#ifdef _M_PPC
      int m_ppc = _M_PPC;
      return string("Power PC ") + number(m_ppc);
#endif
#ifdef _M_MRX000
      int m_mrx = _M_MRX000;
      return string("R") + number(m_mrx);
#endif

#endif
        return string("");
}

string getCompilerName(){
    string c_name;
#ifdef __GNUC__
    c_name = "GNU C++";
#endif
#if defined (_MSC_VER)
    c_name = "Microsoft Visual C++";
#endif
        return c_name;
}

string getCompilerVersion(){
    string c_version;
#ifdef __GNUC__
    c_version = __VERSION__ ;
#endif
#if defined (_MSC_VER)
    c_version = number(_MSC_VER) ;
    if(c_version == "1200") c_version = "6.0 (1200)";
#endif
        return c_version;
}

string getOSLanguage(){
    string lang = "English";
#ifdef __GNUC__
#endif
#ifdef _WIN32
	  int cchData = GetLocaleInfoA(LOCALE_USER_DEFAULT,LOCALE_SENGLANGUAGE,NULL,0);
	  LPSTR lpLCData = new char[cchData];
	  cchData = GetLocaleInfoA(LOCALE_USER_DEFAULT,LOCALE_SENGLANGUAGE,lpLCData,cchData);
	  lang = string(lpLCData);
#endif
    return lang;
}

string getLanguage(){
    string lang = "English";
#ifdef __GNUC__
#endif
#ifdef _WIN32
	if(!getRegValueA(HKEY_CURRENT_USER,"Software\\AMAP\\AMAPmod",
			"Language",lang)){  	  
	  lang = getOSLanguage();
	  setRegValueA(HKEY_CURRENT_USER,"Software\\AMAP\\AMAPmod","Language",lang);
	}
#endif
    return lang;
}

void setLanguage(const string& lang){
#ifdef _WIN32
	  setRegValueA(HKEY_CURRENT_USER,"Software\\AMAP\\AMAPmod","Language",lang);
#endif
}

TOOLS_END_NAMESPACE

