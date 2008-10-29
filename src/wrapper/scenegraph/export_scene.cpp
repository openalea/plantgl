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

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <plantgl/python/export_list.h>
#include <plantgl/python/exception.h>
#include <plantgl/python/intrusive_ptr_python_helper.h>

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
  class_< SceneObject,SceneObjectPtr, boost::noncopyable >(
	  "SceneObject", 
	  "Abstract base class for all objects of the scenegraph.\n"
	  "It is named, has unique id and support reference counting.\n"
	  "It can support Action application.",
	  no_init)
 	.def("__del__",&pydel<SceneObject>)
    .def("getName", &SceneObject::getName, return_value_policy< copy_const_reference >())
    .def("isNamed", &SceneObject::isNamed)
    .def("setName", &SceneObject::setName)
    .add_property("name",get_sco_name,&SceneObject::setName)
    .def("isValid", &SceneObject::isValid)
    .def("apply", &SceneObject::apply)
    .def("deepcopy", &SceneObject::copy)
    .def("getId", &SceneObject::getId)
	.def("getPglReferenceCount",&RefCountObject::getReferenceCount)
	.enable_pickling()
    ;
}


ScenePtr sc_fromlist( boost::python::list l ) 
{ 
  ScenePtr scene = ScenePtr(new Scene());
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
			scene->add(Shape3DPtr(new Shape(g,Material::DEFAULT_MATERIAL)));
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

Shape3DPtr sc_getitem( Scene* s, int pos )
{
  // Shape3DPtr res;
  if( pos < 0 && pos > -(int)s->getSize() ) return s->getAt( s->getSize() + pos );
  else if (pos < s->getSize()) return s->getAt( pos );
  else throw PythonExc_IndexError();
  // return_py_reference(res);
}

/*
object sc_find( Scene* s, size_t id )
{
  ShapePtr res = s->getShapeId( id );
  if (res) return_py_reference(res);
  else throw PythonExc_IndexError();
}
*/

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

void sc_setitem( Scene* s, int pos, Shape3DPtr v )
{
  if( pos < 0 && pos > -(int)s->getSize() ) return s->setAt( s->getSize() + pos, v );
  if (pos < s->getSize()) s->setAt( pos ,v );
  else throw PythonExc_IndexError();
}

void sc_delitem( Scene* s, int pos )
{
  Scene::iterator it;
  if( pos < 0 && pos > -(int)s->getSize() ) { it = s->getEnd()+pos;  return s->remove( it ); }
  if (pos < s->getSize()) { it = s->getBegin() + pos; s->remove(it ); } 
  else throw PythonExc_IndexError();
}

ScenePtr sc_iadd1(ScenePtr s ,Shape3DPtr sh){
  if(sh)s->add(sh);
  return s;
}

ScenePtr sc_iadd2(ScenePtr s ,ScenePtr s2){
  if(s2)s->merge(s2);
  return s;
}

ScenePtr sc_iadd3(ScenePtr s ,GeometryPtr sh){
  if(sh)s->add(Shape3DPtr(new Shape(sh,Material::DEFAULT_MATERIAL)));
  return s;
}

ScenePtr sc_add(ScenePtr s ,ScenePtr s2){
  if(s && s2){
    ScenePtr s3 = ScenePtr(new Scene(*s));
    s3->merge(s2);
    return s3;
  }
  else if (s) return ScenePtr(new Scene(*s));
  else return ScenePtr(new Scene());
}

void sc_add2(Scene* s ,GeometryPtr sh){
  if(sh)s->add(Shape3DPtr(new Shape(sh,Material::DEFAULT_MATERIAL)));
}

void sc_read(Scene* s ,const std::string& fname){
	s->read(fname);
}

void sc_read2(Scene* s ,const std::string& fname, const std::string& format){
	s->read(fname,format);
}

void sc_save(Scene* s ,const std::string& fname){
	s->save(fname);
}

void sc_save2(Scene* s ,const std::string& fname,const std::string& format){
	s->save(fname,format);
}

uint_t sc_index( Scene* sc, Shape3DPtr sh)
{
  sc->lock();
  Scene::iterator it = std::find(sc->getBegin(),sc->getEnd(),sh);
  if (it ==  sc->getEnd())
	{sc->unlock(); throw PythonExc_ValueError(); }
  uint_t dist = std::distance(sc->getBegin(),it);
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

size_t py_sc_getId(Scene * sc) { return (size_t)sc; }

Scene::Pool& new_pool(object){ return Scene::pool(); }

void export_Scene()
{
  class_<Scene,ScenePtr, boost::noncopyable> sc("Scene",
	  "A 3D Scene defined as a list of objects of type of Shape.",
	  init<const std::string&>("Read a scene from file."));
   scope scsc = sc.def(init< optional< unsigned int > >("Create a scene with n empty elements."));
    sc.def(init< const Scene& >("Create an empty scene."));
    sc.def( "__init__", make_constructor( sc_fromlist ) ) ;
	sc.def("__iadd__", &sc_iadd1);
	sc.def("__iadd__", &sc_iadd2);
	sc.def("__iadd__", &sc_iadd3);
	sc.def("__add__", &sc_add);
    sc.def("add", (void (Scene::*)(const ShapePtr &) ) &Scene::add );
    sc.def("add", (void (Scene::*)(const Shape3DPtr &) )&Scene::add);
	sc.def("add", &sc_add2);
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
    sc.def("read", &sc_read2);
    sc.def("save", &sc_save);
    sc.def("save", &sc_save2);
    sc.def("sort", &Scene::sort);
	sc.def("getId",&py_sc_getId);
  	sc.enable_pickling();
  ;

  class_<Scene::Pool, boost::noncopyable>("Pool","The scene pool. Allow you to access all scene in memory using their id.",no_init)
      .def("get", &Scene::Pool::get, "get scene from id.")
      .def("__getitem__", &Scene::Pool::get, "get scene from id.")
      .def("getScenes", &sp_scenes , "get all scene.")
      ;

    sc.def("pool", &Scene::pool,return_value_policy<reference_existing_object>(),"Scene pool singleton access");
    sc.staticmethod("pool") ;
}


