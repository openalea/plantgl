/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
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

#include "scenegl.h"
#include "camera.h"
#include "light.h"
#include "filemanager.h"
#include "event.h"
#include <qfile.h>
#include <qfiledialog.h>
#include <qgl.h>
#include <qmessagebox.h>

/* ----------------------------------------------------------------------- */

ViewRendererGL3::ViewRendererGL3(QGLWidget * parent, 
			       const char * name):
  ViewObjectGL3(parent,name),
	__readTime(QDateTime::currentDateTime()) 
{
}

ViewRendererGL3::~ViewRendererGL3()
{
}


void 
ViewRendererGL3::connectTo(QGLWidget *g)
{
  ViewObjectGL3::connectTo(g);
  __frame = g;
}

void 
ViewRendererGL3::connectTo(ViewStatusBar3 * s)
{
  ViewObjectGL3::connectTo(s);
}

void 
ViewRendererGL3::connectTo(ViewErrorDialog3 *e)
{
  ViewObjectGL3::connectTo(e);
}


void 
ViewRendererGL3::changeModeEvent(ViewGLFrame3::Mode m)
{
}

void 
ViewRendererGL3::setRotationMode()
{
  emit changeMode(ViewGLFrame3::Rotation);
}

void
ViewRendererGL3::setSelectionMode()
{
  emit changeMode(ViewGLFrame3::Selection);
}

void
ViewRendererGL3::selectGL()
{
}

void
ViewRendererGL3::selectionEvent(uint32_t i)
{
  qDebug("selectionEvent(uint32_t) not implemented in current Renderer");
}

void
ViewRendererGL3::selectionEvent(const std::vector<uint32_t>& i)
{
  for(std::vector<uint32_t>::const_iterator _it = i.begin();
	  _it != i.end(); _it++)
	  selectionEvent(*_it);
}

void
ViewRendererGL3::selectionIdEvent(const std::vector<uint32_t>& i)
{
  for(std::vector<uint32_t>::const_iterator _it = i.begin();
	  _it != i.end(); _it++)
	  selectionEvent(*_it);
}

void
ViewRendererGL3::selectionEvent(QListViewItem *)
{
  qDebug("selectionEvent(QListViewItem *) not implemented in current Renderer");
}

std::vector<uint32_t> 
ViewRendererGL3::getSelectionIds() const
{
  qDebug("std::vector<uint32_t> getSelectionIds() const not implemented in current Renderer");
  return std::vector<uint32_t>(0);
}

uint32_t 
ViewRendererGL3::translateId(uint32_t id) const
{
	return id;
}

bool
ViewRendererGL3::endSelect()
{
	return false;
}

void 
ViewRendererGL3::endEvent()
{
}

void
ViewRendererGL3::clearSelectionEvent()
{
}

bool
ViewRendererGL3::addOpenEntries(QPopupMenu * menu)
{
  return false;
}

bool 
ViewRendererGL3::addImportEntries(QPopupMenu * menu)
{
  return false;
}

bool
ViewRendererGL3::addSaveEntries(QPopupMenu * menu)
{
  return false;
}

bool
ViewRendererGL3::addExportEntries(QPopupMenu * menu)
{
  return false;
}
  
bool
ViewRendererGL3::addProperties(QTabWidget * tab)
{
  return false;
}

bool 
ViewRendererGL3::addOtherMenu(QMenuBar * menu)
{
  return false;
}

bool 
ViewRendererGL3::addEditEntries(QPopupMenu * menu)
{
	return false;
}

bool 
ViewRendererGL3::addOtherToolBar(QMainWindow * menu)
{
  return false;
}

bool ViewRendererGL3::browse(QListView *,bool)
{
  return false;
}

bool 
ViewRendererGL3::open(const QString& filename)
{
  return false;
}

bool 
ViewRendererGL3::openStream(std::istream& stream)
{
  return false;
}

bool
ViewRendererGL3::saveAs(const QString& filename)
{
  return false;
}

void
ViewRendererGL3::openFile(const QString& filename)
{
  if(open(filename))
    emit filenameChanged(__filename);
}

