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
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qframe.h>
    #include <QtWidgets/qlineedit.h>
    #include <QtWidgets/qlabel.h>
    #include <QtWidgets/qtabwidget.h>
    #include <QtWidgets/qslider.h>
    #include <QtWidgets/qcombobox.h>
    #include <QtWidgets/qradiobutton.h>
    #include <QtWidgets/qmessagebox.h>
    #include <QtWidgets/qlayout.h>
    #include <QtWidgets/qapplication.h>
#else
    #include <QtGui/qmenu.h>
    #include <QtGui/qframe.h>
    #include <QtGui/qlineedit.h>
    #include <QtGui/qlabel.h>
    #include <QtGui/qtabwidget.h>
    #include <QtGui/qslider.h>
    #include <QtGui/qcombobox.h>
    #include <QtGui/qradiobutton.h>
    #include <QtGui/qmessagebox.h>
    #include <QtGui/qlayout.h>
    #include <QtGui/qapplication.h>
#endif


#ifdef QT_THREAD_SUPPORT
#ifndef _DEBUG
#define GEOM_THREAD
#endif
#endif

PGL_USING_NAMESPACE
using namespace std;
using namespace STDEXT;

ViewEditMatDialog::ViewEditMatDialog(QWidget * parent,
          const char * name,
          bool modal,
          Qt::WindowFlags f) :
  ViewDialog(parent,name,modal,f),
  __matedit(NULL),
  __appe(){
  __matedit = new MaterialEditor(this);
  QVBoxLayout * l1 = new QVBoxLayout(this);
  l1->addWidget(__matedit);
  QHBoxLayout * l = new QHBoxLayout(this);
  l->addSpacing(__matedit->width()/3);
   __clipboardButton = new QPushButton(tr("&Import Clipboard"),this);
  QObject::connect(__clipboardButton,SIGNAL(pressed()),this,SLOT(importClipboard()));
  l->addWidget(__clipboardButton);
  __clipboardButton->hide();
  QPushButton * b = new QPushButton(tr("&Apply"),this);
  QObject::connect(b,SIGNAL(pressed()),this,SLOT(apply()));
  l->addWidget(b);
  b = new QPushButton(tr("&Reset"),this);
  QObject::connect(b,SIGNAL(pressed()),this,SLOT(reset()));
  l->addWidget(b);
  b = new QPushButton(tr("&Ok"),this);
  QObject::connect(b,SIGNAL(pressed()),this,SLOT(ok()));
  l->addWidget(b);
  b = new QPushButton(tr("&Cancel"),this);
  QObject::connect(b,SIGNAL(pressed()),this,SLOT(cancel()));
  l->addWidget(b);
  l1->addLayout(l);
}

void
ViewEditMatDialog::setMaterial(PGL(MaterialPtr) appe)
{
  __appe = appe;
  __default = MaterialPtr(new Material(*appe));
  __matedit->setMaterial(MaterialPtr(new Material(*appe)));
}

void
ViewEditMatDialog::setClipboardMaterial(PGL(AppearancePtr)* clipboard)
{
    __clipboard = clipboard;
    __clipboardButton->show();
}

void
ViewEditMatDialog::apply(){
  (*__appe) = *(__matedit->getMaterial());
  emit valueChanged();
}

void
ViewEditMatDialog::reset(){
  __matedit->setMaterial(MaterialPtr(new Material(*__default)));
  emit valueChanged();
}

void
ViewEditMatDialog::ok(){
  apply();
  __appe = MaterialPtr();
  accept();
}

void
ViewEditMatDialog::cancel(){
  __appe = MaterialPtr();
  reject();
}

void ViewEditMatDialog::importClipboard()
{
    if(__clipboard && *__clipboard)
    {
        MaterialPtr clipboard;
        if ((clipboard = dynamic_pointer_cast<Material>(*__clipboard)))
        __matedit->setMaterial(MaterialPtr(new Material(*clipboard)));
        // emit valueChanged();
    }
}

/* ----------------------------------------------------------------------- */

