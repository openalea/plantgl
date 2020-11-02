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

#define GL_SILENCE_DEPRECATION

#include <iomanip>

#include "../gui_config.h"

#include <QtCore/qfile.h>
#include <QtCore/qvariant.h>
#include <QtCore/qurl.h>
#include <QtCore/QHash>
#include <QtCore/QTimer>


#include <QtGui/QMouseEvent>
#include <QtGui/qpainter.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qclipboard.h>
#include <QtGui/qcursor.h>

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qcolordialog.h>
    #include <QtWidgets/qslider.h>
    #include <QtWidgets/qmessagebox.h>
    #include <QtWidgets/qtoolbutton.h>
    #include <QtWidgets/qtoolbar.h>
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qapplication.h>
    #include <QtWidgets/qstatusbar.h>
    #include <QtWidgets/qprogressbar.h>
    #include <QtWidgets/qlabel.h>
    #include <QtWidgets/qwhatsthis.h>
    #include <QtWidgets/qbuttongroup.h>
    #include <QtWidgets/qradiobutton.h>
    #include <QtWidgets/qtabwidget.h>
    #include <QtWidgets/qmainwindow.h>

    #include <QtPrintSupport/qprinter.h>
    #include <QtPrintSupport/qprintdialog.h>

    #include <QtCore/QMimeData>
#else
    #include <QtGui/qcolordialog.h>
    #include <QtGui/qslider.h>
    #include <QtGui/qmessagebox.h>
    #include <QtGui/qtoolbutton.h>
    #include <QtGui/qtoolbar.h>
    #include <QtGui/qmenu.h>
    #include <QtGui/qapplication.h>
    #include <QtGui/qstatusbar.h>
    #include <QtGui/qprogressbar.h>
    #include <QtGui/qlabel.h>
    #include <QtGui/qprintdialog.h>
    #include <QtGui/qwhatsthis.h>
    #include <QtGui/qbuttongroup.h>
    #include <QtGui/qradiobutton.h>
    #include <QtGui/qtabwidget.h>
    #include <QtGui/qmainwindow.h>

    #include <QtGui/qprinter.h>
#endif

#include <QtOpenGL/QGLPixelBuffer>

#include "glframe.h"
#include "icons.h"
#include "camera.h"
#include "light.h"
#include "grid.h"
#include "rotcenter.h"
#include "clippingplane.h"
#include "fog.h"
#include "errordialog.h"
#include "util_qwidget.h"
#include "configuration.h"
#include "interface/ui_gloptions.h"
#include "interface/ui_linewidthcontrol.h"

#include "event.h"
// #include "editgeomscenegl.h"
#include "scenegl.h"
#include "zbuffer.h"

#include <plantgl/algo/opengl/util_glu.h>

// PGL_USING_NAMESPACE

using namespace std;
// using namespace STDEXT;

#define MS_EDITION
#define GL_ERROR ViewObjectGL::glError(this,__FILE__,__LINE__)
#define GL_SIMPLECHECK_ERROR {GLenum glerror; if((glerror = glGetError()) != GL_NO_ERROR) printf("%s:%i:%s\n", __FILE__,__LINE__,gluGeomErrorString(glerror));}
// glError(__FILE__,__LINE__)

/*  ------------------------------------------------------------------------ */

ViewGLFrame * ViewGLFrame::LAST_GL_FRAME = NULL;

/*  ------------------------------------------------------------------------ */
#ifndef __APPLE__
#define WITH_OCCLUSION_QUERY
#endif

#ifdef WITH_OCCLUSION_QUERY

#ifndef GL_ARB_occlusion_query
    #define GL_ARB_occlusion_query

    #ifndef APIENTRY
    #define APIENTRY
    #endif
    #ifndef APIENTRYP
    #define APIENTRYP APIENTRY *
    #endif

    #define GL_QUERY_COUNTER_BITS_ARB         0x8864
    #define GL_CURRENT_QUERY_ARB              0x8865
    #define GL_QUERY_RESULT_ARB               0x8866
    #define GL_QUERY_RESULT_AVAILABLE_ARB     0x8867
    #define GL_SAMPLES_PASSED_ARB             0x8914

    typedef void (APIENTRYP PFNGLGENQUERIESARBPROC)(GLsizei, GLuint *);
    typedef void (APIENTRYP PFNGLDELETEQUERIESARBPROC) (GLsizei, const GLuint *);
    typedef void (APIENTRYP PFNGLBEGINQUERYARBPROC)   (GLenum, GLuint);
    typedef void (APIENTRYP PFNGLENDQUERYARBPROC)   (GLenum);
    typedef void (APIENTRYP PFNGLGETQUERYOBJECTIVARBPROC)   (GLuint, GLenum, GLint *);

#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE                    0x809D
#endif

static PFNGLGENQUERIESARBPROC          glGenQueriesARB = NULL;
static PFNGLDELETEQUERIESARBPROC       glDeleteQueriesARB = NULL;
static PFNGLBEGINQUERYARBPROC          glBeginQueryARB = NULL;
static PFNGLENDQUERYARBPROC            glEndQueryARB = NULL;
static PFNGLGETQUERYOBJECTIVARBPROC    glGetQueryObjectivARB = NULL;

bool hasGLExtension(char * extname){
   QString extensions = (char *)glGetString(GL_EXTENSIONS);
   return extensions.contains(extname);
}

bool glInitOcclusionQuery(){
    static bool tested = false;
    static bool hasOcclusionQuery = false;
    if (!tested){
        tested = true;
#ifdef GL_ARB_occlusion_query
        hasOcclusionQuery = hasGLExtension("GL_ARB_occlusion_query")  ;
        if(!hasOcclusionQuery)printf("OcclusionQuery not supported\n");
        else{
            glGenQueriesARB = (PFNGLGENQUERIESARBPROC)QGLContext::currentContext()->getProcAddress("glGenQueriesARB");
            assert(glGenQueriesARB);
            glDeleteQueriesARB = (PFNGLDELETEQUERIESARBPROC)QGLContext::currentContext()->getProcAddress("glDeleteQueriesARB");
            assert(glDeleteQueriesARB);
            glBeginQueryARB = (PFNGLBEGINQUERYARBPROC)QGLContext::currentContext()->getProcAddress("glBeginQueryARB");
            assert(glBeginQueryARB);
            glEndQueryARB = (PFNGLENDQUERYARBPROC)QGLContext::currentContext()->getProcAddress("glEndQueryARB");
            assert(glEndQueryARB);
            glGetQueryObjectivARB = (PFNGLGETQUERYOBJECTIVARBPROC)QGLContext::currentContext()->getProcAddress("glGetQueryObjectivARB");
            assert(glGetQueryObjectivARB);
        }
#endif
    }
    return hasOcclusionQuery;
}

#endif

/*  ------------------------------------------------------------------------ */

/// Create a ViewGLFrame widget
ViewGLFrame::ViewGLFrame( QWidget* parent, const char* name, ViewRendererGL * r, const QGLWidget * shareWidget) :
  QGLWidget(  QGLFormat( QGL::AlphaChannel ), parent, shareWidget ),
  __camera(0),
  __light(0),
  __grid(0),
  __rotCenter(0),
  __clippingPlane(0),
  __fog(0),
  __scene(r),
  __BgColor(170,170,170),
  __linewidth(1),
  __mode(Rotation),
  __lastSelectionMode(Selection),
  __linedialog(0),
  __selectionRect(0),
  __useOcclusionQuery(false),
  __pixelbuffer(0),
  __usePBuffer(true),
  __pBufferActivated(false),
  __redrawEnabled(true),
  __timer(this),
  __fpsDisplay(false),
  __fps(-1),
  __fpscounter(0)
{
    if(name)setObjectName(name);

  setAttribute(Qt::WA_DeleteOnClose, false);

  /// Creation
  __camera = new ViewCameraGL(this,"Camera");
  __light = new ViewLightGL(__camera,this,"Light");
  __grid = new ViewGridGL(__camera,this,"Grid");
  __rotCenter = new ViewRotCenterGL(__camera,this,"Rotating Center");
  __clippingPlane = new ViewClippingPlaneGL(this,"Clipping Planes");
  __fog = new ViewFogGL(__camera,this,"Fog");

  LAST_GL_FRAME = this;

  ViewerSettings settings;
  settings.beginGroup("FrameGL");
  __BgColor = settings.value("BgColor",__BgColor).value<QColor>();
  __usePBuffer = settings.value("PixelBuffer",__usePBuffer).toBool();
  __fpsDisplay = settings.value("FPSDisplay",__fpsDisplay).toBool();
  setBackGroundColor(__BgColor);
  settings.endGroup();

  /// Connexion
  rendererStatus();
  __scene->connectTo(this);
  QObject::connect(__scene,SIGNAL(changeMode(ViewGLFrame::Mode)),
                   this,SLOT(setMode(ViewGLFrame::Mode)));
  if(__scene){
    ViewSceneRendererGL * sc = dynamic_cast<ViewSceneRendererGL *>(__scene);
    if(sc){
      sc->setCamera(__camera);
      sc->setLight(__light);
    }
  }
  QObject::connect(__fog,SIGNAL(colorChanged(const QColor&)),
                   this,SLOT(setBackGroundColor(const QColor&)));

  /// Qt Option
  setAcceptDrops(true);
  setFocusPolicy(Qt::StrongFocus);


  __timer.setInterval(200);
  __timer.setSingleShot(false);
  QObject::connect(&__timer,SIGNAL(timeout()),this,SLOT(updateMessage()));

}


