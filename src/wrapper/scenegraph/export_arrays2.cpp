#include "arrays2.h"
#include "tuple.h"
#include "macro_refptr.h"
#include "vector.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

#include <util_array2.h>
#include <util_vector.h>
#include <geom_pointmatrix.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include "exception.hh"

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;

// Index3Array

#include "arrays2_macro.h"

EXPORT_FUNCTION( p2m, Vector2, Point2Matrix )
EXPORT_FUNCTION( p3m, Vector3, Point3Matrix )
EXPORT_FUNCTION( p4m, Vector4, Point4Matrix )
//EXPORT_FUNCTION( ra, real_t, RealArray2 )

void class_arrays2()
{
  define_stl_exceptions();

  EXPORT_ARRAY( p2m, Point2Matrix, "Point2Matrix(rows,cols)" )
  EXPORT_ARRAY( p3m, Point3Matrix, "Point3Matrix(rows,cols)" )
  EXPORT_ARRAY( p4m, Point4Matrix, "Point4Matrix(rows,cols)" )
    //  EXPORT_ARRAY( ra, RealArray2, "RealArray2(rows,cols)" )
}


