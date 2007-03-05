/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): C. Pradal (christophe.pradal@cirad.fr)
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

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include <scenegraph/geometry/geometry.h>
#include <scenegraph/geometry/group.h>
#include <scenegraph/container/geometryarray2.h>
#include <scenegraph/geometry/polyline.h>
#include "../util/export_refcountptr.h"
#include "../util/export_property.h"
#include "../util/exception.h"
#include "../util/extract_list.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;
using namespace std;

DEF_POINTEE(Group)

GroupPtr gg_fromlist( boost::python::object l ) 
{ 
  return new Group(extract_pgllist<GeometryArray>(l)());
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

size_t gg_len( Group * a )
{  return a->getGeometryListSize();}

SETGET(Group,Skeleton,PolylinePtr);

void export_Group()
{
  
  class_< Group, GroupPtr, bases< Geometry >,boost::noncopyable >
    ("Group",no_init)
    .def( "__init__", make_constructor( gg_fromlist ) ) 
    .def( "__getitem__", gg_getitem /*, return_internal_reference<1>()*/ )
    .def( "__setitem__", gg_setitem )
    .def( "__len__", gg_len )
	.DEC_SETGET_WD(skeleton,Group,Skeleton,PolylinePtr);
    ;

  implicitly_convertible< GroupPtr, GeometryPtr >();
}
