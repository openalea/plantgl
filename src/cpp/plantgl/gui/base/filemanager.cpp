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



#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/util_enviro.h>
#include "filemanager.h"
#include "icons.h"
#include "properties.h"
#include "glframe.h"

#include <sstream>

#include <time.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qfile.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qregexp.h>

#include <QtGui/qbitmap.h>
#include <QtGui/qimage.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qimagewriter.h>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qfiledialog.h>
    #include <QtWidgets/qmessagebox.h>
    #include <QtWidgets/qtoolbutton.h>
    #include <QtWidgets/qcombobox.h>
    #include <QtWidgets/qlabel.h>
    #include <QtWidgets/qstatusbar.h>
    #include <QtWidgets/qapplication.h>
    #include <QtWidgets/qwhatsthis.h>
    #include <QtWidgets/qinputdialog.h>
    #include <QtWidgets/qdesktopwidget.h>
    #include <QtWidgets/qmainwindow.h>
#else
    #include <QtGui/qfiledialog.h>
    #include <QtGui/qmessagebox.h>
    #include <QtGui/qtoolbutton.h>
    #include <QtGui/qcombobox.h>
    #include <QtGui/qlabel.h>
    #include <QtGui/qstatusbar.h>
    #include <QtGui/qapplication.h>
    #include <QtGui/qwhatsthis.h>
    #include <QtGui/qinputdialog.h>
    #include <QtGui/qdesktopwidget.h>
    #include <QtGui/qmainwindow.h>
#endif

#include <QtOpenGL/qgl.h>

#include "grid.h"
#include "camera.h"
#include "controlpanel.h"
#include "scenegl.h"
#include "locationbar.h"
#include "helpmenu.h"
#include "errordialog.h"
#include "util_qwidget.h"
#include "configuration.h"

PGL_USING_NAMESPACE

/*------------------------------------------------------------------------------------*/


const uint ViewFileManager::REVIEW_SIZE(50);

/*------------------------------------------------------------------------------------*/


ViewFileManager::ViewFileManager(QMainWindow * parent,
                                 const char * name,
                                 ViewGLFrame * frame,
                                 ViewHelpMenu * helpmenu,
                                 ViewControlPanel * controlpanel)
    : QMenu( name, parent ),
      __hasOpenFile(false),
      __GLFrame(frame),
      __controlPanel(controlpanel),
      __helpmenu(helpmenu),
      __propposition(-1,-1)
{
    if(name)setObjectName(name);
    QObject::connect(__GLFrame,SIGNAL(rendererChanged()), this,SLOT(initialize()));
    __locatToolBar = new ViewLocationBar(QString("Location ToolBar"), parent,"LocationBar");
  __OpenFileMenu = new QMenu( this );
  __ImportFileMenu = new QMenu( this );
  __SaveFileMenu = new QMenu( this );
  __ExportFileMenu = new QMenu( this );
  __RecentFilesMenu = new QMenu( this );
  initialize();

}

