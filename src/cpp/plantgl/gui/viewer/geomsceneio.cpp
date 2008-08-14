/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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

#include "geomscenegl.h"

/// Tools
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/bfstream.h>

/// GEOM
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/scene/factory.h>
#include <plantgl/scenegraph/geometry/amapsymbol.h>

/// Action
#include <plantgl/algo/base/polygoncomputer.h>
#include <plantgl/algo/codec/povprinter.h>
#include <plantgl/algo/codec/plyprinter.h>
#include <plantgl/algo/codec/vrmlprinter.h>
#include <plantgl/algo/codec/vgstarprinter.h>
#include <plantgl/algo/codec/binaryprinter.h>
#include <plantgl/algo/codec/linetreeprinter.h>
#include <plantgl/algo/codec/ligfile.h>
#include <plantgl/algo/codec/vgsfile.h>
#include <plantgl/algo/codec/geomview_file.h>

/// Viewer
#include "../base/light.h"
#include "../base/camera.h"
#include "../base/icons.h"
#include "../base/glframe.h"
#include "../base/filemanager.h"

#include "linetree.h"
#include "reader.h"
#include "interface/importselection.h"

/// Qt
#include <qlabel.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qslider.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qapplication.h>


#ifdef QT_THREAD_SUPPORT
#ifndef PGL_DEBUG
#define GEOM_THREAD
#endif
#endif

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;
/* ----------------------------------------------------------------------- */

bool
ViewGeomSceneGL::addOpenEntries(QMenu * menu)
{
  QPixmap openIcon( ViewerIcon::getPixmap(ViewerIcon::fileopen ));
  menu->addAction( openIcon, tr("Open &Geom File"),
                       this,SLOT(openGeomFile()),Qt::CTRL+Qt::Key_G);
  menu->addAction( openIcon, tr("&Add Geom File"),
                       this,SLOT(addGeomFile()));
  return true;
}

bool
ViewGeomSceneGL::addSaveEntries(QMenu * menu)
{
  QPixmap saveIcon(ViewerIcon::getPixmap(ViewerIcon::filefloppy ));
  menu->addAction( saveIcon, tr("Save As &Geom"),
                       this,SLOT(saveAsGeom()));
  menu->addAction( saveIcon, tr("Save &Selection"),
                       this,SLOT(saveSelection()));
  menu->addAction( saveIcon, tr("Save &Not Selection"),
                       this,SLOT(saveNotSelection()));
  return true;
}

bool
ViewGeomSceneGL::addImportEntries(QMenu * menu)
{
  QPixmap openIcon(ViewerIcon::getPixmap(ViewerIcon::fileopen ));
  menu->addAction( openIcon , tr("Import &AmapSymbol"),
                    this,SLOT(openAmapSymbol()),Qt::CTRL+Qt::Key_A);
  menu->addAction( openIcon, tr("Import &Linetree Files"),
                       this,SLOT(openLinetree()),Qt::CTRL+Qt::Key_L);
  menu->addAction( openIcon, tr("Import &GeomView Files"),
                       this,SLOT(openGeomViewFile()));
  menu->addAction( openIcon, tr("Import &VegeStar Files"),
                       this,SLOT(openVegeStar()));
  QMenu * submenu = new QMenu(menu);
  submenu->addAction( openIcon , tr("Import Symbol")+" 11",
                    this,SLOT(openVegeStarSymbol1()));
  submenu->addAction( openIcon , tr("Import Symbol")+" 12",
                    this,SLOT(openVegeStarSymbol2()));
  submenu->addAction( openIcon , tr("Import Symbol")+" 13",
                    this,SLOT(openVegeStarSymbol3()));
  submenu->addSeparator();
  submenu->addAction( openIcon , tr("Show Symbol")+" 11",
                    this,SLOT(showVegeStarSymbol1()));
  submenu->addAction( openIcon , tr("Show Symbol")+" 12",
                    this,SLOT(showVegeStarSymbol2()));
  submenu->addAction( openIcon , tr("Show Symbol")+" 13",
                    this,SLOT(showVegeStarSymbol3()));
  submenu->addSeparator();
  submenu->addAction( openIcon , tr("Set Selection as Symbol")+" 11",
                    this,SLOT(setSelectionVegeStarSymbol1()));
  submenu->addAction( openIcon , tr("Set Selection as Symbol")+" 12",
                    this,SLOT(setSelectionVegeStarSymbol1()));
  submenu->addAction( openIcon , tr("Set Selection as Symbol")+" 13",
                    this,SLOT(setSelectionVegeStarSymbol1()));
  submenu->setTitle(tr("Import VegeStar Symbol"));
  menu->addMenu(submenu);
  return true;
}

bool
ViewGeomSceneGL::addExportEntries(QMenu * menu)
{
  QPixmap saveIcon( ViewerIcon::getPixmap(ViewerIcon::filefloppy) );
  QPixmap savePovIcon( ViewerIcon::getPixmap( ViewerIcon::pov) );
  QPixmap saveVrmlIcon( ViewerIcon::getPixmap(ViewerIcon::vrml) );
  menu->addAction( saveIcon , tr("Export as &AmapSymbol"),
                    this,SLOT(saveAsAmapSymbol()));
  menu->addAction( saveIcon , tr("Export as &Linetree"),
                    this,SLOT(saveAsLinetree()));
  menu->addAction( savePovIcon , tr("Export as Pov&Ray"),
                    this,SLOT(saveAsPovRay()),Qt::CTRL+Qt::Key_R);
  menu->addAction( saveVrmlIcon , tr("Export as Vr&ml"),
                    this,SLOT(saveAsVrml()),Qt::CTRL+Qt::Key_M);
  menu->addAction( saveIcon , tr("Export as Ply"),
                    this,SLOT(saveAsPly()));
  menu->addAction( saveIcon , tr("Export as V&egeStar"),
                    this,SLOT(saveAsVegeStar()),Qt::CTRL+Qt::Key_E);
  return true;
}

