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

#include <algo/opengl/util_gl.h>
#include <algo/opengl/util_glut.h>
#include <math/util_math.h>
#include <gui3/viewer/util_qstring.h>

#include <qtextstream.h>
#include <qtabwidget.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qfile.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qfiledialog.h>
#include <qwhatsthis.h>
#include <qmessagebox.h>
#include <qbitmap.h>

#include "filemanager.h"
#include "camera.h"
#include "interface/cameraqt.h"
#include "interface/cameraqt2.h"
#include "icons.h"
#include "glframe.h"
#include "event.h"

#include <math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */
ViewCameraMenu3::ViewCameraMenu3(ViewCameraGL3 * camera,QWidget * parent, const char * name):
  QPopupMenu(parent,name)
{
  QPixmap home(ViewerIcon3::getPixmap(ViewerIcon3::icon_home));
  QPixmap read(ViewerIcon3::getPixmap(ViewerIcon3::icon_fileopen));
  QPixmap save(ViewerIcon3::getPixmap(ViewerIcon3::icon_filefloppy));
  QPixmap persp(ViewerIcon3::getPixmap(ViewerIcon3::icon_perspective));
  QPixmap ortho(ViewerIcon3::getPixmap(ViewerIcon3::icon_orthographic));
  QPixmap linked(ViewerIcon3::getPixmap(ViewerIcon3::icon_linked));
  QPixmap eyef(ViewerIcon3::getPixmap(ViewerIcon3::icon_eyef));

  insertItem(home,tr("&Home"),       camera,SLOT(home()),CTRL+Key_H);
  insertItem(tr("&Front View (YZ)"),camera,SLOT(YZView()));
  insertItem(tr("&Right View (XZ)"),camera,SLOT(XZView()));
  insertItem(tr("&Top View (XY)"),camera,SLOT(XYView()));
  insertSeparator();
  __coordMenu = new QPopupMenu(this);
  idGEOMcoord = __coordMenu->insertItem(tr("GEOM System"),   camera,SLOT(setGEOMCoordSys()));
  idGLcoord   = __coordMenu->insertItem(tr("GL System"),      camera,SLOT(setGLCoordSys()));
  idchgcoord  = __coordMenu->insertItem(tr("&Change"),       camera,SLOT(changeCoordSys()));
  __coordMenu->setCheckable( TRUE );
  setCoordSys(camera->getCoordSys());
  insertItem(tr("Coordinates System"),__coordMenu);
  insertSeparator();
  __projectionMenu = new QPopupMenu(this);
  idPerspective = __projectionMenu->insertItem(persp,tr("&Perspective"),  camera,SLOT(setPerspectiveMode()));
  idOrtho       = __projectionMenu->insertItem(ortho,tr("Ort&hographic"), camera,SLOT(setOrthographicMode()));
  idchgProj     = __projectionMenu->insertItem(tr("&Change"),       camera,SLOT(changeCameraMode()),CTRL+Key_D);
  __projectionMenu->setCheckable( TRUE );
  insertItem(tr("Projection"),__projectionMenu);
  setProjectionMode(camera->getProjectionMode());
  QObject::connect(camera,SIGNAL(projectionChanged(bool)),this,SLOT(setProjectionMode(bool)));
  QObject::connect(camera,SIGNAL(coordSysChanged(int)),this,SLOT(setCoordSys(int)));
  insertSeparator();
  insertItem(save,tr("&Save"),       camera,SLOT(save()));
  insertItem(read,tr("&Read"),       camera,SLOT(read()));
  insertSeparator();
  idLock = insertItem(linked,tr("&Auto Fit to Window"),       camera,SLOT(lockDim()));
  if(camera)setItemChecked(idLock, !camera->isDimLock());
  QObject::connect(camera,SIGNAL(lockDimChanged(bool)),this,SLOT(setLock(bool)));
  insertItem(eyef,tr("&Fit to Window"),       camera,SLOT(reDim()));
}

ViewCameraMenu3::~ViewCameraMenu3()
{
}

void
ViewCameraMenu3::setProjectionMode(bool b)
{
  __projectionMenu->setItemChecked(idPerspective,b );
  __projectionMenu->setItemChecked(idOrtho,!b);
  __projectionMenu->setItemChecked(idchgProj,false);
}

void
ViewCameraMenu3::setCoordSys(int b)
{
  __coordMenu->setItemChecked(idGEOMcoord,(b ==1));
  __coordMenu->setItemChecked(idGLcoord,(b ==0));
  __coordMenu->setItemChecked(idchgcoord,false);
}

void 
ViewCameraMenu3::setLock(bool b)
{
  if(isItemChecked(idLock) == b)
  setItemChecked(idLock,!b );
}

/* ----------------------------------------------------------------------- */


ViewCameraGL3::ViewCameraGL3(QGLWidget * parent, const char * name) :
  ViewObjectGL3(parent,name),
  __azimuth(0),
  __elevation(0),
  __stepMove(1),
  __center(Vector3::ORIGIN),
  __eye(Vector3::OX),
  __translation(Vector3::ORIGIN),
  __radius(2),
  __far_plane(1000),
  __near_plane(1.1),
  __default_view_angle(30),
  __view_angle(30),
  __width(500),
  __height(500),
  __projectionmode(true),
  __geomsys(true),
  __lockdim(false)
{
}

ViewCameraGL3::ViewCameraGL3(QObject * parent, const char * name) :
  ViewObjectGL3(parent,name),
  __azimuth(0),
  __elevation(0),
  __stepMove(1),
  __center(Vector3::ORIGIN),
  __eye(Vector3::OX),
  __translation(Vector3::ORIGIN),
  __radius(2),
  __far_plane(1000),
  __near_plane(1.1),
  __default_view_angle(30),
  __view_angle(30),
  __width(500),
  __height(500),
  __projectionmode(true),
  __geomsys(true)
{
}

