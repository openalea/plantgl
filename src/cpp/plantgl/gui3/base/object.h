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
    \brief Definition of the viewer class ViewObjectGL3.
*/


#ifndef __view_object_h__
#define __view_object_h__

/* ----------------------------------------------------------------------- */

#include <qobject.h>
#include "../gui3_config.h"

/* ----------------------------------------------------------------------- */

class QGLWidget;
class QPopupMenu;
class QToolBar;
class QPoint;

/* ----------------------------------------------------------------------- */

class ViewErrorDialog3;
class ViewStatusBar3;

/* ----------------------------------------------------------------------- */

/**   
   \class ViewObjectGL3
   \brief Abstact class of object for GL Display

*/

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewObjectGL3  : public QObject
{
  Q_OBJECT

public:
  
  /// Constructor.
  ViewObjectGL3(QObject * parent=0, const char * name=0);

  /// Constructor. Connect GL Frame to \e this.
  ViewObjectGL3(QGLWidget * parent, const char * name=0);

  /// Destructor.
  virtual ~ViewObjectGL3();

  /// Create a Popup menu that reflect the functionality of this.
  virtual QPopupMenu * createToolsMenu(QWidget * parent);
  
  /// Fill a toolBar that reflect the functionality of this.
  virtual void fillToolBar(QToolBar * toolBar);

  /// Connect this to a statusbar.
  virtual void connectTo(ViewStatusBar3 *);

  /// Connect this to a GL Widget.
  virtual void connectTo(QGLWidget *);

  /// Connect this to a error dialog.
  virtual void connectTo(ViewErrorDialog3 *);

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
#define GEOM_GL_ERROR_FROM(widget) ViewObjectGL3::glError(widget,__FILE__,__LINE__)

  /// Test if there is any gl error for ViewObjectGL3
#define GEOM_GL_ERROR glError(__FILE__,__LINE__)

/* ----------------------------------------------------------------------- */

class ViewCameraGL3;

/**   
   \class ViewRelativeObjectGL3
   \brief Abstact class of object for GL Display connected to the step of the camera.
   This object have a relative representation.

*/
class VIEW3_API ViewRelativeObjectGL3  : public ViewObjectGL3
{
  Q_OBJECT

public:
  
  /// Constructor. 
  ViewRelativeObjectGL3(ViewCameraGL3 *camera, QObject * parent=0, const char * name=0);

  /// Constructor.
  ViewRelativeObjectGL3(ViewCameraGL3 *camera, QGLWidget * parent, const char * name=0);

  

  /// Destructor.
  virtual ~ViewRelativeObjectGL3();

  virtual void changeStepEvent(const int newStep, const int oldStep);

  virtual void geomCoordSys();

  virtual void glCoordSys();

  const int getStep() const;

  /// Connect this to a statusbar.
  virtual void connectTo(ViewStatusBar3 *);

  /// Connect this to a GL Widget.
  virtual void connectTo(QGLWidget *);

  /// Connect this to a error dialog.
  virtual void connectTo(ViewErrorDialog3 *);

  /// Connect this to a camera.
  virtual void connectTo(ViewCameraGL3 *);

private slots:

  /// setStep.
  void setStep(int);

  void coordSys(int);


private :

  int __step;
};

/* ----------------------------------------------------------------------- */

#endif

