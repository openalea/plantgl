/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): C. Nouguier & F. Boudon
 *                       N. Dones & B. Adam
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


/*! \file view_controlpanel.h
    \brief Definition of the viewer class ViewControlPanel.
*/


#ifndef __view_controlpanel_h__
#define __view_controlpanel_h__

/* ----------------------------------------------------------------------- */

#include "util_qwidget.h"
#include "view_icons.h"

/* ----------------------------------------------------------------------- */

class QCheckBox;
class QDialog;
class QSlider;
class QLineEdit;
class QGroupBox;
class QPushButton;
class QDial;
class QSpinBox;
/* ----------------------------------------------------------------------- */

class ViewGLFrame;

/* ----------------------------------------------------------------------- */

/**   
   \class ViewControlPanel
   \brief The Control Panel of the viewer
*/

/* ----------------------------------------------------------------------- */

class GEOM_API ViewControlPanel : public ViewToolBar
{
  Q_OBJECT
public:
    
  /// Constructor.
  ViewControlPanel( ViewGLFrame * _glframe, 
		const QString & label, 
		QMainWindow * mw, 
#if QT_VERSION >= 300
		ToolBarDock = DockTop,
#else
		QMainWindow::ToolBarDock dock = QMainWindow::Right, 
#endif
		bool newLine = TRUE, 
		const char * name = 0 );

  /// Destructor.
  ~ViewControlPanel();
  
  bool spinBoxUsed() const;
    
public slots:

  /// This event handler is called when the orientation of the widget is changed.
  virtual void orientationChangedEvent( Orientation orientation );

 /// Change the Light Mode.
  void setLightEnable(bool b);

  /// Set the light ambient color.
  void setLightAmbient(const QColor&);

  /// Select a light ambient color.
  void selectLightAmbient();

  /// Set the light diffuse color.
  void setLightDiffuse(const QColor&);

  /// Select a light diffuse color.
  void selectLightDiffuse();

  /// Set the light specular color.
  void setLightSpecular(const QColor&);

  /// Select a light specular color.
  void selectLightSpecular();

  void useSpinBox(bool);
  void setLinked(bool);

 signals:
  
  /// Emit when user has select a new color for light.
  void lightAmbientChanged(const QColor&);
  /// Emit when user has select a new color for light.
  void lightDiffuseChanged(const QColor&);
  /// Emit when user has select a new color for light.
  void lightSpecularChanged(const QColor&);

protected:

  /// Function called when the widget is moved.
  virtual void moveEvent ( QMoveEvent * event);

  /// Function called when the widget is resized.
  virtual void resizeEvent( QResizeEvent * event); 


private:


  /// Scene Position Group Box.
  QGroupBox * __sceneGroupBox;

  /// LineEdit for Scene Elevation.
  // QLineEdit* __elevation;
  QDial* __elevation;
  QSpinBox * __elevationSpin;
  /// LineEdit for Scene Azimuth.
  //QLineEdit* __azimuth;
  QDial* __azimuth;
  QSpinBox * __azimuthSpin;
  /// LineEdit for Scene Zoom.
  QSpinBox* __zoom;
  QPushButton * __linkedZoom;

  /// Light Position Group Box.
  QGroupBox * __lightPosGroupBox;

  /// Light Position Group Box.
  QGroupBox * __lightColGroupBox;

  QDial * __lightElevation;
  QSpinBox * __lightElevationSpin;
  QDial * __lightAzimuth;
  QSpinBox * __lightAzimuthSpin;

  /// Light Ambient Color.
  QPushButton* __LightAmbientButton;
  QColor __LightAmbient;
  /// Light Diffuse Color.
  QPushButton* __LightDiffuseButton;
  QColor __LightDiffuse;
  /// Light Specular Color.
  QPushButton* __LightSpecularButton;
  QColor __LightSpecular;

  /// Step Position Group Box.
  QGroupBox * __gridGroupBox;

  bool __useSpinBox;
};


/* ----------------------------------------------------------------------- */

// __view_controlpanel_h__
#endif