ViewCameraGL3::~ViewCameraGL3()
{
}

void ViewCameraGL3::home()
{
  __azimuth = 0;
  __elevation = 0;
  __translation = Vector3::ORIGIN;
  glInitProjectionMatrix();
//  emit azimuthChanged(QString::number(__azimuth));
//  emit elevationChanged(QString::number(__elevation));
//  emit zoomChanged(QString::number(__translation.x()));
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit zoomChanged(__translation.x());
  emit valueChanged();
}

void ViewCameraGL3::XYView()
{
  __azimuth = 0;
  __elevation = 90;
//  emit azimuthChanged(QString::number(__azimuth));
//  emit elevationChanged(QString::number(__elevation));
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}

void ViewCameraGL3::XZView()
{
  __azimuth = 90;
  __elevation = 0;
//  emit azimuthChanged(QString::number(__azimuth));
//  emit elevationChanged(QString::number(__elevation));
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}

void ViewCameraGL3::YZView()
{
  __azimuth = 0;
  __elevation = 0;
//  emit azimuthChanged(QString::number(__azimuth));
//  emit elevationChanged(QString::number(__elevation));
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}


void ViewCameraGL3::nextView()
{
  switch((int)__azimuth){
  case 90:
	__elevation = 0;
	__azimuth = 180;
	break;
  case 180:
	__elevation = 0;
	__azimuth = -90;
	break;
  case -90:
	__elevation = 90;
	__azimuth = 0;
	break;
  case 0:
	if(__elevation == 90)
	  __elevation = -90;
	else if(__elevation == -90){
	  __elevation = 0;
	}
	else if(__elevation == 0){
	  __azimuth = 90;
	}
	else __elevation = 0;
	break;
  default:
	__elevation = 0;
	__azimuth = 0;
	break;
  }
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}

void ViewCameraGL3::setAngles(double azimuth, double elevation)
{
	__azimuth = azimuth;
	__elevation = elevation;
}

void
ViewCameraGL3::setAzimuth(double azimuth)
{
  if(__azimuth != azimuth){
	__azimuth = azimuth;
//	emit azimuthChanged(QString::number(__azimuth));
	emit azimuthChanged(__azimuth);
	emit valueChanged();
  }
}

void
ViewCameraGL3::setElevation(double elevation)
{
  if(__elevation != elevation){
	__elevation = elevation;
//	emit elevationChanged(QString::number(__elevation));
	emit elevationChanged(__elevation);
	emit valueChanged();
  }
}

void
ViewCameraGL3::setZoom(double zoom)
{
  if(__translation.x() != zoom){
	__translation.x() = zoom;
	emit zoomChanged(__translation.x());
	emit valueChanged();
  }
}


void
ViewCameraGL3::setAzimuth(int azimuth)
{
  if(__azimuth != azimuth){
	__azimuth = azimuth;
//	emit azimuthChanged(QString::number(__azimuth));
	emit azimuthChanged(__azimuth);
	emit valueChanged();
  }
}

void
ViewCameraGL3::setElevation(int elevation)
{
  if(__elevation != elevation){
	__elevation = elevation;
//	emit elevationChanged(QString::number(__elevation));
	emit elevationChanged(__elevation);
	emit valueChanged();
  }
}

void
ViewCameraGL3::setZoom(int zoom)
{
  if(__translation.x() != zoom){
	__translation.x() = zoom;
	emit zoomChanged(__translation.x());
	emit valueChanged();
  }
}

/*
void
ViewCameraGL3::setAzimuth(const QString& azimuth)
{
  setAzimuth(double(azimuth.toInt()));
}

void
ViewCameraGL3::setElevation(const QString& elevation)
{
  setElevation(double(elevation.toInt()));
}

void
ViewCameraGL3::setZoom(const QString& zoom)
{
  __translation.x() = zoom.toInt();
}
*/
void
ViewCameraGL3::setViewAngle(const QString& angle)
{
  __default_view_angle = (double)angle.toInt();
}

void
ViewCameraGL3::setFarPlane(const QString& plane)
{
  __far_plane = (double)plane.toFloat();
  emit farPlaneChanged(QString::number(__far_plane));
}

void
ViewCameraGL3::setNearPlane(const QString& plane)
{
  __near_plane = (double)plane.toFloat();
  emit nearPlaneChanged(QString::number(__near_plane));
}

void
ViewCameraGL3::validViewAngle()
{
  initialize(__radius,__center);
  glInitProjectionMatrix();
  emit viewAngleChanged(QString::number(__default_view_angle));
  emit currentViewAngleChanged(QString::number(__view_angle));
  emit valueChanged();
}

void
ViewCameraGL3::validValue(){
  glInitProjectionMatrix();
  emit valueChanged();
}

void
ViewCameraGL3::setStepMove(const QString& step)
{

  setStepMove(step.toInt());
}

void
ViewCameraGL3::setStepMove(int step)
{
  __stepMove = step;
}

void
ViewCameraGL3::setDistanceX(const double& dist)
{
  __translation.y() = dist;
  emit valueChanged();
}

void
ViewCameraGL3::setDistanceY(const double& dist)
{
  __translation.z() = dist;
  emit valueChanged();
}

void
ViewCameraGL3::setCenter(const Vector3& center)
{
  __center = center;
  emit valueChanged();
}


void
ViewCameraGL3::moving(int dx, int dy)
{
  double ratio = 0.05;
  __translation.y() -= ratio *__stepMove*dx;
  __translation.z() += ratio *__stepMove*dy;
}

void
ViewCameraGL3::zooming(int dx, int dy)
{
  __translation.x() += dy*__stepMove;
  glInitProjectionMatrix();
  emit zoomChanged(__translation.x());
// emit zoomChanged(QString::number(__translation.x()));
}


