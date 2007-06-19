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

#include "rotcenter.h"
#include "interface/rotcenteredit.h"
#include "icons.h"
#include "glframe.h"
#include "scenegl.h"
#include "util_qwidget.h"

#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/math/util_math.h>

#include <qlineedit.h>
#include <qslider.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qfile.h>
#include <qgl.h>
#include <plantgl/algo/opengl/util_glut.h>

TOOLS_USING_NAMESPACE
PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

ViewRotCenterGL3::ViewRotCenterGL3(ViewCameraGL3 *camera,
				 QGLWidget * parent, 
				 const char * name):
  ViewRelativeObjectGL3(camera,parent,name),
  __active(false),
  __visible(false),
  __position(0,0,0),
  __displayList(0){
  __sliders= new ViewDialog3(parent,tr("Rotating Center"),false);
  __sliders->setCaption("Rotating Center Coordinates");
  __editor = new RotCenterEdit(__sliders,"Rotating Center Coordinates");
  __sliders->setFixedSize(__editor->size());
  __editor->XEdit->setText(QString::number(__position.x()));
  __editor->YEdit->setText(QString::number(__position.y()));
  __editor->ZEdit->setText(QString::number(__position.z()));
  QObject::connect(__editor->XEdit,SIGNAL(textChanged(const QString&)),this,SLOT(setX(const QString&)));
  QObject::connect(__editor->YEdit,SIGNAL(textChanged(const QString&)),this,SLOT(setY(const QString&)));
  QObject::connect(__editor->ZEdit,SIGNAL(textChanged(const QString&)),this,SLOT(setZ(const QString&)));
  QObject::connect(this,SIGNAL(XvalueChanged(const QString&)),__editor->XEdit,SLOT(setText(const QString&)));
  QObject::connect(this,SIGNAL(YvalueChanged(const QString&)),__editor->YEdit,SLOT(setText(const QString&)));
  QObject::connect(this,SIGNAL(ZvalueChanged(const QString&)),__editor->ZEdit,SLOT(setText(const QString&)));
  QObject::connect(__editor->XSlider,SIGNAL(sliderMoved(int)),this,SLOT(setX(int)));
  QObject::connect(__editor->YSlider,SIGNAL(sliderMoved(int)),this,SLOT(setY(int)));
  QObject::connect(__editor->ZSlider,SIGNAL(sliderMoved(int)),this,SLOT(setZ(int)));
  QObject::connect(this,SIGNAL(XvalueChanged(int)),__editor->XSlider,SLOT(setValue(int)));
  QObject::connect(this,SIGNAL(YvalueChanged(int)),__editor->YSlider,SLOT(setValue(int)));
  QObject::connect(this,SIGNAL(ZvalueChanged(int)),__editor->ZSlider,SLOT(setValue(int)));
  QObject::connect(__editor->OkButton,SIGNAL(clicked()),__sliders,SLOT(hide()));
  QObject::connect(__editor->CancelButton,SIGNAL(clicked()),__sliders,SLOT(hide()));
  QObject::connect(__editor->ClearButton,SIGNAL(clicked()),this,SLOT(init()));
  setSliderStep(1);
}


ViewRotCenterGL3::~ViewRotCenterGL3()
{
  if(__displayList)glDeleteLists(__displayList,1);
}


void 
ViewRotCenterGL3::setSliderStep(const int step)
{
  __editor->XSlider->setMaxValue(150*step);
  __editor->YSlider->setMaxValue(150*step);
  __editor->ZSlider->setMaxValue(150*step);
  __editor->XSlider->setMinValue(-150*step);
  __editor->YSlider->setMinValue(-150*step);
  __editor->ZSlider->setMinValue(-150*step);
  __editor->XSlider->setPageStep(step);
  __editor->YSlider->setPageStep(step);
  __editor->ZSlider->setPageStep(step);
  __editor->XSlider->setValue(int(__position.x()));
  __editor->YSlider->setValue(int(__position.y()));
  __editor->ZSlider->setValue(int(__position.z()));
  __editor->XEdit->setText(QString::number(__position.x()));
  __editor->YEdit->setText(QString::number(__position.y()));
  __editor->ZEdit->setText(QString::number(__position.z()));
}

/* ----------------------------------------------------------------------- */

bool
ViewRotCenterGL3::isVisible() const
{
  return __visible;
}

bool
ViewRotCenterGL3::isActive() const
{
  return __active;
}

int 
ViewRotCenterGL3::x() const
{
  return int(__position.x());
}

int 
ViewRotCenterGL3::y() const
{
  return int(__position.y());
}

int 
ViewRotCenterGL3::z() const
{
  return int(__position.z());
}

