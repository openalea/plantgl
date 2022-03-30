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

#ifdef __GNUC__
#include <getopt.h>
#else
#include <QtCore/qstring.h>
#endif

#include <QtGlobal>

#include <QtCore/QPointer>
#include <QtCore/qfileinfo.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qurl.h>

#include <QtGui/QDragEnterEvent>
#include <QtGui/qclipboard.h>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtCore/QMimeData>
    #include <QtWidgets/qapplication.h>
    #include <QtWidgets/qstatusbar.h>
    #include <QtWidgets/qprogressbar.h>
    #include <QtWidgets/qspinbox.h>
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qmenubar.h>
    #include <QtWidgets/qtoolbar.h>
    #include <QtWidgets/qfiledialog.h>
    #include <QtWidgets/qmessagebox.h>
    #include <QtWidgets/qlabel.h>
    #include <QtWidgets/qwhatsthis.h>
    #include <QtWidgets/qinputdialog.h>
    #include <QtWidgets/qsystemtrayicon.h>
    #include <QtWidgets/qdesktopwidget.h>
#else
    #include <QtGui/qapplication.h>
    #include <QtGui/qstatusbar.h>
    #include <QtGui/qprogressbar.h>
    #include <QtGui/qspinbox.h>
    #include <QtGui/qmenu.h>
    #include <QtGui/qmenubar.h>
    #include <QtGui/qtoolbar.h>
    #include <QtGui/qfiledialog.h>
    #include <QtGui/qmessagebox.h>
    #include <QtGui/qlabel.h>
    #include <QtGui/qwhatsthis.h>
    #include <QtGui/qinputdialog.h>
    #include <QtGui/qsystemtrayicon.h>
    #include <QtGui/qdesktopwidget.h>
#endif

#include "viewer.h"
#include "event.h"
#include "scenegl.h"
#include "light.h"
#include "camera.h"
#include "glframe.h"
#include "controlpanel.h"
#include "helpmenu.h"
#include "icons.h"
#include "errordialog.h"
#include "filemanager.h"
#include "browser.h"
#include "event.h"
#include "translator.h"
#include "daemon.h"
#include "configuration.h"
#include "util_qwidget.h"

#include "interface/ui_frameglsizedialog.h"

#include <plantgl/tool/util_enviro.h>

#include <iostream>


#ifdef _WIN32
#include "guicon.h"
#endif

using namespace std;
/* ----------------------------------------------------------------------- */

Viewer::Viewer( QWidget * parent, const char * name, ViewRendererGL * r , Qt::WindowFlags f )
    : QMainWindow( parent ,  f ),
      __GLFrame(0),
      __EditMenu(0),
      __ToolsMenu(0),
      __service(0),
      __toolbarsvisibility(0),
      __trayIcon(0),
      __aborter(0),
      __standalone(false)
{
    setObjectName(name);
  if(TOOLS(getLanguage()) == "French")
    setFrenchTranslator();

  // Preparation de la boite de dialog ErrorDialog
  __ErrorDialog= new ViewErrorDialog(this);
  addDockWidget(Qt::BottomDockWidgetArea,__ErrorDialog);

  __GLFrame = new ViewGLFrame(this,"PGLFrameGL",r);
  initialize();
}

Viewer::Viewer( int argc, char ** argv, ViewRendererGL * r)
  : QMainWindow( 0 ),
      __GLFrame(0),
      __EditMenu(0),
      __ToolsMenu(0),
      __service(0),
      __toolbarsvisibility(0),
      __trayIcon(0),
      __aborter(0),
      __standalone(true)
{
  // if(QMessageBox::information(this,"Language","Select Language","English","French")==1)
  if(TOOLS(getLanguage()) == "French")
    setFrenchTranslator();

  // Preparation de la boite de dialog ErrorDialog
  __ErrorDialog= new ViewErrorDialog(this);
  addDockWidget(Qt::BottomDockWidgetArea,__ErrorDialog);

  if(argv!=NULL){
    // QFileInfo f(argv[0]);
    // ViewFileManager::FILE_CONFIG = f.baseName();
  }

    __GLFrame = new ViewGLFrame(this,"PGLFrameGL",r);
    initialize();
    bool stdinput = false;

    if( argc != 0 ){
#ifdef __GNUC__
                /// Read of calling options
        const char * optstring = "hs";

        struct option longopts [] = {
            /* name      has_arg   flag        val */
            { "help",    0,        NULL,       'h' },
            { "add",     0,        NULL,       'a' },
            { "stdin",   0,        NULL,       's' },
            /* The last arg must be null */
            { NULL,      0,        NULL,        0 },
        };

        int longindex;
        int option;
        bool addition = true;



        while ((option = getopt_long (argc, argv, optstring, longopts, & longindex)) != -1 ){
            switch (option) {
                case 's' :
                    stdinput = true;
                    break;
                case '?':
                case 'h' :
                    cout << "Geom 3D Viewer" << endl;
                    cout << "Compiled the " << __DATE__ << "." << endl ;
                    cout << "Display 3D Geometric Shape." << endl << endl;
                    cout << "Usage:" << argv[0] <<" [ -h | --help | -s | --stdin ] [ filename ]" << endl;
                    cout << "\t -h --help     : display this help." << endl;
                    exit(0);
                    break;
                case 'a' :
                  addition = true;
                  break;
            }

        }

        if (optind != argc) {
            if(!stdinput){
                __FileMenu->openFile(QFileInfo(argv[optind++]).absoluteFilePath());
            }
            else {
                cerr << "Could not read " << argv[optind++] << "." << endl
                     << "Option -s used. Read Standart Input." << endl;
            }
        }

#else
        if(argc == 2 || argc == 3){
                std::cout << "Geom 3D Viewer" << std::endl;
                QString option = argv[1];
                if(option == "-h" || option == "--help"){
                    std::cout << "Geom 3D Viewer" << std::endl;
                    std::cout << "Compiled the " << __DATE__ << "." << std::endl ;
                    std::cout << "Display 3D Geometric Shape." << std::endl << std::endl;
                    std::cout << "Usage:" << argv[0] <<" [ -h | --help  ] [ filename ]" << std::endl;
                    std::cout << "\t -h --help     : display this help." << std::endl;
                    exit(0);
                }
                else if (option == "-a" || option == "--add" && argc == 3){
                        __FileMenu->openFile(QFileInfo(argv[2]).absoluteFilePath ());
                }
                else if(option[0] == '-'){
                        std::cout << "Invalid option" << std::endl;
                }
                else {
                      __FileMenu->openFile(QFileInfo(option).absoluteFilePath ());
                }
        }
        else std::cout << "Geom 3D Viewer " << argc << std::endl;
#endif
    }
}