void
ViewFileManager::initialize(){
  clear();
  ViewRendererGL * scene = __GLFrame->getSceneRenderer();
  QObject::connect(scene,SIGNAL(filenameChanged(const QString&)),
           this,SLOT(addOpenFile(const QString&)));

  // *  The popup menu *

  QPixmap doc(ViewerIcon::getPixmap(ViewerIcon::document));
  QPixmap openIcon(ViewerIcon::getPixmap(ViewerIcon::fileopen) );
  QPixmap refreshIcon( ViewerIcon::getPixmap(ViewerIcon::reload) );
  QPixmap exitIcon( ViewerIcon::getPixmap(ViewerIcon::exit) );
  QPixmap saveIcon( ViewerIcon::getPixmap(ViewerIcon::filefloppy) );
  QPixmap savePicIcon( ViewerIcon::getPixmap(ViewerIcon::camerafile) );
  QPixmap copyPicIcon( ViewerIcon::getPixmap(ViewerIcon::camera) );
  QPixmap printIcon(ViewerIcon::getPixmap(ViewerIcon::fileprint) );

  QAction* act=0;

   addMenu( __OpenFileMenu );
  __OpenFileMenu->setIcon(openIcon);
  __OpenFileMenu->setTitle(tr("Open"));
  act = __OpenFileMenu->defaultAction();
  if (act) {
     act->setWhatsThis("<b>Open Menu</b><br>Menu related to file loading<br>");
     }

  // Open File Menu
  __OpenFileMenu->clear();
  act = __OpenFileMenu->addAction( openIcon , tr("&Open File"),this,SLOT(openFile()),Qt::CTRL+Qt::Key_O);
  act->setWhatsThis("<b>Open File</b><br><br>Load 3D scene from a file.<br>");

  scene->addOpenEntries(__OpenFileMenu);
  addSeparator();

  // Import File Menu
  __ImportFileMenu->clear();

  if(scene->addImportEntries(__ImportFileMenu)){
    __ImportFileMenu->setTitle(tr("Import"));
    __ImportFileMenu->setIcon(openIcon);
    act = __ImportFileMenu->defaultAction();
    if (act) {
       act->setWhatsThis("<b>Import Menu</b><br>Importing scene from various file format<br>");
       }
    addMenu(__ImportFileMenu );
    addSeparator();
  }

  // Save File Menu
  __SaveFileMenu->clear();
  __SaveFileMenu->setIcon(openIcon);
  __SaveFileMenu->setTitle(tr("Save"));
  act = __SaveFileMenu->defaultAction();
    if (act) {
      act->setWhatsThis("<b>Save Menu</b><br>Menu related to file saving<br>");
      }
  addMenu( __SaveFileMenu );

  act = __SaveFileMenu->addAction( saveIcon , tr("&Save"),scene,SLOT(save()),Qt::CTRL+Qt::Key_S);
    if (act) {
  act->setWhatsThis("<b>Save</b><br>Save current scene.<br>");
  }
  act = __SaveFileMenu->addAction( saveIcon , tr("&Save As"),scene,SLOT(saveAs()));
    if (act) {
      act->setWhatsThis("<b>Save</b><br>Save current scene in a new file.<br>");
      }
  scene->addSaveEntries(__SaveFileMenu);

  addSeparator();

  __ExportFileMenu->clear();
  if(scene->addExportEntries(__ExportFileMenu)){
      __ExportFileMenu->setIcon(openIcon);
      __ExportFileMenu->setTitle(tr("Export"));
      addMenu( __ExportFileMenu );
      addSeparator();
  }

  QMenu * __screenshot = new QMenu(this);
  __screenshot->setTitle(tr("ScreenShot"));
  __screenshot->setIcon(copyPicIcon);
  act = __screenshot->defaultAction();
    if (act) {
      act->setWhatsThis("<b>ScreenShot Menu</b><br>Menu related to screenshot<br>");
      }
  addMenu( __screenshot );

  act = __screenshot->addAction( savePicIcon , tr("Save as Bitmap"),this,SLOT(saveImage()),Qt::CTRL+Qt::Key_B);
  act->setWhatsThis("<b>Save ScreenShot as Bitmap</b><br><br>Take a screenshot and save it on one of the various picture formats.<br>");

  act = __screenshot->addAction( savePicIcon , tr("Save as Bitmap with Alpha"),this,SLOT(saveImageWithAlpha()),Qt::CTRL+Qt::SHIFT+Qt::Key_B);
  act->setWhatsThis("<b>Save ScreenShot as Bitmap</b><br><br>Take a screenshot and save it on one of the various picture formats.<br>");

  act = __screenshot->addAction( copyPicIcon , tr("Copy To Clipboard"),__GLFrame,SLOT(copyImageToClipboard()),Qt::CTRL+Qt::Key_C);
  act->setWhatsThis("<b>Copy ScreenShot to Clipboard</b><br><br>Take a screenshot and copy it on the global clipboard.<br>");

  act = __screenshot->addAction( copyPicIcon , tr("Copy To Clipboard with Alpha"),__GLFrame,SLOT(copyImageToClipboardWithAlpha()),Qt::CTRL+Qt::SHIFT+Qt::Key_C);
  act->setWhatsThis("<b>Copy ScreenShot to Clipboard</b><br><br>Take a screenshot and copy it on the global clipboard.<br>");

  addSeparator();

  // Recent Files Menu
  __RecentFilesMenu->setIcon(openIcon);
  __RecentFilesMenu->setTitle(tr("Recents"));
  addMenu(__RecentFilesMenu );
  act = __RecentFilesMenu->defaultAction();
      if (act) {
        act->setWhatsThis("<b>History Menu</b><br>This Menu contains all the file names previously opened<br>");
        }


  drawRecentFilesMenu();
  addSeparator();

  // Refresh  icon
  act = addAction( printIcon ,  tr("&Print..."),__GLFrame,SLOT(printImage()));
  act->setWhatsThis("<b>Print</b><br>Take a screenshot and print it.<br>");
  addSeparator();

  // Refresh  icon
  act = addAction( refreshIcon ,  tr("&Refresh"),this,SLOT(refresh()),Qt::Key_F5);
  act->setWhatsThis("<b>Refresh</b><br><br>Reload the current scene.<br>");
  addSeparator();

  // Close  icon
  QPixmap _close(ViewerIcon::getPixmap(ViewerIcon::fileclose));
  act = addAction( _close,tr("&Close"),scene,SLOT(close()));
  act->setWhatsThis("<b>Close</b><br><br>Close the current scene.<br>");
  addSeparator();

  // Properties icon
  act = addAction( doc ,  tr("Properties"), this,SLOT(properties()),Qt::CTRL+Qt::Key_P);
  act->setWhatsThis("<b>Properties</b><br><br>Properties of the current scene,<br> selected shapes, file, camera, ... .<br>");
  addSeparator();

  // Configuration
  act = addAction( doc ,  tr("&Configuration"),this,SLOT(configuration()));
  act->setWhatsThis("<b>Configuration</b><br><br>Configuration of the Viewer.<br>");
  addSeparator();

  // Configuration
  act = addAction(saveIcon,tr("Save Configuration"),parent(),SLOT(saveConfig()));
  act->setWhatsThis("<b>Save Configuration</b><br><br> Save the current configuration in the init file<br>");

  // Exit icon
  addSeparator();
  act = addAction( exitIcon , tr("Exit"), parent(), SLOT(close()), Qt::CTRL+Qt::Key_Q );
  act->setWhatsThis("<b>Exit</b><br><br>Quit current application.<br>");

  QObject::connect(__locatToolBar->__Location,SIGNAL(activated(const QString&)),this,SLOT(openFile(const QString&)));
  drawLocationBar(true);

  ViewerSettings settings;
  settings.beginGroup("FileManager");
  setRecentFiles(settings.value("RecentFiles").toStringList());
  __pictureDir = settings.value("PictureRep").toString();
  settings.endGroup();

}