ViewDialog3 * 
ViewRotCenterGL3::getSliders() const
{
  return __sliders;
}

/* ----------------------------------------------------------------------- */

void 
ViewRotCenterGL3::init()
{
  if(__position != Vector3::ORIGIN){
    __position = Vector3::ORIGIN;
    emit XvalueChanged(int(__position.x()/getStep()));
    emit XvalueChanged(QString::number(__position.x()));
    emit YvalueChanged(int(__position.y()/getStep()));
    emit YvalueChanged(QString::number(__position.y()));
    emit ZvalueChanged(int(__position.z()/getStep()));
    emit ZvalueChanged(QString::number(__position.z()));
    emit valueChanged();
  }
}

void 
ViewRotCenterGL3::show()
{
  if(!__visible){
    if(!__sliders->isVisible())__sliders->show();
    __visible = true;
    emit visibilityChanged(__visible);
    emit valueChanged();
  }
}

void 
ViewRotCenterGL3::hide()
{
  if(__visible){
    if(__sliders->isVisible())__sliders->hide();
    __visible = false;
    emit visibilityChanged(__visible);
    emit valueChanged();
  }
}

void 
ViewRotCenterGL3::changeVisibility()
{
  __visible = ! __visible;
  if(__visible  && !__sliders->isVisible())__sliders->show();
  else if(!__visible && __sliders->isVisible())__sliders->hide();
  emit visibilityChanged(__visible);
  emit valueChanged();  
}

void 
ViewRotCenterGL3::changeVisibility(bool b)
{
  if(__visible != b){
    __visible = b;
    if(__visible && !__sliders->isVisible())__sliders->show();
    else if(!__visible && __sliders->isVisible())__sliders->hide();
    emit visibilityChanged(__visible);
    emit valueChanged();
  }
}

void 
ViewRotCenterGL3::activate()
{
  if(!__active){
    __active = true;
    emit activationChanged(__active);
    emit valueChanged();
  }
}

void ViewRotCenterGL3::center()
{
	if(__frame){
		ViewGLFrame3 * f = dynamic_cast<ViewGLFrame3 *>(__frame);
		if(f){
			// ViewGeomSceneGL3 * sc = dynamic_cast<ViewGeomSceneGL3 *>(f->getSceneRenderer());
			// if(sc){
				BoundingBoxPtr bbx = f->getSceneRenderer()->getGlobalBoundingBox();
				Vector3 center = bbx->getCenter();
				// center.z() = bbx->getLowerLeftCorner().z();
				__position = center;
				if(!__active)activate();
			    else emit valueChanged();
			// }
		}
	}
}

void 
ViewRotCenterGL3::desactivate()
{
  if(__active){
    __active = false;
    emit activationChanged(__active);
    emit valueChanged();
  }
}

void 
ViewRotCenterGL3::changeActivation()
{
  __active = ! __active;
  emit activationChanged(__active);
  emit valueChanged();
}

void 
ViewRotCenterGL3::changeActivation(bool b)
{
  if(__active != b){
    __active = b;
    emit activationChanged(__active);
    emit valueChanged();
  }
}

void 
ViewRotCenterGL3::setX(int x)
{
  __position.x() = x;
  emit XvalueChanged(QString::number(x/getStep()));
  emit valueChanged();
}

void 
ViewRotCenterGL3::setY(int y)
{
  __position.y() = y;
  emit YvalueChanged(QString::number(y/getStep()));
  emit valueChanged();
}

void 
ViewRotCenterGL3::setZ(int z)
{
  __position.z() = z;
  emit ZvalueChanged(QString::number(z/getStep()));
  emit valueChanged();
}

void 
ViewRotCenterGL3::setX(const QString& x)
{
  __position.x() = real_t(x.toInt());
  emit XvalueChanged(int(__position.x()));
  emit valueChanged();
  
}

void 
ViewRotCenterGL3::setY(const QString& y)
{
  __position.y() = real_t(y.toInt());
  emit YvalueChanged(int(__position.y()));
  emit valueChanged();
  
}
void 
ViewRotCenterGL3::setZ(const QString& z)
{
  __position.z() = real_t(z.toInt());
  emit ZvalueChanged(int(__position.z()));
  emit valueChanged();  
}

