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



/*! \file view_viewer.h
    \brief Definition of the main viewer class : Viewer.
*/


#ifndef __view_viewer_h__
#define __view_viewer_h__

/* ----------------------------------------------------------------------- */

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/QMainWindow>
#else
    #include <QtGui/QMainWindow>
#endif
#include <plantgl/tool/util_types.h>
#include <vector>

#ifdef QT_THREAD_SUPPORT
#include <QtCore/qwaitcondition.h>

#if QT_VERSION >= QT_VERSION_CHECK(3,0,0)
#include <QtCore/qmutex.h>
#else
#include <QtCore/qthread.h>
#endif

#endif
#include "flags.h"

/* ----------------------------------------------------------------------- */

class QMenu;
class QMenuBar;

/* ----------------------------------------------------------------------- */

class ViewErrorDialog;
class ViewFileManager;
class ViewHelpMenu;
class ViewBrowser;
class ViewGLFrame;
class ViewControlPanel;
class ViewRendererGL;
class ViewerDaemon;
class ViewStatusBar;
class QSystemTrayIcon;

#include "util_qwidget.h"

/* ----------------------------------------------------------------------- */

/**
   \class Viewer
   \brief The Main Window of the viewer
*/

/* ----------------------------------------------------------------------- */

class VIEW_API Viewer : public QMainWindow
{
  Q_OBJECT
public:


  /// Constructor.
  Viewer(  QWidget * parent = 0,
           const char * name = 0,
           ViewRendererGL * r = 0 ,
           Qt::WindowFlags f = 0 );

  /// Constructor.
  Viewer( int argc, char ** argv, ViewRendererGL * r = 0 );

  /// Destructor.
  ~Viewer();

  /// Get the SceneRenderer.
  ViewRendererGL * getSceneRenderer() const;
  void setSceneRenderer(ViewRendererGL * s);

  // Get the GL Frame
  ViewGLFrame * getFrameGL() const { return __GLFrame; }

  // Save the view as a bitmap
  void saveImage( QString _filename, const char* _format, bool withAlpha = false );

  /// Get the selection.
  std::vector<uint_t> getSelection() const;

  void setFrameGLSize(int width, int height);

  void post(QEvent * e) ;
  void send(QEvent * e) ;

  void question(const QString& caption, const QString& text,
                const QString& but0txt = QString(),
                const QString& but1txt = QString(),
                const QString& but2txt = QString(),
                int * result = NULL);

  void itemSelection(const QString& caption, const QString& text,
                     const QStringList& values, bool editable,
                     QString* result, bool * ok);

  void fileSelection(const QString& caption,
                     const QString& filter,
                     const QString& startPath,
                     bool existing,
                     QString* result);

  void dirSelection(const QString& caption,
                    const QString& startPath,
                    QString* result);

  void  doubleSelection(const QString& caption, const QString& text, double value,
                        double minvalue, double maxvalue, double* result, bool * ok);

  void setAnimation(eAnimationFlag);

  inline bool hasFocusAtRefresh() const { return __focusAtRefresh; }


  typedef bool (* DialogAborter)();
  inline void setAborter(DialogAborter aborter) { __aborter = aborter; }
      
  inline void setStandAlone(bool v) { __standalone = v;  }
  inline bool isStandAlone() const { return __standalone; }

signals:
  void closing();

public slots:

  /// (Un)Display Menu Bar.
  void displayMenuBar();

  /// On Windows, display a Debug Console.
  void debugLog();

  /// Set Viewer as the Full Screen Application.
  void displayFullScreen();

  /// Display only the GL Widget.
  void displayGLWidgetOnly();

   /// set \e _msg to status bar.
  void setStatusBarMsg(QString _msg);

  virtual void polish ();

  void displayHelp() const;
  void displayAbout() const;
  void saveConfig() const;

  void openFile(const QString& filename);

  void addFile(const QString& filename);

  void receiveRequest(const QString&);
  void appear();

  void startDaemon();
  void stopDaemon();

  void set800x600FrameGL();
  void set640x480FrameGL();
  void set512x384FrameGL();
  void set320x240FrameGL();
  void setCustomFrameGLSize();

  void displayTrayIcon(bool);

  void setFocusAtRefresh(bool value) { __focusAtRefresh = value; }

protected:
  /// This event handler is called when a key is pressed for this widget.
  virtual bool event ( QEvent * e);

  /// This event handler is called when a key is pressed for this widget.
  virtual void keyPressEvent ( QKeyEvent * e);

  /// This event handler is called when a drag is in progress and the mouse enters this widget.
  virtual void dragEnterEvent(QDragEnterEvent* event);

  /// This event handler is called when the drag is dropped on this widget.
  virtual void dropEvent(QDropEvent* event);

  /// This event handler is called when a new scene is asked to be shown.
  virtual void customEvent(QEvent *e);

  /// This event handler is called when \e this must be close.
  virtual void closeEvent ( QCloseEvent * );

  /// This event handler is called when \e this must be shown.
  virtual void showEvent ( QShowEvent * );

protected slots:

  /// Initializer.
  virtual void initializeRenderer();

  void whatsThis();

private slots:

  /// For Exit.
  void bye();

private:

  /// Initializer.
  void initialize();

  inline bool shouldAbortDialog() const { return (__aborter && __aborter()); }

  /// The Open GL Frame : To display 3D Object.
  ViewGLFrame * __GLFrame;

  /// The File Manager : For Menu, Toolbar.
  ViewFileManager * __FileMenu;

  /// Menu View.
  QMenu *      __EditMenu;

  /// Menu View.
  QMenu *      __ViewMenu;

  /// Menu Tools
  QMenu *      __ToolsMenu;

  /// Menu Help
  ViewHelpMenu *    __HelpMenu;

  /// Menu Bar.
  QMenuBar *        __MainMenu;

  /// Tool Bar
  QToolBar * __FileToolBar;

  /// Tool Bar
  ViewToolBar * __ViewToolBar;

  /// Control Panel.
  ViewControlPanel * __controlPanel;

  /// Tray Icon
  QSystemTrayIcon * __trayIcon;

  /// The Browser Dialog.
  ViewBrowser * __Browser;

  /// The Error Dialog.
  ViewErrorDialog * __ErrorDialog;

  /// The Status Bar.
  ViewStatusBar * __statusBar;

  /// Is this widget full_screen or not.
  bool __isFullScreen;

  /// id of the Entry Menu : View ->Full Screen.
  QAction * __actFullScreen;

  int __toolbarsvisibility;

  ViewerDaemon * __service;

  bool __focusAtRefresh;

  bool __standalone;

  DialogAborter __aborter;

#ifdef QT_THREAD_SUPPORT
  QMutex send_event_mutex;
  QMutex send_lock_mutex;
  QWaitCondition send_event_condition;
#endif
};


class SelectionListener : public QObject
{
  Q_OBJECT
public:
    SelectionListener() : done(false) {}

    uint_t selection;
    bool done;

public slots:
    void selectionMade(uint_t s) { selection = s; done = true; }
    void finalize() { done = true; }
};

/* ----------------------------------------------------------------------- */

// __view_viewer_h__
#endif
