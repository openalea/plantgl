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

/*! \file view_light.h
    \brief Definition of the viewer class ViewLightGL3.
*/

#ifndef __view_light_h__
#define __view_light_h__

/* ----------------------------------------------------------------------- */

#include <qcolor.h>
#include <qpopupmenu.h>
#include <scenegraph/geometry/boundingbox.h>
#include "object.h"

TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

class QString;
class ViewLightGL3;

/* ----------------------------------------------------------------------- */

/// Menu to control a ViewLightGL3.
class VIEW3_API ViewLightMenu3 : public QPopupMenu {
   Q_OBJECT
 
public :
  
  ViewLightMenu3(ViewLightGL3 * light, QWidget * parent=0, const char * name=0);
  
  ~ViewLightMenu3();

public slots :

  void setVisibility(const bool);

protected :

  int idVisibility;

};

/* ----------------------------------------------------------------------- */


/**   
   \class ViewLightGL3
   \brief A ViewLightGL3 for GL Display

*/
class VIEW3_API ViewLightGL3  : public ViewRelativeObjectGL3
{
  Q_OBJECT
  Q_PROPERTY(double Azimuth READ getAzimuth WRITE setAzimuth );
  Q_PROPERTY(double Elevation READ getElevation WRITE setElevation );
  Q_PROPERTY(double Distance READ getDistance WRITE setDistance );
  Q_PROPERTY(QColor Ambient READ getAmbient WRITE setAmbient );
  Q_PROPERTY(QColor Diffuse READ getDiffuse WRITE setDiffuse );
  Q_PROPERTY(QColor Specular READ getSpecular WRITE setSpecular );

public:

  /// Constructor.
  ViewLightGL3(ViewCameraGL3 *camera, QGLWidget * parent=0, const char * name=0);

  /// Destructor.
  virtual ~ViewLightGL3();
  
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

  /// Create a Popup menu that reflect the functionality of this.
  virtual QPopupMenu * createToolsMenu(QWidget * parent);

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
  void enable(); 
  /// GL command for disabling Light.
  void disable(); 

signals:

  /// azimuth changed signal.
  void azimuthChanged(int);
  /// elevation changed signal.
  void elevationChanged(int);
  /// distance changed.
  void distanceChanged(int);
  /// Ambient color changed.
  void ambientChanged(const QColor&);
  /// Diffuse color changed.
  void diffuseChanged(const QColor&);
  /// Specular color changed.
  void specularChanged(const QColor&);
  /// visibility changed.
  void visibilityChanged(const bool);

protected :
  /// Paint Light material.
  void gllightMaterial();

  virtual void changeStepEvent(const int newStep, const int oldStep);

  double __azimuth;
  double __elevation;
  double __distance;
  QColor __ambient;
  QColor __diffuse;
  QColor __specular;
  bool __show;
};

/* ----------------------------------------------------------------------- */
#endif

