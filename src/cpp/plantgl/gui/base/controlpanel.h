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