void
ViewCameraGL3::rotating(int dx, int dy)
{
  __azimuth +=(float)dx; // /4.0f;
  if(__azimuth >= 180)
    __azimuth -= 360;
  if(__azimuth <= -180)
    __azimuth += 360;
//  emit azimuthChanged(QString::number(__azimuth));
  emit azimuthChanged(__azimuth);
  __elevation -= (float)dy; // /4.0f;
  if(__elevation >= 180)
    __elevation -= 360;
  if(__elevation <= -180)
    __elevation += 360;
//  emit elevationChanged(QString::number(__elevation));
  emit elevationChanged(__elevation);
}

void
ViewCameraGL3::setViewAngle(double angle)
{
  __default_view_angle = angle;
  initialize(__radius,__center);
  glInitProjectionMatrix();
  emit viewAngleChanged(QString::number(__default_view_angle));
  emit currentViewAngleChanged(QString::number(__view_angle));
  emit valueChanged();
}


void
ViewCameraGL3::initializeGL()
{
  BoundingBoxPtr bbox(0);
  initialize(bbox);
}

static bool first = true;
void
ViewCameraGL3::buildCamera(const BoundingBoxPtr& bbox, bool force)
{
   if(!__lockdim || force || first){
	initialize(bbox);
    glInitProjectionMatrix();
	first = false;
   }
}



void
ViewCameraGL3::initialize(const BoundingBoxPtr& bbox)
{
  double radius = 2;
  Vector3 center(0,0,0);
  if(bbox){
    Vector3 size = bbox->getSize();
    radius = norm(Vector2(size.y(),size.z()));
	center = bbox->getCenter();
  }
  initialize(radius,center);
}

void
ViewCameraGL3::initialize(const double& _radius,const Vector3& center)
{

  double _default_far_plane = 200;
  __view_angle = __default_view_angle;

  __radius = _radius;
  __center = center;

  double dist = (__radius)/tan((__default_view_angle/2)*GEOM_RAD);

  // distance of the eye to the center of the scene:
  __eye = Vector3(dist,__center.y(),__center.z());

  // we try to keep a ratio far plane / near plane less than 1000.
  // (cf FAQ MESA3D http/www.mesa3d.org)
  __far_plane = 12 * __radius;
  if (__far_plane < _default_far_plane)
      __far_plane = _default_far_plane;
  __near_plane = __far_plane / 800;

  /// compute the view_angle according to the size of the frame.
  /// For 500x500 window, the view_angle is __default_view_angle (60)
  /// Else view_angle is recomputed (OpenGL programming guide, second edition v1.1 p131)
  /// to see a bigger part of the scene but at the same scale.
  double rad = __radius * __height/500 ;
  __view_angle = (2.0*atan2(rad,dist))*GEOM_DEG;

  int newstep = int(__radius/10);
  if(newstep < 1)newstep =1;
  if(newstep != __stepMove && newstep !=0){
    double r = (double)newstep / (double)__stepMove;
    __translation *= r;
    __stepMove=newstep;
    emit(stepMoveChanged(__stepMove));
    emit zoomChanged(__translation.x());
  }
}

void
ViewCameraGL3::resizeGL(int w, int h)
{
  __width = w;
  __height = h;

  glViewport( 0, 0, (GLint)w, (GLint)h );

  double dist = __eye.x();
  double rad  = __radius;
  rad *= __height / 500;
  __view_angle = (2.0*atan2(rad,dist))*GEOM_DEG;

  glInitProjectionMatrix();

}

void
ViewCameraGL3::beginSelectGL(const QPoint& point)
{
  beginSelectGL(point.x(),point.y());
}

void
ViewCameraGL3::beginSelectGL(int x, int y)
{
  GEOM_GL_ERROR;

  GLint viewport[4];

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glGetIntegerv(GL_VIEWPORT, viewport);
  gluPickMatrix((GLdouble)x,(GLdouble)viewport[3]-y,2.0,2.0,viewport);
  glPushProjectionMatrix();

  glInitModelViewMatrix();

  GEOM_GL_ERROR;

}

void 
ViewCameraGL3::beginSelectGL(const QRect& region)
{
  GEOM_GL_ERROR;

  GLint viewport[4];
  int x = region.center().x();
  int y = region.center().y();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glGetIntegerv(GL_VIEWPORT, viewport);
  gluPickMatrix((GLdouble)x,(GLdouble)viewport[3]-y,
#ifdef _MSC_VER
				(GLdouble)max(region.width(),2),
				(GLdouble)max(region.height(),2),viewport);
#else
				(GLdouble)std::max(region.width(),2),
				(GLdouble)std::max(region.height(),2),viewport);
#endif
  glPushProjectionMatrix();

  glInitModelViewMatrix();

  GEOM_GL_ERROR;

}

void
ViewCameraGL3::endSelectGL()
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}


void
ViewCameraGL3::paintGL()
{
        glInitProjectionMatrix();
        glInitModelViewMatrix();
}

void
ViewCameraGL3::glInitModelViewMatrix()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushModelViewMatrix();
}

void
ViewCameraGL3::glPushModelViewMatrix()
{
  if( __geomsys){ /// Geom Sys Coordinates
                glGeomTranslate(__translation);
                glGeomRotate(Vector3::OY,__elevation);
                glGeomRotate(Vector3::OZ,-__azimuth);
  }
  else { /// GL Coordinates
                glGeomTranslate(geom2gl(__translation));
				glGeomRotate(Vector3::OX,__elevation);
                glGeomRotate(Vector3::OY,-__azimuth);
  }
  GEOM_GL_ERROR;
}

void
ViewCameraGL3::glInitProjectionMatrix()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glPushProjectionMatrix();
}