/* ----------------------------------------------------------------------- */
void 
ViewGeomSceneGL::addFile(const QString& filename)
{
  if(!filename.isEmpty()){
    QString ext = QFileInfo(filename).suffix();
    ext = ext.toUpper();
    if(ext == "SMB"){
      openAmapSymbol(filename,true);
    }
    else if(ext == "GEO" || ext == "GEOM" || ext == "BGEOM" ){
      addGeomFile(filename);
    }
    else if(ext == "LIG"){
      openLinetree(filename,true);
    }
    else if(ext == "VGX" || ext == "VGS"){
      openVegeStar(filename,true);
    }
    else if(ext == "POL"){
      openVegeStarSymbol1(filename,true);
    }
    else if(ext == "QUAD" || ext == "BEZ" || ext == "OFF"){
      openGeomViewFile(filename);
    }
  }
}

bool
ViewGeomSceneGL::open(const QString& filename)
{
  if(!filename.isEmpty()){
    QString ext = QFileInfo(filename).suffix();
    ext = ext.toUpper();
    if(ext == "SMB"){
      openAmapSymbol(filename);
      return true;
    }
    else if(ext == "GEO" || ext == "GEOM" || ext == "BGEOM" ){
      return openGeomFile(filename);
    }
    else if(ext == "LIG"){
      return openLinetree(filename);
    }
    else if(ext == "VGX" || ext == "VGS"){
      return openVegeStar(filename);
    }
    else if(ext == "POL"){
      return openVegeStarSymbol1(filename);
    }
    else if(ext == "QUAD" || ext == "BEZ" || ext == "OFF"){
      return openGeomViewFile(filename);
    }
    else if(SceneFactory::get().isReadable(filename.toStdString())){
      return openGeomFile(filename);
    }
    else {
			static int selection = 5;
			QDialog dialog(__frame);
			dialog.setModal(true);
			Ui::ViewImporterSelection importer;
			importer.setupUi(&dialog);
			QObject::connect(importer.OkButton,SIGNAL(pressed()),&dialog,SLOT(accept()));
			QObject::connect(importer.CancelButton,SIGNAL(pressed()),&dialog,SLOT(reject()));
			importer.Type->setText(ext);
			importer.File->setText(QFileInfo(filename).fileName());
			importer.ImporterCombo->setCurrentIndex(selection);
			if(dialog.exec() == QDialog::Rejected)return false;
			else {
				selection = importer.ImporterCombo->currentIndex();
				switch(selection){
				case 0 :return openGeomFile(filename);break;
				case 1 :return openAmapSymbol(filename);break;
				case 2 :return openLinetree(filename);break;
				case 3 :return openVegeStar(filename);break;
				case 4 :return openVegeStarSymbol1(filename);break;
				case 5 :return openGeomViewFile(filename);break;
                default : return false; break;
				}
			}
        }
  }
  else {
            QMessageBox::warning(__frame,tr("Empty Filename"),
                         tr("Cannot open Empty filename"),1,0,0);
          return false;
  }
}
/*
bool
ViewGeomSceneGL::openStream(std::istream& stream)
{
#ifdef GEOM_THREAD
  if(__reader && __reader->isRunning()){
    QMessageBox::warning(__frame,tr("Already Reading File"),
                         QString(tr("Currently Reading File")+" %1").
                         arg(__reader->getFilename()),1,0,0);
    return false;
  }
  else{
    if(__reader) {
      delete __reader;
      __reader = NULL;
    }
    __reader = new ViewGeomStreamReader(&stream,this);
    __reader->start();
    return true;
  }
#else
    stringstream _errlog(ios::out) ;
    ScenePtr scene = ScenePtr(new Scene(stream,_errlog));
    setScene(scene);
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
    emit valueChanged();
    setFilename("");
    return true;
#endif
  return false;
}*/

/* ----------------------------------------------------------------------- */

void
ViewGeomSceneGL::openGeomFile()
{
  QString filename = QFileDialog::getOpenFileName(__frame,"Open PlantGL",getFilename(),tr("PlantGL File")+" (*.geom;*.bgeom);;"+tr("All Files (*.*)"));
  if(!filename.isEmpty())
     openGeomFile(filename);
}

void
ViewGeomSceneGL::addGeomFile()
{
  QString filename = QFileDialog::getOpenFileName(__frame,"Open PlantGL",getFilename(),tr("PlantGL File")+" (*.geom;*.bgeom);;"+tr("All Files (*.*)"));
  if(!filename.isEmpty())
     addGeomFile(filename);
}

/* ----------------------------------------------------------------------- */

void
ViewGeomSceneGL::openAmapSymbol()
{
  QString filename = QFileDialog::getOpenFileName(__frame,"Open Symbol",getFilename(),tr("Amap Symbol")+" (*.smb);;"+tr("All Files (*.*)"));
  if(!filename.isEmpty()){
    openAmapSymbol(filename);
  }
}