/// Release allocated resources
ViewGLFrame::~ViewGLFrame() {

  if(__scene)delete __scene;
  if(__pixelbuffer) delete __pixelbuffer;

#ifdef  PGL_DEBUG
    cout << "GL Frame deleted" << endl;
#endif
}

void ViewGLFrame::endEvent()
{
  ViewerSettings settings;
  settings.beginGroup("FrameGL");
  settings.setValue("BgColor",__BgColor);
  settings.setValue("PixelBuffer",__usePBuffer);
  settings.setValue("FPSDisplay",__fpsDisplay);
  settings.endGroup();
  if(__scene)__scene->endEvent();
  if(__grid)__grid->endEvent();
  if(__camera)__camera->endEvent();
  if(__light)__light->endEvent();
  if(__rotCenter)__rotCenter->endEvent();
  if(__clippingPlane)__clippingPlane->endEvent();
  if(__fog)__fog->endEvent();
}

/*  ------------------------------------------------------------------------ */

ViewCameraGL *
ViewGLFrame::getCamera() const
{
  return __camera;
}

ViewLightGL *
ViewGLFrame::getLight() const
{
  return __light;
}

ViewGridGL *
ViewGLFrame::getGrid() const
{
  return __grid;
}

ViewRotCenterGL *
ViewGLFrame::getRotCenter() const
{
  return __rotCenter;
}

ViewClippingPlaneGL *
ViewGLFrame::getClippingPlanes() const
{
  return __clippingPlane;
}

ViewFogGL *
ViewGLFrame::getFog() const
{
  return __fog;
}

ViewRendererGL *
ViewGLFrame::getSceneRenderer() const
{
  return __scene;
}

void
ViewGLFrame::setSceneRenderer(ViewRendererGL * s)
{
  if(__scene) delete __scene;
  __scene = s;
  __scene->connectTo(this);
  QObject::connect(__scene,SIGNAL(changeMode(ViewGLFrame::Mode)),
                   this,SLOT(setMode(ViewGLFrame::Mode)));
  if(__scene){
    ViewSceneRendererGL * sc = dynamic_cast<ViewSceneRendererGL *>(__scene);
    if(sc){
      sc->setCamera(__camera);
      sc->setLight(__light);
    }
  }
  rendererStatus();
  emit rendererChanged();
}

QGLPixelBuffer * ViewGLFrame::getPBuffer() { return __pixelbuffer; }

void
ViewGLFrame::rendererStatus() {
  if(__scene)
  emit initMessage(QString("Renderer ... ")+__scene->metaObject()->className());
  else
  emit initMessage(QString("Renderer ... None"));
}


const QColor&
ViewGLFrame::getBackGroundColor() const
{
  return __BgColor;
}



/*  ------------------------------------------------------------------------ */


void
ViewGLFrame::connectTo(ViewStatusBar *s)
{
    if(s){
        QObject::connect(this,SIGNAL(statusMessage(const QString&,int)),
            s,SLOT(showMessage(const QString&,int)) );
        QObject::connect(this,SIGNAL(statusMessage(const QString&)),
            s,SLOT(showMessage(const QString&)) );
        QObject::connect(this,SIGNAL(progressMessage(int,int)),
            s,SLOT(setProgress(int,int)) );
        __camera->connectTo(s);
        __light->connectTo(s);
        __grid->connectTo(s);
        __rotCenter->connectTo(s);
        __clippingPlane->connectTo(s);
        __fog->connectTo(s);
    }
}

void
ViewGLFrame::connectTo(ViewErrorDialog *e)
{
  if(e){
    QObject::connect(this,SIGNAL(errorMessage(const QString&)),
             e,SLOT(setError(const QString&)) );
    QObject::connect(this,SIGNAL(warningMessage(const QString&)),
             e,SLOT(appendWarning(const QString&)) );
    QObject::connect(this,SIGNAL(infoMessage(const QString&)),
             e,SLOT(appendInfo(const QString&)) );
  __camera->connectTo(e);
  __light->connectTo(e);
  __grid->connectTo(e);
  __rotCenter->connectTo(e);
  __clippingPlane->connectTo(e);
  __fog->connectTo(e);
  }
}

void
ViewGLFrame::error(const QString& s)
{
  emit errorMessage(s);
}

void
ViewGLFrame::warning(const QString& s)
{
  emit warningMessage(s);
}

void
ViewGLFrame::info(const QString& s)
{
  emit infoMessage(s);
}

void
ViewGLFrame::status(const QString& s)
{
  emit statusMessage(s);
}

void
ViewGLFrame::status(const QString& s,int t)
{
  emit statusMessage(s,t);
}

void
ViewGLFrame::progress(int p,int t)
{
  emit progressMessage(p,t);
}

/*  ------------------------------------------------------------------------ */

void
ViewGLFrame::setBackground()
{
  QColor m = QColorDialog::getColor(__BgColor,this);
  if(m.isValid()){
    __BgColor=m;
  __fog->setColor(m);
    qglClearColor( __BgColor );
    updateGL();
  }
}