ViewFileManager::~ViewFileManager(){
#ifdef  GEOM_DEBUG
    cout << "File Manager deleted" << endl;
#endif
  ViewerSettings settings;
  settings.beginGroup("FileManager");
  settings.setValue("RecentFiles",getRecentFiles());
  settings.setValue("PictureRep",__pictureDir);
  settings.endGroup();
}

const QStringList& ViewFileManager::getRecentFiles() const
{
    return __lastOpenFiles;
}

void ViewFileManager::setRecentFiles(const QStringList& files)
{
    __lastOpenFiles = files;
    drawRecentFilesMenu();
    drawLocationBar(true);
}


const QString&
ViewFileManager::getLastOpenFiles(int i) const{
  return __lastOpenFiles[i];
}

const QString
ViewFileManager::getLastOpenFile() const {
  if( !__lastOpenFiles.empty()) return (__lastOpenFiles[__lastOpenFiles.size()-1]);
  else  return QString();
}

const bool
ViewFileManager::hasOpenFile() const {
  return  __hasOpenFile;
}

ViewToolBar *
ViewFileManager::getLocationBar()
{
  return __locatToolBar;
}

bool
ViewFileManager::clearReview() {
  __hasOpenFile = false;
  __lastOpenFiles.clear();
  clearTmpFiles();
  drawRecentFilesMenu();
  drawLocationBar();
  return true;
}

