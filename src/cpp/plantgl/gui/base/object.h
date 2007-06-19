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

/*! \file view_object.h
    \brief Definition of the viewer class ViewObjectGL.
*/


#ifndef __view_object_h__
#define __view_object_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"

/* ----------------------------------------------------------------------- */

#include <qobject.h>
#include <qpoint.h>
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
  static bool BASHMODE;

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

