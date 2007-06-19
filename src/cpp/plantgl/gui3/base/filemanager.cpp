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


#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/util_enviro.h>
#include "filemanager.h"
#include "icons.h"
#include "properties.h"
#include "glframe.h"

#include <sstream>

#include <time.h>
#include <qtextstream.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qbitmap.h>
#include <qimage.h> 
#include <qfile.h>
#include <qpixmap.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qgl.h>
#include <qtabdialog.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include <qapplication.h>
#include <qwhatsthis.h>
#include <qnetwork.h>
#include <qinputdialog.h>
#include <qstringlist.h>
#include <qobjectlist.h>
#include <qregexp.h>

#include "grid.h"
#include "camera.h"
#include "controlpanel.h"
#include "scenegl.h"
#include "locationbar.h"
#include "helpmenu.h"
#include "errordialog.h"
#include "util_qwidget.h"

#if QT_VERSION < 300
#ifndef QT_NO_NETWORKPROTOCOL_HTTP
#include "http.h"
#endif
#endif

// #include "cropimages.h"

TOOLS_USING_NAMESPACE

/*------------------------------------------------------------------------------------*/


const uint ViewFileManager3::REVIEW_SIZE(50);
QString ViewFileManager3::FILE_CONFIG("geomviewer");

/*------------------------------------------------------------------------------------*/


ViewFileManager3::ViewFileManager3(QMainWindow * parent, 
								 const char * name, 
								 ViewGLFrame3 * frame,
								 ViewHelpMenu3 * helpmenu,
								 ViewControlPanel3 * controlpanel)
    : QPopupMenu( parent, name ),     
      __hasOpenFile(false),
      __GLFrame(frame),
	  __controlPanel(controlpanel),
	  __helpmenu(helpmenu),
      __saveoptions(254)
{

    QObject::connect(__GLFrame,SIGNAL(rendererChanged()),
		     this,SLOT(initialize()));
    __locatToolBar = new ViewLocationBar3(QString("Location ToolBar"),
				     parent,
				     QMainWindow::Top, 
				     TRUE);
  __OpenFileMenu = new QPopupMenu( this );
  __ImportFileMenu = new QPopupMenu( this );
  __SaveFileMenu = new QPopupMenu( this );
  __ExportFileMenu = new QPopupMenu( this );
  __RecentFilesMenu = new QPopupMenu( this );
  initialize();
  
  connect( &__url, SIGNAL( data( const QByteArray &, QNetworkOperation * ) ),
     this, SLOT( downloaded( const QByteArray &, QNetworkOperation * ) ) );
  connect( &__url, SIGNAL( connectionStateChanged( int , const QString &  ) ),
     this, SLOT( connectionStatus ( int , const QString &  ) ) );
  connect( &__url, SIGNAL( finished( QNetworkOperation * ) ),
     this, SLOT( net( QNetworkOperation *  ) ) );
  connect( &__url, SIGNAL( dataTransferProgress ( int, int, QNetworkOperation * ) ),
	  this, SLOT( transferProgress ( int, int, QNetworkOperation * ) ) );

   // if(QNetworkProtocol::hasOnlyLocalFileSystem())
#if QT_VERSION < 300
    qInitNetworkProtocols();
    QNetworkProtocol::registerNetworkProtocol( "http", new QNetworkProtocolFactory<MyHttp> );
#endif
//    loadConfig();
}

