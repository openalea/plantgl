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
#ifdef __GNUC__
#include <getopt.h>
#else
#include <qstring.h>
#endif

// #include <qdragobject.h>
#include <qapplication.h>
#include <qstatusbar.h>
#include <qprogressbar.h>
#include <qspinbox.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qwhatsthis.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qclipboard.h>
#include <qinputdialog.h>
#include <qsystemtrayicon.h>
#include <qurl.h>
#include <qdesktopwidget.h>

#include "viewer.h"
#include "event.h"
#include "scenegl.h"
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
#include "util_qwidget.h"
#include "interface/frameglsizedialog.h"

#include <tool/util_enviro.h>

#include <iostream>


#ifdef _WIN32
#include "guicon.h"
#endif

using namespace std;
/* ----------------------------------------------------------------------- */

QMessageBox * ViewerMessageBox = 0;

Viewer::Viewer( QWidget * parent, const char * name, ViewRendererGL * r , Qt::WindowFlags f )
    : QMainWindow( parent ,  f ),
      __GLFrame(0),
      __EditMenu(0),
      __ToolsMenu(0),
      __service(0),
      __toolbarsvisibility(0)
      ,__trayIcon(0)
{
	setObjectName(name);
#ifdef _WIN32
  if(TOOLS(getLanguage()) == "French")
    setFrenchTranslator();
#else
  #warning No translation on Linux
#endif

  // Preparation de la boite de dialog ErrorDialog
  __ErrorDialog= new ViewErrorDialog(this);
  addDockWidget(Qt::BottomDockWidgetArea,__ErrorDialog);

  __GLFrame = new ViewGLFrame(this,"FrameGL",r);
  initialize();
}

