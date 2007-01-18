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

#include "view_light.h"
#include "view_icons.h"
#include "Tools/util_vector.h"
#include "Tools/util_math.h"
#include "Tools/util_glut.h"
#include "util_appegl.h"
#include <qfile.h>
#include <qstring.h>

TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

ViewLightMenu::ViewLightMenu(ViewLightGL * light,QWidget * parent, const char * name):
  QPopupMenu(parent,name)
{
  if(light){
    QPixmap home(ViewerIcon::icon_home);
    QPixmap _light(ViewerIcon::icon_light);
    insertItem(home,tr("&Home"),light,SLOT(home()),CTRL+SHIFT+Key_H);
    insertItem(tr("on X axis"),light,SLOT(XAxis()),CTRL+SHIFT+Key_X);
    insertItem(tr("on Y axis"),light,SLOT(YAxis()),CTRL+SHIFT+Key_Y);
    insertItem(tr("on Z axis"),light,SLOT(ZAxis()),CTRL+SHIFT+Key_Z);
    insertSeparator();
    idVisibility = insertItem(_light,tr("Visible"),light,SLOT(changeVisibility()),CTRL+SHIFT+Key_S);
    setCheckable( TRUE );
    setItemChecked(idVisibility,light->isVisible());
    QObject::connect(light,SIGNAL(visibilityChanged(const bool)),this,SLOT(setVisibility(const bool)));
  }
}

ViewLightMenu::~ViewLightMenu()
{
}

void 
ViewLightMenu::setVisibility(const bool b)
{
  setItemChecked(idVisibility,b);
  
}

/* ----------------------------------------------------------------------- */

ViewLightGL::ViewLightGL(ViewCameraGL *camera,QGLWidget * parent, const char * name):
  ViewRelativeObjectGL(camera,parent,name),
  __azimuth(0),
  __elevation(45),
  __distance(15),
  __ambient(153,153,153),
  __diffuse(255,255,255),
  __specular(255,255,255),
  __show(false)
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
  __ambient = color;
  gllightMaterial();
  emit valueChanged();
}

void 
ViewLightGL::setDiffuse(const QColor& color)
{
  __diffuse = color;
  gllightMaterial();
  emit valueChanged();
}

void 
ViewLightGL::setSpecular(const QColor& color)
{
  __specular = color;
  gllightMaterial();
  emit valueChanged();  
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
ViewLightGL::changeStepEvent(const int newStep, const int oldStep)
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

  enable();
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

void 
ViewLightGL::enable()
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);  
}

void 
ViewLightGL::disable()
{
  glDisable(GL_LIGHTING);
}

void 
ViewLightGL::gllightMaterial()
{
  glGeomLightMaterial(GL_LIGHT0, GL_AMBIENT,  __ambient);
  glGeomLightMaterial(GL_LIGHT0, GL_DIFFUSE,  __diffuse);
  glGeomLightMaterial(GL_LIGHT0, GL_SPECULAR, __specular);
}

QPopupMenu *
ViewLightGL::createToolsMenu(QWidget * parent)
{
  return new ViewLightMenu(this,parent);
}
