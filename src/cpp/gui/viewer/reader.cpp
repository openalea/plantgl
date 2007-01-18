

#ifdef QT_THREAD_SUPPORT

#include <qapplication.h>

#include "view_reader.h"
#include "view_geomevent.h"
#include "view_geomscenegl.h"
#include "ligfile.h"

#include <sstream>

GEOM_USING_NAMESPACE
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
      ScenePtr scene = ScenePtr(new Scene(_filename.latin1(),_errlog,
                                          maxerror));
      _errlog << std::ends;
      string _msg = _errlog.str();
      GeomSceneChangeEvent * e = new GeomSceneChangeEvent(scene,_msg.c_str(),_filename,addition);
      QThread::postEvent(_g,e);
    }
}


/* ----------------------------------------------------------------------- */

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
    ScenePtr scene = ScenePtr(new Scene(*__stream,_errlog,
                                        maxerror));
    _errlog << ends;
    string _msg = _errlog.str();
    GeomSceneChangeEvent * e = new GeomSceneChangeEvent(scene,_msg.c_str(),QString::null);
    QThread::postEvent(_g,e);
  }
}



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
    ScenePtr scene1 = ScenePtr(new Scene(_filename.latin1(),_errlog,
                                        maxerror));
    ScenePtr scene2 = ScenePtr(new Scene(__second.latin1(),_errlog,
                                        maxerror));
    _errlog << ends;
    string _msg = _errlog.str();
    GeomMultiSceneChangeEvent * e = new GeomMultiSceneChangeEvent(scene1,
                                                                  scene2,
                                                                  _msg.c_str(),
                                                                  _filename,
                                                                  __second);
    QThread::postEvent(_g,e);
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
      ScenePtr scene = readLineTree(string(_filename.data()),
                                    string(_dtaFile.data()),
                                    string(_smbPath.data()),
									_bigendian,
                                    _errlog);
      _errlog << ends;
      string _msg = _errlog.str();
      if(!scene) scene = ScenePtr(new Scene());
      GeomSceneChangeEvent * e = new GeomSceneChangeEvent(scene,_msg.c_str(),_filename,addition);
      QThread::postEvent(_g,e);
    }
}
#endif