void
ViewGeomSceneGL::openGeomViewFile()
{
  QString filename = QFileDialog::getOpenFileName(__frame,"Open GeomView File",getFilename(),"");
  if(!filename.isEmpty())
     openGeomViewFile(filename);
}

void
ViewGeomSceneGL::openLinetree()
{
  openLinetree(QString::null);
}


void
ViewGeomSceneGL::openVegeStar()
{
  QString filename = QFileDialog::getOpenFileName(__frame,"Open VegeStar File",getFilename(),tr("VegeStar File")+" (*.vgx;*.vgs);;"+tr("All Files (*.*)"));
  if(!filename.isEmpty()){
    openVegeStar(filename);
  }
}

void
ViewGeomSceneGL::openVegeStarSymbol1()
{
  QString filename = QFileDialog::getOpenFileName(__frame,"Open Symbol",getFilename(),tr("VegeStar Symbol")+" (*.pol);;"+tr("All Files (*.*)"));
  if(!filename.isEmpty()){
	openVegeStarSymbol1(filename);
  }
}

bool
ViewGeomSceneGL::openVegeStarSymbol1(const QString& filename,bool add)
{
  if(!filename.isEmpty()){
    stringstream _errlog(ios::out) ;
    GeometryPtr _symbol = VegeStarFile::importPolygonFile(filename.toStdString(),_errlog);
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());

    if (_symbol->isValid()) {
	  VegeStarFile::setShape11(_symbol);
      ScenePtr scene(new Scene());
      scene->add(Shape3DPtr(new Shape(_symbol)));
	  if(add){
		addScene(scene);
		return true;
	  }
	  else {
      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename(filename);
		return true;
      }
	  else return false;
	  }
    }
	else return false;
  }
  else return false;
}

void
ViewGeomSceneGL::openVegeStarSymbol2()
{
  QString filename = QFileDialog::getOpenFileName(__frame,"Open Symbol",getFilename(),tr("VegeStar Symbol")+" (*.pol);;"+tr("All Files (*.*)"));
  if(!filename.isEmpty()){
    stringstream _errlog(ios::out) ;
    GeometryPtr _symbol = VegeStarFile::importPolygonFile(filename.toStdString(),_errlog);
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
 
    if (_symbol->isValid()) {
	  VegeStarFile::setShape12(_symbol);
      ScenePtr scene(new Scene());
      scene->add(Shape3DPtr(new Shape(_symbol)));

      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename(filename);
      }
    }
  }
}

void
ViewGeomSceneGL::openVegeStarSymbol3()
{
  QString filename = QFileDialog::getOpenFileName(__frame,"Open Symbol",getFilename(),tr("VegeStar Symbol")+" (*.pol);;"+tr("All Files (*.*)"));
  if(!filename.isEmpty()){
    stringstream _errlog(ios::out) ;
    GeometryPtr _symbol = VegeStarFile::importPolygonFile(filename.toStdString(),_errlog);
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
 
    if (_symbol->isValid()) {
	  VegeStarFile::setShape13(_symbol);
      ScenePtr scene(new Scene());
      scene->add(Shape3DPtr(new Shape(_symbol)));

      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename(filename);
      }
    }
  }
}
void
ViewGeomSceneGL::showVegeStarSymbol1(){
	  GeometryPtr _symbol = VegeStarFile::getShape11();
      ScenePtr scene(new Scene());
      scene->add(Shape3DPtr(new Shape(_symbol)));

      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename("");
      }
 }

void
ViewGeomSceneGL::setSelectionVegeStarSymbol1(){
  if(!__selectedShapes.empty()){
	ShapePtr sh = ShapePtr::Cast(__selectedShapes.begin()->second);
	if(sh.isValid()&&sh->getGeometry().isValid())
	  VegeStarFile::setShape11(sh->getGeometry());
  }
 }

void
ViewGeomSceneGL::setSelectionVegeStarSymbol2(){
  if(!__selectedShapes.empty()){
	ShapePtr sh = ShapePtr::Cast(__selectedShapes.begin()->second);
	if(sh.isValid()&&sh->getGeometry().isValid())
	  VegeStarFile::setShape12(sh->getGeometry());
  }
 }

void
ViewGeomSceneGL::setSelectionVegeStarSymbol3(){
  if(!__selectedShapes.empty()){
	ShapePtr sh = ShapePtr::Cast(__selectedShapes.begin()->second);
	if(sh.isValid()&&sh->getGeometry().isValid())
	  VegeStarFile::setShape13(sh->getGeometry());
  }
 }

void
ViewGeomSceneGL::showVegeStarSymbol2(){
	  GeometryPtr _symbol = VegeStarFile::getShape12();
      ScenePtr scene(new Scene());
      scene->add(Shape3DPtr(new Shape(_symbol)));

      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename("");
      }
 }

void
ViewGeomSceneGL::showVegeStarSymbol3(){
	  GeometryPtr _symbol = VegeStarFile::getShape13();
      ScenePtr scene(new Scene());
      scene->add(Shape3DPtr(new Shape(_symbol)));

      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename("");
      }
 }

/* ----------------------------------------------------------------------- */