void
ViewGLFrame::setBackGroundColor(const QColor& color)
{
  __BgColor=color;
  __fog->setColor(color);
  if(isVisible()){
      qglClearColor(__BgColor);
      redrawGL();
      status(QString(tr("Set Background Color to")+" (%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue()),2000);
  }
}

bool ViewGLFrame::glError(const char * file , int line ){
  return ViewObjectGL::glError(this,__FILE__,__LINE__);
}

void
ViewGLFrame::setLineWidth(int i)
{
  __linewidth = (GLfloat)float(i);
  status(QString(tr("Set Line Width to")+" %1").arg(i),2000);
  __scene->refreshDisplay();
  // redrawGL();
}

void
ViewGLFrame::setMultipleSelectionMode()
{
  if(__mode != MultipleSelection){
      setCursor(QCursor( Qt::CrossCursor  ));
    __mode = MultipleSelection;
    __scene->changeModeEvent(__mode);
    __lastSelectionMode = __mode;
    emit modeChanged(__mode);
    emit selectionMode(true);
    status(tr("Mode Multiple Selection"),2000);
  }
}

void
ViewGLFrame::setSelectionMode()
{
  if(__mode != Selection){
      setCursor(QCursor( Qt::PointingHandCursor ));
    __mode = Selection;
    __scene->changeModeEvent(__mode);
    __lastSelectionMode = __mode;
    emit modeChanged(__mode);
    emit selectionMode(true);
    status(tr("Mode Selection"),2000);
  }
}

void
ViewGLFrame::setRotationMode()
{
  if(__mode !=Rotation){
      setCursor( Qt::ArrowCursor);
    __mode = Rotation;
    __scene->changeModeEvent(__mode);
    emit modeChanged(__mode);
    emit selectionMode(false);
  }
}

void
ViewGLFrame::setLastSelectionMode()
{
  setMode(__lastSelectionMode);
}

void
ViewGLFrame::setMode(Mode i)
{
  if(__mode !=i){
    __mode = i;
    if(__mode == Rotation){
      setCursor(Qt::ArrowCursor);
    }
    else if(__mode == Selection){
      setCursor(QCursor( Qt::PointingHandCursor ));
      status(tr("Continue Selection"),2000);
    }
    else if(__mode == MultipleSelection){
      setCursor(QCursor(Qt::CrossCursor  ));
      status(tr("Continue Multiple Selection"),2000);
    }
    __scene->changeModeEvent(__mode);
    emit modeChanged(__mode);
    emit selectionMode(__mode == Selection || __mode == MultipleSelection);
    __lastSelectionMode = __mode;
  }
}
/*
void
ViewGLFrame::changeMode()
{
  if(__mode == true){
    setCursor(Qt::ArrowCursor);
    __mode = false;
    __scene->rotationModeEvent();
    status(QString("Mode Rotation"),2000);
  }
  else {
      setCursor(QCursor( 13 ));
    __mode = true;
    __scene->selectionModeEvent();
    status(QString("Mode Selection"),2000);
  }
  emit modeChanged(__mode);
}
*/
void
ViewGLFrame::clearSelection()
{
  // __selected_shapes.clear();
  __scene->clearSelectionEvent();
  status(tr("Selection cleared"),5000);
}

void ViewGLFrame::activatePBuffer(bool b){
    static bool PBufferSupport = QGLPixelBuffer::hasOpenGLPbuffers();
    if(PBufferSupport && __usePBuffer) __pBufferActivated = b;
    // printf("activated pixel buffer : %i\n",__pBufferActivated);
    if(__pBufferActivated) {
      if (!__pixelbuffer || __pixelbuffer->size() != size()){
          if(__pixelbuffer) delete __pixelbuffer;
          __pixelbuffer = new QGLPixelBuffer(size(),format(),this);
      }
      __pixelbuffer->makeCurrent();
      reinitializeGL();
   }
}


void ViewGLFrame::activateRedraw(bool b) { __redrawEnabled = b; }

/*  ------------------------------------------------------------------------ */

void ViewGLFrame::makeItCurrent()
{
    if(__pBufferActivated)__pixelbuffer->makeCurrent();
    else makeCurrent();
}

void ViewGLFrame::redrawGL()
{
    if (__redrawEnabled) updateGL();
}

void ViewGLFrame::paintPixelBuffer(){
    bool pbufactivation = __pBufferActivated;
    if(!pbufactivation) activatePBuffer(true);
    else makeItCurrent();
    paintGL();
    if(!pbufactivation) activatePBuffer(false);
}


/*!
  Set up the OpenGL rendering state, and define display list
*/
void ViewGLFrame::initializeGL(){
   reinitializeGL();
  __camera->initializeGL();
  __grid->initializeGL();
  __rotCenter->initializeGL();
  __clippingPlane->initializeGL();
  __fog->initializeGL();
  GL_ERROR;
  __lastdraw = clock();
}

void ViewGLFrame::reinitializeGL()
{
  // clears the current GL context
  qglClearColor( __BgColor );

  /*
    Light settings
    IMPORTANT - GL_NORMALIZE is enabled because GL needs to re-normalize
    normals as we use anisotropic transformation such as scaling.
  */
  glEnable(GL_NORMALIZE);
  // glEnable(GL_RESCALE_NORMAL);
  __light->initializeGL();
  glShadeModel(GL_SMOOTH);

  /*
    Hidden-surfaces settings (ZBuffer)
  */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER,0.01);

  glEnable(GL_MULTISAMPLE);

  glEnable(GL_LIGHTING);
//  __light->enable();
  glEnable(GL_BLEND);

  // glEnable(GL_POLYGON_OFFSET_LINE);
  // glPolygonOffset(0.0, 0.0);

  __scene->initializeGL();


}


/*!
  Set up the OpenGL view port, matrix mode, etc.
  This function is called by Viewer::show() and when a window resize is done
  by the user.
*/

void ViewGLFrame::resizeGL( int w, int h )
{
  GL_ERROR;
  if (w == 0 || h == 0) { return; }
  __camera->resizeGL(w,h);
  GL_ERROR;
}



/*!
  Paint the box. The actual openGL commands for drawing the box are
  performed here.
*/

void ViewGLFrame::paintGL()
{
  // clock_t previousdraw = clock();
  if(!isVisible()) return ;
#ifndef Q_OS_MAC
  if (width() == 0 || height() == 0) { return; }
#endif
  if(__pBufferActivated) {
      if (!__pixelbuffer || __pixelbuffer->size() != size()){
          if(__pixelbuffer) delete __pixelbuffer;
          __pixelbuffer = new QGLPixelBuffer(size(),format(),this);
        __pixelbuffer->makeCurrent();
        reinitializeGL();
      }
  }

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_SIMPLECHECK_ERROR;
  __camera->paintGL();
  __light->paintGL();
  __fog->paintGL();
  glLineWidth(GLfloat(1.0));
  glPointSize(GLfloat(1.0));

  __rotCenter->paintGL();
  __clippingPlane->paintGL();
  __light->switchOff();
  __grid->paintGL();

  GL_ERROR;

  glLineWidth(__linewidth);
  glPointSize(__linewidth);

  if(__scene){
    __scene->paintGL();
    if(GL_ERROR)__scene->clear();
  }

  if (!__pBufferActivated) { // __pBufferActivated = false; }
  // else {
      if(__mode == MultipleSelection && __selectionRect){
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          glOrtho(0,width(),0,height(),-50,50);
          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();
          glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
          glColor3f(0.2,0.2,0.8);
          glEnable (GL_LINE_STIPPLE);
          glLineStipple(1,0x0FFF);
          glBegin(GL_LINE_LOOP);
          int x = __selectionRect->x();
          int y = height() - __selectionRect->y();
          glVertex3f(x,y,0);
          glVertex3f(x+__selectionRect->width(),y,0);
          glVertex3f(x+__selectionRect->width(),y-__selectionRect->height(),0);
          glVertex3f(x,y-__selectionRect->height(),0);
          glEnd();
          glDisable (GL_LINE_STIPPLE);
      }
      if (!__message.isEmpty()) {
          __light->switchOff();
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          int w = width();
          int h = height();
          int minw = w * 0.52, maxw = w * 0.98;
          int minh = h * 0.1;
          int deltaw = w * 0.4;
          QFontMetrics fm(font());
          QStringList msglines = __message.split('\n');
          int nblines = 0;
          for(QStringList::const_iterator itline = msglines.begin(); itline != msglines.end(); ++itline){
              int size = fm.width(*itline);
              nblines += 1+(size/deltaw);
          }
          int fmheigth = fm.height();
          int htext = nblines * fmheigth;
          int maxh = max(h * 0.3, h * 0.2 + htext);

          glOrtho(0,w,0,h,-50,50);
          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();
          glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
          glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
          glBegin(GL_QUADS);
          glColor4f(0.5,0.1,0.1,0.5*__msg_transparency);
          glVertex3f(minw,minh,0);
          glColor4f(0.9,0.1,0.1,0.9*__msg_transparency);
          glVertex3f(maxw,minh,0);
          glColor4f(0.5,0.1,0.1,0.5*__msg_transparency);
          glVertex3f(maxw,maxh,0);
          glColor4f(0.2,0.2,0.2,0.1*__msg_transparency);
          glVertex3f(minw,maxh,0);
          glEnd();
          glColor4f(1,1,1,0.5+0.5*__msg_transparency);
          int avgWidth = fm.averageCharWidth();
          int hmargin = (maxh-minh-htext)/2;
          int currenth = maxh - hmargin - fm.ascent();
          for(QStringList::const_iterator itline = msglines.begin(); itline != msglines.end(); ++itline){
              int size = fm.width(*itline);
              int cw = 0;
              int nbsubline = 1 + size/deltaw;
              int len = itline->size();
              if (nbsubline == 1){
                renderText(w * 0.55, currenth,1, *itline);
                currenth -= fmheigth;
              }
              else {
                  for (int subline = 0; subline < nbsubline; ++subline){
                      int lastw = cw + deltaw / avgWidth;
                      if (lastw > len) lastw = len;
                      renderText(w * 0.55, currenth,1, QString( itline->begin()+cw, lastw-cw));
                      currenth -= fmheigth;
                      cw = lastw;
                  }
              }
          }
          __light->switchOn();

      }
      if (__fpsDisplay){
          ++__fpscounter;
          if (__fpscounter == 5 || __fps < 0){
              clock_t previousdraw = __lastdraw;
              __lastdraw = clock();
              double elapsed_time = double(__lastdraw - previousdraw) / (__fpscounter* CLOCKS_PER_SEC);
              __fps = 1./elapsed_time;
              __fpscounter = 0;
          }
          if (__fps > 0) {
              __light->switchOff();
              qglColor(QColor(0,0,0));
              QString text("FPS : %1");
              if (__fps > 10 ) text = text.arg(int(__fps));
              else text = text.arg(double(__fps),0,'f',2);
              renderText(20, 20, text);
              __light->switchOn();
          }
      }
  }
  // glFinish();


}


/*!
  SelectGL
*/

void ViewGLFrame::selectGL()
{
  makeCurrent();

  GLint hits;
  GLsizei bufsize = 512;
  GLuint selectBuf[512];

  glSelectBuffer(bufsize,selectBuf);
  (void)glRenderMode(GL_SELECT);

  GL_ERROR;

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // transformations induites dans le repere du labo a partir
  // des coord de la souris dans le repere de la GL

  __camera->beginSelectGL(__mouse);

  glLineWidth(1);
  glPointSize(1);

  __rotCenter->paintGL();
  __clippingPlane->paintGL();

  glLineWidth(__linewidth);
  glPointSize(__linewidth);

  GL_ERROR;

  if (__scene)__scene->selectGL();

  __camera->endSelectGL();

  GL_ERROR;

  hits = glRenderMode(GL_RENDER);
  GLuint names, *ptr;
  ptr = (GLuint *) selectBuf;
  uint_t val, selectedval;
  float z, zmin;

  if(hits > 0){
    for(uint_t i = 0 ; i < (uint_t)hits ; i++){
      names = *ptr; //    cerr << "Number of names for this hits = " << names << endl;
      ptr++;
      z = (float)*ptr;        //    cerr << "z1 = " << *ptr << endl;
      if(i == 0)zmin = z;
      ptr++;        //    cerr << "z2 = " << *ptr << endl;
      ptr++;
      val = (uint_t)*ptr;
      if(i == 0)selectedval = val;
      else if(z < zmin){
          selectedval = val;
          zmin = z;
      }
      for(unsigned int j = 0 ; j < names ; j++)ptr++;
    }
    __scene->selectionEvent(selectedval);
    emit selectedShape(__scene->translateId(selectedval));
  }
#ifdef GEOM_DEBUG
  else warning("*** WARNING : hit miss all shapes");
#endif
}

void ViewGLFrame::multipleSelectGL(const QPoint& p)
{
  makeCurrent();

  GLint hits;
  GLsizei bufsize = 400000;
  GLuint selectBuf[400000];

  glSelectBuffer(bufsize,selectBuf);
  (void)glRenderMode(GL_SELECT);

  GL_ERROR;

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // transformations induites dans le repere du labo a partir
  // des coord de la souris dans le repere de la GL

  __camera->beginSelectGL(QRect(min(__mouse.x(),p.x()),
                                min(__mouse.y(),p.y()),
                                abs(__mouse.x()-p.x()),
                                abs(__mouse.y()-p.y())));

  glLineWidth(1);
  glPointSize(1);

  __rotCenter->paintGL();
  __clippingPlane->paintGL();

  glLineWidth(__linewidth);
  glPointSize(__linewidth);

  GL_ERROR;

  if (__scene)__scene->selectGL();

  __camera->endSelectGL();

  GL_ERROR;

  hits = glRenderMode(GL_RENDER);
  GLuint names, *ptr;
  ptr = (GLuint *) selectBuf;
  uint_t val;
  float z;

  vector<uint_t> vals;
  vector<uint_t> actualvals;
  if(hits > 0){
    for(uint_t i = 0 ; i < (uint_t)hits ; i++){
      names = *ptr; //    cerr << "Number of names for this hits = " << names << endl;
      ptr++;
      z = (float)*ptr;        //    cerr << "z1 = " << *ptr << endl;
      ptr++;        //    cerr << "z2 = " << *ptr << endl;
      ptr++;
      val = (uint_t)*ptr;
      vals.push_back(__scene->translateId(val));
      actualvals.push_back(val);
      for(unsigned int j = 0 ; j < names ; j++)ptr++;
    }

    if(!vals.empty()){
      if(vals.size()==1){
        __scene->selectionEvent(actualvals[0]);
        emit selectedShape(vals[0]);
      }
      else {
        __scene->selectionEvent(actualvals);
        emit selectedShapes(vals);
      }
    }
  }
#ifdef GEOM_DEBUG
  else warning("*** WARNING : hit miss all shapes");
#endif
}

ViewRayBuffer *
ViewGLFrame::castRays( const Vector3& position,
                          const Vector3& direction,
                          const Vector3& dx,
                          const Vector3& dy,
                          int sx, int sy)  {
    ViewRayBuffer * res = new ViewRayBuffer(sx,sy);

    GLint hits;
    GLsizei bufsize = 40960;
    GLuint selectBuf[40960];

    makeItCurrent();
    Vector3 oldpos = __camera->getPosition();
    Vector3 olddir = __camera->getDirection();
    bool mode = __camera->getProjectionMode();
    if(mode)__camera->setOrthographicMode();

    size_t nbray = sx*sy;
    for(size_t i = 0; i < sx; i++){
        for(size_t j = 0; j < sy; j++){
            Vector3 delta = dx*i+dy*j;
            __camera->lookIn(position+delta,direction);

            glSelectBuffer(bufsize,selectBuf);
            (void)glRenderMode(GL_SELECT);
            GL_ERROR;

            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            __camera->beginSelectGL(QPoint(width()/2,height()/2));
            __clippingPlane->paintGL();

            glLineWidth(__linewidth);
            glPointSize(__linewidth);
            GL_ERROR;

            if (__scene)__scene->selectGL();
            __camera->endSelectGL();

            GL_ERROR;
            hits = glRenderMode(GL_RENDER);

            if(hits > 0){
                res->setAt(i,j,selectBuf,hits,position+delta);
                GL_ERROR;
                RayHitList& l = res->getAt(i,j);
                for(RayHitList::iterator _it = l.begin();_it != l.end(); _it++)
                    _it->id = __scene->translateId(_it->id);
            }
            size_t numray = i*sy+j;
            if (numray % 20 == 0){
                std::cerr << "\x0d Ray " <<numray << " of " << nbray << std::flush;
                progress(numray,nbray);
            }
        }
    }
    std::cerr << "\x0d Ray " <<nbray << " of " << nbray << std::endl;
    progress(nbray,nbray);
    __camera->lookAt(oldpos,oldpos+olddir);
    if(mode)__camera->setProjectionMode(mode);
    return res;
}

ViewZBuffer *
ViewGLFrame::grabZBuffer( bool all_values  )
{
    bool pbufactivation = true;
    if(!isRedrawEnabled()){
        if (isPixelBufferUsed()){
            pbufactivation = __pBufferActivated;
            if(!pbufactivation){
                activatePBuffer(true);
                paintGL();
            }
            else  makeItCurrent();
        }
        else updateGL();
    }
    ViewZBuffer * res = ViewZBuffer::importglZBuffer(all_values);
    if(!pbufactivation) activatePBuffer(false);
    return res;
}

ViewZBuffer *
ViewGLFrame::grabDepthBuffer( bool all_values )
{
    makeItCurrent();
    return ViewZBuffer::importglDepthBuffer(all_values);
}

std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)>
ViewGLFrame::grabZBufferPoints( )
{
    bool pbufactivation = true;
    if(!isRedrawEnabled()){
        if (isPixelBufferUsed()){
            pbufactivation = __pBufferActivated;
            if(!pbufactivation){
                activatePBuffer(true);
                paintGL();
            }
            else  makeItCurrent();
        }
        else updateGL();
    }
    std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)> res = ViewZBuffer::importglZBufferPoints();
    if(!pbufactivation) activatePBuffer(false);
    return res;
}

