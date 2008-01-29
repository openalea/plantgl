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

#include "editgeomscenegl.h"


/// GEOM
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/scene/inline.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/scenegraph/appearance/material.h>

/// Action
#include <plantgl/algo/base/surfcomputer.h>
#include <plantgl/algo/base/volcomputer.h>
#include <plantgl/algo/base/statisticcomputer.h>
#include <plantgl/algo/base/polygoncomputer.h>
#include <plantgl/algo/fitting/fit.h>
#include "qgeomlistview.h"

/// Viewer
#include "../base/util_qwidget.h"
#include "interface/approximation.h"
#include "util_qstring.h"
#include "geomevent.h"
#include "materialeditor.h"

#include <plantgl/tool/util_string.h>

/// Qt
#include <qpopupmenu.h>
#include <qframe.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qslider.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qapplication.h>

#ifdef QT_THREAD_SUPPORT
#ifndef _DEBUG
#define GEOM_THREAD
#endif
#endif

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;
using namespace STDEXT;

ViewEditMatDialog3::ViewEditMatDialog3(QWidget * parent, 
	      const char * name, 
	      bool modal, 
	      WFlags f) : 
  ViewDialog3(parent,name,modal,f),
  __matedit(NULL),
  __appe(0){
  __matedit = new MaterialEditor3(this);
  QVBoxLayout * l1 = new QVBoxLayout(this);
  l1->addWidget(__matedit);
  QHBoxLayout * l = new QHBoxLayout(l1);
  l->addSpacing(__matedit->width()/3);
  QPushButton * b = new QPushButton(tr("&Import Clipboard"),this,"ImportMatButton");
  QObject::connect(b,SIGNAL(pressed()),this,SLOT(importClipboard()));
  l->addWidget(b);
  b = new QPushButton(tr("&Apply"),this,"ApplyButton");
  QObject::connect(b,SIGNAL(pressed()),this,SLOT(apply()));
  l->addWidget(b);
  b = new QPushButton(tr("&Reset"),this,"ResetButton");
  QObject::connect(b,SIGNAL(pressed()),this,SLOT(reset()));
  l->addWidget(b);
  b = new QPushButton(tr("&Ok"),this,"OkButton");
  QObject::connect(b,SIGNAL(pressed()),this,SLOT(ok()));
  l->addWidget(b);
  b = new QPushButton(tr("&Cancel"),this,"CancelButton");
  QObject::connect(b,SIGNAL(pressed()),this,SLOT(cancel()));
  l->addWidget(b);
}

void 
ViewEditMatDialog3::setMaterial(PGL(MaterialPtr) appe)
{
  __appe = appe;
  __default = new Material(*appe);
  __matedit->setMaterial(new Material(*appe));
}

void 
ViewEditMatDialog3::setClipboardMaterial(PGL(AppearancePtr)* clipboard)
{
	__clipboard = clipboard;
}

void
ViewEditMatDialog3::apply(){
  (*__appe) = *(__matedit->getMaterial());
  emit valueChanged();
}

void
ViewEditMatDialog3::reset(){
  __matedit->setMaterial(new Material(*__default));
  emit valueChanged();
}

void
ViewEditMatDialog3::ok(){
  apply();
  __appe = 0;
  accept();
}

void
ViewEditMatDialog3::cancel(){
  __appe = 0;
  reject();
}

void ViewEditMatDialog3::importClipboard()
{
	if(__clipboard && __clipboard->isValid())
	{
		MaterialPtr clipboard;
		if (clipboard.cast(*__clipboard))
		__matedit->setMaterial(new Material(*clipboard));
		// emit valueChanged();
	}
}


/* ----------------------------------------------------------------------- */

ViewEditGeomSceneGL3::ViewEditGeomSceneGL3(ViewCameraGL3 * camera,
										 ViewLightGL3 * light,
										 QGLWidget * parent, 
										 const char * name):
ViewMultiGeomSceneGL3(camera,light,parent,name),
__appeclipboard(0),
__mateditor(0){
}

ViewEditGeomSceneGL3::~ViewEditGeomSceneGL3(){
}

void
ViewEditGeomSceneGL3::clear(){
  __appeclipboard = 0;
  ViewMultiGeomSceneGL3::clear();
}


/* ----------------------------------------------------------------------- */

