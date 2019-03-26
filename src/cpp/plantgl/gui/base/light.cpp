/* -*-c++-*-
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

#include "light.h"
#include "event.h"
#include "icons.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/math/util_math.h>
#include <plantgl/algo/opengl/util_glut.h>
#include <plantgl/algo/opengl/util_appegl.h>

#include <QtCore/qfile.h>
#include <QtCore/qstring.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/QToolBar>
#else
    #include <QtGui/QToolBar>
#endif

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

ViewLightGL::ViewLightGL(ViewCameraGL *camera,QGLWidget * parent, const char * name):
  ViewRelativeObjectGL(camera,parent,name),
  __azimuth(0),
  __elevation(45),
  __distance(15),
  __ambient(153,153,153),
  __diffuse(255,255,255),
  __specular(255,255,255),
  __show(false),
  __enable(true)
{
}


ViewLightGL::~ViewLightGL()
{
}

/* ----------------------------------------------------------------------- */

void
ViewLightGL::home()
{
  __azimuth = 0;
  __elevation = 45;
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}

void
ViewLightGL::XAxis()
{
  __azimuth = 0;
  __elevation = 0;
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}

void
ViewLightGL::YAxis()
{
  __azimuth = 90;
  __elevation = 0;
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}

void
ViewLightGL::ZAxis()
{
  __azimuth = 0;
  __elevation = 90;
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}

void
ViewLightGL::changeVisibility()
{
  __show = ! __show;
  emit visibilityChanged(__show);
  emit valueChanged();
  if(__show)status(tr("Light Source Visible"),5000);
  else status(tr("Light Source Invisible"),5000);
}

void
ViewLightGL::show()
{
  __show = true;
  emit visibilityChanged(__show);
  emit valueChanged();
  status(tr("Light Source Visible"),5000);
}

void
ViewLightGL::show(bool b)
{
  if(__show != b){
    __show = b;
    emit visibilityChanged(__show);
    emit valueChanged();
    if(__show)status(tr("Light Source Visible"),5000);
    else status(tr("Light Source Invisible"),5000);
  }
}

void
ViewLightGL::hide()
{
  __show = false;
  emit visibilityChanged(__show);
  emit valueChanged();
  status(tr("Light Source Invisible"),5000);
}


void
ViewLightGL::setAzimuth(double azimuth)
{
  if(__azimuth != azimuth){
    __azimuth = azimuth;
    emit azimuthChanged(__azimuth);
    emit valueChanged();
  }
}

void
ViewLightGL::setElevation(double elevation)
{
  if(__elevation != elevation){
    __elevation = elevation;
    emit elevationChanged(__elevation);
    emit valueChanged();
  }
}

void
ViewLightGL::setDistance(double distance)
{
  if(__distance != distance){
    __distance = distance;
    emit distanceChanged(__distance);
    emit valueChanged();
  }
}

void
ViewLightGL::setAzimuth(int azimuth)
{
  setAzimuth(double(azimuth));
}

void
ViewLightGL::setElevation(int elevation)
{
  setElevation(double(elevation));
}

void
ViewLightGL::setDistance(int distance)
{
  setDistance(double(distance));
}

void
ViewLightGL::setAmbient(const QColor& color)
{
    if( __ambient != color){
        __ambient = color;
        gllightMaterial();
        emit ambientChanged(__ambient);
        emit valueChanged();
    }
}

void
ViewLightGL::setDiffuse(const QColor& color)
{
    if( __diffuse != color){
        __diffuse = color;
        gllightMaterial();
        emit diffuseChanged(__diffuse);
        emit valueChanged();
    }
}

void
ViewLightGL::setSpecular(const QColor& color)
{
  if( __specular != color){
     __specular = color;
    gllightMaterial();
    emit specularChanged(__specular);
    emit valueChanged();
  }
}

void
ViewLightGL::moving(int dx, int dy)
{
  int r = __ambient.red()+dy;
  int g = __ambient.green()+dy;
  int b = __ambient.blue()+dy;
  if(r < 255 && r > 0 && g < 255 && g > 0 && b < 255 && b > 0 )
    setAmbient(QColor(r,g,b));
  emit ambientChanged(__ambient);
}

void
ViewLightGL::zooming(int dx, int dy)
{
  __distance += dy*getStep();
  emit distanceChanged(__distance);
}

void
ViewLightGL::rotating(int dx, int dy)
{
  __azimuth -=dx;
  if(__azimuth >= 180)
    __azimuth -= 360;
  if(__azimuth <= -180)
    __azimuth += 360;
  emit azimuthChanged(__azimuth);
  __elevation -= dy;
  if(__elevation >= 180)
    __elevation -= 360;
  if(__elevation <= -180)
    __elevation += 360;
  emit elevationChanged(__elevation);
}

void
ViewLightGL::initializeGL()
{
  gllightMaterial();
  Vector3 pos(Vector3::Spherical(__distance,__azimuth*GEOM_RAD,(__elevation)*GEOM_RAD));
  glGeomLightPosition(GL_LIGHT0,pos);
/*
  pos *= -1;
  pos.normalize();
  glGeomLightDirection(GL_LIGHT0,pos);
*/

}


void
ViewLightGL::changeStepEvent(double newStep, double oldStep)
{
  __distance *= float(newStep)/float(oldStep);
  emit distanceChanged(__distance);
}


