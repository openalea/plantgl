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
#include <plantgl/algo/opengl/util_appegl.h>

/// GEOM
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/appearance/appearance.h>
#include <plantgl/scenegraph/geometry/explicitmodel.h>
#include <plantgl/algo/base/wirecomputer.h>
#include <plantgl/algo/base/tesselator.h>

/// Viewer
#include "../base/light.h"
#include "../base/camera.h"
#include "../base/zbuffer.h"

#include "geomevent.h"

/// Qt
#include <QtGui/qslider.h>
#include <QtGui/qpainter.h>
#include <Qt3Support/q3listview.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qapplication.h>
#include <QtGui/qclipboard.h>
#include <QtOpenGL/qgl.h>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtGui/QImage>
#include <QtOpenGL/QGLPixelBuffer>

#include <plantgl/algo/codec/scne_parser.h>

#ifdef QT_THREAD_SUPPORT
#ifndef PGL_DEBUG
#define GEOM_THREAD
#endif
#endif

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;
using namespace STDEXT;
/* ----------------------------------------------------------------------- */

ViewGeomSceneGL::ViewGeomSceneGL(ViewCameraGL * camera,
                                 ViewLightGL * light,
                                 QGLWidget * parent,
                                 const char * name) :
  ViewModalRendererGL(camera,light,parent,name),
  __scene(),
  __discretizer(),
  __renderer(__discretizer,parent),
  __skelComputer(__discretizer),
  __bboxComputer(__discretizer),
  __skelRenderer(__skelComputer),
  __bboxRenderer(__bboxComputer),
  __ctrlPtRenderer(__discretizer),
  __bbox(new BoundingBox(Vector3(-1,-1,-1),Vector3(1,1,1))),
  __selectedShapes(),
  __blending(true)
#ifdef QT_THREAD_SUPPORT
  ,__reader(0)
#endif
{
  __renderingOption[0] = false;
  __renderingOption[1] = false;
  parserVerbose(false);
  __renderer.setSelectionMode(GLRenderer::SceneObjectId);
//  __renderer.setRenderingMode(GLRenderer::Dynamic);

  //  QObject::connect(QApplication::clipboard(),SIGNAL(dataChanged()),
//				   this,SLOT(clipboard()));

}

ViewGeomSceneGL::~ViewGeomSceneGL()
{
}
bool 
ViewGeomSceneGL::useThread()
{
#ifdef GEOM_THREAD
	return true;
#else
	return false;
#endif
}

/// Connect this to a GL Widget.
void ViewGeomSceneGL::connectTo(QGLWidget * glw)
{
	ViewRendererGL::connectTo(glw);
	__renderer.setGLFrame(glw);
}

/* ----------------------------------------------------------------------- */

bool
ViewGeomSceneGL::sceneChangeEvent( ViewSceneChangeEvent * k)
{
	if(k->getSceneType() == GeomSceneChangeEvent::eGeomSceneEvent){
		GeomSceneChangeEvent * event = ( GeomSceneChangeEvent * )k;
		if(event->addition)addScene(ScenePtr(event->scene));
		else setScene(event->scene);
		if(!event->error.isEmpty()){
			error(event->error);
		}
		setFilename(event->file);
		return true;
	}
	else return false;
}

void
ViewGeomSceneGL::clear()
{
  __scene = ScenePtr();
  __bbox= BoundingBoxPtr(new BoundingBox(Vector3(-1,-1,-1),Vector3(1,1,1)));
  setFilename("");
  clearCache();
}

bool ViewGeomSceneGL::isEmpty() const { return is_null_ptr(__scene) || __scene->empty(); }

void
ViewGeomSceneGL::clearCache()
{
  __discretizer.clear();
  __selectedShapes.clear();
  clearDisplayList();
}

void
ViewGeomSceneGL::clearDisplayList()
{
  __renderer.clear();
  __skelComputer.clear();
  __bboxComputer.clear();
  __skelRenderer.clear();
  __bboxRenderer.clear();
  __ctrlPtRenderer.clear();
}

ScenePtr
ViewGeomSceneGL::getScene( ) const
{
  return __scene;
}

std::vector<uint_t>
ViewGeomSceneGL::getSelectionIds() const
{
  std::vector<uint_t> res;
  for(SelectionCache::const_iterator _it = __selectedShapes.begin();
  _it !=__selectedShapes.end(); _it++)
	  res.push_back(get_item_value(_it)->getId());
  return res;
}

uint_t
ViewGeomSceneGL::translateId(uint_t id) const
{
    Shape3DPtr ptr;
    for(Scene::iterator _it = __scene->begin();
        _it != __scene->end(); _it++){
      if( (ptr = dynamic_pointer_cast<Shape3D>(*_it)) && (ptr->SceneObject::getId() == id))
	    return ptr->getId();
    }
	return id;
}


