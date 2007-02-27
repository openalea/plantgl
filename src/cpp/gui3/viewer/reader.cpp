/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry 
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
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


#include "reader.h"

#ifdef QT_THREAD_SUPPORT

#include <qapplication.h>

#include "geomevent.h"
#include "geomscenegl.h"
#include <algo/codec/ligfile.h>

#include <sstream>

PGL_USING_NAMESPACE
using namespace std;
using namespace STDEXT;

/* ----------------------------------------------------------------------- */

ViewGeomReader3::ViewGeomReader3(const QString& f,
                               ViewGeomSceneGL3 * g,
                               int i,
                                   bool add):
  QThread(),
  _filename(f),
  _g(g),
  maxerror(i),
  addition(add)
{
}

ViewGeomReader3::~ViewGeomReader3()
{
}

void
ViewGeomReader3::setFilename(const QString& f)
{
  _filename = f;
}

const QString&
ViewGeomReader3::getFilename() const
{
  return _filename;
}

void
ViewGeomReader3::setMaxError(int i)
{
  maxerror = i;
}

void ViewGeomReader3::run()
{

    if(! _filename.isEmpty()) {
	  stringstream _errlog(ios::out) ;
      ScenePtr scene = ScenePtr(new Scene(_filename.latin1(),"", _errlog,
                                          maxerror));
      _errlog << std::ends;
      string _msg = _errlog.str();
      GeomSceneChangeEvent3 * e = new GeomSceneChangeEvent3(scene,_msg.c_str(),_filename,addition);
      QThread::postEvent(_g,e);
    }
}


/* ----------------------------------------------------------------------- */
/*
ViewGeomStreamReader::ViewGeomStreamReader(istream *stream,
                                           ViewGeomSceneGL3 * g,
                                           int i) :
  ViewGeomReader3("istream",g,i),
  __stream(stream)
{
}

ViewGeomStreamReader::~ViewGeomStreamReader()
{
}

void
ViewGeomStreamReader::setStream(istream * stream)
{
  __stream = stream;
}

void ViewGeomStreamReader::run()
{
  if(__stream != NULL && *__stream) {
    stringstream _errlog(ios::out) ;
    ScenePtr scene = ScenePtr(new Scene(*__stream,"GEOM",_errlog,
                                        maxerror));
    _errlog << ends;
    string _msg = _errlog.str();
    GeomSceneChangeEvent3 * e = new GeomSceneChangeEvent3(scene,_msg.c_str(),QString::null);
    QThread::postEvent(_g,e);
  }
}
*/


/* ----------------------------------------------------------------------- */

ViewMultiGeomReader3::ViewMultiGeomReader3(const QString& f1,
                                         const QString& f2,
                                         ViewGeomSceneGL3 * g,
                                         int i) :
  ViewGeomReader3(f1,g,i),
  __second(f2)
{
}

ViewMultiGeomReader3::~ViewMultiGeomReader3()
{
}

void
ViewMultiGeomReader3::setSecond(const QString& f)
{
  __second = f;
}

const QString&
ViewMultiGeomReader3::getSecond() const
{
  return __second;
}

void ViewMultiGeomReader3::run()
{
  if(! _filename.isEmpty() && ! __second.isEmpty()) {
    stringstream _errlog(ios::out) ;
    ScenePtr scene1 = ScenePtr(new Scene(_filename.latin1(),"",_errlog,
                                        maxerror));
    ScenePtr scene2 = ScenePtr(new Scene(__second.latin1(),"",_errlog,
                                        maxerror));
    _errlog << ends;
    string _msg = _errlog.str();
    GeomMultiSceneChangeEvent3 * e = new GeomMultiSceneChangeEvent3(scene1,
                                                                  scene2,
                                                                  _msg.c_str(),
                                                                  _filename,
                                                                  __second);
    QThread::postEvent(_g,e);
  }
}



/* ----------------------------------------------------------------------- */
ViewLinetreeReader3::ViewLinetreeReader3(const QString& ligFile,
                               const QString& dtaFile,
                               const QString& smbPath,
                               ViewGeomSceneGL3 * g,
							   bool bigendian,
                               bool add) :
  ViewGeomReader3(ligFile,g,-1,add) ,
  _dtaFile(dtaFile),
  _smbPath(smbPath),
  _bigendian(bigendian){
}

const QString&
ViewLinetreeReader3::getLigFile() const
{
  return _filename;
}

void
ViewLinetreeReader3::setLigFile(const QString& f)
{
  _filename = f;
}

const QString&
ViewLinetreeReader3::getDtaFile() const
{
  return _dtaFile;
}

void
ViewLinetreeReader3::setDtaFile(const QString& f)
{
  _dtaFile = f;
}

const QString&
ViewLinetreeReader3::getSmbPath() const
{
  return _smbPath;
}

void
ViewLinetreeReader3::setSmbPath(const QString& f)
{
  _smbPath = f;
}

ViewLinetreeReader3::~ViewLinetreeReader3()
{
}

void ViewLinetreeReader3::run(){

    if(! _filename.isEmpty()) {
      stringstream _errlog(ios::out) ;
      ScenePtr scene = readLineTree(string(_filename.data()),
                                    string(_dtaFile.data()),
                                    string(_smbPath.data()),
									_bigendian,
                                    _errlog);
      _errlog << ends;
      string _msg = _errlog.str();
      if(!scene) scene = ScenePtr(new Scene());
      GeomSceneChangeEvent3 * e = new GeomSceneChangeEvent3(scene,_msg.c_str(),_filename,addition);
      QThread::postEvent(_g,e);
    }
}
#endif