void
ViewLightGL::paintGL()
{

  Vector3 pos(Vector3::Spherical(__distance,__azimuth*GEOM_RAD,(__elevation)*GEOM_RAD));
  if(fabs(pos.x()) < GEOM_EPSILON && fabs(pos.y()) < GEOM_EPSILON && fabs(pos.z()) < GEOM_EPSILON){
    pos = Vector3(0,0,1);
  }
   glGeomLightPosition(GL_LIGHT0,pos);
  Vector3 dir(pos);
  dir *= -1;
  dir.normalize();
  glGeomLightDirection(GL_LIGHT0,dir);

  switchOn();
  if(__show){
    glPushMatrix();
    glGeomTranslate(pos);
    glGeomMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, __ambient);
    glutSolidSphere(getStep()*0.1,8,8);
    glPopMatrix();
  }

  GEOM_GL_ERROR;
}

Vector3
ViewLightGL::getPosition() const{
  return Vector3(Vector3::Spherical(__distance,__azimuth*GEOM_RAD,(__elevation)*GEOM_RAD));
}

void ViewLightGL::setPosition(const Vector3& pos)
{
    Vector3::Spherical spos(pos);
    __distance = spos.radius;
    __azimuth = spos.theta * GEOM_DEG;
    __elevation = spos.phi * GEOM_DEG;

}

void
ViewLightGL::switchOn()
{
    if (__enable){
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }
    else {
        glDisable(GL_LIGHTING);
    }
}

void
ViewLightGL::switchOff()
{
  glDisable(GL_LIGHTING);
}

bool
ViewLightGL::isEnabled() const
{
  return __enable;
}

void
ViewLightGL::toggleEnabled()
{
  setEnabled(!__enable);
}

void
ViewLightGL::setEnabled(bool b)
{
   __enable = b;
  emit enabledChanged(__enable);
  emit valueChanged();
  if(__enable)status("Light Enable",5000);
  else status("Light Disable",5000);
}


void
ViewLightGL::gllightMaterial()
{
  glGeomLightMaterial(GL_LIGHT0, GL_AMBIENT,  __ambient);
  glGeomLightMaterial(GL_LIGHT0, GL_DIFFUSE,  __diffuse);
  glGeomLightMaterial(GL_LIGHT0, GL_SPECULAR, __specular);
}

QMenu *
ViewLightGL::createToolsMenu(QWidget * parent)
{
    QMenu * menu = new QMenu(parent);
    QPixmap home(ViewerIcon::getPixmap(ViewerIcon::home));
    QPixmap _light(ViewerIcon::getPixmap(ViewerIcon::light));
    menu->addAction(home,tr("&Home"),this,SLOT(home()),Qt::CTRL+Qt::SHIFT+Qt::Key_H);
    menu->addAction(tr("on X axis"),this,SLOT(XAxis()),Qt::CTRL+Qt::SHIFT+Qt::Key_X);
    menu->addAction(tr("on Y axis"),this,SLOT(YAxis()),Qt::CTRL+Qt::SHIFT+Qt::Key_Y);
    menu->addAction(tr("on Z axis"),this,SLOT(ZAxis()),Qt::CTRL+Qt::SHIFT+Qt::Key_Z);
    menu->addSeparator();
    QAction * idVisibility = menu->addAction(_light,tr("Visible"),this,SLOT(changeVisibility()),Qt::CTRL+Qt::SHIFT+Qt::Key_S);
    idVisibility->setCheckable( true );
    idVisibility->setChecked( isVisible() );
    QObject::connect(this,SIGNAL(visibilityChanged( bool)),idVisibility,SLOT(setChecked(bool)));
    menu->addSeparator();
    QAction * idLight = menu->addAction(_light,     tr("&Enabled"),     this, SLOT(toggleEnabled()));
    idLight->setCheckable(true);
    idLight->setChecked(isEnabled());
    idLight->setWhatsThis(tr("<b>Light Rendering</b><br><br>"
    "Set <b>Light Rendering</b> enable/disable.<br><br>"
    "The Rendering will (not) take into account ligth source.<br><br>"
    "You can also use Menu <br><b>Tools > Renderer > Light</b><br>"));
    return menu;
}

void
ViewLightGL::fillToolBar(QToolBar * toolBar)
{
    QPixmap _light(ViewerIcon::getPixmap(ViewerIcon::light));
    QAction * idLight = toolBar->addAction(_light,     tr("&Light"),     this, SLOT(toggleEnabled()));
    idLight->setCheckable(true);
    idLight->setChecked(isEnabled());
    idLight->setWhatsThis(tr("<b>Light Rendering</b><br><br>"
    "Set <b>Light Rendering</b> enable/disable.<br><br>"
    "The Rendering will (not) take into account ligth source.<br><br>"
    "You can also use Menu <br><b>Tools > Renderer > Light</b><br>"));
}


void ViewLightGL::lightEvent(ViewEvent * event)
{
    int etype = event->type();
    if (etype == ViewEvent::eLightSet) {
        ViewLightSetEvent * e = (ViewLightSetEvent *)event;
        switch(e->def){
            case ViewLightSetEvent::ePosition:
                setPosition(e->position);
            break;
            case ViewLightSetEvent::eActivation:
                setEnabled(e->activation);
            break;
            case ViewLightSetEvent::eAmbient:
                setAmbient(e->ambient);
            break;
            case ViewLightSetEvent::eDiffuse:
                setDiffuse(e->diffuse);
            break;
            case ViewLightSetEvent::eSpecular:
                setSpecular(e->specular);
            break;
        }
    }
    else if (etype == ViewEvent::eLightGet){
        ViewLightGetEvent * e = (ViewLightGetEvent *)event;
        switch(e->def){
            case ViewLightSetEvent::ePosition:
                *e->position = getPosition();
            break;
            case ViewLightSetEvent::eActivation:
                *e->activation = isEnabled();
            break;
            case ViewLightSetEvent::eAmbient:
                *e->ambient = __ambient;
            break;
            case ViewLightSetEvent::eDiffuse:
                *e->diffuse = __diffuse;
            break;
            case ViewLightSetEvent::eSpecular:
                *e->specular = __specular;
            break;
        }
    }

}

