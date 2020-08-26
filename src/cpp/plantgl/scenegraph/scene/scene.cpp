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
#include <plantgl/tool/util_mutex.h>

#include <algorithm>


PGL_USING_NAMESPACE
using namespace std;

/* ----------------------------------------------------------------------- */

/*Scene::Scene() :
  RefCountObject()
#ifdef PGL_THREAD_SUPPORT
  ,  __mutex(new PglMutex())
#endif
  {
  GEOM_ASSERT(isValid());
}*/

#define WITH_POOL

Scene::Scene(unsigned int size ) :
  RefCountObject(),
  __shapeList(size,Shape3DPtr())
  {
#ifdef PGL_THREAD_SUPPORT
   __mutex = new PglMutex();
#endif
#ifdef WITH_POOL
      POOL.registerScene(this);
#endif
  GEOM_ASSERT(isValid());
}

Scene::Scene(const Scene& scene) :
  RefCountObject()
{
#ifdef PGL_THREAD_SUPPORT
   __mutex = new PglMutex();
#endif
#ifdef WITH_POOL
      POOL.registerScene(this);
#endif
  scene.lock();
  __shapeList = std::vector<Shape3DPtr>(scene.__shapeList);
  scene.unlock();
  GEOM_ASSERT(isValid());
}

Scene::Scene(const Scene::const_iterator begin, const Scene::const_iterator end) :
  RefCountObject()
{
#ifdef PGL_THREAD_SUPPORT
   __mutex = new PglMutex();
#endif
#ifdef WITH_POOL
      POOL.registerScene(this);
#endif
  __shapeList = std::vector<Shape3DPtr>(begin, end);
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
{
#ifdef PGL_THREAD_SUPPORT
   __mutex = new PglMutex();
#endif
#ifdef WITH_POOL
      POOL.registerScene(this);
#endif
    read(filename,format,errlog,max_error);
    GEOM_ASSERT(isValid());
}

Scene::Scene(const SceneObjectSymbolTable& table) :
  RefCountObject(),
  __shapeList()
{
#ifdef PGL_THREAD_SUPPORT
   __mutex = new PglMutex();
#endif
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
      if((shape = dynamic_pointer_cast<Shape3D>(_it->second))){
      added = true;
      if((_shape = dynamic_pointer_cast<Shape>(_it->second))){
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
      if((_geom = dynamic_pointer_cast<Geometry>(_it->second))){
        add(Shape3DPtr(new Shape(_geom,Material::DEFAULT_MATERIAL,0)));
      }
    }
  }
}

Scene::~Scene( ){
#ifdef WITH_POOL
      POOL.unregisterScene(this);
#endif
#ifdef PGL_THREAD_SUPPORT
    if (__mutex)delete __mutex;
#endif
#ifdef GEOM_DEBUG
    cerr << "Delete Scene" << endl;
#endif
}

/* ----------------------------------------------------------------------- */
void Scene::lock( ) const{
#ifdef PGL_THREAD_SUPPORT
  __mutex->lock();
#endif
}

void Scene::unlock( ) const{
#ifdef PGL_THREAD_SUPPORT
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
    if(it != end())__shapeList.erase(it);
    unlock();
}

void Scene::erase( Scene::iterator it )
{
   __shapeList.erase(it);
}

 void Scene::erase( Scene::iterator itbeg, Scene::iterator itend )
 {
    __shapeList.erase(itbeg, itend);
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
Scene::findShapeId(uint_t id ) const {
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
Scene::findSceneObjectId(uint_t id ) const {
  lock();
  for(Scene::const_iterator _it = __shapeList.begin() ;
                      _it != __shapeList.end();
                      _it++)
    {
        if(*_it && (*_it)->SceneObject::getObjectId() == id){
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

bool Scene::hasDynamicRendering() const
{
  lock();
  for (vector<Shape3DPtr>::const_iterator _i = __shapeList.begin();
  _i != __shapeList.end();
  _i++){
    if((*_i)->hasDynamicRendering())return true;
  }
  return false;

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

void Scene::Pool::lock() const {
#ifdef PGL_THREAD_SUPPORT
    __mutex->lock();
#endif
}

void Scene::Pool::unlock() const {
#ifdef PGL_THREAD_SUPPORT
    __mutex->unlock();
#endif
}

ScenePtr Scene::Pool::get(size_t id) const
{
    lock();
    ScenePtr res;
    PoolList::const_iterator it = __pool.find(id);
    if(it != __pool.end())
        res = ScenePtr(it->second);
    unlock();
    return res;
}


std::vector<ScenePtr> Scene::Pool::getScenes() const
{
    std::vector<ScenePtr> result;
    lock();
    for (PoolList::const_iterator it = __pool.begin();it != __pool.end(); ++it)
        result.push_back(ScenePtr(it->second));
    unlock();
    return result;
}

void Scene::Pool::registerScene(Scene * s)
{
  lock();
  __pool[(size_t)s] = s;
  unlock();
}
void Scene::Pool::unregisterScene(const Scene * s)
{
  lock();
    PoolList::iterator it = __pool.find((size_t)s);
    if(it != __pool.end()) __pool.erase(it);
  unlock();
}

Scene::Pool::Pool() {
#ifdef PGL_THREAD_SUPPORT
   __mutex = new PglMutex();
#endif
}

Scene::Pool::~Pool() {
#ifdef PGL_THREAD_SUPPORT
   delete __mutex;
#endif
}

/* ----------------------------------------------------------------------- */
