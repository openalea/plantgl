/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/appearance/appearance.h>
#include <plantgl/scenegraph/core/action.h>
#include <plantgl/scenegraph/appearance/material.h>

#include <string>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"
#include "../util/export_list.h"
#include "../util/exception.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Scene)
DEF_POINTEE(SceneObject)

std::string get_sco_name(SceneObject * obj){ return obj->getName(); } 
void set_sco_name(SceneObject * obj, std::string v){ obj->setName(v); } 

void export_SceneObject()
{
  class_< SceneObject,SceneObjectPtr, boost::noncopyable >("SceneObject", no_init)
    .def("getName", &SceneObject::getName, return_value_policy< copy_const_reference >())
    .def("isNamed", &SceneObject::isNamed)
    .def("setName", &SceneObject::setName)
    .add_property("name",get_sco_name,&SceneObject::setName)
    .def("isValid", &SceneObject::isValid)
    .def("apply", &SceneObject::apply)
    .def("copy", &SceneObject::copy)
    .def("getId", &SceneObject::getId)
	.enable_pickling()
    ;
}


ScenePtr sc_fromlist( boost::python::list l ) 
{ 
  ScenePtr scene = new Scene();
  object iter_obj = boost::python::object( handle<>( PyObject_GetIter( l.ptr() ) ) );
  while( 1 )
  {
	    object obj;
		try { 
          obj = iter_obj.attr( "next" )();
		} catch( error_already_set ) { PyErr_Clear(); break; }
		boost::python::extract<GeometryPtr> geom( obj );
		if(geom.check()){
			GeometryPtr g = geom();
			scene->add(Shape(g,Material::DEFAULT_MATERIAL));
		}
		else {
		    boost::python::extract<ScenePtr> sc( obj );
            if (sc.check()){
                ScenePtr s = sc();
                scene->merge(s);
            }
            else {
			    Shape3DPtr val = boost::python::extract<Shape3DPtr>( obj );
			    scene->add( val );
            }
		}
  }
  return scene;
}

Shape3DPtr sc_getitem( Scene* s, size_t pos )
{
  if( pos < 0 && pos > -s->getSize() ) return s->getAt( s->getSize() + pos );
  if (pos < s->getSize()) return s->getAt( pos );
  else throw PythonExc_IndexError();
}

ShapePtr sc_find( Scene* s, size_t id )
{
  ShapePtr res = s->getShapeId( id );
  if (res) return res;
  else throw PythonExc_IndexError();
}

Shape3DPtr sc_findSceneObject( Scene* s, size_t id )
{
  Shape3DPtr res = s->getSceneObjectId( id );
  if (res) return res;
  else throw PythonExc_IndexError();
}

void sc_setitem( Scene* s, size_t pos, Shape3DPtr v )
{
  if( pos < 0 && pos > -s->getSize() ) return s->setAt( s->getSize() + pos, v );
  if (pos < s->getSize()) s->setAt( pos ,v );
  else throw PythonExc_IndexError();
}

void sc_delitem( Scene* s, size_t pos )
{
  Scene::iterator it;
  if( pos < 0 && pos > -s->getSize() ) { it = s->getEnd()+pos;  return s->remove( it ); }
  if (pos < s->getSize()) { it = s->getBegin() + pos; s->remove(it ); } 
  else throw PythonExc_IndexError();
}

ScenePtr sc_iadd(Scene* s ,Shape sh){
  s->add(sh);
  return s;
}

ScenePtr sc_iadd2(Scene* s ,Shape3DPtr sh){
  s->add(sh);
  return s;
}

ScenePtr sc_iadd3(Scene* s ,Scene* s2){
  s->merge(s2);
  return s;
}

ScenePtr sc_iadd4(Scene* s ,GeometryPtr sh){
	s->add(Shape(sh,Material::DEFAULT_MATERIAL));
	return s;
}

ScenePtr sc_add(Scene* s ,Scene* s2){
  ScenePtr s3 = ScenePtr(new Scene(*s));
  s3->merge(s2);
  return s3;
}
void sc_read(Scene* s ,const std::string& fname){
	s->read(fname);
}

void sc_save(Scene* s ,const std::string& fname){
	s->save(fname);
}

void sc_save2(Scene* s ,const std::string& fname,const std::string& format){
	s->save(fname,format);
}

uint32_t sc_index( Scene* sc, Shape3DPtr sh)
{
  sc->lock();
  Scene::iterator it = std::find(sc->getBegin(),sc->getEnd(),sh);
  if (it ==  sc->getEnd())
	{sc->unlock(); throw PythonExc_ValueError(); }
  uint32_t dist = std::distance(sc->getBegin(),it);
  sc->unlock();
  return dist;
}

void sc_remove( Scene* sc, Shape3DPtr sh)
{
  sc->lock();
  Scene::iterator it = std::find(sc->getBegin(),sc->getEnd(),sh);
  if (it ==  sc->getEnd())
	{sc->unlock(); throw PythonExc_ValueError(); }
  sc->remove(it);
  sc->unlock();
}

object sp_scenes(Scene::Pool * pool){
    return make_list<std::vector<ScenePtr> >(pool->getScenes())();
}

void export_Scene()
{
  class_<Scene,ScenePtr, boost::noncopyable> sc("Scene",init<const std::string&>("Scene() -> Create an empty scene"));
   scope scsc = sc.def(init< optional< unsigned int > >());
    sc.def(init< const Scene& >());
    sc.def( "__init__", make_constructor( sc_fromlist ) ) ;
	sc.def("__iadd__", &sc_iadd);
	sc.def("__iadd__", &sc_iadd2);
	sc.def("__iadd__", &sc_iadd3);
	sc.def("__iadd__", &sc_iadd4);
	sc.def("__add__", &sc_add);
    sc.def("add", (void (Scene::*)(const Shape &) ) &Scene::add );
    sc.def("add", (void (Scene::*)(const RefCountPtr<Shape3D> &) )&Scene::add);
    sc.def("add", &Scene::merge);
    sc.def("merge", &Scene::merge);
    sc.def("__len__", &Scene::getSize);
    sc.def("__getitem__", &sc_getitem);
    sc.def("__setitem__", &sc_setitem);
    sc.def("__delitem__", &sc_delitem);
    sc.def("clear", &Scene::clear);
    sc.def("merge", &Scene::merge);
    sc.def("find", &sc_find);
    sc.def("findSceneObject", &sc_findSceneObject);
    sc.def("index", &sc_index);
    sc.def("remove", &sc_remove);
    sc.def("isValid", (bool (Scene::*)() const)&Scene::isValid);
    sc.def("apply", &Scene::apply);
    sc.def("applyGeometryFirst", &Scene::applyGeometryFirst);
    sc.def("applyGeometryOnly", &Scene::applyGeometryOnly);
    sc.def("applyAppearanceFirst", &Scene::applyAppearanceFirst);
    sc.def("applyAppearanceOnly", &Scene::applyAppearanceOnly);
    sc.def("copy", &Scene::copy);
    sc.def("read", &sc_read);
    sc.def("save", &sc_save);
    sc.def("save", &sc_save2);
    sc.def("sort", &Scene::sort);
  	sc.enable_pickling();
  ;

  class_<Scene::Pool, boost::noncopyable>("Pool",no_init)
      .def("get", &Scene::Pool::get)
      .def("getScenes", &sp_scenes)
      ;

    sc.def("pool", &Scene::pool,return_value_policy<reference_existing_object>());
    sc.staticmethod("pool") ;
}


