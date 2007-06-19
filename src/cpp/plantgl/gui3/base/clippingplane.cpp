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


#include <qcheckbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qgl.h>
   
#include "clippingplane.h"
#include "interface/clippingplanewidget.h"
#include "icons.h"
#include "util_qwidget.h"
#include "GL/gl.h"

/* ----------------------------------------------------------------------- */

ViewClippingPlaneGL3::ViewClippingPlaneGL3(QGLWidget * parent, const char * name):
  ViewObjectGL3(parent,name)
{
  for(int i = 0; i < 6 ; i++)
    __enable[i] = false;
  for(int j = 0; j < 6 ; j++)
    __A[j] = 0;
  for(int k = 0; k < 6 ; k++)
    __B[k] = 0;
  for(int l = 0; l < 6 ; l++)
    __C[l] = 0;
  for(int m = 0; m < 6 ; m++)
    __D[m] = 0;
  __A[0] = -1;
  __A[1] = 1;
  __B[2] = -1;
  __B[3] = 1;
  __C[4] = -1;
  __C[5] = 1;

  __control = new ViewDialog3(parent,"Clipping Planes Control");
  __control->setCaption(tr("Clipping Planes Control"));
  __cpw = new ClippingPlaneWidget(__control,"CP Widget", false);
  QObject::connect(__cpw->OkButton,SIGNAL(clicked()),__control,SLOT(hide()));
  QObject::connect(__cpw->Enable1,SIGNAL(toggled(bool)),this,SLOT(setPlane1Enable(bool)));
  QObject::connect(__cpw->Enable2,SIGNAL(toggled(bool)),this,SLOT(setPlane2Enable(bool)));
  QObject::connect(__cpw->Enable3,SIGNAL(toggled(bool)),this,SLOT(setPlane3Enable(bool)));
  QObject::connect(__cpw->Enable4,SIGNAL(toggled(bool)),this,SLOT(setPlane4Enable(bool)));
  QObject::connect(__cpw->Enable5,SIGNAL(toggled(bool)),this,SLOT(setPlane5Enable(bool)));
  QObject::connect(__cpw->Enable6,SIGNAL(toggled(bool)),this,SLOT(setPlane6Enable(bool)));
/*  QObject::connect(this,SIGNAL(plane1Enabled(bool)),__cpw->Enable1,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(plane2Enabled(bool)),__cpw->Enable2,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(plane3Enabled(bool)),__cpw->Enable3,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(plane4Enabled(bool)),__cpw->Enable4,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(plane5Enabled(bool)),__cpw->Enable5,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(plane6Enabled(bool)),__cpw->Enable6,SLOT(setChecked(bool))); */
  QObject::connect(__cpw->A1,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane1A(const QString&)));
  QObject::connect(__cpw->A1,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->A2,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane2A(const QString&)));
  QObject::connect(__cpw->A2,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->A3,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane3A(const QString&)));
  QObject::connect(__cpw->A3,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->A4,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane4A(const QString&)));
  QObject::connect(__cpw->A4,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->A5,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane5A(const QString&)));
  QObject::connect(__cpw->A5,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->A6,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane6A(const QString&)));
  QObject::connect(__cpw->A6,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->B1,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane1B(const QString&)));
  QObject::connect(__cpw->B1,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->B2,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane2B(const QString&)));
  QObject::connect(__cpw->B2,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->B3,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane3B(const QString&)));
  QObject::connect(__cpw->B3,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->B4,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane4B(const QString&)));
  QObject::connect(__cpw->B4,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->B5,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane5B(const QString&)));
  QObject::connect(__cpw->B5,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->B6,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane6B(const QString&)));
  QObject::connect(__cpw->B6,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->C1,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane1C(const QString&)));
  QObject::connect(__cpw->C1,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->C2,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane2C(const QString&)));
  QObject::connect(__cpw->C2,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->C3,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane3C(const QString&)));
  QObject::connect(__cpw->C3,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->C4,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane4C(const QString&)));
  QObject::connect(__cpw->C4,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->C5,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane5C(const QString&)));
  QObject::connect(__cpw->C5,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->C6,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane6C(const QString&)));
  QObject::connect(__cpw->C6,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->D1,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane1D(const QString&)));
  QObject::connect(__cpw->D1,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->D2,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane2D(const QString&)));
  QObject::connect(__cpw->D2,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->D3,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane3D(const QString&)));
  QObject::connect(__cpw->D3,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->D4,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane4D(const QString&)));
  QObject::connect(__cpw->D4,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->D5,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane5D(const QString&)));
  QObject::connect(__cpw->D5,SIGNAL(returnPressed()),this,SLOT(validValue()));
  QObject::connect(__cpw->D6,SIGNAL(textChanged(const QString&)),this,SLOT(setPlane6D(const QString&)));
  QObject::connect(__cpw->D6,SIGNAL(returnPressed()),this,SLOT(validValue()));  
  __cpw->A1->setText(QString::number(__A[0]));
  __cpw->A2->setText(QString::number(__A[1]));
  __cpw->A3->setText(QString::number(__A[2]));
  __cpw->A4->setText(QString::number(__A[3]));
  __cpw->A5->setText(QString::number(__A[4]));
  __cpw->A6->setText(QString::number(__A[5]));
  __cpw->B1->setText(QString::number(__B[0]));
  __cpw->B2->setText(QString::number(__B[1]));
  __cpw->B3->setText(QString::number(__B[2]));
  __cpw->B4->setText(QString::number(__B[3]));
  __cpw->B5->setText(QString::number(__B[4]));
  __cpw->B6->setText(QString::number(__B[5]));
  __cpw->C1->setText(QString::number(__C[0]));
  __cpw->C2->setText(QString::number(__C[1]));
  __cpw->C3->setText(QString::number(__C[2]));
  __cpw->C4->setText(QString::number(__C[3]));
  __cpw->C5->setText(QString::number(__C[4]));
  __cpw->C6->setText(QString::number(__C[5]));
  __cpw->D1->setText(QString::number(__D[0]));
  __cpw->D2->setText(QString::number(__D[1]));
  __cpw->D3->setText(QString::number(__D[2]));
  __cpw->D4->setText(QString::number(__D[3]));
  __cpw->D5->setText(QString::number(__D[4]));
  __cpw->D6->setText(QString::number(__D[5]));

}
  
