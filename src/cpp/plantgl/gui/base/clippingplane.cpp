/*
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


#include <qcheckbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qgl.h>
   
#include "clippingplane.h"
#include "event.h"
#include "interface/clippingplanewidget.h"
#include "icons.h"
#include "util_qwidget.h"
#include "GL/gl.h"

/* ----------------------------------------------------------------------- */

static bool DEFAULT_PLANE_ACTIVATION[6] = { false, false, false, false, false, false };
static double DEFAULT_A[6] = { -1, 1, 0, 0, 0, 0 };
static double DEFAULT_B[6] = { 0, 0, -1, 1, 0, 0 };
static double DEFAULT_C[6] = { 0, 0, 0, 0, -1, 1 };
static double DEFAULT_D[6] = { 0, 0, 0, 0, 0, 0 };

ViewClippingPlaneGL::ViewClippingPlaneGL(QGLWidget * parent, const char * name):
  ViewObjectGL(parent,name)
{
	memcpy(__enable,DEFAULT_PLANE_ACTIVATION,sizeof(__enable));
	memcpy(__A,DEFAULT_A,sizeof(__A));
	memcpy(__B,DEFAULT_B,sizeof(__B));
	memcpy(__C,DEFAULT_C,sizeof(__C));
	memcpy(__D,DEFAULT_D,sizeof(__D));

  __control = new ViewDialog(parent,"Clipping Planes Control");
  __control->setWindowTitle(tr("Clipping Planes Control"));
  Ui::ClippingPlaneWidget * __cpw = new Ui::ClippingPlaneWidget();
  __cpw->setupUi(__control);
  __cpw->A1->setRange(-DBL_MAX,DBL_MAX);
  __cpw->A1->setValue(__A[0]);
  __cpw->A2->setRange(-DBL_MAX,DBL_MAX);
  __cpw->A2->setValue(__A[1]);
  __cpw->A3->setRange(-DBL_MAX,DBL_MAX);
  __cpw->A3->setValue(__A[2]);
  __cpw->A4->setRange(-DBL_MAX,DBL_MAX);
  __cpw->A4->setValue(__A[3]);
  __cpw->A5->setRange(-DBL_MAX,DBL_MAX);
  __cpw->A5->setValue(__A[4]);
  __cpw->A6->setRange(-DBL_MAX,DBL_MAX);
  __cpw->A6->setValue(__A[5]);
  __cpw->B1->setRange(-DBL_MAX,DBL_MAX);
  __cpw->B1->setValue(__B[0]);
  __cpw->B2->setRange(-DBL_MAX,DBL_MAX);
  __cpw->B2->setValue(__B[1]);
  __cpw->B3->setRange(-DBL_MAX,DBL_MAX);
  __cpw->B3->setValue(__B[2]);
  __cpw->B4->setRange(-DBL_MAX,DBL_MAX);
  __cpw->B4->setValue(__B[3]);
  __cpw->B5->setRange(-DBL_MAX,DBL_MAX);
  __cpw->B5->setValue(__B[4]);
  __cpw->B6->setRange(-DBL_MAX,DBL_MAX);
  __cpw->B6->setValue(__B[5]);
  __cpw->C1->setRange(-DBL_MAX,DBL_MAX);
  __cpw->C1->setValue(__C[0]);
  __cpw->C2->setRange(-DBL_MAX,DBL_MAX);
  __cpw->C2->setValue(__C[1]);
  __cpw->C3->setRange(-DBL_MAX,DBL_MAX);
  __cpw->C3->setValue(__C[2]);
  __cpw->C4->setRange(-DBL_MAX,DBL_MAX);
  __cpw->C4->setValue(__C[3]);
  __cpw->C5->setRange(-DBL_MAX,DBL_MAX);
  __cpw->C5->setValue(__C[4]);
  __cpw->C6->setRange(-DBL_MAX,DBL_MAX);
  __cpw->C6->setValue(__C[5]);
  __cpw->D1->setRange(-DBL_MAX,DBL_MAX);
  __cpw->D1->setValue(__D[0]);
  __cpw->D2->setRange(-DBL_MAX,DBL_MAX);
  __cpw->D2->setValue(__D[1]);
  __cpw->D3->setRange(-DBL_MAX,DBL_MAX);
  __cpw->D3->setValue(__D[2]);
  __cpw->D4->setRange(-DBL_MAX,DBL_MAX);
  __cpw->D4->setValue(__D[3]);
  __cpw->D5->setRange(-DBL_MAX,DBL_MAX);
  __cpw->D5->setValue(__D[4]);
  __cpw->D6->setRange(-DBL_MAX,DBL_MAX);
  __cpw->D6->setValue(__D[5]);
  QObject::connect(__cpw->OkButton,SIGNAL(clicked()),__control,SLOT(hide()));
  QObject::connect(__cpw->Enable1,SIGNAL(clicked(bool)),this,SLOT(setPlane1Enable(bool)));
  QObject::connect(__cpw->Enable2,SIGNAL(clicked(bool)),this,SLOT(setPlane2Enable(bool)));
  QObject::connect(__cpw->Enable3,SIGNAL(clicked(bool)),this,SLOT(setPlane3Enable(bool)));
  QObject::connect(__cpw->Enable4,SIGNAL(clicked(bool)),this,SLOT(setPlane4Enable(bool)));
  QObject::connect(__cpw->Enable5,SIGNAL(clicked(bool)),this,SLOT(setPlane5Enable(bool)));
  QObject::connect(__cpw->Enable6,SIGNAL(clicked(bool)),this,SLOT(setPlane6Enable(bool)));
  QObject::connect(this,SIGNAL(plane1Enabled(bool)),__cpw->Enable1,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(plane2Enabled(bool)),__cpw->Enable2,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(plane3Enabled(bool)),__cpw->Enable3,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(plane4Enabled(bool)),__cpw->Enable4,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(plane5Enabled(bool)),__cpw->Enable5,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(plane6Enabled(bool)),__cpw->Enable6,SLOT(setChecked(bool))); 
  QObject::connect(__cpw->A1,SIGNAL(valueChanged(double)),this,SLOT(setPlane1A(double)));
  QObject::connect(__cpw->A2,SIGNAL(valueChanged(double)),this,SLOT(setPlane2A(double)));
  QObject::connect(__cpw->A3,SIGNAL(valueChanged(double)),this,SLOT(setPlane3A(double)));
  QObject::connect(__cpw->A4,SIGNAL(valueChanged(double)),this,SLOT(setPlane4A(double)));
  QObject::connect(__cpw->A5,SIGNAL(valueChanged(double)),this,SLOT(setPlane5A(double)));
  QObject::connect(__cpw->A6,SIGNAL(valueChanged(double)),this,SLOT(setPlane6A(double)));
  QObject::connect(__cpw->B1,SIGNAL(valueChanged(double)),this,SLOT(setPlane1B(double)));
  QObject::connect(__cpw->B2,SIGNAL(valueChanged(double)),this,SLOT(setPlane2B(double)));
  QObject::connect(__cpw->B3,SIGNAL(valueChanged(double)),this,SLOT(setPlane3B(double)));
  QObject::connect(__cpw->B4,SIGNAL(valueChanged(double)),this,SLOT(setPlane4B(double)));
  QObject::connect(__cpw->B5,SIGNAL(valueChanged(double)),this,SLOT(setPlane5B(double)));
  QObject::connect(__cpw->B6,SIGNAL(valueChanged(double)),this,SLOT(setPlane6B(double)));
  QObject::connect(__cpw->C1,SIGNAL(valueChanged(double)),this,SLOT(setPlane1C(double)));
  QObject::connect(__cpw->C2,SIGNAL(valueChanged(double)),this,SLOT(setPlane2C(double)));
  QObject::connect(__cpw->C3,SIGNAL(valueChanged(double)),this,SLOT(setPlane3C(double)));
  QObject::connect(__cpw->C4,SIGNAL(valueChanged(double)),this,SLOT(setPlane4C(double)));
  QObject::connect(__cpw->C5,SIGNAL(valueChanged(double)),this,SLOT(setPlane5C(double)));
  QObject::connect(__cpw->C6,SIGNAL(valueChanged(double)),this,SLOT(setPlane6C(double)));
  QObject::connect(__cpw->D1,SIGNAL(valueChanged(double)),this,SLOT(setPlane1D(double)));
  QObject::connect(__cpw->D2,SIGNAL(valueChanged(double)),this,SLOT(setPlane2D(double)));
  QObject::connect(__cpw->D3,SIGNAL(valueChanged(double)),this,SLOT(setPlane3D(double)));
  QObject::connect(__cpw->D4,SIGNAL(valueChanged(double)),this,SLOT(setPlane4D(double)));
  QObject::connect(__cpw->D5,SIGNAL(valueChanged(double)),this,SLOT(setPlane5D(double)));
  QObject::connect(__cpw->D6,SIGNAL(valueChanged(double)),this,SLOT(setPlane6D(double)));
}
  
