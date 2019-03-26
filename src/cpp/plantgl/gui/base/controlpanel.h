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


/*! \file view_controlpanel.h
    \brief Definition of the viewer class ViewControlPanel.
*/


#ifndef __view_controlpanel_h__
#define __view_controlpanel_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qdockwidget.h>
#else
    #include <QtGui/qdockwidget.h>
#endif
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
namespace Ui { class ViewControlPanelWidget; }

/* ----------------------------------------------------------------------- */

/**
   \class ViewControlPanel
   \brief The Control Panel of the viewer
*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewControlPanel : public QDockWidget
{
  Q_OBJECT
public:

  /// Constructor.
  ViewControlPanel( ViewGLFrame * _glframe, const QString & label, QMainWindow * mw );

  /// Destructor.
  ~ViewControlPanel();

public slots:

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

  void setLinked(bool);

 signals:

  /// Emit when user has select a new color for light.
  void lightAmbientChanged(const QColor&);
  /// Emit when user has select a new color for light.
  void lightDiffuseChanged(const QColor&);
  /// Emit when user has select a new color for light.
  void lightSpecularChanged(const QColor&);



private:


  /// Light Ambient Color.
  QColor __LightAmbient;
  /// Light Diffuse Color.
  QColor __LightDiffuse;
  /// Light Specular Color.
  QColor __LightSpecular;

  Ui::ViewControlPanelWidget * __panel;
  bool __useSpinBox;
};


/* ----------------------------------------------------------------------- */

// __view_controlpanel_h__
#endif