ScenePtr 
ViewGeomSceneGL::getSelection( ) const
{
  ScenePtr scene(new Scene);
  for(SelectionCache::const_iterator _it = __selectedShapes.begin();
  _it !=__selectedShapes.end(); _it++)
	  scene->add(get_item_value(_it));
  return scene;
}

ScenePtr 
ViewGeomSceneGL::getNotSelection( ) const
{
  ScenePtr scene(new Scene);
  uint_t id;
  for(Scene::const_iterator _it = __scene->begin(); _it !=__scene->end(); _it++){
	id = (*_it)->SceneObject::getId();
	if(__selectedShapes.find(id)==__selectedShapes.end())
	  scene->add(*_it);
  }	  
  return scene;
}

const BoundingBoxPtr
ViewGeomSceneGL::getGlobalBoundingBox() const
{
  return __bbox;
}

const BoundingBoxPtr
ViewGeomSceneGL::getSelectionBoundingBox() 
{
  BoundingBoxPtr bbox;
  for(SelectionCache::const_iterator _it = __selectedShapes.begin();
  _it !=__selectedShapes.end(); _it++)
	  if(get_item_value(_it)->apply(__bboxComputer)){
		if(bbox)bbox->extend(__bboxComputer.getBoundingBox());
		else bbox = BoundingBoxPtr(new BoundingBox(*__bboxComputer.getBoundingBox()));
	  }
  return bbox;
}


/* ----------------------------------------------------------------------- */
void
ViewGeomSceneGL::changeDisplayListUse(){
  if(__renderer.getRenderingMode() & GLRenderer::Dynamic){
	__renderer.setRenderingMode(GLRenderer::Normal);
	emit displayList(true);
  }
  else {
	__renderer.setRenderingMode(GLRenderer::Dynamic);
	emit displayList(false);
  }
}

void
ViewGeomSceneGL::useDisplayList(bool b){
  if( getDisplayListUse() != b){
	changeDisplayListUse();
  }
}

bool 
ViewGeomSceneGL::getDisplayListUse() const {
  return !(__renderer.getRenderingMode() & GLRenderer::Dynamic);
}

void 
ViewGeomSceneGL::refreshDisplay() {
  if(__scene)setScene(ScenePtr(__scene));
}

void ViewGeomSceneGL::enableBlending(bool b) { __blending = b; emit valueChanged(); }

void ViewGeomSceneGL::animationChangedEvent(eAnimationFlag a)
{
	if(a == eAnimatedScene) __renderer.setRenderingMode(GLRenderer::DynamicScene);
	else if(a == eAnimatedPrimitives) __renderer.setRenderingMode(GLRenderer::Dynamic);
	else __renderer.setRenderingMode(GLRenderer::Normal);
}

/* ----------------------------------------------------------------------- */
int
ViewGeomSceneGL::addScene( const ScenePtr& scene )
{
  if (!scene){
    QString _mess = "<b>[GeomSceneGL] "+tr("GEOM Error")+" !!</b><br>"+tr("Empty Scene to Add")+"<br>";
	if(!BATCHMODE)
		QMessageBox::critical(__frame,tr("GEOM Error"),_mess,1,0,0);
	else warning(_mess);
    return -1;
  }
  ScenePtr scenunion = scene;
  if(__scene)scenunion->merge(__scene);
  return setScene(scenunion);
}

