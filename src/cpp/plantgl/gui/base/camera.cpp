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


#include <plantgl/algo/opengl/util_gl.h>
#include <plantgl/algo/opengl/util_glu.h>
#include <plantgl/math/util_math.h>
#include <plantgl/gui/viewer/util_qstring.h>

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/QTabWidget>
    #include <QtWidgets/qlineedit.h>
    #include <QtWidgets/qcombobox.h>
    #include <QtWidgets/qlabel.h>
    #include <QtWidgets/qtoolbutton.h>
    #include <QtWidgets/qtoolbar.h>
    #include <QtWidgets/qfiledialog.h>
    #include <QtWidgets/qwhatsthis.h>
    #include <QtWidgets/qmessagebox.h>
#else
    #include <QtGui/QTabWidget>
    #include <QtGui/qlineedit.h>
    #include <QtGui/qcombobox.h>
    #include <QtGui/qlabel.h>
    #include <QtGui/qtoolbutton.h>
    #include <QtGui/qtoolbar.h>
    #include <QtGui/qfiledialog.h>
    #include <QtGui/qwhatsthis.h>
    #include <QtGui/qmessagebox.h>
#endif

#include <QtCore/qtextstream.h>
#include <QtCore/qfile.h>
#include <QtGui/qbitmap.h>

#include "filemanager.h"
#include "camera.h"
#include "interface/ui_cameraqt.h"
#include "interface/ui_cameraqt2.h"
#include "icons.h"
#include "glframe.h"
#include "event.h"
#include "configuration.h"

#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */
ViewCameraMenu::ViewCameraMenu(ViewCameraGL * camera,QWidget * parent, const char * name):
  QMenu(parent)
{
  if(name)setObjectName(name);

  QPixmap home(ViewerIcon::getPixmap(ViewerIcon::home));
  QPixmap read(ViewerIcon::getPixmap(ViewerIcon::fileopen));
  QPixmap save(ViewerIcon::getPixmap(ViewerIcon::filefloppy));
  QPixmap persp(ViewerIcon::getPixmap(ViewerIcon::perspective));
  QPixmap ortho(ViewerIcon::getPixmap(ViewerIcon::orthographic));
  QPixmap linked(ViewerIcon::getPixmap(ViewerIcon::linked));
  QPixmap eyef(ViewerIcon::getPixmap(ViewerIcon::eyef));

  QAction * action = addAction(home,tr("Home"),      camera,SLOT(home()), Qt::CTRL+Qt::Key_I);

  addAction(tr("&Front View (YZ)"),camera,SLOT(YZView()));
  addAction(tr("&Right View (XZ)"),camera,SLOT(XZView()));
  addAction(tr("&Top View (XY)"),  camera,SLOT(XYView()));
  addSeparator();
  QMenu * __coordMenu = new QMenu(this);
  idGEOMcoord = __coordMenu->addAction(tr("GEOM System"),   camera,SLOT(setGEOMCoordSys()));
  idGLcoord   = __coordMenu->addAction(tr("GL System"),     camera,SLOT(setGLCoordSys()));
  __coordMenu->addSeparator();
  __coordMenu->addAction(tr("&Change"),       camera,SLOT(changeCoordSys()));
  idGEOMcoord->setCheckable(true);
  idGLcoord->setCheckable(true);
  setCoordSys(camera->getCoordSys());
  __coordMenu->setTitle(tr("Coordinates System"));
  addMenu(__coordMenu);
  addSeparator();
  QMenu * __projectionMenu = new QMenu(this);
  idPerspective = __projectionMenu->addAction(persp,tr("&Perspective"),  camera,SLOT(setPerspectiveMode()));
  idOrtho       = __projectionMenu->addAction(ortho,tr("Ort&hographic"), camera,SLOT(setOrthographicMode()));
  __projectionMenu->addSeparator();
  __projectionMenu->addAction(tr("&Change"),       camera,SLOT(changeCameraMode()),Qt::CTRL+Qt::Key_D);
  idPerspective->setCheckable(true);
  idOrtho->setCheckable(true);
  __projectionMenu->setTitle(tr("Projection"));
  addMenu(__projectionMenu);

  setProjectionMode(camera->getProjectionMode());
  QObject::connect(camera,SIGNAL(projectionChanged(bool)),this,SLOT(setProjectionMode(bool)));
  QObject::connect(camera,SIGNAL(coordSysChanged(int)),this,SLOT(setCoordSys(int)));
  addSeparator();
  addAction(save,tr("&Save"),       camera,SLOT(save()));
  addAction(read,tr("&Read"),       camera,SLOT(read()));
  addSeparator();
  QAction * idLock = addAction(linked,tr("&Auto Fit to Window"),       camera,SLOT(lockDim()));
  if(camera)idLock->setChecked(camera->isDimLock());
  QObject::connect(camera,SIGNAL(lockDimChanged(bool)),idLock,SLOT(setChecked(bool)));
  addAction(eyef,tr("&Fit to Window"),       camera,SLOT(reDim()));

}

