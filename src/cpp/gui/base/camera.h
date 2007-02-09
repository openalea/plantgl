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

/*! \file view_camera.h
    \brief Definition of the viewer class ViewCameraGL.
*/

#ifndef __view_camera_h__
#define __view_camera_h__

/* ----------------------------------------------------------------------- */

#include <qstring.h>
#include <qpopupmenu.h>

#include <scenegraph/geometry/boundingbox.h>
#include "object.h"

TOOLS_USING(Vector3)
TOOLS_USING(Matrix4)
PGL_USING(BoundingBoxPtr)
/* ----------------------------------------------------------------------- */

class QTabWidget;
class ViewCameraGL;
class ViewCameraEvent;

/// Menu to control a ViewCameraGL
class VIEW_API ViewCameraMenu : public QPopupMenu {
   Q_OBJECT
 
public :
  
  ViewCameraMenu(ViewCameraGL * camera, QWidget * parent=0, const char * name=0);
  
  ~ViewCameraMenu();
  
public slots :
  /// Set Camera Mode to Perspective Projection.
  void setProjectionMode(bool);
  
  /// Set GEOM Coordinates Sytem.
  void setCoordSys(int);

  void setLock(bool);
  
protected :    
  
  int idGLcoord;
  int idGEOMcoord;
  int idchgcoord;
  int idPerspective;
  int idOrtho;
  int idchgProj;
  int idLock;
  QPopupMenu * __coordMenu;
  QPopupMenu * __projectionMenu;
};

/* ----------------------------------------------------------------------- */

/**   
   \class ViewCameraGL
   \brief A ViewCameraGL for GL Display

*/
class VIEW_API ViewCameraGL  : public ViewObjectGL
{
  Q_OBJECT
  Q_PROPERTY(double Azimuth READ getAzimuth WRITE setAzimuth );
  Q_PROPERTY(double Elevation READ getElevation WRITE setElevation );
  Q_PROPERTY(double Zoom READ getZoom WRITE setZoom );
  Q_PROPERTY(double DistX READ getDistX WRITE setDistanceX );
  Q_PROPERTY(double DistY READ getDistY WRITE setDistanceY );
  Q_PROPERTY(bool ProjectionMode READ getProjectionMode WRITE setProjectionMode );
  Q_PROPERTY(bool CoordSys READ getCoordSys WRITE setCoordSys );
  Q_PROPERTY(int StepMove READ getStepMove WRITE setStepMove );

public:


  /// Constructor.
  ViewCameraGL(QGLWidget * parent=0, const char * name=0);

  /// Constructor.
  ViewCameraGL(QObject * parent, const char * name=0);


  /// Destructor.
  virtual ~ViewCameraGL();
  
  /// Get View Angle value.
  const double& getViewAngle() const {
    return __view_angle;
  }

  /// Get View Angle value.
  const double& getDefaultViewAngle() const {
    return __default_view_angle;
  }
  /// Get Azimuth value.
  const double& getAzimuth() const {
    return __azimuth;
  }
  /// Get Elevation value.
  const double& getElevation() const {
    return __elevation;
  }
  /// Get Zoom value.
  const double getZoom() const {
    return __translation.x();
  }
  /// Get distX value.
  const double getDistX() const {
    return __translation.y();
  }
  /// Get distY value.
  const double getDistY() const {
    return __translation.z();
  }
  /// Get Step Move.
  int getStepMove() const {
    return __stepMove;
  }

  /// Get Eye position.
  const Vector3& getEye() const {
    return __eye;
  }

  /// Get Center position.
  const Vector3& getCenter() const {
    return __center;
  }

  /// Get Distance translation.
  const Vector3& getTranslation() const {
    return __translation;
  }

  /// Compute a camera to display the volume of bbox.
  void buildCamera(const BoundingBoxPtr& bbox, bool force = false);

  /// Get the projection mode : true -> perspective, false -> orthographic
  bool getProjectionMode() const {
    return __projectionmode;
  }

  /// Get the coordinate system : true -> GEOM, false -> GL
  bool getCoordSys() const {
    return __geomsys;
  }

  bool isDimLock() const { return __lockdim; }

  /// Create a Tools menu that reflect the functionality of this.
  virtual QPopupMenu * createToolsMenu(QWidget * parent);

  void fillToolBar(QToolBar * toolBar);

  void addProperties(QTabWidget * tab);

  /// Get The Camera position 
  Vector3 getPosition();

  /// Get The Camera direction
  Vector3 getDirection();

  /// Get The Camera Up vector
  Vector3 getUp();

  /// Get The Camera Right vector 
  Vector3 getRight();

  /// Get The Camera matrix
  Matrix4 getMatrix();

  virtual void cameraEvent(ViewCameraEvent *);

  void setAngles(double azimuth, double elevation);

public slots:

  /// Set Camera to Home Position.
  void home();
  /// Set Camera to XY View Position
  void XYView();
  /// Set Camera to XZ View Position 
  void XZView();
  /// Set Camera to YZ View Position
  void YZView();
  