MaterialPtr getMaterialFromDialog(QWidget * parent,
                                  const char * caption,
                                  MaterialPtr initial)
{
    ViewEditMatDialog dialog(parent,caption,true);
    dialog.setWindowTitle(caption);
    if(initial)   dialog.setMaterial(initial->casted_deepcopy<Material>());
    else dialog.setMaterial(dynamic_pointer_cast<Material>(Material::DEFAULT_MATERIAL));
    if (dialog.exec() == QDialog::Accepted) return dialog.getMaterial();
    else return MaterialPtr();
}

int editMaterialInDialog(MaterialPtr initial,QWidget * parent, const char * caption)
{
    ViewEditMatDialog dialog(parent,caption,true);
    dialog.setWindowTitle(caption);
    if(initial)dialog.setMaterial(initial);
    else return -1;
    return dialog.exec();
}


/* ----------------------------------------------------------------------- */

ViewEditGeomSceneGL::ViewEditGeomSceneGL(ViewCameraGL * camera,
                                         ViewLightGL * light,
                                         QGLWidget * parent,
                                         const char * name):
ViewMultiGeomSceneGL(camera,light,parent,name),
__appeclipboard(),
__mateditor(0){
}

ViewEditGeomSceneGL::~ViewEditGeomSceneGL(){
}

void
ViewEditGeomSceneGL::clear(){
  __appeclipboard = AppearancePtr();
  ViewMultiGeomSceneGL::clear();
}


/* ----------------------------------------------------------------------- */

bool
ViewEditGeomSceneGL::addEditEntries(QMenu * menu)
{
  menu->addAction( tr("Edit Material"),this,SLOT(editMaterial()));
  menu->addAction( tr("Dissociate Material"),this,SLOT(dissociateMaterial()));
  menu->addSeparator();
  menu->addAction( tr("Copy Material"),this,SLOT(copyMaterial()));
  menu->addAction( tr("Paste Material"),this,SLOT(pasteMaterial()),Qt::CTRL+Qt::Key_V);
  menu->addSeparator();
  ViewMultiGeomSceneGL::addEditEntries(menu);
  return true;
}


void
ViewEditGeomSceneGL::editMaterial()
{
  if(__scene->size() != 1){
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
  if((mat = dynamic_pointer_cast<Material>(appe))){
    if(!__mateditor){
      __mateditor = new ViewEditMatDialog(__frame,"Material Editor");
      __mateditor->setClipboardMaterial(&__appeclipboard);
      QObject::connect(__mateditor,SIGNAL(valueChanged()),this,SLOT(applyEdition()));
    }
    __mateditor ->setMaterial(mat);
    if(!__mateditor->isVisible())
      __mateditor->show();
  }
}

void
ViewEditGeomSceneGL::applyEdition(){
  __renderer.clearSceneList();
  emit valueChanged();
}

void
ViewEditGeomSceneGL::dissociateMaterial()
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
  MaterialPtr mat = dynamic_pointer_cast<Material>(appe);
  if(!mat){
    QMessageBox::warning(__frame,tr("GEOM Error"),
      QString("Cannot find selected material."),1,0,0);
    return;
  }
  mat = MaterialPtr(new Material(*(mat)));
  mat->setName("APP_"+number(mat->getId()));
  for( SelectionCache::const_iterator _it = __selectedShapes.begin();
       _it !=__selectedShapes.end(); _it++){
    ShapePtr shape = dynamic_pointer_cast<Shape>(get_item_value(_it));
    if(shape)shape->getAppearance() = mat;
    else {
      InlinePtr in = dynamic_pointer_cast<Inline>(get_item_value(_it));
      if(in){
        setAppearance(in->getScene(),AppearancePtr(mat));
      }
    }
  }
  __renderer.clearSceneList();
  valueChanged();
}

void ViewEditGeomSceneGL::setAppearance(ScenePtr scene,AppearancePtr appe) const
{
  if(!scene)return;
  for(Scene::const_iterator _it = scene->begin(); _it !=scene->end(); _it++){
    ShapePtr shape = dynamic_pointer_cast<Shape>(*_it);
    if(shape) shape->appearance = appe;
    else {
      InlinePtr in = dynamic_pointer_cast<Inline>(*_it);
      if(in)setAppearance(in->getScene(),appe);
    }
  }
}