std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)>
ViewGLFrame::grabZBufferPointsWithJitter(float jitter, int raywidth, bool mixcolor )
{
    bool pbufactivation = true;
    if(!isRedrawEnabled()){
        if (isPixelBufferUsed()){
            pbufactivation = __pBufferActivated;
            if(!pbufactivation){
                activatePBuffer(true);
                paintGL();
            }
            else  makeItCurrent();
        }
        else updateGL();
    }
    std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)> res = ViewZBuffer::importglZBufferPointsWithJitter(jitter,raywidth, true, mixcolor);
    if(!pbufactivation) activatePBuffer(false);
    return res;
}


double ViewGLFrame::getPixelWidth(){
    bool mode = __camera->getProjectionMode();
    if(mode)__camera->setOrthographicMode();

    makeItCurrent();
    GLdouble objx,objx2,objy,objy2,objz,objz2 ;
    if( geomUnProject(0.0,10.0, 0.0, &objx,&objy, &objz) != GL_TRUE  ||
        geomUnProject(1.0,10.0, 0.0, &objx2,&objy2, &objz2) != GL_TRUE  ) return -1;

    double pixelsize = sqrt(pow(objx-objx2,2)+pow(objy-objy2,2)+pow(objz-objz2,2));
    if(mode)__camera->setProjectionMode(mode);
    return pixelsize;
}

