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


#include "dirnames.h"
#include "util_types.h"

#ifndef PGL_CORE_WITHOUT_QT
#include <QtCore/qfile.h>
#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#else
#ifdef __GNUC__
#include <unistd.h>
#endif
#endif

#ifdef _WIN32
#include <windows.h>
#include <Shlwapi.h>
#define MAXPATHLEN _MAX_PATH
#endif

using namespace std;
TOOLS_BEGIN_NAMESPACE

#ifndef PGL_CORE_WITHOUT_QT

inline std::string QString2StdString(const QString& st)
#if QT_VERSION >= QT_VERSION_CHECK(4,0,0)
#ifdef PGL_DEBUG
// On Windows, Qt can be compiled only with VC7 to have PyQt
// Compiling then PlantGL on VC8 will cause a string compatibility pb in debug mode
// for this we, reconstruct a string from char * data of the QString
{ return std::string(st.toAscii().data()); }
#else
{ return st.toStdString(); }
#endif
#else
{ return st.latin1(); }
#endif


#else
#ifdef _WIN32
std::string convert(LPCWSTR pwsz, UINT cp) {
    int cch = WideCharToMultiByte(cp, 0, pwsz, -1, 0, 0, NULL, NULL);
    char* psz = new char[cch];
    WideCharToMultiByte(cp, 0, pwsz, -1, psz, cch, NULL, NULL);
    std::string st(psz);
    delete[] psz;
   return st;
}

std::string convert(LPCSTR psz, UINT cp) {
    return std::string(psz);
}