bool
ViewEditGeomSceneGL::hasSameMaterial(ScenePtr scene,AppearancePtr appe) const
{
  if(!scene || !appe)return true;
  for(Scene::const_iterator _it = scene->begin(); _it !=scene->end(); _it++){
    ShapePtr shape = dynamic_pointer_cast<Shape>(*_it);
    if(shape){
      if(shape->getAppearance() != appe)return false;
    }
    else {
      InlinePtr in = dynamic_pointer_cast<Inline>(*_it);
      if(in){
        if(!hasSameMaterial(in->getScene(),appe))return false;
      }
      else return false;
    }
  }
  return true;
}

bool
ViewEditGeomSceneGL::hasSameMaterial() const
{
  AppearancePtr mat = getSelectedAppearance();
  if(!mat)return false;
  for(SelectionCache::const_iterator _it = __selectedShapes.begin();
      _it !=__selectedShapes.end(); _it++){
    ShapePtr shape = dynamic_pointer_cast<Shape>(get_item_value(_it));
    if(shape){
      if(shape->getAppearance() != mat)return false;
    }
    else {
      InlinePtr in = dynamic_pointer_cast<Inline>(get_item_value(_it));
      if(in){
        if(!hasSameMaterial(in->getScene(),mat))return false;
      }
      else return false;
    }
  }
  return true;
}

AppearancePtr
ViewEditGeomSceneGL::getSelectedAppearance() const
{
  if(__selectedShapes.empty()) {
    if (__scene->size() == 1) return getSelectedAppearance(__scene);
    return AppearancePtr();
  }
  for(SelectionCache::const_iterator _it = __selectedShapes.begin();
    _it !=__selectedShapes.end(); _it++){
    ShapePtr shape = dynamic_pointer_cast<Shape>(get_item_value(_it));
    if(shape) {
      if(shape->getAppearance())return shape->getAppearance();
    }
    else {
      InlinePtr in = dynamic_pointer_cast<Inline>(get_item_value(_it));
      if(in){
        AppearancePtr appe = getSelectedAppearance(in->getScene());
        if(appe)return appe;
      }
    }
  }
  return AppearancePtr();
}

AppearancePtr
ViewEditGeomSceneGL::getSelectedAppearance(ScenePtr scene) const
{
  if(!scene || scene->empty()) return AppearancePtr();
  for(Scene::const_iterator _it = scene->begin(); _it !=scene->end(); _it++){
    ShapePtr shape = dynamic_pointer_cast<Shape>(*_it);
    if(shape) {
      if(shape->getAppearance())return shape->getAppearance();
    }
    else {
      InlinePtr in = dynamic_pointer_cast<Inline>(*_it);
      if(in){
        AppearancePtr appe = getSelectedAppearance(in->getScene());
        if(appe)return appe;
      }
    }
  }
  return AppearancePtr();
}

void
ViewEditGeomSceneGL::copyMaterial(){
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
ViewEditGeomSceneGL::pasteMaterial(){
  if(!__appeclipboard){
    QMessageBox::warning(__frame,tr("GEOM Error"),
      tr("No material available."),1,0,0);
  }
  if(__selectedShapes.empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
      tr("At least one shape must be selected to paste material."),1,0,0);
  }

  for( SelectionCache::iterator _it = __selectedShapes.begin();
      _it !=__selectedShapes.end(); _it++){
    ShapePtr shape = dynamic_pointer_cast<Shape>(get_item_value(_it));
    if(shape)shape->getAppearance() = __appeclipboard;
    else {
      InlinePtr in = dynamic_pointer_cast<Inline>(get_item_value(_it));
      setAppearance(in->getScene(),__appeclipboard);
    }
  }
  __renderer.clearSceneList();
  valueChanged();
}

/* ----------------------------------------------------------------------- */

ViewMultiscaleEditGeomSceneGL::ViewMultiscaleEditGeomSceneGL(ViewCameraGL * camera,
                                         ViewLightGL * light,
                                         QGLWidget * parent,
                                         const char * name):