bool
ViewGeomSceneGL::openGeomFile(const QString& filename)
{
  if(!filename.isEmpty()){
#ifdef GEOM_THREAD
    if(__reader && __reader->isRunning()){
      QMessageBox::warning(__frame,tr("Already Reading File"),
                           QString(tr("Currently Reading File")+" %1").
                           arg(__reader->getFilename()),1,0,0);
      return false;
    }
    else {
      if(__reader) {
        delete __reader;
        __reader = NULL;
      }
      __reader = new ViewGeomReader(filename,
                                    this);
      __reader->start();
      return true;
    }
#else
    stringstream _errlog(ios::out) ;
    ScenePtr scene = ScenePtr(new Scene(filename.toAscii().constData(),"",_errlog));
    setScene(scene);
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
    emit valueChanged();
    setFilename(filename);
    return true;
#endif
  }
  return false;
}

bool
ViewGeomSceneGL::addGeomFile(const QString& filename)
{
  if(!filename.isEmpty()){
#ifdef GEOM_THREAD
    if(__reader && __reader->isRunning()){
	  __reader->wait();
    }
	if(__reader) {
	  delete __reader;
	  __reader = NULL;
	}
	__reader = new ViewGeomReader(filename,
	  this,-1,true);
	__reader->start();
	return true;
#else
    stringstream _errlog(ios::out) ;
    ScenePtr scene = ScenePtr(new Scene(filename.toAscii().constData(),"",_errlog));
    addScene(scene);
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
    else error(_msg.c_str());
    emit valueChanged();
    return true;
#endif
  }
  return false;
}

/* ----------------------------------------------------------------------- */

bool
ViewGeomSceneGL::openAmapSymbol(const QString& filename,bool add)
{
  if(! filename.isEmpty()) {
    AppearancePtr _material(new Material());
    _material->setName(string("Default"));
    stringstream _errlog(ios::out) ;
	ostream * s1 = SceneObject::errorStream;
	ostream * s2 = SceneObject::warningStream;
	ostream * s3 = SceneObject::commentStream;
	SceneObject::errorStream = & _errlog;
	SceneObject::warningStream = & _errlog;
	SceneObject::commentStream = & _errlog;

    GeometryPtr _symbol(new AmapSymbol(filename.toStdString(),
                                       AmapSymbol::DEFAULT_SOLID));
	SceneObject::errorStream = s1;
	SceneObject::warningStream = s2;
	SceneObject::commentStream = s3;
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
		  
    if (_symbol->isValid()) {

      ScenePtr scene(new Scene());
      scene->add(Shape3DPtr(new Shape(_symbol,_material)));
	  if(add){
		addScene(scene);
		return true;
	  }
	  else {
      if(setScene(scene) ==1){
		  setFilename(filename);
		  
		  return true;
      }
      else return false;
	  }
    }
    else return false;
  }
  else return false;
}



bool
ViewGeomSceneGL::openGeomViewFile(const QString& filename,bool add)
{
  if(!filename.isEmpty()){
    stringstream _errlog(ios::out) ;
    ScenePtr scene = GeomViewBuilder::Parse(filename.toAscii().constData(),_errlog);
    if(add)addScene(scene);
    else {
	  setScene(scene);
	  setFilename(filename);
	}
	string _msg = _errlog.str();
	if(!_msg.empty() )error(_msg.c_str());
    emit valueChanged();
    return true;
  }
  return false;
}


bool
ViewGeomSceneGL::openLinetree(const QString& filename,bool add)
{
  ViewReadLinetree __linetreeDialog;
  if(!filename.isEmpty())__linetreeDialog.setLigFile(filename);
  if(__linetreeDialog.exec()){
#ifdef GEOM_THREAD
    if(__reader && __reader->isRunning()){
	  if(!add){
      QMessageBox::warning(__frame,tr("Already Reading File"),
                           QString(tr("Currently Reading File")+" %1").
                           arg(__reader->getFilename()),1,0,0);
      return false;
	  }
	  else __reader->wait();
    }
    else{
      if(__reader) {
        delete __reader;
        __reader = NULL;
      }
      __reader = new ViewLinetreeReader(__linetreeDialog.getLigFile(),
                                    __linetreeDialog.getDtaFile(),
                                    __linetreeDialog.getSMBPath(),
                                    this,
									__linetreeDialog.bigEndian(),add);
      __reader->start();
      return true;
    }
#else
    ScenePtr scene = readLineTree(string(__linetreeDialog.getLigFile().toStdString()),
                                  string(__linetreeDialog.getDtaFile().toStdString()),
                                  string(__linetreeDialog.getSMBPath().toStdString()),
								  __linetreeDialog.bigEndian(),cerr);
	if(add){
	  addScene(scene);
	  return true;
	}
	else {
	  if(setScene(scene) == 1)
      {
        setFilename(__linetreeDialog.getLigFile());
        return true;
      }
	  else return false;
	}
#endif
  }
  return false;
}

bool
ViewGeomSceneGL::openVegeStar(const QString& filename,bool add)
{
  if(!filename.isEmpty()){
/*#ifndef QT_THREAD_SUPPORT
    if(__reader && __reader->isRunning()){
      QMessageBox::warning(__frame,"Already Reading File",
                           QString("Reading Yet File %1").
                           arg(__reader->getFilename()),1,0,0);
      return false;
    }
    else {
      if(__reader) {
        delete __reader;
        __reader = NULL;
      }
      __reader = new ViewVegeStarReader(filename,
										this);
      __reader->start();
      return true;
    }
#else*/
    stringstream _errlog(ios::out) ;
    ScenePtr scene = VegeStarFile::read(filename.toStdString(),_errlog);
	if(add)addScene(scene);
	else {
	  setScene(scene);
	  setFilename(filename);
	}
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
    emit valueChanged();
    return true;
// #endif
  }
  return false;
}

