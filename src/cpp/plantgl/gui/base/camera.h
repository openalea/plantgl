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


/*! \file view_camera.h
    \brief Definition of the viewer class ViewCameraGL.
*/

#ifndef __view_camera_h__
#define __view_camera_h__

/* ----------------------------------------------------------------------- */
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
#else
    #include <QtGui/qmenu.h>
#endif

#include <QtCore/qstring.h>

#include <plantgl/scenegraph/geometry/boundingbox.h>
#include "object.h"

TOOLS_USING(Vector3)
TOOLS_USING(Matrix4)
PGL_USING(BoundingBoxPtr)
/* ----------------------------------------------------------------------- */

class QTabWidget;
class ViewCameraGL;
class ViewEvent;

/// Menu to control a ViewCameraGL
class VIEW_API ViewCameraMenu : public QMenu {
   Q_OBJECT

public :

  ViewCameraMenu(ViewCameraGL * camera, QWidget * parent=0, const char * name=0);

  ~ViewCameraMenu();

public slots :
  /// Set Camera Mode to Perspective Projection.
  void setProjectionMode(bool);

  /// Set GEOM Coordinates Sytem.
  void setCoordSys(int);

protected :

  QAction * idGLcoord;
  QAction * idGEOMcoord;
  QAction * idPerspective;
  QAction *  idOrtho;
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
  Q_PROPERTY(double StepMove READ getStepMove WRITE setStepMove );

public:

   enum eCameraResizePolicy {
     eKeepObjectSize,
     eKeepViewAngle
   };

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
  double getStepMove() const {
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
  virtual QMenu * createToolsMenu(QWidget * parent);

  void fillToolBar(QToolBar * toolBar);

  void fillToolBarAdditionnal(QToolBar * toolBar);

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

  virtual void cameraEvent(ViewEvent *);

  void setAngles(double azimuth, double elevation);

  /// end event.
  virtual void endEvent();

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
  void setDistanceX(double dist);
  /// Set Y Distance from center
  void setDistanceY(double dist);

  /// Set Center
  void setCenter(const Vector3&);

  /// Set View Angle.
  void setViewAngle(double angle);

  // Set the Policy for resizing
  void setResizePolicy(int);

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
  void setStepMove(double step);

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
  void azimuthChanged(double);
  /// elevation changed signal.
  void elevationChanged(double);
  /// zoom changed.
  void zoomChanged(double);
  /// near Plane changed.
  void nearPlaneChanged(const QString&);
  /// far Plane changed.
  void farPlaneChanged(const QString&);
  ///  view Angle changed.
  void viewAngleChanged(double);
  ///  current View Angle Changed.
  void currentViewAngleChanged(const QString&);

  /// stepMove changed.
  void stepMoveChanged(double);
  ///  projection changed.
  void projectionChanged(bool);
  ///  coord system changed.
  void coordSysChanged(int);

  void lockDimChanged(bool);
  void needReDim();

protected :
  void init();
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

  void updateActualViewAngle();

  double __azimuth;
  double __elevation;

  /*double __zoom;
  double __dist_x;
  double __dist_y*/;

  double __stepMove;
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

  eCameraResizePolicy __resizePolicy;
};

/* ----------------------------------------------------------------------- */
#endif