Viewer::Viewer( int argc, char ** argv, ViewRendererGL * r)
  : QMainWindow( 0 ),
      __GLFrame(0),
      __EditMenu(0),
      __ToolsMenu(0),
      __service(0),
      __toolbarsvisibility(0),
      __trayIcon(0)
{
  // if(QMessageBox::information(this,"Language","Select Language","English","French")==1)
  if(TOOLS(getLanguage()) == "French")
    setFrenchTranslator();

  // Preparation de la boite de dialog ErrorDialog
  __ErrorDialog= new ViewErrorDialog(this);
  addDockWidget(Qt::BottomDockWidgetArea,__ErrorDialog);

  if(argv!=NULL){
    QFileInfo f(argv[0]);
    ViewFileManager::FILE_CONFIG = f.baseName();
  }

    __GLFrame = new ViewGLFrame(this,"FrameGL",r);
    initialize();
    bool stdinput = false;

    if( argc != 0 ){
#ifdef __GNUC__
                /// Read of calling options
        char * optstring = "hs";

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
                __FileMenu->openFile(QFileInfo(argv[optind++]).absFilePath());
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


void Viewer::initialize()
{
  setWindowIcon(QPixmap(ViewerIcon::getPixmap(ViewerIcon::flower)));

  setWhatsThis(tr("<b>The PlantGL 3D Viewer</b><br><br>"
    "A Viewer for 3D scene.<br><br>"));

  setGeometry(50,50,780,675);
  __isFullScreen = false;

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

  /// ToolBar
  __ToolBar = new ViewToolBar("Viewer ToolBar",this);
  addToolBar(__ToolBar);


  __GLFrame->addOtherToolBar(this);

  __GLFrame->connectTo(__ErrorDialog);
  __FileMenu->connectTo(__ErrorDialog);


  // Preparation de la boite de dialog Object Browser
  __Browser= new ViewBrowser(this, "Viewer Object Browser");
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

  _ViewMenuAction = __ToolBar->toggleViewAction();
  __ViewMenu->addAction(_ViewMenuAction);
  _ViewMenuAction->setShortcut(Qt::Key_F8);


  if(__FileMenu->getLocationBar()){
    _ViewMenuAction = __FileMenu->getLocationBar()->toggleViewAction();
    __ViewMenu->addAction(_ViewMenuAction);
    _ViewMenuAction->setShortcut(Qt::Key_F9);
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

}

void 
Viewer::displayTrayIcon(bool s){
#ifdef Q_WS_WIN
	if(__trayIcon)
		if(s)__trayIcon->show();
		else __trayIcon->hide();
#endif
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
  __ToolsMenu->setWhatsThis(tr("The Tools Menu"));

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
  __ToolBar->clear();
  // __ToolBar->setTitle("Viewer ToolBar");
  __FileMenu->fillToolBar(__ToolBar);
  __ToolBar->addSeparator();
  __GLFrame->fillToolBar(__ToolBar);
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


std::vector<uint32_t>
Viewer::getSelection() const
{
  if(__GLFrame->getSceneRenderer() == NULL) return std::vector<uint32_t>(0);
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
  else if( e->key() == Qt::Key_F8)  __ToolBar->changeVisibility();
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

void  Viewer::customEvent(QEvent *e){
#ifdef QT_THREAD_SUPPORT
  bool release_mutex = true;
#endif
  if(e->type() == 12345){
	ViewSceneChangeEvent * k = ( ViewSceneChangeEvent * )e;
    QApplication::sendEvent(__GLFrame->getSceneRenderer(),k->copy());
      if(!isHidden()){
        if(!isActiveWindow())
          activateWindow();
      }
  }

  else if(e->type() == 12346){
    bye();
  }
  else if(e->type() == 12347){
    ViewSelectRecoverEvent * k = ( ViewSelectRecoverEvent * )e;
    k->setSelection(getSelection());
  }
  else if(e->type() == 12355){
    ViewSelectionSet * k = ( ViewSelectionSet * )e;
    __GLFrame->getSceneRenderer()->selectionIdEvent(k->getSelection());
  }
  else if(e->type() == 12350){
    ViewImageSaveEvent * k = ( ViewImageSaveEvent * )e;
	saveImage(k->filename,k->format.toAscii().constData(),k->withAlpha);
  }
  else if(e->type() == 12351){
    ViewQuestionEvent * k = ( ViewQuestionEvent * )e;
	question(k->caption,k->text,
			 k->but0txt,k->but1txt,
			 k->but2txt,
			 k->result);
#ifdef QT_THREAD_SUPPORT
	release_mutex = false;
#endif
  }
  else if(e->type() == 12352){
    ViewFullScreenEvent * k = ( ViewFullScreenEvent * )e;
	if(__isFullScreen != k->value)displayFullScreen();
  }
  else if(e->type() == 12353){
    ViewGLFrameOnlyEvent * k = ( ViewGLFrameOnlyEvent * )e;
	if((__toolbarsvisibility == 0) == k->value)displayGLWidgetOnly();
  }
  else if(e->type() == 12354){
    ViewItemSelectionEvent * k = ( ViewItemSelectionEvent * )e;
	itemSelection(k->caption,k->text,
			 k->values,k->editable,
			 k->result,
			 k->ok);
  }
  else if(e->type() == 12356){
    ViewAnimationEvent * k = ( ViewAnimationEvent * )e;
	__GLFrame->animation(k->mode);
  }
  else if(e->type() == 12357){
    ViewBgColorEvent * k = ( ViewBgColorEvent * )e;
	__GLFrame->setBackGroundColor(k->color);
  }
  else if(e->type() == 12358){
    ViewGridEvent * k = ( ViewGridEvent * )e;
	__GLFrame->gridEvent(k);
  }
  else if(e->type() == 12359){
    ViewCameraEvent * k = ( ViewCameraEvent * )e;
	__GLFrame->cameraEvent(k);
  }
  else if(e->type() == 12360){
    ViewPosEvent * k = ( ViewPosEvent * )e;
	switch(k->def){
	case 0:
	  setFrameGLSize(k->w,k->h);
	  break;
	case 1:
	  resize(k->w,k->h);
	  break;
	case 2:
	  move(k->x,k->y);
	  break;
	case 3:
	  setGeometry(k->x,k->y,k->w,k->h);
	  break;
	}
  }
  else if(e->type() == 12361){
    ViewFileSelEvent * k = ( ViewFileSelEvent * )e;
	if(k->dir)
	  dirSelection(k->caption,k->startPath,k->result);
	else
	  fileSelection(k->caption,k->filter,k->startPath,k->existing,k->result);
  }
  else if(e->type() == 12362){
    ViewRayBuffEvent * k = ( ViewRayBuffEvent * )e;
	*(k->result) = __GLFrame->castRays(k->pos,k->dir,k->dx,k->dy,k->sx,k->sy);
  }
  else if(e->type() == 12366){
    ViewZBuffEvent * k = ( ViewZBuffEvent * )e;
	*(k->result) = __GLFrame->grabZBuffer();
  }
  else if(e->type() == 12363){
    ViewProjSizeEvent * k = ( ViewProjSizeEvent * )e;
	*(k->size) = __GLFrame->getProjectionSize(k->nbpixel,k->pixelwidth);
  }
  else if(e->type() == 12364){
    ViewCameraProjEvent * k = ( ViewCameraProjEvent * )e;
	__GLFrame->getCamera()->setProjectionMode(k->mode);
  }
  else if(e->type() >= 12365 && e->type() <= 12367){
    QApplication::sendEvent(__GLFrame->getSceneRenderer(),e);
  }
  else {
    QApplication::sendEvent(__GLFrame->getSceneRenderer(),e);
  }
#ifdef QT_THREAD_SUPPORT
  if(release_mutex){
	ViewEvent * k = dynamic_cast<ViewEvent *>(e);
	if(k->sent_event){
		while(!send_lock_mutex.tryLock());
		send_lock_mutex.unlock();
		send_event_condition.wakeAll();
	}
  }
#endif
}



void Viewer::send(QEvent * e) {
#ifdef QT_THREAD_SUPPORT
    send_event_mutex.lock();
	ViewEvent * b = dynamic_cast<ViewEvent *>(e);
	if(b)b->sent_event = true;
	send_lock_mutex.lock();
#endif
    QApplication::postEvent( this, e );
#ifdef QT_THREAD_SUPPORT
	send_event_condition.wait(&send_lock_mutex);
	send_lock_mutex.unlock();
    send_event_mutex.unlock();
#endif
}

void Viewer::post(QEvent * e) {
#ifdef QT_THREAD_SUPPORT
    send_event_mutex.lock();
#endif
    QApplication::postEvent( this, e );
#ifdef QT_THREAD_SUPPORT
    send_event_mutex.unlock();
#endif
}


void Viewer::closeEvent ( QCloseEvent * e)
{
  if(ViewerMessageBox&&ViewerMessageBox->isVisible())ViewerMessageBox->close();
  __FileMenu->saveConfig();
  __GLFrame->getSceneRenderer()->endEvent();
  __ErrorDialog->registerQtMsg(false);
  e->accept();
}

void Viewer::showEvent ( QShowEvent * e)
{
  __ErrorDialog->registerQtMsg(true);
}

void Viewer::polish (){
  __FileMenu->loadConfig();
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
  __FileMenu->saveConfig();
}


void Viewer::debugLog(){
#ifdef _WIN32
    RedirectIOToConsole();
#endif
}

void Viewer::displayMenuBar(){
  if(!__MainMenu->isVisible()){
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

  if( __MainMenu->isVisible()||
	  __controlPanel->isVisible() ||
	  __ToolBar->isVisible() ||
	  __FileMenu->getLocationBar()->isVisible()){
	__toolbarsvisibility = 0;
	if(__MainMenu->isVisible()){
	  displayMenuBar();
	  __toolbarsvisibility += 1;
	}
	if(__controlPanel->isVisible()){
	  __controlPanel->hide();
	  __toolbarsvisibility += 2;
	}
	if(__ToolBar->isVisible() ){
	  __ToolBar->changeVisibility();
	  __toolbarsvisibility += 4;
	}
	if(__FileMenu->getLocationBar()->isVisible()){
	  __FileMenu->getLocationBar()->changeVisibility();
	  __toolbarsvisibility += 8;
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
	  __ToolBar->changeVisibility();
	}
	if(!__toolbarsvisibility || __toolbarsvisibility & 8){
	  __FileMenu->getLocationBar()->changeVisibility();
	}
 	__toolbarsvisibility = 0;
 }
}

void Viewer::setStatusBarMsg(QString _msg){
    __statusBar->showMessage(_msg,10000);
}

void
Viewer::receiveRequest(const QString& s){
  appear();
  qWarning(("Net Request : '"+s+"'").toAscii().data());
}

void
Viewer::appear(){
  if(isMinimized())showNormal();
  activateWindow();
}

void Viewer::saveImage( QString _filename, const char* _format, bool withAlpha )
{
  activateWindow();
  __GLFrame->updateGL();
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
#ifdef QT_THREAD_SUPPORT
		 QWaitCondition& w,
#endif
		 int * result = NULL):
	  QMessageBox(caption,text,
			  QMessageBox::Question,
			  1,
			  (but1txt.isEmpty()?QMessageBox::NoButton:2),
			  (but2txt.isEmpty()?QMessageBox::NoButton:3),
				  parent),
	  __result(result)
#ifdef QT_THREAD_SUPPORT
	  , send_event_condition(w)
#endif
	  {
	  setModal(false);
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

  virtual ~ViewMessageBox(){}

  virtual void done ( int res ){
	if(__result)*__result = res;
#ifdef QT_THREAD_SUPPORT
    send_event_condition.wakeAll();
#endif
	if(!__messageBoxPos)__messageBoxPos = new QPoint(pos());
	else *__messageBoxPos = pos();
	QDialog::done(res);
  }

#ifdef QT_THREAD_SUPPORT
  QWaitCondition& send_event_condition;
#endif
  int * __result;
};

void 
Viewer::question(const QString& caption, const QString& text,
			    const QString& but0txt, const QString& but1txt,
			    const QString& but2txt, int * result){
  ViewerMessageBox = new ViewMessageBox(this,caption,text,
		  but0txt,but1txt,but2txt,
#ifdef QT_THREAD_SUPPORT
		  send_event_condition,
#endif
		  result);

  activateWindow();
  ViewerMessageBox->show();
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
Viewer::animation(bool b){
  __GLFrame->animation(b);
}
