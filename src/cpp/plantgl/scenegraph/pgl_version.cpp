/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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



#include <plantgl/tool/util_string.h>
#include "pgl_version.h"

using namespace std;

TOOLS_USING_NAMESPACE;


float getPGLVersion(){
  float major = ((PGL_VERSION & 0xff0000) >> 16);
  float minor = ((PGL_VERSION & 0x00ff00) >> 8);
  return major+minor*0.01;
}

int getPGLRevision(){
  return float(PGL_VERSION & 0x0000ff);
}

int getPGLSvnRevision(){
  std::string svnrev = PGL_SVNREVISION;
  if (svnrev.size() > 13){
	  svnrev = svnrev.substr(11,svnrev.size()-13);
	  return atoi(svnrev.c_str());
  }
  return 0;
}

std::string getPGLVersionString(){ 
  std::string version = number((PGL_VERSION & 0xff0000) >> 16)+'.'+number((PGL_VERSION & 0x00ff00) >> 8)+'.'+number(PGL_VERSION & 0x0000ff);
  std::string svnrev = PGL_SVNREVISION;
  if (svnrev.size() > 13)
	  version += "-r"+svnrev.substr(11,svnrev.size()-13);
  return version;
}


