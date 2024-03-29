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


#include "rotcenter.h"
#include "interface/ui_rotcenteredit.h"
#include "icons.h"
#include "glframe.h"
#include "scenegl.h"
#include "util_qwidget.h"

#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/scenegraph/geometry/sphere.h>
#include <plantgl/algo/base/discretizer.h>
#include <plantgl/algo/opengl/glrenderer.h>
#include <plantgl/math/util_math.h>

#include <QtCore/qfile.h>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qlineedit.h>
    #include <QtWidgets/qslider.h>
    #include <QtWidgets/qpushbutton.h>
    #include <QtWidgets/qtoolbutton.h>
    #include <QtWidgets/qtoolbar.h>
    #include <QtWidgets/qdockwidget.h>
    #include <QtWidgets/qmainwindow.h>
#else
    #include <QtGui/qlineedit.h>
    #include <QtGui/qslider.h>
    #include <QtGui/qpushbutton.h>
    #include <QtGui/qtoolbutton.h>
    #include <QtGui/qtoolbar.h>
    #include <QtGui/qdockwidget.h>
    #include <QtGui/qmainwindow.h>
#endif

#include <QOpenGLWidget>
#include <plantgl/algo/opengl/util_glut.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

ViewRotCenterGL::ViewRotCenterGL(ViewCameraGL *camera,
                 QOpenGLBaseWidget * parent,
                 const char * name, PGLOpenGLFunctionsPtr ogl):
  ViewRelativeObjectGL(camera,parent,name, ogl),
  __active(false),
  __visible(false),
  __position(0,0,0),
  __displayList(0){
  QMainWindow * mwidget = dynamic_cast<QMainWindow *>(parent->parent());
  __sliders= new QDockWidget(mwidget);
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
    __ogl->glDeleteLists(__displayList,1);
  }

  GeometryPtr msphere(new Sphere(0.1,8,8));
  Discretizer d;
  GLRenderer renderer(d, NULL, __ogl);

  __displayList = __ogl->glGenLists(1);
  __ogl->glNewList(__displayList,GL_COMPILE);
  GLfloat color[] =   { 1.0, 1.0, 0.0, 1.0 };
  __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
  msphere->apply(renderer);
  __ogl->glColor3f(1,1,0);
  __ogl->glBegin(GL_LINES);
  __ogl->glVertex3f(0,0,0.2f);
  __ogl->glVertex3f(0,0,-0.2f);
  __ogl->glEnd();
  __ogl->glBegin(GL_LINES);
  __ogl->glVertex3f(0,0.2f,0);
  __ogl->glVertex3f(0,-0.2f,0);
  __ogl->glEnd();
  __ogl->glBegin(GL_LINES);
  __ogl->glVertex3f(0.2f,0,0);
  __ogl->glVertex3f(-0.2f,0,0);
  __ogl->glEnd();
  __ogl->glEndList();

  GEOM_GL_ERROR;

}

void
ViewRotCenterGL::paintGL()
{
  if(__active)
    __ogl->glTranslatef(-__position.x(),-__position.y(),-__position.z());
  if(__visible){
    __ogl->glPushMatrix();
    __ogl->glTranslatef(__position.x(),__position.y(),__position.z());
    __ogl->glScalef(float(getStep()),float(getStep()),float(getStep()));
    __ogl->glCallList(__displayList);
    __ogl->glPopMatrix();
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

