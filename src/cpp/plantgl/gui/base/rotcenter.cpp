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
#include <qdockwidget.h>
#include <qmainwindow.h>
#include <plantgl/algo/opengl/util_glut.h>

TOOLS_USING_NAMESPACE
PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

ViewRotCenterGL::ViewRotCenterGL(ViewCameraGL *camera,
				 QGLWidget * parent, 
				 const char * name):
  ViewRelativeObjectGL(camera,parent,name),
  __active(false),
  __visible(false),
  __position(0,0,0),
  __displayList(0){
  QMainWindow * mwidget = dynamic_cast<QMainWindow *>(parent->parent());
  __sliders= new QDockWidget(mwidget,false);
  __sliders->setObjectName("RotCenterSliders");
  __sliders->setWindowTitle(tr("Rotating Center Coordinates"));
  QWidget * widgetContent = new QWidget(__sliders);
  __sliders->setWidget(widgetContent);
  __editor = new Ui::RotCenterEdit();
  __editor->setupUi(widgetContent);
  __editor->XEdit->setRange(-REAL_MAX,REAL_MAX);
  __editor->YEdit->setRange(-REAL_MAX,REAL_MAX);
  __editor->ZEdit->setRange(-REAL_MAX,REAL_MAX);

  __editor->XEdit->setValue(__position.x());
  __editor->YEdit->setValue(__position.y());
  __editor->ZEdit->setValue(__position.z());
  QObject::connect(__editor->XEdit,SIGNAL(valueChanged(double)),this,SLOT(setX(double)));
  QObject::connect(__editor->YEdit,SIGNAL(valueChanged(double)),this,SLOT(setY(double)));
  QObject::connect(__editor->ZEdit,SIGNAL(valueChanged(double)),this,SLOT(setZ(double)));
  QObject::connect(this,SIGNAL(XvalueChanged(double)),__editor->XEdit,SLOT(setValue(double)));
  QObject::connect(this,SIGNAL(YvalueChanged(double)),__editor->YEdit,SLOT(setValue(double)));
  QObject::connect(this,SIGNAL(ZvalueChanged(double)),__editor->ZEdit,SLOT(setValue(double)));
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

  mwidget->addDockWidget(Qt::BottomDockWidgetArea,__sliders);
  __sliders->setVisible(false);
}


ViewRotCenterGL::~ViewRotCenterGL()
{
  if(__displayList)glDeleteLists(__displayList,1);
}


void 
ViewRotCenterGL::setSliderStep(const int step)
{
  __editor->XSlider->setRange(-150*step,150*step);
  __editor->YSlider->setRange(-150*step,150*step);
  __editor->ZSlider->setRange(-150*step,150*step);
  __editor->XSlider->setPageStep(step);
  __editor->YSlider->setPageStep(step);
  __editor->ZSlider->setPageStep(step);
  __editor->XSlider->setValue(int(__position.x()));
  __editor->YSlider->setValue(int(__position.y()));
  __editor->ZSlider->setValue(int(__position.z()));
  __editor->XEdit->setValue(__position.x());
  __editor->YEdit->setValue(__position.y());
  __editor->ZEdit->setValue(__position.z());
}

/* ----------------------------------------------------------------------- */

bool
ViewRotCenterGL::isVisible() const
{
  return __visible;
}

bool
ViewRotCenterGL::isActive() const
{
  return __active;
}

int 
ViewRotCenterGL::x() const
{
  return int(__position.x());
}

int 
ViewRotCenterGL::y() const
{
  return int(__position.y());
}

int 
ViewRotCenterGL::z() const
{
  return int(__position.z());
}

QDockWidget * 
ViewRotCenterGL::getSliders() const
{
  return __sliders;
}

/* ----------------------------------------------------------------------- */

void 
ViewRotCenterGL::init()
{
  if(__position != Vector3::ORIGIN){
    __position = Vector3::ORIGIN;
	float step = getStep();
    emit XvalueChanged(int(__position.x()/step));
    emit XvalueChanged(__position.x());
    emit YvalueChanged(int(__position.y()/step));
    emit YvalueChanged(__position.y());
    emit ZvalueChanged(int(__position.z()/step));
    emit ZvalueChanged(__position.z());
    emit valueChanged();
  }
}

void 
ViewRotCenterGL::show()
{
  if(!__visible){
    if(!__sliders->isVisible())__sliders->show();
    __visible = true;
    emit visibilityChanged(__visible);
    emit valueChanged();
  }
}

void 
ViewRotCenterGL::hide()
{
  if(__visible){
    if(__sliders->isVisible())__sliders->hide();
    __visible = false;
    emit visibilityChanged(__visible);
    emit valueChanged();
  }
}

void 
ViewRotCenterGL::changeVisibility()
{
  __visible = ! __visible;
  if(__visible  && !__sliders->isVisible())__sliders->show();
  else if(!__visible && __sliders->isVisible())__sliders->hide();
  emit visibilityChanged(__visible);
  emit valueChanged();  
}

void 
ViewRotCenterGL::changeVisibility(bool b)
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
ViewRotCenterGL::activate()
{
  if(!__active){
    __active = true;
    emit activationChanged(__active);
    emit valueChanged();
  }
}