int ViewGLFrame::getProjectionPixel(){
    uint_t projpix = 0;
    int gridstate = __grid->getState();
    __grid->setState(0);
    bool mode = __camera->getProjectionMode();
    if(mode)__camera->setOrthographicMode();
    // else if(gridstate != 0)updateGL();
#ifdef WITH_OCCLUSION_QUERY
    if(__useOcclusionQuery && glInitOcclusionQuery()){
        makeItCurrent();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        GLuint qn;
        GLint sampleCount;
        glGenQueriesARB(1,&qn);
        glBeginQueryARB(GL_SAMPLES_PASSED_ARB, qn);
        paintGL();
        glEndQueryARB(GL_SAMPLES_PASSED_ARB);
        glGetQueryObjectivARB(qn, GL_QUERY_RESULT_ARB,&sampleCount);
        projpix= sampleCount;
        glDeleteQueriesARB(1,&qn);
    }
    else{
#endif
        makeItCurrent();
        paintGL();
        int w = width();
        int h = height();

        int nbpix = w*h;
        float  * zvalues = new float[nbpix];
        glReadPixels(0,0,w,h,GL_DEPTH_COMPONENT, GL_FLOAT, zvalues);

        float  * zvaluesiter = zvalues;
        for(uint_t i = 0; i < nbpix; ++i,++zvaluesiter)
            if(0 < *zvaluesiter && *zvaluesiter < 1) projpix++;

        delete [] zvalues;
#ifdef WITH_OCCLUSION_QUERY
    }
#endif
    __grid->setState(gridstate);
    if(mode)__camera->setProjectionMode(mode);
    // else if(gridstate != 0)updateGL();
    return projpix;
}

double ViewGLFrame::getProjectionSize(int* nbpixel, double* pixelwidth){
    bool mode = __camera->getProjectionMode();
    if(mode)__camera->setOrthographicMode();
    int nbpix = getProjectionPixel();
    double w = getPixelWidth();
    double res = nbpix * w * w;
    if(mode)__camera->setProjectionMode(mode);
    if(nbpixel)*nbpixel = nbpix;
    if(pixelwidth)*pixelwidth=w;
    return res;
}

void ViewGLFrame::printProjectionSize(){
    bool mode = __camera->getProjectionMode();
    if(mode)__camera->setOrthographicMode();
    float pixelwidth = getPixelWidth();
    int projpix = getProjectionPixel();
    if(mode)__camera->setProjectionMode(mode);
    float projsurf = pixelwidth * pixelwidth * projpix;
    status("Projection =  "+QString::number(projpix)+
           " pixels ; Pixel Width = "+QString::number(pixelwidth)+
           " (Screen = "+QString::number(pixelwidth*width())+" x "+QString::number(pixelwidth*height())+
           " ) ; Projection Size = "+QString::number(projsurf));
}

std::vector<std::pair<uint_t,uint_t> >
ViewGLFrame::getProjectionPixelPerColor(double* pixelwidth)
{
    uint_t projpix = 0;
    bool redrawEnabledPrevious = __redrawEnabled;
    if(redrawEnabledPrevious)activateRedraw(false);
    int gridstate = __grid->getState();
    __grid->setState(0);
    QColor curcolor = getBackGroundColor();
    setBackGroundColor(QColor(255,255,255,0));
    bool lightmode = __light->isEnabled();
    bool mode = __camera->getProjectionMode();
    if(lightmode)__light->setEnabled(false);
    if(mode)__camera->setOrthographicMode();
    makeItCurrent();
    paintGL();
    glFlush();

    if(pixelwidth) *pixelwidth = getPixelWidth();

    int w = width();
    int h = height();

    int nbpix = w*h;
    uchar_t  * cvalues = new uchar_t[4*nbpix];
    glReadPixels(0,0,w,h,GL_RGBA, GL_UNSIGNED_BYTE, cvalues);

    QHash<uint_t,uint_t> pcount;
    uchar_t  * cvaluesiter = cvalues;
    for(uint_t i = 0; i < nbpix; ++i){
        uchar_t red =   *cvaluesiter; ++cvaluesiter;
        uchar_t green = *cvaluesiter; ++cvaluesiter;
        uchar_t blue =  *cvaluesiter; ++cvaluesiter;
        uchar_t alpha = 255-(*cvaluesiter); ++cvaluesiter;
        uint_t id = (uint_t(alpha) << 24)+
                    (uint_t(red) << 16)+
                    (uint_t(green) << 8)+
                     uint_t(blue);
        if(id != UINT32_MAX){
            pcount[id] += 1;
        }
    }

    delete [] cvalues;
    __grid->setState(gridstate);
    setBackGroundColor(curcolor);
    if(lightmode)__light->setEnabled(lightmode);
    if(mode)__camera->setProjectionMode(mode);
    if(redrawEnabledPrevious)activateRedraw(redrawEnabledPrevious);
    std::vector<std::pair<uint_t,uint_t> > res;
    if(pixelwidth)
        for(QHash<uint_t,uint_t>::const_iterator it = pcount.begin();
            it != pcount.end(); ++it) {
                res.push_back(std::pair<uint_t,uint_t>(it.key(),it.value()));
        }
    return res;
}

/*  ------------------------------------------------------------------------ */

void ViewGLFrame::mousePressEvent( QMouseEvent* event)
{
  __mouse = event->pos();

  if(__mode == Selection){
    selectGL();
  }
  else if(__mode == MultipleSelection){
    status(QString(tr("Mouse on")+"(%1,%2)").arg(__mouse.x()).arg(__mouse.y()),2000);
    __selectionRect = new QRect(__mouse.x(),__mouse.y(),0,0);
  }
  else if(__mode == Rotation){
      if(event->modifiers() & Qt::AltModifier )
      {
        QBitmap bm = QBitmap::fromData(QSize(32,32),ViewerIcon::LIGHT_BITS,QImage::Format_MonoLSB );
        QBitmap mask = QBitmap::fromData(QSize(32,32),ViewerIcon::LIGHT_MASK,QImage::Format_MonoLSB);
        setCursor(QCursor(bm,mask));
      }
      else if (event->modifiers() & Qt::ShiftModifier)
      {
        setLastSelectionMode();
        if(__mode == Selection)selectGL();
      }
      else if (event->modifiers() & Qt::ControlModifier)
      {
          if (event->button()==Qt::LeftButton)
          {
              setCursor(Qt::SizeAllCursor);
          }
          else
          {
              QBitmap bm= QBitmap::fromData(QSize(32,32),ViewerIcon::ZOOM_BITS,QImage::Format_MonoLSB);
              QBitmap mask= QBitmap::fromData(QSize(32,32),ViewerIcon::ZOOM_MASK,QImage::Format_MonoLSB);
              setCursor(QCursor(bm,mask));
          }
      }
      else {
          if (event->button()==Qt::LeftButton)
          {
              QBitmap bm= QBitmap::fromData(QSize(32,32),ViewerIcon::ROTATE_BITS,QImage::Format_MonoLSB);
              QBitmap mask= QBitmap::fromData(QSize(32,32),ViewerIcon::ROTATE_MASK,QImage::Format_MonoLSB);
              setCursor(QCursor(bm,mask));
          }
          else if (event->button()==Qt::MidButton)
          {
              QBitmap bm= QBitmap::fromData(QSize(32,32),ViewerIcon::ZOOM_BITS,QImage::Format_MonoLSB);
              QBitmap mask= QBitmap::fromData(QSize(32,32),ViewerIcon::ZOOM_MASK,QImage::Format_MonoLSB);
              setCursor(QCursor(bm,mask));
          }
          else if(event->button()==Qt::RightButton)
          {
              setCursor(Qt::SizeAllCursor);
          }
      }
  }
  updateGL();
}

void ViewGLFrame::mouseReleaseEvent( QMouseEvent* event)
{
  if(__mode == Selection){
    if(!__scene->endSelect())setRotationMode();
  }
  else if(__mode == MultipleSelection){
    delete __selectionRect;
    __selectionRect = 0;
    QPoint mouse = event->pos();
    status(QString(tr("Selection from")+" (%1,%2) "+tr("to")+" (%3,%4)")
            .arg(__mouse.x()).arg(__mouse.y()).arg(mouse.x()).arg(mouse.y()),2000);
    multipleSelectGL(mouse);
    setRotationMode();
  }
  else setCursor(Qt::ArrowCursor);

  updateGL();
}

