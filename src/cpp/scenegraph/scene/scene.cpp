/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
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




#include "scne_scene.h"
#include "scne_shape.h"
#include "scne_binaryparser.h"
#include "actn_binaryprinter.h"
#include "actn_vgstarprinter.h"
#include "actn_povprinter.h"
#include "actn_vrmlprinter.h"
#include "actn_discretizer.h"
#include "appe_material.h"
#include "geom_geometry.h"
#include "scne_smbtable.h"
#include "util_messages.h"
#include "Tools/util_string.h"
#include "Tools/bfstream.h"
#include "Tools/dirnames.h"

#include <qglobal.h>

#ifdef QT_THREAD_SUPPORT
#include <qthread.h>
#if QT_VERSION >= 300
#include <qmutex.h>
#endif
#endif

GEOM_USING_NAMESPACE
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

Scene::Scene(unsigned int size ) :
  RefCountObject(),
  __shapeList(size,GeomShape3DPtr(0))
#ifdef QT_THREAD_SUPPORT
  ,  __mutex(new QMutex())
#endif
  {
  GEOM_ASSERT(isValid());
}

Scene::Scene(const Scene& scene) :
  RefCountObject()
#ifdef QT_THREAD_SUPPORT
  ,  __mutex(new QMutex())
#endif
{
  scene.lock();
  __shapeList = std::vector<GeomShape3DPtr>(scene.__shapeList);
  scene.unlock();
  GEOM_ASSERT(isValid());
}

Scene& Scene::operator=( const Scene& scene){
  lock();
  scene.lock();
  __shapeList = std::vector<GeomShape3DPtr>(scene.__shapeList);
  scene.unlock();
  unlock();
  return *this;
}

Scene::Scene(const string& filename, ostream& errlog, int max_error ) :
  RefCountObject(),
  __shapeList()
#ifdef QT_THREAD_SUPPORT
  ,  __mutex(new QMutex())
#endif
{
	read(filename,errlog,max_error);
	GEOM_ASSERT(isValid());
}

Scene::Scene(istream& input, ostream& errlog, int max_error ) :
  RefCountObject(),
  __shapeList()
#ifdef QT_THREAD_SUPPORT
  ,  __mutex(new QMutex())
#endif
{
  read(input,errlog,max_error);
  GEOM_ASSERT(isValid());
}

Scene::Scene(const SceneObjectSymbolTable& table) :
  RefCountObject(),
  __shapeList()
#ifdef QT_THREAD_SUPPORT
  , __mutex(new QMutex())
#endif
{
  add(table);
  GEOM_ASSERT(isValid());
}

void Scene::read( const std::string& filename , std::ostream& errlog, int max_error ){
    if(BinaryParser::isAGeomBinaryFile(filename)){
        BinaryParser _parser(errlog,max_error);
        _parser.parse(filename);
		lock();
        __shapeList = _parser.getScene()->__shapeList;
		unlock();
    }
    else {
		SceneObjectSymbolTable table;
		SceneObject::parse(filename,errlog,table,max_error);
		add(table);
	}
}

void Scene::read( std::istream& input, std::ostream& errlog, int max_error  ){
  SceneObjectSymbolTable table;
  SceneObject::parse(input,errlog,table,max_error);
  add(table);
}

void Scene::add( const SceneObjectSymbolTable& table ){
  GeomShape3DPtr shape;
  GeomShapePtr _shape;
  bool added = false;
  for (SceneObjectSymbolTable::const_iterator _it = table.begin();
       _it != table.end();
       _it++){
    if(shape.cast(_it->second)){
	  added = true;
      if(_shape.cast(_it->second)){
        if(!_shape->appearance)
          _shape->appearance = Material::DEFAULT_MATERIAL;
        add(*_shape);
      }
      else add(shape);
    }
  }
  if(!added){
    GeometryPtr _geom;
    for (SceneObjectSymbolTable::const_iterator _it = table.begin();
         _it != table.end();
         _it++){
      if(_geom.cast(_it->second)){
        add(GeomShape3DPtr(new GeomShape(_geom,Material::DEFAULT_MATERIAL,0)));
      }
    }
  }
}

