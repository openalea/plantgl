/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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



#include "scene.h"
#include "shape.h"
#include "factory.h"
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/core/smbtable.h>
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/bfstream.h>
#include <plantgl/tool/dirnames.h>

#include <QtCore/qglobal.h>
#include <algorithm>

#ifdef QT_THREAD_SUPPORT
#include <QtCore/qthread.h>
#if QT_VERSION >= 300
#include <QtCore/qmutex.h>
#endif
#endif

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;

/* ----------------------------------------------------------------------- */

/*Scene::Scene() :
  RefCountObject()
#ifdef QT_THREAD_SUPPORT
  ,  __mutex(new QMutex())
#endif
  {
  GEOM_ASSERT(isValid());
}*/

#define WITH_POOL

Scene::Scene(unsigned int size ) :
  RefCountObject(),
  __shapeList(size,Shape3DPtr())
#ifdef QT_THREAD_SUPPORT
  ,  __mutex(new QMutex())
#endif
  {
#ifdef WITH_POOL
      POOL.registerScene(this);
#endif
  GEOM_ASSERT(isValid());
}

Scene::Scene(const Scene& scene) :
  RefCountObject()
#ifdef QT_THREAD_SUPPORT
  ,  __mutex(new QMutex())
#endif
{
#ifdef WITH_POOL
      POOL.registerScene(this);
#endif
  scene.lock();
  __shapeList = std::vector<Shape3DPtr>(scene.__shapeList);
  scene.unlock();
  GEOM_ASSERT(isValid());
}

Scene& Scene::operator=( const Scene& scene){
  lock();
  scene.lock();
  __shapeList = std::vector<Shape3DPtr>(scene.__shapeList);
  scene.unlock();
  unlock();
  return *this;
}

Scene::Scene(const string& filename, const std::string& format, ostream& errlog, int max_error ) :
  RefCountObject(),
  __shapeList()
#ifdef QT_THREAD_SUPPORT
  ,  __mutex(new QMutex())
#endif
{
#ifdef WITH_POOL
      POOL.registerScene(this);
#endif
	read(filename,format,errlog,max_error);
	GEOM_ASSERT(isValid());
}

Scene::Scene(const SceneObjectSymbolTable& table) :
  RefCountObject(),
  __shapeList()
#ifdef QT_THREAD_SUPPORT
  , __mutex(new QMutex())
#endif
{
#ifdef WITH_POOL
      POOL.registerScene(this);
#endif
  convert(table);
  GEOM_ASSERT(isValid());
}

bool Scene::read( const std::string& filename,
				  const std::string& format,
				  std::ostream& errlog, 
				  int max_error ){
	PglErrorStream::Binder psb(errlog);
	ScenePtr scne;
	if(format.empty())scne = SceneFactory::get().read(filename);
	else scne = SceneFactory::get().read(filename,format);
    if(scne) { merge(scne); return true; }
    else return false;
}


bool Scene::save( const std::string& fname, const std::string& format)  {
	if(format.empty()) return SceneFactory::get().write(fname,ScenePtr(this));
	else return SceneFactory::get().write(fname,ScenePtr(this),format);
}

void Scene::convert( const SceneObjectSymbolTable& table ){
  Shape3DPtr shape;
  ShapePtr _shape;
  bool added = false;
  for (SceneObjectSymbolTable::const_iterator _it = table.begin();
       _it != table.end();
       _it++){
	  if(shape = dynamic_pointer_cast<Shape3D>(_it->second)){
	  added = true;
      if(_shape = dynamic_pointer_cast<Shape>(_it->second)){
        if(!_shape->appearance)
          _shape->appearance = Material::DEFAULT_MATERIAL;
        add(Shape3DPtr(_shape));
      }
      else add(shape);
    }
  }
  if(!added){
    GeometryPtr _geom;
    for (SceneObjectSymbolTable::const_iterator _it = table.begin();
         _it != table.end();
         _it++){
      if(_geom = dynamic_pointer_cast<Geometry>(_it->second)){
        add(Shape3DPtr(new Shape(_geom,Material::DEFAULT_MATERIAL,0)));
      }
    }
  }
}

Scene::~Scene( ){
#ifdef WITH_POOL
      POOL.unregisterScene(this);
#endif
#ifdef QT_THREAD_SUPPORT
	if (__mutex)delete __mutex;
#endif
#ifdef GEOM_DEBUG
    cerr << "Delete Scene" << endl;
#endif
}