void
ViewCameraGL3::glPushProjectionMatrix()
{
  double dep(0);
  if(__translation.x() < -((__far_plane-__near_plane)/2))
    dep = __translation.x()+((__far_plane-__near_plane)/2) ;

  if(__projectionmode){
    gluPerspective(__view_angle,__width/__height,__near_plane-dep,__far_plane-dep);

 
  }
  else {
    double l = (__eye.x()-__translation.x())*tan((__view_angle/2)*GEOM_RAD);
    if(l<1)l=1;
    double l2 = l*__width/__height;
    if(l2<1)l2=1;


    double left =  - l2 ;
    double rigth = l2;
    double bottom = - l;
    double top = l;

    glOrtho(left,rigth,bottom,top,__near_plane-dep,__far_plane-dep);
  }

  // passage du repere GL dans repere labo
  if( __geomsys) glGeomLookAt(__eye,__center,Vector3::OZ);
  else           glGeomLookAt(geom2gl(__eye),geom2gl(__center),Vector3::OY);

  GEOM_GL_ERROR;
}


void
ViewCameraGL3::setPerspectiveMode()
{
  if(!__projectionmode){
    status(tr("Perspective Camera"),5000);
    __projectionmode = true;
    glInitProjectionMatrix();
    emit projectionChanged(__projectionmode);
    emit valueChanged();
  }
}

void
ViewCameraGL3::setOrthographicMode()
{
  if(__projectionmode){
    status(tr("Orthographic Camera"),5000);
    __projectionmode = false;
    glInitProjectionMatrix();
    emit projectionChanged(__projectionmode);
    emit valueChanged();
  }
}

void
ViewCameraGL3::changeCameraMode()
{
  __projectionmode = ! __projectionmode;
  if( __projectionmode)status(tr("Perspective Camera"),5000);
  else status(tr("Orthographic Camera"),5000);
  glInitProjectionMatrix();
  emit projectionChanged(__projectionmode);
  emit valueChanged();
}

void
ViewCameraGL3::setProjectionMode(int i){
        setProjectionMode(i != 0);
}

void
ViewCameraGL3::setProjectionMode(bool b)
{
  if(__projectionmode != b){
    __projectionmode = b;
    if( __projectionmode)status(tr("Perspective Camera"),5000);
    else status(tr("Orthographic Camera"),5000);
    glInitProjectionMatrix();
    emit projectionChanged(__projectionmode);
    emit valueChanged();
  }
}

void
ViewCameraGL3::changeCoordSys()
{
  __geomsys = ! __geomsys;
  if( __geomsys)status(tr("GEOM Coordinates System"),5000);
  else status(tr("GL Coordinates System"),5000);
  glInitProjectionMatrix();
  emit coordSysChanged(__geomsys);
  emit valueChanged();
}

void
ViewCameraGL3::setCoordSys(int i ){
        setCoordSys(i != 0);
}

void
ViewCameraGL3::setCoordSys(bool b)
{
  if( __geomsys != b){
    __geomsys = b;
    if( __geomsys)status(tr("GEOM Coordinates System"),5000);
    else status(tr("GL Coordinates System"),5000);
    glInitProjectionMatrix();
    emit coordSysChanged(__geomsys);
    emit valueChanged();
  }
}

void ViewCameraGL3::lockDim(int i)
{
  lockDim(i != 0);
  emit lockDimChanged(__lockdim);
  if(!__lockdim)reDim();
}

void ViewCameraGL3::lockDim(bool b)
{
  if(__lockdim != b){
	__lockdim = b;
	emit lockDimChanged(__lockdim);
	if(!__lockdim)reDim();
  }
}

void ViewCameraGL3::lockDim()
{
  __lockdim = ! __lockdim;
  emit lockDimChanged(__lockdim);
  if(!__lockdim)reDim();
}

void ViewCameraGL3::reDim()
{
  emit needReDim();
  emit valueChanged();
}

void
ViewCameraGL3::setGEOMCoordSys()
{
  if(!__geomsys){
    __geomsys =  true;
    status(tr("GEOM Coordinates System"),5000);
    glInitProjectionMatrix();
    emit coordSysChanged(__geomsys);
    emit valueChanged();
  }
}

void
ViewCameraGL3::setGLCoordSys()
{
  if(__geomsys){
    __geomsys =  false;
    status(tr("GL Coordinates System"),5000);
    glInitProjectionMatrix();
    emit coordSysChanged(__geomsys);
    emit valueChanged();
  }
}
#define QSV(v) "&lt; "+QString::number(v.x())+" , "+QString::number(v.y())+" , "+QString::number(v.z())+" &gt;"

Vector3 ViewCameraGL3::getPosition(){
        Vector3 pos = __eye - __translation;
        pos = Matrix3::axisRotation(Vector3::OY,-__elevation * GEOM_RAD ) * pos;
        pos = Matrix3::axisRotation(Vector3::OZ,__azimuth  * GEOM_RAD ) * pos;
        if(!__geomsys)pos = geom2gl(pos);
        return pos;
}

void 
ViewCameraGL3::setPosition(const QString& position){
  QStringList l = QStringList::split(',',position);
  setPosition(Vector3(l[0].toFloat(),l[1].toFloat(),l[2].toFloat()),
	l[3].toFloat(),l[4].toFloat());
}

void 
ViewCameraGL3::setPosition(const Vector3& position){
  Vector3::Spherical tr(position);
  __azimuth = tr.theta* GEOM_DEG;
  __elevation = tr.phi* GEOM_DEG;
  __translation = __eye - Vector3(tr.radius,0,0);
//  qWarning(QString("**>")+QSV(position));
//  qWarning(QString("-->")+QSV(getPosition())+" , "+QString::number(__azimuth)+" , "+QString::number(__elevation));
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit zoomChanged(__translation.x());
  emit valueChanged();
}