bool
ViewEditGeomSceneGL3::addEditEntries(QPopupMenu * menu)
{
  menu->insertItem( tr("Edit Material"),
                    this,SLOT(editMaterial()));
  menu->insertItem( tr("Dissociate Material"),
                    this,SLOT(dissociateMaterial()));
  menu->insertSeparator();
  menu->insertItem( tr("Copy Material"),
                    this,SLOT(copyMaterial()));
  menu->insertItem( tr("Paste Material"),
                    this,SLOT(pasteMaterial()),CTRL+Key_V);
  menu->insertSeparator();
  ViewMultiGeomSceneGL3::addEditEntries(menu);
  return true;
}


void 
ViewEditGeomSceneGL3::editMaterial()
{
  if(__scene->getSize() != 1){
	if(__selectedShapes.empty()){
	  QMessageBox::warning(__frame,tr("GEOM Error"),
		tr("Material edition can be apply only on selected shapes."),1,0,0);
	  return;
	}
	if(!hasSameMaterial()){
	  QMessageBox::warning(__frame,tr("GEOM Error"),
		tr("Material edition can be apply on one material."),1,0,0);
	  return;
	}
  }
  AppearancePtr appe = getSelectedAppearance();
  MaterialPtr mat;
  if(mat.cast(appe)){
	if(!__mateditor){
	  __mateditor = new ViewEditMatDialog3(__frame,"Material Editor");
	  __mateditor->setClipboardMaterial(&__appeclipboard);
	  QObject::connect(__mateditor,SIGNAL(valueChanged()),this,SLOT(applyEdition()));
	}
	__mateditor ->setMaterial(mat);
	if(!__mateditor->isVisible())
	  __mateditor->show();
  }
}

void
ViewEditGeomSceneGL3::applyEdition(){
  __renderer.clearSceneList();
  emit valueChanged();
}

void 
ViewEditGeomSceneGL3::dissociateMaterial()
{
  if(__selectedShapes.empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("At least one shape must be selected to dissociate material."),1,0,0);
	return;
  }
  if(!hasSameMaterial()){
    if(QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("The Material of the first shape will be copy on all selected shapes."),
	  QMessageBox::tr("Ok"),
	  QMessageBox::tr("Cancel"))!=0)
	return;
  }
  AppearancePtr appe = getSelectedAppearance();
  MaterialPtr mat = MaterialPtr::Cast(appe);
  if(!mat){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  QString("Cannot find selected material."),1,0,0);
	return;
  }
  mat = MaterialPtr(new Material(*(mat)));
  mat->setName("APP_"+number(mat->getId()));
  for( STDEXT::hash_map<uint_t,Shape3DPtr>::const_iterator _it = __selectedShapes.begin();
	   _it !=__selectedShapes.end(); _it++){
	ShapePtr shape = ShapePtr::Cast(_it->second);
	if(shape)shape->getAppearance() = mat;
	else {
	  InlinePtr in = InlinePtr::Cast(_it->second);
	  if(in){
		setAppearance(in->getScene(),AppearancePtr(mat));
	  }
	}
  }
  __renderer.clearSceneList();
  valueChanged();
}

void ViewEditGeomSceneGL3::setAppearance(ScenePtr scene,AppearancePtr appe) const
{
  if(!scene)return;
  for(Scene::const_iterator _it = scene->getBegin(); _it !=scene->getEnd(); _it++){
	ShapePtr shape = ShapePtr::Cast(*_it);
	if(shape) shape->appearance = appe;
	else {
	  InlinePtr in = InlinePtr::Cast(*_it);
	  if(in)setAppearance(in->getScene(),appe);
	}
  }
}

bool 
ViewEditGeomSceneGL3::hasSameMaterial(ScenePtr scene,AppearancePtr appe) const
{
  if(scene.isNull() ||appe.isNull())return true;
  for(Scene::const_iterator _it = scene->getBegin(); _it !=scene->getEnd(); _it++){
	ShapePtr shape = ShapePtr::Cast(*_it);
	if(shape.isValid()){
	  if(shape->getAppearance() != appe)return false;
	}
	else {
	  InlinePtr in = InlinePtr::Cast(*_it);
	  if(in){
		if(!hasSameMaterial(in->getScene(),appe))return false;
	  }
	  else return false;
	}
  }
  return true;
}