/* ----------------------------------------------------------------------- */
void
ViewGeomSceneGL::save()
{
  if(getFilename().isEmpty()){
	saveAs();return; }
  QString extension=QFileInfo(getFilename()).suffix();
  extension= extension.toUpper();
  if(extension.isEmpty())saveAs();
  else if(extension == "GEOM" || extension == "BGEOM" )
    saveAsGeom(getFilename());
  else if(extension == "VGX" )
    saveAsVegeStar(getFilename());
  else if(extension == "LIG" )
    saveAsLinetree(getFilename());
  else saveAs();
  setFilename(getFilename());
}

void
ViewGeomSceneGL::saveAs()
{
  QString initial = getFilename();
  QFileInfo fileinfo(initial);
  QString extension=fileinfo.suffix();
  extension= extension.toUpper();
  if(!extension.isEmpty() && extension != "GEOM" && extension != "BGEOM" ){
	initial = initial.left(initial.lastIndexOf('.')+1);
    initial += "geom";
  }
  QString filename = ViewFileManager::getSaveFileName(initial,"",tr("PlantGL File")+" (*.geom;*.bgeom);;"+tr("All Files (*.*)"),__frame);
  if(!filename.isEmpty()){
    if(saveAsGeom(filename)){
      setFilename(filename);
    }
  }
}

void
ViewGeomSceneGL::saveAsGeom()
{
  QString shape, geom, mat;
  if(getGeomFilenames(shape,geom, mat)){
	if(saveScene(shape,geom,mat,__scene))
	  setFilename(shape);
  }
}

void
ViewGeomSceneGL::saveSelection()
{
  if(__selectedShapes.empty()){
	QMessageBox::warning(__frame,tr("Empty Selection"),tr("No Shape are selected!"),
								   "Ok");
	return;
  }
  QString shape, geom, mat;
  if(getGeomFilenames(shape,geom, mat)){
	saveScene(shape,geom,mat,getSelection());
  }
}

void
ViewGeomSceneGL::saveNotSelection()
{
  if(__selectedShapes.empty()){
	QMessageBox::warning(__frame,tr("Empty Selection"),tr("No Shape are selected!"),
								   "Ok");
	return;
  }
  QString shape, geom, mat;
  if(getGeomFilenames(shape,geom, mat)){
	saveScene(shape,geom,mat,getNotSelection());
  }
}

/* ----------------------------------------------------------------------- */
bool 
ViewGeomSceneGL::saveAsGeom(const QString& filename)
{
  return saveScene(filename,QString::null,QString::null,__scene);
}

bool 
ViewGeomSceneGL::saveAsGeom(const QString& shape,
						    const QString& geom,
							const QString& mat)
{
  return saveScene(shape,geom,mat,__scene);
}

/*
bool
ViewGeomSceneGL::saveAs(const QString& filename){
  QString extension=filename.right(filename.length()-filename.findRev('.')-1);
  extension= extension.upper();
  if(extension == "GEOM" ){
    QString _geomFilename = filename;
    QString _matFilename = filename.left(filename.findRev('.')+1) + "mat";
    ofstream fgeom(_geomFilename);
    fgeom << " # -Geom File-\n" ;
    ofstream fmat(_matFilename);
    fmat << " # -Appearance File-\n";
    Printer _printer(fgeom,fgeom,fmat);
    _printer.header(fgeom,_geomFilename,"File Generated with Geom 3D Viewer");
    fgeom  << ":include " << _matFilename.toAscii().constData() << "\n\n\n";
    _printer.header(fmat,_matFilename,"File Generated with Geom 3D Viewer");
    return __scene->apply(_printer);
  }
  else if(extension == "BGEOM" ){
    return BinaryPrinter::print(__scene,string(filename.data()),"File Generated with Geom 3D Viewer");
  }
  else return false;
}*/

class GeomDialog : public QFileDialog {
public :
  GeomDialog( const QString & initial, 
			  const QString & filter = QString::null,
			  QWidget * parent=0,
			  const char * name = 0, 
			  bool modal = true ) :
  QFileDialog ( parent, name, !initial.isEmpty()?QFileInfo(initial).filePath():"", filter   ) {
	// setSelection(initial);
	setFileMode(QFileDialog::AnyFile);
	QGroupBox * box = new QGroupBox(qApp->translate("ViewGeomSceneGL","Separated File for"),this);
	__geometry = new QRadioButton(qApp->translate("ViewGeomSceneGL","Geometry"),box);
	__appearance = new QRadioButton(qApp->translate("ViewGeomSceneGL","Appearance"),box);
	setExtension(box);
  }

  bool geometrySeparated() const {return __geometry->isChecked();}
  bool appearanceSeparated() const {return __appearance->isChecked();}

protected:

  QRadioButton *  __geometry;
  QRadioButton *  __appearance;
};