void 
ViewCameraGL3::setPosition(const Vector3& position, double az, double el){

  Matrix3 m = Matrix3::eulerRotationZYX(Vector3(az* GEOM_RAD,el* GEOM_RAD,0));
  m = m.inverse();
  Vector3 pos = m*position;
  __azimuth = az;
  __elevation = -el;
  __translation = __eye - pos;
 // qWarning(QString("**>")+QSV(pos)+" , "+QString::number(__azimuth)+" , "+QString::number(__elevation));
 // qWarning(QString("-->")+QSV(getPosition()));
 emit azimuthChanged(__azimuth);
 emit elevationChanged(__elevation);
 emit zoomChanged(__translation.x());
 emit valueChanged();
}

void 
ViewCameraGL3::lookAt(const Vector3& pos,const Vector3& target){
//  qWarning(QString("**>")+QSV(pos)+' - '+QSV(dir));
  Vector3 p = pos - target  ;
  Vector3::Spherical tr(p);
//  qWarning(QString("-->")+QString::number(tr.theta* GEOM_DEG)+' - '+QString::number(90-(tr.phi* GEOM_DEG)));
  setPosition(pos,tr.theta* GEOM_DEG,(tr.phi* GEOM_DEG)-90);
}

void 
ViewCameraGL3::lookIn(const Vector3& position,const Vector3& dir){
  Vector3::Spherical tr(-dir);
  double az = tr.theta* GEOM_DEG;
  double el = (tr.phi* GEOM_DEG)-90;
  Matrix3 m = Matrix3::eulerRotationZYX(Vector3(az* GEOM_RAD,el* GEOM_RAD,0));
  m = m.inverse();
  Vector3 pos = m*position;
  __azimuth = az;
  __elevation = -el;
  __translation = __eye - pos;
}

void 
ViewCameraGL3::cameraEvent(ViewCameraEvent3 * e){
  switch(e->def){
  case 0:
	setPosition(e->pos);
	break;
  case 1:
	setPosition(e->pos,e->azimuth,e->elevation);
	break;
  case 2:
	lookAt(e->pos,e->target);
	break;
  case 3:
	lookAt(getPosition(),e->target);
	break;
  }
}

Vector3 ViewCameraGL3::getDirection(){
        Vector3 dir = __center - __eye;
        dir.normalize();
        dir = Matrix3::axisRotation(Vector3::OY,-__elevation * GEOM_RAD ) * dir;
        dir = Matrix3::axisRotation(Vector3::OZ,__azimuth  * GEOM_RAD ) * dir;

        if(!__geomsys) dir = geom2gl(dir);
        return dir;
}

Vector3 ViewCameraGL3::getUp(){
        Vector3 up   (Vector3::OZ);
        up = Matrix3::axisRotation(Vector3::OY,-__elevation * GEOM_RAD ) * up;
        up = Matrix3::axisRotation(Vector3::OZ,__azimuth  * GEOM_RAD ) * up;

        if(!__geomsys) up = geom2gl(up);
        return up;
}

Vector3 ViewCameraGL3::getRight(){
        Vector3 dir = __center - __eye;
        dir.normalize();
        Vector3 right = cross(dir,Vector3::OZ);
        right = Matrix3::axisRotation(Vector3::OY,-__elevation * GEOM_RAD ) * right;
        right = Matrix3::axisRotation(Vector3::OZ,__azimuth  * GEOM_RAD ) * right;


        if(!__geomsys) right = geom2gl(right);

        return right;
}

Matrix4 ViewCameraGL3::getMatrix(){
        if( __geomsys){ /// Geom Sys Coordinates

                Vector3 up   (Vector3::OZ);

                Vector3 dir = __center - __eye;
                dir.normalize();

                Vector3 right = cross(dir,up);

                /*
                 On Geom sytem, since Z coordinate must represent the Up vector,
                 the matrix that give the ref of the camera is M3[dir,rigth,up]
                 default is dir = X, up = Z, rigth = -Y.
                */

                Matrix4 m(Matrix3(dir,right,up));
                m = Matrix4::translation(__eye) * m;

        m = Matrix4::translation(- __translation) * m;
                m = Matrix4(Matrix3::axisRotation(Vector3::OY,- __elevation * GEOM_RAD )) * m;
                m = Matrix4(Matrix3::axisRotation(Vector3::OZ,__azimuth  * GEOM_RAD )) * m;
                return m;
        }
        else {

                /*
                 On GL sytem, since Z coordinate must represent the Up vector,
                 the matrix that give the ref of the camera is M3[dir,up,rigth]
                 default is dir = X, up = Y, rigth = Z.
                */

                Vector3 orig (geom2gl(__eye));
                Vector3 cent (geom2gl(__center));

                Vector3 up   (Vector3::OY);
                Vector3 dir = cent - orig;
                dir.normalize();
                Vector3 right = cross(dir,up);
                Matrix4 m(Matrix3(dir,up,right));

                m = Matrix4::translation(orig) * m;
        m = Matrix4::translation(geom2gl(__translation)) * m;
                m = Matrix4(Matrix3::axisRotation(Vector3::OX,-__elevation * GEOM_RAD )) * m;
                m = Matrix4(Matrix3::axisRotation(Vector3::OY,__azimuth  * GEOM_RAD )) * m;
                return m;
        }
}


QPopupMenu *
ViewCameraGL3::createToolsMenu(QWidget * parent)
{
  ViewCameraMenu3 * __CameraMenu = new ViewCameraMenu3(this,parent);
  return __CameraMenu;
}