int
ViewGeomSceneGL::setScene( const ScenePtr& scene )
{
  if (!scene){
	  QString _mess = "<b>[GeomSceneGL] "+tr("GEOM Error")+" !!</b><br>"+tr("Empty Scene")+"<br>";
	  if(!BATCHMODE){
		QMessageBox::critical(__frame,tr("GEOM Error"),_mess,1,0,0);
	  }
	  else warning("*** Warning : "+_mess);
    return -1;
  }
  if (!scene->isValid()){
	  QString _mess1 = "<b>[GeomSceneGL] "+tr("GEOM Error")+" !!</b><br>"+tr("Scene Not Valid")+"<br>"
		  +tr("To continue can cause dysfunction of this program")+"<br>";
	  QString _mess2 = _mess1 + "<b>"+tr("File")+" :</b> " + QString(__FILE__) + "<br>";
	  _mess2 += "<b>"+tr("Line")+" :</b> " + QString::number(__LINE__) + "<br>";
	  if(!BATCHMODE){
		  int res = QMessageBox::critical(__frame,tr("GEOM Error"),_mess2,tr("Abort"),tr("Continue"),tr("Validity"));
		  if(res == 0 || res == -1)return -1;
		  else if(res == 2){
			  stringstream _errlog(ios::out) ;
			  bool res2 = scene->isValid(_errlog);
			  std::string _msg = _errlog.str();
			  if(!_msg.empty())error(_msg.c_str());
			  if(!res2){
				  QString _mess3 = _mess1 + "<b>"+tr("File")+" :</b> " + QString(__FILE__) + "<br>";
				  _mess3 += "<b>"+tr("Line")+" :</b> " + QString::number(__LINE__) + "<br>";
				  res = QMessageBox::critical(__frame,tr("GEOM Error"),_mess3,tr("Abort"),tr("Continue"));
				  if(res == 0 || res == -1)return -1;
			  }
		  }
	  }
	  else {
		  warning("*** Warning : "+_mess2);
		  return -1;
	  }
  }

  // Clears all the actions
  if (isAnimated() == eStatic){
    __scene = ScenePtr();
    __bbox= BoundingBoxPtr();
    clearCache();
  }
  else { 
	  __selectedShapes.clear(); 
	  if(isAnimated() == eAnimatedPrimitives) clearCache();
  }

  // Sets the scene
  __scene = scene;

  if (is_null_ptr(__bbox)){
	  // Computes the global bounding box
	  if (! __scene->empty()) {
		  if(__bboxComputer.process(__scene))
			  __bbox = __bboxComputer.getBoundingBox();
		  QString _msg(tr("Display")+" ");
		  _msg+=QString::number(__scene->size());
		  _msg+=(" "+tr("geometric shapes."));
		  status(_msg,10000);
	 }
	 else{
		 status(tr("Display empty scene."));
    }
    __camera->buildCamera(__bbox);
  }
  emit sceneChanged();
  if(__frame != NULL && __frame->isVisible())emit valueChanged();
  return 1;
}

void  
ViewGeomSceneGL::computeCamera()
{
  __camera->buildCamera(__bbox, true);
}

/* ----------------------------------------------------------------------- */