void deleteInsideDir(QDir& dir){
  QStringList filenames = dir.entryList();
  uint_t f_count = filenames.count();
  for(uint_t i = 0 ; i < f_count ; i++  ) {
    QString filename = filenames[i];
    if(filename != "." && filename != ".."){
      QFileInfo file(dir.absolutePath() + '/' + filename);
      if(file.isDir()){
        QDir d = dir;
        if(d.cd(filename)){
          deleteInsideDir(d);
          dir.rmdir(filename);
        }
      }
      else dir.remove(filename);
    }
  }
}

bool
ViewFileManager::clearTmpFiles() {
    QString tmp_path;
#ifdef _WIN32
    tmp_path = "C:/TEMP/GeomViewer/";
#else
    tmp_path = "/tmp/GeomViewer/";
#endif
    QDir tmp_p(tmp_path);
    deleteInsideDir(tmp_p);
    return false;
}

QString
ViewFileManager::getSaveFileName( const QString& initial,
                  const QString& ext,
                  const QString& filter,
                  QWidget* parent,
                  const QString& caption){
  QString filename = initial;
  if(!ext.isEmpty()&&!filename.isEmpty()){
    QString extension=QFileInfo(filename).suffix();
    extension= extension.toUpper();
    if (extension != ext.toUpper())
      filename = QFileInfo(filename).completeBaseName()+'.'+ext;
  }

  filename = QFileDialog::getSaveFileName ( parent, caption, QFileInfo(filename).path(), filter+";;All Files (*.*)" );

  if(filename.isEmpty())return QString();
  else {
    if(!ext.isEmpty()){
        QString extension=QFileInfo(filename).suffix();
        extension= extension.toUpper();
        if (extension != ext.toUpper())
            filename = QFileInfo(filename).completeBaseName()+'.'+ext;
    }
    return filename;
  }
}

void ViewFileManager::drawRecentFilesMenu()
{
  QPixmap doc(ViewerIcon::getPixmap(ViewerIcon::document));
  __RecentFilesMenu->clear();
  QPixmap _close(ViewerIcon::getPixmap(ViewerIcon::fileclose));
  QAction * actClearHistory =__RecentFilesMenu->addAction(_close,tr("Clear"),this,SLOT(clearReview()));
  actClearHistory->setWhatsThis("<b>Clear History</b>");
  if(__lastOpenFiles.empty())actClearHistory->setEnabled(false); // __RecentFilesMenu->setItemEnabled(idClear,false);
  __RecentFilesMenu->addSeparator();
  QObject::connect(__RecentFilesMenu,SIGNAL(triggered(QAction *)),this, SLOT(reOpenFile(QAction *)));
  int count = 0;
    for ( int i = __lastOpenFiles.size() - 1 ; i >= 0 && count < 10; i-- ) {
      count++;
      QString display_name=(__lastOpenFiles[i]).right((__lastOpenFiles[i]).length()- (__lastOpenFiles[i]).lastIndexOf('/')-1 );
      QAction * act = __RecentFilesMenu->addAction(doc, display_name); // , this, SLOT( reOpenFile( int ) ) );
      act->setData(QVariant(__lastOpenFiles[i]));
      int accel = __lastOpenFiles.size()-1-i;
      if(accel < 10) act->setShortcut(Qt::CTRL+Qt::Key_0+accel);
    }
}

void ViewFileManager::drawLocationBar(bool b)
{
  QPixmap doc(ViewerIcon::getPixmap(ViewerIcon::document));
  __locatToolBar->__Location->clear();
  if(b)__locatToolBar->__Location->addItem(QIcon(doc),"");
  for ( int i = __lastOpenFiles.size() - 1 ; i >=0 ; i-- ) {
    __locatToolBar->__Location->addItem(QIcon(doc),__lastOpenFiles[i]);
  }
  __locatToolBar->__Location->clearFocus();
}

