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

#include "object.h"

#include <qpoint.h>
#include <qmessagebox.h>
#include <qgl.h>

#include "camera.h"
#include "errordialog.h"
#include "util_qwidget.h"
#include <stdlib.h>

/* ----------------------------------------------------------------------- */

ViewObjectGL::ViewObjectGL(QObject * parent, const char * name) :
  QObject(parent),
  __frame(NULL)
{
	if(name) setObjectName(name);
}

ViewObjectGL::ViewObjectGL(QGLWidget * parent, const char * name) :
  QObject(parent),
  __frame(parent)
{
  if(parent)
    QObject::connect (this,SIGNAL(valueChanged()),
		      parent,SLOT(updateGL()) ); 	
  if(name) setObjectName(name);
}

ViewObjectGL::~ViewObjectGL()
{
}

void 
ViewObjectGL::move(QPoint p)
{
  moving(p.x(),p.y());
}

void 
ViewObjectGL::moving(int dx, int dy)
{
}

void 
ViewObjectGL::zoom(QPoint p)
{
  zooming(p.x(),p.y());
}

void 
ViewObjectGL::zooming(int dx, int dy)
{
}

void 
ViewObjectGL::rotate(QPoint p)
{
  rotating(p.x(),p.y());
}

void 
ViewObjectGL::rotating(int dx, int dy)
{
}

void 
ViewObjectGL::initializeGL()
{
}

void 
ViewObjectGL::resizeGL(int w, int h)
{
}

QMenu * 
ViewObjectGL::createToolsMenu(QWidget * parent)
{
  return NULL;
}

void ViewObjectGL::fillToolBar(QToolBar * toolBar)
{
}

void 
ViewObjectGL::connectTo(ViewStatusBar * s)
{
  if(s){
    QObject::connect(this,SIGNAL(statusMessage(const QString&,int)),s,SLOT(showMessage(const QString&,int)) );  
    QObject::connect(this,SIGNAL(statusMessage(const QString&)), s,SLOT(showMessage(const QString&)) );  
	QObject::connect(this,SIGNAL(progressMessage(int,int)), s,SLOT(setProgress(int,int)) );  
  }
}

void 
ViewObjectGL::connectTo(QGLWidget *g)
{
  if(g){
    QObject::connect (this,SIGNAL(valueChanged()),
		      g,SLOT(updateGL()) );
    __frame = g;
  }
}

void 
ViewObjectGL::connectTo(ViewErrorDialog *e)
{
  if(e){
    QObject::connect(this,SIGNAL(errorMessage(const QString&)),
		     e,SLOT(setError(const QString&)) );
    QObject::connect(this,SIGNAL(warningMessage(const QString&)),
		     e,SLOT(appendWarning(const QString&)) );
    QObject::connect(this,SIGNAL(infoMessage(const QString&)),
		     e,SLOT(appendInfo(const QString&)) );
  }
}


void 
ViewObjectGL::error(const QString& s)
{
  emit errorMessage(s);
}

void 
ViewObjectGL::warning(const QString& s)
{
  emit warningMessage(s);
}

void 
ViewObjectGL::info(const QString& s)
{
  emit infoMessage(s);
}

void 
ViewObjectGL::status(const QString& s)
{
  emit statusMessage(s);
}

void 
ViewObjectGL::status(const QString& s,int t)
{
  emit statusMessage(s,t);
}

void 
ViewObjectGL::progress(int p,int t)
{
  emit progressMessage(p,t);
}

bool
ViewObjectGL::glError(const char * file, int line) const
{
  return glError(__frame, file, line);
}

static bool lock = false;
bool ViewObjectGL::BASHMODE = false;

bool
ViewObjectGL::glError(QWidget * widget, const char * file, int line)
{
  GLenum _glerror;
  
  if((_glerror = glGetError()) != GL_NO_ERROR){
	  QString _mess = "<b>[ObjectGL] GL Error ["+QString::number(_glerror)+"] !!</b><br>";
	  int i = 0;
	  while(_glerror != GL_NO_ERROR && i < 10){
		  _mess +=(char *)gluErrorString(_glerror);
		  _mess += "<br>\n";
		  _glerror = glGetError();
		  i++;
	  }
	  if(file != NULL){
		  _mess += "<br><b>File :</b>";
		  _mess += file;
		  _mess += "<br><b>Line :</b>";
		  _mess += QString::number(line);
	  }
	  if(!BASHMODE){
		  if(!lock){
			  lock = true;
			  int res = QMessageBox::critical(widget,tr("GL Error"),_mess,tr("Abort"),tr("Continue"));
			  if(res == 0 || res == -1){
				  abort();
			  }
			  lock = false;
		  }
	  }
	  else qWarning(_mess.toAscii());
	  return true;
  }
  return false;
}

/* ----------------------------------------------------------------------- */

ViewRelativeObjectGL::ViewRelativeObjectGL(ViewCameraGL *camera, QObject * parent, const char * name):
  ViewObjectGL(parent,name),
  __step(1){
  if(camera){
    QObject::connect(camera,SIGNAL(stepMoveChanged(double)),this,SLOT(setStep(double)));
    QObject::connect(camera,SIGNAL(coordSys(int)),this,SLOT(coordSys(int)));
  }
}

ViewRelativeObjectGL::ViewRelativeObjectGL(ViewCameraGL *camera, QGLWidget * parent, const char * name):
  ViewObjectGL(parent,name),
  __step(1){
  if(camera){
    QObject::connect(camera,SIGNAL(stepMoveChanged(double)),this,SLOT(setStep(double)));  
    QObject::connect(camera,SIGNAL(coordSysChanged(int)),this,SLOT(coordSys(int)));
  }
}

void 
ViewRelativeObjectGL::connectTo(ViewCameraGL *camera)
{
  if(camera)
	QObject::connect(camera,SIGNAL(stepMoveChanged(double)),this,SLOT(setStep(double)));  
}

void 
ViewRelativeObjectGL::connectTo(ViewStatusBar * s)
{
  ViewObjectGL::connectTo(s);
}

void 
ViewRelativeObjectGL::connectTo(QGLWidget *g)
{
  ViewObjectGL::connectTo(g);
}

void 
ViewRelativeObjectGL::connectTo(ViewErrorDialog *e)
{
  ViewObjectGL::connectTo(e);
}


ViewRelativeObjectGL::~ViewRelativeObjectGL()
{
}

void
ViewRelativeObjectGL::setStep(double step)
{
  changeStepEvent(step,__step);
  __step = step;
}

void
ViewRelativeObjectGL::changeStepEvent(double newStep, double oldStep)
{
}

const int 
ViewRelativeObjectGL::getStep() const
{
  return __step;
}

void 
ViewRelativeObjectGL::coordSys(int i)
{
  if(i == 1)geomCoordSys();
  else if(i == 0)glCoordSys();
}

void 
ViewRelativeObjectGL::geomCoordSys()
{
}

void 
ViewRelativeObjectGL::glCoordSys()
{
}