void ViewGLFrame::mouseMoveEvent( QMouseEvent* event)
{
  QPoint mouse = event->pos();
  if(__mode == MultipleSelection){
    status(QString(tr("Selection from")+" (%1,%2) "+tr("to")+" (%3,%4)")
            .arg(__mouse.x()).arg(__mouse.y()).arg(mouse.x()).arg(mouse.y()),2000);
    if(__selectionRect)
      *__selectionRect = QRect(min(__mouse.x(),mouse.x()),
                                min(__mouse.y(),mouse.y()),
                                abs(__mouse.x()-mouse.x()),
                                abs(__mouse.y()-mouse.y()));
    else
    __selectionRect = new QRect(min(__mouse.x(),mouse.x()),
                                min(__mouse.y(),mouse.y()),
                                abs(__mouse.x()-mouse.x()),
                                abs(__mouse.y()-mouse.y()));
    repaint();
  }
  else if(__mode == Selection || event->modifiers() & Qt::ShiftModifier)
    {
      if(__scene){
        if(event->buttons() & Qt::LeftButton)
          {
            __scene->rotate(__mouse-mouse);
          }
        else if(event->buttons() & Qt::RightButton)
          {
            __scene->move(__mouse-mouse);
          }
        else if(event->buttons() & Qt::MidButton)
          {
            __scene->zoom(__mouse-mouse);
          }
      }
      __mouse = mouse;
      updateGL();
    }
  else if(event->modifiers() & Qt::AltModifier)
    {
      if(event->buttons() & Qt::LeftButton)
        {
          __light->rotate(__mouse-mouse);
        }
      else if(event->buttons() & Qt::RightButton)
        {
          __light->move(__mouse-mouse);
        }
      else if(event->buttons() & Qt::MidButton)
    {
      __light->zoom(__mouse-mouse);
    }
    __mouse = mouse;
    updateGL();
    }
  else if(event->modifiers() & Qt::ControlModifier)
  {
      if(event->buttons() & Qt::LeftButton)
      {
        __camera->move(__mouse-mouse);
      }
      else if(event->buttons() & Qt::RightButton)
      {
        __camera->zoom(__mouse-mouse);
      }
      else if(event->buttons() & Qt::MidButton)
      {
        __camera->zoom(__mouse-mouse);
      }
      __mouse = mouse;
      updateGL();
  }
  else {
      if(event->buttons() & Qt::LeftButton)
      {
        __camera->rotate(__mouse-mouse);
      }
    else if(event->buttons() & Qt::RightButton)
      {
        __camera->move(__mouse-mouse);
    }
    else if(event->buttons() & Qt::MidButton)
      {
        __camera->zoom(__mouse-mouse);
      }
    __mouse = mouse;
    updateGL();
  }
}

void ViewGLFrame::dragEnterEvent(QDragEnterEvent* myevent){
    if (myevent->mimeData()->hasUrls()){
        myevent->accept();
    }
    else myevent->ignore();
}

void ViewGLFrame::dropEvent(QDropEvent* myevent){
    if(__scene){
        QList<QUrl> urls = myevent->mimeData()->urls();
        QList<QUrl>::const_iterator itUrls = urls.constBegin();
        if(myevent->dropAction() == Qt::CopyAction)
            __scene->openFile(itUrls->toLocalFile());
        else
            __scene->addFile(itUrls->toLocalFile());
        for(++itUrls;itUrls != urls.constEnd();++itUrls ){
            __scene->addFile(itUrls->toLocalFile());
        }
    }
}

#ifndef WHEEL_DELTA
#ifdef Q_OS_MAC
#define WHEEL_DELTA 30.
#else
#define WHEEL_DELTA 120.
#endif
#endif

void
ViewGLFrame::wheelEvent ( QWheelEvent * e){
  if(__mode == Selection || e->modifiers() & Qt::ShiftModifier){
      __scene->zooming(0,e->delta()/WHEEL_DELTA);
      // __scene->rotating(0,e->delta()*20/WHEEL_DELTA);
      e->accept();
  }
  else if(e->modifiers() & Qt::AltModifier){
      __light->zooming(0,e->delta()/WHEEL_DELTA);
      // __light->rotating(0,e->delta()*20/WHEEL_DELTA);
      e->accept();
  }
  else {
      __camera->zooming(0,e->delta()/WHEEL_DELTA);
      // __camera->rotating(0,e->delta()*20/WHEEL_DELTA);
      e->accept();
  }
  updateGL();
}

void
ViewGLFrame::keyPressEvent ( QKeyEvent * e)
{
  if(e->modifiers() & Qt::ShiftModifier){
    if( e->key() == Qt::Key_Up){
      __scene->rotating(0,2);
      e->accept();
      updateGL();
    }
    else if( e->key() == Qt::Key_Down){
      __scene->rotating(0,-2);
      e->accept();
      updateGL();
    }
    else if( e->key() == Qt::Key_Left){
      __scene->rotating(2,0);
      e->accept();
      updateGL();
    }
    else if( e->key() == Qt::Key_Right){
      __scene->rotating(-2,0);
      e->accept();
      updateGL();
    }
    else {
      setSelectionMode();
      e->accept();
    }
  }
  else if(e->modifiers() & Qt::ControlModifier){
    if( e->key() == Qt::Key_Up){
      __camera->move(QPoint(0,4));
      // __light->rotating(0,4);
    }
    else if( e->key() == Qt::Key_Down){
      __camera->move(QPoint(0,-4));
      // __light->rotating(0,-4);
    }
    else if( e->key() == Qt::Key_Left){
      __camera->move(QPoint(4,0));
      // __light->rotating(4,0);
    }
    else if( e->key() == Qt::Key_Right){
      __camera->move(QPoint(-4,0));
      // __light->rotating(-4,0);
    }
    else  {
     /*QBitmap bm = QBitmap::fromData(QSize(32,32),ViewerIcon::LIGHT_BITS,QImage::Format_MonoLSB);
     QBitmap mask = QBitmap::fromData(QSize(32,32),ViewerIcon::LIGHT_MASK,QImage::Format_MonoLSB);
     setCursor(QCursor(bm,mask));*/
    }
    updateGL();
    e->accept();
  }
  else {
    if( e->key() == Qt::Key_Up){
      __camera->zooming(0,1);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_Down){
      __camera->zooming(0,-1);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_Left){
      __camera->rotating(4,0);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_Right){
      __camera->rotating(-4,0);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_PageUp){
      __camera->rotating(0,-4);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_PageDown){
      __camera->rotating(0,4);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_Home)
      {
      e->ignore();
    }
    else if( e->key() == Qt::Key_End)
      {
      e->ignore();
    }
    else if( e->key() == Qt::Key_N)
      {
      __camera->nextView();
      e->accept();
    }
    else if( e->key() == Qt::Key_P)
      {
      printProjectionSize();
      e->accept();
    }
    else e->ignore();
  }
}

void
ViewGLFrame::keyReleaseEvent ( QKeyEvent * e)
{
  if(e->modifiers() & Qt::ShiftModifier){
    setRotationMode();
    e->accept();
  }
  else if(e->modifiers() & Qt::AltModifier){
    setCursor(Qt::ArrowCursor);
    e->accept();
  }
}

// #define DEBUG_EVENTDISPATCH

bool ViewGLFrame::event(QEvent *e){
#ifdef DEBUG_EVENTDISPATCH
    if(e->type() >= ViewEvent::eFirstEvent && e->type() <= ViewEvent::eLastEvent)
        printf("** gl ** receive pgl event\n");
    // else printf("** gl ** receive event\n");
#endif
    return QGLWidget::event(e);
}

void ViewGLFrame::customEvent(QEvent *e)
{
    int etype = e->type();
    if(etype == ViewEvent::eSceneChange){
    ViewSceneChangeEvent * event = (ViewSceneChangeEvent *)e;
    __scene->sceneChangeEvent(event);
  }
}

void ViewGLFrame::focusInEvent ( QFocusEvent * e) {
//  if(e->reason()== QFocusEvent::ActiveWindow)
  __scene->checkFileModification();
}

void ViewGLFrame::gridEvent ( ViewEvent * e) {
  __grid->gridEvent(e);
}

void ViewGLFrame::cameraEvent ( ViewEvent * e) {
  __camera->cameraEvent(e);
}

void ViewGLFrame::clippingPlaneEvent(ViewEvent * e)
{
    __clippingPlane->clippingPlaneEvent(e);
}

/*  ------------------------------------------------------------------------ */


