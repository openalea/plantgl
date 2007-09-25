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

#include <plantgl/scenegraph/geometry/elevationgrid.h>
#include <plantgl/tool/util_array2.h>

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <string>
#include <sstream>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"
#include "../util/exception.h"

using namespace boost::python;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DEF_POINTEE(ElevationGrid)

ElevationGridPtr geg_fromlist( boost::python::object l ) 
{ 
	object row_iter_obj = boost::python::object( handle<>( PyObject_GetIter( l.ptr() ) ) );
	uint32_t rows= extract<uint32_t>(row_iter_obj.attr("__len__")());	
	object col_obj= row_iter_obj.attr( "next" )();
	object col_iter_obj= boost::python::object( handle<>( PyObject_GetIter( col_obj.ptr() ) ) );
	uint32_t cols= extract<uint32_t>(col_iter_obj.attr("__len__")());	

	RealArray2 * array= new RealArray2(rows,cols);
    for(uint32_t i=0; i < rows; ++i )
	{
		if (i != 0){
			object col_obj= row_iter_obj.attr( "next" )();
			col_iter_obj= boost::python::object( handle<>( PyObject_GetIter( col_obj.ptr() ) ) );
			uint32_t c= extract<uint32_t>(col_iter_obj.attr("__len__")());	
			if( c != cols ) throw PythonExc_IndexError();
		}
	    for(uint32_t j=0; j < cols; ++j )
		{
			object obj = col_iter_obj.attr( "next" )();
			real_t val = boost::python::extract<real_t>( obj );
			array->setAt(i,j,val);
		}
	}								
	return ElevationGridPtr(new ElevationGrid(array));
}									

real_t eg_getHeightAt(ElevationGrid *  e, uint32_t i, uint32_t j)
{ return e->getHeightAt(i,j); }


void export_ElevationGrid()
{
  class_< ElevationGrid, ElevationGridPtr, bases< Patch >,boost::noncopyable >
    ("ElevationGrid",init<RealArray2Ptr, optional<real_t,real_t,bool> >
     ("ElevationGrid(heights [,xspacing,yspacing,ccw])"))
    .def( "__init__", make_constructor( geg_fromlist ) ) 
    .def( "copy", &ElevationGrid::copy )
    .def( "getPointAt", &ElevationGrid::getPointAt )
    .def( "getHeightAt", &eg_getHeightAt )
    .DEC_BT_PROPERTY_WD(xspacing,ElevationGrid,XSpacing,real_t)
    .DEC_BT_PROPERTY_WD(yspacing,ElevationGrid,YSpacing,real_t)
    .DEC_PTR_PROPERTY(heightList,ElevationGrid,HeightList,RealArray2Ptr)
    ;

  implicitly_convertible< ElevationGridPtr,PatchPtr >();
}