ViewClippingPlaneGL3::~ViewClippingPlaneGL3()
{
}

QPopupMenu * 
ViewClippingPlaneGL3::createToolsMenu(QWidget * parent)
{
  return new ViewCPlaneMenu3(this,parent,"Clipping Plane Menu");
}

bool ViewClippingPlaneGL3::isPlaneEnable(int i)
{
  if(i < 1 || i > 6 )return false;
  else return __enable[i-1];
}

double 
ViewClippingPlaneGL3::valA(int i)
{
  if(i < 1 || i > 6 )return 0;
  else return __A[i-1];  
}

double  
ViewClippingPlaneGL3::valB(int i)
{
  if(i < 1 || i > 6 )return 0;
  else return __B[i-1];  
}

double 
ViewClippingPlaneGL3::valC(int i)
{
  if(i < 1 || i > 6 )return 0;
  else return __C[i-1];  
}

double 
ViewClippingPlaneGL3::valD(int i)
{
  if(i < 1 || i > 6 )return 0;
  else return __D[i-1];  
}

ViewDialog3 * 
ViewClippingPlaneGL3::getControl() const
{
  return __control;
}

void 
ViewClippingPlaneGL3::setPlane1Enable(bool b)
{
  if(__enable[0] != b){
    __enable[0] = b;
    emit valueChanged();
    emit plane1Enabled(__enable[0]);
    __cpw->Enable1->setChecked(__enable[0]);
  }
}

void 
ViewClippingPlaneGL3::setPlane2Enable(bool b)
{
  if(__enable[1] != b){
    __enable[1] = b;
    emit valueChanged();
    emit plane2Enabled(__enable[1]);  
    __cpw->Enable2->setChecked(__enable[1]);
  }
}

void 
ViewClippingPlaneGL3::setPlane3Enable(bool b)
{
  if(__enable[2] != b){
    __enable[2] = b;
    emit valueChanged();
    emit plane3Enabled(__enable[2]);
    __cpw->Enable3->setChecked(__enable[2]); 
  }
}

void 
ViewClippingPlaneGL3::setPlane4Enable(bool b)
{
  if(__enable[3] != b){
    __enable[3] = b;
    emit valueChanged();
    emit plane4Enabled(__enable[3]);
    __cpw->Enable4->setChecked(__enable[3]); 
  }
}

void 
ViewClippingPlaneGL3::setPlane5Enable(bool b)
{
  if(__enable[4] != b){
    __enable[4] = b;
    emit valueChanged();
    emit plane5Enabled(__enable[4]);
    __cpw->Enable5->setChecked(__enable[4]); 
  }
}

void 
ViewClippingPlaneGL3::setPlane6Enable(bool b)
{
  if(__enable[5] != b){
    __enable[5] = b;
    emit valueChanged();
    emit plane6Enabled(__enable[5]);
    __cpw->Enable6->setChecked(__enable[5]);
  }
}