bool 
ViewEditGeomSceneGL3::hasSameMaterial() const
{
  AppearancePtr mat = getSelectedAppearance();
  if(!mat)return false;
  for(STDEXT::hash_map<uint_t,Shape3DPtr>::const_iterator _it = __selectedShapes.begin();
	  _it !=__selectedShapes.end(); _it++){
	ShapePtr shape = ShapePtr::Cast(_it->second);
	if(shape.isValid()){
	  if(shape->getAppearance() != mat)return false;
	}
	else {
	  InlinePtr in = InlinePtr::Cast(_it->second);
	  if(in){
		if(!hasSameMaterial(in->getScene(),mat))return false;
	  }
	  else return false;
	}
  }
  return true;
}

AppearancePtr
ViewEditGeomSceneGL3::getSelectedAppearance() const
{
  if(__selectedShapes.empty()) {
	if (__scene->getSize() == 1) return getSelectedAppearance(__scene);
	return NULL;
  }
  for(STDEXT::hash_map<uint_t,Shape3DPtr>::const_iterator _it = __selectedShapes.begin();
    _it !=__selectedShapes.end(); _it++){
	ShapePtr shape = ShapePtr::Cast(_it->second);
	if(shape) {
	  if(shape->getAppearance())return shape->getAppearance();
	}
	else {
	  InlinePtr in = InlinePtr::Cast(_it->second);
	  if(in){
		AppearancePtr appe = getSelectedAppearance(in->getScene());
		if(appe)return appe;
	  }
	}
  }
  return NULL;
}

AppearancePtr 
ViewEditGeomSceneGL3::getSelectedAppearance(ScenePtr scene) const
{
  if(scene.isNull() || scene->isEmpty()) return NULL;
  for(Scene::const_iterator _it = scene->getBegin(); _it !=scene->getEnd(); _it++){
	ShapePtr shape = ShapePtr::Cast(*_it);
	if(shape) {
	  if(shape->getAppearance())return shape->getAppearance();
	}
	else {
	  InlinePtr in = InlinePtr::Cast(*_it);
	  if(in){
		AppearancePtr appe = getSelectedAppearance(in->getScene());
		if(appe)return appe;
	  }
	}
  }
  return NULL;
}

void
ViewEditGeomSceneGL3::copyMaterial(){
  if(__selectedShapes.empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("One shape must be selected to copy material."),1,0,0);
	return;
  }
  if(!hasSameMaterial()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("Cannot copy multiple Material."),QMessageBox::tr("Ok"));
	return;
  }
  __appeclipboard = getSelectedAppearance();
}

void
ViewEditGeomSceneGL3::pasteMaterial(){
  if(!__appeclipboard){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("No material available."),1,0,0);
  }
  if(__selectedShapes.empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("At least one shape must be selected to paste material."),1,0,0);
  }
  
  for( hash_map<uint_t,Shape3DPtr>::iterator _it = __selectedShapes.begin();
	  _it !=__selectedShapes.end(); _it++){
	ShapePtr shape = ShapePtr::Cast(_it->second);
	if(shape)shape->getAppearance() = __appeclipboard;
	else {
	  InlinePtr in = InlinePtr::Cast(_it->second);
	  setAppearance(in->getScene(),__appeclipboard);
	}
  }
  __renderer.clearSceneList();
  valueChanged();
}

/* ----------------------------------------------------------------------- */

ViewMultiscaleEditGeomSceneGL3::ViewMultiscaleEditGeomSceneGL3(ViewCameraGL3 * camera,
										 ViewLightGL3 * light,
										 QGLWidget * parent, 
										 const char * name):
ViewEditGeomSceneGL3(camera,light,parent,name),
__appform(0),
__matmacro(new Material(*MaterialPtr::Cast(Material::DEFAULT_MATERIAL))){
  __matmacro->setName("");
  __matmacro->getTransparency() = 0.6;
  
}

ViewMultiscaleEditGeomSceneGL3::~ViewMultiscaleEditGeomSceneGL3(){
}

bool 
ViewMultiscaleEditGeomSceneGL3::addEditEntries(QPopupMenu * menu)
{
  menu->insertItem( tr("Fit Geometry"),
                    this,SLOT(editMultiScaleGeometry()));
  menu->insertSeparator();
  ViewEditGeomSceneGL3::addEditEntries(menu);
  return true;

}