void ViewFileManager::fillToolBar(QToolBar * __fileTools){
  ViewRendererGL * scene = __GLFrame->getSceneRenderer();
  QPixmap doc(ViewerIcon::getPixmap(ViewerIcon::document));
  QPixmap openIcon(ViewerIcon::getPixmap(ViewerIcon::fileopen) );
  QPixmap refreshIcon( ViewerIcon::getPixmap(ViewerIcon::reload) );
  QPixmap exitIcon( ViewerIcon::getPixmap(ViewerIcon::exit ));
  QPixmap saveIcon( ViewerIcon::getPixmap(ViewerIcon::filefloppy) );
  QPixmap savePicIcon( ViewerIcon::getPixmap(ViewerIcon::camerafile) );
  QPixmap copyPicIcon( ViewerIcon::getPixmap(ViewerIcon::camera) );

  char opentext[] = "<b>Open File</b><br><br>"
    "Load 3D scene from a file.<br><br>"
    "You can also use Menu <br><b>File > Open > Open File</b><br>";
  char savetext[] = "<b>Save File</b><br><br>"
    "Save the current 3D scene in a file.<br><br>"
    "You can also use Menu <br><b>File > Save > Save</b><br>";
  char savepictext[] = "<b>Save ScreenShot</b><br><br>"
    "Take a screenshot and save it on one of the various picture formats.<br><br>"
    "You can also use Menu <br><b>File > ScreenShot > Save as Bitmap</b><br>";
  char copypictext[] = "<b>Copy ScreenShot to Clipboard</b><br><br>"
    "Take a screenshot and copy it on the global clipboard.<br><br>"
    "You can also use Menu <br><b>File > ScreenShot > Copy to Clipboard</b><br>";
  char refreshtext[] = "<b>Refresh</b><br><br>"
    "Reload the current scene.<br><br>"
    "You can also use Menu <br><b>File > Refresh</b><br>";
  char exittext[] = "<b>Exit</b><br><br>"
    "Quit current application.<br><br>"
    "You can also use Menu <br><b>File > Exit</b><br>";
  char proptext[] ="<b>Properties</b><br><br>"
    "Properties of the current scene,<br> selected shapes, file, camera, ... .<br><br>"
    "You can also use Menu <br><b>File > Properties</b><br>";

  QAction * newButton = __fileTools->addAction( QIcon(openIcon),tr("Open File") , this, SLOT(openFile()) );
  newButton->setWhatsThis(tr(opentext));

  newButton = __fileTools->addAction( QIcon(saveIcon),tr("Save File") , scene, SLOT(save()) );
  newButton->setWhatsThis(tr(savetext));

  newButton = __fileTools->addAction( QIcon(savePicIcon),tr("Save Picture") , this, SLOT(saveImageWithAlpha()) );
  newButton->setWhatsThis(tr(savepictext));

  newButton = __fileTools->addAction( QIcon(copyPicIcon),tr("Copy Picture To Clipboard") , __GLFrame, SLOT(copyImageToClipboardWithAlpha()) );
  newButton->setWhatsThis(tr(copypictext));

  newButton = __fileTools->addAction( QIcon(refreshIcon),tr("Refresh") , this, SLOT(refresh()) );
  newButton->setWhatsThis(tr(refreshtext));

  newButton = __fileTools->addAction( QIcon(doc),tr("Properties") , this, SLOT(properties()) );
  newButton->setWhatsThis(tr(proptext));

  QObject * parentW = parentWidget();
  newButton = __fileTools->addAction( QIcon(exitIcon),tr("Exit") , parent(), SLOT(close()) );
  newButton->setWhatsThis(tr(exittext));
}

