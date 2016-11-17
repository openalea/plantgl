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

/*! \file view_light.h
    \brief Definition of the viewer class ViewLightGL.
*/

#ifndef __view_light_h__
#define __view_light_h__

/* ----------------------------------------------------------------------- */

#include <QtGui/qcolor.h>
#include <QtGlobal>
#if QT_VERSION >= 0x050000 
    #include <QtWidgets/qmenu.h>
#else
    #include <QtGui/qmenu.h>
#endif
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include "object.h"

TOOLS_USING_NAMESPACE

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
  ViewLightGL(ViewCameraGL *camera, QGLWidget * parent=0, const char * name=0);

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