bool 
ViewGeomSceneGL::getGeomFilenames(QString& shape,QString& geom, QString& mat)
{
  shape = QString::null;
  QString initial = getFilename();
  QString extension=getFilename().right(getFilename().length()-getFilename().lastIndexOf('.')-1);
  extension= extension.toUpper();
  if(!extension.isEmpty() && extension != "GEOM" && extension != "BGEOM" ){
    initial = getFilename().left(getFilename().lastIndexOf('.')+1);
    initial += "geom";
  }
  
  GeomDialog dialog( initial, tr("PlantGL File")+" (*.geom;*.bgeom);;"+tr("All Files (*.*)"), __frame,tr("Save").toAscii().data());
  if(dialog.exec() != QFileDialog::Rejected) shape = dialog.selectedFiles()[0];
  
  if(shape.isEmpty())return false;
  else {
	QString extension=shape.right(shape.length()-shape.lastIndexOf('.')-1);
	extension= extension.toUpper();
	if (extension != "GEOM" && extension != "BGEOM" ) {
	  shape = shape.left(shape.lastIndexOf('.')+1);
	  shape += "GEOM";
	}
  }
  extension=shape.right(shape.length()-shape.lastIndexOf('.')-1);
  extension= extension.toUpper();

  if(extension != "BGEOM" &&dialog.geometrySeparated()){
	geom = shape.left(shape.lastIndexOf('.')-1);
	geom += "_geom.geom";
  }
  if(extension != "BGEOM" &&dialog.appearanceSeparated()){
	mat = shape.left(shape.lastIndexOf('.')+1);
	mat += "mat";
  }
  if(QFile::exists(shape)){

	int res;
	if(extension != "BGEOM" &&
	  (dialog.geometrySeparated() || dialog.appearanceSeparated()))
	res = QMessageBox::warning(__frame,tr("File Already Exists"),tr("Shape File")+" "+ shape 
								   + " "+tr("already exists. Overwrite ?"),
								   tr("Yes"),tr("Yes To All"),tr("Cancel"));
	else 
	res = QMessageBox::warning(__frame,tr("File Already Exists"),tr("Shape File")+" "+ shape 
								   + " "+tr("already exists. Overwrite ?"),
								   tr("Yes"),tr("Cancel"));
	if(res !=0 && res !=1)return false;
	if(res == 1)return true;
	if(extension != "BGEOM"){
	  if(dialog.geometrySeparated()){
		if(dialog.appearanceSeparated())
		  res = QMessageBox::warning(__frame,tr("File Already Exists"),tr("Geometry File")+" "+ geom 
		  + " " + tr("already exists. Overwrite ?"),
		  tr("Yes"),tr("Yes To All"),tr("Cancel"));
		else 
		  res = QMessageBox::warning(__frame,tr("File Already Exists"),tr("Geometry File")+" "+ geom 
		  + " "+ tr("already exists. Overwrite ?"),
		  tr("Yes"),tr("Cancel"));
		if(res !=0 && res !=1)return false;
		if(res == 1)return true;
	  }
	  if(dialog.appearanceSeparated()){
		res = QMessageBox::warning(__frame,tr("File Already Exists"),tr("Appearance File")+" "+ mat 
		  + " "+tr("already exists. Overwrite ?"),
		  tr("Yes"),tr("Cancel"));
		if(res !=0)return false;
	  }
	}
  }
  return true;
}

bool 
ViewGeomSceneGL::saveScene(const QString& shape,
						   const QString& geom,
						   const QString& mat,
						   PGL(ScenePtr) scene){
  if(shape.isEmpty())return false;
  QString extension=shape.right(shape.length()-shape.lastIndexOf('.')-1);
  extension= extension.toUpper();
  if(extension == "GEOM" ){
	  ofstream fshape(shape.toAscii().data());
	if(!fshape)return false;
	fshape << " # -Geom File-\n";
	ofstream * fgeom;
	bool separatedgeom = false;
	if(!geom.isEmpty()){
	  fgeom = new ofstream(geom.toAscii().data());
	  if(!*fgeom)fgeom = &fshape;
	  else {
		*fgeom << " # -Geom File-\n";
		separatedgeom = true;
	  }
	}
	else fgeom = &fshape;

	ofstream * fmat;
	bool separatedmat = false;
	if(!mat.isEmpty()){
	  fmat = new ofstream(mat.toAscii().data());
	  if(!*fmat)fmat = &fshape;
	  else {
        *fmat << " # -Appearance File-\n";
	    separatedmat = true;
	  }
	}
	else fmat = &fshape;

    Printer _printer(fshape,*fgeom,*fmat);
	_printer.header(fshape,shape.toAscii().data(),tr("File Generated with PlantGL 3D Viewer").toAscii().data());
    if(separatedgeom){
      _printer.header(*fgeom,geom.toAscii().data(),tr("File Generated with PlantGL 3D Viewer").toAscii().data());
	  fshape  << ":include " << geom.toStdString() << "\n\n\n";
	}
    if(separatedmat){
      _printer.header(*fmat,mat.toAscii().data(),tr("File Generated with PlantGL 3D Viewer").toAscii().data());
	  fshape  << ":include " << mat.toStdString() << "\n\n\n";
	}
    return scene->apply(_printer);
  }
  else if(extension == "BGEOM" ){
	  return BinaryPrinter::print(__scene,shape.toStdString(),tr("File Generated with PlantGL 3D Viewer").toAscii().data());
  }
  else return false;
}


/* ----------------------------------------------------------------------- */
void
ViewGeomSceneGL::saveAsPovRay()
{
  QString filename = ViewFileManager::getSaveFileName(getFilename(),"pov",tr("PovRay File")+" (*.pov);;"+tr("All Files (*.*)"),__frame);
  if(!filename.isEmpty()){
    saveAsPovRay(filename);
  }
}