ViewCameraMenu::~ViewCameraMenu()
{
}

void
ViewCameraMenu::setProjectionMode(bool b)
{
  idPerspective->setChecked(b);
  idOrtho->setChecked(!b);
}

void
ViewCameraMenu::setCoordSys(int b)
{
    idGEOMcoord->setChecked(b ==1);
    idGLcoord->setChecked(b ==0);
}


/* ----------------------------------------------------------------------- */


ViewCameraGL::ViewCameraGL(QGLWidget * parent, const char * name) :
  ViewObjectGL(parent,name),
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
  __lockdim(false),
  __resizePolicy(eKeepObjectSize)
{
    init();
}

ViewCameraGL::ViewCameraGL(QObject * parent, const char * name) :
  ViewObjectGL(parent,name),
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
  __lockdim(false),
  __resizePolicy(eKeepObjectSize)
{
    init();
}

ViewCameraGL::~ViewCameraGL()
{
}

void ViewCameraGL::init()
{

  ViewerSettings settings;
  settings.beginGroup("Camera");
  bool autofit = settings.value("AutoFit",!__lockdim).toBool();
  __geomsys = settings.value("PglRefSystem",__geomsys).toBool();
  lockDim(!autofit);
  __resizePolicy = (eCameraResizePolicy)settings.value("ResizePolicy",(int)__resizePolicy).toInt();
  settings.endGroup();
}

void ViewCameraGL::endEvent()
{
  ViewerSettings settings;
  settings.beginGroup("Camera");
  settings.setValue("AutoFit",!isDimLock());
  settings.setValue("PglRefSystem",__geomsys);
  settings.setValue("ResizePolicy",(int)__resizePolicy);
  settings.endGroup();
}

void ViewCameraGL::home()
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

void ViewCameraGL::XYView()
{
  __azimuth = 0;
  __elevation = 90;
//  emit azimuthChanged(QString::number(__azimuth));
//  emit elevationChanged(QString::number(__elevation));
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}

void ViewCameraGL::XZView()
{
  __azimuth = 90;
  __elevation = 0;
//  emit azimuthChanged(QString::number(__azimuth));
//  emit elevationChanged(QString::number(__elevation));
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}

void ViewCameraGL::YZView()
{
  __azimuth = 0;
  __elevation = 0;
//  emit azimuthChanged(QString::number(__azimuth));
//  emit elevationChanged(QString::number(__elevation));
  emit azimuthChanged(__azimuth);
  emit elevationChanged(__elevation);
  emit valueChanged();
}


void ViewCameraGL::nextView()
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

void ViewCameraGL::setAngles(double azimuth, double elevation)
{
    __azimuth = azimuth;
    __elevation = elevation;
}

void
ViewCameraGL::setAzimuth(double azimuth)
{
  if(__azimuth != azimuth){
    __azimuth = azimuth;
//  emit azimuthChanged(QString::number(__azimuth));
    emit azimuthChanged(__azimuth);
    emit valueChanged();
  }
}

void
ViewCameraGL::setElevation(double elevation)
{
  if(__elevation != elevation){
    __elevation = elevation;
//  emit elevationChanged(QString::number(__elevation));
    emit elevationChanged(__elevation);
    emit valueChanged();
  }
}

void
ViewCameraGL::setZoom(double zoom)
{
  if(__translation.x() != zoom){
    __translation.x() = zoom;
    emit zoomChanged(__translation.x());
    emit valueChanged();
  }
}


void
ViewCameraGL::setAzimuth(int azimuth)
{
  if(__azimuth != azimuth){
    __azimuth = azimuth;
//  emit azimuthChanged(QString::number(__azimuth));
    emit azimuthChanged(__azimuth);
    emit valueChanged();
  }
}

void
ViewCameraGL::setElevation(int elevation)
{
  if(__elevation != elevation){
    __elevation = elevation;
//  emit elevationChanged(QString::number(__elevation));
    emit elevationChanged(__elevation);
    emit valueChanged();
  }
}