ViewClippingPlaneGL::~ViewClippingPlaneGL()
{
}

#define PLANEMENUICON(num) \
    planeAction = menu->addAction(tr("Plane")+" " #num,this,SLOT(setPlane##num##Enable())); \
	planeAction->setCheckable( TRUE ); \
	planeAction->setChecked( isPlaneEnable(num) ); \
	QObject::connect(this,SIGNAL(plane##num##Enabled(bool)),planeAction,SLOT(setChecked(bool))); \

QMenu * 
ViewClippingPlaneGL::createToolsMenu(QWidget * parent)
{
	QMenu * menu = new QMenu(parent);
    QPixmap wheel(ViewerIcon::getPixmap(ViewerIcon::wheel));
    QAction * ctrl = menu->addAction(wheel,tr("Control"),getControl(),SLOT(show()));
    ctrl->setCheckable(true);
    ctrl->setChecked(getControl()->isVisible());
    QObject::connect(getControl(),SIGNAL(visibilityChanged(bool)),ctrl,SLOT(setChecked(bool)));
    menu->addSeparator();

	QAction * planeAction = NULL;
	PLANEMENUICON(1);
	PLANEMENUICON(2);
	PLANEMENUICON(3);
	PLANEMENUICON(4);
	PLANEMENUICON(5);
	PLANEMENUICON(6);

  return menu;
}

void ViewClippingPlaneGL::clippingPlaneEvent(ViewEvent * e)
{
	if(e->type() ==ViewEvent::eClippingPlaneActivate)
	{
		ViewCPActivateEvent * ev = (ViewCPActivateEvent *)e;
		int i = ev->cpid;
		if(i < 1 || i > 6 )return ;
		if (__enable[i-1] != ev->activation){
			__enable[i-1] = ev->activation;
			emit valueChanged();
		}
	}
	else if(e->type() ==ViewEvent::eClippingPlaneSet)
	{
		ViewCPSetEvent * ev = (ViewCPSetEvent *)e;
		int i = ev->cpid;
		if(i < 1 || i > 6 )return ;
		__A[i-1] = ev->a;
		__B[i-1] = ev->b;
		__C[i-1] = ev->c;
		__D[i-1] = ev->d;
		if (__enable[i-1]){
			emit valueChanged();
		}
	}
}

bool ViewClippingPlaneGL::isPlaneEnable(int i)
{
  if(i < 1 || i > 6 )return false;
  else return __enable[i-1];
}

double 
ViewClippingPlaneGL::valA(int i)
{
  if(i < 1 || i > 6 )return 0;
  else return __A[i-1];  
}

double  
ViewClippingPlaneGL::valB(int i)
{
  if(i < 1 || i > 6 )return 0;
  else return __B[i-1];  
}

double 
ViewClippingPlaneGL::valC(int i)
{
  if(i < 1 || i > 6 )return 0;
  else return __C[i-1];  
}

double 
ViewClippingPlaneGL::valD(int i)
{
  if(i < 1 || i > 6 )return 0;
  else return __D[i-1];  
}

ViewDialog * 
ViewClippingPlaneGL::getControl() const
{
  return __control;
}

void 
ViewClippingPlaneGL::setPlane1Enable(bool b)
{
  if(__enable[0] != b){
    __enable[0] = b;
    emit valueChanged();
    emit plane1Enabled(__enable[0]);
  }
}

void 
ViewClippingPlaneGL::setPlane2Enable(bool b)
{
  if(__enable[1] != b){
    __enable[1] = b;
    emit valueChanged();
    emit plane2Enabled(__enable[1]);  
  }
}

void 
ViewClippingPlaneGL::setPlane3Enable(bool b)
{
  if(__enable[2] != b){
    __enable[2] = b;
    emit valueChanged();
    emit plane3Enabled(__enable[2]);
  }
}

void 
ViewClippingPlaneGL::setPlane4Enable(bool b)
{
  if(__enable[3] != b){
    __enable[3] = b;
    emit valueChanged();
    emit plane4Enabled(__enable[3]);
  }
}

void 
ViewClippingPlaneGL::setPlane5Enable(bool b)
{
  if(__enable[4] != b){
    __enable[4] = b;
    emit valueChanged();
    emit plane5Enabled(__enable[4]);
  }
}

void 
ViewClippingPlaneGL::setPlane6Enable(bool b)
{
  if(__enable[5] != b){
    __enable[5] = b;
    emit valueChanged();
    emit plane6Enabled(__enable[5]);
  }
}

void 
ViewClippingPlaneGL::setPlane1Enable()
{
  __enable[0] = ! __enable[0];
  emit valueChanged();
  emit plane1Enabled(__enable[0]);  
}

void 
ViewClippingPlaneGL::setPlane2Enable()
{
  __enable[1] = ! __enable[1];
  emit valueChanged();
  emit plane2Enabled(__enable[1]);  
}

void 
ViewClippingPlaneGL::setPlane3Enable()
{
  __enable[2] = ! __enable[2];
  emit valueChanged();
  emit plane3Enabled(__enable[2]);
}

void 
ViewClippingPlaneGL::setPlane4Enable()
{
  __enable[3] = ! __enable[3];
  emit valueChanged();
  emit plane4Enabled(__enable[3]);
}

void 
ViewClippingPlaneGL::setPlane5Enable()
{
  __enable[4] = ! __enable[4];
  emit valueChanged();
  emit plane5Enabled(__enable[4]);
}

void 
ViewClippingPlaneGL::setPlane6Enable()
{
  __enable[5] = ! __enable[5];
  emit valueChanged();
  emit plane6Enabled(__enable[5]);
}

void 
ViewClippingPlaneGL::setPlane1A(double s)
{
  __A[0] = s;
  emit valueChanged();
}
 
void 
ViewClippingPlaneGL::setPlane2A(double s)
{
  __A[1] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane3A(double s)
{
  __A[2] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane4A(double s)
{
  __A[3] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane5A(double s)
{
  __A[4] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane6A(double s)
{
  __A[5] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane1B(double s)
{
  __B[0] = s;
  emit valueChanged();
}
 
void 
ViewClippingPlaneGL::setPlane2B(double s)
{
  __B[1] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane3B(double s)
{
  __B[2] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane4B(double s)
{
  __B[3] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane5B(double s)
{
  __B[4] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane6B(double s)
{
  __B[5] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane1C(double s)
{
  __C[0] = s;
  emit valueChanged();
}
 
void 
ViewClippingPlaneGL::setPlane2C(double s)
{
  __C[1] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane3C(double s)
{
  __C[2] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane4C(double s)
{
  __C[3] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane5C(double s)
{
  __C[4] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane6C(double s)
{
  __C[5] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane1D(double s)
{
  __D[0] = s;
  emit valueChanged();
}
 
void 
ViewClippingPlaneGL::setPlane2D(double s)
{
  __D[1] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane3D(double s)
{
  __D[2] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane4D(double s)
{
  __D[3] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane5D(double s)
{
  __D[4] = s;
  emit valueChanged();
}

void 
ViewClippingPlaneGL::setPlane6D(double s)
{
  __D[5] = s;
  emit valueChanged();
}



void 
ViewClippingPlaneGL::initializeGL()
{
}

void 
ViewClippingPlaneGL::paintGL()
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
