#include <plantgl/scenegraph/geometry/elevationgrid.h>
#include <plantgl/tool/util_array2.h>

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <string>
#include <sstream>

#include "../util/export_refcountptr.h"
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


RealArray2Ptr geg_getHeightList( ElevationGrid* pl )
{
  return pl->getHeightList();
}

void export_ElevationGrid()
{
  class_< ElevationGrid, ElevationGridPtr, bases< Patch >,boost::noncopyable >
    ("ElevationGrid",init<RealArray2Ptr, optional<real_t,real_t,bool> >
     ("ElevationGrid(heights [,xspacing,yspacing,ccw])"))
    .def( "__init__", make_constructor( geg_fromlist ) ) 
    .def( "copy", &ElevationGrid::copy )
    .def( "getHeightList", geg_getHeightList )
    ;

  implicitly_convertible< ElevationGridPtr,PatchPtr >();
}
