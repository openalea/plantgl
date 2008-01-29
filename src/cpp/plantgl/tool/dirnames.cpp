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


#include "dirnames.h"
#include "util_types.h"

#include <qfile.h>
#include <qdir.h>
#include <qfileinfo.h>

#ifdef _WIN32
#include <windows.h>
#define MAXPATHLEN _MAX_PATH
#endif

using namespace std;
TOOLS_BEGIN_NAMESPACE

inline std::string QString2StdString(const QString& st)
#if QT_VERSION >= 0x040000
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

string get_dirname(const string & filename) {
#if QT_VERSION >= 0x040000
	return QString2StdString(QFileInfo(filename.c_str()).path()); 
#else
	return QString2StdString(QFileInfo(filename.c_str()).dirPath());
#endif
}


string get_filename(const string & filename) {
	return QString2StdString(QFileInfo(filename.c_str()).fileName());
}

string absolute_dirname(const string & filename) {
#if QT_VERSION >= 0x040000
	return QString2StdString(QFileInfo(filename.c_str()).absolutePath()); 
#else
	return QString2StdString(QFileInfo(filename.c_str()).dirPath(true));
#endif
}

string absolute_filename(const string & filename) {
#if QT_VERSION >= 0x040000
	return QString2StdString(QFileInfo(filename.c_str()).absoluteFilePath()); 
#else
	return QString2StdString(QFileInfo(filename.c_str()).absFilePath());
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
#if QT_VERSION >= 0x040000
	return QString2StdString(QDir::currentPath());
#else
	return QString2StdString(QDir::current().path());
#endif
}

bool chg_dir(const string & newdir) {
	if(newdir == get_cwd()) return true;
    return QDir::setCurrent(newdir.c_str());
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
#ifdef PGL_DEBUG
	size_t pos = filename.find_last_of('.');
	if (pos == std::string::npos) return string("");
	else return std::string(filename.begin()+filename.find_last_of('.')+1,filename.end());
#else
	QFileInfo fi(filename.c_str());
#if QT_VERSION >= 0x040000
	return QString2StdString(fi.suffix()); 
#else
	return QString2StdString(fi.extension()); 
#endif
#endif
}

string set_suffix(const string & filename,const string & extension){
	QFileInfo fi(filename.c_str());
#if QT_VERSION >= 0x040000
	QString nname = fi.path()+'/'+fi.baseName()+'.'+QString(extension.c_str());
#else
	QString nname = fi.dirPath()+'/'+fi.baseName()+'.'+QString(extension.c_str());
#endif
	return QString2StdString(nname);
}

bool exists(const string & filename){
	return QFileInfo(filename.c_str()).exists();
}

bool similar_dir(const std::string& filename,const std::string& filename2){
        string f1 = short_dirname(absolute_filename(filename));
        string f2 = short_dirname(absolute_filename(filename2));
        if(f1.size() != f2.size()) return false;
        bool r = true;
        for(size_t i = 0 ; i < f1.size()&&r ; i++)
                if(f1[i] != f2[i] && f1[i] != '\\' && f2[i] != '\\' && f1[i] != '/' && f2[i] != '/' ) r = false;
        return r;

}

#if defined(__GNUC__) 
#ifndef __MINGW32__
#define LINKCOPY
#endif
#endif


bool copy(const std::string& src,const std::string& dest){
        if(!exists(src))return false;
#if QT_VERSION >= 0x040000
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
}

TOOLS_END_NAMESPACE