ViewEditGeomSceneGL(camera,light,parent,name),
__appdialog(0),
__appform(0),
__matmacro(new Material(*dynamic_pointer_cast<Material>(Material::DEFAULT_MATERIAL))){
  __matmacro->setName("");
  __matmacro->getTransparency() = 0.6;
}

ViewMultiscaleEditGeomSceneGL::~ViewMultiscaleEditGeomSceneGL(){
}

bool
ViewMultiscaleEditGeomSceneGL::addEditEntries(QMenu * menu)
{
  menu->addAction( tr("Fit Geometry"), this,SLOT(editMultiScaleGeometry()));
  menu->addSeparator();
  ViewEditGeomSceneGL::addEditEntries(menu);
  return true;

}


void
ViewMultiscaleEditGeomSceneGL::editMultiScaleGeometry(){
  if(!__scene || __scene->empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
      tr("No Geometry to Fit."),1,0,0);
    return;
  }
  if(!__appform){
    __appdialog = new QDialog(__frame);
    __appdialog->setModal(false);
    __appform = new Ui::ViewApproximationForm();
    __appform->setupUi(__appdialog);
    vector<string> algos = Fit::getClassNames();
    QStringList listalgos;
    for(vector<string>::iterator _it = algos.begin(); _it != algos.end(); _it++)
      listalgos.append(_it->c_str());
    __appform->AlgorithmBox->addItems(listalgos);
    QObject::connect(__appform->OkButton,SIGNAL(clicked()),
                     this,SLOT(computeMultiScaleGeometry()));
    QObject::connect(__appform->CancelButton,SIGNAL(clicked()),
                     __appdialog,SLOT(reject()));
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
  __appdialog->show();
}

void
ViewMultiscaleEditGeomSceneGL::computeMultiScaleGeometry(){
  if(!__appform)return;
  __appdialog->hide();
  ScenePtr scene;
  if(__appform->SceneButton->isChecked())scene = __scene;
  else if (__appform->SelectionButton->isChecked())scene = getSelection();
  else scene = getNotSelection();
  if(!scene||scene->empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
      tr("No Geometry to Fit."),1,0,0);
    return;
  }
  Point3ArrayPtr points(new Point3Array);
  for(Scene::iterator _it = scene->begin();_it != scene->end(); _it++){
    if((*_it)->apply(__discretizer)){
      *points += *(__discretizer.getDiscretization()->getPointList());
    }
  }
  if(points->empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
      tr("No Geometry to Fit."),1,0,0);
    return;
  }
  Fit approx(points);
  GeometryPtr result = approx.use( __appform->AlgorithmBox->currentText().toStdString() );
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
  GeomSceneChangeEvent * e = new GeomSceneChangeEvent(newscene,NULL,NULL);
  QApplication::postEvent(this,e);

}

void
ViewMultiscaleEditGeomSceneGL::editMacroMaterial(){
  if(!__mateditor){
    __mateditor = new ViewEditMatDialog(__frame,"Material Editor");
    __mateditor->setClipboardMaterial(&__appeclipboard);
    QObject::connect(__mateditor,SIGNAL(valueChanged()),this,SLOT(applyEdition()));
  }
  __mateditor ->setMaterial(__matmacro);
  if(!__mateditor->isVisible())
    __mateditor->show();
}


void
ViewMultiscaleEditGeomSceneGL::updateMaterial(const QColor& col){
  if(!__appform)return;
  QPixmap colpic(40,30);
  colpic.fill(col);
  __appform->MaterialButton->setIcon(QIcon(colpic));
  /*QPalette pal = __appform->MaterialButton->palette();
  QColorGroup c = pal.active();
  c.setColor(QColorGroup::Button,col);
  pal.setActive(c);
  __appform->MaterialButton->setPalette(pal);
  __appform->MaterialButton->update();*/
}

#define GCOL2QCOL(col) QColor(col.getRed(),col.getGreen(),col.getBlue())

void
ViewMultiscaleEditGeomSceneGL::applyEdition(){
  if(__mateditor && (__mateditor->getMaterial() == __matmacro)){
    updateMaterial(GCOL2QCOL(__matmacro->getDiffuseColor()));
  }
  else {
    ViewEditGeomSceneGL::applyEdition();
  }
}
