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


#include "reader.h"

#ifdef QT_THREAD_SUPPORT

#include <qapplication.h>

#include "geomevent.h"
#include "geomscenegl.h"
#include <plantgl/algo/codec/ligfile.h>

#include <sstream>

PGL_USING_NAMESPACE
using namespace std;
using namespace STDEXT;

/* ----------------------------------------------------------------------- */

ViewGeomReader::ViewGeomReader(const QString& f,
                               ViewGeomSceneGL * g,
                               int i,
                                   bool add):
  QThread(),
  _filename(f),
  _g(g),
  maxerror(i),
  addition(add)
{
}

ViewGeomReader::~ViewGeomReader()
{
}

void
ViewGeomReader::setFilename(const QString& f)
{
  _filename = f;
}

const QString&
ViewGeomReader::getFilename() const
{
  return _filename;
}

void
ViewGeomReader::setMaxError(int i)
{
  maxerror = i;
}

void ViewGeomReader::run()
{

    if(! _filename.isEmpty()) {
	  stringstream _errlog(ios::out) ;
      ScenePtr scene = ScenePtr(new Scene(_filename.toAscii().constData(),"", _errlog,
                                          maxerror));
      _errlog << std::ends;
      string _msg = _errlog.str();
      GeomSceneChangeEvent * e = new GeomSceneChangeEvent(scene,_msg.c_str(),_filename,addition);
      QApplication::postEvent(_g,e);
    }
}


/* ----------------------------------------------------------------------- */
/*
ViewGeomStreamReader::ViewGeomStreamReader(istream *stream,
                                           ViewGeomSceneGL * g,
                                           int i) :
  ViewGeomReader("istream",g,i),
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
    GeomSceneChangeEvent * e = new GeomSceneChangeEvent(scene,_msg.c_str(),QString::null);
    QThread::postEvent(_g,e);
  }
}
*/


/* ----------------------------------------------------------------------- */

ViewMultiGeomReader::ViewMultiGeomReader(const QString& f1,
                                         const QString& f2,
                                         ViewGeomSceneGL * g,
                                         int i) :
  ViewGeomReader(f1,g,i),
  __second(f2)
{
}

ViewMultiGeomReader::~ViewMultiGeomReader()
{
}

void
ViewMultiGeomReader::setSecond(const QString& f)
{
  __second = f;
}

const QString&
ViewMultiGeomReader::getSecond() const
{
  return __second;
}

void ViewMultiGeomReader::run()
{
  if(! _filename.isEmpty() && ! __second.isEmpty()) {
    stringstream _errlog(ios::out) ;
    ScenePtr scene1 = ScenePtr(new Scene(_filename.toAscii().constData(),"",_errlog,
                                        maxerror));
    ScenePtr scene2 = ScenePtr(new Scene(__second.toAscii().constData(),"",_errlog,
                                        maxerror));
    _errlog << ends;
    string _msg = _errlog.str();
    GeomMultiSceneChangeEvent * e = new GeomMultiSceneChangeEvent(scene1,
                                                                  scene2,
                                                                  _msg.c_str(),
                                                                  _filename,
                                                                  __second);
    QApplication::postEvent(_g,e);
  }
}



/* ----------------------------------------------------------------------- */
ViewLinetreeReader::ViewLinetreeReader(const QString& ligFile,
                               const QString& dtaFile,
                               const QString& smbPath,
                               ViewGeomSceneGL * g,
							   bool bigendian,
                               bool add) :
  ViewGeomReader(ligFile,g,-1,add) ,
  _dtaFile(dtaFile),
  _smbPath(smbPath),
  _bigendian(bigendian){
}

const QString&
ViewLinetreeReader::getLigFile() const
{
  return _filename;
}

void
ViewLinetreeReader::setLigFile(const QString& f)
{
  _filename = f;
}

const QString&
ViewLinetreeReader::getDtaFile() const
{
  return _dtaFile;
}

void
ViewLinetreeReader::setDtaFile(const QString& f)
{
  _dtaFile = f;
}

const QString&
ViewLinetreeReader::getSmbPath() const
{
  return _smbPath;
}

void
ViewLinetreeReader::setSmbPath(const QString& f)
{
  _smbPath = f;
}

ViewLinetreeReader::~ViewLinetreeReader()
{
}

void ViewLinetreeReader::run(){

    if(! _filename.isEmpty()) {
      stringstream _errlog(ios::out) ;
      ScenePtr scene = readLineTree(string(_filename.toAscii().data()),
                                    string(_dtaFile.toAscii().data()),
                                    string(_smbPath.toAscii().data()),
									_bigendian,
                                    _errlog);
      _errlog << ends;
      string _msg = _errlog.str();
      if(!scene) scene = ScenePtr(new Scene());
      GeomSceneChangeEvent * e = new GeomSceneChangeEvent(scene,_msg.c_str(),_filename,addition);
      QApplication::postEvent(_g,e);
    }
}
#endif
