/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
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

#include "geomscenegl.h"

/// Tools
#include <tool/util_string.h>
#include <tool/bfstream.h>

/// GEOM
#include <scenegraph/scene/shape.h>
#include <scenegraph/geometry/amapsymbol.h>

/// Action
#include <algo/base/polygoncomputer.h>
#include <algo/codec/povprinter.h>
#include <algo/codec/plyprinter.h>
#include <algo/codec/vrmlprinter.h>
#include <algo/codec/vgstarprinter.h>
#include <algo/codec/binaryprinter.h>
#include <algo/codec/linetreeprinter.h>
#include <algo/codec/ligfile.h>
#include <algo/codec/vgsfile.h>
#include <algo/codec/geomview_file.h>

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
#ifndef _DEBUG
#define GEOM_THREAD
#endif
#endif

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;
/* ----------------------------------------------------------------------- */

bool
ViewGeomSceneGL3::addOpenEntries(QPopupMenu * menu)
{
  QPixmap openIcon(ViewerIcon3::icon_fileopen );
  menu->insertItem( openIcon, tr("Open &Geom File"),
                       this,SLOT(openGeomFile()),CTRL+Key_G);
  menu->insertItem( openIcon, tr("&Add Geom File"),
                       this,SLOT(addGeomFile()));
  return true;
}

bool
ViewGeomSceneGL3::addSaveEntries(QPopupMenu * menu)
{
  QPixmap saveIcon(ViewerIcon3::icon_filefloppy );
  menu->insertItem( saveIcon, tr("Save As &Geom"),
                       this,SLOT(saveAsGeom()));
  menu->insertItem( saveIcon, tr("Save &Selection"),
                       this,SLOT(saveSelection()));
  menu->insertItem( saveIcon, tr("Save &Not Selection"),
                       this,SLOT(saveNotSelection()));
  return true;
}

bool
ViewGeomSceneGL3::addImportEntries(QPopupMenu * menu)
{
  QPixmap openIcon(ViewerIcon3::icon_fileopen );
  menu->insertItem( openIcon , tr("Import &AmapSymbol"),
                    this,SLOT(openAmapSymbol()),CTRL+Key_A);
  menu->insertItem( openIcon, tr("Import &Linetree Files"),
                       this,SLOT(openLinetree()),CTRL+Key_L);
  menu->insertItem( openIcon, tr("Import &GeomView Files"),
                       this,SLOT(openGeomViewFile()));
  menu->insertItem( openIcon, tr("Import &VegeStar Files"),
                       this,SLOT(openVegeStar()));
  QPopupMenu * submenu = new QPopupMenu(menu,"VGStar Symbol");
  submenu->insertItem( openIcon , tr("Import Symbol")+" 11",
                    this,SLOT(openVegeStarSymbol1()));
  submenu->insertItem( openIcon , tr("Import Symbol")+" 12",
                    this,SLOT(openVegeStarSymbol2()));
  submenu->insertItem( openIcon , tr("Import Symbol")+" 13",
                    this,SLOT(openVegeStarSymbol3()));
  submenu->insertSeparator();
  submenu->insertItem( openIcon , tr("Show Symbol")+" 11",
                    this,SLOT(showVegeStarSymbol1()));
  submenu->insertItem( openIcon , tr("Show Symbol")+" 12",
                    this,SLOT(showVegeStarSymbol2()));
  submenu->insertItem( openIcon , tr("Show Symbol")+" 13",
                    this,SLOT(showVegeStarSymbol3()));
  submenu->insertSeparator();
  submenu->insertItem( openIcon , tr("Set Selection as Symbol")+" 11",
                    this,SLOT(setSelectionVegeStarSymbol1()));
  submenu->insertItem( openIcon , tr("Set Selection as Symbol")+" 12",
                    this,SLOT(setSelectionVegeStarSymbol1()));
  submenu->insertItem( openIcon , tr("Set Selection as Symbol")+" 13",
                    this,SLOT(setSelectionVegeStarSymbol1()));
  menu->insertItem(tr("Import VegeStar Symbol"),submenu);
  return true;
}