/* ----------------------------------------------------------------------- */
void Scene::lock( ) const{
#ifdef QT_THREAD_SUPPORT
  __mutex->lock();
#endif
}

void Scene::unlock( ) const{
#ifdef QT_THREAD_SUPPORT
  __mutex->unlock();
#endif
}
  
/* ----------------------------------------------------------------------- */
void Scene::clear( ){
  lock();
  __shapeList.clear();
  unlock();
}

/*
void Scene::add( const Shape& shape ) {
  GEOM_ASSERT(shape.isValid());
  lock();
  __shapeList.insert(__shapeList.end(),Shape3DPtr(new Shape(shape)));
  unlock();
}
*/
void Scene::add( const ShapePtr& shape ) {
  GEOM_ASSERT(shape.isValid());
  lock();
  __shapeList.insert(__shapeList.end(),Shape3DPtr(shape));
  unlock();
}


void Scene::add( const Shape3DPtr& shape ) {
  GEOM_ASSERT(shape.isValid());
  lock();
  __shapeList.insert(__shapeList.end(),shape);
  unlock();
}

  /** Remove a shape to the \e self
      \pre
      - shape must be non null and valid. */
void Scene::remove( const Shape3DPtr& shape )
{
	Scene::iterator it = begin();
    lock();
	while(it != end() && *it != shape)++it;
	if(it != end())remove(it);
    unlock();
}

void Scene::remove( Scene::iterator& it )
{
  __shapeList.erase(it);
}

/* ----------------------------------------------------------------------- */

bool Scene::apply( Action& action ) {
  bool _result;
  if( ! (_result = action.beginProcess()))return false;
  lock();
  for (vector<Shape3DPtr>::iterator _i = __shapeList.begin();
       _i != __shapeList.end();
       _i++) {
    if (! (*_i)->apply(action)) _result = false;
  };
  unlock();
  action.endProcess();
  return _result;
}

bool Scene::applyGeometryFirst( Action& action ) {
  bool _result;
  if( ! (_result = action.beginProcess()))return false;
  lock();
  for (vector<Shape3DPtr>::iterator _i = __shapeList.begin();
       _i != __shapeList.end();
       _i++) {

    if (! (*_i)->applyGeometryFirst(action)) _result = false;
  };
  unlock();
  action.endProcess();
  return _result;
}

bool Scene::applyGeometryOnly( Action& action ) {
  bool _result;
  if( ! (_result = action.beginProcess()))return false;
  lock();
  for (vector<Shape3DPtr>::iterator _i = __shapeList.begin();
       _i != __shapeList.end();
       _i++)
    if (! (*_i)->applyGeometryOnly(action)) _result = false;
  unlock();
  action.endProcess();
  return _result;
}

bool Scene::applyAppearanceFirst( Action& action ) {
  bool _result;
  if( ! (_result = action.beginProcess()))return false;
  lock();
  for (vector<Shape3DPtr>::iterator _i = __shapeList.begin();
       _i != __shapeList.end();
       _i++) {
    if (! (*_i)->applyAppearanceFirst(action)) _result = false;
  };
  unlock();
  action.endProcess();
  return _result;
}

bool Scene::applyAppearanceOnly( Action& action ) {
  bool _result;
  if( ! (_result = action.beginProcess()))return false;
  lock();
  for (vector<Shape3DPtr>::iterator _i = __shapeList.begin();
       _i != __shapeList.end();
       _i++)
    if (! (*_i)->applyAppearanceOnly(action)) _result = false;
  unlock();
  action.endProcess();
  return _result;
}

/* ----------------------------------------------------------------------- */
uint_t Scene::size( ) const {
  lock();
  uint_t s = __shapeList.size();
  unlock();
  return s;
}

void Scene::resize(const uint_t size ) {
  lock();
  __shapeList.resize(size);
  unlock();
}

bool Scene::empty( ) const {
  lock();
  bool b = __shapeList.empty();
  unlock();
  return b;
}

const Shape3DPtr Scene::getAt(uint_t i ) const {
  lock();
  Shape3DPtr ptr = __shapeList[i];
  unlock();
  return ptr;
}

void Scene::setAt(uint_t i, const Shape3DPtr& ptr) {
  lock();
  __shapeList[i] = ptr;
  unlock();
}

const ShapePtr 
Scene::getShapeId(uint_t id ) const {
  lock();
  for(Scene::const_iterator _it = __shapeList.begin() ; 
					  _it != __shapeList.end(); 
					  _it++)
	{
	  ShapePtr ptr = dynamic_pointer_cast<Shape>(*_it);
	  if(ptr && ptr->getId() == id){
		  unlock();
		  return ptr;
	  }
	}
  unlock();
  return ShapePtr();
}