void
ViewFileManager3::initialize(){
  clear();
  ViewRendererGL3 * scene = __GLFrame->getSceneRenderer();
  QObject::connect(scene,SIGNAL(filenameChanged(const QString&)),
		   this,SLOT(addOpenFile(const QString&)));
  
  // *  The popup menu *
  
  QPixmap doc(ViewerIcon3::getPixmap(ViewerIcon3::icon_document));
  QPixmap openIcon(ViewerIcon3::getPixmap(ViewerIcon3::icon_fileopen) );
  QPixmap refreshIcon( ViewerIcon3::getPixmap(ViewerIcon3::reload) );
  QPixmap exitIcon( ViewerIcon3::getPixmap(ViewerIcon3::icon_exit) );
  QPixmap saveIcon( ViewerIcon3::getPixmap(ViewerIcon3::icon_filefloppy) );
  QPixmap savePicIcon( ViewerIcon3::getPixmap(ViewerIcon3::icon_camerafile) );
  QPixmap copyPicIcon( ViewerIcon3::getPixmap(ViewerIcon3::icon_camera) );
  QPixmap printIcon(ViewerIcon3::getPixmap(ViewerIcon3::icon_fileprint) );
  
  
  // Open File Menu
  __OpenFileMenu->clear();
  
  int id = insertItem(QIconSet(openIcon), tr("Open") , 
	     __OpenFileMenu );
  setWhatsThis(id,"<b>Open Menu</b><br>Menu related to file loading<br>");
  id = __OpenFileMenu->insertItem( openIcon , tr("&Open File"),
			      this,SLOT(openFile()),CTRL+Key_O);
  __OpenFileMenu->setWhatsThis(id,"<b>Open File</b><br><br>Load 3D scene from a file.<br>");
  
  scene->addOpenEntries(__OpenFileMenu);
  insertSeparator();
  
  // Import File Menu
  __ImportFileMenu->clear();
  
  if(scene->addImportEntries(__ImportFileMenu)){
    id = insertItem(QIconSet(openIcon), tr("Import") , 
	       __ImportFileMenu );
	setWhatsThis(id,"<b>Import Menu</b><br>Importing scene from various file format<br>");
    insertSeparator();
  }
  
  // Save File Menu
  __SaveFileMenu->clear();
  id = insertItem(QIconSet(openIcon), tr("Save") , __SaveFileMenu );
  setWhatsThis(id,"<b>Save Menu</b><br>Menu related to file saving<br>");
 
  id = __SaveFileMenu->insertItem( saveIcon , tr("&Save"),
			      scene,SLOT(save()),CTRL+Key_S);
   __SaveFileMenu->setWhatsThis(id,"<b>Save</b><br>Save current scene.<br>");
  id = __SaveFileMenu->insertItem( saveIcon , tr("&Save As"),
			      scene,SLOT(saveAs()));
   __SaveFileMenu->setWhatsThis(id,"<b>Save</b><br>Save current scene in a new file.<br>");
  scene->addSaveEntries(__SaveFileMenu);
  
  insertSeparator();
  __ExportFileMenu->clear();
  
  if(scene->addExportEntries(__ExportFileMenu)){
    insertItem(QIconSet(openIcon), tr("Export") , 
	       __ExportFileMenu );
    insertSeparator();
  }
  
  QPopupMenu * __screenshot = new QPopupMenu(this,"Screenshot");
  id = __screenshot->insertItem( savePicIcon , tr("Save as Bitmap"),
			      this,SLOT(saveImage()),CTRL+Key_B);
  __screenshot->setWhatsThis(id,"<b>Save ScreenShot as Bitmap</b><br><br>Take a screenshot and save it on one of the various picture formats.<br>");

  id = __screenshot->insertItem( savePicIcon , tr("Save as Bitmap with Alpha"),
			      this,SLOT(saveImageWithAlpha()),CTRL+SHIFT+Key_B);
  __screenshot->setWhatsThis(id,"<b>Save ScreenShot as Bitmap</b><br><br>Take a screenshot and save it on one of the various picture formats.<br>");

  id = __screenshot->insertItem( copyPicIcon , tr("Copy To Clipboard"),
			      __GLFrame,SLOT(copyImageToClipboard()),CTRL+Key_C);
  __screenshot->setWhatsThis(id,"<b>Copy ScreenShot to Clipboard</b><br><br>Take a screenshot and copy it on the global clipboard.<br>");

  id = __screenshot->insertItem( copyPicIcon , tr("Copy To Clipboard with Alpha"),
			      __GLFrame,SLOT(copyImageToClipboardWithAlpha()),CTRL+SHIFT+Key_C);
  __screenshot->setWhatsThis(id,"<b>Copy ScreenShot to Clipboard</b><br><br>Take a screenshot and copy it on the global clipboard.<br>");

  id = insertItem(QIconSet(copyPicIcon), tr("ScreenShot") , __screenshot );
  setWhatsThis(id,"<b>ScreenShot Menu</b><br>Menu related to screenshot<br>");
  insertSeparator();

  // Recent Files Menu
  id = insertItem(QIconSet(openIcon), tr("Recents") , 
	     __RecentFilesMenu );
  setWhatsThis(id,"<b>History Menu</b><br>This Menu contains all the file names previously opened<br>");
  
  
  drawRecentFilesMenu();
  insertSeparator();
  
  // Refresh  icon
  id = insertItem( printIcon ,  tr("&Print..."),
			  __GLFrame,SLOT(printImage()));
  setWhatsThis(id,"<b>Print</b><br>Take a screenshot and print it.<br>");
  insertSeparator();

  // Refresh  icon
  id = insertItem( refreshIcon ,  tr("&Refresh"),
				   this,SLOT(refresh()),Key_F5);
  setWhatsThis(id,"<b>Refresh</b><br><br>Reload the current scene.<br>");
  insertSeparator();
  
  // Close  icon
  QPixmap _close(ViewerIcon3::getPixmap(ViewerIcon3::icon_fileclose));
  id = insertItem( _close,tr("&Close"),scene,SLOT(close()));
  setWhatsThis(id,"<b>Close</b><br><br>Close the current scene.<br>");
  insertSeparator();
  
  // Properties icon
  id = insertItem( doc ,  tr("Properties"),
	      this,SLOT(properties()),CTRL+Key_P);
  setWhatsThis(id,"<b>Properties</b><br><br>Properties of the current scene,<br> selected shapes, file, camera, ... .<br>");
  insertSeparator();
  id = insertItem( doc ,  tr("&Configuration"),
	      this,SLOT(configuration()));
  setWhatsThis(id,"<b>Configuration</b><br><br>Configuration of the Viewer.<br>");
  insertSeparator();
  // Configuration
  id = insertItem(saveIcon,tr("Save Configuration"),this,SLOT(saveConfig()));
  setWhatsThis(id,"<b>Save Configuration</b><br><br> Save the current configuration in the init file<br>");
  // Exit icon
  QObject * parentW = parentWidget();
  QMainWindow * a = dynamic_cast<QMainWindow *>(parentW);
  if(a){
	insertSeparator();
	id = insertItem( exitIcon , tr("Exit"), a, SLOT(bye()), CTRL+Key_Q );
	setWhatsThis(id,"<b>Exit</b><br><br>Quit current application.<br>");
  }
  
  QObject::connect(__locatToolBar->__Location,SIGNAL(activated(const QString&)),this,SLOT(openFile(const QString&)));
  drawLocationBar(true);

}

ViewFileManager3::~ViewFileManager3(){
#ifdef  GEOM_DEBUG
    cout << "File Manager deleted" << endl;
#endif
}


const QString& 
ViewFileManager3::getLastOpenFiles(int i) const{
  return __lastOpenFiles[i];
}

const QString 
ViewFileManager3::getLastOpenFile() const {
  if( !__lastOpenFiles.empty()) return (__lastOpenFiles[__lastOpenFiles.size()-1]);
  else  return QString::null;
}

const bool 
ViewFileManager3::hasOpenFile() const {
  return  __hasOpenFile;
}

ViewToolBar3 * 
ViewFileManager3::getLocationBar()
{
  return __locatToolBar;
}

bool 
ViewFileManager3::clearReview() {
  __hasOpenFile = false;
  __lastOpenFiles.clear();
  clearTmpFiles();
  drawRecentFilesMenu();
  drawLocationBar();
  return true;
}