void
ViewCameraGL3::addProperties(QTabWidget * tab)
{
  CameraProperties * tab2 = new CameraProperties( tab, "Camera Prop" );
  tab2->ZoomEdit->setText(QString::number(__translation.x()));
  tab2->ZoomEdit->setReadOnly(true);
  tab2->XTranslationEdit->setText(QString::number(__translation.y()));
  tab2->XTranslationEdit->setReadOnly(true);
  tab2->YTranslationEdit->setText(QString::number(__translation.z()));
  tab2->YTranslationEdit->setReadOnly(true);
  tab2->AzimuthEdit->setText(QString::number(__azimuth));
  tab2->AzimuthEdit->setReadOnly(true);
  tab2->ElevationEdit->setText(QString::number(__elevation));
  tab2->ElevationEdit->setReadOnly(true);
  Vector3 pos = getPosition();
  tab2->PositionEdit->setText('<'+QString::number(pos.x())+','+QString::number(pos.y())+','+QString::number(pos.z())+'>');
  tab2->PositionEdit->setReadOnly(true);
  tab2->NearEdit->setText(QString::number(__near_plane));
  QObject::connect(tab2->NearEdit,SIGNAL(textChanged(const QString&)),this,SLOT(setNearPlane(const QString&)));
  QObject::connect(tab2->NearEdit,SIGNAL(returnPressed()),this,SLOT(validValue()));
  tab2->FarEdit->setText(QString::number(__far_plane));
  QObject::connect(tab2->FarEdit,SIGNAL(textChanged(const QString&)),this,SLOT(setFarPlane(const QString&)));
  QObject::connect(tab2->FarEdit,SIGNAL(returnPressed()),this,SLOT(validValue()));
  tab2->EyeEdit->setText(toQString(__eye));
  tab2->EyeEdit->setReadOnly(true);
  tab2->CenterEdit->setText(toQString(__center));
  tab2->CenterEdit->setReadOnly(true);
  tab2->DefaultAngleEdit->setText(QString::number(__default_view_angle));
  QObject::connect(tab2->DefaultAngleEdit,SIGNAL(textChanged(const QString&)),this,SLOT(setViewAngle(const QString&)));
  QObject::connect(tab2->DefaultAngleEdit,SIGNAL(returnPressed()),this,SLOT(validViewAngle()));
  tab2->CurrentAngleEdit->setText(QString::number(__view_angle));
  QObject::connect(this,SIGNAL(currentViewAngleChanged(const QString&)),
                   tab2->CurrentAngleEdit,SLOT(setText(const QString&)));
  QObject::connect(this,SIGNAL(viewAngleChanged(const QString&)),
                   tab2->DefaultAngleEdit,SLOT(setText(const QString&)));
  tab2->CurrentAngleEdit->setReadOnly(true);
  tab2->ProjectionBox->setCurrentItem(int(__projectionmode));
  QObject::connect(tab2->ProjectionBox,SIGNAL(activated(int)),this,SLOT(setProjectionMode(int)));
  tab2->CoordinatesBox->setCurrentItem(int(__geomsys));
  QObject::connect(tab2->CoordinatesBox,SIGNAL(activated(int)),this,SLOT(setCoordSys(int)));
  tab->insertTab( tab2, tr( "&Camera" ) );

  /*Matrix4 m;
  glGeomGetMatrix(GL_PROJECTION_MATRIX,m);
  CameraProp2 * tab3 = new CameraProp2( tab, "Camera Prop2" );
  tab3->MA00->setText(QString::number((fabs(m(0,0)) < GEOM_EPSILON?0:m(0,0))));
  tab3->MA01->setText(QString::number((fabs(m(0,1)) < GEOM_EPSILON?0:m(0,1))));
  tab3->MA02->setText(QString::number((fabs(m(0,2)) < GEOM_EPSILON?0:m(0,2))));
  tab3->MA03->setText(QString::number((fabs(m(0,3)) < GEOM_EPSILON?0:m(0,3))));

  tab3->MA10->setText(QString::number((fabs(m(1,0)) < GEOM_EPSILON?0:m(1,0))));
  tab3->MA11->setText(QString::number((fabs(m(1,1)) < GEOM_EPSILON?0:m(1,1))));
  tab3->MA12->setText(QString::number((fabs(m(1,2)) < GEOM_EPSILON?0:m(1,2))));
  tab3->MA13->setText(QString::number((fabs(m(1,3)) < GEOM_EPSILON?0:m(1,3))));

  tab3->MA20->setText(QString::number((fabs(m(2,0)) <GEOM_EPSILON?0:m(2,0))));
  tab3->MA21->setText(QString::number((fabs(m(2,1)) <GEOM_EPSILON?0:m(2,1))));
  tab3->MA22->setText(QString::number((fabs(m(2,2)) <GEOM_EPSILON?0:m(2,2))));
  tab3->MA23->setText(QString::number((fabs(m(2,3)) <GEOM_EPSILON?0:m(2,3))));

  tab3->MA30->setText(QString::number((fabs(m(3,0)) <GEOM_EPSILON?0:m(3,0))));
  tab3->MA31->setText(QString::number((fabs(m(3,1)) <GEOM_EPSILON?0:m(3,1))));
  tab3->MA32->setText(QString::number((fabs(m(3,2)) <GEOM_EPSILON?0:m(3,2))));
  tab3->MA33->setText(QString::number((fabs(m(3,3)) <GEOM_EPSILON?0:m(3,3))));
  glGeomGetMatrix(GL_MODELVIEW_MATRIX,m);
  tab3->MB00->setText(QString::number((fabs(m(0,0)) <GEOM_EPSILON?0:m(0,0))));
  tab3->MB01->setText(QString::number((fabs(m(0,1)) <GEOM_EPSILON?0:m(0,1))));
  tab3->MB02->setText(QString::number((fabs(m(0,2)) <GEOM_EPSILON?0:m(0,2))));
  tab3->MB03->setText(QString::number((fabs(m(0,3)) <GEOM_EPSILON?0:m(0,3))));
  tab3->MB10->setText(QString::number((fabs(m(1,0)) <GEOM_EPSILON?0:m(1,0))));
  tab3->MB11->setText(QString::number((fabs(m(1,1)) <GEOM_EPSILON?0:m(1,1))));
  tab3->MB12->setText(QString::number((fabs(m(1,2)) <GEOM_EPSILON?0:m(1,2))));
  tab3->MB13->setText(QString::number((fabs(m(1,3)) <GEOM_EPSILON?0:m(1,3))));
  tab3->MB20->setText(QString::number((fabs(m(2,0)) <GEOM_EPSILON?0:m(2,0))));
  tab3->MB21->setText(QString::number((fabs(m(2,1)) <GEOM_EPSILON?0:m(2,1))));
  tab3->MB22->setText(QString::number((fabs(m(2,2)) <GEOM_EPSILON?0:m(2,2))));
  tab3->MB23->setText(QString::number((fabs(m(2,3)) <GEOM_EPSILON?0:m(2,3))));
  tab3->MB30->setText(QString::number((fabs(m(3,0)) <GEOM_EPSILON?0:m(3,0))));
  tab3->MB31->setText(QString::number((fabs(m(3,1)) <GEOM_EPSILON?0:m(3,1))));
  tab3->MB32->setText(QString::number((fabs(m(3,2)) <GEOM_EPSILON?0:m(3,2))));
  tab3->MB33->setText(QString::number((fabs(m(3,3)) <GEOM_EPSILON?0:m(3,3))));
  tab->insertTab( tab3, tr( "&GL Matrix" ) );*/

/*  CameraProp2 * tab4 = new CameraProp2( tab, "Camera Prop4" );
  tab4->Label->setText("Camera Matrix");
  tab4->Label2->setText("Camera Vectors");
 // m = getMatrix();
  Vector3 p = getPosition();
  tab4->MA00->setText(QString::number(p.x())+','+QString::number(p.y())+','+QString::number(p.z())+','+QString::number(__azimuth)+','+QString::number(__elevation));
  QObject::connect(tab4->MA00,SIGNAL(textChanged(const QString&)),this,SLOT(setPosition(const QString&)));

  tab4->MA01->setText(QString::number((fabs(m(0,0)) < GEOM_EPSILON?0:m(0,1))));
  tab4->MA01->setText(QString::number((fabs(m(0,1)) < GEOM_EPSILON?0:m(0,1))));
  tab4->MA02->setText(QString::number((fabs(m(0,2)) < GEOM_EPSILON?0:m(0,2))));
  tab4->MA03->setText(QString::number((fabs(m(0,3)) < GEOM_EPSILON?0:m(0,3))));

  tab4->MA10->setText(QString::number((fabs(m(1,0)) < GEOM_EPSILON?0:m(1,0))));
  tab4->MA11->setText(QString::number((fabs(m(1,1)) < GEOM_EPSILON?0:m(1,1))));
  tab4->MA12->setText(QString::number((fabs(m(1,2)) < GEOM_EPSILON?0:m(1,2))));
  tab4->MA13->setText(QString::number((fabs(m(1,3)) < GEOM_EPSILON?0:m(1,3))));

  tab4->MA20->setText(QString::number((fabs(m(2,0)) <GEOM_EPSILON?0:m(2,0))));
  tab4->MA21->setText(QString::number((fabs(m(2,1)) <GEOM_EPSILON?0:m(2,1))));
  tab4->MA22->setText(QString::number((fabs(m(2,2)) <GEOM_EPSILON?0:m(2,2))));
  tab4->MA23->setText(QString::number((fabs(m(2,3)) <GEOM_EPSILON?0:m(2,3))));

  tab4->MA30->setText(QString::number((fabs(m(3,0)) <GEOM_EPSILON?0:m(3,0))));
  tab4->MA31->setText(QString::number((fabs(m(3,1)) <GEOM_EPSILON?0:m(3,1))));
  tab4->MA32->setText(QString::number((fabs(m(3,2)) <GEOM_EPSILON?0:m(3,2))));
  tab4->MA33->setText(QString::number((fabs(m(3,3)) <GEOM_EPSILON?0:m(3,3))));

  if(!__geomsys)  m = Matrix4(Matrix3(getDirection(),getUp(),getRight()));
  else m = Matrix4(Matrix3(getDirection(),getRight(),getUp()));
  Vector3 p = getPosition();
  m(0,3) = p.x();m(1,3) = p.y();m(2,3) = p.z();
  tab4->MB00->setText(QString::number((fabs(m(0,0)) <GEOM_EPSILON?0:m(0,0))));
  tab4->MB01->setText(QString::number((fabs(m(0,1)) <GEOM_EPSILON?0:m(0,1))));
  tab4->MB02->setText(QString::number((fabs(m(0,2)) <GEOM_EPSILON?0:m(0,2))));
  tab4->MB03->setText(QString::number((fabs(m(0,3)) <GEOM_EPSILON?0:m(0,3))));

  tab4->MB10->setText(QString::number((fabs(m(1,0)) <GEOM_EPSILON?0:m(1,0))));
  tab4->MB11->setText(QString::number((fabs(m(1,1)) <GEOM_EPSILON?0:m(1,1))));
  tab4->MB12->setText(QString::number((fabs(m(1,2)) <GEOM_EPSILON?0:m(1,2))));
  tab4->MB13->setText(QString::number((fabs(m(1,3)) <GEOM_EPSILON?0:m(1,3))));

  tab4->MB20->setText(QString::number((fabs(m(2,0)) <GEOM_EPSILON?0:m(2,0))));
  tab4->MB21->setText(QString::number((fabs(m(2,1)) <GEOM_EPSILON?0:m(2,1))));
  tab4->MB22->setText(QString::number((fabs(m(2,2)) <GEOM_EPSILON?0:m(2,2))));
  tab4->MB23->setText(QString::number((fabs(m(2,3)) <GEOM_EPSILON?0:m(2,3))));

  tab4->MB30->setText(QString::number((fabs(m(3,0)) <GEOM_EPSILON?0:m(3,0))));
  tab4->MB31->setText(QString::number((fabs(m(3,1)) <GEOM_EPSILON?0:m(3,1))));
  tab4->MB32->setText(QString::number((fabs(m(3,2)) <GEOM_EPSILON?0:m(3,2))));
  tab4->MB33->setText(QString::number((fabs(m(3,3)) <GEOM_EPSILON?0:m(3,3))));

  tab->insertTab( tab4, tr( "&GEOM Camera" ) );*/
}