void
ViewGeomSceneGL::initializeGL()
{
	glTexGenf(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
	GLfloat f[] = { 0.,1.,0.,0.};
	glTexGenfv(GL_S,GL_OBJECT_PLANE,f);
	glTexGenf(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
	GLfloat f2[] = { 0.,0.,1.,0.};
	glTexGenfv(GL_T,GL_OBJECT_PLANE,f2);
}

void
ViewGeomSceneGL::paintGL()
{

    if (__scene && !__scene->empty()){

    switch (__renderingMode) {
    case 1:
      __light->switchOn();
      if(__blending)glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      else glBlendFunc(GL_ONE,GL_ZERO);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      if(__renderer.beginSceneList()){
		if(__renderer.getRenderingMode() & GLRenderer::Dynamic)
			__scene->apply(__renderer);
		else {
			int cur = 0;
			int tot = __scene->size();
			int tenpercent = max(1,tot / 10);
			int cpercent = 0;
			__renderer.beginProcess();
			for(Scene::iterator it = __scene->begin();it != __scene->end(); it++){
				(*it)->apply(__renderer);
				cur++;
				if(cur / tenpercent > cpercent){ 
					cpercent = cur / tenpercent;
					progress(cur,tot);
				}
			}
			progress(tot,tot);
			__renderer.endProcess();
		}
        __renderer.endSceneList();
      }
      if(GEOM_GL_ERROR) clear();
      break;
    case 2:
      __light->switchOff();
      glBlendFunc(GL_ONE,GL_ZERO);
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      if(__renderer.beginSceneList()){
        __scene->apply(__renderer);
        __renderer.endSceneList();
      }
      if(GEOM_GL_ERROR) clear();
      break;
    case 3:
      __light->switchOff();
      glBlendFunc(GL_ONE,GL_ZERO);
      if(__skelRenderer.beginSceneList()){
        __scene->apply(__skelRenderer);
        __skelRenderer.endSceneList();
      }
      if(GEOM_GL_ERROR) clear();
      break;
    case 4:
      __light->switchOff();
      glBlendFunc(GL_ONE,GL_ZERO);
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      if(__renderer.beginSceneList()){
        __scene->apply(__renderer);
        __renderer.endSceneList();
      }
      __light->switchOn();
      if(__blending)glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      else glBlendFunc(GL_ONE,GL_ZERO);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      if(__renderer.beginSceneList()){
        __scene->apply(__renderer);
        __renderer.endSceneList();
      }
      if(GEOM_GL_ERROR) clear();
      break;
    };

    if(__renderingOption[0]){
      __light->switchOn();
      if(__blending)glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      else glBlendFunc(GL_ONE,GL_ZERO);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      if(__bboxRenderer.beginSceneList()){
        __scene->apply(__bboxRenderer);
        __bboxRenderer.endSceneList();
      }
      if(GEOM_GL_ERROR) clear();
    };
    if(__renderingOption[1]){
      __light->switchOff();
      glBlendFunc(GL_ONE,GL_ZERO);
	  glGeomColor(Color3(64,64,64));
      if(__ctrlPtRenderer.beginSceneList()){
        __scene->apply(__ctrlPtRenderer);
        __ctrlPtRenderer.endSceneList();
      }
      if(GEOM_GL_ERROR) clear();
    }
    if(!__selectedShapes.empty()){
      __light->switchOff();
      glBlendFunc(GL_ONE,GL_ZERO);
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	  glGeomColor(Color3(64,64,64));
      for(SelectionCache::iterator _it = __selectedShapes.begin();
          _it !=__selectedShapes.end(); _it++)
        get_item_value(_it)->apply(__bboxRenderer);
      if(GEOM_GL_ERROR) clear();
    }
  }
}

void
ViewGeomSceneGL::selectGL()
{
  if (__scene){
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    if(__blending)glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    else glBlendFunc(GL_ONE,GL_ZERO);
    GLRenderer::RenderingMode rtype = __renderer.getRenderingMode();
    __renderer.setRenderingMode(GLRenderer::Selection);
    __scene->apply(__renderer);
    __renderer.setRenderingMode(rtype);
   if(GEOM_GL_ERROR) clear();
  }
}

void
ViewGeomSceneGL::selectionEvent(uint_t id)
{
  SelectionCache::iterator _it =__selectedShapes.find(id);
  if(_it!=__selectedShapes.end()){
    Shape3DPtr ptr = get_item_value(_it);
	__selectedShapes.erase(_it);
    uint_t _id = (ptr->getId() == Shape::NOID?id:ptr->getId());
	info("*** Comment : "+tr("Shape")+" " +QString::number(_id)+ " "+tr("unselected")+".");
	status(tr("Shape")+" "
	  + (ptr->isNamed()?QString((ptr->getName()+" ").c_str()):"")+"(Id=" +QString::number(_id)
	  + ") "+tr("unselected")+".",20000);
  }
  else {
    Shape3DPtr ptr;
    for(Scene::iterator _it = __scene->begin();
        _it != __scene->end(); _it++){
      if((ptr = dynamic_pointer_cast<Shape>(*_it)) && 
		(ptr->SceneObject::getId() == id)){
        __selectedShapes[id]=ptr;
        uint_t _id = (ptr->getId() == Shape::NOID?id:ptr->getId());
		info("*** Comment : "+tr("Shape")+" " +QString::number(_id)+ " "+tr("selected")+".");
		status(tr("Shape")+" "
		       + (ptr->isNamed()?QString((ptr->getName()+" ").c_str()):"")+"(Id=" +QString::number(_id)
		       + ") "+tr("selected")+".",20000);
		emit selectionChanged(ptr->getName().c_str());
        break;
      }
    }
  }
}

void
ViewGeomSceneGL::selectionEvent(const vector<uint_t>& id)
{
	SelectionCache::iterator _it;
	uint_t selected = 0;
	uint_t unselected = 0;
	QSet<uint_t> selection;

	for(vector<uint_t>::const_iterator _id = id.begin();_id != id.end();_id++){
		_it =__selectedShapes.find(*_id);
		if(_it != __selectedShapes.end()){
			__selectedShapes.erase(_it);
			unselected++;
		}
		else {
			selection.insert(*_id);
		}
	}

	Shape3DPtr ptr;
	for(Scene::iterator _it = __scene->begin(); _it != __scene->end(); _it++){
		if( (ptr = dynamic_pointer_cast<Shape>(*_it)) && 
			selection.find(ptr->SceneObject::getId()) != selection.end()){
			__selectedShapes[ptr->SceneObject::getId()]=ptr;
			selected++;
		}
	}

	QString mess;
	if(selected > 0){
		mess = "shape";
		if(selected != 1)mess +='s';
		mess +=" "+tr("selected");
		mess = QString::number(selected) + " "+ mess;
	}
	if(selected > 0 && unselected > 0)mess += " "+tr("and")+" ";
	if(unselected > 0){
		QString mess2 = "shape";
		if(unselected != 1)mess2 +='s';
		mess2 +=" unselected";
		mess += mess + QString::number(unselected)+ " " + mess2;
	}
	mess +='.';
	info("*** Comment : " +mess);
	status(mess,20000);
}

void
ViewGeomSceneGL::selectionIdEvent(const vector<uint_t>& id)
{
  __selectedShapes.clear();
  uint_t selected = 0;
  for(vector<uint_t>::const_iterator _id = id.begin();_id != id.end();_id++){
	  __scene->lock();
	  for(Scene::const_iterator _it = __scene->begin() ; 
					  _it != __scene->end(); 
					  _it++){
		ShapePtr ptr = dynamic_pointer_cast<Shape>(*_it);
		if(ptr && ptr->getId() == *_id){
		  __selectedShapes[ptr->SceneObject::getId()]=ptr;
		  selected++;
		}
	  }
	  __scene->unlock();
  }

  QString mess;
  if(selected > 0){
	mess = "shape";
	if(selected != 1)mess +='s';
	mess +=" "+tr("selected");
	mess = QString::number(selected) + " "+ mess;
  }
  mess +='.';
  info("*** Comment : " +mess);
  status(mess,20000);
  emit valueChanged();
}


void
ViewGeomSceneGL::selectionEvent(Q3ListViewItem * item)
{
  if(item && item->text(2) == "Shape"){
	string name = item->text(0).toAscii().constData();
	bool found = false;
    for(Scene::iterator _it = __scene->begin();
	!found && _it != __scene->end(); _it++){
	  if((*_it)->getName() == name){
		uint_t id = (*_it)->getId();
        if((*_it)->getId() == Shape::NOID){
		  id = (*_it)->SceneObject::getId();
		}
		SelectionCache::iterator _it2 
		  =__selectedShapes.find(id);
		if(_it2!=__selectedShapes.end()){
		  __selectedShapes.erase(_it2);
		  info("*** Comment : "+tr("Shape")+" " +QString::number(id)+ " "+tr("unselected")+".");
		  status(tr("Shape")+" " +QString::number(id)+ " "+tr("unselected")+".",20000);
		}
		else {
		  __selectedShapes[id]=*_it;
		  info("*** Comment : "+tr("Shape")+" " +QString::number(id)+ " "+tr("selected")+".");
		  status(tr("Shape")+" " +QString::number(id)+ " "+tr("selected")+".",20000);
		}
		emit valueChanged();
		found = true;
	  }
	}
	if(!found){
	  warning("*** Warning : "+tr("Shape")+" \"" +item->text(0)+ "\" "+tr("not found")+".");
	  status(tr("Shape")+" \"" +item->text(0)+ "\" "+tr("not found")+".",20000);
	}
  }
}

void
ViewGeomSceneGL::clearSelectionEvent()
{
  __selectedShapes.clear();
  emit valueChanged();
}

void
ViewGeomSceneGL::removeSelection()
{
  if(__selectedShapes.empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("Empty Selection. Cannot Remove!"),1,0,0);
	return;
  }
  if(QMessageBox::warning(__frame,tr("Confirmation"),
				tr("Remove Selection?"),tr("Ok"),tr("Cancel")) != 0)
	return;
  GeomSceneChangeEvent * e = new GeomSceneChangeEvent(getNotSelection(),NULL,NULL);
  QApplication::postEvent(this,e);
}

void
ViewGeomSceneGL::keepSelectionOnly()
{
  if(__selectedShapes.empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("Empty Selection. Cannot Remove!"),1,0,0);
	return;
  }
  GeomSceneChangeEvent * e = new GeomSceneChangeEvent(getSelection(),NULL,NULL);
  SelectionCache selection = __selectedShapes;
  QApplication::sendEvent(this,e);
  __selectedShapes = selection;
}