void deleteInsideDir(QDir& dir){
  QStringList filenames = dir.entryList();
  uint32_t f_count = filenames.count();
  for(uint32_t i = 0 ; i < f_count ; i++  ) {
	QString filename = filenames[i];
	if(filename != "." && filename != ".."){
	  QFileInfo file(dir.absPath() + '/' + filename);
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
ViewFileManager3::clearTmpFiles() {
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
ViewFileManager3::getSaveFileName( const QString& initial, 
			      const QString& ext, 
			      const QString& filter,
			      QWidget* parent,
			      const QString& caption){
  QString filename = initial;
  if(!ext.isEmpty()&&!filename.isEmpty()){
    QString extension=filename.right(filename.length()-filename.findRev('.')-1);
    extension= extension.upper();    
    if (extension != ext.upper()) {
      filename = filename.left(filename.findRev('.')+1);
      filename += ext;
    }
  }

  filename = QFileDialog::getSaveFileName ( filename,
											filter,
											parent,
											"Save",
											caption);
  
  if(filename.isEmpty())return QString::null;
  else {
    if(!ext.isEmpty()){
      QString extension=filename.right(filename.length()-filename.findRev('.')-1);
      extension= extension.upper();    
      if (extension != ext.upper()) {
	filename = filename.left(filename.findRev('.')+1);
	filename += ext;
      }
    }
    if(QFile::exists(filename)){
      if(QMessageBox::warning(parent,tr("File Exists"),filename + tr(" already exists. Overwrite ?"),
			      tr("Yes"),tr("No"))!=0)
	return QString::null;
    }
    return filename;
  }
}

void ViewFileManager3::drawRecentFilesMenu()
{
  QPixmap doc(ViewerIcon3::getPixmap(ViewerIcon3::icon_document));
  __RecentFilesMenu->clear();
  int idClear;
  QPixmap _close(ViewerIcon3::getPixmap(ViewerIcon3::icon_fileclose));
  idClear =__RecentFilesMenu->insertItem(_close,tr("Clear"),this,SLOT(clearReview()));
  __RecentFilesMenu->setWhatsThis(idClear ,"<b>Clear History</b>");
  if(__lastOpenFiles.empty())__RecentFilesMenu->setItemEnabled(idClear,false);
  __RecentFilesMenu->insertSeparator();
#if QT_VERSION >=211
  int count = 0;
    for ( int i = __lastOpenFiles.size() - 1 ; i >= 0 && count < 10; i-- ) {
	  count++;	
      QString display_name=(__lastOpenFiles[i]).right((__lastOpenFiles[i]).length()- (__lastOpenFiles[i]).findRev('/')-1 );
      int id = __RecentFilesMenu->insertItem(doc, display_name,
					    this, SLOT( reOpenFile( int ) ) );
      int accel = __lastOpenFiles.size()-1-i;
      if(accel < 10) __RecentFilesMenu->setAccel(CTRL+Key_0+accel,id);
      __RecentFilesMenu->setItemParameter( id,  i  );
    }
#else
    int i=__lastOpenFiles.size();
    if( i > 0 ){
      QString display_name=
	(__lastOpenFiles[i - 1]).right((__lastOpenFiles[ i - 1]).length()
				       -(__lastOpenFiles[ i - 1]).findRev('/')-1);
      __RecentFilesMenu->insertItem(doc, display_name,this, SLOT( reOpenFile( ) ) );
    }
#endif
}

void ViewFileManager3::drawLocationBar(bool b)
{
  QPixmap doc(ViewerIcon3::getPixmap(ViewerIcon3::icon_document));
  __locatToolBar->__Location->clear();
  if(b)__locatToolBar->__Location->insertItem(doc,"");
  for ( int i = __lastOpenFiles.size() - 1 ; i >=0 ; i-- ) {
    __locatToolBar->__Location->insertItem(doc,__lastOpenFiles[i]);
  }
  __locatToolBar->__Location->clearFocus();
}

void ViewFileManager3::fillToolBar(QToolBar * __fileTools){
  ViewRendererGL3 * scene = __GLFrame->getSceneRenderer();
  QPixmap doc(ViewerIcon3::getPixmap(ViewerIcon3::icon_document));
  QPixmap openIcon(ViewerIcon3::getPixmap(ViewerIcon3::icon_fileopen) );
  QPixmap refreshIcon( ViewerIcon3::getPixmap(ViewerIcon3::reload) );
  QPixmap exitIcon( ViewerIcon3::getPixmap(ViewerIcon3::icon_exit ));
  QPixmap saveIcon( ViewerIcon3::getPixmap(ViewerIcon3::icon_filefloppy) );
  QPixmap savePicIcon( ViewerIcon3::getPixmap(ViewerIcon3::icon_camerafile) );
  QPixmap copyPicIcon( ViewerIcon3::getPixmap(ViewerIcon3::icon_camera) );

  QString opentext = "<b>Open File</b><br><br>"
	"Load 3D scene from a file.<br><br>"
	"You can also use Menu <br><b>File > Open > Open File</b><br>";
  QString savetext = "<b>Save File</b><br><br>"
	"Save the current 3D scene in a file.<br><br>"
	"You can also use Menu <br><b>File > Save > Save</b><br>";
  QString savepictext = "<b>Save ScreenShot</b><br><br>"
	"Take a screenshot and save it on one of the various picture formats.<br><br>"
	"You can also use Menu <br><b>File > ScreenShot > Save as Bitmap</b><br>";
  QString copypictext = "<b>Copy ScreenShot to Clipboard</b><br><br>"
	"Take a screenshot and copy it on the global clipboard.<br><br>"
	"You can also use Menu <br><b>File > ScreenShot > Copy to Clipboard</b><br>";
  QString refreshtext = "<b>Refresh</b><br><br>"
	"Reload the current scene.<br><br>"
	"You can also use Menu <br><b>File > Refresh</b><br>";
  QString exittext = "<b>Exit</b><br><br>"
	"Quit current application.<br><br>"
	"You can also use Menu <br><b>File > Exit</b><br>";
  QString proptext ="<b>Properties</b><br><br>"
	"Properties of the current scene,<br> selected shapes, file, camera, ... .<br><br>"
	"You can also use Menu <br><b>File > Properties</b><br>";
  QToolButton * newButton 
    = new QToolButton( openIcon,tr("Open File") , tr("Open File"),
		       this, SLOT(openFile()), __fileTools,"Open File" );
  QWhatsThis::add(newButton,tr(opentext));
  
  newButton = new QToolButton( saveIcon,tr("Save File") , tr("Save File"), 
			       scene, SLOT(save()), __fileTools, "Save File" );
  QWhatsThis::add(newButton,tr(savetext));
  
  newButton = new QToolButton( savePicIcon,tr("Save Picture") , tr("Save Picture"), 
			       this, SLOT(saveImage()), __fileTools, "Save Picture" );
  QWhatsThis::add(newButton,tr(savepictext));
  
  newButton = new QToolButton( copyPicIcon,tr("Copy Picture To Clipboard") , tr("Copy Picture To Clipboard"), 
			       __GLFrame, SLOT(copyImageToClipboard()), __fileTools,"Copy To Clipboard" );
  QWhatsThis::add(newButton,tr(copypictext));
  
  newButton = new QToolButton( refreshIcon, tr("Refresh"), tr("Refresh"),
			       this, SLOT(refresh()), __fileTools, "Refresh" );
  QWhatsThis::add(newButton,tr(refreshtext));

  newButton = new QToolButton( doc, tr("Properties"), tr("Properties"),
			       this, SLOT(properties()), __fileTools, tr("Properties") );
  QWhatsThis::add(newButton,tr(proptext));
    
  QObject * parentW = parentWidget();
  newButton = new QToolButton( exitIcon, tr("Exit"), tr("Exit"),
			       parentW, SLOT(bye()), __fileTools, "Exit" );
  QWhatsThis::add(newButton,tr(exittext));
}

void ViewFileManager3::addOpenFile(const QString& _name){
  if(!_name.isEmpty()){
	QString name = _name;
	QString tmp_path;
#ifdef _WIN32
	tmp_path = "C:/TEMP/GeomViewer/";
#else
	tmp_path = "/tmp/GeomViewer/";
#endif
	if(name.find(tmp_path)==0){
	  name = name.right(name.length()-tmp_path.length());
	  name.replace(name.find('/'),1,"://");
	}
    __hasOpenFile = true;
    if ( __lastOpenFiles.empty() || 
	 name != __lastOpenFiles[__lastOpenFiles.size()-1]){
      removeOpenFile(name);
      if(__lastOpenFiles.size()>= REVIEW_SIZE)
	__lastOpenFiles.pop_front();
      __lastOpenFiles.push_back(name);
    }
    drawRecentFilesMenu();
    drawLocationBar(false);    
  }
  else {
    drawLocationBar(true);    
  }
}


void ViewFileManager3::removeOpenFile(const QString& _name){
  if(!__lastOpenFiles.empty() && !_name.isEmpty()){
	QString name = _name;
	QString tmp_path;
#ifdef _WIN32
	tmp_path = "C:/TEMP/GeomViewer/";
#else
	tmp_path = "/tmp/GeomViewer/";
#endif
	if(name.find(tmp_path)==0){
	  name = name.right(name.length()-tmp_path.length());
	  name.replace(name.find('/'),1,"://");
	}
    std::deque<QString>::iterator _beg = __lastOpenFiles.begin();
    for(; _beg != (__lastOpenFiles.end()) && *_beg != name ;_beg++);
    if (_beg != (__lastOpenFiles.end()))
      if(_beg == __lastOpenFiles.begin())
	__lastOpenFiles.pop_front();
      else if(_beg == __lastOpenFiles.end()-1)
	__lastOpenFiles.pop_back();
      else  __lastOpenFiles.erase(_beg);
  }
}

void ViewFileManager3::saveImage()
{
	saveImage(false);
}

void ViewFileManager3::saveImageWithAlpha()
{
	saveImage(true);
}

void ViewFileManager3::saveImage(bool withAlpha)
{
    QStrList _formatList=QImageIO::outputFormats();
    QString first_format=_formatList.at(0);
    QStringList _formats(first_format+" "+tr("Files")+" (*."+(first_format.lower())+")");
    first_format=first_format.lower();

    for(uint it=1;it<_formatList.count();it++){
		QString format=_formatList.at(it);
		if(format == "PNG"){
			first_format=format.lower();
			format=format+" "+tr("Files")+" (*."+(format.lower())+")";
			_formats.prepend(format);
		}
		else {
			format=format+" "+tr("Files")+" (*."+(format.lower())
						 +(format=="JPEG" ? ",*.jpg)" : ")");
			_formats.append(format);
		}
    }

    QPixmap icon(ViewerIcon3::getPixmap(ViewerIcon3::icon_filefloppy));
    QFileDialog _fileDialog(this,"Save Image",true);
	_fileDialog.setMode(QFileDialog::AnyFile);
    _fileDialog.setIcon(icon);
    _fileDialog.setFilters(_formats);

	if(!__pictureDir.isEmpty()){
	  _fileDialog.setDir(__pictureDir);
	}
    if(__hasOpenFile){
	QString filename = getLastOpenFile();
	if(!__pictureDir.isEmpty())
	  filename = QFileInfo(filename).fileName();
	int ind_ext=filename.findRev('.');
	if(ind_ext!=-1)
	    filename.replace(ind_ext+1,(filename.length()-ind_ext-1),first_format);
	_fileDialog.setSelection(filename);
    }
    _fileDialog.setCaption(tr("Save Image"));  
    if(_fileDialog.exec()){
	QString _filename=_fileDialog.selectedFile();
	if(!_filename.isEmpty()){
		__pictureDir = QFileInfo(_filename).dirPath(true);
	    QString extension=_filename.right(_filename.length()-_filename.findRev('.')-1);
	    if(extension.contains('/')){
		QString ext = _fileDialog.selectedFilter();
		ext = ext.left(ext.find(' '));
		_filename +=  '.' +ext.lower();
		extension = ext;
	    }
	    extension= extension.upper();
		if(TOOLS(exists)(std::string(_filename.data()))){
	      if(QMessageBox::warning(this,tr("File Exists"),_filename + tr(" already exist. Overwrite ?"),
				      tr("Yes"),tr("No"))!=0)
		return;
	    }	    
	    uint it=0;
	    for(; (it<_formatList.count()) && (extension!=(_formatList.at(it))) ; it++ );
	    if(it==_formatList.count()){
		QString ext = _fileDialog.selectedFilter();
		ext = ext.left(ext.find(' '));
		__GLFrame->saveImage(_filename,ext,withAlpha);
	    }
	    else {
		__GLFrame->saveImage(_filename,extension,withAlpha);  
	    }
	}
    }
}




void ViewFileManager3::openFile()
{
  QString file =  QFileDialog::getOpenFileName ( getLastOpenFile(),tr("All Files")+" (*.*)" ,
				    this, tr("Open"), tr("Open") );
  openFile(file);
}
 
 
void ViewFileManager3::reOpenFile()
{
    reOpenFile( __lastOpenFiles.size() - 1);
}


void ViewFileManager3::reOpenFile( int i )
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

void ViewFileManager3::openFile(const QString& file){
  if(!file.isEmpty()){
	QUrl url = file;
	if(url.isLocalFile()){
	  if(QFile::exists(file)){
		ViewRendererGL3 * scene = __GLFrame->getSceneRenderer();
		scene->openFile(file);
	  }
	  else {
		error("*** Error : File \""+file+"\" does not exist!");
		removeOpenFile(file);
	  }

	}
	else {
	  __url = file;
	  if(__url.protocol() =="ftp"){
		bool ok = FALSE;
		QString text = QInputDialog::getText( tr( "Login" ),
						      tr( "Login :" ),
#if QT_VERSION > 300
						      QLineEdit::Normal,
#endif
						      QString::null, 
						      &ok,this );
		if ( ok && !text.isEmpty() ){
		  __url.setUser(text);
		  QString text = QInputDialog::getText( tr( "Password" ),
							tr( "Password :" ),
#if QT_VERSION > 300
							QLineEdit::Normal,
#endif
							QString::null, 
							&ok, this );
		if ( ok && !text.isEmpty() )
		  __url.setPassword(text);

		}		
	  }
	  __url.get();
	}
  }
}

/* ---------------------------------------------------------------------------*/

void ViewFileManager3::downloaded( const QByteArray & ba,
								 QNetworkOperation * op ){
#if QT_VERSION < 300
  if(__url.protocol() == "http")
  status("File Packet loaded ("+QString::number(ba.size())+" octet(s)).",10000);
#endif
  if(__downloadingfile.isEmpty()){
	QString tmp_path;
	QString fname =__url.fileName();
	if(fname.isEmpty())fname = "index";
	QString udir = __url.dirPath();
#ifdef _WIN32
	tmp_path = "C:/TEMP/";
#else
	tmp_path = "/tmp/";
#endif
	QDir tmp_p(tmp_path);
	if(!tmp_p.exists("GeomViewer"))tmp_p.mkdir("GeomViewer");
	tmp_p.cd("GeomViewer");
	if(!tmp_p.exists(__url.protocol()))tmp_p.mkdir(__url.protocol());
	tmp_p.cd(__url.protocol());
	if(!tmp_p.exists(__url.host()))tmp_p.mkdir(__url.host());
	tmp_p.cd(__url.host());
	QStringList dirs = QStringList::split( "/", udir);
	uint first = 0;
	if(dirs[0].contains(QRegExp("[A-Z]:"))==1){
	  first = 1;
	}
	for(uint i = first; i < dirs.count(); i++){
	  if(!tmp_p.exists(dirs[i])){
		tmp_p.mkdir(dirs[i]);
	  }
	  else if(QFileInfo(tmp_p.path()+'/'+dirs[i]).isFile()){
		QFile::remove(tmp_p.path()+'/'+dirs[i]);
		tmp_p.mkdir(dirs[i]);
	  }
	  if(!tmp_p.cd(dirs[i])){
		qWarning("Cannot go from \""+tmp_p.path()+"\" to \""+dirs[i]+"\".");
	  }
	}
	if(QDir(tmp_p.path()+'/'+fname).exists()){
	  tmp_p.cd(fname);
	  fname = "index";
	}
	
	if(__downloadingfile.isEmpty() && QFile::exists(tmp_p.path()+'/'+fname)){
	  QFile::remove(tmp_p.path()+'/'+fname);
	}
	__downloadingfile = tmp_p.path()+'/'+fname;
  }

  QFile f(__downloadingfile);
  int mode = IO_WriteOnly;
  if(f.exists()){
	mode |= IO_Append;
  }
  if(f.open(mode)){
	f.writeBlock (ba);
	f.close();
  }
  else 
	qWarning(tr("Cannot open temporary file")+" \""+f.name()+"\".");
}

void 
ViewFileManager3::connectionStatus ( int i, const QString & msg )
{
  status(msg,10000);
}

void 
ViewFileManager3::net( QNetworkOperation * op )
{
  if(op->state() == QNetworkProtocol::StFailed){
	QMessageBox::warning(this,tr("File Download Failed"),
		op->protocolDetail());
	qWarning(tr("Network Error : ")+op->protocolDetail());
	status(tr("Network Error : ")+op->protocolDetail(),10000);
  }
  else if(op->state() == QNetworkProtocol::StStopped){
	QString detail = op->protocolDetail();
	if(!detail.isEmpty())detail = " : "+detail;
	qWarning("Transfert Stopped"+detail);
	status("Transfert Stopped"+detail,10000);
  }
//  else if(op->state() == QNetworkProtocol::StWaiting){
//	status("Waiting ... ",10000);
//  }
  else if(op->state() == QNetworkProtocol::StDone || 
		  op->state() == QNetworkProtocol::StWaiting){
	QString file = __downloadingfile;
	__downloadingfile = QString::null;
	QFile f(file);
	QString text;
	if(f.open(IO_ReadOnly)){
	  {
		QTextStream st(&f);
		text = st.read();
		f.close();
		if(text.contains(QRegExp("<[Hh][Tt][Mm][Ll]>"))){
		  error(text);
		  removeOpenFile(file);
		  return;
		}
	  }
	  ViewRendererGL3 * scene = __GLFrame->getSceneRenderer();
	  scene->open(file);
	}
	else 
	  qWarning(tr("Temporary File")+" \""+file+"\" "+tr("does not exist."));
  }
}

void
ViewFileManager3::transferProgress ( int i, int j, QNetworkOperation * ) 
{
  status(tr("Transfert progress")+" :"+QString::number((100*i)/j)+"%.",5000);
}


/* ---------------------------------------------------------------------------*/
void 
ViewFileManager3::error(const QString& s)
{
  emit errorMessage(s);
}

void 
ViewFileManager3::warning(const QString& s)
{
  emit warningMessage(s);
}

void 
ViewFileManager3::info(const QString& s)
{
  emit infoMessage(s);
}

void 
ViewFileManager3::status(const QString& s)
{
  emit statusMessage(s);
}

void 
ViewFileManager3::status(const QString& s,int t)
{
  emit statusMessage(s,t);
}

void 
ViewFileManager3::connectTo(QStatusBar *s)
{
	if(s){
		QObject::connect(this,SIGNAL(statusMessage(const QString&,int)),
			s,SLOT(message(const QString&,int)) );  
		QObject::connect(this,SIGNAL(statusMessage(const QString&)),
			s,SLOT(message(const QString&)) );  
	}
}

void 
ViewFileManager3::connectTo(ViewErrorDialog3 *e)
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
ViewFileManager3::properties()
{ 
  ViewProperties3 mb(__GLFrame ,this,__controlPanel,false,tr("Properties"),true);
  mb.setIcon(QPixmap(ViewerIcon3::getPixmap(ViewerIcon3::icon_document)));
  mb.exec();
}

void 
ViewFileManager3::configuration()
{ 
  ViewProperties3 mb(__GLFrame ,this,__controlPanel,true,tr("Properties"),true);
  mb.setIcon(QPixmap(ViewerIcon3::getPixmap(ViewerIcon3::icon_document)));
  mb.exec();
}

void
ViewFileManager3::refresh()
{
  __GLFrame->getSceneRenderer()->refresh();
  // if(__hasOpenFile)
  // else openFile();
}

void
ViewFileManager3::saveToolbarState(bool b){ saveOption(0,b); }

void
ViewFileManager3::saveWinPos(bool b){ saveOption(1,b); }

void
ViewFileManager3::saveStyle(bool b){ saveOption(2,b); }

void
ViewFileManager3::saveHistory(bool b){ saveOption(3,b); }

void
ViewFileManager3::saveBgColor(bool b){ saveOption(4,b); }

void
ViewFileManager3::saveGridVisibility(bool b){ saveOption(5,b); }

void
ViewFileManager3::saveCameraReDim(bool b){ saveOption(6,b); }

void
ViewFileManager3::saveUseSpinBox(bool b){ saveOption(7,b); }

bool 
ViewFileManager3::getToolbarStateSave() const 
{ return getSaveOption(0);}

bool 
ViewFileManager3::getSaveWinPos() const 
{ return getSaveOption(1);}

bool 
ViewFileManager3::getSaveStyle() const 
{ return getSaveOption(2);}

bool 
ViewFileManager3::getSaveHistory() const 
{ return getSaveOption(3);}

bool 
ViewFileManager3::getSaveBgColor() const 
{ return getSaveOption(4);}

bool 
ViewFileManager3::getSaveGridVisibility() const 
{ return getSaveOption(5);}

bool 
ViewFileManager3::getSaveCameraReDim() const 
{ return getSaveOption(6);}

bool 
ViewFileManager3::getSaveUseSpinBox() const 
{ return getSaveOption(7);}

bool 
ViewFileManager3::getSaveOption(int i) const 
{ return ((__saveoptions & (1 << i)) != 0); }

void
ViewFileManager3::saveOption(int i, bool b){
  if(b)__saveoptions |= (1 << i);
  else if(getSaveOption(i))__saveoptions ^= (1 << i);
}

void
ViewFileManager3::showInit(bool b){ saveOption(8,b); }

bool
ViewFileManager3::getShowInit() const
{ return getSaveOption(8);}

/* ---------------------------------------------------------------------------*/

float ViewFileManager3::CONFIG_VERSION(1.16f);

QString 
ViewFileManager3::getInitFilename(){
  QString filename=FILE_CONFIG;
#ifndef _WIN32
  QDir _dir(QDir::homeDirPath()+QString("/.kde/share/config/"));
  filename+="rc";

  if(!_dir.exists()){ 
     _dir=QDir::homeDirPath();
     filename="."+filename;
  }
  filename=_dir.absPath()+"/"+filename;
#else
  filename+=".ini";
  filename=QString(TOOLS(getHome().c_str()))+"\\"+filename;
#endif
  return filename;
}

void ViewFileManager3::loadConfig()
{
  QString filename = getInitFilename();
  //read init file
  QFile _fileInit(filename);
  if(_fileInit.exists()){
	if(_fileInit.open( IO_ReadOnly )) {
	  int options;
	  int w = 0, h = 0;
	  int x = 0, y = 0;
	  int  r,g,b;
	  int tabGridtemp[4];
	  int gridSize;
	  int camReDim = 1;
	  int useSpinBox = 1;
	  int style;
	  
	  int numToolBar = 0;
	  QString * lname;
	  int * lindex;
	  int * lnl;
	  int * lextraOffset;
	  int * ldock;
	  int * lv;
	  
	  QTextStream f_initFile( & _fileInit );
	  float version;
	  f_initFile >> version;
	  if(version != CONFIG_VERSION){
		QString _mess = tr("Incompatible init file version")+" !";
		_mess += "<br><b>"+tr("File")+" :</b>";
        _mess += filename;
        _mess += "<br><b>"+tr("Version")+" :</b>";
        _mess += QString::number(version);
        _mess += "<br><b>"+tr("Current Version")+" :</b>";
        _mess += QString::number(CONFIG_VERSION);
		//		QMessageBox::warning(this,"Init File Error",
		//		  _mess,"Ignore",0,0);
		qWarning(_mess);
		_fileInit.close();
		__helpmenu->showInit();
		return;
	  }
	  else 
		qWarning(tr("Init file")+" : \""+filename+"\" - "+tr("Version")+" : "+QString::number(version));
	  f_initFile >> options;
	  __saveoptions = options;
	  if(getShowInit())__helpmenu->showInit();
	  if(getSaveGridVisibility())
	  f_initFile >> tabGridtemp[0]  >> tabGridtemp[1]  
		>> tabGridtemp[2]  >> tabGridtemp[3] >> gridSize;
	  if(getSaveBgColor())  f_initFile >> r  >> g  >> b ;
	  if(getSaveStyle())	f_initFile >> style;
	  if(getSaveWinPos())	f_initFile >> x >> y >> w >> h;
	  if(getSaveCameraReDim())	f_initFile >> camReDim;
	  if(getSaveUseSpinBox())	f_initFile >> useSpinBox;
	  if(getToolbarStateSave()){
		f_initFile >> numToolBar;
		if(numToolBar > 0){
		  lname = new QString[numToolBar];
		  lindex = new int[numToolBar];
		  lnl = new int[numToolBar];
		  lextraOffset = new int[numToolBar];
		  ldock = new int[numToolBar];
		  lv = new int[numToolBar];
		  QChar cchar;
		  for(int i = 0 ; i < numToolBar ; i++){
			do { f_initFile >> cchar; }while(cchar != '"');
			do { f_initFile >> cchar;
			if(cchar != '"')lname[i] +=cchar;
			}while(cchar != '"');
			f_initFile >> ldock[i] >> lindex[i];
			f_initFile >> lnl[i] >> lextraOffset[i] >> lv[i] ;
		  }
		}
	  }
	  
	  if(getSaveHistory()){
		while (!f_initFile.eof()){
		  QString  lastOpenFile;
		  QChar car = ' ';
		  while(!_fileInit.atEnd() && car!='"' ){
			QString namepart;
			f_initFile >>  namepart;
			if(!lastOpenFile.isEmpty())lastOpenFile += " ";
			lastOpenFile += namepart;
			car = lastOpenFile.at(lastOpenFile.length()-1);
		  }
		  lastOpenFile.remove(lastOpenFile.length()-1,1);
		  lastOpenFile.remove(0,1);
		  if(! lastOpenFile.isEmpty() ){
			__lastOpenFiles.push_back(lastOpenFile);
		  }
		}
	  }
	  _fileInit.close();
	  drawRecentFilesMenu();
	  drawLocationBar(true);
	  if(getSaveBgColor()){
		if(__GLFrame){
		  __GLFrame->setBackGroundColor(QColor(r,g,b));
		  qDebug(QString("FrameGL.setBackgroundColor(&lt;%1,%1,%1&gt;)").arg(r).arg(g).arg(b));
		}
	  }
	  if(getSaveGridVisibility()){
		bool tabGrid[4];
		for(int i=0;i<4;i++)
		  tabGrid[i]=(tabGridtemp[i]==1);
		if(__GLFrame){
		  ViewGridGL3 * grid = __GLFrame->getGrid();
		  if(grid){
			grid->showXYGrid(tabGrid[0]);
			grid->showYZGrid(tabGrid[1]);
			grid->showXZGrid(tabGrid[2]);
			grid->showAxis(tabGrid[3]);
			grid->setGridSize(gridSize);
			qDebug(QString("Axis.show(%1) - XYGrid.show(%1)  - XZGrid.show(%1) - YZGrid.show(%1)")
			  .arg(tabGrid[3]).arg(tabGrid[0]).arg(tabGrid[2]).arg(tabGrid[1]));
		  }
		}
	  }
	  if(getSaveStyle() && __helpmenu){
		__helpmenu->setStyle(style);
	  }
	  if(getSaveWinPos()){
		QRect maxrect = QApplication::desktop()->geometry();
		if(x > 0 && x < maxrect.width() && 
		  y > 0 && y < maxrect.height() && 
		  w > 100 && h > 100){
		  if(parentWidget()){
			QRect rect = parentWidget()->geometry();
			parentWidget()->setGeometry(x,y,w,h);
			qDebug(QString("MainWindow.setGeometry(%1,%1,%1,%1)").arg(x).arg(y).arg(w).arg(h));
		  }
		}
	  }
	  if(getSaveCameraReDim()){
		ViewCameraGL3 * camera = __GLFrame->getCamera();
		if(camera)camera->lockDim(camReDim);
	  }
	  if(getSaveUseSpinBox() && __controlPanel) {
		__controlPanel->useSpinBox(useSpinBox);
	  }
	  if(getToolbarStateSave()){
		QMainWindow * mw = __locatToolBar->mainWindow();
		QObjectList * tbars = (mw?mw->queryList("QToolBar"):NULL);
		if(mw&&tbars){
		  QObjectListIt it( *tbars ); 
		  QObject * obj;
		  while((obj=it.current()) != 0 ){
			++it;
			QString tname = obj->name(0);
			if(!tname.isEmpty()){
			  int i = 0;
			  while(i < numToolBar && lname[i] != tname)i++;
			  if(i < numToolBar){
				QToolBar * tbar = (QToolBar *)obj;
				QMainWindow::ToolBarDock _ldock = QMainWindow::Top;
				switch (ldock[i]){
#if QT_VERSION < 300
				case 0 : _ldock = QMainWindow::Top ; break;
				case 1 : _ldock = QMainWindow::Bottom ; break;
				case 2 : _ldock = QMainWindow::Left ; break;
				case 3 : _ldock = QMainWindow::Right ; break;
				case 4 : _ldock = QMainWindow::Minimized ; break;
#else
				case 0 : _ldock = Qt::DockTop ; break;
				case 1 : _ldock = Qt::DockBottom ; break;
				case 2 : _ldock = Qt::DockLeft ; break;
				case 3 : _ldock = Qt::DockRight ; break;
				case 4 : _ldock = Qt::DockMinimized ; break;
				case 5 : _ldock = Qt::DockTornOff ; break;
				case 6 : _ldock = Qt::DockUnmanaged ; break;
#endif
				}
				mw->moveToolBar( tbar,
				  _ldock, lindex[i], lnl[i], lextraOffset[i] );
				
				if(lv[i]==1) tbar->show();
				else tbar->hide();
			  }
			}
		  }
		}
	  }
	}
    else {
	qWarning(tr("Cannot access to init file")+" :\""+filename+'"');
	}
  }
  else {
	__helpmenu->showInit();
  }
}


void ViewFileManager3::saveConfig()
{
  QString filename = getInitFilename();
  QFile _fileConfig(filename.latin1());
  if(_fileConfig.open(IO_WriteOnly)){
	QColor _Color = __GLFrame->getBackGroundColor();
	// m_fileConfig.writeBlock("grilleXY grilleYZ grilleXZ axes red green blue\n",32);
	QTextStream f_fileConfig( &  _fileConfig );
	const ViewGridGL3 * grid = __GLFrame->getGrid();
	const ViewCameraGL3 * camera = __GLFrame->getCamera();
	int style = (__helpmenu?__helpmenu->getStyle():-1);
	int w = 0, h = 0;
	int x = 0, y = 0;
   
	if(parentWidget()){
	  QWidget * par = parentWidget();
	  QPoint pos = par->mapToGlobal(QPoint(0,0));
	  x = pos.x();
	  y = pos.y();
	  w = par->width();
	  h = par->height();
	}

	f_fileConfig << CONFIG_VERSION << endl;
	f_fileConfig << __saveoptions << endl;
	if(getSaveGridVisibility())f_fileConfig << (int)grid->getXYGrid() << " "<< (int)grid->getYZGrid() << " " 
	  << (int)grid->getXZGrid() << " " << (int)grid->getAxis() << " " << grid->getGridSize() << endl;
	if(getSaveBgColor())f_fileConfig << _Color.red() << " "   << _Color.green() << " "  << _Color.blue() <<  endl;
    if(getSaveStyle()) f_fileConfig << style << endl;
	if(getSaveWinPos())
	  f_fileConfig << x << " "   << y << " "  << w << " "  << h << endl;
    if(getSaveCameraReDim())	
	  f_fileConfig << (camera?(int)camera->isDimLock():1) << endl;
	  
	if(getSaveUseSpinBox())	
	  f_fileConfig << (__controlPanel?(int)__controlPanel->spinBoxUsed():1) << endl;	
	if(getToolbarStateSave()){
	  QMainWindow * mw = (__locatToolBar ?__locatToolBar->mainWindow():NULL);
	  QString lname;
	  QMainWindow::ToolBarDock _ldock = QMainWindow::Top;
	  int ldock = 0;
	  int lindex = 0;
	  bool lnl = true;
	  int lextraOffset = -1;
	  int lv = true;
	  QObjectList * tbars = (mw?mw->queryList("QToolBar"):NULL);
	  if(!tbars||!mw)f_fileConfig << 0 <<  endl;
	  else {
		uint tbcount = 0;
		for(uint i = 0; i < tbars->count(); i++)
		  if(tbars->at(i)->name(0))tbcount++;
		  f_fileConfig << tbcount <<  endl;
		  QObjectListIt it( *tbars ); 
		  QObject * obj;
		  while((obj=it.current()) != 0 ){
			++it;
			if(obj->name(0)){
			  QToolBar * tbar = (QToolBar *)obj;
			  mw->getLocation( tbar, _ldock, lindex, lnl, lextraOffset );
			  lv = tbar->isVisible();
			  lname = tbar->name();
			  switch (_ldock){
#if QT_VERSION < 300
			  case QMainWindow::Top : ldock = 0; break;
			  case QMainWindow::Bottom : ldock = 1; break;
			  case QMainWindow::Left : ldock = 2; break;
			  case QMainWindow::Right : ldock = 3; break;
			  case QMainWindow::Minimized : ldock = 4; break;
#else
			  case Qt::DockTop : ldock = 0; break;
			  case Qt::DockBottom : ldock = 1; break;
			  case Qt::DockLeft : ldock = 2; break;
			  case Qt::DockRight : ldock = 3; break;
			  case Qt::DockMinimized : ldock = 4; break;
			  case Qt::DockTornOff : ldock = 5; break;
			  case Qt::DockUnmanaged : ldock = 6; break;
#endif
			  }		
			  f_fileConfig << '"' << lname << '"' << " " << ldock << " "   << lindex << " "  << (int)lnl << " "  << lextraOffset << " "  << lv <<  endl;
			}
		  }
	  }
	}

    if(getSaveHistory()){
	  for (uint32_t ind=0; ind < __lastOpenFiles.size() ; ind++)
		f_fileConfig << "\"" << __lastOpenFiles[ind] << "\"" << endl;
	}
	_fileConfig.close();
  }
  else if(_fileConfig.exists())
	  qWarning(tr("Cannot access to existing init file")+" :\""+filename+'"');
  else qWarning(tr("Cannot access to init file")+" :\""+filename+'"');
}


void ViewFileManager3::saveConfigBeforeQuit()
 {
  saveConfig();
  return;

   QString filename = getInitFilename();

    QFile _fileConfig(filename);
    bool tabGrid[4];
    int r,g,b;
	int style = (__helpmenu?__helpmenu->getStyle():-1);
	int w = 0, h = 0;
	int x = 0, y = 0;
    bool overwrite = true;
  /*  if(_fileConfig.exists() && _fileConfig.open( IO_ReadOnly )) {
		int tabGridtemp[4];		
		QTextStream f_configFile( & _fileConfig );
		float version;
		f_configFile >> version;
		if(version != CONFIG_VERSION){			
			f_configFile >> tabGridtemp[0]  >> tabGridtemp[1]  
				>> tabGridtemp[2]  >> tabGridtemp[3] ;
			f_configFile >> r  >> g  >> b ;
			f_configFile >> style;
			f_configFile >> x  >> y  >> w >> h ;
			for(int i=0;i<4;i++)
				tabGrid[i]=(tabGridtemp[i]==1);
			overwrite = false;
		}
		_fileConfig.close();
   }*/

    if(overwrite) {
		const ViewGridGL3 * grid = __GLFrame->getGrid();
		if(grid != NULL){
			tabGrid[0] = grid->getXYGrid();
			tabGrid[1] = grid->getYZGrid();
			tabGrid[2] = grid->getXZGrid();
			tabGrid[3] = grid->getAxis();
			r=255;g=255;b=255;
		}
    }
    if(_fileConfig.open(IO_WriteOnly)){
	   qDebug("Write init file :\""+filename+'"');
		if(parentWidget()){
			QWidget * par = parentWidget();
			QPoint pos = par->mapToGlobal(QPoint(0,0));
			x = pos.x();
			y = pos.y();
			w = par->width();
			h = par->height();
		}
		QTextStream f_fileConfig( &  _fileConfig );
		f_fileConfig << CONFIG_VERSION << endl;
		f_fileConfig << (int)tabGrid[0] << " "<< (int)tabGrid[1] << " " 
			<< (int)tabGrid[2] << " " << (int)tabGrid[3] << endl;
		f_fileConfig << r << " "   << g << " "  << b <<  endl;
		f_fileConfig << style << endl;
		f_fileConfig << x << " "   << y << " "  << w << " "  << h <<  endl;
		for( uint32_t ind=0; ind < __lastOpenFiles.size() ; ind++ )
			f_fileConfig << "\"" << __lastOpenFiles[ind] << "\"" << endl;
		_fileConfig.close();
	}
	else qWarning(tr("Cannot access to init file")+" :\""+filename+'"');
}

/*-----------------------------------------------------------------*/


/* 
void ViewFileManager3::crop(){
	CropImages n(this,"",true);
	n.exec();
}
*/
