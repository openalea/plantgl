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



#include "reader.h"

#ifdef QT_THREAD_SUPPORT

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qapplication.h>
#else
    #include <QtGui/qapplication.h>
#endif

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
      ScenePtr scene = ScenePtr(new Scene(qPrintable(_filename),"", _errlog,
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
    GeomSceneChangeEvent * e = new GeomSceneChangeEvent(scene,_msg.c_str(),QString());
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
    ScenePtr scene1 = ScenePtr(new Scene(qPrintable(_filename),"",_errlog,
                                        maxerror));
    ScenePtr scene2 = ScenePtr(new Scene(qPrintable(__second),"",_errlog,
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
      ScenePtr scene = readLineTree(_filename.toStdString(),
                                    _dtaFile.toStdString(),
                                    _smbPath.toStdString(),
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
