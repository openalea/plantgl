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


#include "../util/exception.h"
#include <boost/python.hpp>

#include <math/util_vector.h>
#include <math/util_math.h>
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

#define EXPORT_FUNCTION( PREFIX, SIZE, ARRAY) \
real_t PREFIX##_getx( ARRAY* v ) \
{ \
  return v->x(); \
} \
\
real_t PREFIX##_gety( ARRAY* v ) \
{ \
  return v->y(); \
} \
\
void PREFIX##_setx( ARRAY* v, real_t val ) \
{ \
  v->x() = val; \
} \
\
void PREFIX##_sety( ARRAY* v, real_t val ) \
{ \
  v->y() = val; \
} \

EXPORT_FUNCTION(v3, 3, Vector3 )

real_t v3_getz( Vector3* v )
{
  return v->z();
}

void v3_setz( Vector3* v, real_t val )
{
  v->z() = val;
}


#define VECTOR_SETGET(PREFIX,ARRAY,SIZE) \
real_t PREFIX##_size( ARRAY* v) { return SIZE; } \
real_t PREFIX##_getAt( ARRAY* v, size_t i ) \
{ \
  if (i < SIZE) return v->getAt(i); \
  else throw PythonExc_IndexError(); \
} \
void PREFIX##_setAt( ARRAY* v, size_t i, real_t val ) \
{ \
  if (i < SIZE) v->getAt(i) = val; \
  else throw PythonExc_IndexError(); \
} \

VECTOR_SETGET(v3,Vector3,3);

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


std::string sph_str(Vector3::Spherical * v){
  std::stringstream ss;
  ss << "Vector3::Spherical(" << v->radius << ',' << v->theta*GEOM_DEG << "°," << v->phi*GEOM_DEG << "°)";
  return ss.str();
}

std::string sph_repr(Vector3::Spherical * v){
  std::stringstream ss;
  ss << "Vector3::Spherical(" << v->radius << ',' << v->theta << ',' << v->phi << ')';
  return ss.str();
}

std::string cyl_str(Vector3::Cylindrical * v){
  std::stringstream ss;
  ss << "Vector3.Cylindrical(" << v->radius << ',' << v->theta*GEOM_DEG << "°," << v->z << ")";
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

void class_Vector3()
{
  {
  scope v3 = class_< Vector3 >("Vector3", init< const Vector3 & >("Vector3(Vector3 v)",args("v")))
    .def(init< optional< real_t, real_t, real_t > >("Vector3(real_t x, real_t y, real_t z)",args("x","y","z")))
    .def(init< const Vector3::Cylindrical& >("Vector3(Vector3.Cylindrical c)",args("c")))
    .def(init< const Vector3::Spherical& >("Vector3(Vector3.Spherical s)",args("s")))
    .def(init< const Vector2&, real_t >("Vector3(Vector2 v, real_t z)",args("v","z")))
    .def( self + self )
    .def( -self )
    .def( self / other< real_t >() )
    .def( self * other< real_t >() )
    .def( self == self )
    .def( self != self )
    .def( self += self )
    .def( self /= other< real_t >() )
    .def( self - self )
    .def( self * self )
    .def( self ^ self )
    .def( "normalize", &Vector3::normalize , "Normalizes self and returns the norm before.")
    .def( "isNormalized", &Vector3::isNormalized , "Returns whether self is normalized.")
    .def( "isOrthogonalTo", &Vector3::isOrthogonalTo, args("v"), "Returns whether self is orthogonal to v." )
    .def( "isValid", &Vector3::isValid , "Returns whether self is valid." )
    .def( "getMaxAbsCoord", &Vector3::getMaxAbsCoord, "Returns the index of the maximum absolute coordinate." )
    .def( "getMinAbsCoord", &Vector3::getMinAbsCoord, "Returns the index of the minimum absolute coordinate." )
    .add_property( "x", v3_getx, v3_setx )
    .add_property( "y", v3_gety, v3_sety )
    .add_property( "z", v3_getz, v3_setz )
    .def( "__str__", v3_repr )
    .def( "__repr__", v3_repr )
    .def( "__getitem__", v3_getAt )
    .def( "__setitem__", v3_setAt )
    .def( "__len__", v3_size )
    .def( "__abs__",        (Vector3 (*) ( const Vector3&)) abs , "Returns the absolute value of self.")
    .def( "__norm__",       (real_t (*) ( const Vector3&)) norm , "Returns the norm of self.")
    .def( "__normL1__",     (real_t (*) ( const Vector3&)) normL1 , "Returns the L1 (Manhattan) norm of self.")
    .def( "__normLinf__",   (real_t (*) ( const Vector3&)) normLinf , "Returns the L-infinite norm of self.")
    .def( "__normSquared__",(real_t (*) ( const Vector3&)) normSquared , "Returns the square of the norm of self.")
    .def( "__dir__",        (Vector3 (*) ( const Vector3&)) direction , "Returns the direction of self.")
    .add_static_property( "ORIGIN", make_getter(Vector3::ORIGIN))
    .add_static_property( "OX", make_getter(Vector3::OX))
    .add_static_property( "OY", make_getter(Vector3::OY))
    .add_static_property( "OZ", make_getter(Vector3::OZ))
	.def_pickle(v3_pickle_suite());
	;

  class_<Vector3::Cylindrical>("Cylindrical", init<const Vector3&>("Cylindrical(Vector3 v)",args("v")))
    .def(init< real_t,real_t,real_t>("Cylindrical(real_t radius, real_t theta, real_t z)",args("radius","theta","z")))
    .def("isValid",&Vector3::Cylindrical::isValid , "Returns whether self is valid.")
    .def("__str__",&cyl_str)
    .def("__repr__",&cyl_repr)
    .add_property("radius",cyl_getradius,cyl_setradius)
    .add_property("theta",cyl_gettheta,cyl_settheta)
    .add_property("z",cyl_getz,cyl_setz)
	.def_pickle(v3cyl_pickle_suite());
    ;

  class_<Vector3::Spherical>("Spherical", init<const Vector3&>("Spherical(Vector3 v)",args("v")))
    .def(init< real_t,real_t,real_t>("Spherical(real_t radius, real_t theta, real_t phi)",args("radius","theta","phi")))
    .def("isValid",&Vector3::Spherical::isValid, "Returns whether self is valid.")
    .def("__str__",&sph_str)
    .def("__repr__",&sph_repr)
    .add_property("radius",sph_getradius,sph_setradius)
    .add_property("theta",sph_gettheta,sph_settheta)
    .add_property("phi",sph_getphi,sph_setphi)
	.def_pickle(v3sph_pickle_suite());
    ;

  }
def("angle",(real_t (*)   ( const Vector3&, const Vector3&))angle , args("v1","v2"), "The angle between v1 and v2");
def("dot",  (real_t (*)   ( const Vector3&, const Vector3&)) dot , args("v1","v2"), "The dot product of v1 and v2");
def("cross",(Vector3 (*) ( const Vector3&, const Vector3&)) cross, args("v1","v2"), "The cross product of v1 and v2"  );


}

