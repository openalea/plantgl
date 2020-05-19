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
#include <plantgl/python/pyinterpreter.h>
#include <boost/python/make_constructor.hpp>
#include "export_sceneobject.h"
#include "arrays_macro.h"

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Scene)

ScenePtr sc_fromlist( boost::python::list l )
{
  ScenePtr scene = ScenePtr(new Scene());
  object iter_obj = boost::python::object( boost::python::handle<PyObject>( PyObject_GetIter( l.ptr() ) ) );
  while( 1 )
  {
        object obj;
        try {
          obj = boost::python::object(boost::python::handle<PyObject>(PyIter_Next(iter_obj.ptr())));
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
  if( pos < 0 && pos > -(int)s->size() ) return s->getAt( s->size() + pos );
  else if (pos < s->size()) return s->getAt( pos );
  else throw PythonExc_IndexError();
}


ShapePtr sc_find( Scene* s, size_t id )
{
  ShapePtr res = s->findShapeId( id );
  if (res) return res;
  else throw PythonExc_IndexError();
}

Shape3DPtr sc_findSceneObject( Scene* s, size_t id )
{
  Shape3DPtr res = s->findSceneObjectId( id );
  if (res) return res;
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
  Scene::iterator it = std::find(sc->begin(),sc->end(),sh);
  if (it ==  sc->end())
    {sc->unlock(); throw PythonExc_ValueError(); }
  uint_t dist = std::distance(sc->begin(),it);
  sc->unlock();
  return dist;
}

void sc_remove( Scene* sc, Shape3DPtr sh)
{
  sc->lock();
  Scene::iterator it = std::find(sc->begin(),sc->end(),sh);
  if (it ==  sc->end())
    {sc->unlock(); throw PythonExc_ValueError(); }
  sc->erase(it);
  sc->unlock();
}


boost::python::dict sc2dict(Scene * sc) {
    boost::python::dict result;
    for(Scene::const_iterator it = sc->begin(); it != sc->end(); ++it)
    {
        ShapePtr sh = dynamic_pointer_cast<Shape>(*it);
        uint32_t sid = (is_valid_ptr(sh)?sh->getId():(*it)->getObjectId());
        boost::python::list clist(result.get(sid,boost::python::list()));
        clist.append(*it);
        result[sid] = clist;
    }
    return result;
}

object sp_scenes(Scene::Pool * pool){
    return make_list(pool->getScenes())();
}

Scene::Pool& new_pool(object){ return Scene::pool(); }

bool scene_is_valid(Scene * sc){
    PyStateSaver s;
    return sc->isValid();
}



void export_Scene()
{
  class_<Scene,ScenePtr, bases<RefCountObject>, boost::noncopyable> sc("Scene",
      "A 3D Scene defined as a list of objects of type of Shape.",
      init<const std::string&>("Read a scene from file."));
   scope scsc = sc.def(init< boost::python::optional< unsigned int > >("Create a scene with n empty elements."));
    sc.def(init< const Scene& >("Create an empty scene."));
    sc.def( "__init__", make_constructor( sc_fromlist ) ) ;
    sc.def("__iadd__", &sc_iadd1);
    sc.def("__iadd__", &sc_iadd2);
    sc.def("__iadd__", &sc_iadd3);
    sc.def("__add__", &sc_add);
    sc.def("todict", &sc2dict);
    sc.def("add", (void (Scene::*)(const ShapePtr &) ) &Scene::add );
    sc.def("add", (void (Scene::*)(const Shape3DPtr &) )&Scene::add);
    sc.def("add", &sc_add2);
    sc.def("add", &Scene::merge);
    sc.def("merge", &Scene::merge);
    sc.def("__len__", &Scene::size);

    sc.def("__getitem__", &sc_getitem);
    sc.def( "__getitem__", &array_getitem_slice<Scene>, boost::python::return_value_policy<boost::python::manage_new_object>() );
    sc.def( "__getitem__",  &array_getitem_list<Scene>, boost::python::return_value_policy<boost::python::manage_new_object>() );
    sc.def( "__setitem__",  &array_setitem<Scene>   );
    sc.def( "__setitem__",  &array_setsliceitem<Scene>   ) ;
    sc.def( "__setitem__",  &array_setlistitem<Scene>   ) ;
    sc.def( "__setitem__",  &array_setsliceitem_list<Scene>   ) ;
    sc.def( "__setitem__",  &array_setlistitem_list<Scene>   ) ;
    sc.def( "__delitem__",  &array_delitem<Scene>   ) ;
    sc.def( "__delitem__",  &array_delitem_slice<Scene>  ) ;

    /*sc.def("__getitem__", &sc_getitem);
    sc.def("__getitem__", &sc_getitemslice );
    sc.def("__setitem__", &sc_setitem);
    //sc.def("__setitem__", &sc_setitemslice);
    sc.def("__delitem__", &sc_delitem);
    //sc.def("__delitem__", &sc_delitemslice);*/

    sc.def("clear", &Scene::clear);
    sc.def("merge", &Scene::merge);
    sc.def("find", &sc_find);
    sc.def("findSceneObject", &sc_findSceneObject);
    sc.def("index", &sc_index);
    sc.def("remove", &sc_remove);
    sc.def("isValid",  (bool (Scene::*)() const)&Scene::isValid);
    sc.def("apply", &Scene::apply);
    sc.def("applyGeometryFirst", &Scene::applyGeometryFirst);
    sc.def("applyGeometryOnly", &Scene::applyGeometryOnly);
    sc.def("applyAppearanceFirst", &Scene::applyAppearanceFirst);
    sc.def("applyAppearanceOnly", &Scene::applyAppearanceOnly);
    sc.def("deepcopy", (ScenePtr (Scene::*)() const)&Scene::deepcopy);
    sc.def("deepcopy", (ScenePtr (Scene::*)(DeepCopier&) const)&Scene::deepcopy,args("copier"));
    sc.def("read", &sc_read);
    sc.def("read", &sc_read2);
    sc.def("save", &sc_save);
    sc.def("save", &sc_save2);
    sc.def("sort", &Scene::sort);
    sc.def("getId",&RefCountObject::uid);
    sc.def("getPglReferenceCount",&RefCountObject::use_count);
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


