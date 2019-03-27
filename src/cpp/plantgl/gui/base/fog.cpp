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


#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qcombobox.h>
    #include <QtWidgets/qpushbutton.h>
    #include <QtWidgets/qslider.h>
    #include <QtWidgets/qlineedit.h>
    #include <QtWidgets/qcolordialog.h>
#else
    #include <QtGui/qmenu.h>
    #include <QtGui/qcombobox.h>
    #include <QtGui/qpushbutton.h>
    #include <QtGui/qslider.h>
    #include <QtGui/qlineedit.h>
    #include <QtGui/qcolordialog.h>
#endif
#include <QtOpenGL/qgl.h>
#include <plantgl/algo/opengl/util_appegl.h>
#include "fog.h"
#include "icons.h"
#include "util_qwidget.h"
#include "interface/fogwidget.h"
#include <assert.h>
#include <iostream>

/* ----------------------------------------------------------------------- */


ViewFogGL::ViewFogGL(ViewCameraGL *camera,QGLWidget * parent, const char * name):
  ViewRelativeObjectGL(camera,parent,name),
  __enable(false),
  __mode(0),
  __hintmode(0),
  __color(255,255,255),
  __density(0.5),
  __start(0),
  __end(10),
  __control(new ViewDialog(parent,"Fog Control",false)),
  __fogw(new Ui::FogWidget())
{
  __control->setWindowTitle(tr("Fog Control"));
  __control->setModal(false);
  __fogw->setupUi(__control);
  QPixmap col(__fogw->ColorButton->size());
  col.fill(__color);
  __fogw->ColorButton->setIcon(QIcon(col));
  __fogw->EndEdit->setText(QString::number(__end));
  __fogw->StartEdit->setText(QString::number(__start));
  __fogw->DensitySlider->setValue(50);
  QObject::connect(__fogw->OkButton,SIGNAL(pressed()), __control,SLOT(hide()));
  QObject::connect(__fogw->ModeCombo,SIGNAL(activated(int)),this,SLOT(setMode(int)));
  QObject::connect(__fogw->HintsCombo,SIGNAL(activated(int)),this,SLOT(setHints(int)));
  QObject::connect(__fogw->ColorButton,SIGNAL(pressed()),this,SLOT(changeColor()));
  QObject::connect(__fogw->DensitySlider,SIGNAL(valueChanged(int)),this,SLOT(setDensity(int)));
  QObject::connect(__fogw->StartEdit,SIGNAL(textChanged(const QString&)),this,SLOT(setStart(const QString&)));
  QObject::connect(__fogw->EndEdit,SIGNAL(textChanged(const QString&)),this,SLOT(setEnd(const QString&)));
  QObject::connect(__fogw->StartEdit,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__fogw->EndEdit,SIGNAL(returnPressed()),this,SLOT(validValue()));
}

ViewFogGL::~ViewFogGL()
{
}

QMenu *
ViewFogGL::createToolsMenu(QWidget * parent)
{
  QPixmap wheel(ViewerIcon::getPixmap(ViewerIcon::wheel));
  QMenu * menu = new QMenu(parent);
  QAction * act = menu->addAction(wheel,tr("Control"),__control, SLOT(show()));
  QObject::connect(__control,SIGNAL(visibilityChanged(bool)),act,SLOT(setChecked(bool)));
  menu->addSeparator();
  act = menu->addAction(tr("Enable"),this, SLOT(setEnable()));
  QObject::connect(this,SIGNAL(enabled(bool)),act,SLOT(setChecked(bool)));
  return menu;
}


void
ViewFogGL::changeStepEvent(double newStep, double oldStep)
{
  real_t r = real_t(newStep)/real_t(oldStep);
  __start *= r;
  __end *= r;
  __fogw->EndEdit->setText(QString::number(__end));
  __fogw->StartEdit->setText(QString::number(__start));
  __density /= r;
}

bool
ViewFogGL::enable() const
{
  return __enable;
}

int
ViewFogGL::getMode() const
{
  return __mode;
}

int
ViewFogGL::getHintMode() const
{
  return __hintmode;
}

const QColor&
ViewFogGL::getColor() const
{
  return __color;
}

double
ViewFogGL::getDensity() const
{
  return __density;
}

double
ViewFogGL::getStart() const
{
  return __start;
}

double
ViewFogGL::getEnd() const
{
  return __end;
}

void
ViewFogGL::setEnable(bool b)
{
  if(__enable != b){
    __enable = b;
    emit enabled(__enable);
    emit valueChanged();
  }
}

void
ViewFogGL::setEnable()
{
  __enable = ! __enable;
  emit enabled(__enable);
  emit valueChanged();
}

void
ViewFogGL::setMode(int i)
{
  if(__mode != i){
    __mode = i;
    __fogw->ModeCombo->setCurrentIndex(i);
    emit valueChanged();
  }
}

void
ViewFogGL::setHints(int i)
{
  if(__hintmode != i){
    __hintmode = i;
    __fogw->HintsCombo->setCurrentIndex(i);
    emit valueChanged();
  }
}

void
ViewFogGL::changeColor()
{
  QColor m = QColorDialog::getColor(__color,__control);
  setColor(m);
  emit colorChanged(m);
}

void
ViewFogGL::setColor(const QColor& m){
  if(m.isValid()){
    __color=m;
    QPixmap col(__fogw->ColorButton->size());
    col.fill(__color);
    __fogw->ColorButton->setIcon(QIcon(col));
    if(__frame->isVisible())emit valueChanged();
  }
}

void
ViewFogGL::setDensity(int i)
{
  __density = float(i)/(100.0*getStep());
  emit valueChanged();
}

void
ViewFogGL::setDensity(const double& d)
{
  __density = d;
  emit valueChanged();
}

void
ViewFogGL::setStart(const QString& s)
{
  __start = s.toDouble();
}

void
ViewFogGL::setStart(const double& s)
{
  __start = s;
}

void
ViewFogGL::setEnd(const QString& e)
{
  __end = e.toDouble();
}


void
ViewFogGL::setEnd(const double& e)
{
  __end = e;
}


void
ViewFogGL::validValue()
{
  emit valueChanged();
}

void
ViewFogGL::initializeGL()
{
}

void
ViewFogGL::paintGL()
{
  if(__enable){
    glEnable(GL_FOG);
    if(__mode == 0)glFogi(GL_FOG_MODE,GL_LINEAR);
    else if(__mode == 1)glFogi(GL_FOG_MODE,GL_EXP);
    else if(__mode == 2)glFogi(GL_FOG_MODE,GL_EXP2);
    glGeomFogColor(__color);
    if(__hintmode == 0)glHint(GL_FOG_HINT,GL_DONT_CARE);
    else if(__hintmode == 1)glHint(GL_FOG_HINT,GL_FASTEST);
    else if(__hintmode == 2)glHint(GL_FOG_HINT,GL_NICEST);
    glFogf(GL_FOG_DENSITY,GLfloat(__density));
    glFogf(GL_FOG_START,GLfloat(__start));
    glFogf(GL_FOG_END,GLfloat(__end));
  }
  else {
    glDisable(GL_FOG);
  }
  GEOM_GL_ERROR;
}