void
ViewGeomSceneGL::wireSelection()
{
  if(__selectedShapes.empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("Empty Selection. Cannot Replace!"),1,0,0);
	return;
  }
  ScenePtr scene = getNotSelection();
  WireComputer wire(__discretizer);
  SelectionCache selection;
  for(SelectionCache::iterator _it = __selectedShapes.begin();
	  _it != __selectedShapes.end();_it++){
	ShapePtr sh = dynamic_pointer_cast<Shape>(get_item_value(_it));
	if(sh){
	  if(sh->apply(wire)){
		sh->geometry = wire.getWire();
		scene->add(Shape3DPtr(new Shape(*sh)));
		selection[sh->SceneObject::getId()] = sh;
	  }
	}
	else {
	  qWarning((QString(__FILE__)+QString(":")+QString::number(__LINE__)+QString(": Wire Transformation not yet implemented on Inline.")).toAscii().constData());
	}
  }		

  GeomSceneChangeEvent * e = new GeomSceneChangeEvent(scene,NULL,NULL);
  QApplication::sendEvent(this,e);
  __selectedShapes = selection;
}
void
ViewGeomSceneGL::discretizeSelection()
{
  if(__selectedShapes.empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("Empty Selection. Cannot Replace!"),1,0,0);
	return;
  }
  ScenePtr scene = getNotSelection();
  SelectionCache selection;
  for(SelectionCache::iterator _it = __selectedShapes.begin();
	  _it != __selectedShapes.end();_it++){
	ShapePtr sh = dynamic_pointer_cast<Shape>(get_item_value(_it));
	if(sh){
	  if(sh->apply(__discretizer)){
		sh->geometry = GeometryPtr(__discretizer.getDiscretization());
		scene->add(Shape3DPtr(new Shape(*sh)));
		selection[sh->SceneObject::getId()] = sh;
	  }
	}
	else {
	  qWarning((QString(__FILE__)+QString(":")+QString::number(__LINE__)+QString(": Discretize Transformation not yet implemented on Inline.")).toAscii().constData());
	}
  }		

  GeomSceneChangeEvent * e = new GeomSceneChangeEvent(scene,NULL,NULL);
  QApplication::sendEvent(this,e);
  __selectedShapes = selection;
}

