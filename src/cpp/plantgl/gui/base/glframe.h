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



/*! \file view_glframe.h
    \brief Definition of the viewer class ViewGLFrame.
*/



#ifndef __view_glframe_h__
#define __view_glframe_h__

/* ----------------------------------------------------------------------- */
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/QToolButton>
#else
    #include <QtGui/QToolButton>
#endif
class QToolBar;
class ViewStatusBar;
class QMenu;
class QSlider;
class QMainWindow;
class QTabWidget;

/* ----------------------------------------------------------------------- */

#include <QtOpenGL/qgl.h>
#ifdef QT_NO_OPENGL
#warning Qt compiled without openGL support
#endif
#include <QtCore/QTimer>

#include <plantgl/tool/util_types.h>
#include <vector>
#include <ctime>

/* ----------------------------------------------------------------------- */

class ViewCameraGL;
class ViewLightGL;
class ViewGridGL;
class ViewRotCenterGL;
class ViewClippingPlaneGL;
class ViewFogGL;
class ViewRendererGL;
class ViewErrorDialog;
class ViewEvent;
#include "util_qwidget.h"
#include "flags.h"
#include "zbuffer.h"
//class ViewRayBuffer;
//class ViewZBuffer;
#include <plantgl/math/util_vector.h>

TOOLS_USING(Vector3)

class QGLPixelBuffer;

/* ----------------------------------------------------------------------- */

/**
   \class ViewGLFrame
   \brief An OpenGL Frame

*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewGLFrame : public QGLWidget
{
  Q_OBJECT
  Q_PROPERTY( QColor BgColor READ getBackGroundColor WRITE setBackGroundColor )

    public:

      enum Mode { Rotation, Selection, MultipleSelection };

  /// Last GL Frame created.
  static ViewGLFrame * LAST_GL_FRAME;

  /// Constructor
  ViewGLFrame( QWidget* parent,
           const char* name = NULL,
           ViewRendererGL * r = 0,
           const QGLWidget * shareWidget = 0 );

  /// Destructor
  ~ViewGLFrame();

  /// Get the Camera.
  ViewCameraGL * getCamera() const;

  /// Get the Light.
  ViewLightGL * getLight() const;

  /// Get the Grid.
  ViewGridGL * getGrid() const;

  /// Get the Rotating Center.
  ViewRotCenterGL * getRotCenter() const;

  /// Get the Clipping Planes.
  ViewClippingPlaneGL * getClippingPlanes() const;

  /// Get the Fog.
  ViewFogGL * getFog() const;

  /// Get the SceneRenderer.
  ViewRendererGL * getSceneRenderer() const;

  /// Set the SceneRenderer.
  void setSceneRenderer(ViewRendererGL *);

  /// return the current background color.
  const QColor& getBackGroundColor() const;

  /// return the current line width.
  real_t getLineWidth() const { return __linewidth; }

  /// Create Edit menu that reflect the functionality of this.
  QMenu * createEditMenu(QWidget * parent);

  /// Create Tools menu that reflect the functionality of this.
  QMenu * createToolsMenu(QWidget * parent);

  /// Fill toolBar to reflect the functionality of this.
  void fillToolBar(QToolBar * toolBar);

  /// Add other toolbar.
  void addOtherToolBar(QMainWindow * menu);

  /** ! Print the current Image in the format \b _format
      in the file \b _filename. */
  void saveImage(QString _filename,const char * _format, bool withAlpha = false );
  // copy the current image to the global clipboard

  /// Grab depth and color buffer
  ViewZBuffer * grabZBuffer( bool all_values = true ) ;

  /// Grab depth buffer
  ViewZBuffer * grabDepthBuffer( bool all_values = true  ) ;

  /// Grab depth and color points
  std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)> grabZBufferPoints( ) ;


  /// Grab depth and color points
  std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)> grabZBufferPointsWithJitter(float jitter, int raywidth, bool mixcolor = true ) ;


  ViewRayBuffer * castRays( const Vector3& position,
                             const Vector3& direction,
                             const Vector3& dx,
                             const Vector3& dy,
                             int sx, int sy) ;


  int getProjectionPixel();
  double getPixelWidth();
  double getProjectionSize(int* nbpixel = NULL, double* pixelwidth = NULL);

  std::vector<std::pair<uint_t,uint_t> >
  getProjectionPixelPerColor(double* pixelwidth = NULL);

  virtual void connectTo(ViewStatusBar *);

  virtual void connectTo(ViewErrorDialog *);

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

  void endEvent();

  virtual bool event(QEvent *e);

  void activatePBuffer(bool b);
  QGLPixelBuffer * getPBuffer();

  bool isPixelBufferUsed()  { return __usePBuffer; }

  void makeItCurrent();

  void activateRedraw(bool b);
  bool isRedrawEnabled() { return __redrawEnabled; }

public slots:

  void redrawGL();
  void printProjectionSize();
  void paintPixelBuffer();

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

  void setMode(ViewGLFrame::Mode);

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

  void glTwoSideShadeModel(bool b);

  void glDithering(bool b);

  void glDepthTest(bool b);

  void glNormalization(bool b);

  void setAnimation(eAnimationFlag b);

  void useOcclusionQuery(bool b);

  void usePixelBuffer(bool b);

  void setFPSDisplay(bool b);

  void showMessage(const QString message, int timeout = 0);

  void cleanMessage();

signals:

  /// Emit when renderer change.
  void rendererChanged();

  /// Selection.
  void selectedShape(uint_t i);
  void selectedShapes(std::vector<uint_t> i);

  /// Emit when mode changed.
  void modeChanged(ViewGLFrame::Mode);
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
  void reinitializeGL();

  /// Paint the ViewGLFrame.
  virtual void paintGL();

  /// Selection in the ViewGLFrame.
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
  virtual void customEvent(QEvent *e);

  virtual void wheelEvent ( QWheelEvent * );
  virtual void focusInEvent ( QFocusEvent * ) ;

  protected slots:
  void updateMessage();

  public:

  virtual void gridEvent ( ViewEvent * ) ;
  virtual void cameraEvent ( ViewEvent * ) ;
  virtual void clippingPlaneEvent(ViewEvent * e);

 // virtual void glDraw();

 private:


  /// The camera.
  ViewCameraGL * __camera;

  /// The light.
  ViewLightGL * __light;

  /// The grid.
  ViewGridGL * __grid;

  /// The rotating center.
  ViewRotCenterGL * __rotCenter;

  /// The clipping plane.
  ViewClippingPlaneGL * __clippingPlane;

  /// The fog.
  ViewFogGL * __fog;

  /// scene.
  ViewRendererGL * __scene;

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
  ViewToolBar * __linedialog;
  /// LineWidth dialog slider
  // QSlider * __lineslider;

  bool __useOcclusionQuery;

  QGLPixelBuffer * __pixelbuffer;
  bool __pBufferActivated;
  bool __usePBuffer;

  bool __redrawEnabled;

  QString __message;
  float __msg_transparency;
  float __msg_transparency_step;

  QTimer __timer;

  bool __fpsDisplay;

  clock_t __lastdraw;
  double __fps;
  int __fpscounter;
};

class VIEW_API ViewDoubleToolButton : public QToolButton {
  Q_OBJECT

public :

  ViewDoubleToolButton
            ( const QPixmap & pm,
              const QPixmap & pm2,
              const QString & textLabel,
              QObject * receiver,
              const char * slot,
              QToolBar * parent);

public slots:

  void setButton(ViewGLFrame::Mode);

protected:
  QPixmap __pm1;
  QPixmap __pm2;
};

/* ----------------------------------------------------------------------- */
#endif

