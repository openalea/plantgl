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


/*! \file view_rotcenter.h
    \brief Definition of the viewer class ViewRotCenterGL.
*/

#ifndef __view_rotcenter_h__
#define __view_rotcenter_h__

/* ----------------------------------------------------------------------- */

#include "object.h"

#include <plantgl/algo/opengl/util_gl.h>
#include <plantgl/math/util_vector.h>

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
#else
    #include <QtGui/qmenu.h>
#endif


/* ----------------------------------------------------------------------- */

namespace Ui { class RotCenterEdit; }

/* ----------------------------------------------------------------------- */

class QDockWidget;

/* ----------------------------------------------------------------------- */

/**
   \class ViewRotCenterGL
   \brief A ViewRotCenterGL for GL Display

*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewRotCenterGL  : public ViewRelativeObjectGL
{
  Q_OBJECT
  Q_PROPERTY(int X READ x WRITE setX );
  Q_PROPERTY(int Y READ y WRITE setY );
  Q_PROPERTY(int Z READ z WRITE setZ );
  Q_PROPERTY(bool Visible READ isVisible WRITE changeVisibility );
  Q_PROPERTY(bool Active READ isActive WRITE changeActivation );
public:

  /// Constructor.
  ViewRotCenterGL(ViewCameraGL *camera,
          QOpenGLWidget * parent=0,
          const char * name=0);

  /// Destructor.
  virtual ~ViewRotCenterGL();

  /// Return whether self is visible.
  bool isVisible() const ;

  /// Return whether self is active.
  bool isActive() const;

  /// x coordinates
  int x() const;

  /// y coordinates
  int y() const;

  /// z coordinates
  int z() const;

  /// get the sliders that control this.
  QDockWidget * getSliders() const;

  /// Create a Tools menu that reflect the functionality of this.
  virtual QMenu * createToolsMenu(QWidget * parent);

  /// Fill toolBar to reflect the functionality of this.
  void fillToolBar(QToolBar * toolBar);

public slots:

  /// reinitialize value.
  void init();

  /// show
  void show();

  /// hide
  void hide();

  /// Change Visibility
  void changeVisibility();

  /// Change Visibility
  void changeVisibility(bool);

  /// activate.
  void activate();

  /// center.
  void center();

  /// desactivate.
  void desactivate();

  /// Change Activation
  void changeActivation();

  /// Change Activation
  void changeActivation(bool);

  /// Set X Coordinates of this to \e x
  void setX(int x);

  /// Set Y Coordinates of this to \e y
  void setY(int y);

  /// Set Z Coordinates of this to \e z
  void setZ(int z);

  /// Set X Coordinates of this to \e x
  void setX(double x);

  /// Set Y Coordinates of this to \e y
  void setY(double y);

  /// Set Z Coordinates of this to \e z
  void setZ(double z);

  /// Initialize the rotating center.
  virtual void initializeGL();

  /// GL command for rotating center.
  virtual void paintGL();

  /// Change relative value.
  virtual void changeStepEvent(double newStep, double oldStep);

  signals:

  /// emit when visibility changed.
  void visibilityChanged(bool);

  /// emit when activation changed
  void activationChanged(bool);

  /// emit when X Coordinates changed.
  void XvalueChanged(int);

  /// emit when Y Coordinates changed.
  void YvalueChanged(int);

  /// emit when Z Coordinates changed.
  void ZvalueChanged(int);

  /// emit when X Coordinates changed.
  void XvalueChanged(double);

  /// emit when Y Coordinates changed.
  void YvalueChanged(double);

  /// emit when Z Coordinates changed.
  void ZvalueChanged(double);

protected :

  void setSliderStep(const int step);

  /// Sliders to control self coordinates.
  QDockWidget * __sliders;
  Ui::RotCenterEdit * __editor;

  /// Activation of this.
  bool __active;

  /// Visibility if this.
  bool __visible;

  /// Coordinates.
  PGL(Vector3) __position;

  /// display list.
  GLuint __displayList;

};

/* ----------------------------------------------------------------------- */
#endif