void
ViewMultiscaleEditGeomSceneGL3::editMultiScaleGeometry(){
  if(__scene.isNull() || __scene->isEmpty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("No Geometry to Fit."),1,0,0);
	return;
  }
  if(!__appform){
	__appform = new ViewApproximationForm(__frame,"ApproximationForm",false);
	vector<string> algos = Fit::getClassNames();
	QStringList listalgos;
	for(vector<string>::iterator _it = algos.begin(); _it != algos.end(); _it++)
	  listalgos.append(_it->c_str());
	__appform->AlgorithmBox->insertStringList(listalgos);
	QObject::connect(__appform->OkButton,SIGNAL(clicked()),
					 this,SLOT(computeMultiScaleGeometry()));
	QObject::connect(__appform->CancelButton,SIGNAL(clicked()),
					 __appform,SLOT(reject()));
	QObject::connect(__appform->MaterialButton,SIGNAL(clicked()),
					 this,SLOT(editMacroMaterial()));
	QObject::connect(__appform->SceneButton,SIGNAL(toggled(bool)),
					 __appform->Rest,SLOT(setDisabled(bool)));
  }
  if(__selectedShapes.empty()){
	__appform->SceneButton->setChecked(true);
	__appform->SelectionButton->setEnabled(false);
	__appform->ExeptButton->setEnabled(false);
	__appform->Rest->setChecked(false);
	__appform->Rest->setEnabled(false);
  }
  else {
	__appform->SelectionButton->setEnabled(true);
	__appform->SelectionButton->setChecked(true);
	__appform->ExeptButton->setEnabled(true);
	__appform->Rest->setEnabled(true);
  }
  __appform->show();
}

void
ViewMultiscaleEditGeomSceneGL3::computeMultiScaleGeometry(){
  if(!__appform)return;
  __appform->hide();
  ScenePtr scene;
  if(__appform->SceneButton->isChecked())scene = __scene;
  else if (__appform->SelectionButton->isChecked())scene = getSelection();
  else scene = getNotSelection();
  if(scene.isNull()||scene->isEmpty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("No Geometry to Fit."),1,0,0);
	return;
  }
  Point3ArrayPtr points(new Point3Array);
  for(Scene::iterator _it = scene->getBegin();_it != scene->getEnd(); _it++){
	if((*_it)->apply(__discretizer)){
	  *points += *(__discretizer.getDiscretization()->getPointList());
	}
  }
  if(points->isEmpty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("No Geometry to Fit."),1,0,0);
	return;
  }
  Fit approx(points);
  GeometryPtr result = approx.use(std::string(__appform->AlgorithmBox->currentText().latin1()));
  if(!result){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("Error during Fit computation."),1,0,0);
	return;
  }
  ScenePtr newscene(new Scene );
  if(__appform->InputGeometry->isChecked()&&(
	 __appform->Rest->isChecked()||__appform->SceneButton->isChecked()))
	newscene->merge(__scene);
  else {
	if(__appform->InputGeometry->isChecked())newscene->merge(scene);
	if(__appform->Rest->isChecked() && 
	  !__appform->SceneButton->isChecked()){
	  if (__appform->SelectionButton->isChecked())
		newscene->merge(getNotSelection());
	  else newscene->merge(getSelection());
	}
  }
  newscene->add(Shape3DPtr(new Shape(result,AppearancePtr(new Material(*__matmacro)))));
  GeomSceneChangeEvent3 * e = new GeomSceneChangeEvent3(newscene,NULL,NULL);
  QApplication::postEvent(this,e);

}

void 
ViewMultiscaleEditGeomSceneGL3::editMacroMaterial(){
  if(!__mateditor){
	__mateditor = new ViewEditMatDialog3(__frame,"Material Editor");
    __mateditor->setClipboardMaterial(&__appeclipboard);
	QObject::connect(__mateditor,SIGNAL(valueChanged()),this,SLOT(applyEdition()));
  }
  __mateditor ->setMaterial(__matmacro);
  if(!__mateditor->isVisible())
	__mateditor->show();
}


void
ViewMultiscaleEditGeomSceneGL3::updateMaterial(const QColor& col){
  if(!__appform)return;
  QPalette pal = __appform->MaterialButton->palette();
  QColorGroup c = pal.active();
  c.setColor(QColorGroup::Button,col);
  pal.setActive(c);
  __appform->MaterialButton->setPalette(pal);
  __appform->MaterialButton->update();
}

#define GCOL2QCOL(col) QColor(col.getRed(),col.getGreen(),col.getBlue())

void
ViewMultiscaleEditGeomSceneGL3::applyEdition(){
  if(__mateditor && __mateditor->getMaterial().equal(__matmacro)){
	updateMaterial(GCOL2QCOL(__matmacro->getDiffuseColor()));
  }
  else {
	ViewEditGeomSceneGL3::applyEdition();
  }
}
