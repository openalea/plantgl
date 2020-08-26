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


#include "object.h"


#include <QtCore/qpoint.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmessagebox.h>
#else
    #include <QtGui/qmessagebox.h>
#endif
#include <QtOpenGL/qgl.h>

#include <plantgl/algo/opengl/util_gl.h>
#include <plantgl/algo/opengl/util_glu.h>

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
              parent,SLOT(redrawGL()) );
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

void ViewObjectGL::fillToolBarAdditionnal(QToolBar * toolBar)
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
              g,SLOT(redrawGL()) );
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
bool ViewObjectGL::BATCHMODE = false;

bool
ViewObjectGL::glError(QWidget * widget, const char * file, int line)
{
  GLenum _glerror;

  if((_glerror = glGetError()) != GL_NO_ERROR){
      QString _mess = "<b>[ObjectGL] GL Error ["+QString::number(_glerror)+"] !!</b><br>";
      int i = 0;
      while(_glerror != GL_NO_ERROR && i < 10){
          _mess += gluGeomErrorString(_glerror);
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
      if(false) { // !BATCHMODE){
          if(!lock){
              lock = true;
              int res = QMessageBox::critical(widget,tr("GL Error"),_mess,tr("Abort"),tr("Continue"));
              if(res == 0 || res == -1){
                  abort();
              }
              lock = false;
          }
      }
      else qWarning("%s",qPrintable(_mess));
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