extern void qt_set_sequence_auto_mnemonic(bool b);

void Viewer::initialize()
{
  // qt_set_sequence_auto_mnemonic (true);
  setObjectName("PGLMainWindow");

  setWindowIcon(QPixmap(ViewerIcon::getPixmap(ViewerIcon::flower)));

  setWhatsThis(tr("<b>The PlantGL 3D Viewer</b><br><br>"
    "A Viewer for 3D scene.<br><br>"));

  setGeometry(50,50,780,675);
  __isFullScreen = false;
  __focusAtRefresh = true;

  __GLFrame->setGeometry(0,58,724,574);
  __GLFrame->setWhatsThis(tr("<b>The 3D Display</b><br><br>"
    "This frame displays the 3D scenes.<br><br>"
    "Pressing button of the mouse and moving allows you to move around the scene :<br>"
    "Press <i>Left Button</i> to rotate.<br>"
    "Press <i>Middle Button</i> to zoom.<br>"
    "Press <i>Rigth Button</i> to translate.<br><br>"
    "The same manipulation and pressing the Ctrl Key allows you to control the light source.<br><br>"
    "The same manipulation and pressing the Shift Key allows you to select shapes from the scene.<br>"
    ));

  setCentralWidget(__GLFrame);
  __GLFrame->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  __GLFrame->sizePolicy().setHorizontalStretch(3);

  // Control Group Box.
  __controlPanel = new ViewControlPanel(__GLFrame,"Control Panel",this);
  __controlPanel->setWhatsThis(tr("<b>The Control Panel</b><br><br>"
    "This panel shows you parameters from the <i>ligth source</i>, its position and its material.<br><br>"
    "It shows also parameters from the position of the <i>camera</i> "
    "and the size of an <i>unitary move</i> and of the <i>unit of the grid</i>"));
    addDockWidget(Qt::RightDockWidgetArea,__controlPanel);


  //menubar
  __MainMenu = new QMenuBar( this);
  setMenuBar(__MainMenu);

  __HelpMenu = new ViewHelpMenu(__MainMenu,__GLFrame,"Help Menu");
  QObject::connect(__GLFrame,SIGNAL(initMessage(const QString&)),
           __HelpMenu,SLOT(setInitText(const QString&)) );
  __GLFrame->rendererStatus();


  __FileMenu = new ViewFileManager(this,
                   "File Operation",
                   __GLFrame,
                   __HelpMenu,
                   __controlPanel);
  QToolBar * locatToolBar = __FileMenu->getLocationBar();
  addToolBar(Qt::TopToolBarArea,locatToolBar);
  addToolBarBreak();

  __GLFrame->addOtherToolBar(this);
  addToolBarBreak();

  /// ToolBar
  __ViewToolBar = new ViewToolBar("View ToolBar",this,"ViewToolBar");
  __ViewToolBar->setIconSize(QSize(32,32));
  addToolBar(__ViewToolBar);



  /// ToolBar
  __FileToolBar = new ViewToolBar("File ToolBar",this,"FileToolBar");
  __FileToolBar->setIconSize(QSize(32,32));
  addToolBar(__FileToolBar);



  __GLFrame->connectTo(__ErrorDialog);
  __FileMenu->connectTo(__ErrorDialog);


  // Preparation de la boite de dialog Object Browser
  __Browser= new ViewBrowser(this, "Object Browser");
  __Browser->setWhatsThis(tr("<b>Viewer Object Browser</b><br><br>"
    "This frame displays the structure and the parameters of the 3D scenes.<br><br>"
    "Double clicking on one of the shape will select it"
    ));
    setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea,__Browser);
    // __Browser->setFloating(true);
    __Browser->hide();


  // * View Menu
  // - Camera

  __ViewMenu = new QMenu( this );
  // - ToolBar
  QAction * _ViewMenuAction = __ViewMenu->addAction(tr("&Menu Bar"),this,SLOT(displayMenuBar()),Qt::Key_F6);
  _ViewMenuAction->setCheckable(true);
  _ViewMenuAction->setChecked(true);


  _ViewMenuAction = __controlPanel->toggleViewAction();
  __ViewMenu->addAction(_ViewMenuAction);
  _ViewMenuAction->setShortcut(Qt::Key_F7);

  _ViewMenuAction = __FileToolBar->toggleViewAction();
  __ViewMenu->addAction(_ViewMenuAction);
  _ViewMenuAction->setShortcut(Qt::Key_F8);

  _ViewMenuAction = __ViewToolBar->toggleViewAction();
  __ViewMenu->addAction(_ViewMenuAction);
  // _ViewMenuAction->setShortcut(Qt::Key_F9);


  if(__FileMenu->getLocationBar()){
    _ViewMenuAction = __FileMenu->getLocationBar()->toggleViewAction();
    __ViewMenu->addAction(_ViewMenuAction);
    _ViewMenuAction->setShortcut(Qt::Key_F10);
  }

  __ViewMenu->addSeparator();

  // - Browser
  _ViewMenuAction = __Browser->toggleViewAction();
  __ViewMenu->addAction(_ViewMenuAction);
  _ViewMenuAction->setShortcut(Qt::Key_F2);

  _ViewMenuAction = __ErrorDialog->toggleViewAction();
  __ViewMenu->addAction(_ViewMenuAction);
  _ViewMenuAction->setShortcut(Qt::Key_F3);


  __ViewMenu->addSeparator();