void
ViewGeomSceneGL::triangulateSelection()
{
  if(__selectedShapes.empty()){
    QMessageBox::warning(__frame,tr("GEOM Error"),
	  tr("Empty Selection. Cannot Replace!"),1,0,0);
	return;
  }
  ScenePtr scene = getNotSelection();
  SelectionCache selection;
  for(SelectionCache::iterator _it = __selectedShapes.begin();
	  _it != __selectedShapes.end();_it++){
	ShapePtr sh = dynamic_pointer_cast<Shape>(get_item_value(_it));
	Tesselator t;
	if(sh){
	  if(sh->apply(t)){
		sh->geometry = GeometryPtr(t.getDiscretization());
		scene->add(Shape3DPtr(new Shape(*sh)));
		selection[sh->SceneObject::getId()] = sh;
	  }
	}
	else {
	  qWarning((QString(__FILE__)+QString(":")+QString::number(__LINE__)+QString(": Triangulation Transformation not yet implemented on Inline.")).toAscii().constData());
	}
  }		

  GeomSceneChangeEvent * e = new GeomSceneChangeEvent(scene,NULL,NULL);
  QApplication::sendEvent(this,e);
  __selectedShapes = selection;
}

/* ----------------------------------------------------------------------- */

vector<pair<uint_t,double> > 
ViewGeomSceneGL::getProjectionSizes(const ScenePtr& sc){
	vector<pair<uint_t,double> > res;
	ViewGLFrame * frame = dynamic_cast<ViewGLFrame *>(__frame);
	if (!frame) return res;
	bool mode = frame->getCamera()->getProjectionMode();
	if(mode)frame->getCamera()->setOrthographicMode();
    GLRenderer::RenderingMode rtype = __renderer.getRenderingMode();
    __renderer.setRenderingMode(GLRenderer::Selection);
	ScenePtr nsc(new Scene());
	size_t tot = sc->size();
	size_t per = max(size_t( 1 ),(size_t)((double)tot / 100.0));
	size_t cur = 0;
	bool autoredraw = frame->isRedrawEnabled();
	if(frame->isPixelBufferUsed()){
		frame->activatePBuffer(true);
		if(autoredraw)frame->activateRedraw(false);
	}
	for(Scene::const_iterator it = sc->begin(); it != sc->end(); it++){
		nsc->clear();
		nsc->add(*it);
		setScene(nsc);
		if(frame->isPixelBufferUsed())frame->paintPixelBuffer();
		res.push_back(pair<uint_t,double>((*it)->getId(),frame->getProjectionSize()));
		cur++;
		if(cur % per == 0){
			printf("\x0d Projections %i%% done.",cur*100/tot);
			QCoreApplication::processEvents();
		}
	}
    __renderer.setRenderingMode(rtype);
	if(frame->isPixelBufferUsed()){
		frame->activatePBuffer(false);
		if(autoredraw)frame->activateRedraw(true);
	}
	printf("\x0d Projections 100%% done.");
	if(mode)frame->getCamera()->setProjectionMode(mode);
	return res;
}