void
ViewGeomSceneGL::saveAsVegeStar()
{
  QString filename = ViewFileManager::getSaveFileName(getFilename(),"vgx",tr("VegeStar File")+" (*.vgx;*.vgs);;"+tr("All Files (*.*)"),__frame);
  if(!filename.isEmpty()){
    saveAsVegeStar(filename);
  }
}

void
ViewGeomSceneGL::saveAsAmapSymbol()
{
  QString filename = ViewFileManager::getSaveFileName(getFilename(),"smb",tr("Amap Symbol")+" (*.smb);;"+tr("All Files (*.*)"),__frame);
  if(!filename.isEmpty()){
    saveAsAmapSymbol(filename);
  }
}

void 
ViewGeomSceneGL::saveAsLinetree(){
  QString filename = getFilename();
  if(filename.isEmpty())saveAsLinetree(QString(""));
  else {
	  QString extension=QFileInfo(filename).suffix();
	  extension= extension.toUpper();    
	  if (extension != "LIG") {
		  filename = filename.left(filename.lastIndexOf('.')+1);
		  filename += "lig";
	  }
	  saveAsLinetree(filename);
  }
}

bool 
ViewGeomSceneGL::saveAsLinetree(const QString& filename){
  ViewReadLinetree __linetreeDialog(false);
  if(!filename.isEmpty())__linetreeDialog.setLigFile(filename);
  if(__linetreeDialog.exec()){
    return saveAsLinetree(__linetreeDialog.getLigFile(),
                          __linetreeDialog.getDtaFile(),
                          __linetreeDialog.getSMBPath(),
						  __linetreeDialog.getKey(),
						  __linetreeDialog.bigEndian());
  }
  else return false;
}

/* ----------------------------------------------------------------------- */

bool
ViewGeomSceneGL::saveAsPovRay(const QString& filename)
{
  ofstream _stream(filename.toAscii().data());
  if(_stream){
    Tesselator _tess;
    PovPrinter _printer(_stream,_tess);
    _printer.header(tr("File Generated with PlantGL 3D Viewer").toAscii().constData());
	_printer.beginHeader();
	_printer.setCamera(__camera->getEye(),__camera->getTranslation(),__camera->getViewAngle(),__camera->getAzimuth(),__camera->getElevation());
	const QColor& col = __light->getDiffuse();
	_printer.setLight(__light->getPosition(),Color3(col.red(),
							col.green(),
							col.blue()));
	_stream << "/*\n";
	_printer.setLight(__camera->getPosition(),Color3(col.red(),
												    col.green(),
													col.blue()));
	_stream << "*/\n\n";
    const QGLContext * c =__frame->context();
    if(c){
      QColor bg = c->overlayTransparentColor();
      if(bg.isValid())_printer.setBackGround(Color3(bg.red(),bg.green(),bg.blue()));
	  else {
		ViewGLFrame * f = dynamic_cast<ViewGLFrame *>(__frame);
		if(f != NULL){
			bg = f->getBackGroundColor();
			_printer.setBackGround(Color3(bg.red(),bg.green(),bg.blue()));
			_printer.setLineWidth(f->getLineWidth()/10);
		}
	  }
    }
	_printer.endHeader();
	_stream << "\n\n";
    __scene->apply(_printer);
    return true;
  }
  else return false;
}


bool
ViewGeomSceneGL::saveAsVegeStar(const QString& filename)
{
  ofstream _stream(filename.toAscii().data());
  if(_stream){
    Tesselator _tess;
    VgstarPrinter _printer(_stream,_tess);
    _printer.header(tr("File Generated with PlantGL 3D Viewer").toAscii().constData());
    __scene->apply(_printer);
    setFilename(filename);
    return true;
  }
  else return false;
}


bool
ViewGeomSceneGL::saveAsAmapSymbol(const QString& filename)
{
  Shape3DPtr shape = __scene->getAt(0);
  AmapTranslator translator(__discretizer);
  shape->apply(translator);
  AmapSymbolPtr result = translator.getSymbol();

  if(result) {
	  beofstream _stream(filename.toAscii().constData());
	  if(_stream){
		  result->write(_stream);
		  setFilename(filename);
		  return true;
	  }
	  else return false;
  }
  else return false;
}


bool 
ViewGeomSceneGL::saveAsLinetree(const QString& lig,
								const QString& dta,
								const QString& smb,
								const QString& key,
								bool bigendian){
	bofstream * ligstream;
	if (bigendian) ligstream = new beofstream(lig.toAscii().data());
	else ligstream = new leofstream(lig.toAscii().data());
	if(!*ligstream) return false;
	ofstream dtastream(dta.toAscii().data());
	if(!dtastream) return false;
	AmapTranslator t(__discretizer);
	LinetreePrinter printer(*ligstream,dtastream,smb.toAscii().data(),t);
	printer.header(key.toAscii().data());
	if(__scene->apply(printer)){
		setFilename(lig);
		QString inf = lig.left(lig.lastIndexOf('.')+1);
		inf += "inf";
		ofstream infstream(inf.toAscii().data());
		if(infstream){
			printer.printInf(infstream,
							 __bbox->getUpperRightCorner(),
							 __bbox->getLowerLeftCorner());
		}
		QString cfg = QFileInfo(lig).dir().path();
		string _cfg;
		if(!printer.printCfg(cfg.toAscii().data(),".",smb.toAscii().data(),_cfg)){
			QMessageBox::warning(__frame,tr("Cfg File"),
			tr("Cannot write file")+" '"+QString(_cfg.c_str())+"'",
            "Ok");
		}
		delete ligstream;
		return true;
	}
	else {
		delete ligstream;
		return false;
	}
}