#ifdef _WIN32
  __ViewMenu->addAction(tr("&Debug Log"),this,SLOT(debugLog()));
  __ViewMenu->addSeparator();
#endif
  QMenu * FrameGLSize = new QMenu(tr("GL Frame Size"),__ViewMenu);
  __ViewMenu->addMenu(FrameGLSize);
  FrameGLSize->addAction(tr("800x600"),this,SLOT(set800x600FrameGL()));
  FrameGLSize->addAction(tr("640x480"),this,SLOT(set640x480FrameGL()));
  FrameGLSize->addAction(tr("512x384"),this,SLOT(set512x384FrameGL()));
  FrameGLSize->addAction(tr("320x240"),this,SLOT(set320x240FrameGL()));
  FrameGLSize->addSeparator();
  FrameGLSize->addAction(tr("Customize"),this,SLOT(setCustomFrameGLSize()),Qt::Key_F4);


  __ViewMenu->addSeparator();


  _ViewMenuAction = __ViewMenu->addAction(tr("GL Frame only"),this,SLOT(displayGLWidgetOnly()), Qt::Key_F10);
  QPixmap _fulls(ViewerIcon::getPixmap(ViewerIcon::fullscreen));
  __actFullScreen = __ViewMenu->addAction(_fulls,tr("Full Screen"),this,SLOT(displayFullScreen()), Qt::Key_F11);

  // The Status Bar.
  __statusBar= new ViewStatusBar(this);
  __statusBar->showMessage(tr("Ready"));
  __statusBar->setWhatsThis(tr("<b>The Status Bar</b>"));
  setStatusBar(__statusBar);

  __GLFrame->connectTo(__statusBar);
  __FileMenu->connectTo(__statusBar);
  //Title
  setWindowTitle(tr("PlantGL 3D Viewer"));

  __trayIcon = new QSystemTrayIcon(QPixmap(ViewerIcon::getPixmap(ViewerIcon::flower)),this);
  __trayIcon->setToolTip("PlantGL Viewer");
  QMenu * trayMenu = new QMenu(this);
  trayMenu->addAction(tr("About PlantGL Viewer ..."),__HelpMenu,SLOT(showAbout()));
  trayMenu->addSeparator();
  trayMenu->addAction(tr("GL Frame only"),this,SLOT(displayGLWidgetOnly()));
  trayMenu->addAction(tr("Full Screen"),this,SLOT(displayFullScreen()));
  trayMenu->addSeparator();
  trayMenu->addAction(tr("Unique PGL Windows"),this,SLOT(startDaemon()));
  trayMenu->addAction(tr("Multiple PGL Windows"),this,SLOT(stopDaemon()));
  trayMenu->addSeparator();
  trayMenu->addAction(tr("Exit"),this,SLOT(bye()));
  __trayIcon->setContextMenu(trayMenu);
  // __trayIcon->show();
  __trayIcon->showMessage("PlantGL","PlantGL Viewer ... Ready",QSystemTrayIcon::Information,2000);
  QObject::connect(__trayIcon,SIGNAL(messageClicked()),this,SLOT(appear()));
  QObject::connect(__trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(appear()));

  initializeRenderer();
  QObject::connect(__GLFrame,SIGNAL(rendererChanged()),
           this,SLOT(initializeRenderer()) );

  ViewerSettings settings;
  settings.beginGroup("Viewer");
  // qDebug("Try to retrieve %s Application Data.", qPrintable(ViewerSettings::getAppliName()));
  int version = settings.value("StateVersion",-1).toInt();
  if(version != -1)
  {
    QRect rect = settings.value("Geometry",geometry()).toRect();
    QRect maxrect = QApplication::desktop()->geometry();
    if( maxrect.contains(rect) && rect.width() > 100 && rect.height() > 100){
        setGeometry(rect);
        //qDebug("MainWindow.setGeometry(%i,%i,%i,%i)", rect.x(),rect.y(),rect.width(),rect.height());
    }

    QByteArray b = settings.value("State").toByteArray();
    restoreState(b,version);
    // qDebug("Restore State");
  }
  else qDebug("Cannot restore State");
  __focusAtRefresh = settings.value("FocusAtRefresh",__focusAtRefresh).toBool();
  settings.endGroup();

   setAttribute(Qt::WA_DeleteOnClose, false);

}

void
Viewer::displayTrayIcon(bool s){
    if(__trayIcon) {
        if(s)__trayIcon->show();
        else __trayIcon->hide();
    }
}

