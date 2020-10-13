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




#include <plantgl/tool/util_string.h>
#include "pgl_version.h"
#include <cstdlib>

using namespace std;

PGL_USING_NAMESPACE;


float getPGLVersion(){
  float major = ((PGL_VERSION & 0xff0000) >> 16);
  float minor = ((PGL_VERSION & 0x00ff00) >> 8);
  return major+minor*0.01;
}

int getPGLRevision(){
  return float(PGL_VERSION & 0x0000ff);
}

/*
int getPGLSvnRevision(){
  std::string svnrev = PGL_SVNREVISION;
  if (svnrev.size() > 13){
      svnrev = svnrev.substr(11,svnrev.size()-13);
      return atoi(svnrev.c_str());
  }
  return 0;
}
*/

std::string getPGLVersionString(){
  std::string version = number((PGL_VERSION & 0xff0000) >> 16)+'.'+number((PGL_VERSION & 0x00ff00) >> 8)+'.'+number(PGL_VERSION & 0x0000ff);
  /* std::string svnrev = PGL_SVNREVISION;
  if (svnrev.size() > 13)
      version += "-r"+svnrev.substr(11,svnrev.size()-13);
  */
  return version;
}


int getPGLVersionNumber() { return PGL_VERSION; }
// std::string getPGLRevisionString() { return PGL_SVNREVISION; }

#define ADD_EXTENSION(EXT) res.push_back(std::string(#EXT));

std::vector<std::string> __getPGLSupportedExtensions() {
    std::vector<std::string> res;
#ifdef PGL_USE_DOUBLE
    ADD_EXTENSION(DOUBLE)
#endif
#ifdef USE_GLUT
    ADD_EXTENSION(GLUT)
#endif
#ifdef QT_THREAD_SUPPORT
    ADD_EXTENSION(THREAD)
#endif
#ifndef NO_NAMESPACE
    ADD_EXTENSION(NAMESPACE)
#endif
#ifdef USING_UNORDERED_MAP
    ADD_EXTENSION(USING_UNORDERED_MAP)
#endif
#ifdef PGL_WITH_CGAL
    ADD_EXTENSION(CGAL)
#endif
#ifdef MAINTAIN_PYTHON_OBJECT_ID
    ADD_EXTENSION(MAINTAIN_PYTHON_OBJECT_ID)
#endif
#ifdef PGL_WITH_ANN
    ADD_EXTENSION(ANN)
#endif
#ifdef PGL_WITH_LAPACK
    ADD_EXTENSION(LAPACK)
#endif
#ifdef PGL_WITH_EIGEN
    ADD_EXTENSION(EIGEN)
#endif
#ifdef PGL_WITH_BISONFLEX
    ADD_EXTENSION(PGL_ASCII_PARSER)
#endif
#ifdef PGL_WITH_BOOST
    ADD_EXTENSION(BOOST)
#endif
#ifdef PGL_WITHOUT_QT
    ADD_EXTENSION(PGL_NO_QT_GUI)
#endif
   return res;
}

std::vector<std::string> PGL_EXTENSIONS = __getPGLSupportedExtensions();

const std::vector<std::string>& get_pgl_supported_extensions() { return PGL_EXTENSIONS; }

bool pgl_support_extension(const std::string& ext) {
    for(std::vector<std::string>::const_iterator it = PGL_EXTENSIONS.begin();
        it != PGL_EXTENSIONS.end(); ++it)
        if (*it == ext) return true;
    return false;
}

#ifndef PGL_CORE_WITHOUT_QT
#include <QtGlobal>
#endif

int getPGLQtVersion(){
#ifndef PGL_CORE_WITHOUT_QT
  return  QT_VERSION;
#else
  return 0;
#endif
}

std::string getPGLQtVersionString(){
#ifndef PGL_CORE_WITHOUT_QT
  return std::string(QT_VERSION_STR);
#else
  return std::string();
#endif
}
