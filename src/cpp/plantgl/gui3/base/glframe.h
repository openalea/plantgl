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


/*! \file view_glframe.h
    \brief Definition of the viewer class ViewGLFrame3.
*/



#ifndef __view_glframe_h__
#define __view_glframe_h__

/* ----------------------------------------------------------------------- */

#include <qtoolbutton.h>
class QToolBar;
class ViewStatusBar3;
class QPopupMenu;
class QSlider;
class QMainWindow;
class QTabWidget;

/* ----------------------------------------------------------------------- */

#include <qgl.h>
#ifdef QT_NO_OPENGL
#warning Qt compiled without openGL support
#endif

#include <plantgl/tool/util_types.h>
#include <vector>

/* ----------------------------------------------------------------------- */

class ViewCameraGL3;
class ViewLightGL3;
class ViewGridGL3;
class ViewRotCenterGL3;
class ViewClippingPlaneGL3;
class ViewFogGL3;
class ViewRendererGL3;
class ViewErrorDialog3;
class ViewGridEvent3;
class ViewCameraEvent3;
#include "util_qwidget.h"
// #include "zbuffer.h"
class ViewRayBuffer3;
class ViewZBuffer3;
#include <plantgl/math/util_vector.h>

TOOLS_USING(Vector3)

/* ----------------------------------------------------------------------- */