void
Viewer::startDaemon(){
  if(__service == NULL){
    __service = new ViewerDaemon(this);
    if(__service->isListening()){
      qDebug("Viewer Daemon Binding : Ok.");
      QObject::connect(__service,SIGNAL(requestReadFile(const QString&)),
        this,SLOT(openFile(const QString&)) );

      QObject::connect(__service,SIGNAL(requestAddFile(const QString&)),
        this,SLOT(addFile(const QString&)) );

      QObject::connect(__service,SIGNAL(requestShow()),
        this,SLOT(appear()) );

      QObject::connect(__service,SIGNAL(receiveRequest(const QString&)),
        this,SLOT(receiveRequest(const QString&)) );
    }
    else {
      qWarning("Viewer Daemon Binding : Failure.");
      stopDaemon();
    }
  }
}

void Viewer::stopDaemon(){
  if(__service){
    QObject::disconnect(__service,SIGNAL(requestReadFile(const QString&)),
      this,SLOT(openFile(const QString&)) );

    QObject::disconnect(__service,SIGNAL(requestAddFile(const QString&)),
      this,SLOT(addFile(const QString&)) );

    QObject::disconnect(__service,SIGNAL(requestShow()),
      this,SLOT(appear()) );

    QObject::disconnect(__service,SIGNAL(receiveRequest(const QString&)),
      this,SLOT(receiveRequest(const QString&)) );

    delete __service;
  }
  __service = NULL;
}

void Viewer::initializeRenderer()
{

  ViewRendererGL * renderer = __GLFrame->getSceneRenderer();
  renderer->connectTo(__statusBar);
  renderer->connectTo(__ErrorDialog);
  __Browser->setRenderer(renderer);
  __MainMenu->clear();
  if(__ToolsMenu){
    __ToolsMenu->clear();
    delete __ToolsMenu;
  }
  if(__EditMenu){
    __EditMenu->clear();
    delete __EditMenu;
  }



  __ToolsMenu = __GLFrame->createToolsMenu(__MainMenu);
  QAction *act=0;
  act = __ToolsMenu->defaultAction();
  if (act) {
     act->setWhatsThis(tr("The Tools Menu"));
     }

  __EditMenu  = __GLFrame->createEditMenu(__MainMenu);
  __FileMenu->setTitle(tr("&File"));
  __MainMenu->addMenu( __FileMenu);
  __EditMenu->setTitle(tr("&Edit"));
  __MainMenu->addMenu( __EditMenu);
  __ViewMenu->setTitle(tr("&View"));
  __MainMenu->addMenu( __ViewMenu);
  __ToolsMenu->setTitle(tr("&Tools"));
  __MainMenu->addMenu( __ToolsMenu);
  renderer->addOtherMenu(__MainMenu);
  __MainMenu->addSeparator();
  __HelpMenu->setTitle(tr("&Help"));
  __MainMenu->addMenu(__HelpMenu);

  __FileToolBar->clear();
  __FileMenu->fillToolBar(__FileToolBar);

  __ViewToolBar->clear();
  __GLFrame->fillToolBar(__ViewToolBar);

  renderer->addOtherToolBar(this);
}

Viewer::~Viewer()
{
}


void Viewer::bye()
{
  // emit
    close();
}

ViewRendererGL *
Viewer::getSceneRenderer() const
{
  return __GLFrame->getSceneRenderer();
}

void
Viewer::setSceneRenderer(ViewRendererGL * s)
{
  __GLFrame->setSceneRenderer(s);
}


std::vector<uint_t>
Viewer::getSelection() const
{
  if(__GLFrame->getSceneRenderer() == NULL) return std::vector<uint_t>(0);
  return __GLFrame->getSceneRenderer()->getSelectionIds();
}


void Viewer::whatsThis()
{
    QWhatsThis::enterWhatsThisMode();
}

void
Viewer::dragEnterEvent(QDragEnterEvent* myevent)
{
    qWarning("Viewer drag");
   if (myevent->mimeData()->hasUrls()) myevent->acceptProposedAction();
}

void
Viewer::dropEvent(QDropEvent* myevent)
{
    qWarning("Viewer drop");
    QList<QUrl> urls = myevent->mimeData()->urls();
    QList<QUrl>::const_iterator itUrls = urls.constBegin();
    if(myevent->dropAction() == Qt::CopyAction)
        __GLFrame->getSceneRenderer()->addFile(itUrls->toLocalFile());
    else
        __GLFrame->getSceneRenderer()->openFile(itUrls->toLocalFile());
    for(;itUrls != urls.constEnd();++itUrls ){
        __GLFrame->getSceneRenderer()->addFile(itUrls->toLocalFile());
    }
}


void
Viewer::keyPressEvent ( QKeyEvent * e)
{
  if( e->key() == (Qt::SHIFT+Qt::Key_F1))       __HelpMenu->showHelp();
  else if( e->key() == Qt::Key_F1)  __HelpMenu->showAbout();
  else if( e->key() == Qt::Key_F2)  __Browser->show();
  else if( e->key() == Qt::Key_F3)  __ErrorDialog->show();
  else if( e->key() == Qt::Key_F4)  setCustomFrameGLSize();
  else if( e->key() == Qt::Key_F5)  __GLFrame->getSceneRenderer()->refresh();
  else if( e->key() == Qt::Key_F6)  displayMenuBar();
  else if( e->key() == Qt::Key_F7)  __controlPanel->show();
  else if( e->key() == Qt::Key_F8)  __ViewToolBar->changeVisibility();
  else if( e->key() == Qt::Key_F10) displayGLWidgetOnly();
  else if( e->key() == Qt::Key_F11) displayFullScreen();
  else if( e->key() == Qt::Key_Escape) {
    if(__toolbarsvisibility){
      displayGLWidgetOnly();
      if(!__MainMenu->isVisible())displayMenuBar();
    }
    else if(__isFullScreen)displayFullScreen();
    else if(QMessageBox::critical(this,tr("Exit"),tr("Do you really want to exit ?"),1,2,0) == 1)
      bye();
  }
}