void 
ViewRotCenterGL3::initializeGL()
{
  if (__displayList) {
    glDeleteLists(__displayList,1);
  }

  __displayList = glGenLists(1);
  glNewList(__displayList,GL_COMPILE);
  GLfloat color[] =   { 1.0, 1.0, 0.0, 1.0 };
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
  glutSolidSphere(0.1,8,8);
  glColor3f(1,1,0);
  glBegin(GL_LINES);
  glVertex3f(0,0,0.2f);
  glVertex3f(0,0,-0.2f);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0,0.2f,0);
  glVertex3f(0,-0.2f,0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0.2f,0,0);
  glVertex3f(-0.2f,0,0);
  glEnd();
  glEndList();

  GEOM_GL_ERROR;
  
}

void 
ViewRotCenterGL3::paintGL()
{
  if(__active)
    glTranslatef(-__position.x(),-__position.y(),-__position.z());
  if(__visible){
    glPushMatrix();
    glTranslatef(__position.x(),__position.y(),__position.z());
    glScalef(float(getStep()),float(getStep()),float(getStep()));
    glCallList(__displayList);
    glPopMatrix();
  }
  if(__active || __visible)  GEOM_GL_ERROR;
}

void 
ViewRotCenterGL3::changeStepEvent(const int newStep, const int oldStep)
{
  real_t r = real_t(newStep)/real_t(oldStep);
  if(fabs(r)>GEOM_EPSILON)
    __position *= r;
  setSliderStep(newStep);
}

QPopupMenu * 
ViewRotCenterGL3::createToolsMenu(QWidget * parent)
{
  return new ViewRotCenterMenu3(this,parent,"Rotating Center Menu");
}

void 
ViewRotCenterGL3::fillToolBar(QToolBar * toolBar)
{
  QPixmap visible(ViewerIcon3::icon_rcvisible);
  QPixmap active(ViewerIcon3:: icon_rcactive);
  QPixmap centered(ViewerIcon3::icon_rccentered);
  QToolButton * __Visible = new QToolButton(visible, 
					    tr("Visible Rotating Center"), 
					    tr("Visible Rotating Center"),
					    this, SLOT(changeVisibility()), toolBar);
  __Visible->setToggleButton(true) ;
  QToolButton * __Active = new QToolButton(active, 
					    tr("Enable Rotating Center"), 
					    tr("Enable Rotating Center"),
					    this, SLOT(changeActivation()), toolBar);
  __Active->setToggleButton(true) ;
  QToolButton * __Centered = new QToolButton(centered, 
					    tr("Center Rotating Center"), 
					    tr("Center Rotating Center"),
					    this, SLOT(center()), toolBar);
  toolBar->addSeparator();
  QObject::connect(this,SIGNAL(visibilityChanged(bool)),
		   __Visible,SLOT(setOn(bool)));
  QObject::connect(this,SIGNAL(activationChanged(bool)),
		   __Active,SLOT(setOn(bool)));
  
}

/* ----------------------------------------------------------------------- */

ViewRotCenterMenu3::ViewRotCenterMenu3(ViewRotCenterGL3 * center, 
				       QWidget * parent, 
				       const char * name):
  QPopupMenu(parent,name)
{
  if(center){
    QPixmap wheel(ViewerIcon3::icon_wheel);
    QPixmap home(ViewerIcon3::icon_home);
    insertItem(home,tr("&Home"),center,SLOT(init()));
    insertSeparator();
    QPixmap visible(ViewerIcon3::icon_rcvisible);
    QPixmap active(ViewerIcon3::icon_rcactive);
    QPixmap centered(ViewerIcon3::icon_rccentered);
    idVisible = insertItem(visible,tr("&Visible"),center,SLOT(changeVisibility()));
    idActive  = insertItem(active,tr("&Enable"),center,SLOT(changeActivation()));
    insertItem(centered,tr("&Center"),center,SLOT(center()));
    insertSeparator();
    idControl = insertItem(wheel,tr("&Control"),center->getSliders(),SLOT(show()));
    setCheckable( TRUE );
    setItemChecked(idVisible,center->isVisible());
    setItemChecked(idActive,center->isActive());
    setItemChecked(idControl,center->getSliders()->isVisible());
    QObject::connect(center,SIGNAL(visibilityChanged(bool)),this,SLOT(setVisible(bool)));
    QObject::connect(center,SIGNAL(activationChanged(bool)),this,SLOT(setActive(bool)));
    QObject::connect(center->getSliders(),SIGNAL(visibilityChanged(bool)),this,SLOT(setControl(bool)));    
  }
}

ViewRotCenterMenu3::~ViewRotCenterMenu3()
{
}

void 
ViewRotCenterMenu3::setVisible(bool b)
{
   setItemChecked(idVisible,b);
}

void 
ViewRotCenterMenu3::setActive(bool b)
{
   setItemChecked(idActive,b);
}

void 
ViewRotCenterMenu3::setControl(bool b)
{
   setItemChecked(idControl,b);
}
