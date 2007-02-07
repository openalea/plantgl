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

#include <qdragobject.h>
#include <qapplication.h>
#include <qstatusbar.h>
#include <qprogressbar.h>
#include <qspinbox.h>
#include <qpopupmenu.h>
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

#ifdef Q_WS_WIN
#include "trayicon.h"
#endif

#include <tool/util_enviro.h>

#include <iostream>


#ifdef _WIN32
#include "guicon.h"
#endif

using namespace std;
/* ----------------------------------------------------------------------- */

QMessageBox * ViewerMessageBox = 0;

Viewer::Viewer( QWidget * parent, const char * name, ViewRendererGL * r , WFlags f )
    : QMainWindow( parent , name, f ),
      __GLFrame(0),
      __EditMenu(0),
      __ToolsMenu(0),
      __service(0),
      __toolbarsvisibility(0)
#ifdef Q_WS_WIN
      ,__trayIcon(0)
#endif
{
  // if(QMessageBox::information(this,"Language","Select Language","English","French")==1)
#ifdef _WIN32
  if(TOOLS(getLanguage()) == "French")
    setFrenchTranslator();
#else
  #warning No translation on Linux
#endif

  // Preparation de la boite de dialog ErrorDialog
  __ErrorDialog= new ViewErrorDialog(this, "Viewer Error Dialog");

  __GLFrame = new ViewGLFrame(this,"FrameGL",r);
  initialize();
}

Viewer::Viewer( int argc, char ** argv, ViewRendererGL * r)
  : QMainWindow( 0 , "Geom 3D Viewer" , WType_TopLevel),
      __GLFrame(0),
      __EditMenu(0),
      __ToolsMenu(0),
      __service(0),
      __toolbarsvisibility(0)
#ifdef Q_WS_WIN
      ,__trayIcon(0)
#endif
{
  // if(QMessageBox::information(this,"Language","Select Language","English","French")==1)
#ifdef _WIN32
  if(TOOLS(getLanguage()) == "French")
    setFrenchTranslator();
#else
  #warning No translation on Linux
#endif

  // Preparation de la boite de dialog ErrorDialog
  __ErrorDialog= new ViewErrorDialog(this, "Viewer Error Dialog");

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
                    cout << "\t -s --stdin    : read geom description on standart input (useful for pipe)." << endl;
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
        else if(stdinput){
                __GLFrame->getSceneRenderer()->openStream(std::cin);
        }

#else
        if(argc == 2 || argc == 3){
                std::cout << "Geom 3D Viewer" << std::endl;
                QString option = argv[1];
                if(option == "-h" || option == "--help"){
                    std::cout << "Geom 3D Viewer" << std::endl;
                    std::cout << "Compiled the " << __DATE__ << "." << std::endl ;
                    std::cout << "Display 3D Geometric Shape." << std::endl << std::endl;
                    std::cout << "Usage:" << argv[0] <<" [ -h | --help | -s | --stdin ] [ filename ]" << std::endl;
                    std::cout << "\t -h --help     : display this help." << std::endl;
                    std::cout << "\t -s --stdin    : read geom description on standart input (useful for pipe)." << std::endl;
                    exit(0);
                }
                else if (option == "-s" || option == "--stdin"){
                    stdinput = true;
                }
                else if (option == "-a" || option == "--add" && argc == 3){
                        __FileMenu->openFile(QFileInfo(argv[2]).absFilePath ());
                }
                else if(option[0] == '-'){
                        std::cout << "Invalid option" << std::endl;
                }
                else {
                      __FileMenu->openFile(QFileInfo(option).absFilePath ());
                }
        }
        else std::cout << "Geom 3D Viewer " << argc << std::endl;
        if(stdinput){
                __GLFrame->getSceneRenderer()->openStream(std::cin);
        }
#endif
    }
}