void ViewGLFrame::saveImage(QString _filename,const char * _format, bool withAlpha){
    bool done = false;
    if(isPixelBufferUsed()){
        bool pbufactivation = __pBufferActivated;
        if(!pbufactivation) activatePBuffer(true);
        if(__pixelbuffer){
            paintGL();
            __pixelbuffer->toImage().save(_filename,_format);
            if(!pbufactivation) activatePBuffer(false);
            done = true;
        }
    }
    if(!done) {
        updateGL();
        grabFrameBuffer(withAlpha).save(_filename,_format);
    }
    status(tr("Save screenshot with format")+" \""+QString(_format)+"\" "+tr("in")+" \""+_filename+'"',10000);
}

void ViewGLFrame::copyImageToClipboard(){
    if(QApplication::clipboard()){
        QImage img = grabFrameBuffer(false);
        QApplication::clipboard()->setImage(img);
        status(tr("Copy screenshot to clipboard"),10000);
    }
    else QMessageBox::warning(this,tr("System Error"),tr("Cannot access global clipboard"),"Ok");
}

void ViewGLFrame::copyImageToClipboardWithAlpha(){
    if(QApplication::clipboard()){
        QImage img = grabFrameBuffer(true);
        QApplication::clipboard()->setImage(img);
        status(tr("Copy screenshot to clipboard with alpha channel"),10000);
    }
    else QMessageBox::warning(this,tr("System Error"),tr("Cannot access global clipboard"),"Ok");
}

void ViewGLFrame::printImage(){
  QPrinter printer;
  QPrintDialog dialog(&printer, this);
  if (dialog.exec()){
    QPainter paint;
    if(!paint.begin(&printer))return;
    QPoint origin(0,0);
    QRect r = printer.pageRect();
    QImage img = grabFrameBuffer(false);
    QSize r2 = img.size();
    double x = min((double)r.width()/(double)r2.width(),
                   (double)r.height()/(double)r2.height());
    x *= 0.8;

    paint.translate(((double)r.width() - r2.width()*x)/2.0,
                    ((double)r.height() - r2.height()*x)/2.0);
    paint.scale(x,x);
    paint.drawImage(QPoint(0,0),img);
    paint.setPen( QPen(Qt::black,1) );
    paint.drawRect(QRect(QPoint(0,0),r2));
    paint.end();
  }
}


QMenu *
ViewGLFrame::createEditMenu(QWidget * parent)
{
  QMenu * menu = new QMenu(parent);
  __scene->addEditEntries(menu);
  QPixmap wizard( ViewerIcon::getPixmap(ViewerIcon::wizard) );
  menu->addAction(wizard,tr("Selection"),this, SLOT(setSelectionMode()));
  QPixmap wizardRectIcon( ViewerIcon::getPixmap(ViewerIcon::wizardrect) );
  menu->addAction(wizardRectIcon,tr("Rectangle Selection"),this, SLOT(setMultipleSelectionMode()));

  QPixmap notwizard( ViewerIcon::getPixmap(ViewerIcon::notwizard) );
  menu->addAction(notwizard,tr("Clear Selection"),this, SLOT(clearSelection()));
  menu->addSeparator();

  if(__linedialog){
    QPixmap lineIcon( ViewerIcon::getPixmap(ViewerIcon::line_width) );
    QAction * action= menu->addAction(lineIcon,tr("Edit Line Width"),__linedialog, SLOT(changeVisibility()));
    QObject::connect(__linedialog,SIGNAL(__visibilityChanged(bool)),action,SLOT(setChecked(bool)));
  }
  return menu;
}

QMenu *
ViewGLFrame::createToolsMenu(QWidget * parent)
{

  QMenu * menu = new QMenu(parent);
  QPixmap wheel(ViewerIcon::getPixmap(ViewerIcon::wheel));
  QMenu * __RendererMenu = __scene->createToolsMenu(menu);
  menu->addMenu(__RendererMenu);
  __RendererMenu->setTitle(tr("Renderer"));
  __RendererMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __CameraMenu = __camera->createToolsMenu(menu);
  menu->addMenu(__CameraMenu);
  __CameraMenu->setTitle(tr("Camera"));
  __CameraMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __LightMenu = __light->createToolsMenu(menu);
  menu->addMenu(__LightMenu);
  __LightMenu->setTitle(tr("Light"));
  __LightMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __FogMenu = __fog->createToolsMenu(menu);
  menu->addMenu(__FogMenu);
  __FogMenu->setTitle(tr("Fog"));
  __FogMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __GridMenu = __grid->createToolsMenu(menu);
  menu->addMenu(__GridMenu);
  __GridMenu->setTitle(tr("Grid"));
  __GridMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __CPMenu = __clippingPlane->createToolsMenu(menu);
  menu->addMenu(__CPMenu);
  __CPMenu->setTitle(tr("Clipping Plane"));
  __CPMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __RotCMenu = __rotCenter->createToolsMenu(menu);
  menu->addMenu(__RotCMenu);
  __RotCMenu->setTitle(tr("Rotating Center"));
  __RotCMenu->setIcon(QIcon(wheel));
  menu->addSeparator();

  QPixmap coloricon(ViewerIcon::getPixmap(ViewerIcon::color));
  menu->addAction(coloricon,tr("Background Color"),  this,SLOT(setBackground()));
  return menu;
}


void
ViewGLFrame::fillToolBar(QToolBar * toolBar)
{
  QPixmap wizardIcon( ViewerIcon::getPixmap(ViewerIcon::wizard) );
  QPixmap wizardRectIcon( ViewerIcon::getPixmap(ViewerIcon::wizardrect) );
  QPixmap wizardIconMenu( ViewerIcon::getPixmap(ViewerIcon::wizardmenu) );

  ViewDoubleToolButton * bt2 = new ViewDoubleToolButton( wizardIcon,  wizardRectIcon,tr("Selection"), this, SLOT(setLastSelectionMode()), toolBar);
  QString wizardtext =tr("<b>Selection</b><br><br>Allows you to select shapes from the scene<br><br>"
    "Press deeply this button to have the choice between two modes of selection<br><br>"
    "<b>Point selection</b> allows you to select one particular shapes<br>"
    "<b>Rectangle selection</b> allows you to select all the shapes displayed inside a rectangle<br><br>"
    "Selecting two times the same shape unselect it.<br>");
  bt2->setWhatsThis(wizardtext);
  QMenu * selctmenu = new QMenu(toolBar);
  selctmenu->addAction(wizardIcon,tr("Point"),this,SLOT(setSelectionMode()));
  selctmenu->addAction(wizardRectIcon,tr("Rectangle"),this,SLOT(setMultipleSelectionMode()));
  bt2->setMenu(selctmenu);

  QObject::connect(this,SIGNAL(selectionMode(bool)),bt2,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(modeChanged(ViewGLFrame::Mode)),bt2,SLOT(setButton(ViewGLFrame::Mode)));

  QPixmap notwizardIcon( ViewerIcon::getPixmap(ViewerIcon::notwizard) );
  QString notwizardtext =tr("Clear Selection");


  __camera->fillToolBar(toolBar);
  toolBar->addSeparator();
  __scene->fillToolBar(toolBar);
  __light->fillToolBar(toolBar);
  QPixmap coloricon(ViewerIcon::getPixmap(ViewerIcon::color));
  QAction * bt = toolBar->addAction(coloricon,tr("Background Color"), this,SLOT(setBackground()));
  bt->setWhatsThis(tr("<b>Background Color</b><br><br>Change the background color of the 3D display."));
  if(__linedialog){
    QPixmap lineIcon( ViewerIcon::getPixmap(ViewerIcon::line_width) );
    QString linetext =tr("Line Width");
    bt = toolBar->addAction( lineIcon, linetext, __linedialog, SLOT(changeVisibility()));
    bt->setCheckable(true);
    bt->setChecked(__linedialog->isVisible());
    QObject::connect(__linedialog,SIGNAL(__visibilityChanged(bool)),bt,SLOT(setChecked(bool)));
    bt->setWhatsThis(tr("<b>Line Width</b><br><br>Control of the width of the lines and the points of the scene. By default, this value is egal to one."));
  }
  toolBar->addSeparator();
  __grid->fillToolBar(toolBar);

  toolBar->addWidget(bt2);
  bt = toolBar->addAction( notwizardIcon, notwizardtext, this, SLOT(clearSelection()));
  bt->setWhatsThis(tr("<b>Clear Selection</b><br><br>Clear the current selection."));
  toolBar->addSeparator();
  __rotCenter->fillToolBar(toolBar);
  __camera->fillToolBarAdditionnal(toolBar);
  toolBar->addSeparator();
  __scene->fillToolBarAdditionnal(toolBar);
  

}