void
ViewCameraGL3::fillToolBar(QToolBar * toolBar)
{
  QPixmap home(ViewerIcon3::getPixmap(ViewerIcon3::icon_home));
  QPixmap persp(ViewerIcon3::getPixmap(ViewerIcon3::icon_perspective));
  QPixmap eye(ViewerIcon3::getPixmap(ViewerIcon3::icon_eye));
  QPixmap eyef(ViewerIcon3::getPixmap(ViewerIcon3::icon_eyef));
  drawArrow3(&eye,2);
  drawArrow3(eye.mask(),2);
  QToolButton * bt = new QToolButton(home,
                         tr("Home Position"),
                         tr("Home Position"),
                         this, SLOT(home()), toolBar);
  QWhatsThis::add(bt,tr("<b>Home Position</b><br><br>"
	"Set the view to the Initial Position.<br>"
	"You can also use Menu <br><b>Tools > Camera > Home</b><br>"));

  QToolButton * bt2 = new QToolButton(eye,
                         tr("Camera Position"),
                         tr("Camera Position"),
                         this,NULL,toolBar);
  QPopupMenu * selctmenu = new QPopupMenu(bt2);
  selctmenu->insertItem(tr("Front View (YZ)"),this,SLOT(YZView()));
  selctmenu->insertItem(tr("Right View (XZ)"),this,SLOT(XZView()));
  selctmenu->insertItem(tr("Top View (XY)"),this,SLOT(XYView()));
  bt2->setPopup(selctmenu);
  bt2->setPopupDelay(0);

  QToolButton * bt3 = new QToolButton(eyef,
                         tr("Fit to Window"),
                         tr("Fit to Window"),
                         this, SLOT(reDim()), toolBar);
  QWhatsThis::add(bt3,tr("<b>Fit to Window</b><br><br>"
	"Set the view to fit the scene.<br>"
	"You can also use Menu <br><b>Tools > Camera > Fit to Window</b><br>"));
   QToolButton * b = new QToolButton(persp,
                         tr("Perspective Camera"),
                         tr("Perspective Camera"),
                         this, SLOT(changeCameraMode()), toolBar);
  QWhatsThis::add(b,tr("<b>Projection</b><br><br>"
	"Camera can be <b>Perspective</b> or <b>Orthographic</b>.<br><br>"
	"Setting this button <i>On</i> make the camera in Perspective mode, "
	"<i>Off</i> in Orthographic mode<br><br>"
	"You can also use Menu <br><b>Tools > Camera > Projection</b><br>"
	));
   b->setToggleButton(true) ;
   b->setOn(__projectionmode);
   QObject::connect(this,SIGNAL(projectionChanged(bool)),
					 b,SLOT(setOn(bool)));

   toolBar->addSeparator();
}