void
ViewRendererGL3::addFile(const QString& filename)
{
}

void 
ViewRendererGL3::save()
{
  saveAs(__filename);
  __readTime = QDateTime::currentDateTime();
}

void 
ViewRendererGL3::refresh()
{
  if(__filename){
	openFile(__filename);
  }
  else refreshDisplay();
}

void 
ViewRendererGL3::refreshDisplay()
{
}

void 
ViewRendererGL3::close()
{
  clear();
  emit valueChanged();
}

void 
ViewRendererGL3::saveAs()
{
  QString filename = ViewFileManager3::getSaveFileName(__filename,"","All Files (*.*)",__frame,"SaveAs");
  if(!filename.isEmpty()){
    if(saveAs(filename)){
      __filename = filename;
	  __readTime = QDateTime::currentDateTime();
      emit filenameChanged(__filename);
    }
  }
}

const QString&
ViewRendererGL3::getFilename() const
{
  return __filename;
}

void
ViewRendererGL3::setFilename(const QString& file)
{
  bool t = (file != __filename);
  if(t) __filename = file;
  __readTime = QDateTime::currentDateTime();
  if(t) emit filenameChanged(__filename);
}

void
ViewRendererGL3::checkFileModification()
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
ViewRendererGL3::clear()
{
  if(__filename != QString::null){
	__filename = QString::null;
	emit filenameChanged(__filename);
  }
}

/// Get the global Bounding Box.
const PGL(BoundingBoxPtr) 
ViewRendererGL3::getGlobalBoundingBox() const
{
	return new PGL(BoundingBox)(Vector3(-1,-1,-1),Vector3(1,1,1));
}


/* ----------------------------------------------------------------------- */

ViewSceneRendererGL3::ViewSceneRendererGL3(ViewCameraGL3 * camera,
					 ViewLightGL3 * light,
					 QGLWidget * parent, 
					 const char * name):
  ViewRendererGL3(parent,name),
  __camera(camera),
  __light(light),
  __lightEnable(true)
{
  if(camera)
	QObject::connect(camera,SIGNAL(needReDim()),this,SLOT(computeCamera()));  
}

ViewSceneRendererGL3::~ViewSceneRendererGL3()
{
}

ViewCameraGL3 *
ViewSceneRendererGL3::getCamera() const
{
  return __camera;
}

ViewLightGL3 *
ViewSceneRendererGL3::getLight() const
{
  return __light;
}

void
ViewSceneRendererGL3::setCamera(ViewCameraGL3 * camera)
{
  __camera = camera;
  if(camera)
	QObject::connect(camera,SIGNAL(needReDim()),this,SLOT(computeCamera()));  
}

void 
ViewSceneRendererGL3::setLight(ViewLightGL3 * light)
{
  __light = light;
}


bool 
ViewSceneRendererGL3::isLightEnable() const
{
  return __lightEnable;
}

void 
ViewSceneRendererGL3::setLightEnable()
{
  __lightEnable=!__lightEnable;
  emit lightEnableChanged(__lightEnable);
  emit valueChanged();
  if(__lightEnable)status("Light Enable",5000);
  else status("Light Disable",5000);
}

void  
ViewSceneRendererGL3::computeCamera()
{
}

void 
ViewSceneRendererGL3::useDisplayList(bool)
{
}


/* ----------------------------------------------------------------------- */

bool ViewRendererGL3::event(QEvent * e)
{
  if(e->type() == 12345){
    ViewSceneChangeEvent3 * k = ( ViewSceneChangeEvent3 * )e;
    return sceneChangeEvent(k);
  }
  else if(e->type() == 12348){
    ViewFileChangeEvent3 * k = ( ViewFileChangeEvent3 * )e;
	openFile(k->filename);
    return true;
  }
  else if(e->type() == 12349){
    // ViewRefreshEvent3 * k = ( ViewRefreshEvent3 * )e;
	refresh();
    return true;
  }
  else return(QObject::event(e));
}

bool ViewRendererGL3::sceneChangeEvent( ViewSceneChangeEvent3 * )
{
  return false;
}


/* ----------------------------------------------------------------------- */