/**
   \class ViewGLFrame3
   \brief An OpenGL Frame

*/

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewGLFrame3 : public QGLWidget
{
  Q_OBJECT
  Q_PROPERTY( QColor BgColor READ getBackGroundColor WRITE setBackGroundColor )

    public:

	  enum Mode { Rotation, Selection, MultipleSelection };

  /// Last GL Frame created.
  static ViewGLFrame3 * LAST_GL_FRAME;
  static ViewGLFrame3 * CURRENT_GL_FRAME;

  /// Constructor
  ViewGLFrame3( QWidget* parent,
           const char* name = NULL,
           ViewRendererGL3 * r = 0,
           const QGLWidget * shareWidget = 0 );

  /// Destructor
  ~ViewGLFrame3();

  /// Get the Camera.
  ViewCameraGL3 * getCamera() const;

  /// Get the Light.
  ViewLightGL3 * getLight() const;

  /// Get the Grid.
  ViewGridGL3 * getGrid() const;

  /// Get the Rotating Center.
  ViewRotCenterGL3 * getRotCenter() const;

  /// Get the Clipping Planes.
  ViewClippingPlaneGL3 * getClippingPlanes() const;

  /// Get the Fog.
  ViewFogGL3 * getFog() const;

  /// Get the SceneRenderer.
  ViewRendererGL3 * getSceneRenderer() const;

  /// Set the SceneRenderer.
  void setSceneRenderer(ViewRendererGL3 *);

  /// return the current background color.
  const QColor& getBackGroundColor() const;
  real_t getLineWidth() const { return __linewidth; }

  /// Create Edit menu that reflect the functionality of this.
  QPopupMenu * createEditMenu(QWidget * parent);

  /// Create Tools menu that reflect the functionality of this.
  QPopupMenu * createToolsMenu(QWidget * parent);

  /// Fill toolBar to reflect the functionality of this.
  void fillToolBar(QToolBar * toolBar);

  /// Add other toolbar.
  void addOtherToolBar(QMainWindow * menu);

  /** ! Print the current Image in the format \b _format
      in the file \b _filename. */
  void saveImage(QString _filename,const char * _format, bool withAlpha = false );
  // copy the current image to the global clipboard

  /// Grab depth and color buffer
  ViewZBuffer3 * grabZBuffer( bool all_values = true ) ;

  /// Grab depth buffer
  ViewZBuffer3 * grabDepthBuffer( bool all_values = true  ) ;

  ViewRayBuffer3 * castRays( const Vector3& position, 
						     const Vector3& direction, 
						     const Vector3& dx, 
							 const Vector3& dy,
						     int sx, int sy) ;


  int getProjectionPixel();
  double getPixelWidth();
  double getProjectionSize(int* nbpixel = NULL, double* pixelwidth = NULL);
  
  virtual void connectTo(ViewStatusBar3 *);

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
  /// Display progress.
  void progress(int progress,int totalStep);

  void rendererStatus();

  public slots:

  void printProjectionSize();

  /// Set the Line Width.
  void setLineWidth(int i);

  /// set the color \b _color as the new background color.
  void setBackGroundColor(const QColor &);

  /// Ask and change the Background Color.
  void setBackground();

  /// Test if any GL error occur.
  bool glError(const char * file /*= NULL*/, int line /*= 0*/);

  void setMultipleSelectionMode();

  void setSelectionMode();

  void setLastSelectionMode();

  void setRotationMode();

  void setMode(ViewGLFrame3::Mode);

 // void changeMode();

  void clearSelection();

  void copyImageToClipboard();
  void copyImageToClipboardWithAlpha();
  void printImage();

  void addProperties(QTabWidget * tab);

  void glCullNoFace(bool b);

  void glCullBackFace(bool b);

  void glCullFrontFace(bool b);

  void glSmoothShadeModel(bool b);

  void glFlatShadeModel(bool b);

  void glDithering(bool b);

  void glDepthTest(bool b);

  void glNormalization(bool b);

  void animation(bool b);

signals:

  /// Emit when renderer change.
  void rendererChanged();

  /// Selection.
  void selectedShape(uint_t i);
  void selectedShapes(std::vector<uint_t> i);

  /// Emit when mode changed.
  void modeChanged(ViewGLFrame3::Mode);
  void selectionMode(bool);

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

  void initMessage(const QString&);

  void progressMessage(int, int);

  protected:

  /// Set up the OpenGL rendering state, and define display list.
  virtual void initializeGL();

  /// Paint the ViewGLFrame3.
  virtual void paintGL();

  /// Selection in the ViewGLFrame3.
  virtual void selectGL();
  virtual void multipleSelectGL(const QPoint&);

  /// Set up the OpenGL view port, matrix mode, etc.
  virtual void resizeGL( int w, int h );

  /// This event handler is called when a mouse press event is in progress for this widget.
  virtual void mousePressEvent( QMouseEvent*);

  /// This event handler is called when a mouse move event is in progress for this widget.
  virtual void mouseMoveEvent( QMouseEvent*);

  /// This event handler is called when a mouse release event is in progress for this widget.
  virtual void mouseReleaseEvent(QMouseEvent*);

  /// This event handler is called when a drag is in progress and the mouse enters this widget.
  virtual void dragEnterEvent(QDragEnterEvent* event);

  /// This event handler is called when the drag is dropped on this widget.
  virtual void dropEvent(QDropEvent* event);

  /// This event handler is called when a key is pressed for this widget.
  virtual void keyPressEvent ( QKeyEvent * e);

  /// This event handler is called when a key is pressed for this widget.
  virtual void keyReleaseEvent ( QKeyEvent * e);

  /// This event handler is called when a new scene is asked to be shown.
  virtual void customEvent(QCustomEvent *e);

  virtual void wheelEvent ( QWheelEvent * );
  virtual void focusInEvent ( QFocusEvent * ) ;

  public:

  virtual void gridEvent ( ViewGridEvent3 * ) ;
  virtual void cameraEvent ( ViewCameraEvent3 * ) ;

 // virtual void glDraw();

 private:


  /// The camera.
  ViewCameraGL3 * __camera;

  /// The light.
  ViewLightGL3 * __light;

  /// The grid.
  ViewGridGL3 * __grid;

  /// The rotating center.
  ViewRotCenterGL3 * __rotCenter;

  /// The clipping plane.
  ViewClippingPlaneGL3 * __clippingPlane;

  /// The fog.
  ViewFogGL3 * __fog;

  /// scene.
  ViewRendererGL3 * __scene;

  /// Mouse coordinate.
  QPoint __mouse;

  /// Current color of the background.
  QColor __BgColor;

  /// Line Width
  GLfloat __linewidth;

  /// Mode of the viewer : 0 = rotation, 1 = selection.
  Mode __mode;
  Mode __lastSelectionMode;

  QRect * __selectionRect;

  /// LineWidth dialog
  ViewToolBar3 * __linedialog;
  /// LineWidth dialog slider
  QSlider * __lineslider;

};

class VIEW3_API ViewDoubleToolButton3 : public QToolButton {
  Q_OBJECT

public :

  ViewDoubleToolButton3
			( const QPixmap & pm,
			  const QPixmap & pm2,
			  const QString & textLabel, 
			  const QString & grouptext, 
			  QObject * receiver, 
			  const char * slot, 
			  QToolBar * parent, 
  			  const char * name = 0 );

public slots:

  void setButton(ViewGLFrame3::Mode);

protected:
  QPixmap __pm1;
  QPixmap __pm2;
};

/* ----------------------------------------------------------------------- */
#endif