void 
ViewClippingPlaneGL3::setPlane1Enable()
{
  __enable[0] = ! __enable[0];
  emit valueChanged();
  emit plane1Enabled(__enable[0]);  
  __cpw->Enable1->setChecked(__enable[0]);
}

void 
ViewClippingPlaneGL3::setPlane2Enable()
{
  __enable[1] = ! __enable[1];
  emit valueChanged();
  emit plane2Enabled(__enable[1]);  
  __cpw->Enable2->setChecked(__enable[1]);
}

void 
ViewClippingPlaneGL3::setPlane3Enable()
{
  __enable[2] = ! __enable[2];
  emit valueChanged();
  emit plane3Enabled(__enable[2]);
  __cpw->Enable3->setChecked(__enable[2]); 
}

void 
ViewClippingPlaneGL3::setPlane4Enable()
{
  __enable[3] = ! __enable[3];
  emit valueChanged();
  emit plane4Enabled(__enable[3]);
  __cpw->Enable4->setChecked(__enable[3]);
}

void 
ViewClippingPlaneGL3::setPlane5Enable()
{
  __enable[4] = ! __enable[4];
  emit valueChanged();
  emit plane5Enabled(__enable[4]);
  __cpw->Enable5->setChecked(__enable[4]);
}

void 
ViewClippingPlaneGL3::setPlane6Enable()
{
  __enable[5] = ! __enable[5];
  emit valueChanged();
  emit plane6Enabled(__enable[5]);
  __cpw->Enable6->setChecked(__enable[5]);
}

void 
ViewClippingPlaneGL3::setPlane1A(const QString& s)
{
  __A[0] = s.toDouble();
}
 
