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


/*! \file view_object.h
    \brief Definition of the viewer class ViewObjectGL.
*/


#ifndef __view_object_h__
#define __view_object_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"

/* ----------------------------------------------------------------------- */

#include <QtCore/QObject>
#include <QtCore/QPoint>
class QGLWidget;
class QMenu;
class QToolBar;

/* ----------------------------------------------------------------------- */

class ViewErrorDialog;
class ViewStatusBar;

/* ----------------------------------------------------------------------- */

/**
   \class ViewObjectGL
   \brief Abstact class of object for GL Display

*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewObjectGL  : public QObject
{
  Q_OBJECT

public:

  /// Constructor.
  ViewObjectGL(QObject * parent=0, const char * name=0);

  /// Constructor. Connect GL Frame to \e this.
  ViewObjectGL(QGLWidget * parent, const char * name=0);

  /// Destructor.
  virtual ~ViewObjectGL();

  /// Create a Popup menu that reflect the functionality of this.
  virtual QMenu * createToolsMenu(QWidget * parent);

  /// Fill a toolBar that reflect the functionality of this.
  virtual void fillToolBar(QToolBar * toolBar);

  /// Fill a toolBar that reflect the functionality of this.
  virtual void fillToolBarAdditionnal(QToolBar * toolBar);

  /// Connect this to a statusbar.
  virtual void connectTo(ViewStatusBar *);

  /// Connect this to a GL Widget.
  virtual void connectTo(QGLWidget *);

  /// Connect this to a error dialog.
  virtual void connectTo(ViewErrorDialog *);

  /// end event.
  virtual void endEvent() {};

  /// Display error message.
  void error(const QString&);
  /// Display warning message.
  void warning(const QString&);
  /// Display info message.
  void info(const QString&);
  /// Display status message.
  void status(const QString&);
  /// Display status message for int millisecond.
  void status(const QString&,int);
  void progress(int progress,int totalStep);

  /// Test if there is any gl error.
  bool glError(const char * file = NULL, int line = 0 ) const ;

  /// Test if there is any gl error.
  static bool glError( QWidget * widget, const char * file = NULL, int line = 0 ) ;
  static bool BATCHMODE;

public slots:

  /// Move
  virtual void moving(int dx, int dy);
  /// Move
  void move(QPoint p);
  /// Zoom
  virtual void zooming(int dx, int dy);
  /// Zoom
  void zoom(QPoint p);
  /// Rotate
  virtual void rotating(int dx, int dy);
  /// Rotate
  void rotate(QPoint p);

  /// Initialize the camera
  virtual void initializeGL();
  /// Resize
  virtual void resizeGL(int w, int h);
  /// GL command.
  virtual void paintGL() = 0;

signals:

  /// Value changed signal.
  void valueChanged();

  /// Signal emit when some error where found.
  void errorMessage(const QString&);

  /// Signal emit when some warning where found.
  void warningMessage(const QString&);

  /// Signal emit when some info where found.
  void infoMessage(const QString&);

  /// Status message.
  void statusMessage(const QString&);

  /// Status message.
  void statusMessage(const QString&,int);

  void progressMessage(int, int);

protected:

  /// frame GL.
  QGLWidget * __frame;

};

/* ----------------------------------------------------------------------- */

  /// Test if there is any gl error.
#define GEOM_GL_ERROR_FROM(widget) ViewObjectGL::glError(widget,__FILE__,__LINE__)

  /// Test if there is any gl error for ViewObjectGL
#define GEOM_GL_ERROR glError(__FILE__,__LINE__)

/* ----------------------------------------------------------------------- */

class ViewCameraGL;

/**
   \class ViewRelativeObjectGL
   \brief Abstact class of object for GL Display connected to the step of the camera.
   This object have a relative representation.

*/
class VIEW_API ViewRelativeObjectGL  : public ViewObjectGL
{
  Q_OBJECT

public:

  /// Constructor.
  ViewRelativeObjectGL(ViewCameraGL *camera, QObject * parent=0, const char * name=0);

  /// Constructor.
  ViewRelativeObjectGL(ViewCameraGL *camera, QGLWidget * parent, const char * name=0);



  /// Destructor.
  virtual ~ViewRelativeObjectGL();

  virtual void changeStepEvent(double newStep, double oldStep);

  virtual void geomCoordSys();

  virtual void glCoordSys();

  const int getStep() const;

  /// Connect this to a statusbar.
  virtual void connectTo(ViewStatusBar *);

  /// Connect this to a GL Widget.
  virtual void connectTo(QGLWidget *);

  /// Connect this to a error dialog.
  virtual void connectTo(ViewErrorDialog *);

  /// Connect this to a camera.
  virtual void connectTo(ViewCameraGL *);

private slots:

  /// setStep.
  void setStep(double);

  void coordSys(int);


private :

  double __step;
};

/* ----------------------------------------------------------------------- */

#endif