void Viewer::openFile(const QString& filename)
{
  QString f = filename.right(filename.length() - filename.lastIndexOf('/') -1);
  if(__trayIcon)__trayIcon->showMessage("PlantGL Viewer","Read file "+f,QSystemTrayIcon::Information,4000);
  __FileMenu->openFile(filename);
  appear();
}

void Viewer::addFile(const QString& filename)
{
  QString f = filename.right(filename.length() - filename.lastIndexOf('/') -1);
  if(__trayIcon)__trayIcon->showMessage("PlantGL Viewer","Add file "+f,QSystemTrayIcon::Information,4000);
  __GLFrame->getSceneRenderer()->addFile(filename);
  appear();
}

// #define DEBUG_EVENTDISPATCH

bool Viewer::event(QEvent *e){
#ifdef DEBUG_EVENTDISPATCH
    if(e->type() >= ViewEvent::eFirstEvent && e->type() <= ViewEvent::eLastEvent)
        printf("* ViewerThread : receive pgl event\n");
    // else printf("** receive event\n");
#endif
    return QMainWindow::event(e);
}


void  Viewer::customEvent(QEvent *e){
#ifdef DEBUG_EVENTDISPATCH
    printf("ViewerThread : receive Event\n");
#endif
#ifdef QT_THREAD_SUPPORT
  bool release_mutex = false;
  int etype = e->type();
  if(etype >= ViewEvent::eFirstEvent && etype < ViewGeomEvent::eLastGeomEvent)
      release_mutex = true;
#endif
  if(etype == ViewEvent::eSceneChange){
    ViewSceneChangeEvent * k = ( ViewSceneChangeEvent * )e;
    // QApplication::postEvent(__GLFrame->getSceneRenderer(),k->copy());
    __GLFrame->getSceneRenderer()->sceneChangeEvent(k);
    if(!isHidden()&&__GLFrame->isRedrawEnabled()){
        if(__focusAtRefresh) {
            activateWindow();
            raise();
        }
    }
  }
  else if(etype == ViewEvent::eEnd){
    hide();
  }
  else if(etype == ViewEvent::eShow){
    show();
  }
  else if(etype == ViewEvent::eGetSelection){
    ViewSelectRecoverEvent * k = ( ViewSelectRecoverEvent * )e;
    *k->result = getSelection();
  }
  else if(etype == ViewEvent::eSetSelection){
    ViewSelectionSet * k = ( ViewSelectionSet * )e;
    __GLFrame->getSceneRenderer()->selectionIdEvent(k->arg1);
  }
  else if(etype == ViewEvent::eWaitSelection){
      ViewWaitSelection * k = (ViewWaitSelection *)e;
      if( !__GLFrame->getSceneRenderer()->isEmpty() ) {
          if(!k->arg1.isEmpty()){
              __GLFrame->showMessage(k->arg1,0);
              statusBar()->showMessage(k->arg1);
          }
          SelectionListener s;
          QObject::connect(__GLFrame,SIGNAL(selectedShape(uint_t)),&s,SLOT(selectionMade(uint_t)));
          QObject::connect(this,SIGNAL(closing()),&s,SLOT(finalize()));
          bool aborted = false;
          while(!s.done && !(aborted = shouldAbortDialog())){
              if(!isVisible())show();
              qApp->processEvents();
          }
          QObject::disconnect(__GLFrame,SIGNAL(selectedShape(uint_t)),&s,SLOT(selectionMade(uint_t)));
          QObject::disconnect(this,SIGNAL(closing()),&s,SLOT(finalize()));
          __GLFrame->showMessage(k->arg1,600);
          if (!aborted) *k->result = s.selection;
          else *k->result = UINT32_MAX;
      }
      else *k->result = UINT32_MAX;
  }
  else if(etype == ViewEvent::eGetRedrawPolicy){
    ViewGetRedrawEvent * k = ( ViewGetRedrawEvent * )e;
    *k->result = __GLFrame->isRedrawEnabled();
  }
  else if(etype == ViewEvent::eSetRedrawPolicy){
    ViewSetRedrawEvent * k = ( ViewSetRedrawEvent * )e;
    __GLFrame->activateRedraw(k->arg1);
  }
  else if(etype == ViewEvent::eImageSave){
    ViewImageSaveEvent * k = ( ViewImageSaveEvent * )e;
    saveImage(k->arg1,qPrintable(k->arg2),k->arg3);
  }
  else if(etype == ViewEvent::eShowMessage){
    ViewShowMessageEvent * k = ( ViewShowMessageEvent * )e;
    __GLFrame->showMessage(k->arg1,k->arg2);
  }
  else if(etype == ViewEvent::eQuestion){
    ViewQuestionEvent * k = ( ViewQuestionEvent * )e;
    question(k->arg1,k->arg2,
             k->arg3,k->arg4,
             k->arg5,
             k->result);
  }
  else if(etype == ViewEvent::eFullScreen){
    ViewFullScreenEvent * k = ( ViewFullScreenEvent * )e;
    if(__isFullScreen != k->arg1)displayFullScreen();
  }
  else if(etype == ViewEvent::eGLFrameOnly){
    ViewGLFrameOnlyEvent * k = ( ViewGLFrameOnlyEvent * )e;
    if((__toolbarsvisibility == 0) == k->arg1)displayGLWidgetOnly();
  }
  else if(etype == ViewEvent::eItemSelection){
    ViewItemSelectionEvent * k = ( ViewItemSelectionEvent * )e;
    itemSelection(k->arg1,k->arg2,
             k->arg3,k->arg4,
             k->result,
             k->arg5);
  }
  else if(etype == ViewEvent::eDoubleSelection){
    ViewDoubleSelectionEvent * k = ( ViewDoubleSelectionEvent * )e;
    doubleSelection(k->arg1, k->arg2,
            k->arg3,k->arg4, k->arg5,
            k->result, k->arg6);
  }
  else if(etype == ViewEvent::eAnimation){
    ViewAnimationEvent * k = ( ViewAnimationEvent * )e;
    __GLFrame->setAnimation(k->arg1);
  }
  else if(etype == ViewEvent::eBgColor){
    ViewBgColorEvent * k = ( ViewBgColorEvent * )e;
    __GLFrame->setBackGroundColor(k->arg1);
  }
  else if(etype == ViewEvent::eGrid){
    ViewGridEvent * k = ( ViewGridEvent * )e;
    __GLFrame->gridEvent(k);
  }
  else if(etype == ViewEvent::eCameraGet || etype == ViewEvent::eCameraSet || etype == ViewEvent::eSetViewAngle || etype == ViewEvent::eGetViewAngle ){
    __GLFrame->cameraEvent((ViewEvent *)e);
  }
  else if(etype == ViewEvent::eLightGet || etype == ViewEvent::eLightSet){
    __GLFrame->getLight()->lightEvent((ViewEvent *)e);
  }
  else if(etype == ViewEvent::eClippingPlaneActivate || etype == ViewEvent::eClippingPlaneSet){
    __GLFrame->clippingPlaneEvent((ViewEvent *)e);
  }
  else if(etype == ViewEvent::ePos){
    ViewPosEvent * k = ( ViewPosEvent * )e;
    switch(k->arg5){
    case 0:
      setFrameGLSize(k->arg3,k->arg4);
      break;
    case 1:
      resize(k->arg3,k->arg4);
      break;
    case 2:
      move(k->arg1,k->arg2);
      break;
    case 3:
      setGeometry(k->arg1,k->arg2,k->arg3,k->arg4);
      break;
    }
    QApplication::processEvents();
  }
  else if(etype == ViewEvent::eFileSelection){
    ViewFileSelEvent * k = ( ViewFileSelEvent * )e;
    if(k->arg5)
      dirSelection(k->arg1,k->arg2,k->result);
    else
      fileSelection(k->arg1,k->arg3,k->arg2,k->arg4,k->result);
  }
  else if(etype == ViewEvent::eRayBuff){
    ViewRayBuffEvent * k = ( ViewRayBuffEvent * )e;
    *(k->result) = __GLFrame->castRays(k->arg1,k->arg2,k->arg3,k->arg4,k->arg5,k->arg6);
  }
  else if(etype == ViewEvent::eZBuff){
    ViewZBuffEvent * k = ( ViewZBuffEvent * )e;
    *(k->result) = __GLFrame->grabZBuffer();
  }
  else if(etype == ViewEvent::eZBuffPoints){
    ViewZBuffPointsEvent * k = ( ViewZBuffPointsEvent * )e;
    *(k->result) = __GLFrame->grabZBufferPoints();
  }
  else if(etype == ViewEvent::eZBuffPoints2){
    ViewZBuffPoints2Event * k = ( ViewZBuffPoints2Event * )e;
    *(k->result) = __GLFrame->grabZBufferPointsWithJitter(k->arg1, k->arg2, k->arg3);
  }
  else if(etype == ViewEvent::eProjSize){
    ViewProjSizeEvent * k = ( ViewProjSizeEvent * )e;
    *(k->result) = __GLFrame->getProjectionSize(k->arg1,k->arg2);
  }
  else if(etype == ViewEvent::eCameraProj){
    ViewCameraProjEvent * k = ( ViewCameraProjEvent * )e;
    __GLFrame->getCamera()->setProjectionMode(k->arg1);
  }
  else if(etype == ViewEvent::eSetAborter){
    ViewSetAborterEvent * k = ( ViewSetAborterEvent * )e;
    setAborter(k->arg1);
  }
  else if(etype >= ViewGeomEvent::eFirstGeomEvent && etype <= ViewGeomEvent::eLastGeomEvent){
    QApplication::sendEvent(__GLFrame->getSceneRenderer(),e);
  }
  else {
      if(etype < ViewEvent::eFirstEvent || etype > ViewGeomEvent::eLastGeomEvent)
          qDebug("Custom Event : Type=%i.", etype);
    QApplication::sendEvent(__GLFrame->getSceneRenderer(),e);
  }
#ifdef QT_THREAD_SUPPORT
  if(release_mutex){
    ViewEvent * k = dynamic_cast<ViewEvent *>(e);
    if(k->sent_event){
#ifdef DEBUG_EVENTDISPATCH
        printf("ViewerThread : ready to wake up caller.\n");
#endif
        send_lock_mutex.lock();
#ifdef DEBUG_EVENTDISPATCH
        printf("ViewerThread : wakeAll\n");
#endif
        send_lock_mutex.unlock();
        send_event_condition.wakeAll();
        // qApp->processEvents();
    }
  }
#endif
}