void
ViewCameraGL::setZoom(int zoom)
{
  if(__translation.x() != zoom){
    __translation.x() = zoom;
    emit zoomChanged(__translation.x());
    emit valueChanged();
  }
}

/*
void
ViewCameraGL::setAzimuth(const QString& azimuth)
{
  setAzimuth(double(azimuth.toInt()));
}

void
ViewCameraGL::setElevation(const QString& elevation)
{
  setElevation(double(elevation.toInt()));
}

void
ViewCameraGL::setZoom(const QString& zoom)
{
  __translation.x() = zoom.toInt();
}
*/


void
ViewCameraGL::setFarPlane(const QString& plane)
{
  __far_plane = (double)plane.toFloat();
  emit farPlaneChanged(QString::number(__far_plane));
}

void
ViewCameraGL::setNearPlane(const QString& plane)
{
  __near_plane = (double)plane.toFloat();
  emit nearPlaneChanged(QString::number(__near_plane));
}

void
ViewCameraGL::validViewAngle()
{
  initialize(__radius,__center);
  glInitProjectionMatrix();
  emit viewAngleChanged(__default_view_angle);
  emit currentViewAngleChanged(QString::number(__view_angle));
  emit valueChanged();
}

void
ViewCameraGL::validValue(){
  glInitProjectionMatrix();
  emit valueChanged();
}

void
ViewCameraGL::setStepMove(const QString& step)
{

  setStepMove(step.toInt());
}

void
ViewCameraGL::setStepMove(double step)
{
  __stepMove = step;
}

void
ViewCameraGL::setDistanceX(double dist)
{
  __translation.y() = dist;
  emit valueChanged();
}

void
ViewCameraGL::setDistanceY(double dist)
{
  __translation.z() = dist;
  emit valueChanged();
}

void
ViewCameraGL::setCenter(const Vector3& center)
{
  __center = center;
  emit valueChanged();
}


void
ViewCameraGL::moving(int dx, int dy)
{
  double ratio = 0.05;
  __translation.y() -= ratio *__stepMove*dx;
  __translation.z() += ratio *__stepMove*dy;
}

void
ViewCameraGL::zooming(int dx, int dy)
{
  __translation.x() += dy*__stepMove;
  glInitProjectionMatrix();
  emit zoomChanged(__translation.x());
// emit zoomChanged(QString::number(__translation.x()));
}


void
ViewCameraGL::rotating(int dx, int dy)
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
ViewCameraGL::setViewAngle(double angle)
{
  __default_view_angle = angle;
  initialize(__radius,__center);
  glInitProjectionMatrix();
  emit viewAngleChanged(__default_view_angle);
  emit currentViewAngleChanged(QString::number(__view_angle));
  emit valueChanged();
}


void
ViewCameraGL::initializeGL()
{
  BoundingBoxPtr bbox;
  initialize(bbox);
}

static bool first = true;
void
ViewCameraGL::buildCamera(const BoundingBoxPtr& bbox, bool force)
{
   if(!__lockdim || force || first){
    initialize(bbox);
    glInitProjectionMatrix();
    first = false;
   }
}



void
ViewCameraGL::initialize(const BoundingBoxPtr& bbox)
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
ViewCameraGL::initialize(const double& _radius,const Vector3& center)
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

  updateActualViewAngle();

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
ViewCameraGL::resizeGL(int w, int h)
{
  __width = w;
  __height = h;

  glViewport( 0, 0, (GLint)w, (GLint)h );

  updateActualViewAngle();

  glInitProjectionMatrix();

}

void ViewCameraGL::updateActualViewAngle()
{
  if (__resizePolicy == eKeepObjectSize){
      /// compute the view_angle according to the size of the frame.
      /// For 500x500 window, the view_angle is __default_view_angle (60)
      /// Else view_angle is recomputed (OpenGL programming guide, second edition v1.1 p131)
      /// to see a bigger part of the scene but at the same scale.
      double rad = __radius * __height/500 ;
      double dist = __eye.x();
      __view_angle = (2.0*atan2(rad,dist))*GEOM_DEG;
  }
  else {
      __view_angle = __default_view_angle;
  }
}

void
ViewCameraGL::beginSelectGL(const QPoint& point)
{
  GEOM_GL_ERROR;

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  geomPickMatrix(point);

  glPushProjectionMatrix();
  glInitModelViewMatrix();

  GEOM_GL_ERROR;
}