void ViewRotCenterGL::center()
{
	if(__frame){
		ViewGLFrame * f = dynamic_cast<ViewGLFrame *>(__frame);
		if(f){
			BoundingBoxPtr bbx = f->getSceneRenderer()->getGlobalBoundingBox();
			Vector3 center = bbx->getCenter();
			__position = center;
			if(!__active)activate();
			else emit valueChanged();
		}
	}
}

void 
ViewRotCenterGL::desactivate()
{
  if(__active){
    __active = false;
    emit activationChanged(__active);
    emit valueChanged();
  }
}

void 
ViewRotCenterGL::changeActivation()
{
  __active = ! __active;
  emit activationChanged(__active);
  emit valueChanged();
}

void 
ViewRotCenterGL::changeActivation(bool b)
{
  if(__active != b){
    __active = b;
    emit activationChanged(__active);
    emit valueChanged();
  }
}

void 
ViewRotCenterGL::setX(int x)
{
  __position.x() = x;
  emit XvalueChanged(double(x));
  emit valueChanged();
}

void 
ViewRotCenterGL::setY(int y)
{
  __position.y() = y;
  emit YvalueChanged(double(y));
  emit valueChanged();
}

void 
ViewRotCenterGL::setZ(int z)
{
  __position.z() = z;
  emit ZvalueChanged(double(z));
  emit valueChanged();
}

void 
ViewRotCenterGL::setX(double x)
{
  __position.x() = x;
  emit XvalueChanged(int(__position.x()));
  emit valueChanged();
  
}

void 
ViewRotCenterGL::setY(double y)
{
  __position.y() = y;
  emit YvalueChanged(int(__position.y()));
  emit valueChanged();
  
}
void 
ViewRotCenterGL::setZ(double z)
{
  __position.z() = z;
  emit ZvalueChanged(int(__position.z()));
  emit valueChanged();  
}

void 
ViewRotCenterGL::initializeGL()
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
ViewRotCenterGL::paintGL()
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
ViewRotCenterGL::changeStepEvent(double newStep, double oldStep)
{
  real_t r = real_t(newStep)/real_t(oldStep);
  if(fabs(r)>GEOM_EPSILON)
    __position *= r;
  setSliderStep(newStep);
}

QMenu * 
ViewRotCenterGL::createToolsMenu(QWidget * parent)
{
	QMenu * menu = new QMenu(parent);
	QPixmap wheel(ViewerIcon::getPixmap(ViewerIcon::wheel));
    QPixmap home(ViewerIcon::getPixmap(ViewerIcon::home));
    menu->addAction(home,tr("&Home"),this,SLOT(init()));
	menu->addSeparator();

    QPixmap visible(ViewerIcon::getPixmap(ViewerIcon::rcvisible));
    QPixmap active(ViewerIcon::getPixmap(ViewerIcon::rcactive));
    QPixmap centered(ViewerIcon::getPixmap(ViewerIcon::rccentered));

    QAction * idVisible = menu->addAction(visible,tr("&Visible"),this,SLOT(changeVisibility()));
    idVisible->setCheckable( true );
    idVisible->setChecked( isVisible() );
    QObject::connect(this,SIGNAL(visibilityChanged(bool)),idVisible,SLOT(setChecked(bool)));

    QAction * idActive  = menu->addAction(active,tr("&Enable"),this,SLOT(changeActivation()));
    idActive->setCheckable( true );
    idActive->setCheckable( isActive() );
    QObject::connect(this,SIGNAL(activationChanged(bool)),idActive,SLOT(setChecked(bool)));

	menu->addSeparator();
    menu->addAction(centered,tr("&Center"),this,SLOT(center()));
    //QAction * idControl = menu->addAction(wheel,tr("&Control"),getSliders(),SLOT(show()));
    //idControl->setCheckable( true );
    //idControl->setChecked( getSliders()->isVisible() );
    //QObject::connect(getSliders(),SIGNAL(visibilityChanged(bool)),idControl,SLOT(setChecked(bool)));    

    QAction * idControl = getSliders()->toggleViewAction();
    menu->addAction(idControl);
    idControl->setText("Rot. Center &Control");
    idControl->setIcon(wheel);



    return menu;
}

void 
ViewRotCenterGL::fillToolBar(QToolBar * toolBar)
{
  QPixmap visible(ViewerIcon::getPixmap(ViewerIcon::rcvisible));
  QPixmap active(ViewerIcon::getPixmap(ViewerIcon::rcactive));
  QPixmap centered(ViewerIcon::getPixmap(ViewerIcon::rccentered));
  QAction * __Visible = toolBar->addAction(visible, tr("Visible Rotating Center"),this, SLOT(changeVisibility()));
  __Visible->setCheckable(true) ;
  QObject::connect(this,SIGNAL(visibilityChanged(bool)), __Visible,SLOT(setChecked(bool)));

  QAction * __Active = toolBar->addAction(active, tr("Enable Rotating Center"),this, SLOT(changeActivation()));
  __Active->setCheckable(true) ;
  QObject::connect(this,SIGNAL(activationChanged(bool)), __Active,SLOT(setChecked(bool)));

  QAction * __Centered = toolBar->addAction(centered, tr("Center Rotating Center"), this, SLOT(center()));
  toolBar->addSeparator();  
}

/* ----------------------------------------------------------------------- */