void Viewer::send(QEvent * e) {
#ifdef QT_THREAD_SUPPORT
    send_event_mutex.lock();
    bool inthread = (QThread::currentThread() == thread());
#ifdef DEBUG_EVENTDISPATCH
    if(inthread)printf("** Event dispatch in same thread.\n");
    else printf("** Event dispatch in concurrent threads.\n");
#endif
    if (!inthread){
#ifdef DEBUG_EVENTDISPATCH
        printf("QApp    thread id : %i\n",qApp->thread());
        printf("Viewer  thread id : %i\n",thread());
        printf("Current thread id : %i\n", QThread::currentThread());
#endif
        ViewEvent * b = dynamic_cast<ViewEvent *>(e);
        if(b)b->sent_event = true;
        send_lock_mutex.lock();
#ifdef DEBUG_EVENTDISPATCH
        printf("postEvent : %i\n",e->type());
#endif
        QApplication::postEvent( this, e );
#ifdef DEBUG_EVENTDISPATCH
        printf("Wait\n");
#endif
        send_event_condition.wait(&send_lock_mutex);
#ifdef DEBUG_EVENTDISPATCH
        printf("Release\n");
#endif
        send_lock_mutex.unlock();
#ifdef DEBUG_EVENTDISPATCH
        printf("Release done\n");
#endif
    }
    else {
#ifdef DEBUG_EVENTDISPATCH
        printf("Send within-thread event.\n");
#endif
        QApplication::sendEvent( this, e );
        delete e;
#ifdef DEBUG_EVENTDISPATCH
        printf("Send within-thread event done.\n");
#endif
    }
    send_event_mutex.unlock();
#else
    QApplication::sendEvent( this, e );
    delete e;
#endif
}