ViewRayPointHitBuffer *
ViewGeomSceneGL::castRays(const ScenePtr& sc, bool back_test){
	ViewGLFrame * frame = dynamic_cast<ViewGLFrame *>(__frame);
	if (!frame) return NULL;
	int w = frame->width();
	int h = frame->height();
	ViewRayPointHitBuffer * res = new ViewRayPointHitBuffer(w,h);
	double az = frame->getCamera()->getAzimuth();
	double el = frame->getCamera()->getElevation();
	double b_az = az + 180;
	if(b_az >= 180) b_az -= 360;
        if(b_az <= -180) b_az += 360;
	double b_el = -el;

	ScenePtr nsc(new Scene());
	size_t tot = sc->size();
	size_t per = max(size_t( 1 ),(size_t)((double)tot / 100.0));
	size_t cur = 0;
	bool autoredraw = frame->isRedrawEnabled();
	if(frame->isPixelBufferUsed()){
		frame->activatePBuffer(true);
		if(autoredraw)frame->activateRedraw(false);
	}
	bool mode = frame->getCamera()->getProjectionMode();
	if(mode)frame->getCamera()->setOrthographicMode();
    GLRenderer::RenderingMode rtype = __renderer.getRenderingMode();
    __renderer.setRenderingMode(GLRenderer::Dynamic);
	for(Scene::const_iterator it = sc->begin(); it != sc->end(); it++){
		nsc->clear();
		nsc->add(*it);
		uint_t id = (*it)->getId();
		setScene(nsc);
		if(frame->isPixelBufferUsed())frame->paintPixelBuffer();
		else if (!autoredraw) frame->updateGL();
		ViewZBuffer * cbuff = frame->grabDepthBuffer(false);

		if(! back_test) {
			for(int c = 0;  c < w; ++c){
				for(int r = 0;  r < h; ++r){
					if (cbuff->getAt(r,c).depth < 1){
						const Vector3& pos = cbuff->getAt(r,c).pos;
						res->getAt(r,c).push_back(RayPointHit(id,pos,pos));
					}
				}
			}
		}
		else {
			frame->getCamera()->setAngles(b_az,b_el);
			if(frame->isPixelBufferUsed())frame->paintPixelBuffer();
			else frame->updateGL();
			ViewZBuffer * cbackbuff = frame->grabDepthBuffer(false);
			for(int c = 0;  c < w; ++c){
				for(int r = 0;  r < h; ++r){
					if (cbuff->getAt(r,c).depth < 1){
						res->getAt(r,c).push_back(RayPointHit(id,cbuff->getAt(r,c).pos,cbackbuff->getAt(r,w-1-c).pos));
					}
				}
			}
			delete cbackbuff;
			frame->getCamera()->setAngles(az,el);
		}
		delete cbuff;
		cur++;
		if(cur % per == 0){
			printf("\x0d Projections %i%",cur*100/tot);
			QCoreApplication::processEvents();
		}
	}
	printf("\x0d Projections 100%\n");
    __renderer.setRenderingMode(rtype);
	// std::cerr << "\x0d Projections 100% done.\n";
	if(frame->isPixelBufferUsed()){
		frame->activatePBuffer(false);
		if(autoredraw)frame->activateRedraw(true);
	}
	setScene(sc);
	return res;
}


std::vector<std::pair<uint_t,uint_t> > 
ViewGeomSceneGL::getPixelPerShape(double* pixelwidth)
{
	ViewGLFrame * frame = dynamic_cast<ViewGLFrame *>(__frame);
	if (!frame) return std::vector<std::pair<uint_t,uint_t> >();
	else {
		// custom scene creation : color represent id
		ScenePtr nsc(new Scene());
		QHash<uint_t,uint_t> coloridmap;
		for(Scene::const_iterator it = __scene->begin(); it != __scene->end(); it++){
			uint_t id = (*it)->getId();
			Color4 c = Color4::fromUint(id);
			MaterialPtr mat = new Material(Color3(c),1);
			mat->getTransparency() = c.getAlphaClamped();
			ShapePtr sh = dynamic_pointer_cast<Shape>(*it);
			nsc->add(ShapePtr(new Shape(sh->getGeometry(),AppearancePtr(mat),id)));
		}


		// auto repaint desactivate
		bool autoredraw = frame->isRedrawEnabled();
		if(autoredraw)frame->activateRedraw(false);

		// dynamic rendering init
		GLRenderer::RenderingMode rtype = __renderer.getRenderingMode();
		__renderer.setRenderingMode(GLRenderer::Dynamic);
		__renderer.clear();

		// camera init
	    bool mode = __camera->getProjectionMode();
		bool cameralockdim = __camera->isDimLock();
	    if(mode)__camera->setOrthographicMode();
		if(!cameralockdim) __camera->lockDim(true);

		// pixel buffer init
		if(frame->isPixelBufferUsed())
			frame->activatePBuffer(true);

		// blending init
		GLboolean glblend = glIsEnabled(GL_BLEND);
		bool blending = __blending;
		if(!glblend)glEnable(GL_BLEND);
		if(blending)enableBlending(false);

		// scene init
		ScenePtr oldscene = __scene;
		setScene(nsc);
		if(!frame->isPixelBufferUsed()) frame->updateGL();

		// projection computation
		std::vector<std::pair<uint_t,uint_t> > res = frame->getProjectionPixelPerColor(pixelwidth);

		/* static int nbimg = 0;
		nbimg += 1;
	    QString fname =  "test"+QString::number(nbimg)+".png";*/
		// pixel buffer desactive
		if(frame->isPixelBufferUsed()){
			// frame->getPBuffer()->toImage().save(fname);
			frame->activatePBuffer(false);
		}
		// else { frame->grabFrameBuffer(true).save(fname); }

		// scene restore
		setScene(oldscene);

		// blending restore
		if(!glblend)glDisable(GL_BLEND);
		if(blending)enableBlending(blending);

		// dynamic rendering restore
		__renderer.setRenderingMode(rtype);

		// camera restore
		if(!cameralockdim) __camera->lockDim(cameralockdim);
		if(mode) __camera->setPerspectiveMode();

		// auto repaint desactivate
		if(autoredraw)frame->activateRedraw(autoredraw);
		return res;
	}

}

/* ----------------------------------------------------------------------- */