void 
ViewClippingPlaneGL3::setPlane2A(const QString& s)
{
  __A[1] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane3A(const QString& s)
{
  __A[2] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane4A(const QString& s)
{
  __A[3] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane5A(const QString& s)
{
  __A[4] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane6A(const QString& s)
{
  __A[5] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane1B(const QString& s)
{
  __B[0] = s.toDouble();
}
 
void 
ViewClippingPlaneGL3::setPlane2B(const QString& s)
{
  __B[1] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane3B(const QString& s)
{
  __B[2] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane4B(const QString& s)
{
  __B[3] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane5B(const QString& s)
{
  __B[4] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane6B(const QString& s)
{
  __B[5] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane1C(const QString& s)
{
  __C[0] = s.toDouble();
}
 
void 
ViewClippingPlaneGL3::setPlane2C(const QString& s)
{
  __C[1] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane3C(const QString& s)
{
  __C[2] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane4C(const QString& s)
{
  __C[3] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane5C(const QString& s)
{
  __C[4] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane6C(const QString& s)
{
  __C[5] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane1D(const QString& s)
{
  __D[0] = s.toDouble();
}
 
void 
ViewClippingPlaneGL3::setPlane2D(const QString& s)
{
  __D[1] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane3D(const QString& s)
{
  __D[2] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane4D(const QString& s)
{
  __D[3] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane5D(const QString& s)
{
  __D[4] = s.toDouble();
}

void 
ViewClippingPlaneGL3::setPlane6D(const QString& s)
{
  __D[5] = s.toDouble();
}


void 
ViewClippingPlaneGL3::validValue()
{
  emit valueChanged();
}

void 
ViewClippingPlaneGL3::initializeGL()
{
}

void 
ViewClippingPlaneGL3::paintGL()
{
  if(__enable[0]){
    GLdouble eq[4];
    eq[0] = __A[0];
    eq[1] = __B[0];
    eq[2] = __C[0];
    eq[3] = __D[0];
    glClipPlane(GL_CLIP_PLANE0,eq);
    glEnable(GL_CLIP_PLANE0);
  }
  else {
    glDisable(GL_CLIP_PLANE0);
  }
  if(__enable[1]){
    GLdouble eq[4];
    eq[0] = __A[1];
    eq[1] = __B[1];
    eq[2] = __C[1];
    eq[3] = __D[1];
    glClipPlane(GL_CLIP_PLANE1,eq);
    glEnable(GL_CLIP_PLANE1);
  }
  else {
    glDisable(GL_CLIP_PLANE1);
  }
  if(__enable[2]){
    GLdouble eq[4];
    eq[0] = __A[2];
    eq[1] = __B[2];
    eq[2] = __C[2];
    eq[3] = __D[2];
    glClipPlane(GL_CLIP_PLANE2,eq);
    glEnable(GL_CLIP_PLANE2);
  }
  else {
    glDisable(GL_CLIP_PLANE2);
  }

  if(__enable[3]){
    GLdouble eq[4];
    eq[0] = __A[3];
    eq[1] = __B[3];
    eq[2] = __C[3];
    eq[3] = __D[3];
    glClipPlane(GL_CLIP_PLANE3,eq);
    glEnable(GL_CLIP_PLANE3);
  }
  else {
    glDisable(GL_CLIP_PLANE3);
  }
  if(__enable[4]){
    GLdouble eq[4];
    eq[0] = __A[4];
    eq[1] = __B[4];
    eq[2] = __C[4];
    eq[3] = __D[4];
    glClipPlane(GL_CLIP_PLANE4,eq);
    glEnable(GL_CLIP_PLANE4);
  }
  else {
    glDisable(GL_CLIP_PLANE4);
  }
  if(__enable[5]){
    GLdouble eq[4];
    eq[0] = __A[5];
    eq[1] = __B[5];
    eq[2] = __C[5];
    eq[3] = __D[5];
    glClipPlane(GL_CLIP_PLANE5,eq);
    glEnable(GL_CLIP_PLANE5);
  }
  else {
    glDisable(GL_CLIP_PLANE5);
  }
  GEOM_GL_ERROR;
}

/* ----------------------------------------------------------------------- */

ViewCPlaneMenu3::ViewCPlaneMenu3(ViewClippingPlaneGL3 * cp, QWidget * parent, const char * name):
  QPopupMenu(parent,name)
{
  if(cp){
    QPixmap wheel(ViewerIcon3::icon_wheel);
    ctrl = insertItem(wheel,tr("Control"),cp->getControl(),SLOT(show()));
    insertSeparator();
    idP1 = insertItem(tr("Plane")+" 1",cp,SLOT(setPlane1Enable()));
    idP2 = insertItem(tr("Plane")+" 2",cp,SLOT(setPlane2Enable()));
    idP3 = insertItem(tr("Plane")+" 3",cp,SLOT(setPlane3Enable()));
    idP4 = insertItem(tr("Plane")+" 4",cp,SLOT(setPlane4Enable()));
    idP5 = insertItem(tr("Plane")+" 5",cp,SLOT(setPlane5Enable()));
    idP6 = insertItem(tr("Plane")+" 6",cp,SLOT(setPlane6Enable()));
    setCheckable( TRUE );
    setItemChecked(idP1,cp->isPlaneEnable(1));
    setItemChecked(idP2,cp->isPlaneEnable(2));
    setItemChecked(idP3,cp->isPlaneEnable(3));
    setItemChecked(idP4,cp->isPlaneEnable(4));
    setItemChecked(idP5,cp->isPlaneEnable(5));
    setItemChecked(idP6,cp->isPlaneEnable(6));
    setItemChecked(ctrl,cp->getControl()->isVisible());
    QObject::connect(cp->getControl(),SIGNAL(visibilityChanged(bool)),this,SLOT(controlVisibility(bool)));
    QObject::connect(cp,SIGNAL(plane1Enabled(bool)),this,SLOT(setPlane1Enable(bool)));
    QObject::connect(cp,SIGNAL(plane2Enabled(bool)),this,SLOT(setPlane2Enable(bool)));
    QObject::connect(cp,SIGNAL(plane3Enabled(bool)),this,SLOT(setPlane3Enable(bool)));
    QObject::connect(cp,SIGNAL(plane4Enabled(bool)),this,SLOT(setPlane4Enable(bool)));
    QObject::connect(cp,SIGNAL(plane5Enabled(bool)),this,SLOT(setPlane5Enable(bool)));
    QObject::connect(cp,SIGNAL(plane6Enabled(bool)),this,SLOT(setPlane6Enable(bool)));
  }
}

ViewCPlaneMenu3::~ViewCPlaneMenu3()
{
}


void 
ViewCPlaneMenu3::setPlane1Enable(bool b)
{
  setItemChecked(idP1,b);
}

void 
ViewCPlaneMenu3::setPlane2Enable(bool b)
{
  setItemChecked(idP2,b);
}

void 
ViewCPlaneMenu3::setPlane3Enable(bool b)
{
  setItemChecked(idP3,b);
}
void 
ViewCPlaneMenu3::setPlane4Enable(bool b)
{
  setItemChecked(idP4,b);
}

void
ViewCPlaneMenu3::setPlane5Enable(bool b)
{
  setItemChecked(idP5,b);
}

void 
ViewCPlaneMenu3::setPlane6Enable(bool b)
{
  setItemChecked(idP6,b);
}

void 
ViewCPlaneMenu3::controlVisibility(bool b)
{
  setItemChecked(ctrl,b);
}