Scene::~Scene( ){
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

void Scene::add( const GeomShape& shape ) {
  GEOM_ASSERT(shape.isValid());
  lock();
  __shapeList.insert(__shapeList.end(),GeomShape3DPtr(new GeomShape(shape)));
  unlock();
}

void Scene::add( const GeomShapePtr& shape ) {
  GEOM_ASSERT(shape.isValid());
  lock();
  __shapeList.insert(__shapeList.end(),GeomShape3DPtr(shape));
  unlock();
}

void Scene::add( const GeomShape3DPtr& shape ) {
  GEOM_ASSERT(shape.isValid());
  lock();
  __shapeList.insert(__shapeList.end(),shape);
  unlock();
}

/* ----------------------------------------------------------------------- */

bool Scene::apply( Action& action ) {
  bool _result;
  if( ! (_result = action.beginProcess()))return false;
  lock();
  for (vector<GeomShape3DPtr>::iterator _i = __shapeList.begin();
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
  for (vector<GeomShape3DPtr>::iterator _i = __shapeList.begin();
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
  for (vector<GeomShape3DPtr>::iterator _i = __shapeList.begin();
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
  for (vector<GeomShape3DPtr>::iterator _i = __shapeList.begin();
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
  for (vector<GeomShape3DPtr>::iterator _i = __shapeList.begin();
       _i != __shapeList.end();
       _i++)
    if (! (*_i)->applyAppearanceOnly(action)) _result = false;
  unlock();
  action.endProcess();
  return _result;
}

/* ----------------------------------------------------------------------- */
Scene::const_iterator
Scene::getBegin( ) const {
  return __shapeList.begin();
}

Scene::iterator
Scene::getBegin( ) {
  return __shapeList.begin();
}

Scene::const_iterator
Scene::getEnd( ) const {
  return __shapeList.end();
}

Scene::iterator
Scene::getEnd( ) {
  return __shapeList.end();
}

uint32_t Scene::getSize( ) const {
  lock();
  uint32_t s = __shapeList.size();
  unlock();
  return s;
}

void Scene::Resize(const uint32_t size ) {
  lock();
  __shapeList.resize(size);
  unlock();
}

bool Scene::isEmpty( ) const {
  lock();
  bool b = __shapeList.empty();
  unlock();
  return b;
}

const GeomShape3DPtr Scene::getAt(uint32_t i ) const {
  lock();
  GeomShape3DPtr ptr = __shapeList[i];
  unlock();
  return ptr;
}

void Scene::setAt(uint32_t i, const GeomShape3DPtr& ptr) {
  lock();
  __shapeList[i] = ptr;
  unlock();
}

const GeomShapePtr 
Scene::getShapeId(uint32_t id ) const {
  lock();
  for(Scene::const_iterator _it = __shapeList.begin() ; 
					  _it != __shapeList.end(); 
					  _it++)
	{
	  GeomShapePtr ptr = GeomShapePtr::Cast(*_it);
	  if(ptr.isValid() && ptr->getId() == id){
		  unlock();
		  return ptr;
	  }
	}
  unlock();
  return 0;
}

/* ----------------------------------------------------------------------- */

ScenePtr Scene::copy() const {
  ScenePtr ptr(new Scene(*this));
  lock();
  for(Scene::iterator _it = ptr->getBegin() ; _it != ptr->getEnd(); _it++)
    if ( *_it  )_it->cast((*_it)->copy());
  unlock();
  return ptr;
}

bool Scene::isValid( ) const {
  lock();
  for (vector<GeomShape3DPtr>::const_iterator _i = __shapeList.begin();
  _i != __shapeList.end();
  _i++){
    if (! (*_i) ) {
      genMessage
		(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Scene","ShapeList",number(distance(__shapeList.begin(),_i) + 1).c_str(),"Must not be a null Shape.");
	  unlock();
      return false;
    };	
    if (!(*_i)->isValid() ) {
      genMessage
		(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Scene","ShapeList",number(distance(__shapeList.begin(),_i) + 1).c_str(),"Must be a valid Shape.");
	  unlock();
      return false;
    };	
  }
  unlock();
  return true;
}

bool Scene::isValid(ostream& stream ) const{
  ostream * errlog = SceneObject::errorStream;
  ostream * warlog = SceneObject::warningStream;
  ostream * infolog = SceneObject::commentStream;
  SceneObject::commentStream = &stream;
  SceneObject::warningStream = &stream;
  SceneObject::errorStream = &stream;
  bool b = isValid( );
  SceneObject::commentStream = infolog;
  SceneObject::warningStream = warlog;
  SceneObject::errorStream = errlog;
  return b;
}

bool Scene::isValid( ostream& error,ostream& warning, ostream& info) const {
  ostream * errlog = SceneObject::errorStream;
  ostream * warlog = SceneObject::warningStream;
  ostream * infolog = SceneObject::commentStream;
  SceneObject::commentStream = &info;
  SceneObject::warningStream = &warning;
  SceneObject::errorStream = &error;
  bool b = isValid( );
  SceneObject::commentStream = infolog;
  SceneObject::warningStream = warlog;
  SceneObject::errorStream = errlog;
  return b;
}

void Scene::merge( const ScenePtr& scene ) {
  GEOM_ASSERT((scene) && (scene->isValid()));
  lock();
  scene->lock();
  __shapeList.insert(__shapeList.end(),scene->getBegin(),scene->getEnd());
  scene->unlock();
  unlock();
}

void Scene::save( const std::string& fname ) {
	string ext = get_extension(fname);
	ext = toUpper(ext);
	std::cerr << "Extension = " << ext << std::endl;
	if(ext == "BGEOM"){
		BinaryPrinter::print(this,fname,"File Generated with PlantGL.");
	}
	else if(ext == "POV"){
		ofstream stream(fname.c_str());
		Tesselator t;
		PovPrinter p(stream,t);
		apply(p);
	}
	else if(ext == "WRL"){
		ofstream stream(fname.c_str());
		Discretizer d;
		VrmlPrinter p(stream,d);
		apply(p);
	}
	else if(ext == "VGX"||ext == "VGS"){
		ofstream stream(fname.c_str());
		Tesselator t;
		VgstarPrinter p(stream,t);
		apply(p);
	}
	else {
		ofstream stream(fname.c_str());
		Printer p(stream,stream,stream);
		apply(p);
	}
}


ostream& operator<<( ostream& stream, Scene& s ){
    Printer p(stream,stream,stream);
    s.apply(p);
    return stream;
}


/* ----------------------------------------------------------------------- */


