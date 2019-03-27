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



#include "export_vector.h"
#include <plantgl/python/tuple_converter.h>

#include <string>
#include <sstream>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

std::string v4_repr( Vector4* v )
{
  stringstream ss;
  ss << "Vector4(" << v->x() << ',' << v->y() << ',' << v->z() << ',' << v->w() << ')';
  return ss.str();
}


struct v4_pickle_suite : boost::python::pickle_suite
{
    static boost::python::tuple getinitargs(Vector4 const& v)
    {
        return boost::python::make_tuple(v.x(),v.y(),v.z(),v.w());
    }
};


void export_Vector4()
{
  class_< Vector4 >("Vector4", init< const Vector4 & >("Vector4(Vector4 v)",args("v")))
    .def(init< optional< real_t, real_t, real_t, real_t > >("Vector4(real_t x, real_t y, real_t z, real_t w)",args("x","y","z","w")))
    .def(init< const Vector2&,  optional<real_t,real_t> >("Vector4(Vector2 v, real_t z, real_t w)",args("v","z","w")))
    .def(init< const Vector3&,  optional<real_t> >("Vector4(Vector3 v, real_t w)",args("v","w")))
    .def( "__str__", v4_repr )
    .def( "__repr__", v4_repr )
    .def(vector_dim4_func<Vector4>())
    .def_pickle(v4_pickle_suite())
    ;


   pgltuple_from_tuple<Vector4,4>();

}