void ViewCameraGL3::save() const
{
  QString filename = ViewFileManager3::getSaveFileName("","","",__frame);
  if(!filename.isEmpty()){
    save(filename);
  }
}

void
ViewCameraGL3::read()
{
  QString filename = QFileDialog::getOpenFileName("","",__frame);
  if(!filename.isEmpty()){
    read(filename);
  }
}

#define OV(s,v) s << " " << v.x() << " " << v.y() << " " << v.z()

void ViewCameraGL3::save(const QString& filename) const
{
    QFile file(filename);
    if(file.open( IO_WriteOnly )) {
        file.flush();
		QTextStream stream ( &file );
        stream << __azimuth << " " << __elevation;
        stream << " " << __stepMove;
        OV(stream,__center);
        OV(stream,__eye);
        OV(stream,__translation);

        stream << " " << __radius << " " << __far_plane << " " << __near_plane;
		stream << " " << __default_view_angle << " " <<  __view_angle;
		stream << " " << __projectionmode << " " << __geomsys;
     }
	else QMessageBox::warning(__frame,tr("File Access"),tr("Cannot Open File")+" \""+filename+"\".",tr("Abort"));
}


#define IV(s,v) s >> v.x() >> v.y() >> v.z()

void ViewCameraGL3::read(const QString& filename)
{
    QFile file(filename);
    if(file.exists() && file.open( IO_ReadOnly )) {
    QTextStream stream ( &file );

        stream >> __azimuth >> __elevation;
        printf("%d\n",__azimuth);
        printf("%d\n",__elevation);
        stream >> __stepMove;
        printf("%i\n",__stepMove);

        IV(stream,__center);
        IV(stream,__eye);
        IV(stream,__translation);

        stream >> __radius >> __far_plane >> __near_plane;
    stream >> __default_view_angle >> __view_angle;
        int i = 0;
    stream >> i;
        __projectionmode = (i != 0);
    stream >> i;
        __geomsys = (i != 0);
//    emit azimuthChanged(QString::number(__azimuth));
//    emit elevationChanged(QString::number(__elevation));
//    emit zoomChanged(QString::number(__translation.x()));
    emit azimuthChanged(__azimuth);
    emit elevationChanged(__elevation);
    emit zoomChanged(__translation.x());
    emit farPlaneChanged(QString::number(__far_plane));
    emit nearPlaneChanged(QString::number(__near_plane));
    emit projectionChanged(__projectionmode);
    emit coordSysChanged(__geomsys);
    emit valueChanged();
        }
	else QMessageBox::warning(__frame,tr("File Access"),tr("Cannot Open File")+" \""+filename+"\".",tr("Abort"));

}