void
ViewGLFrame::addOtherToolBar(QMainWindow * menu)
{
  __linedialog  = new ViewToolBar(tr("Line Width Bar"),menu,"LineWidthBar");
  Ui::LineWidthControl form;
  QWidget * widget = new QWidget(__linedialog);
  form.setupUi(widget);
  __linedialog->addWidget(widget);
  QObject::connect (form.LineWidthSlider,SIGNAL(valueChanged(int)), this,SLOT(setLineWidth(int)) );

  menu->addToolBar(__linedialog);
  __linedialog->hide();
}


void
ViewGLFrame::addProperties(QTabWidget * tab)
{
    if(__scene)__scene->addProperties(tab);
    if(__camera)__camera->addProperties(tab);

    QWidget * mtab = new QWidget( tab  );
    Ui::ViewGLOptionsForm  glform;
    glform.setupUi(mtab);
    tab->addTab( mtab, tr( "GL Options" ) );

    makeCurrent();
    GLboolean glb = glIsEnabled(GL_CULL_FACE);

    GLint * res = new GLint;
    if(glb == GL_FALSE)glform.NoCullingButton->setChecked(true);
    else {
      glGetIntegerv(GL_CULL_FACE_MODE ,res);
      if(*res == GL_BACK)glform.BackFaceButton->setChecked(true);
      else if(*res == GL_FRONT)glform.FrontFaceButton->setChecked(true);
      else {
        qWarning("Error with glGet(GL_CULL_FACE_MODE) : %i", *res);
      }
    }

    glGetIntegerv(GL_SHADE_MODEL,res);
    if(*res == GL_FLAT)glform.FlatButton->setChecked(true);
    else if(*res == GL_SMOOTH)glform.SmoothButton->setChecked(true);

    QObject::connect(glform.FlatButton,SIGNAL(toggled(bool)),this,SLOT(glFlatShadeModel(bool)));
    QObject::connect(glform.SmoothButton,SIGNAL(toggled(bool)),this,SLOT(glSmoothShadeModel(bool)));

    glb = glIsEnabled(GL_DITHER);
    if(glb == GL_TRUE)glform.DitheringButton->setChecked(true);
    QObject::connect(glform.DitheringButton,SIGNAL(toggled(bool)),this,SLOT(glDithering(bool)));

    glb = glIsEnabled(GL_DEPTH_TEST);
    if(glb == GL_TRUE)glform.DepthTestButton->setChecked(true);
    QObject::connect(glform.DepthTestButton,SIGNAL(toggled(bool)),this,SLOT(glDepthTest(bool)));

  glb = glIsEnabled(GL_NORMALIZE);
  if(glb == GL_TRUE)glform.NormalizationButton->setChecked(true);
  QObject::connect(glform.NormalizationButton,SIGNAL(toggled(bool)),this,SLOT(glNormalization(bool)));

  glGetBooleanv(GL_LIGHT_MODEL_TWO_SIDE,&glb);
  if(glb == GL_TRUE)glform.TwoSideLightButton->setChecked(true);
  QObject::connect(glform.TwoSideLightButton,SIGNAL(toggled(bool)),this,SLOT(glTwoSideShadeModel(bool)));

    glform.FPSButton->setChecked(__fpsDisplay);
    QObject::connect(glform.FPSButton,SIGNAL(toggled(bool)),this,SLOT(setFPSDisplay(bool)));

    glform.OcclusionQueryButton->setChecked(__useOcclusionQuery);
    glform.PixelBufferButton->setChecked(__usePBuffer);

    QObject::connect(glform.NoCullingButton,SIGNAL(toggled(bool)),this,SLOT(glCullNoFace(bool)));
    QObject::connect(glform.BackFaceButton,SIGNAL(toggled(bool)),this,SLOT(glCullBackFace(bool)));
    QObject::connect(glform.FrontFaceButton,SIGNAL(toggled(bool)),this,SLOT(glCullFrontFace(bool)));
    QObject::connect(glform.OcclusionQueryButton,SIGNAL(toggled(bool)),this,SLOT(useOcclusionQuery(bool)));
    QObject::connect(glform.PixelBufferButton,SIGNAL(toggled(bool)),this,SLOT(usePixelBuffer(bool)));

    delete res;
}

/* ----------------------------------------------------------------------- */
#define GL_COM(a)qWarning(#a);a;

void
ViewGLFrame::glCullNoFace(bool b){
  if(b){ makeItCurrent(); GL_COM(glDisable(GL_CULL_FACE)); redrawGL();}
}

void
ViewGLFrame::glCullBackFace(bool b){
  if(b){ makeItCurrent(); glEnable(GL_CULL_FACE); GL_COM(glCullFace(GL_BACK));redrawGL();}
}

void
ViewGLFrame::glCullFrontFace(bool b){
  if(b){ makeItCurrent(); glEnable(GL_CULL_FACE); GL_COM(glCullFace(GL_FRONT));redrawGL();}
}

void
ViewGLFrame::glSmoothShadeModel(bool b){
  if(b){ makeItCurrent(); GL_COM(glShadeModel(GL_SMOOTH));redrawGL();}
}

void
ViewGLFrame::glFlatShadeModel(bool b){
  if(b){ makeItCurrent(); GL_COM(glShadeModel(GL_FLAT));redrawGL();}
}

void
ViewGLFrame::glTwoSideShadeModel(bool b){
  makeItCurrent(); GL_COM(glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, (b?GL_TRUE:GL_FALSE)));redrawGL();
}

void
ViewGLFrame::glDithering(bool b){
  makeItCurrent();
  if(b){GL_COM(glEnable(GL_DITHER));}
  else {GL_COM(glDisable(GL_DITHER));}
  redrawGL();
}

void
ViewGLFrame::glDepthTest(bool b){
  makeItCurrent();
  if(b){GL_COM(glEnable(GL_DEPTH_TEST));}
  else {GL_COM(glDisable(GL_DEPTH_TEST));}
  redrawGL();
}

void
ViewGLFrame::glNormalization(bool b){
  makeItCurrent();
  if(b){GL_COM(glEnable(GL_NORMALIZE));}
  else {GL_COM(glDisable(GL_NORMALIZE));}
  redrawGL();
}

void
ViewGLFrame::setAnimation(eAnimationFlag b){
  ViewSceneRendererGL * r = dynamic_cast<ViewSceneRendererGL *>(__scene);
  if(r)r->setAnimation(b);
  __camera->lockDim(b != eStatic);
}

void ViewGLFrame::useOcclusionQuery(bool b)
{
    __useOcclusionQuery = b;
}

void ViewGLFrame::usePixelBuffer(bool b) { __usePBuffer = b; }

void ViewGLFrame::setFPSDisplay(bool b) { __fpsDisplay = b; }

void ViewGLFrame::showMessage(const QString message, int timeout)
{
    __timer.stop();
    __msg_transparency = 1.0;
    __message = message;
    updateGL();
    if(timeout > 0){
        __msg_transparency_step = min(1.0,200. / timeout);
        __timer.start();
    }
}

void ViewGLFrame::updateMessage()
{
    __msg_transparency -= __msg_transparency_step;
    if (__msg_transparency > 0){
        updateGL();
    }
    else cleanMessage();
}

void ViewGLFrame::cleanMessage()
{
    __timer.stop();
    __message.clear();
    updateGL();
}


ViewDoubleToolButton::ViewDoubleToolButton( const QPixmap & pm,
                                         const QPixmap & pm2,
                                         const QString & textLabel,
                                         QObject * receiver,
                                         const char * slot,
                                         QToolBar * parent ):
QToolButton(parent),
  __pm1(pm),__pm2(pm2){
  connect(this,SIGNAL(triggered(QAction *)),receiver,slot);
  PGL(drawArrow)(&__pm1);
  QBitmap mask = __pm1.mask();
  PGL(drawArrow)(&mask);
  __pm1.setMask(mask);
  PGL(drawArrow)(&__pm2);
  mask = __pm2.mask();
  PGL(drawArrow)(&mask);
  __pm1.setMask(mask);
  setIcon(QIcon(__pm1));
  setText(textLabel);
  setPopupMode(QToolButton::DelayedPopup);
  setCheckable(true);
}

void ViewDoubleToolButton::setButton(ViewGLFrame::Mode m)
{
  if(m == ViewGLFrame::Selection){
    setIcon(QIcon(__pm1));
  }
  else if(m == ViewGLFrame::MultipleSelection){
    setIcon(QIcon(__pm2));
  }
}


