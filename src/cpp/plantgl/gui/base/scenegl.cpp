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


#include "scenegl.h"
#include "camera.h"
#include "light.h"
#include "filemanager.h"
#include "event.h"
#include <QtCore/qfile.h>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qfiledialog.h>
    #include <QtWidgets/qmessagebox.h>
#else
    #include <QtGui/qfiledialog.h>
    #include <QtGui/qmessagebox.h>
#endif
#include <QtOpenGL/qgl.h>

/* ----------------------------------------------------------------------- */

ViewRendererGL::ViewRendererGL(QGLWidget * parent,
                   const char * name):
  ViewObjectGL(parent,name),
    __readTime(QDateTime::currentDateTime())
{
}

ViewRendererGL::~ViewRendererGL()
{
}

bool ViewRendererGL::isEmpty() const { return true; }

void
ViewRendererGL::connectTo(QGLWidget *g)
{
  ViewObjectGL::connectTo(g);
  __frame = g;
}

void
ViewRendererGL::connectTo(ViewStatusBar * s)
{
  ViewObjectGL::connectTo(s);
}

void
ViewRendererGL::connectTo(ViewErrorDialog *e)
{
  ViewObjectGL::connectTo(e);
}


void
ViewRendererGL::changeModeEvent(ViewGLFrame::Mode m)
{
}

void
ViewRendererGL::setRotationMode()
{
  emit changeMode(ViewGLFrame::Rotation);
}

void
ViewRendererGL::setSelectionMode()
{
  emit changeMode(ViewGLFrame::Selection);
}

void
ViewRendererGL::selectGL()
{
}

void
ViewRendererGL::selectionEvent(uint_t i)
{
  qDebug("selectionEvent(uint_t) not implemented in current Renderer");
}

void
ViewRendererGL::selectionEvent(const std::vector<uint_t>& i)
{
  for(std::vector<uint_t>::const_iterator _it = i.begin();
      _it != i.end(); _it++)
      selectionEvent(*_it);
}

void
ViewRendererGL::selectionIdEvent(const std::vector<uint_t>& i)
{
  for(std::vector<uint_t>::const_iterator _it = i.begin();
      _it != i.end(); _it++)
      selectionEvent(*_it);
}

void
ViewRendererGL::selectionEvent(QTreeWidgetItem *)
{
  qDebug("selectionEvent(QTreeWidgetItem *) not implemented in current Renderer");
}

std::vector<uint_t>
ViewRendererGL::getSelectionIds() const
{
  qDebug("std::vector<uint_t> getSelectionIds() const not implemented in current Renderer");
  return std::vector<uint_t>(0);
}

uint_t
ViewRendererGL::translateId(uint_t id) const
{
    return id;
}

bool
ViewRendererGL::endSelect()
{
    return false;
}

void
ViewRendererGL::endEvent()
{
}

void
ViewRendererGL::clearSelectionEvent()
{
}

bool
ViewRendererGL::addOpenEntries(QMenu * menu)
{
  return false;
}

bool
ViewRendererGL::addImportEntries(QMenu * menu)
{
  return false;
}

bool
ViewRendererGL::addSaveEntries(QMenu * menu)
{
  return false;
}

bool
ViewRendererGL::addExportEntries(QMenu * menu)
{
  return false;
}

bool
ViewRendererGL::addProperties(QTabWidget * tab)
{
  return false;
}

bool
ViewRendererGL::addOtherMenu(QMenuBar * menu)
{
  return false;
}

bool
ViewRendererGL::addEditEntries(QMenu * menu)
{
    return false;
}

bool
ViewRendererGL::addOtherToolBar(QMainWindow * menu)
{
  return false;
}

bool ViewRendererGL::browse(QTreeWidget *,bool)
{
  return false;
}

bool
ViewRendererGL::open(const QString& filename)
{
  return false;
}

bool
ViewRendererGL::openStream(std::istream& stream)
{
  return false;
}

bool
ViewRendererGL::saveAs(const QString& filename)
{
  return false;
}

void
ViewRendererGL::openFile(const QString& filename)
{
  if(open(filename))
    emit filenameChanged(__filename);
}

void
ViewRendererGL::addFile(const QString& filename)
{
}