void
ViewGeomSceneGL::saveAsVrml()
{
  QString filename = ViewFileManager::getSaveFileName(getFilename(),"wrl",tr("Vrml File")+" (*.wrl);;"+tr("All Files (*.*)"),__frame);
  ofstream _stream(filename.toAscii().data());
  if(_stream){
    VrmlPrinter _printer(_stream,__discretizer);
    _printer.header(tr("File Generated with PlantGL 3D Viewer").toAscii().constData());
	if(fabs(__camera->getAzimuth()) > GEOM_EPSILON   || 
	   fabs(__camera->getElevation()) > GEOM_EPSILON ||
	   __camera->getTranslation() != Vector3::ORIGIN)
    _printer.setCamera(__camera->getEye()-__camera->getTranslation(),__camera->getAzimuth(),__camera->getElevation(),"Geom View");
    _printer.setCamera(__camera->getEye(),0,0,"Home");
	const QColor& col = __light->getDiffuse();
	const QColor& col2 = __light->getAmbient();
	_printer.setLight(__light->getPosition(),Color3(col2.red(),col2.green(),col2.blue()),
					  Color3(col.red(),col.green(),col.blue()),4*norm(__light->getPosition()));
    const QGLContext * c =__frame->context();
    if(c){
      QColor bg = c->overlayTransparentColor();
      if(bg.isValid())_printer.setBackGround(Color3(bg.red(),bg.green(),bg.blue()));
	  else {
		ViewGLFrame * f = dynamic_cast<ViewGLFrame *>(__frame);
		if(f != NULL){
			bg = f->getBackGroundColor();
			_printer.setBackGround(Color3(bg.red(),bg.green(),bg.blue()));
		}
	  }
    }
    __scene->apply(_printer);
  }
}

void
ViewGeomSceneGL::saveAsPly()
{  
  QString filename = ViewFileManager::getSaveFileName(getFilename(),"ply",tr("Ply File")+ " (*.ply);;"+tr("All Files (*.*)"),__frame);
  if(!filename.isEmpty()){
    PlyPrinter::ply_format _format = PlyPrinter::ply_ascii;
    switch(QMessageBox::warning(__frame,"Ply Format",tr("Ply File Format"),
                                tr("Ascii"),tr("Binary Little Endian"),tr("Binary Big Endian"))){
    case 0 :
      _format = PlyPrinter::ply_ascii;
      break;
    case 1 :
      _format = PlyPrinter::ply_binary_little_endian;
      break;
    case 2 :
      _format = PlyPrinter::ply_binary_big_endian;
      break;
    case -1 :
      return;
      break;
    }
    Tesselator t;
    PlyPrinter::print(__scene,t,string(filename.toAscii().data()),
	  tr("File Generated with PlantGL 3D Viewer").toAscii().constData(),_format);
  }
}



/* ----------------------------------------------------------------------- */


bool
ViewMultiGeomSceneGL::openGeomFiles(const QString& filename,const QString& filename2)
{
  if(!filename.isEmpty() && !filename2.isEmpty()){
#ifdef GEOM_THREAD
    if(__reader && __reader->isRunning()){
      QMessageBox::warning(__frame,tr("Already Reading File"),
                           QString(tr("Currently Reading File")+" %1").
                           arg(__reader->getFilename()),1,0,0);
      return false;
    }
    else {
      if(__reader) {
        delete __reader;
        __reader = NULL;
      }
      __reader = new ViewMultiGeomReader(filename,
                                         filename2,
                                         this);
      __reader->start();
      return true;
    }
#else
    stringstream _errlog(ios::out) ;
    ScenePtr scene1 = ScenePtr(new Scene(filename.toAscii().constData(),"",_errlog));
    ScenePtr scene2 = ScenePtr(new Scene(filename2.toAscii().constData(),"",_errlog));
    setScene(scene1,scene2);
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
    emit valueChanged();
    setFilename(filename);
    if(__transSlider)__transSlider->show();
    return true;
#endif
  }
  return false;
}

void
ViewMultiGeomSceneGL::openGeomFiles()
{
  QString filename = QFileDialog::getOpenFileName(__frame,"Open PlantGL",getFilename(),tr("PlantGL File")+" (*.geom;*.bgeom);;"+tr("All Files (*.*)"));
  if(!filename.isEmpty()){
    QString filename2 = QFileDialog::getOpenFileName(__frame,"Open PlantGL",getFilename(),tr("PlantGL File")+" (*.geom;*.bgeom);;"+tr("All Files (*.*)"));
    if(!filename.isEmpty()){
      openGeomFiles(filename,filename2);
    }
  }
}


bool
ViewMultiGeomSceneGL::addOpenEntries(QMenu * menu)
{
  QPixmap openIcon( ViewerIcon::getPixmap(ViewerIcon::fileopen) );
  menu->addAction( openIcon, tr("Open &Geom File"),  this,SLOT(openGeomFile()),Qt::CTRL+Qt::Key_G);
  menu->addAction( openIcon, tr("&Add Geom File"),   this,SLOT(addGeomFile()));
  menu->addAction( openIcon, tr("Open &2 Geom File"),this,SLOT(openGeomFiles()));
  return true;
}

/* ----------------------------------------------------------------------- */