bool
ViewGeomSceneGL3::addExportEntries(QPopupMenu * menu)
{
  QPixmap saveIcon( ViewerIcon3::icon_filefloppy );
  QPixmap savePovIcon( ViewerIcon3::icon_pov );
  QPixmap saveVrmlIcon( ViewerIcon3::icon_vrml );
  menu->insertItem( saveIcon , tr("Export as &AmapSymbol"),
                    this,SLOT(saveAsAmapSymbol()));
  menu->insertItem( saveIcon , tr("Export as &Linetree"),
                    this,SLOT(saveAsLinetree()));
  menu->insertItem( savePovIcon , tr("Export as Pov&Ray"),
                    this,SLOT(saveAsPovRay()),CTRL+Key_R);
  menu->insertItem( saveVrmlIcon , tr("Export as Vr&ml"),
                    this,SLOT(saveAsVrml()),CTRL+Key_M);
  menu->insertItem( saveIcon , tr("Export as Ply"),
                    this,SLOT(saveAsPly()));
  menu->insertItem( saveIcon , tr("Export as V&egeStar"),
                    this,SLOT(saveAsVegeStar()),CTRL+Key_E);
 // menu->insertItem( saveIcon , "&Fred Export",
 //                   this,SLOT(exportFred()),CTRL+Key_E);
  return true;
}

