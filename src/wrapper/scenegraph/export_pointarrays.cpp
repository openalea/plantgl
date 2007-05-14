/*
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
*/
#include <tool/util_array.h>
#include <tool/util_tuple.h>
#include <scenegraph/container/pointarray.h>
#include <scenegraph/transformation/ifs.h>
#include <scenegraph/transformation/mattransformed.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include "../util/exception.h"
#include "../util/export_refcountptr.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;

#include "arrays_macro.h"

EXPORT_FUNCTION( p2a, Vector2, Point2Array )
EXPORT_FUNCTION( p3a, Vector3, Point3Array )
EXPORT_FUNCTION( p4a, Vector4, Point4Array )
EXPORT_FUNCTION( t4a, Transform4Ptr, Transform4Array )
EXPORT_FUNCTION( m4a, Matrix4, Matrix4Array )

EXPORT_NUMPY( p2a, Vector2, Point2Array, 0, 2, real_t )
EXPORT_NUMPY( p3a, Vector3, Point3Array, 0, 3, real_t )
EXPORT_NUMPY( p4a, Vector4, Point4Array, 0, 4, real_t )

Point3Array * p3_from_p2(Point2Array * pts, real_t z)
{
    return new Point3Array(*pts,z);
}

Point4Array * p4_from_p2(Point2Array * pts, real_t z, real_t w)
{
    return new Point4Array(*pts,z,w);
}

Point4Array * p4_from_p3(Point3Array * pts,  real_t w)
{
    return new Point4Array(*pts,w);
}

void export_pointarrays()
{
  EXPORT_ARRAY( p2a, Point2Array, "Point2Array([Vector2(x,y),...])")
    DEFINE_NUMPY( p2a );
  EXPORT_CONVERTER(p2a);

  EXPORT_ARRAY( p3a, Point3Array, "Point3Array([Vector3(x,y,z),...])")
    .def( "__init__", make_constructor( p3_from_p2 ), "Point3Array(Point2Array a, z)" ) 
    DEFINE_NUMPY( p3a );
  EXPORT_CONVERTER(p3a);

  EXPORT_ARRAY( p4a, Point4Array, "Point4Array([Vector4(x,y,z,w),...])")
    .def( "__init__", make_constructor( p4_from_p2 ), "Point4Array(Point2Array a, z, w)" ) 
    .def( "__init__", make_constructor( p4_from_p3 ), "Point4Array(Point3Array a, w)" ) 
    DEFINE_NUMPY( p4a );
  EXPORT_CONVERTER(p4a);

  EXPORT_ARRAY( t4a, Transform4Array,"Transform4Array([Transform4(...),...])" );
  EXPORT_CONVERTER(t4a);
  EXPORT_ARRAY( m4a, Matrix4Array,"Matrix4Array([Matrix4(...),...])" );
  EXPORT_CONVERTER(m4a);
}