void ViewFileManager::addOpenFile(const QString& _name){
  if(!_name.isEmpty()){
    __hasOpenFile = true;
    if ( __lastOpenFiles.empty() ||
     _name != __lastOpenFiles[__lastOpenFiles.size()-1]){
      removeOpenFile(_name);
      if(__lastOpenFiles.size()>= REVIEW_SIZE)
            __lastOpenFiles.pop_front();
      __lastOpenFiles.push_back(_name);
    }
    drawRecentFilesMenu();
    drawLocationBar(false);
  }
  else {
    drawLocationBar(true);
  }
}


void ViewFileManager::removeOpenFile(const QString& _name){
  if(!__lastOpenFiles.empty() && !_name.isEmpty()){
    QStringList::iterator _beg = __lastOpenFiles.begin();
    for(; _beg != (__lastOpenFiles.end()) && *_beg != _name ;_beg++);
    if (_beg != (__lastOpenFiles.end())){
      if(_beg == __lastOpenFiles.begin())
        __lastOpenFiles.pop_front();
      else if(_beg == (__lastOpenFiles.end()-1))
        __lastOpenFiles.pop_back();
      else  __lastOpenFiles.erase(_beg);
    }
  }
}

void ViewFileManager::saveImage()
{
    saveImage(false);
}

void ViewFileManager::saveImageWithAlpha()
{
    saveImage(true);
}

void ViewFileManager::saveImage(bool withAlpha)
{
    QList<QByteArray> _formatList=QImageWriter::supportedImageFormats();
    QString first_format=_formatList[0];
    QStringList _formats;
    first_format=first_format.toLower();

    for(uint it=0;it<_formatList.count();it++){
        QString format=_formatList[it];
        if(format == "png"){
            first_format=format.toLower();
            format=format+" "+tr("Files")+" (*."+(format.toLower())+")";
            _formats.prepend(format);
        }
        else {
            format=format+" "+tr("Files")+" (*."+(format.toLower())
                         +(format=="JPEG" ? ",*.jpg)" : ")");
            _formats.append(format);
        }
    }
    /*
    QPixmap icon(ViewerIcon::getPixmap(ViewerIcon::filefloppy));
    QFileDialog _fileDialog(this,"Save Image");
    _fileDialog.setModal(true);
    _fileDialog.setFileMode(QFileDialog::AnyFile);
    _fileDialog.setWindowIcon(QIcon(icon));
    _fileDialog.setFilters(_formats);
    */

    QString ext;
    QString _filename = QFileDialog::getSaveFileName(this, "Save Image", __pictureDir, _formats.join(";;"),& ext);

    /*
    if(!__pictureDir.isEmpty()){
      _fileDialog.setDirectory(__pictureDir);
    }
    if(__hasOpenFile){
    QString filename = getLastOpenFile();
    if(!__pictureDir.isEmpty())
      filename = QFileInfo(filename).fileName();
    int ind_ext=filename.lastIndexOf('.');
    if(ind_ext!=-1)
        filename.replace(ind_ext+1,(filename.length()-ind_ext-1),first_format);
       // _fileDialog.setSelection(filename);
    }
    _fileDialog.setWindowTitle(tr("Save Image"));
    if(_fileDialog.exec()){
    QStringList _filenames=_fileDialog.selectedFiles();
    if(!_filenames.isEmpty()){
        QString _filename = _filenames[0];
    */
    if (!_filename.isEmpty()) {
        __pictureDir = QFileInfo(_filename).dir().absolutePath();
        QString extension=_filename.right(_filename.length()-_filename.lastIndexOf('.')-1);
        if(extension.contains('/')){
            // QString ext = _fileDialog.selectedFilter();
            ext = ext.left(ext.indexOf(' '));
            _filename +=  '.' +ext.toLower();
            extension = ext;
        }
        extension= extension.toUpper();
        /*if(exists(_filename.toStdString())){
          if(QMessageBox::warning(this,tr("File Exists"),_filename + tr(" already exist. Overwrite ?"),
                      tr("Yes"),tr("No"))!=0)
            return;
        }*/
        uint it=0;
        for(; (it<_formatList.count()) && (extension!=(_formatList.at(it))) ; it++ );
        if(it==_formatList.count()){
            //QString ext = _fileDialog.selectedFilter();
            ext = ext.left(ext.indexOf(' '));
            __GLFrame->saveImage(_filename,qPrintable(ext));
        }
        else {
            __GLFrame->saveImage(_filename,qPrintable(extension));
        }
    }
}




