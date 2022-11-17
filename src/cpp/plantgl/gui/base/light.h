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


/*! \file view_light.h
    \brief Definition of the viewer class ViewLightGL.
*/

#ifndef __view_light_h__
#define __view_light_h__

/* ----------------------------------------------------------------------- */

#include <QtGui/qcolor.h>
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
#else
    #include <QtGui/qmenu.h>
#endif
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include "object.h"

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

class QString;
class ViewEvent;

/* ----------------------------------------------------------------------- */


/**
   \class ViewLightGL
   \brief A ViewLightGL for GL Display

*/
class VIEW_API ViewLightGL  : public ViewRelativeObjectGL
{
  Q_OBJECT
  Q_PROPERTY(double Azimuth READ getAzimuth WRITE setAzimuth );
  Q_PROPERTY(double Elevation READ getElevation WRITE setElevation );
  Q_PROPERTY(double Distance READ getDistance WRITE setDistance );
  Q_PROPERTY(QColor Ambient READ getAmbient WRITE setAmbient );
  Q_PROPERTY(QColor Diffuse READ getDiffuse WRITE setDiffuse );
  Q_PROPERTY(QColor Specular READ getSpecular WRITE setSpecular );
  Q_PROPERTY( bool Enable READ isEnabled WRITE setEnabled )
public:

  /// Constructor.
  ViewLightGL(ViewCameraGL *camera, QOpenGLBaseWidget * parent=0, const char * name=0, PGLOpenGLFunctionsPtr ogl = NULL);

  /// Destructor.
  virtual ~ViewLightGL();

  /// Get Azimuth value.
  double getAzimuth() const {
    return __azimuth;
  }
  /// Get Elevation value.
  double getElevation() const {
    return __elevation;
  }
  /// Get Distance value.
  double getDistance() const {
    return __distance;
  }
  /// Get Ambient.
  const QColor& getAmbient() const{
    return __ambient;
  }

  /// Get Diffuse.
  const QColor& getDiffuse() const{
    return __diffuse;
  }
  /// Get Specular.
  const QColor& getSpecular() const{
    return __specular;
  }

  const bool isVisible() const {
    return __show;
  }

  /// Get Position value.
  Vector3 getPosition() const;

  /// Set Position value.
  void setPosition(const Vector3&);

  /// Create a Popup menu that reflect the functionality of this.
  virtual QMenu * createToolsMenu(QWidget * parent);
  void  fillToolBar(QToolBar * toolBar);

  void lightEvent(ViewEvent * event);

  /// Return whether Rendering with light is enable.
  bool isEnabled() const;

public slots:

  /// Set Light to Home Position.
  void home();
  /// Set Light on X Axis
  void XAxis();
  /// Set Light on Y Axis
  void YAxis();
  /// Set Light on Z Axis
  void ZAxis();
  /// Show a representation of the Light
  void show();
  /// Show a representation of the Light
  void show(bool);
  /// Dont Show a representation of the Light
  void hide();
  /// change the visibility of a representation of the Light
  void changeVisibility();

  /// Set Azimuth value.
  void setAzimuth(double azimuth);
  /// Set Elevation value.
  void setElevation(double elevation);
  /// Set Distance value
  void setDistance(double distance);

  /// Set Azimuth value.
  void setAzimuth(int azimuth);
  /// Set Elevation value.
  void setElevation(int elevation);
  /// Set Distance value.
  void setDistance(int distance);
  /// Set Ambient Color
  void setAmbient(const QColor& color);
  /// Set Diffuse Color
  void setDiffuse(const QColor& color);
  /// Set Specular Color
  void setSpecular(const QColor& color);

  /// Move light
  virtual void moving(int dx, int dy);
  /// Zoom
  virtual void zooming(int dx, int dy);
  /// Rotate
  virtual void rotating(int dx, int dy);

  /// Initialize the light
  virtual void initializeGL();
  /// GL command for Light.
  virtual void paintGL();
  /// GL command for enabling Light.
  void switchOn();
  /// GL command for disabling Light.
  void switchOff();

  void setEnabled(bool);
  void toggleEnabled();

signals:

  /// azimuth changed signal.
  void azimuthChanged(double);
  /// elevation changed signal.
  void elevationChanged(double);
  /// distance changed.
  void distanceChanged(double);
  /// Ambient color changed.
  void ambientChanged(const QColor&);
  /// Diffuse color changed.
  void diffuseChanged(const QColor&);
  /// Specular color changed.
  void specularChanged(const QColor&);
  /// visibility changed.
  void visibilityChanged(const bool);
  /// Emit when light enable status change.
  void enabledChanged(bool);

protected :
  /// Paint Light material.
  void gllightMaterial();

  virtual void changeStepEvent(double newStep, double oldStep);

  double __azimuth;
  double __elevation;
  double __distance;
  QColor __ambient;
  QColor __diffuse;
  QColor __specular;
  bool __show;
  bool __enable;
};

/* ----------------------------------------------------------------------- */
#endif