void Viewer::post(QEvent * e) {
    QApplication::postEvent( this, e );
}


QPointer<QMessageBox> ViewerMessageBox = 0;

void Viewer::closeEvent ( QCloseEvent * e)
{
  if(!ViewerMessageBox.isNull()&&ViewerMessageBox->isVisible()){
        ViewerMessageBox->close();
        // in case python thread is locked
        send_lock_mutex.unlock();
  }
  saveConfig();
  __ErrorDialog->registerQtMsg(false);
  emit closing();

  if (__standalone) e->accept();
  else { e->ignore(); hide(); }
}

void Viewer::showEvent ( QShowEvent * e)
{
  __ErrorDialog->registerQtMsg(true);
}

void Viewer::polish (){
  // __FileMenu->loadConfig();
  setStatusBarMsg(tr("Ready"));
}

void
Viewer::displayHelp() const
{
  __HelpMenu->showHelp();
}

void
Viewer::displayAbout() const
{
  __HelpMenu->showAbout();
}

void
Viewer::saveConfig() const
{
  ViewerSettings settings;
  settings.beginGroup("Viewer");
  int version = 0;
  settings.setValue("StateVersion",version);
  settings.setValue("State",saveState(version));
  settings.setValue("Geometry",geometry());
  settings.setValue("FocusAtRefresh",__focusAtRefresh);
  settings.endGroup();

  __GLFrame->endEvent();
  __HelpMenu->endEvent();
}


void Viewer::debugLog(){
#ifdef _WIN32
    RedirectIOToConsole();
#endif
}

void Viewer::displayMenuBar(){
  if(!__statusBar->isVisible()){
    __MainMenu->show();
    __statusBar->show();
  }
  else {
    __MainMenu->hide();
    __statusBar->hide();
  }
}

void
Viewer::displayFullScreen(){
  if(!__isFullScreen){
    __actFullScreen->setChecked(true);
    showFullScreen();
  }
  else{
    __actFullScreen->setChecked(false);
    showNormal();
  }
  __isFullScreen = !__isFullScreen;
}

void Viewer::displayGLWidgetOnly(){

  if( __statusBar->isVisible()||
      __controlPanel->isVisible() ||
      __ViewToolBar->isVisible() ||
      __FileToolBar->isVisible() ||
      __FileMenu->getLocationBar()->isVisible()){
      __toolbarsvisibility = 0;
    if(__statusBar->isVisible()){
      displayMenuBar();
      __toolbarsvisibility += 1;
    }
    if(__controlPanel->isVisible()){
      __controlPanel->hide();
      __toolbarsvisibility += 2;
    }
    if(__ViewToolBar->isVisible() ){
      __ViewToolBar->changeVisibility();
      __toolbarsvisibility += 4;
    }
    if(__FileToolBar->isVisible() ){
      __FileToolBar->hide();
      __toolbarsvisibility += 64;
    }
    if(__FileMenu->getLocationBar()->isVisible()){
      __FileMenu->getLocationBar()->changeVisibility();
      __toolbarsvisibility += 8;
    }
    if(__ErrorDialog->isVisible()){
      __ErrorDialog->hide();
      __toolbarsvisibility += 16;
    }
    if(__Browser->isVisible()){
      __Browser->hide();
      __toolbarsvisibility += 32;
    }
  }
  else {
    if(!__toolbarsvisibility || __toolbarsvisibility & 1 ){
      displayMenuBar();
    }
    if(!__toolbarsvisibility || __toolbarsvisibility & 2){
      __controlPanel->show();
    }
    if(!__toolbarsvisibility || __toolbarsvisibility & 4){
      __ViewToolBar->changeVisibility();
    }
    if(!__toolbarsvisibility || __toolbarsvisibility & 8){
      __FileMenu->getLocationBar()->changeVisibility();
    }
    if(!__toolbarsvisibility || __toolbarsvisibility & 16){
      __ErrorDialog->show();
    }
    if(!__toolbarsvisibility || __toolbarsvisibility & 32){
      __Browser->show();
    }
    if(!__toolbarsvisibility || __toolbarsvisibility & 64){
      __FileToolBar->show();
    }
    __toolbarsvisibility = 0;
 }
}