/* ----------------------------------------------------------------------- */
void 
ViewGeomSceneGL3::addFile(const QString& filename)
{
  if(!filename.isEmpty()){
    QString ext = QFileInfo(filename).extension();
    ext = ext.upper();
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
ViewGeomSceneGL3::open(const QString& filename)
{
  if(!filename.isEmpty()){
    QString ext = QFileInfo(filename).extension();
    ext = ext.upper();
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
    else {
			static int selection = 5;
//            QMessageBox::warning(__frame,"File Format Not Supported",						 	
//                         filename+"\nFile Format '"+ext+"' Not Supported",1,0,0);
			ViewImporterSelection dialog(__frame,"ImporterSelection",true);
			QObject::connect(dialog.OkButton,SIGNAL(pressed()),
							 &dialog,SLOT(accept()));
			QObject::connect(dialog.CancelButton,SIGNAL(pressed()),
							 &dialog,SLOT(reject()));
			dialog.Type->setText(ext);
			dialog.File->setText(QFileInfo(filename).fileName());
			dialog.ImporterCombo->setCurrentItem(selection);
			if(dialog.exec() == QDialog::Rejected)return false;
			else {
				selection = dialog.ImporterCombo->currentItem();
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
ViewGeomSceneGL3::openStream(std::istream& stream)
{
#ifdef GEOM_THREAD
  if(__reader && __reader->running()){
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
ViewGeomSceneGL3::openGeomFile()
{
  QString filename = QFileDialog::getOpenFileName(getFilename(),tr("Geom File")+" (*.geom;*.bgeom)",__frame,"Open Geom",tr("Open GEOM File"));
  if(!filename.isEmpty())
     openGeomFile(filename);
}

void
ViewGeomSceneGL3::addGeomFile()
{
  QString filename = QFileDialog::getOpenFileName(getFilename(),tr("Geom File")+" (*.geom;*.bgeom)",__frame,"Open Geom",tr("Add GEOM File"));
  if(!filename.isEmpty())
     addGeomFile(filename);
}

/* ----------------------------------------------------------------------- */

void
ViewGeomSceneGL3::openAmapSymbol()
{
  QString filename = QFileDialog::getOpenFileName(getFilename(),tr("Amap Symbol")+" (*.smb)",__frame,"Open Symbol",tr("Open Amap Symbol"));
  if(!filename.isEmpty()){
    openAmapSymbol(filename);
  }
}



void
ViewGeomSceneGL3::openGeomViewFile()
{
  QString filename = QFileDialog::getOpenFileName(getFilename(),"",__frame,"Open GeomView File","Import GeomView File");
  if(!filename.isEmpty())
     openGeomViewFile(filename);
}

void
ViewGeomSceneGL3::openLinetree()
{
  openLinetree(QString::null);
}


void
ViewGeomSceneGL3::openVegeStar()
{
  QString filename = QFileDialog::getOpenFileName(getFilename(),tr("VegeStar File")+" (*.vgx;*.vgs)",__frame,"Open VegeStar File",tr("Open VegeStar File"));
  if(!filename.isEmpty()){
    openVegeStar(filename);
  }
}

void
ViewGeomSceneGL3::openVegeStarSymbol1()
{
  QString filename = QFileDialog::getOpenFileName(getFilename(),tr("VegeStar Symbol")+" (*.pol)",__frame,"Open Symbol",tr("Open VegeStar Symbol"));
  qWarning("VGStar Symbol :"+filename);
  if(!filename.isEmpty()){
	openVegeStarSymbol1(filename);
	qWarning("VGStar Symbol '"+filename+"' loaded.");
  }
}

bool
ViewGeomSceneGL3::openVegeStarSymbol1(const QString& filename,bool add)
{
  if(!filename.isEmpty()){
    stringstream _errlog(ios::out) ;
    GeometryPtr _symbol = VegeStarFile::importPolygonFile(filename.data(),_errlog);
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());

    if (_symbol->isValid()) {
	  VegeStarFile::setShape11(_symbol);
      ScenePtr scene(new Scene());
      scene->add(Shape(_symbol,Material::DEFAULT_MATERIAL,0));
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
ViewGeomSceneGL3::openVegeStarSymbol2()
{
  QString filename = QFileDialog::getOpenFileName(getFilename(),tr("VegeStar Symbol")+" (*.pol)",__frame,"Open Symbol",tr("Open VegeStar Symbol"));
  if(!filename.isEmpty()){
    stringstream _errlog(ios::out) ;
    GeometryPtr _symbol = VegeStarFile::importPolygonFile(filename.data(),_errlog);
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
 
    if (_symbol->isValid()) {
	  VegeStarFile::setShape12(_symbol);
      ScenePtr scene(new Scene());
      scene->add(Shape(_symbol,Material::DEFAULT_MATERIAL,0));

      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename(filename);
      }
    }
  }
}

void
ViewGeomSceneGL3::openVegeStarSymbol3()
{
  QString filename = QFileDialog::getOpenFileName(getFilename(),tr("VegeStar Symbol")+" (*.pol)",__frame,"Open Symbol",tr("Open VegeStar Symbol"));
  if(!filename.isEmpty()){
    stringstream _errlog(ios::out) ;
    GeometryPtr _symbol = VegeStarFile::importPolygonFile(filename.data(),_errlog);
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
 
    if (_symbol->isValid()) {
	  VegeStarFile::setShape13(_symbol);
      ScenePtr scene(new Scene());
      scene->add(Shape(_symbol,Material::DEFAULT_MATERIAL,0));

      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename(filename);
      }
    }
  }
}
void
ViewGeomSceneGL3::showVegeStarSymbol1(){
	  GeometryPtr _symbol = VegeStarFile::getShape11();
      ScenePtr scene(new Scene());
      scene->add(Shape(_symbol,Material::DEFAULT_MATERIAL,0));

      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename("");
      }
 }

void
ViewGeomSceneGL3::setSelectionVegeStarSymbol1(){
  if(!__selectedShapes.empty()){
	ShapePtr sh = ShapePtr::Cast(__selectedShapes.begin()->second);
	if(sh.isValid()&&sh->getGeometry().isValid())
	  VegeStarFile::setShape11(sh->getGeometry());
  }
 }

void
ViewGeomSceneGL3::setSelectionVegeStarSymbol2(){
  if(!__selectedShapes.empty()){
	ShapePtr sh = ShapePtr::Cast(__selectedShapes.begin()->second);
	if(sh.isValid()&&sh->getGeometry().isValid())
	  VegeStarFile::setShape12(sh->getGeometry());
  }
 }

void
ViewGeomSceneGL3::setSelectionVegeStarSymbol3(){
  if(!__selectedShapes.empty()){
	ShapePtr sh = ShapePtr::Cast(__selectedShapes.begin()->second);
	if(sh.isValid()&&sh->getGeometry().isValid())
	  VegeStarFile::setShape13(sh->getGeometry());
  }
 }

void
ViewGeomSceneGL3::showVegeStarSymbol2(){
	  GeometryPtr _symbol = VegeStarFile::getShape12();
      ScenePtr scene(new Scene());
      scene->add(Shape(_symbol,Material::DEFAULT_MATERIAL,0));

      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename("");
      }
 }

void
ViewGeomSceneGL3::showVegeStarSymbol3(){
	  GeometryPtr _symbol = VegeStarFile::getShape13();
      ScenePtr scene(new Scene());
      scene->add(Shape(_symbol,Material::DEFAULT_MATERIAL,0));

      if(setScene(scene) ==1){
		emit valueChanged();
        setFilename("");
      }
 }

/* ----------------------------------------------------------------------- */

bool
ViewGeomSceneGL3::openGeomFile(const QString& filename)
{
  if(!filename.isEmpty()){
#ifdef GEOM_THREAD
    if(__reader && __reader->running()){
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
      __reader = new ViewGeomReader3(filename,
                                    this);
      __reader->start();
      return true;
    }
#else
    stringstream _errlog(ios::out) ;
    ScenePtr scene = ScenePtr(new Scene(filename.latin1(),"",_errlog));
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
ViewGeomSceneGL3::addGeomFile(const QString& filename)
{
  if(!filename.isEmpty()){
#ifdef GEOM_THREAD
    if(__reader && __reader->running()){
	  __reader->wait();
    }
	if(__reader) {
	  delete __reader;
	  __reader = NULL;
	}
	__reader = new ViewGeomReader3(filename,
	  this,-1,true);
	__reader->start();
	return true;
#else
    stringstream _errlog(ios::out) ;
    ScenePtr scene = ScenePtr(new Scene(filename.latin1(),"",_errlog));
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
ViewGeomSceneGL3::openAmapSymbol(const QString& filename,bool add)
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

    GeometryPtr _symbol(new AmapSymbol(filename.data(),
                                       AmapSymbol::DEFAULT_SOLID));
	SceneObject::errorStream = s1;
	SceneObject::warningStream = s2;
	SceneObject::commentStream = s3;
	string _msg = _errlog.str();
	if(!_msg.empty())error(_msg.c_str());
		  
    if (_symbol->isValid()) {

      ScenePtr scene(new Scene());
      scene->add(Shape(_symbol,_material,0));
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
ViewGeomSceneGL3::openGeomViewFile(const QString& filename,bool add)
{
  if(!filename.isEmpty()){
    stringstream _errlog(ios::out) ;
    ScenePtr scene = GeomViewBuilder::Parse(filename.latin1(),_errlog);
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
ViewGeomSceneGL3::openLinetree(const QString& filename,bool add)
{
  ViewReadLinetree3 __linetreeDialog;
  if(!filename.isEmpty())__linetreeDialog.setLigFile(filename);
  if(__linetreeDialog.exec()){
#ifdef GEOM_THREAD
    if(__reader && __reader->running()){
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
      __reader = new ViewLinetreeReader3(__linetreeDialog.getLigFile(),
                                    __linetreeDialog.getDtaFile(),
                                    __linetreeDialog.getSMBPath(),
                                    this,
									__linetreeDialog.bigEndian(),add);
      __reader->start();
      return true;
    }
#else
    ScenePtr scene = readLineTree(string(__linetreeDialog.getLigFile().data()),
                                  string(__linetreeDialog.getDtaFile().data()),
                                  string(__linetreeDialog.getSMBPath().data()),
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
ViewGeomSceneGL3::openVegeStar(const QString& filename,bool add)
{
  if(!filename.isEmpty()){
/*#ifndef QT_THREAD_SUPPORT
    if(__reader && __reader->running()){
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
    ScenePtr scene = VegeStarFile::read(filename.latin1(),_errlog);
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
ViewGeomSceneGL3::save()
{
  if(getFilename().isEmpty()){
	saveAs();return; }
  QString extension=getFilename().right(getFilename().length()-getFilename().findRev('.')-1);
  extension= extension.upper();
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
ViewGeomSceneGL3::saveAs()
{
  QString initial = getFilename();
  QString extension=getFilename().right(getFilename().length()-getFilename().findRev('.')-1);
  extension= extension.upper();
  if(!extension.isEmpty() && extension != "GEOM" && extension != "BGEOM" ){
    initial = getFilename().left(getFilename().findRev('.')+1);
    initial += "geom";
  }
  QString filename = ViewFileManager3::getSaveFileName(initial,"",tr("Geom File")+" (*.geom;*.bgeom)",__frame);
  if(!filename.isEmpty()){
    if(saveAsGeom(filename)){
      setFilename(filename);
    }
  }
}

void
ViewGeomSceneGL3::saveAsGeom()
{
  QString shape, geom, mat;
  if(getGeomFilenames(shape,geom, mat)){
	if(saveScene(shape,geom,mat,__scene))
	  setFilename(shape);
  }
}

void
ViewGeomSceneGL3::saveSelection()
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
ViewGeomSceneGL3::saveNotSelection()
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
ViewGeomSceneGL3::saveAsGeom(const QString& filename)
{
  return saveScene(filename,QString::null,QString::null,__scene);
}

bool 
ViewGeomSceneGL3::saveAsGeom(const QString& shape,
						    const QString& geom,
							const QString& mat)
{
  return saveScene(shape,geom,mat,__scene);
}

/*
bool
ViewGeomSceneGL3::saveAs(const QString& filename){
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
    fgeom  << ":include " << _matFilename.latin1() << "\n\n\n";
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
  QFileDialog ( !initial.isEmpty()?QFileInfo(initial).filePath():"", filter , parent, name , modal  ) {
	setSelection(initial);
	setMode(QFileDialog::AnyFile);
	QGroupBox * box = new QGroupBox(2,Qt::Vertical,qApp->translate("ViewGeomSceneGL3","Separated File for"),this);
	__geometry = new QRadioButton(qApp->translate("ViewGeomSceneGL3","Geometry"),box);
	__appearance = new QRadioButton(qApp->translate("ViewGeomSceneGL3","Appearance"),box);
	addRightWidget(box);
  }

  bool geometrySeparated() const {return __geometry->isChecked();}
  bool appearanceSeparated() const {return __appearance->isChecked();}

protected:

  QRadioButton *  __geometry;
  QRadioButton *  __appearance;
};

bool 
ViewGeomSceneGL3::getGeomFilenames(QString& shape,QString& geom, QString& mat)
{
  shape = QString::null;
  QString initial = getFilename();
  QString extension=getFilename().right(getFilename().length()-getFilename().findRev('.')-1);
  extension= extension.upper();
  if(!extension.isEmpty() && extension != "GEOM" && extension != "BGEOM" ){
    initial = getFilename().left(getFilename().findRev('.')+1);
    initial += "geom";
  }
  
  GeomDialog dialog( initial, tr("Geom File")+" (*.geom;*.bgeom)", __frame,tr("Save"));
  if(dialog.exec() != QFileDialog::Rejected) shape = dialog.selectedFile();
  
  if(shape.isEmpty())return false;
  else {
	QString extension=shape.right(shape.length()-shape.findRev('.')-1);
	extension= extension.upper();
	if (extension != "GEOM" && extension != "BGEOM" ) {
	  shape = shape.left(shape.findRev('.')+1);
	  shape += "GEOM";
	}
  }
  extension=shape.right(shape.length()-shape.findRev('.')-1);
  extension= extension.upper();

  if(extension != "BGEOM" &&dialog.geometrySeparated()){
	geom = shape.left(shape.findRev('.')-1);
	geom += "_geom.geom";
  }
  if(extension != "BGEOM" &&dialog.appearanceSeparated()){
	mat = shape.left(shape.findRev('.')+1);
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
ViewGeomSceneGL3::saveScene(const QString& shape,
						   const QString& geom,
						   const QString& mat,
						   PGL(ScenePtr) scene){
  if(shape.isEmpty())return false;
  QString extension=shape.right(shape.length()-shape.findRev('.')-1);
  extension= extension.upper();
  if(extension == "GEOM" ){
    ofstream fshape(shape);
	if(!fshape)return false;
	fshape << " # -Geom File-\n";
	ofstream * fgeom;
	bool separatedgeom = false;
	if(!geom.isEmpty()){
	  fgeom = new ofstream(geom);
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
	  fmat = new ofstream(mat);
	  if(!*fmat)fmat = &fshape;
	  else {
        *fmat << " # -Appearance File-\n";
	    separatedmat = true;
	  }
	}
	else fmat = &fshape;

    Printer _printer(fshape,*fgeom,*fmat);
    _printer.header(fshape,shape,tr("File Generated with PlantGL 3D Viewer").latin1());
    if(separatedgeom){
      _printer.header(*fgeom,geom,tr("File Generated with PlantGL 3D Viewer").latin1());
	  fshape  << ":include " << geom.latin1() << "\n\n\n";
	}
    if(separatedmat){
      _printer.header(*fmat,mat,tr("File Generated with PlantGL 3D Viewer").latin1());
	  fshape  << ":include " << mat.latin1() << "\n\n\n";
	}
    return scene->apply(_printer);
  }
  else if(extension == "BGEOM" ){
    return BinaryPrinter::print(__scene,string(shape.data()),tr("File Generated with PlantGL 3D Viewer").latin1());
  }
  else return false;
}


/* ----------------------------------------------------------------------- */
void
ViewGeomSceneGL3::saveAsPovRay()
{
  QString filename = ViewFileManager3::getSaveFileName(getFilename(),"pov",tr("PovRay File")+" (*.pov)",__frame);
  if(!filename.isEmpty()){
    saveAsPovRay(filename);
  }
}

void
ViewGeomSceneGL3::saveAsVegeStar()
{
  QString filename = ViewFileManager3::getSaveFileName(getFilename(),"vgx",tr("VegeStar File")+" (*.vgx;*.vgs)",__frame);
  if(!filename.isEmpty()){
    saveAsVegeStar(filename);
  }
}

void
ViewGeomSceneGL3::saveAsAmapSymbol()
{
  QString filename = ViewFileManager3::getSaveFileName(getFilename(),"smb",tr("Amap Symbol")+" (*.smb)",__frame);
  if(!filename.isEmpty()){
    saveAsAmapSymbol(filename);
  }
}

void 
ViewGeomSceneGL3::saveAsLinetree(){
  QString filename = getFilename();
  if(filename.isEmpty())saveAsLinetree(QString(""));
  else {
	  QString extension=filename.right(filename.length()-filename.findRev('.')-1);
	  extension= extension.upper();    
	  if (extension != "LIG") {
		  filename = filename.left(filename.findRev('.')+1);
		  filename += "lig";
	  }
	  saveAsLinetree(filename);
  }
}

bool 
ViewGeomSceneGL3::saveAsLinetree(const QString& filename){
  ViewReadLinetree3 __linetreeDialog(false);
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
ViewGeomSceneGL3::saveAsPovRay(const QString& filename)
{
  ofstream _stream(filename);
  if(_stream){
    Tesselator _tess;
    PovPrinter _printer(_stream,_tess);
    _printer.header(tr("File Generated with PlantGL 3D Viewer").latin1());
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
		ViewGLFrame3 * f = dynamic_cast<ViewGLFrame3 *>(__frame);
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
ViewGeomSceneGL3::saveAsVegeStar(const QString& filename)
{
  ofstream _stream(filename);
  if(_stream){
    Tesselator _tess;
    VgstarPrinter _printer(_stream,_tess);
    _printer.header(tr("File Generated with PlantGL 3D Viewer").latin1());
    __scene->apply(_printer);
    setFilename(filename);
    return true;
  }
  else return false;
}


bool
ViewGeomSceneGL3::saveAsAmapSymbol(const QString& filename)
{
  Shape3DPtr shape = __scene->getAt(0);
  AmapTranslator translator(__discretizer);
  shape->apply(translator);
  AmapSymbolPtr result = translator.getSymbol();

  if(result) {
	  beofstream _stream(filename.latin1());
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
ViewGeomSceneGL3::saveAsLinetree(const QString& lig,
								const QString& dta,
								const QString& smb,
								const QString& key,
								bool bigendian){
	bofstream * ligstream;
	if (bigendian) ligstream = new beofstream(lig.data());
	else ligstream = new leofstream(lig.data());
	if(!*ligstream) return false;
	ofstream dtastream(dta.data());
	if(!dtastream) return false;
	AmapTranslator t(__discretizer);
	LinetreePrinter printer(*ligstream,dtastream,smb.data(),t);
	printer.header(key.data());
	if(__scene->apply(printer)){
		setFilename(lig);
		QString inf = lig.left(lig.findRev('.')+1);
		inf += "inf";
		ofstream infstream(inf.data());
		if(infstream){
			printer.printInf(infstream,
							 __bbox->getUpperRightCorner(),
							 __bbox->getLowerLeftCorner());
		}
		QString cfg = QFileInfo(lig).dirPath(true);
		string _cfg;
		if(!printer.printCfg(cfg.data(),".",smb.data(),_cfg)){
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
ViewGeomSceneGL3::saveAsVrml()
{
  QString filename = ViewFileManager3::getSaveFileName(getFilename(),"wrl",tr("Vrml File")+" (*.wrl)",__frame);
  ofstream _stream(filename);
  if(_stream){
    VrmlPrinter _printer(_stream,__discretizer);
    _printer.header(tr("File Generated with PlantGL 3D Viewer").latin1());
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
		ViewGLFrame3 * f = dynamic_cast<ViewGLFrame3 *>(__frame);
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
ViewGeomSceneGL3::saveAsPly()
{  
  QString filename = ViewFileManager3::getSaveFileName(getFilename(),"ply",tr("Ply File")+ " (*.ply)",__frame);
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
    PlyPrinter::print(__scene,t,string(filename.data()),
	  tr("File Generated with PlantGL 3D Viewer").latin1(),_format);
  }
}



/* ----------------------------------------------------------------------- */


bool
ViewMultiGeomSceneGL3::openGeomFiles(const QString& filename,const QString& filename2)
{
  if(!filename.isEmpty() && !filename2.isEmpty()){
#ifdef GEOM_THREAD
    if(__reader && __reader->running()){
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
      __reader = new ViewMultiGeomReader3(filename,
                                         filename2,
                                         this);
      __reader->start();
      return true;
    }
#else
    stringstream _errlog(ios::out) ;
    ScenePtr scene1 = ScenePtr(new Scene(filename.latin1(),"",_errlog));
    ScenePtr scene2 = ScenePtr(new Scene(filename2.latin1(),"",_errlog));
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
ViewMultiGeomSceneGL3::openGeomFiles()
{
  QString filename = QFileDialog::getOpenFileName(getFilename(),tr("Geom File")+" (*.geom;*.bgeom)",
                                                  __frame,"Open Geom",tr("Open GEOM File"));
  if(!filename.isEmpty()){
    QString filename2 = QFileDialog::getOpenFileName(getFilename(),tr("Geom File")+" (*.geom;*.bgeom)",
                                                     __frame,"Open Geom",tr("Open GEOM File"));
    if(!filename.isEmpty()){
      openGeomFiles(filename,filename2);
    }
  }
}


bool
ViewMultiGeomSceneGL3::addOpenEntries(QPopupMenu * menu)
{
  QPixmap openIcon(ViewerIcon3::icon_fileopen );
  menu->insertItem( openIcon, tr("Open &Geom File"),
                       this,SLOT(openGeomFile()),CTRL+Key_G);
  menu->insertItem( openIcon, tr("&Add Geom File"),
                       this,SLOT(addGeomFile()));
  menu->insertItem( openIcon, tr("Open &2 Geom File"),
                    this,SLOT(openGeomFiles()));
  return true;
}

/* ----------------------------------------------------------------------- */

void
ViewGeomSceneGL3::exportFred()
{
/*  QString initial = getFilename();
  QString extension=getFilename().right(getFilename().length()-getFilename().findRev('.')-1);
  extension= extension.upper();
  if(extension != "GEOM" ){
    initial = getFilename().left(getFilename().findRev('.')+1);
    initial += "geom";
  }
  QString filename = ViewFileManager3::getSaveFileName(initial,"","Geom Files (*.geom;*.bgeom)",__frame);
  if(!filename.isEmpty()){
    exportFred(filename);
  }*/
}

// #include "geom_asymmetrichull.h"

bool
ViewGeomSceneGL3::exportFred(const QString& filename)
{
/*  AsymmetricHullPtr asymmetricHull = AsymmetricHullPtr::Cast(ShapePtr::Cast(__scene->getAt(0))->getGeometry());
  if(!asymmetricHull)return false;
  const Vector3& _botPoint = asymmetricHull->getBottom();
  const Vector3& _topPoint = asymmetricHull->getTop();
  const real_t& _shapeBot = asymmetricHull->getBottomShape();
  const real_t& _shapeTop = asymmetricHull->getTopShape();
  uint32_t _slices = asymmetricHull->getSlices();
  uint32_t _stacks = asymmetricHull->getStacks();

  uint32_t _totalSlices = _slices * 4;
  uint32_t _totalStacks = _stacks * 2;

  uint32_t _ringCount = _totalStacks - 1;     // number of rings of points
  uint32_t _bot = _totalSlices * _ringCount;  // index of the lower point
  uint32_t _top = _bot + 1;                   // index of the upper point

  Point3ArrayPtr * pointSet = new Point3ArrayPtr[_totalSlices + 1];
  {
	for(uint32_t i = 0 ; i < _totalSlices ; i++){
	  pointSet[i] = Point3ArrayPtr(new Point3Array(_totalStacks));
	  pointSet[i]->setAt(0,_botPoint);
	  pointSet[i]->setAt(_totalStacks-1,_topPoint);
	}
  }
  pointSet[_totalSlices] = Point3ArrayPtr(new Point3Array(_totalSlices+1));

  uint32_t _cur = 0;
  uint32_t _next = _ringCount;

  uint32_t _pointCount = 0;
  uint32_t _indexCount = 0;

  real_t _azStep = real_t(GEOM_TWO_PI) / _totalSlices;
  real_t _azIter = 0;

  real_t _radius1;
  real_t _radius2;
  real_t _deltaZ;
  real_t _zStep;
  real_t _zIter;


  // Constructs quadrats by quadrats
  for (uint32_t _q = 0; _q < 4; _q++) {

    switch(_q) {
    case 0:
      _radius1 = asymmetricHull->getPosXRadius();
      _radius2 = asymmetricHull->getPosYRadius();
      _zIter = asymmetricHull->getPosXHeight();
      _deltaZ = asymmetricHull->getPosYHeight() - _zIter;
      _zStep = _deltaZ / _slices;
      break;
    case 1:
      _radius1 = asymmetricHull->getNegXRadius();
      _radius2 = asymmetricHull->getPosYRadius();
      _zIter = asymmetricHull->getPosYHeight();
      _deltaZ = asymmetricHull->getNegXHeight() - _zIter;
      _zStep = _deltaZ / _slices;
      break;
    case 2:
      _radius1 = asymmetricHull->getNegXRadius();
      _radius2 = asymmetricHull->getNegYRadius();
      _zIter = asymmetricHull->getNegXHeight();
      _deltaZ = asymmetricHull->getNegYHeight() - _zIter;
      _zStep = _deltaZ / _slices;
      break;
    default:
      _radius1 = asymmetricHull->getPosXRadius();
      _radius2 = asymmetricHull->getNegYRadius();
      _zIter = asymmetricHull->getNegYHeight();
      _deltaZ = asymmetricHull->getPosXHeight() - _zIter;
      _zStep = _deltaZ / _slices;
    };

    for (uint32_t _i = 0; _i < _slices; _i++) {

      // Computes the peripheral point
      real_t _cosAz = cos(_azIter);
      real_t _sinAz = sin(_azIter);
      Vector3 _Pi(_cosAz * _radius1,_sinAz * _radius2,_zIter);

      // Constructs the Lower part

      Vector3::Cylindrical _PiBot(_botPoint - _Pi);
      real_t _rStepBot = _PiBot.radius / _stacks;
      real_t _rIterBot = 0;

      for (uint32_t _j = 0; _j < _stacks - 1; _j++) {
        _rIterBot += _rStepBot;

        Vector3::Cylindrical _PijBot(_rIterBot,
                                     _PiBot.theta,
                                     _PiBot.z * pow(_rIterBot / _PiBot.radius,
                                                    _shapeBot));

        Vector3 _Pij(_botPoint - Vector3(_PijBot));

        pointSet[_slices*_q+_i]->setAt(_j+1,_Pij);

      };

      // Adds the peripheral point
      pointSet[_slices*_q+_i]->setAt(_stacks-1,_Pi);
      pointSet[_totalSlices]->setAt(_slices*_q+_i,_Pi);

      // Constructs the upper part
      Vector3::Cylindrical _PiTop(_topPoint - _Pi);
      real_t _rStepTop = _PiTop.radius / _stacks;
      real_t _rIterTop = _PiTop.radius;

      for (uint32_t _j2 = 0; _j2 < _stacks - 1; _j2++) {
        _rIterTop -= _rStepTop;

        Vector3::Cylindrical _PijTop(_rIterTop,
                                     _PiTop.theta,
                                     _PiTop.z * pow(_rIterTop / _PiTop.radius,
                                                    _shapeTop));
        Vector3 _Pij(_topPoint - Vector3(_PijTop));

        pointSet[_slices*_q+_i]->setAt(_stacks+_j2,_Pij);

      };
      _azIter += _azStep;
      _zIter += _zStep;
    };

  };

  pointSet[_totalSlices]->setAt(_totalSlices,pointSet[_totalSlices]->getAt(0));

  ScenePtr scene(new Scene());
  AppearancePtr mat(new Material(Color3::BLACK));
  { 
	for(uint32_t i = 0 ; i < _totalSlices + 1; i++){
	  scene->add(Shape(new Polyline(pointSet[i]),mat));
	}
  }
  delete [] pointSet;

  ofstream fgeom(filename.latin1());
  Printer _printer(fgeom,fgeom,fgeom);
  _printer.header(fgeom,filename,"File Generated with Geom 3D Viewer");
  return scene->apply(_printer);*/
	return false;
}
