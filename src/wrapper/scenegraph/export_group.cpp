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

#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/geometry/group.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <plantgl/python/exception.h>
#include <plantgl/python/extract_list.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;
using namespace std;

DEF_POINTEE(Group)

GroupPtr gg_fromobject2( GeometryPtr o, GeometryPtr o2 ) 
{ 
  GeometryArrayPtr geometries = GeometryArrayPtr(new GeometryArray());
  geometries->pushBack(o);
  geometries->pushBack(o2);
  return GroupPtr(new Group(geometries));
}

GroupPtr gg_fromobject3( GeometryPtr o, GeometryPtr o2, GeometryPtr o3 ) 
{ 
  GeometryArrayPtr geometries = GeometryArrayPtr(new GeometryArray());
  geometries->pushBack(o);
  geometries->pushBack(o2);
  geometries->pushBack(o3);
  return GroupPtr(new Group(geometries));
}

GroupPtr gg_fromobject4( GeometryPtr o, GeometryPtr o2, GeometryPtr o3, GeometryPtr o4 ) 
{ 
  GeometryArrayPtr geometries = GeometryArrayPtr(new GeometryArray());
  geometries->pushBack(o);
  geometries->pushBack(o2);
  geometries->pushBack(o3);
  geometries->pushBack(o4);
  return GroupPtr(new Group(geometries));
}

GroupPtr gg_fromobject5( GeometryPtr o, GeometryPtr o2, GeometryPtr o3, GeometryPtr o4, GeometryPtr o5 ) 
{ 
  GeometryArrayPtr geometries = GeometryArrayPtr(new GeometryArray());
  geometries->pushBack(o);
  geometries->pushBack(o2);
  geometries->pushBack(o3);
  geometries->pushBack(o4);
  geometries->pushBack(o5);
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
  return GroupPtr(new Group(GeometryArrayPtr(new GeometryArray(array->getGeometryList()->getBegin()+beg,
                                                  array->getGeometryList()->getBegin()+end))));
}

size_t gg_len( Group * a )
{  return a->getGeometryListSize();}


void export_Group()
{
  
  class_< Group, GroupPtr, bases< Geometry >,boost::noncopyable >
    ("Group", "A Group of Geometry", init<const GeometryArrayPtr&, optional<const PolylinePtr&> >
    ("Group(list geometryList [,Polyline skeleton])",args("geometryList","skeleton") ))
    .def( "__init__", make_constructor( gg_fromobject2 ) ) 
    .def( "__init__", make_constructor( gg_fromobject3 ) ) 
    .def( "__init__", make_constructor( gg_fromobject4 ) ) 
    .def( "__init__", make_constructor( gg_fromobject5 ) ) 
    .def( "__getitem__", gg_getitem /*, return_internal_reference<1>()*/ )
    .def( "__setitem__", gg_setitem )
    .def( "__getslice__", gg_getslice )
    .def( "__len__", gg_len )
	.DEC_PTR_PROPERTY_WDV(skeleton,Group,Skeleton,PolylinePtr,DEFAULT_SKELETON)
	.DEC_PTR_PROPERTY(geometryList,Group,GeometryList,GeometryArrayPtr)
    ;

  implicitly_convertible< GroupPtr, GeometryPtr >();
}