void
ViewCameraGL::beginSelectGL(int x, int y)
{

    beginSelectGL(QPoint(x,y));

}

void
ViewCameraGL::beginSelectGL(const QRect& region)
{
  GEOM_GL_ERROR;

  // printf("%i %i %i %i\n",x,y,region.width(),region.height());

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  geomPickMatrix(region);

  glPushProjectionMatrix();
  glInitModelViewMatrix();

  GEOM_GL_ERROR;

}

void
ViewCameraGL::endSelectGL()
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}


void
ViewCameraGL::paintGL()
{
        glInitProjectionMatrix();
        glInitModelViewMatrix();
}

void
ViewCameraGL::glInitModelViewMatrix()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushModelViewMatrix();
}

void
ViewCameraGL::glPushModelViewMatrix()
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
ViewCameraGL::glInitProjectionMatrix()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glPushProjectionMatrix();
}

void
ViewCameraGL::glPushProjectionMatrix()
{
  double dep(0);
  if(__translation.x() < -((__far_plane-__near_plane)/2))
    dep = __translation.x()+((__far_plane-__near_plane)/2) ;

  if(__projectionmode){
    geomPerspective(__view_angle,__width/__height,__near_plane-dep,__far_plane-dep);


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

#ifndef Q_OS_MAC
  GEOM_GL_ERROR;
#endif
}


void
ViewCameraGL::setPerspectiveMode()
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
ViewCameraGL::setOrthographicMode()
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
ViewCameraGL::changeCameraMode()
{
  __projectionmode = ! __projectionmode;
  if( __projectionmode)status(tr("Perspective Camera"),5000);
  else status(tr("Orthographic Camera"),5000);
  glInitProjectionMatrix();
  emit projectionChanged(__projectionmode);
  emit valueChanged();
}

void
ViewCameraGL::setProjectionMode(int i){
        setProjectionMode(i != 0);
}

void
ViewCameraGL::setProjectionMode(bool b)
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
ViewCameraGL::changeCoordSys()
{
  __geomsys = ! __geomsys;
  if( __geomsys)status(tr("GEOM Coordinates System"),5000);
  else status(tr("GL Coordinates System"),5000);
  glInitProjectionMatrix();
  emit coordSysChanged(__geomsys);
  emit valueChanged();
}

void
ViewCameraGL::setCoordSys(int i ){
        setCoordSys(i != 0);
}

void
ViewCameraGL::setCoordSys(bool b)
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

void ViewCameraGL::lockDim(int i)
{
  lockDim(i != 0);
}

void ViewCameraGL::lockDim(bool b)
{
  if(__lockdim != b){
    __lockdim = b;
    emit lockDimChanged(__lockdim);
    if(!__lockdim)reDim();
  }
}

void ViewCameraGL::lockDim()
{
  __lockdim = ! __lockdim;
  emit lockDimChanged(__lockdim);
  if(!__lockdim)reDim();
}

void ViewCameraGL::reDim()
{
  emit needReDim();
  emit valueChanged();
}

void
ViewCameraGL::setGEOMCoordSys()
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
ViewCameraGL::setGLCoordSys()
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

Vector3 ViewCameraGL::getPosition(){
        Vector3 pos = __eye - __translation;
        pos = Matrix3::axisRotation(Vector3::OY,-__elevation * GEOM_RAD ) * pos;
        pos = Matrix3::axisRotation(Vector3::OZ,__azimuth  * GEOM_RAD ) * pos;
        if(!__geomsys)pos = geom2gl(pos);
        return pos;
}

void
ViewCameraGL::setPosition(const QString& position){
    QStringList l = position.split(',');
    setPosition(Vector3(l[0].toFloat(),l[1].toFloat(),l[2].toFloat()),
    l[3].toFloat(),l[4].toFloat());
}

void
ViewCameraGL::setPosition(const Vector3& position){
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
ViewCameraGL::setPosition(const Vector3& position, double az, double el){

  Matrix3 m = Matrix3::eulerRotationZYX(Vector3(az* GEOM_RAD,el* GEOM_RAD,0));
  m = m.inverse();
  Vector3 pos = m*position;
  __azimuth = az;
  __elevation = -el;
  __translation = __eye - pos;
 // qWarning((QString("**>")+QSV(pos)+" , "+QString::number(__azimuth)+" , "+QString::number(__elevation)).toAscii());
 // qWarning((QString("-->")+QSV(getPosition())).toAscii());
 emit azimuthChanged(__azimuth);
 emit elevationChanged(__elevation);
 emit zoomChanged(__translation.x());
 emit valueChanged();
}

void
ViewCameraGL::lookAt(const Vector3& pos,const Vector3& target){
//  qWarning(QString("**>")+QSV(pos)+' - '+QSV(dir));
  Vector3 p = pos - target  ;
  Vector3::Spherical tr(p);
//  qWarning(QString("-->")+QString::number(tr.theta* GEOM_DEG)+' - '+QString::number(90-(tr.phi* GEOM_DEG)));
  setPosition(pos,tr.theta* GEOM_DEG,(tr.phi* GEOM_DEG)-90);
}

void
ViewCameraGL::lookIn(const Vector3& position,const Vector3& dir){
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
ViewCameraGL::setResizePolicy(int value)
{
    __resizePolicy = (eCameraResizePolicy)value;
    setViewAngle(__default_view_angle);

}

void
ViewCameraGL::cameraEvent(ViewEvent * ev){
    int evtype = ev->type();
    if (evtype == ViewEvent::eCameraSet) {
        ViewCameraSetEvent * e = (ViewCameraSetEvent *)ev;
        switch(e->arg5){
            case 0:
                setPosition(e->arg1);
            break;
            case 1:
                setPosition(e->arg1,e->arg3,e->arg4);
            break;
            case 2:
                lookAt(e->arg1,e->arg2);
            break;
            case 3:
                lookAt(getPosition(),e->arg2);
            break;
        }
    }
    else if (evtype == ViewEvent::eCameraGet){
        ViewCameraGetEvent * e = (ViewCameraGetEvent *)ev;
        *e->result = getPosition();
        *e->arg1 = getDirection();
        *e->arg2 = getUp();
    }
    else if (evtype == ViewEvent::eSetViewAngle){
        ViewSetViewAngleEvent * e = (ViewSetViewAngleEvent *)ev;
        setViewAngle(e->arg1);
    }
    else if (evtype == ViewEvent::eGetViewAngle){
        ViewGetViewAngleEvent * e = (ViewGetViewAngleEvent *)ev;
        *e->result = getDefaultViewAngle();
    }
}

Vector3 ViewCameraGL::getDirection(){
        Vector3 dir = __center - __eye;
        dir.normalize();
        dir = Matrix3::axisRotation(Vector3::OY,-__elevation * GEOM_RAD ) * dir;
        dir = Matrix3::axisRotation(Vector3::OZ,__azimuth  * GEOM_RAD ) * dir;
        dir.normalize();
        if(!__geomsys) dir = geom2gl(dir);
        return dir;
}

Vector3 ViewCameraGL::getUp(){
        Vector3 up   (Vector3::OZ);
        up = Matrix3::axisRotation(Vector3::OY,-__elevation * GEOM_RAD ) * up;
        up = Matrix3::axisRotation(Vector3::OZ,__azimuth  * GEOM_RAD ) * up;

        if(!__geomsys) up = geom2gl(up);
        return up;
}

Vector3 ViewCameraGL::getRight(){
        Vector3 dir = __center - __eye;
        dir.normalize();
        Vector3 right = cross(dir,Vector3::OZ);
        right = Matrix3::axisRotation(Vector3::OY,-__elevation * GEOM_RAD ) * right;
        right = Matrix3::axisRotation(Vector3::OZ,__azimuth  * GEOM_RAD ) * right;


        if(!__geomsys) right = geom2gl(right);

        return right;
}

Matrix4 ViewCameraGL::getMatrix(){
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


QMenu *
ViewCameraGL::createToolsMenu(QWidget * parent)
{
  ViewCameraMenu * __CameraMenu = new ViewCameraMenu(this,parent);
  return __CameraMenu;
}


void
ViewCameraGL::addProperties(QTabWidget * tab)
{
  QWidget * mytabWidget = new QWidget(tab);
  Ui::CameraProperties * tab2 = new Ui::CameraProperties( );
  tab2->setupUi(mytabWidget);
  tab->addTab( mytabWidget, tr( "&Camera" ) );

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
  tab2->DefaultAngleEdit->setValue(__default_view_angle);
  QObject::connect(tab2->DefaultAngleEdit,SIGNAL(valueChanged(double)),this,SLOT(setViewAngle(double)));

  tab2->CurrentAngleEdit->setText(QString::number(__view_angle));
  QObject::connect(this,SIGNAL(currentViewAngleChanged(const QString&)),
                   tab2->CurrentAngleEdit,SLOT(setText(const QString&)));
  QObject::connect(this,SIGNAL(viewAngleChanged(double)),
                   tab2->DefaultAngleEdit,SLOT(setValue(double)));
  tab2->CurrentAngleEdit->setReadOnly(true);
  tab2->ProjectionBox->setCurrentIndex(int(__projectionmode));
  QObject::connect(tab2->ProjectionBox,SIGNAL(activated(int)),this,SLOT(setProjectionMode(int)));
  tab2->CoordinatesBox->setCurrentIndex(int(__geomsys));
  QObject::connect(tab2->CoordinatesBox,SIGNAL(activated(int)),this,SLOT(setCoordSys(int)));
  tab2->ResizePolicyBox->setCurrentIndex(int(__resizePolicy));
  QObject::connect(tab2->ResizePolicyBox,SIGNAL(activated(int)),this,SLOT(setResizePolicy(int)));
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
ViewCameraGL::fillToolBar(QToolBar * toolBar)
{
  QPixmap home(ViewerIcon::getPixmap(ViewerIcon::home));
  QPixmap persp(ViewerIcon::getPixmap(ViewerIcon::perspective));
  QAction * bt = toolBar->addAction(home,tr("Home Position"),this, SLOT(home()));
  bt->setWhatsThis(tr("<b>Home Position</b><br><br>"
    "Set the view to the Initial Position.<br>"
    "You can also use Menu <br><b>Tools > Camera > Home</b><br>"));

  bt = toolBar->addAction(persp,tr("Perspective Camera"),
                         this, SLOT(changeCameraMode()));
  bt->setWhatsThis(tr("<b>Projection</b><br><br>"
    "Camera can be <b>Perspective</b> or <b>Orthographic</b>.<br><br>"
    "Setting this button <i>On</i> make the camera in Perspective mode, "
    "<i>Off</i> in Orthographic mode<br><br>"
    "You can also use Menu <br><b>Tools > Camera > Projection</b><br>"
    ));
   bt->setCheckable(true) ;
   bt->setChecked(__projectionmode);
   QObject::connect(this,SIGNAL(projectionChanged(bool)),bt,SLOT(setChecked(bool)));
}

void
ViewCameraGL::fillToolBarAdditionnal(QToolBar * toolBar)
{
  QPixmap eye(ViewerIcon::getPixmap(ViewerIcon::eye));
  QPixmap eyef(ViewerIcon::getPixmap(ViewerIcon::eyef));
  drawArrow(&eye,2);
  QBitmap mask=eye.mask();
  drawArrow(&mask,2);

  QAction * bt = toolBar->addAction(eye,tr("Camera Position"));
  QMenu * selctmenu = new QMenu(toolBar);
  selctmenu->addAction(tr("Front View (YZ)"),this,SLOT(YZView()));
  selctmenu->addAction(tr("Right View (XZ)"),this,SLOT(XZView()));
  selctmenu->addAction(tr("Top View (XY)"),this,SLOT(XYView()));
  bt->setMenu(selctmenu);
  // bt2->setPopupDelay(0);

  bt = toolBar->addAction(eyef,tr("Fit to Window"),this, SLOT(reDim()));
  bt->setWhatsThis(tr("<b>Fit to Window</b><br><br>"
    "Set the view to fit the scene.<br>"
    "You can also use Menu <br><b>Tools > Camera > Fit to Window</b><br>"));

}

void ViewCameraGL::save() const
{
  QString filename = ViewFileManager::getSaveFileName("","","",__frame);
  if(!filename.isEmpty()){
    save(filename);
  }
}

void
ViewCameraGL::read()
{
  QString filename = QFileDialog::getOpenFileName(__frame,"Read Camera","");
  if(!filename.isEmpty()){
    read(filename);
  }
}

#define OV(s,v) s << " " << v.x() << " " << v.y() << " " << v.z()

void ViewCameraGL::save(const QString& filename) const
{
    QFile file(filename);
    if(file.open( QIODevice::WriteOnly )) {
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

void ViewCameraGL::read(const QString& filename)
{
    QFile file(filename);
    if(file.exists() && file.open( QIODevice::ReadOnly )) {
    QTextStream stream ( &file );

        stream >> __azimuth >> __elevation;
        printf("%f\n",__azimuth);
        printf("%f\n",__elevation);
        stream >> __stepMove;
        printf("%f\n",__stepMove);

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