#ifdef UNICODE
std::wstring toWinString(const std::string& fname) {
    int slength = (int)fname.length() + 1;
    int len = MultiByteToWideChar(CP_ACP, 0, fname.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, fname.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
#else
std::string toWinString(const std::string& fname) {
    return fname;
}
#endif

#endif

#endif

string get_dirname(const string & filename) {
#ifndef PGL_CORE_WITHOUT_QT
    return QString2StdString(QFileInfo(filename.c_str()).path());
#else
    size_t slashit = filename.rfind("/");
    size_t backslashit = filename.rfind("\\");
    if (slashit == std::string::npos && backslashit == std::string::npos) return filename;
    size_t end = slashit;
    if (slashit == std::string::npos || slashit <   backslashit)
        end = backslashit;
    return std::string(filename.begin(), filename.begin()+end);

#endif
}


string get_filename(const string & filename) {
#ifndef PGL_CORE_WITHOUT_QT
    return QString2StdString(QFileInfo(filename.c_str()).fileName());
#else
    size_t slashit = filename.rfind("/");
    size_t backslashit = filename.rfind("\\");
    if (slashit == std::string::npos && backslashit == std::string::npos) return filename;
    size_t begin = slashit;
    if (slashit == std::string::npos || slashit <   backslashit)
        begin = backslashit;
    return std::string(filename.begin()+begin+1, filename.end());

#endif
}

string absolute_dirname(const string & filename) {
#ifndef PGL_CORE_WITHOUT_QT
    return QString2StdString(QFileInfo(filename.c_str()).absolutePath());

#else
#ifdef _WIN32
    LPTSTR  lpBuffer = new TCHAR[MAXPATHLEN];
    DWORD nBufferLength(MAXPATHLEN);

    nBufferLength = GetFullPathName(toWinString(filename).c_str(),nBufferLength, lpBuffer, NULL);
    lpBuffer[nBufferLength] ='\0';
    string result = convert(lpBuffer, nBufferLength);
    return result;
#else
    char resolved_path[PATH_MAX];
    realpath(filename.c_str(), resolved_path);
    return string(resolved_path);
#endif

#endif
}

string absolute_filename(const string & filename) {
#ifndef PGL_CORE_WITHOUT_QT
    return QString2StdString(QFileInfo(filename.c_str()).absoluteFilePath());

#else
    return absolute_dirname(filename)+"/"+get_filename(filename);

#endif
}


#ifdef UNICODE
string short_dirnameU(const string& filename){
#ifdef _WIN32
    LPTSTR lpszShortPath = new TCHAR[MAXPATHLEN];
    DWORD cchBuffer(MAXPATHLEN);
    LPTSTR lpszLongPath  = new TCHAR[filename.length()+1];
    for(uint_t i = 0; i < filename.length(); i++)
        lpszLongPath[i] = filename[i];
    lpszLongPath[filename.length()] = '\0';

    cchBuffer = GetShortPathName(lpszLongPath,lpszShortPath, cchBuffer);
    string result;
    for(DWORD j = 0 ; j < cchBuffer; j++){
        char c = lpszShortPath[j];
        result += c;
    }
    return result;
#else
    return filename;
#endif
}
#endif

string short_dirname(const string& filename){
#ifdef _WIN32
    char * lpszShortPath = new char[MAXPATHLEN];
    DWORD cchBuffer(MAXPATHLEN);

    cchBuffer = GetShortPathNameA(filename.c_str(),lpszShortPath, cchBuffer);
    lpszShortPath[cchBuffer] ='\0';
    string result(lpszShortPath);
    return result;
#else
    return filename;
#endif
}


string get_cwd() {
#ifndef PGL_CORE_WITHOUT_QT
    return QString2StdString(QDir::currentPath());

#else
#ifdef _WIN32
    LPTSTR lpBuffer = new TCHAR[MAXPATHLEN];
    DWORD nBufferLength(MAXPATHLEN);

    nBufferLength = GetCurrentDirectory(nBufferLength, lpBuffer);
    lpBuffer[nBufferLength] ='\0';
    string result = convert(lpBuffer, nBufferLength);
    return result;

#else
    char resolved_path[PATH_MAX];
    getcwd(resolved_path, PATH_MAX);
    return string(resolved_path);
#endif
#endif
}

bool chg_dir(const string & newdir) {
    if(newdir == get_cwd()) return true;
#ifndef PGL_CORE_WITHOUT_QT
    return QDir::setCurrent(newdir.c_str());
#else
#ifdef _WIN32
    return SetCurrentDirectory(toWinString(newdir).c_str());

#else
    return chdir(newdir.c_str()) == 0;
#endif
#endif
}


string cat_dir_file(const string & filename, const string & name) {

  string file(filename);
  int l = file.length();
  if (l > 0) {                              // checks if the dirname ends with '/'
    if (file[l-1] != '/') {
      file.append("/");
    }
  }
  return file.append(name);

}

string get_suffix(const string & filename){
#ifdef PGL_CORE_WITHOUT_QT
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos) return string("");
    else return std::string(filename.begin()+filename.find_last_of('.')+1,filename.end());
#else
    QFileInfo fi(filename.c_str());
    return QString2StdString(fi.suffix());
#endif
}

string set_suffix(const string & filename,const string & extension){
#ifdef PGL_CORE_WITHOUT_QT
    return filename+'.'+extension;
#else
    QFileInfo fi(filename.c_str());
    QString nname = fi.path()+'/'+fi.baseName()+'.'+QString(extension.c_str());
    return QString2StdString(nname);
#endif
}

bool exists(const string & filename){
#ifndef PGL_CORE_WITHOUT_QT
    return QFileInfo(filename.c_str()).exists();
#else
#ifdef _WIN32
    return PathFileExists(toWinString(filename).c_str());
#else
    return ( access( filename.c_str(), F_OK ) != -1 );
#endif
#endif
}

bool similar_dir(const std::string& filename,const std::string& filename2){
        string f1 = short_dirname(absolute_filename(filename));
        string f2 = short_dirname(absolute_filename(filename2));
        if(f1.size() != f2.size()) return false;
        if(f1.size() == 0) return false;
        for(size_t i = 0 ; i < f1.size(); i++)
           if(f1[i] != f2[i] && f1[i] != '\\' && f2[i] != '\\' && f1[i] != '/' && f2[i] != '/' ) return false;
        return true;

}

#if defined(__GNUC__)
#ifndef __MINGW32__
#define LINKCOPY
#endif
#endif


bool copy(const std::string& src,const std::string& dest){
        if(!exists(src))return false;
#ifndef PGL_CORE_WITHOUT_QT
#if QT_VERSION >= QT_VERSION_CHECK(4,0,0)
        QFile f( src.c_str() );
        if(f.exists()) return f.copy(dest.c_str());
        else return false;
#else

#ifdef LINKCOPY
        return link(src.c_str(),dest.c_str());
#else
        QFile f( src.c_str() );
        if(f.exists()){
          QFile f2( dest.c_str() );
          if ( f.open( IO_ReadOnly ) &&  f2.open( IO_WriteOnly )) {
            f2.writeBlock(f.readAll());
            f.close();
            f2.close();
            return true;
          }
        }
        return false;
#endif

#endif

#else

#ifdef LINKCOPY
    return link(src.c_str(),dest.c_str());
#endif

#endif
}

TOOLS_END_NAMESPACE