void Viewer::initialize()
{
  setIcon(QPixmap(ViewerIcon::getPixmap(ViewerIcon::icon_flower)));

  QWhatsThis::add(this,tr("<b>The PlantGL 3D Viewer</b><br><br>"
    "A Viewer for 3D scene.<br><br>"));

  lineUpToolBars(false);
  setGeometry(50,50,780,675);
  __isFullScreen = false;

  __GLFrame->setGeometry(0,58,724,574);
  QWhatsThis::add(__GLFrame,tr("<b>The 3D Display</b><br><br>"
    "This frame displays the 3D scenes.<br><br>"
    "Pressing button of the mouse and moving allows you to move around the scene :<br>"
    "Press <i>Left Button</i> to rotate.<br>"
    "Press <i>Middle Button</i> to zoom.<br>"
    "Press <i>Rigth Button</i> to translate.<br><br>"
    "The same manipulation and pressing the Ctrl Key allows you to control the light source.<br><br>"
    "The same manipulation and pressing the Shift Key allows you to select shapes from the scene.<br>"
    ));

  setCentralWidget(__GLFrame);

  //menubar
  __MainMenu = new QMenuBar( this ,"Viewer MenuBar");

  __HelpMenu = new ViewHelpMenu(__MainMenu,__GLFrame,"Help Menu");
  QObject::connect(__GLFrame,SIGNAL(initMessage(const QString&)),
           __HelpMenu,SLOT(setInitText(const QString&)) );
  __GLFrame->rendererStatus();


  // Control Group Box.
  __controlPanel = new ViewControlPanel(__GLFrame,"Control Panel",this,QMainWindow::Right,TRUE,"Control Panel");
//  __controlPanel->hide();
  QWhatsThis::add(__controlPanel,tr("<b>The Control Panel</b><br><br>"
    "This panel shows you parameters from the <i>ligth source</i>, its position and its material.<br><br>"
    "It shows also parameters from the position of the <i>camera</i> "
    "and the size of an <i>unitary move</i> and of the <i>unit of the grid</i>"));

  __FileMenu = new ViewFileManager(this,
                   "File Operation",
                   __GLFrame,
                   __HelpMenu,
                   __controlPanel);

  /// ToolBar
  __ToolBar = new ViewToolBar("Viewer ToolBar",this,QMainWindow::Top, TRUE);
  __ToolBar->setHorizontalStretchable (false);


  __GLFrame->addOtherToolBar(this);

  __GLFrame->connectTo(__ErrorDialog);
  __FileMenu->connectTo(__ErrorDialog);


  // Preparation de la boite de dialog Object Browser
  __Browser= new ViewBrowser(this, "Viewer Object Browser");
  QWhatsThis::add(__Browser,tr("<b>Viewer Object Browser</b><br><br>"
    "This frame displays the structure and the parameters of the 3D scenes.<br><br>"
    "Double clicking on one of the shape will select it"
    ));


  // * View Menu
  // - Camera

  __ViewMenu = new QPopupMenu(this,"View Menu");
  __ViewMenu->setCheckable( TRUE );
  // - ToolBar
  int _idViewMenu = __ViewMenu->insertItem(tr("&Menu Bar"),this,SLOT(displayMenuBar()),Key_F6);
  __ViewMenu->setItemChecked(_idViewMenu,true);

  __idControlPanel = __ViewMenu->insertItem(tr("&Control Panel"),__controlPanel,
                                            SLOT(changeVisibility()),Key_F7);
  __idFileToolBar = __ViewMenu->insertItem(tr("&Tools Bar"),__ToolBar,
                                           SLOT(changeVisibility()),Key_F8);

  QObject::connect(__ToolBar,SIGNAL(__visibilityChanged(bool)),
           this,SLOT(checkFileToolBarItem(bool)) );

  QObject::connect(__controlPanel,SIGNAL(__visibilityChanged(bool)),
           this,SLOT(checkControlPanelItem(bool)) );

  if(__FileMenu->getLocationBar()){
    __idLocationToolBar = __ViewMenu->insertItem(tr("&Location Bar"),__FileMenu->getLocationBar(),
                                                 SLOT(changeVisibility()),Key_F9);
    QObject::connect(__FileMenu->getLocationBar(),
             SIGNAL(__visibilityChanged(bool)),
             this,SLOT(checkLocationBarItem(bool)) );
  }

  __ViewMenu->insertSeparator();

  // - Browser
  __idViewBrowser = __ViewMenu->insertItem(tr("&Object Browser"),__Browser,SLOT(show()),Key_F2);
  __idErrorDialog = __ViewMenu->insertItem(tr("&Error Log"),__ErrorDialog,SLOT(show()),Key_F3);

  __ViewMenu->insertSeparator();
#ifdef _WIN32
  __ViewMenu->insertItem(tr("&Debug Log"),this,SLOT(debugLog()));
  __ViewMenu->insertSeparator();
#endif
  QPopupMenu * FrameGLSize = new QPopupMenu(__ViewMenu,"FrameGL Size Menu");
  __ViewMenu->insertItem(tr("GL Frame Size"),FrameGLSize);
  FrameGLSize->insertItem(tr("800x600"),this,SLOT(set800x600FrameGL()));
  FrameGLSize->insertItem(tr("640x480"),this,SLOT(set640x480FrameGL()));
  FrameGLSize->insertItem(tr("512x384"),this,SLOT(set512x384FrameGL()));
  FrameGLSize->insertItem(tr("320x240"),this,SLOT(set320x240FrameGL()));
  FrameGLSize->insertSeparator();
  FrameGLSize->insertItem(tr("Customize"),this,SLOT(setCustomFrameGLSize()),Key_F4);

  QObject::connect(__ErrorDialog,SIGNAL(visibilityChanged(bool)),
                   this,SLOT(checkErrorDialogItem(bool)) );
  QObject::connect(__Browser,SIGNAL(visibilityChanged(bool)),
                   this,SLOT(checkViewBrowserItem(bool)) );

  __ViewMenu->insertSeparator();


  __ViewMenu->insertItem(tr("GL Frame only"),this,SLOT(displayGLWidgetOnly()), Key_F10);
  QPixmap _fulls(ViewerIcon::fullscreen);
  __idFullScreen = __ViewMenu->insertItem(_fulls,tr("Full Screen"),this,SLOT(displayFullScreen()), Key_F11);

  //menubar
//  __MainMenu->setFrameStyle(  QFrame::NoFrame );

  // The Status Bar.
  __statusBar= new ViewStatusBar(this,"Viewer Status Bar");
  __statusBar->message(tr("Ready"));

  QWhatsThis::add(__statusBar,tr("<b>The Status Bar</b>"));
  __GLFrame->connectTo(__statusBar);
  __FileMenu->connectTo(__statusBar);
  //Title
  setCaption(tr("PlantGL 3D Viewer"));

#ifdef Q_WS_WIN
  __trayIcon = new TrayIcon(QPixmap(ViewerIcon::getPixmap(ViewerIcon::icon_flower)),
								 "PlantGL Viewer",0,this,"PlantGL Viewer Tray Icon");
  __trayIcon->setToolTip("PlantGL Viewer");
  QPopupMenu * trayMenu = new QPopupMenu(this,"Tray Menu");
  trayMenu->insertItem(tr("About PlantGL Viewer ..."),__HelpMenu,SLOT(showAbout()));
  trayMenu->insertSeparator();
  trayMenu->insertItem(tr("GL Frame only"),this,SLOT(displayGLWidgetOnly()));
  trayMenu->insertItem(tr("Full Screen"),this,SLOT(displayFullScreen()));
  trayMenu->insertSeparator();
  trayMenu->insertItem(tr("Unique PGL Windows"),this,SLOT(startDaemon()));
  trayMenu->insertItem(tr("Multiple PGL Windows"),this,SLOT(stopDaemon()));
  trayMenu->insertSeparator();
  trayMenu->insertItem(tr("Exit"),this,SLOT(bye()));
  __trayIcon->setPopup(trayMenu);
  // __trayIcon->show();
  __trayIcon->setBalloonMessage("PlantGL","PlantGL Viewer ... Ready",2000);
  QObject::connect(__trayIcon,SIGNAL(balloonClicked(const QPoint&)),this,SLOT(appear()));
  QObject::connect(__trayIcon,SIGNAL(clicked(const QPoint&)),this,SLOT(appear()));
  QObject::connect(__trayIcon,SIGNAL(doubleClicked(const QPoint&)),this,SLOT(displayFullScreen()));
#endif

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
    if(__service->ok()){
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
  QWhatsThis::add(__ToolsMenu,tr("The Tools Menu"));

  __EditMenu  = __GLFrame->createEditMenu(__MainMenu);
  __MainMenu->insertItem(tr("&File"),  __FileMenu);
  __MainMenu->insertItem(tr("&Edit"),  __EditMenu);
  __MainMenu->insertItem(tr("&View"),  __ViewMenu);
  __MainMenu->insertItem(tr("&Tools"), __ToolsMenu);
  renderer->addOtherMenu(__MainMenu);
  __MainMenu->insertSeparator();
  __MainMenu->insertItem(tr("&Help"),  __HelpMenu);
  __ToolBar->clear();
  __ToolBar->setName("Viewer ToolBar");
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



void
Viewer::resizeEvent( QResizeEvent * event){
}

void
Viewer::mousePressEvent( QMouseEvent* event)
{
}

void
Viewer::mouseMoveEvent( QMouseEvent* event)
{
}

void
Viewer::dragEnterEvent(QDragEnterEvent* event)
{
  event->accept(QTextDrag::canDecode(event));
}

void
Viewer::dropEvent(QDropEvent* event)
{
  QString text;
  if ( QTextDrag::decode(event, text) && text.contains("file:"))
    __FileMenu->openFile(text.remove(0,5));

}


void
Viewer::keyPressEvent ( QKeyEvent * e)
{
  if( e->key() == (SHIFT+Key_F1))       __HelpMenu->showHelp();
  else if( e->key() == Key_F1)  __HelpMenu->showAbout();
  else if( e->key() == Key_F2)  __Browser->show();
  else if( e->key() == Key_F3)  __ErrorDialog->show();
  else if( e->key() == Key_F4)  setCustomFrameGLSize();
  else if( e->key() == Key_F5)  __GLFrame->getSceneRenderer()->refresh();
  else if( e->key() == Key_F6)  displayMenuBar();
  else if( e->key() == Key_F7)  __controlPanel->changeVisibility();
  else if( e->key() == Key_F8)  __ToolBar->changeVisibility();
  else if( e->key() == Key_F10) displayGLWidgetOnly();
  else if( e->key() == Key_F11) displayFullScreen();
  else if( e->key() == Key_Escape) {
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
  QString f = filename.right(filename.length() - filename.findRev('/') -1);
#ifdef Q_WS_WIN
  if(__trayIcon)__trayIcon->setBalloonMessage("PlantGL Viewer","Read file "+f,4000);
#endif
  __FileMenu->openFile(filename);
  appear();
}

void Viewer::addFile(const QString& filename)
{
  QString f = filename.right(filename.length() - filename.findRev('/') -1);
#ifdef Q_WS_WIN
  if(__trayIcon)__trayIcon->setBalloonMessage("PlantGL Viewer","Add file "+f,4000);
#endif
  __GLFrame->getSceneRenderer()->addFile(filename);
  appear();
}

void  Viewer::customEvent(QCustomEvent *e){
#ifdef QT_THREAD_SUPPORT
  bool release_mutex = true;
#endif
  if(e->type() == 12345){
	ViewSceneChangeEvent * k = ( ViewSceneChangeEvent * )e;
    QApplication::sendEvent(__GLFrame->getSceneRenderer(),k->copy());
      if(!isHidden()){
        if(!isActiveWindow())
          setActiveWindow();
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
	saveImage(k->filename,k->format.latin1(),k->withAlpha);
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
  else if(e->type() == 12365){
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



void Viewer::send(QCustomEvent * e) {
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

void Viewer::post(QCustomEvent * e) {
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
  __FileMenu->saveConfigBeforeQuit();
  __GLFrame->getSceneRenderer()->endEvent();
  __ErrorDialog->registerQtMsg(false);
  e->accept();
}

void Viewer::showEvent ( QShowEvent * e)
{
  __ErrorDialog->registerQtMsg(true);
}

void Viewer::polish (){
  QMainWindow::polish();
  __FileMenu->loadConfig();
  setStatusBar(tr("Ready"));
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

void Viewer::checkFileToolBarItem(bool b){
    __ViewMenu->setItemChecked(__idFileToolBar,b);
}

void Viewer::checkViewBrowserItem(bool visibility){
    __ViewMenu->setItemChecked(__idViewBrowser,visibility);
}

void Viewer::checkErrorDialogItem(bool visibility){
    __ViewMenu->setItemChecked(__idErrorDialog,visibility);
}

void Viewer::checkControlPanelItem(bool visibility){
    __ViewMenu->setItemChecked(__idControlPanel,visibility);
}

void Viewer::checkLocationBarItem(bool visibility){
    __ViewMenu->setItemChecked(__idLocationToolBar,visibility);
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
    showFullScreen();
    __ViewMenu->setItemChecked(__idFullScreen,TRUE);
  }
  else{
    __ViewMenu->setItemChecked(__idFullScreen,FALSE);
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

void Viewer::setStatusBar(QString _msg){
    __statusBar->message(_msg,10000);
}

void
Viewer::receiveRequest(const QString& s){
  appear();
  qWarning("Net Request : '"+s+"'");
}

void
Viewer::appear(){
  if(isMinimized())showNormal();
  setActiveWindow();
}

void Viewer::saveImage( QString _filename, const char* _format, bool withAlpha )
{
  setActiveWindow();
  __GLFrame->repaint(true);
  __GLFrame->saveImage( _filename, _format );
}

void Viewer::set800x600FrameGL(){ setFrameGLSize(800,600); }
void Viewer::set640x480FrameGL(){ setFrameGLSize(640,480); }
void Viewer::set512x384FrameGL(){ setFrameGLSize(512,384); }
void Viewer::set320x240FrameGL(){ setFrameGLSize(320,240); }

void Viewer::setCustomFrameGLSize() { 
#if QT_VERSION >= 300
  FrameGLDialog g(this,"FrameGL Size",true);

  g.width->setMinValue(50); 
  g.width->setMaxValue(qApp->desktop()->width()); 
  g.height->setMinValue(50); 
  g.height->setMaxValue(qApp->desktop()->height()); 

  g.width->setValue(__GLFrame->width()); 
  g.height->setValue(__GLFrame->height()); 

  if(g.exec())
	setFrameGLSize(g.width->value(),g.height->value()); 
#else
  QMessageBox::warning(self,"FrameGL Size","This functionnality works only with version of Qt >= 3.0.0. Upgrade.");
#endif
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
				  parent,"Viewer Question",false),
	  __result(result)
#ifdef QT_THREAD_SUPPORT
	  , send_event_condition(w)
#endif
	  {
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

  setActiveWindow();
  ViewerMessageBox->show();
}

void 
Viewer::itemSelection(const QString& caption, const QString& text,
					  const QStringList& values, bool editable,
					  QString* result, bool * ok){
  setActiveWindow();
  *result = QInputDialog::getItem(caption,text,values,0,editable,ok,this,"itemSelection");
}

void 
Viewer::fileSelection(const QString& caption, 
					  const QString& filter,
					  const QString& startPath,
					  bool existing,
					  QString* result){
  setActiveWindow();
  if(existing)
    *result = QFileDialog::getOpenFileName(startPath,filter,this,"Choose File",caption);
  else
    *result = QFileDialog::getSaveFileName(startPath,filter,this,"Choose File",caption);
}

void 
Viewer::dirSelection(const QString& caption, 
					 const QString& startPath,
					 QString* result){
  setActiveWindow();
  *result = QFileDialog::getExistingDirectory(startPath,this,"Choose Directory",caption);
}

void
Viewer::animation(bool b){
  __GLFrame->animation(b);
}
