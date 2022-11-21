
#define PGL_USE_QOPENGLWIDGET


#ifndef PGL_WITHOUT_QT

#ifndef PGL_QT_FWD_DECL

// includes
#ifdef PGL_USE_QOPENGLWIDGET
#include <QOpenGLWidget>
#else
#include <QGLWidget>
#endif

#else
// Forward declaration
#ifdef PGL_USE_QOPENGLWIDGET
class QOpenGLWidget; 
#else
class QGLWidget;
#endif

#endif

// class definition
#ifdef PGL_USE_QOPENGLWIDGET
#define QOpenGLBaseWidget QOpenGLWidget
#define QOpenGLBaseContext QOpenGLContext
#define QOpenGLBaseFormat QSurfaceFormat
#else
#define QOpenGLBaseWidget QGLWidget
#define grabFramebuffer grabFrameBuffer
#define QOpenGLBaseContext QGLContext
#define QOpenGLBaseFormat QGLFormat
#endif




#endif
