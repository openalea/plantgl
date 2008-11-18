/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
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


#include "export_vector.h"
#include <plantgl/python/tuple_converter.h>
//#include <plantgl/math/util_math.h>

#include <string>
#include <sstream>

TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

std::string v3_repr( Vector3* v ) 
{ 
  stringstream ss; 
  ss << "Vector3(" << v->x() << ',' << v->y() << ',' << v->z() << ')';
  return ss.str(); 
} 


#define VECTOR_STRUCT_SG(PREFIX,ARRAY,MEMBER) \
real_t PREFIX##_get##MEMBER( ARRAY* v ) \
{  return v->MEMBER; } \
  \
void PREFIX##_set##MEMBER( ARRAY* v, real_t val ) \
{  v->MEMBER = val; } \

VECTOR_STRUCT_SG(cyl,Vector3::Cylindrical,radius);
VECTOR_STRUCT_SG(cyl,Vector3::Cylindrical,theta);
VECTOR_STRUCT_SG(cyl,Vector3::Cylindrical,z);
VECTOR_STRUCT_SG(sph,Vector3::Spherical,radius);
VECTOR_STRUCT_SG(sph,Vector3::Spherical,theta);
VECTOR_STRUCT_SG(sph,Vector3::Spherical,phi);



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
	static tuple getinitargs(Vector3 const& v)
	{
		return make_tuple(v.x(),v.y(),v.z());
	}
};

struct v3cyl_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Vector3::Cylindrical const& v)
	{
		return make_tuple(v.radius,v.theta,v.z);
	}
};

struct v3sph_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Vector3::Spherical const& v)
	{
		return make_tuple(v.radius,v.theta,v.phi);
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
    .def(init< optional< real_t, real_t, real_t > >("Vector3(real_t x, real_t y, real_t z)",args("x","y","z")))
    .def(init< const Vector3::Cylindrical& >("Vector3(Vector3.Cylindrical c)",args("c")))
    .def(init< const Vector3::Spherical& >("Vector3(Vector3.Spherical s)",args("s")))
    .def(init< const Vector2&, optional<real_t> >("Vector3(Vector2 v, real_t z)",args("v","z")))
    .def( "__str__", v3_repr )
    .def( "__repr__", v3_repr )
    .def(vector_dim3_func<Vector3>())
    .def(vector_crossdot<Vector3>())
	.def_pickle(v3_pickle_suite());
	;

   pgltuple_from_tuple<Vector3,3>();
   pglvec3_from_vec3();

  class_<Vector3::Cylindrical>("Cylindrical", init<const Vector3&>("Cylindrical(Vector3 v)",args("v")))
    .def(init< real_t,real_t,real_t>("Cylindrical(real_t radius, real_t theta, real_t z)",args("radius","theta","z")))
    .def("isValid",&Vector3::Cylindrical::isValid , "Returns whether self is valid.")
    .def("__str__",&cyl_repr)
    .def("__repr__",&cyl_repr)
    .add_property("radius",make_getter(&Vector3::Cylindrical::radius),make_setter(&Vector3::Cylindrical::radius))
    .add_property("theta",make_getter(&Vector3::Cylindrical::theta),make_setter(&Vector3::Cylindrical::theta))
    .add_property("z",make_getter(&Vector3::Cylindrical::z),make_setter(&Vector3::Cylindrical::z))
	.def_pickle(v3cyl_pickle_suite());
    ;

  class_<Vector3::Spherical>("Spherical", init<const Vector3&>("Spherical(Vector3 v)",args("v")))
    .def(init< real_t,real_t,real_t>("Spherical(real_t radius, real_t theta, real_t phi)",args("radius","theta","phi")))
    .def("isValid",&Vector3::Spherical::isValid, "Returns whether self is valid.")
    .def("__str__",&sph_repr)
    .def("__repr__",&sph_repr)
    .add_property("radius",make_getter(&Vector3::Spherical::radius),make_setter(&Vector3::Spherical::radius))
    .add_property("theta",make_getter(&Vector3::Spherical::theta),make_setter(&Vector3::Spherical::theta))
    .add_property("phi",make_getter(&Vector3::Spherical::phi),make_setter(&Vector3::Spherical::phi))
	.def_pickle(v3sph_pickle_suite());
    ;

  }
def("angle",(real_t (*)   ( const Vector3&, const Vector3&)) &angle , args("v1","v2"), "The angle between v1 and v2");
def("angle",(real_t (*)   ( const Vector3&, const Vector3&, const Vector3&)) &angle , args("v1","v2","axis"), "The angle around axis between v1 and v2 ");
def("cross",(Vector3 (*) ( const Vector3&, const Vector3&))  &cross, args("v1","v2"), "The cross product of v1 and v2"  );


}

