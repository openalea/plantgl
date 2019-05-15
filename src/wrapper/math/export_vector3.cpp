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
//#include <plantgl/math/util_math.h>

#include <string>
#include <sstream>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

std::string v3_repr( Vector3* v )
{
  stringstream ss;
  ss << "Vector3(" << v->x() << ',' << v->y() << ',' << v->z() << ')';
  return ss.str();
}




std::string sph_repr(Vector3::Spherical * v){
  std::stringstream ss;
  ss << "Vector3.Spherical(" << v->radius << ',' << v->theta << ',' << v->phi << ')';
  return ss.str();
}

std::string cyl_repr(Vector3::Cylindrical * v){
  std::stringstream ss;
  ss << "Vector3.Cylindrical(" << v->radius << ',' << v->theta << ',' << v->z << ')';
  return ss.str();
}

struct v3_pickle_suite : boost::python::pickle_suite
{
    static boost::python::tuple getinitargs(Vector3 const& v)
    {
        return boost::python::make_tuple(v.x(),v.y(),v.z());
    }
};

struct v3cyl_pickle_suite : boost::python::pickle_suite
{
    static boost::python::tuple getinitargs(Vector3::Cylindrical const& v)
    {
        return boost::python::make_tuple(v.radius,v.theta,v.z);
    }
};

struct v3sph_pickle_suite : boost::python::pickle_suite
{
    static boost::python::tuple getinitargs(Vector3::Spherical const& v)
    {
        return boost::python::make_tuple(v.radius,v.theta,v.phi);
    }
};


struct pglvec3_from_vec3 {
  pglvec3_from_vec3() {
    boost::python::converter::registry::push_back( &convertible, &construct, boost::python::type_id<Vector3>());
  }

  static void* convertible(PyObject* py_obj){
    boost::python::object bpy_obj( boost::python::handle<>( boost::python::borrowed( py_obj ) ) );
    if(boost::python::extract<Vector3::Cylindrical>(bpy_obj).check()||boost::python::extract<Vector3::Spherical>(bpy_obj).check())
        return py_obj;
    else return 0;
  }

  static void construct( PyObject* py_obj, boost::python::converter::rvalue_from_python_stage1_data* data){
      boost::python::object bpy_obj( boost::python::handle<>( boost::python::borrowed( py_obj ) ) );
      boost::python::extract<Vector3::Cylindrical> cyl(bpy_obj);
      Vector3 res;
      if(cyl.check())
          res = Vector3(cyl());
      else {
          res = Vector3(boost::python::extract<Vector3::Spherical>(bpy_obj)());
      }
    typedef boost::python::converter::rvalue_from_python_storage<Vector3> vector_storage_t;
    vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data );
    void* memory_chunk = the_storage->storage.bytes;
    Vector3 * result = new (memory_chunk)Vector3(res);
    data->convertible = memory_chunk;
  }
};


void export_Vector3()
{
  {
  scope v3 = class_< Vector3 >("Vector3", init< const Vector3 & >("Vector3(Vector3 v)",args("v")))
    .def(init< boost::python::optional< real_t, real_t, real_t > >("Vector3(real_t x, real_t y, real_t z)",args("x","y","z")))
    .def(init< const Vector3::Cylindrical& >("Vector3(Vector3.Cylindrical c)",args("c")))
    .def(init< const Vector3::Spherical& >("Vector3(Vector3.Spherical s)",args("s")))
    .def(init< const Vector2&, boost::python::optional<real_t> >("Vector3(Vector2 v, real_t z)",args("v","z")))
    .def( "__str__", v3_repr )
    .def( "__repr__", v3_repr )
    .def(vector_dim3_func<Vector3>())
    .def(vector_crossdot<Vector3>())
    .def_pickle(v3_pickle_suite())
    .def("radialAnisotropicNorm",&radialAnisotropicNorm)
    .def("anisotropicNorm",&anisotropicNorm)
    .def("anOrthogonalVector",&Vector3::anOrthogonalVector)
    .def("reflect",&Vector3::reflect)
    .def("refract",&Vector3::refract)
    ;

   pgltuple_from_tuple<Vector3,3>();
   pglvec3_from_vec3();

  class_<Vector3::Cylindrical>("Cylindrical", init<const Vector3&>("Cylindrical(Vector3 v)",args("v")))
    .def(init< real_t,real_t,real_t>("Cylindrical(real_t radius, real_t theta, real_t z)",args("radius","theta","z")))
    .def("isValid",&Vector3::Cylindrical::isValid , "Returns whether self is valid.")
    .def("__str__",&cyl_repr)
    .def("__repr__",&cyl_repr)
    .def_readwrite("radius",&Vector3::Cylindrical::radius)
    .def_readwrite("theta",&Vector3::Cylindrical::theta)
    .def_readwrite("z",&Vector3::Cylindrical::z)
    .def_pickle(v3cyl_pickle_suite());
    ;

  class_<Vector3::Spherical>("Spherical", init<const Vector3&>("Spherical(Vector3 v)",args("v")))
    .def(init< real_t,real_t,real_t>("Spherical(real_t radius, real_t theta, real_t phi)",args("radius","theta","phi")))
    .def("isValid",&Vector3::Spherical::isValid, "Returns whether self is valid.")
    .def("__str__",&sph_repr)
    .def("__repr__",&sph_repr)
    .def("spherical_distance",&Vector3::Spherical::spherical_distance)
    .def_readwrite("radius",&Vector3::Spherical::radius)
    .def_readwrite("theta",&Vector3::Spherical::theta)
    .def_readwrite("phi",&Vector3::Spherical::phi)
    .def_pickle(v3sph_pickle_suite());
    ;

  }
def("angle",(real_t (*)   ( const Vector3&, const Vector3&)) &angle , args("v1","v2"), "The angle between v1 and v2");
def("angle",(real_t (*)   ( const Vector3&, const Vector3&, const Vector3&)) &angle , args("v1","v2","axis"), "The angle around axis between v1 and v2 ");
def("cross",(Vector3 (*) ( const Vector3&, const Vector3&))  &cross, args("v1","v2"), "The cross product of v1 and v2"  );
def("cross",(Vector3 (*) ( const Vector3&, const Vector3&))  &cross, args("v1","v2"), "The cross product of v1 and v2"  );
def("spherical_distance",(real_t (*) ( real_t, real_t , real_t, real_t, real_t))  &spherical_distance, args("theta1","phi1","theta2","phi2","radius"), "The distance on a unit sphere of the 2 directions given by v1 and v2"  );


}

