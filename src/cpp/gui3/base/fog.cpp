/*
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

#include <qpopupmenu.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlineedit.h>
#include <qcolordialog.h>
#include <qgl.h>
#include <algo/opengl/util_appegl.h>
#include "fog.h"
#include "icons.h"
#include "util_qwidget.h"
#include "interface/fogwidget.h"
#include <assert.h>
#include <iostream>

/* ----------------------------------------------------------------------- */


ViewFogGL3::ViewFogGL3(ViewCameraGL3 *camera,QGLWidget * parent, const char * name):
  ViewRelativeObjectGL3(camera,parent,name),
  __enable(false),
  __mode(0),
  __hintmode(0),
  __color(255,255,255),
  __density(0.5),
  __start(0),
  __end(10),
  __control(new ViewDialog3(parent,"Fog Control",false)),
  __fogw(new FogWidget(__control,"Fog Control Widget"))
{
  __control->setCaption(tr("Fog Control"));
  QPixmap col(__fogw->ColorButton->size());
  col.fill(__color);
  __fogw->ColorButton->setPixmap(col);  
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

ViewFogGL3::~ViewFogGL3()
{
}

QPopupMenu * 
ViewFogGL3::createToolsMenu(QWidget * parent)
{
  QPixmap wheel(ViewerIcon3::icon_wheel);
  QPopupMenu * menu = new QPopupMenu(parent,"Fog Menu");
  int id = menu->insertItem(wheel,tr("Control"),__control, SLOT(show()));
  ViewPopupButton3 * bt = new ViewPopupButton3(menu,id,"Control Fog");
  QObject::connect(__control,SIGNAL(visibilityChanged(bool)),bt,SLOT(check(bool)));
  menu->insertSeparator();
  id = menu->insertItem(tr("Enable"),this, SLOT(setEnable()));
  bt = new ViewPopupButton3(menu,id,"Enable Fog");
  QObject::connect(this,SIGNAL(enabled(bool)),bt,SLOT(check(bool)));
  return menu;
}


void 
ViewFogGL3::changeStepEvent(const int newStep, const int oldStep)
{
  real_t r = real_t(newStep)/real_t(oldStep);
  __start *= r;
  __end *= r;
  __fogw->EndEdit->setText(QString::number(__end));
  __fogw->StartEdit->setText(QString::number(__start));
  __density /= r;
}

bool 
ViewFogGL3::enable() const
{
  return __enable;
}

int 
ViewFogGL3::getMode() const
{
  return __mode;
}

int 
ViewFogGL3::getHintMode() const
{
  return __hintmode;
}

const QColor& 
ViewFogGL3::getColor() const
{
  return __color;
}

double 
ViewFogGL3::getDensity() const
{
  return __density;
}

double 
ViewFogGL3::getStart() const
{
  return __start;
}

double 
ViewFogGL3::getEnd() const
{
  return __end;
}

void 
ViewFogGL3::setEnable(bool b)
{
  if(__enable != b){
    __enable = b;
    emit enabled(__enable);
    emit valueChanged();
  }
}

void 
ViewFogGL3::setEnable()
{
  __enable = ! __enable;
  emit enabled(__enable);
  emit valueChanged();
}

void 
ViewFogGL3::setMode(int i)
{
  if(__mode != i){
    __mode = i;
    __fogw->ModeCombo->setCurrentItem(i);
    emit valueChanged();
  }
}

void 
ViewFogGL3::setHints(int i)
{
  if(__hintmode != i){
    __hintmode = i;
    __fogw->HintsCombo->setCurrentItem(i);
    emit valueChanged();
  }
}

void 
ViewFogGL3::changeColor()
{
  QColor m = QColorDialog::getColor(__color,__control);
  setColor(m);
  emit colorChanged(m);
}

void 
ViewFogGL3::setColor(const QColor& m){
  if(m.isValid()){
    __color=m;
    QPixmap col(__fogw->ColorButton->size());
    col.fill(__color);
    __fogw->ColorButton->setPixmap(col);  
    emit valueChanged();
  }
}

void 
ViewFogGL3::setDensity(int i)
{
  __density = float(i)/(100.0*getStep());
  emit valueChanged();
}

void 
ViewFogGL3::setDensity(const double& d)
{
  __density = d;
  emit valueChanged();
}

void 
ViewFogGL3::setStart(const QString& s)
{
  __start = s.toDouble();
}

void 
ViewFogGL3::setStart(const double& s)
{
  __start = s;
}

void 
ViewFogGL3::setEnd(const QString& e)
{
  __end = e.toDouble();
}


void 
ViewFogGL3::setEnd(const double& e)
{
  __end = e;
}


void 
ViewFogGL3::validValue()
{
  emit valueChanged();
}

void 
ViewFogGL3::initializeGL()
{
}

void 
ViewFogGL3::paintGL()
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