void 
ViewGeomSceneGL::customEvent(QEvent * e) {
	if(e->type() == ViewGeomEvent::eProjList){
		GeomProjListEvent * myevent = (GeomProjListEvent *)e;
		*(myevent->result) = getProjectionSizes(myevent->arg1);
	}
	else if (e->type() == ViewGeomEvent::eRayBuff2){
		ViewRayBuff2Event * myevent = (ViewRayBuff2Event *)e;
		*(myevent->result) = castRays(myevent->arg1,myevent->arg2);
	}
	else if (e->type() == ViewGeomEvent::eGetScene){
		GeomGetSceneEvent * myevent = (GeomGetSceneEvent *)e;
		*(myevent->result) = __scene;
	}
	else if (e->type() == ViewGeomEvent::eIntegratedProjList){
		ViewIntegratedProjListEvent * myevent = (ViewIntegratedProjListEvent *)e;
		*(myevent->result) = getPixelPerShape(myevent->arg1);
	}
}


/* ----------------------------------------------------------------------- */

ViewMultiGeomSceneGL::ViewMultiGeomSceneGL(ViewCameraGL * camera,
                                           ViewLightGL * light,
                                           QGLWidget * parent,
                                           const char * name):
  ViewGeomSceneGL(camera,light,parent,name),
  __transitionRenderer(__discretizer),
  __simpleScene(true),
  __renderingStep(0),
  __transSlider(0),
  __transitionBar(0)
{
}


ViewMultiGeomSceneGL::~ViewMultiGeomSceneGL()
{
}

void
ViewMultiGeomSceneGL::clear()
{
  ViewGeomSceneGL::clear();
  __transitionRenderer.clear();
  __simpleScene = true;
  __renderingStep = 0;
}

/* ----------------------------------------------------------------------- */

void
ViewMultiGeomSceneGL::paintGL()
{
  if(__simpleScene)ViewGeomSceneGL::paintGL();
  else {
    if (__scene){
      switch (__renderingMode) {
      case 1:
        __light->switchOn();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        __transitionRenderer.rend(__renderingStep);
        break;
      case 2:
        __light->switchOff();
        glBlendFunc(GL_ONE,GL_ZERO);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        __transitionRenderer.rend(__renderingStep);
        break;
      case 3:
        __light->switchOff();
        glBlendFunc(GL_ONE,GL_ZERO);
        if(__skelRenderer.beginSceneList()){
          __scene->apply(__skelRenderer);
          __skelRenderer.endSceneList();
        }
      };

      if(__renderingOption[0]){
        __light->switchOn();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        if(__bboxRenderer.beginSceneList()){
          __scene->apply(__bboxRenderer);
          __bboxRenderer.endSceneList();
        }
      };
      if(__renderingOption[1]){
        __light->switchOff();
        glBlendFunc(GL_ONE,GL_ZERO);
        if(__ctrlPtRenderer.beginSceneList()){
          __scene->apply(__ctrlPtRenderer);
          __ctrlPtRenderer.endSceneList();
        }
      }
      if(!__selectedShapes.empty()){
        __light->switchOff();
        glBlendFunc(GL_ONE,GL_ZERO);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        for(SelectionCache::iterator _it = __selectedShapes.begin();
            _it !=__selectedShapes.end(); _it++)
          get_item_value(_it)->apply(__bboxRenderer);
      }
    }
  }
}

/* ----------------------------------------------------------------------- */

int
ViewMultiGeomSceneGL::setScene(const ScenePtr& scene1,const ScenePtr& scene2)
{
  __transSlider->setValue(0);
  if(__transSlider)__transSlider->show();
  ScenePtr scene(new Scene(*scene1));
  scene->merge(scene2);
  int a = ViewGeomSceneGL::setScene(scene);
  __simpleScene = false;
  __transitionRenderer.setScene(scene1,scene2);
  emit valueChanged();
  return a;
}

/* ----------------------------------------------------------------------- */

void
ViewMultiGeomSceneGL::setRenderingStep(int i)
{
  __renderingStep = i;
  emit valueChanged();
}

/* ----------------------------------------------------------------------- */


bool 
ViewMultiGeomSceneGL::sceneChangeEvent( ViewSceneChangeEvent * k)
{
	if(k->getSceneType() == GeomSceneChangeEvent::eGeomSceneEvent)
		return ViewGeomSceneGL::sceneChangeEvent(k);
	else if(k->getSceneType() == GeomSceneChangeEvent::eMultiSceneEvent){
		GeomMultiSceneChangeEvent * event = ( GeomMultiSceneChangeEvent * )k;
		setScene(event->scene,event->scene2);
		if(!event->error.isEmpty()) error(event->error);
		setFilename(event->file);
		if(__transSlider)__transSlider->show();
		return true;
	}
	else return false;
}
/* ----------------------------------------------------------------------- */


