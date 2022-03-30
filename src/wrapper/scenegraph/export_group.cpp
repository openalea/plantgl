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




#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/geometry/group.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"
#include "arrays_macro.h"
#include <plantgl/python/exception.h>
#include <plantgl/python/extract_list.h>
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE

using namespace boost::python;
using namespace std;

DEF_POINTEE(Group)

GroupPtr gg_fromobject2( GeometryPtr o, GeometryPtr o2 )
{
  GeometryArrayPtr geometries = GeometryArrayPtr(new GeometryArray());
  geometries->push_back(o);
  geometries->push_back(o2);
  return GroupPtr(new Group(geometries));
}

GroupPtr gg_fromobject3( GeometryPtr o, GeometryPtr o2, GeometryPtr o3 )
{
  GeometryArrayPtr geometries = GeometryArrayPtr(new GeometryArray());
  geometries->push_back(o);
  geometries->push_back(o2);
  geometries->push_back(o3);
  return GroupPtr(new Group(geometries));
}

GroupPtr gg_fromobject4( GeometryPtr o, GeometryPtr o2, GeometryPtr o3, GeometryPtr o4 )
{
  GeometryArrayPtr geometries = GeometryArrayPtr(new GeometryArray());
  geometries->push_back(o);
  geometries->push_back(o2);
  geometries->push_back(o3);
  geometries->push_back(o4);
  return GroupPtr(new Group(geometries));
}

GroupPtr gg_fromobject5( GeometryPtr o, GeometryPtr o2, GeometryPtr o3, GeometryPtr o4, GeometryPtr o5 )
{
  GeometryArrayPtr geometries = GeometryArrayPtr(new GeometryArray());
  geometries->push_back(o);
  geometries->push_back(o2);
  geometries->push_back(o3);
  geometries->push_back(o4);
  geometries->push_back(o5);
  return GroupPtr(new Group(geometries));
}

GeometryPtr gg_getitem( Group * array, size_t i)
{
  if( i < array->getGeometryListSize() )
    return array->getGeometryListAt( i);
  else throw PythonExc_IndexError();
}

void gg_setitem( Group * array, size_t i, GeometryPtr v )
{
  if( i < array->getGeometryListSize())
    array->getGeometryListAt( i) = v ;
  else throw PythonExc_IndexError();
}

GroupPtr gg_getslice( Group * array, int beg, int end )
{
  if( beg >= -array->getGeometryListSize() && beg < 0  )  beg += array->getGeometryListSize();
  else if( beg >= array->getGeometryListSize() ) throw PythonExc_IndexError();
  if( end >= -array->getGeometryListSize() && end < 0  )  end += array->getGeometryListSize();
  else if( end > array->getGeometryListSize() ) throw PythonExc_IndexError();
  return GroupPtr(new Group(GeometryArrayPtr(new GeometryArray(array->getGeometryList()->begin()+beg,
                                                  array->getGeometryList()->begin()+end))));
}

size_t gg_len( Group * a )
{  return a->getGeometryListSize();}


void export_Group()
{

  class_< Group, GroupPtr, bases< Geometry >,boost::noncopyable >
    ("Group", "A Group of Geometry", init<const GeometryArrayPtr&, boost::python::optional<const PolylinePtr&> >
    ("Group(list geometryList [,Polyline skeleton])",args("geometryList","skeleton") ))
    .def( "__init__", make_constructor( gg_fromobject2 ) )
    .def( "__init__", make_constructor( gg_fromobject3 ) )
    .def( "__init__", make_constructor( gg_fromobject4 ) )
    .def( "__init__", make_constructor( gg_fromobject5 ) )
    .DEF_PGLBASE(Group)
    .def( "__getitem__", gg_getitem /*, return_internal_reference<1>()*/ )
    .def( "__getitem__", &array_getitem_slice<Group>, boost::python::return_value_policy<boost::python::manage_new_object>() )
    .def( "__getitem__",  &array_getitem_list<Group>, boost::python::return_value_policy<boost::python::manage_new_object>() )
    .def( "__setitem__",  &array_setitem<Group>   )
    .def( "__setitem__",  &array_setsliceitem<Group>   ) 
    .def( "__setitem__",  &array_setlistitem<Group>   ) 
    .def( "__setitem__",  &array_setsliceitem_list<Group>   ) 
    .def( "__setitem__",  &array_setlistitem_list<Group>   ) 
    .def( "__delitem__",  &array_delitem<Group>   )
    .def( "__delitem__",  &array_delitem_slice<Group>  ) 

    // .def( "__setitem__", gg_setitem )
    // .def( "__getslice__", gg_getslice )
    .def( "__len__", gg_len )
    .DEC_PTR_PROPERTY_WDV(skeleton,Group,Skeleton,PolylinePtr,DEFAULT_SKELETON)
    .DEC_PTR_PROPERTY(geometryList,Group,GeometryList,GeometryArrayPtr)
    ;

  implicitly_convertible< GroupPtr, GeometryPtr >();
}
