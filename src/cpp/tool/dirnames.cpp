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

#include <stdio.h>
#ifdef __GNUC__

#include <sys/param.h>
#include <unistd.h>

#else
#include <qfile.h>
#include <qtextstream.h>
#endif

#ifdef _WIN32

#include <direct.h>
#include <windows.h>
#define MAXPATHLEN _MAX_PATH
#define chgdir _chgdir

#endif

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef _WIN32
#include <unistd.h>
#endif
#include <iostream>

using namespace std;
TOOLS_BEGIN_NAMESPACE

string get_dirname(const string & filename) {

  if (filename.length() == 0) return string(".");
  else {
    size_t index = filename.find_last_of('/'); // finds the index of the last '/' in the string
    if (index == string::npos) {
#ifdef _WIN32
                index = filename.find_last_of('\\');
            if (index == string::npos) {
                        return string(".");
            }
                else {
                        if (index == 0) return "";
                        else return filename.substr(0,index);
                }
#else
                return string("."); // case of file = "fruit.txt" for instance
#endif
    }
    else { // there is at least a '/' somewhere in the string
      if (index == 0) return ""; // filename of the form "/root"
      else return filename.substr(0,index);
    }
  }

}


string get_notdirname(const string & filename) {

  uint32_t len = filename.length();

  if (len == 0) return string("");
  else {
    size_t index = filename.find_last_of('/'); // finds the index of the last '/' in the string
    if (index == string::npos) {
#ifdef _WIN32
                index = filename.find_last_of('\\');
            if (index == string::npos) {
              return filename;
            }
                else {
                        if (index == len) return "";
                        else return filename.substr(index+1,len);
                }
#else
      return filename; // case of file = "fruit.txt" for instance
#endif
    }
    else { // there is at least a '/' somewhere in the string
      if (index == len) return "";  // filename of the form "/usr/people/"
      else return filename.substr(index+1,len);
    }
  }
}


string expand_dirname(const string & filename) {

  char resolved_path[MAXPATHLEN];
#ifdef _WIN32
  char* path = _fullpath(resolved_path,filename.c_str(),MAXPATHLEN);
#else
  char* path = realpath(filename.c_str(),resolved_path);
#endif

  string result(resolved_path);

  if (path) {
    return result;
    // cerr << "Symbol pathname expanded: " << _symbol_path << endl;
  }
  else {
    perror("realpath");
        cerr << "pathname error at : " << result << endl;
    return "";
  }

}

#ifdef UNICODE
string short_dirnameU(const string& filename){
#ifdef _WIN32
        LPTSTR lpszShortPath = new WORD[MAXPATHLEN];
    DWORD cchBuffer(MAXPATHLEN);
        LPTSTR lpszLongPath  = new WORD[filename.length()+1];
        for(uint32_t i = 0; i < filename.length(); i++)
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

  char cwd_path[MAXPATHLEN];
  if (getcwd(cwd_path, MAXPATHLEN) == NULL) { // stores the current cwd
    cwd_path[0] = '\0'; // empty string denotes an error
    perror("getcwd");
  }

  return cwd_path;

}

bool chg_dir(const string & newdir) {

  if (chdir(newdir.c_str())!=0) { // the name of the current file becomes the cwd
        perror("chdir");
    return false;
  }
  else return true;

}


string append_suffix(const string & filename, const string & suffix) {

  string result = filename;
  int delta = filename.size() - suffix.size();

  if (delta > 0) {
    int index = filename.find(suffix); // finds the index of suffix if any
    if (index != (int)string::npos) {
      if (index == delta) return result;
    }
  }

  result.append(suffix);

  return result;
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


string get_extension(const string & filename){
  string extension("");
  uint32_t len = filename.length();

  if (len == 0){
    return extension;
  }
  else {
    size_t index = filename.find_last_of('.'); // finds the index of the last '.' in the string
    if (index == string::npos) {
      return extension; // case of file = "fruit" for instance
    }
    else { // there is at least a '.' somewhere in the string
      if (index == len){
        return extension;  // filename of the form "/usr/fruit."
      }
      else {
          extension = filename.substr(index+1,len);
#ifdef _WIN32
          index = min(filename.find_last_of('/'),filename.find_last_of('\\'));
#else
          index = filename.find_last_of('/');
#endif
          if (index == string::npos || index >= filename.size() /* because of a bug */ ) {
              return extension; // case of file = "fruit.txt" for instance
          }
          else {
            return string(""); // case of file "./fruit"
          }
      }
    }
  }
}

string change_extension(const string & filename,const string & extension){
  uint32_t len = filename.length();
  string ext(extension);
  if(ext[0]!='.')ext = string(".").append(ext);

  if (len == 0) return ext;
  else {
      size_t index = filename.find_last_of('.'); // finds the index of the last '.' in the string
      if (index == string::npos || index >= filename.size()) {
        return append_suffix(filename,ext); // case of file = "fruit" for instance
      }
      else { // there is at least a '.' somewhere in the string
        if (index == len){
          return  append_suffix(string(filename.begin(),filename.end()-1),ext);  // filename of the form "/usr/fruit."
        }
        else {
          string file = filename.substr(0,index);
#ifdef _WIN32
          size_t index2 = min(filename.find_last_of('/'),filename.find_last_of('\\'));
#else
          size_t index2 = filename.find_last_of('/');
#endif
          if (index2 == string::npos || index2 >= filename.size() || index > index2 ) {
            return append_suffix(file,ext); // case of file = "fruit.txt" for instance
          }
          else
            return append_suffix(filename,ext);
        }
      }
  }
}

bool exists(const string & filename){
  struct stat * buf = new struct stat();
  bool res = false;
  if(stat(filename.c_str(),buf)==0)res = true;
  delete buf;
  return res;
}

bool similar_dir(const std::string& filename,const std::string& filename2){
        string f1 = short_dirname(expand_dirname(filename));
        string f2 = short_dirname(expand_dirname(filename2));
        if(f1.size() != f2.size()) return false;
        bool r = true;
        for(size_t i = 0 ; i < f1.size()&&r ; i++)
                if(f1[i] != f2[i] && f1[i] != '\\' && f2[i] != '\\' && f1[i] != '/' && f2[i] != '/' ) r = false;
        return r;

}

bool copy(const std::string& src,const std::string& dest){
        if(!exists(src))return false;
#ifdef __GNUC__
        return link(src.c_str(),dest.c_str());
#else
        QFile f( src.c_str() );
        if(f.exists()){
                QFile f2( dest.c_str() );
                if ( f.open( IO_ReadOnly ) &&  f2.open( IO_WriteOnly )) {
                        QTextStream t1(&f);
                        QTextStream t2(&f2);
                        t2 << t1.read();
                        f.close();
                        f2.close();
                        return true;
                }
                else return false;
        }
        else return false;
#endif
}

TOOLS_END_NAMESPACE