void ViewFileManager::openFile()
{
  QString file =  QFileDialog::getOpenFileName ( this, tr("Open"), QFileInfo(getLastOpenFile()).path() ,tr("All Files")+" (*.*)" );
  openFile(file);
}


void ViewFileManager::reOpenFile()
{
    reOpenFile( __lastOpenFiles.size() - 1);
}


void ViewFileManager::reOpenFile( int i )
{
    if(  i >= 0 &&  __lastOpenFiles.size() > (uint)i ){
    QString toOpen=__lastOpenFiles[i];
    openFile(toOpen);
    }
    else{
    QMessageBox::warning(this,tr("File Name Error"),
                QString(tr("File name of index %1 doesn't exist !")).arg(i),1,0,0);
    }
//    drawRecentFilesMenu();
}

void ViewFileManager::reOpenFile( QAction * action )
{
    openFile(action->data().toString());
}


void ViewFileManager::openFile(const QString& file){
  if(!file.isEmpty()){
      if(QFile::exists(file)){
        ViewRendererGL * scene = __GLFrame->getSceneRenderer();
        scene->openFile(file);
      }
      else {
        error("*** Error : File \""+file+"\" does not exist!");
        removeOpenFile(file);
      }
  }
}

/* ---------------------------------------------------------------------------*/
void
ViewFileManager::error(const QString& s)
{
  emit errorMessage(s);
}

void
ViewFileManager::warning(const QString& s)
{
  emit warningMessage(s);
}

void
ViewFileManager::info(const QString& s)
{
  emit infoMessage(s);
}

void
ViewFileManager::status(const QString& s)
{
  emit statusMessage(s);
}

void
ViewFileManager::status(const QString& s,int t)
{
  emit statusMessage(s,t);
}

void
ViewFileManager::connectTo(QStatusBar *s)
{
    if(s){
        QObject::connect(this,SIGNAL(statusMessage(const QString&,int)),
            s,SLOT(showMessage(const QString&,int)) );
        QObject::connect(this,SIGNAL(statusMessage(const QString&)),
            s,SLOT(showMessage(const QString&)) );
    }
}

void
ViewFileManager::connectTo(ViewErrorDialog *e)
{
  if(e){
    QObject::connect(this,SIGNAL(errorMessage(const QString&)),
             e,SLOT(setError(const QString&)) );
    QObject::connect(this,SIGNAL(warningMessage(const QString&)),
             e,SLOT(appendWarning(const QString&)) );
    QObject::connect(this,SIGNAL(infoMessage(const QString&)),
             e,SLOT(appendInfo(const QString&)) );
  }
}
/* ---------------------------------------------------------------------------*/

void
ViewFileManager::properties()
{
  ViewProperties mb(__GLFrame ,this,__controlPanel,false,qPrintable(tr("Properties")),true);
  mb.setWindowIcon(QIcon(QPixmap(ViewerIcon::getPixmap(ViewerIcon::document))));
  if (__propposition != QPoint(-1,-1)) {
    mb.move(__propposition);
  }
  mb.exec();
  __propposition = mb.pos();
}

void
ViewFileManager::configuration()
{
  ViewProperties mb(__GLFrame ,this,__controlPanel,true,qPrintable(tr("Properties")),true);
  mb.setWindowIcon(QIcon(QPixmap(ViewerIcon::getPixmap(ViewerIcon::document))));
  if (__propposition != QPoint(-1,-1)) {
    mb.move(__propposition);
  }
  mb.exec();
  __propposition = mb.pos();
}

void
ViewFileManager::refresh()
{
  __GLFrame->getSceneRenderer()->refresh();
}


/*-----------------------------------------------------------------*/

