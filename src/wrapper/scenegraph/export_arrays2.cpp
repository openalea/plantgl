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

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/exception.h>

#include <plantgl/tool/util_array2.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/numpy.hpp>


#include "arrays2_macro.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;
#define bp boost::python
#define np boost::python::numpy


EXPORT_FUNCTION( p2m, Point2Matrix )
EXPORT_FUNCTION( p3m, Point3Matrix )
EXPORT_FUNCTION( p4m, Point4Matrix )
EXPORT_FUNCTION( ra,  RealArray2 )

np::ndarray array_to_nparray(RealArray2 * data)
{
    np::dtype dt = np::dtype::get_builtin<real_t>();
    size_t s = sizeof(real_t);

    np::ndarray array = np::from_data(data->data(), 
                                      dt,
                                      bp::make_tuple(data->getColumnSize(), data->getRowSize()),
                                      bp::make_tuple(data->getRowSize()*s, s),
                                      bp::object());
    return array;
}

void threshold_max_values(RealArray2 * data, real_t maxvalue) {
    for(RealArray2::iterator it = data->begin(); it != data->end(); ++it)
        if (*it > maxvalue) { *it = maxvalue; }
}

void threshold_min_values(RealArray2 * data, real_t minvalue) {
    for(RealArray2::iterator it = data->begin(); it != data->end(); ++it)
        if (*it < minvalue) { *it = minvalue; }
}

void export_arrays2()
{

  EXPORT_ARRAY_CT( p2m, Point2Matrix );
  EXPORT_CONVERTER(Point2Matrix);

  EXPORT_ARRAY_CT( p3m, Point3Matrix );
  EXPORT_CONVERTER(Point3Matrix);

  EXPORT_ARRAY_CT( p4m, Point4Matrix );
  EXPORT_CONVERTER(Point4Matrix);

  EXPORT_ARRAY_BT( ra, RealArray2 )
   .def(numarray2_func<RealArray2>())
   .def("to_array",&array_to_nparray)
   .def("threshold_max_values",&threshold_max_values)
   .def("threshold_min_values",&threshold_min_values);
   
  EXPORT_CONVERTER(RealArray2);
}