  /// Set Azimuth value.
  void setAzimuth(int);
  /// Set Elevation value.
  void setElevation(int);
  /// Set Zoom value.
  void setZoom(int);
  /// Set Azimuth value.
  void setAzimuth(double);
  /// Set Elevation value.
  void setElevation(double);
  /// Set Zoom value.
  void setZoom(double);

  /// Set X Distance from center
  void setDistanceX(const double& dist);
  /// Set Y Distance from center
  void setDistanceY(const double& dist);

  /// Set Center
  void setCenter(const Vector3&);
  
  /// Set View Angle.
  void setViewAngle(double angle);

  /// Set Position
  void setPosition(const Vector3&);
  void setPosition(const Vector3&,double az,double el);
  void lookAt(const Vector3& pos,const Vector3& target);
  void setPosition(const QString&);
  void lookIn(const Vector3& pos,const Vector3& dir);

  /// Set Azimuth value.
  // void setAzimuth(const QString&);
  /// Set Elevation value.
  // void setElevation(const QString&);
  /// Set Zoom value.
  // void setZoom(const QString&);
  /// Set View Angle.
  void setViewAngle(const QString&);
  /// Set Far Plane.
  void setFarPlane(const QString&);
  /// Set Near Plane.
  void setNearPlane(const QString&);
  /// Valid View Angle value set.
  void validViewAngle();
  /// Valid value set.
  void validValue();

  /// Set Step Move
  void setStepMove(const QString& step);
  /// Set Step Move
  void setStepMove(int step);
  
  /// Move camera
  virtual void moving(int dx, int dy);
  /// Zoom
  virtual void zooming(int dx, int dy);
  /// Rotate
  virtual void rotating(int dx, int dy);

  /// Initialize the camera
  virtual void initializeGL();
  /// Resize
  virtual void resizeGL(int w, int h);
  /// GL command for Camera.
  virtual void paintGL(); 


  /// GL command for Selection Camera.
  void beginSelectGL(int x, int y);
  /// GL command for Selection Camera.
  void beginSelectGL(const QPoint& point);
  /// GL command for Selection Camera.
  void beginSelectGL(const QRect& region);
  /// GL command for Selection Camera.
  void endSelectGL();

  /// Change Camera Mode.
  void changeCameraMode();
  /// Change Camera Mode.
  void setProjectionMode(int);
  void setProjectionMode(bool);
  /// Set Camera Mode to Perspective Projection.
  void setPerspectiveMode();
  /// Set Camera Mode to Orthographic Projection.
  void setOrthographicMode();
  /// Change of Coordinates Sytem.
  void changeCoordSys();
  /// Change of Coordinates Sytem.
  void setCoordSys(int);
  void setCoordSys(bool);
  /// Set GEOM Coordinates Sytem.
  void setGEOMCoordSys();
  /// Set GL Coordinates Sytem.
  void setGLCoordSys();
  
  void lockDim(int);
  void lockDim(bool);
  void lockDim();
  void reDim();

  void save() const;
  void read();

  void nextView();

signals:

  /// azimuth changed signal.
  void azimuthChanged(int);
  /// elevation changed signal.
  void elevationChanged(int);
  /// zoom changed.
  void zoomChanged(int);
  /// near Plane changed.
  void nearPlaneChanged(const QString&);
  /// far Plane changed.
  void farPlaneChanged(const QString&);
  ///  view Angle changed.
  void viewAngleChanged(const QString&);
  ///  current View Angle Changed.
  void currentViewAngleChanged(const QString&);

  /// stepMove changed.
  void stepMoveChanged(int);
  ///  projection changed.
  void projectionChanged(bool);
  ///  coord system changed.
  void coordSysChanged(int);

  void lockDimChanged(bool);
  void needReDim();

protected :
  /// Initialize the camera
  void initialize(const PGL(BoundingBoxPtr)& bbox);
  /// Initialize the camera
  void initialize(const double& _radius,const Vector3& center);

  void save(const QString& file) const;
  void read(const QString& file);
  
  /// Initialise and Push Projection Matrix
  void glInitProjectionMatrix();
  /// Push Projection Matrix
  void glPushProjectionMatrix();
  /// Initialise and Push Model View Matrix
  void glInitModelViewMatrix();
  /// Push Model View Matrix
  void glPushModelViewMatrix();

  double __azimuth;
  double __elevation;
  
  /*double __zoom; 
  double __dist_x;
  double __dist_y*/;

  int __stepMove;
  Vector3 __center;
  Vector3 __eye;
  Vector3 __translation;

  double __radius;
  double __far_plane;
  double __near_plane;
  double __default_view_angle;
  double __view_angle;
  double __width;
  double __height;
  bool __projectionmode;
  bool __geomsys;
  bool __lockdim;
};

/* ----------------------------------------------------------------------- */
#endif

