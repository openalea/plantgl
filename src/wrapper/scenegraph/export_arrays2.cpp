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



#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/exception.h>

#include <plantgl/tool/util_array2.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#if WITH_BOOST_NUMPY
#include <boost/python/numpy.hpp>
#define np boost::python::numpy
#endif

#include "arrays2_macro.h"

PGL_USING_NAMESPACE

using namespace boost::python;
#define bp boost::python


EXPORT_FUNCTION( p2m, Point2Matrix )
EXPORT_FUNCTION( p3m, Point3Matrix )
EXPORT_FUNCTION( p4m, Point4Matrix )
EXPORT_FUNCTION( ra,  RealArray2 )

#if WITH_BOOST_NUMPY
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
#endif

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
#if WITH_BOOST_NUMPY
   .def("to_array",&array_to_nparray)
#endif
   .def("threshold_max_values",&threshold_max_values)
   .def("threshold_min_values",&threshold_min_values);

  EXPORT_CONVERTER(RealArray2);
}