const Shape3DPtr 
Scene::getSceneObjectId(uint_t id ) const {
  lock();
  for(Scene::const_iterator _it = __shapeList.begin() ; 
					  _it != __shapeList.end(); 
					  _it++)
	{
        if(*_it && (*_it)->SceneObject::getId() == id){
		  unlock();
		  return *_it;
	  }
	}
  unlock();
  return Shape3DPtr();
}

/* ----------------------------------------------------------------------- */

ScenePtr Scene::deepcopy(DeepCopier& copier) const {
  ScenePtr ptr(new Scene(*this));
  lock();
  for(Scene::iterator _it = ptr->begin() ; _it != ptr->end(); _it++)
	  copier.copy_object_attribute(*_it);
  unlock();
  return ptr;
}

bool Scene::isValid( ) const {
  lock();
  for (vector<Shape3DPtr>::const_iterator _i = __shapeList.begin();
  _i != __shapeList.end();
  _i++){
    if (! (*_i) ) {
      pglErrorEx
		(PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Scene","ShapeList",number(distance(__shapeList.begin(),_i) + 1).c_str(),"Must not be a null Shape.");
	  unlock();
      return false;
    };	
    if (!(*_i)->isValid() ) {
      pglErrorEx
		(PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Scene","ShapeList",number(distance(__shapeList.begin(),_i) + 1).c_str(),"Must be a valid Shape.");
	  unlock();
      return false;
    };	
  }
  unlock();
  return true;
}

bool Scene::isValid(ostream& stream ) const{
  PglErrorStream::Binder psb(stream);
  bool b = isValid( );
  return b;
}

bool Scene::isValid( ostream& error,ostream& warning, ostream& info) const {
  PglErrorStream::Binder psb(error,warning,info);
  bool b = isValid( );
  return b;
}

void Scene::merge( const ScenePtr& scene ) {
  GEOM_ASSERT((scene) && (scene->isValid()));
  lock();
  scene->lock();
  __shapeList.insert(__shapeList.end(),scene->begin(),scene->end());
  scene->unlock();
  unlock();
}

/* ----------------------------------------------------------------------- */
struct shapecmp{
    bool operator()(const Shape3DPtr& a, const Shape3DPtr& b)
    {
        ShapePtr a1 = dynamic_pointer_cast<Shape>(a);
        ShapePtr b1 = dynamic_pointer_cast<Shape>(b);
        if(!a1 || !b1)return false;
        else {
            MaterialPtr ma = dynamic_pointer_cast<Material>(a1->appearance);
            real_t ta = (!ma?0:ma->getTransparency());
            MaterialPtr mb = dynamic_pointer_cast<Material>(b1->appearance);
            real_t tb = (!mb?0:mb->getTransparency());
            return ta < tb;
        }
    }
};

void Scene::sort() {
    std::sort(__shapeList.begin(),__shapeList.end(),shapecmp());
}


/* ----------------------------------------------------------------------- */

Scene::Pool Scene::POOL;

Scene::Pool& Scene::pool() { return POOL; }

ScenePtr Scene::Pool::get(size_t id) const
{ 
    __mutex->lock();
    ScenePtr res;
    PoolList::const_iterator it = __pool.find(id);
    if(it != __pool.end())
        res = ScenePtr(it->second);
    __mutex->unlock();
    return res;
}


std::vector<ScenePtr> Scene::Pool::getScenes() const
{ 
    std::vector<ScenePtr> result;
    __mutex->lock();
    for (PoolList::const_iterator it = __pool.begin();it != __pool.end(); ++it)
        result.push_back(ScenePtr(it->second));
    __mutex->unlock();
    return result;
}

void Scene::Pool::registerScene(Scene * s)
{
  __mutex->lock();
  __pool[(size_t)s] = s;
  __mutex->unlock();
}
void Scene::Pool::unregisterScene(const Scene * s)
{
  __mutex->lock();
    PoolList::iterator it = __pool.find((size_t)s);
    if(it != __pool.end()) __pool.erase(it);
  __mutex->unlock();
}

Scene::Pool::Pool() { 
#ifdef QT_THREAD_SUPPORT
   __mutex = new QMutex();
#endif
}

Scene::Pool::~Pool() { 
#ifdef QT_THREAD_SUPPORT
   delete __mutex;
#endif
}

/* ----------------------------------------------------------------------- */