void Viewer::setStatusBarMsg(QString _msg){
    if (__statusBar->isVisible())__statusBar->showMessage(_msg,10000);
}

void
Viewer::receiveRequest(const QString& s){
  appear();
  qWarning("Net Request : '%s'",qPrintable(s));
}

void
Viewer::appear(){
  if(isMinimized())showNormal();
  activateWindow();
}

void Viewer::saveImage( QString _filename, const char* _format, bool withAlpha )
{
    if(__GLFrame->isPixelBufferUsed())
        activateWindow();
    __GLFrame->saveImage( _filename, _format );
}

void Viewer::set800x600FrameGL(){ setFrameGLSize(800,600); }
void Viewer::set640x480FrameGL(){ setFrameGLSize(640,480); }
void Viewer::set512x384FrameGL(){ setFrameGLSize(512,384); }
void Viewer::set320x240FrameGL(){ setFrameGLSize(320,240); }

void Viewer::setCustomFrameGLSize() {
  QDialog dialog(this);
  dialog.setModal(true);
  Ui::FrameGLDialog g;
  g.setupUi(&dialog);
  g.width->setRange(50,qApp->desktop()->width());
  g.height->setRange(50,qApp->desktop()->height());

  g.width->setValue(__GLFrame->width());
  g.height->setValue(__GLFrame->height());
  QObject::connect(g.buttonOk,SIGNAL(pressed()),&dialog,SLOT(accept()));
  QObject::connect(g.buttonCancel,SIGNAL(pressed()),&dialog,SLOT(reject()));

  if(dialog.exec())
    setFrameGLSize(g.width->value(),g.height->value());
}

void Viewer::setFrameGLSize(int width, int height){
  QSize s1 = size();
  QSize s2 = __GLFrame->size();
  int w = width - s2.width();
  int h = height - s2.height();
  resize(s1.width()+w,s1.height()+h);
}


QPoint * __messageBoxPos = NULL;

class ViewMessageBox : public QMessageBox {
public:
  ViewMessageBox(QWidget * parent, const QString& caption, const QString& text,
         const QString& but0txt, const QString& but1txt,
         const QString& but2txt,
         int * result = NULL):
      QMessageBox(caption,text,
              QMessageBox::Question,
              1,
              (but1txt.isEmpty()?QMessageBox::NoButton:2),
              (but2txt.isEmpty()?QMessageBox::NoButton:3),
                  parent),
      __result(result)
      {
      setModal(false);
      setAttribute(Qt::WA_DeleteOnClose,true);
        if(!but0txt.isEmpty())setButtonText(1,but0txt);
        if(!but1txt.isEmpty())setButtonText(2,but1txt);
        if(!but2txt.isEmpty())setButtonText(3,but2txt);
        if(__messageBoxPos)move(*__messageBoxPos);
        else {
          QPoint pos = parent->pos();
          QSize s = parent->size();
          pos.setX(pos.x() + s.width()*2/3);
          pos.setY(pos.y() + s.height()*2/3);
          move(pos);
        }
      }

  virtual ~ViewMessageBox(){
#ifdef DEBUG_EVENTDISPATCH
        printf("delete ViewMessageBox\n");
#endif
  }

  virtual void done ( int res ){
    // printf("done\n");
    if(__result)*__result = res;
    if(!__messageBoxPos)__messageBoxPos = new QPoint(pos());
    else *__messageBoxPos = pos();
    QDialog::done(res);
  }

  int * __result;
};

void
Viewer::question(const QString& caption, const QString& text,
                const QString& but0txt, const QString& but1txt,
                const QString& but2txt, int * result){
  ViewerMessageBox = new ViewMessageBox(this,caption,text,
          but0txt,but1txt,but2txt,
          result);

  activateWindow();
  ViewerMessageBox->show();
  bool aborting = false;
  while(!ViewerMessageBox.isNull() && ViewerMessageBox->isVisible()) {
      assert(qApp != NULL);
      qApp->processEvents();
      if (!aborting){
          aborting = shouldAbortDialog();
          if(aborting) {
              ViewerMessageBox->reject();
              qApp->processEvents();
          }
      }
  }
  if(!ViewerMessageBox.isNull()){
      delete ViewerMessageBox;
  }
}

void
Viewer::itemSelection(const QString& caption, const QString& text,
                      const QStringList& values, bool editable,
                      QString* result, bool * ok){
  activateWindow();
  *result = QInputDialog::getItem(this,caption,text,values,0,editable,ok);
}

void
Viewer::fileSelection(const QString& caption,
                      const QString& filter,
                      const QString& startPath,
                      bool existing,
                      QString* result){
  activateWindow();
  if(existing)
    *result = QFileDialog::getOpenFileName(this,caption,startPath,filter);
  else
    *result = QFileDialog::getSaveFileName(this,caption,startPath,filter);
}

void
Viewer::dirSelection(const QString& caption,
                     const QString& startPath,
                     QString* result){
  activateWindow();
  *result = QFileDialog::getExistingDirectory(this,caption,startPath);
}

void
Viewer::doubleSelection(const QString& caption, const QString& text, double value,
                        double minvalue, double maxvalue,  double* result, bool * ok){
  activateWindow();
  *result = QInputDialog::getDouble(this,caption,text,value,minvalue,maxvalue,2,ok);
}

void
Viewer::setAnimation(eAnimationFlag b){
  __GLFrame->setAnimation(b);
}