void
ViewRendererGL::save()
{
  saveAs(__filename);
  __readTime = QDateTime::currentDateTime();
}

void
ViewRendererGL::refresh()
{
  if(!__filename.isEmpty()){
    openFile(__filename);
  }
  else refreshDisplay();
}

void
ViewRendererGL::refreshDisplay()
{
}

void
ViewRendererGL::close()
{
  clear();
  emit valueChanged();
}

void
ViewRendererGL::saveAs()
{
  QString filename = ViewFileManager::getSaveFileName(__filename,"","All Files (*.*)",__frame,"SaveAs");
  if(!filename.isEmpty()){
    if(saveAs(filename)){
      __filename = filename;
      __readTime = QDateTime::currentDateTime();
      emit filenameChanged(__filename);
    }
  }
}

const QString&
ViewRendererGL::getFilename() const
{
  return __filename;
}

void
ViewRendererGL::setFilename(const QString& file)
{
  bool t = (file != __filename);
  if(t) __filename = file;
  __readTime = QDateTime::currentDateTime();
  if(t) emit filenameChanged(__filename);
}

void
ViewRendererGL::checkFileModification()
{
  if(!__filename.isEmpty()){
    if(QFileInfo(__filename).lastModified() > __readTime){
      __readTime = QDateTime::currentDateTime();
      if(QFileInfo(__filename).lastModified() > __readTime)
        __readTime = QFileInfo(__filename).lastModified();
      if(QMessageBox:: information (__frame, "File Modification",
        __filename + " has been modified.\nLast Modification : "
        +QFileInfo(__filename).lastModified().toString()
        +" - Reading Time : "
        +__readTime.toString()
        +"\nReload ?","Yes","No") == 0){
        refresh();
      }
    }
  }
}

void
ViewRendererGL::clear()
{
  if(__filename != QString()){
    __filename = QString();
    emit filenameChanged(__filename);
  }
}

/// Get the global Bounding Box.
const PGL(BoundingBoxPtr)
ViewRendererGL::getGlobalBoundingBox() const
{
    return PGL(BoundingBoxPtr)(new PGL(BoundingBox)(Vector3(-1,-1,-1),Vector3(1,1,1)));
}


/* ----------------------------------------------------------------------- */

ViewSceneRendererGL::ViewSceneRendererGL(ViewCameraGL * camera,
                     ViewLightGL * light,
                     QGLWidget * parent,
                     const char * name):
  ViewRendererGL(parent,name),
  __camera(camera),
  __light(light),
  __animated(eStatic)
{
  if(camera)
    QObject::connect(camera,SIGNAL(needReDim()),this,SLOT(computeCamera()));
}

ViewSceneRendererGL::~ViewSceneRendererGL()
{
}

ViewCameraGL *
ViewSceneRendererGL::getCamera() const
{
  return __camera;
}

ViewLightGL *
ViewSceneRendererGL::getLight() const
{
  return __light;
}

void
ViewSceneRendererGL::setCamera(ViewCameraGL * camera)
{
  __camera = camera;
  if(camera)
    QObject::connect(camera,SIGNAL(needReDim()),this,SLOT(computeCamera()));
}

void
ViewSceneRendererGL::setLight(ViewLightGL * light)
{
  __light = light;
}

void
ViewSceneRendererGL::computeCamera()
{
}

void
ViewSceneRendererGL::useDisplayList(bool)
{
}


/* ----------------------------------------------------------------------- */

bool ViewRendererGL::event(QEvent * e)
{
  int etype = e->type();
  if(etype == ViewEvent::eSceneChange){
    ViewSceneChangeEvent * k = ( ViewSceneChangeEvent * )e;
    return sceneChangeEvent(k);
  }
  else if(etype == ViewEvent::eFileChange){
    ViewFileChangeEvent * k = ( ViewFileChangeEvent * )e;
    openFile(k->arg1);
    return true;
  }
  else if(etype == ViewEvent::eRefresh){
    // ViewRefreshEvent * k = ( ViewRefreshEvent * )e;
    refresh();
    return true;
  }
  else return(QObject::event(e));
}

bool ViewRendererGL::sceneChangeEvent( ViewSceneChangeEvent * )
{
  return false;
}


/* ----------------------------------------------------------------------- */

