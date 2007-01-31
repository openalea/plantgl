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


#include "exception.hh"

#include <boost/python.hpp>

#include <util_vector.h>
#include <util_math.h>
#include <iostream>
#include <string>
#include <sstream>

TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;



std::string v2_repr( Vector2* v ) 
{ 
  stringstream ss; 
  ss << "Vector2(" << v->x() << ',' << v->y() << ')';
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

EXPORT_FUNCTION(v2, 2, Vector2 )

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

VECTOR_SETGET(v2,Vector2,2);

#define VECTOR_STRUCT_SG(PREFIX,ARRAY,MEMBER) \
real_t PREFIX##_get##MEMBER( ARRAY* v ) \
{  return v->MEMBER; } \
  \
void PREFIX##_set##MEMBER( ARRAY* v, real_t val ) \
{  v->MEMBER = val; } \

VECTOR_STRUCT_SG(pol,Vector2::Polar,radius);
VECTOR_STRUCT_SG(pol,Vector2::Polar,theta);

std::string pol_str(Vector2::Polar * v){
  std::stringstream ss;
  ss << "Vector2::Polar(" << v->radius << ',' << v->theta*GEOM_DEG << "°)";
  return ss.str();
}

std::string pol_repr(Vector2::Polar * v){
  std::stringstream ss;
  ss << "Vector2::Polar(" << v->radius << ',' << v->theta << ')';
  return ss.str();
}


object pgl_py_normSquared(object obj){
	try { 
       return obj.attr( "__normSquared__" )();
	}
	catch( error_already_set ){ PyErr_Clear(); }
	object iter_obj = boost::python::object( handle<>( PyObject_GetIter( obj.ptr() ) ) );
	real_t val = 0;
    while( 1 )
       {
		object lobj; 
		try {  lobj = iter_obj.attr( "next" )(); 		}
		catch( error_already_set ){ PyErr_Clear(); break; }
        real_t lval = boost::python::extract<real_t>( lobj );
		val += lval * lval;
   }
   return object(val);
}

object pgl_py_norm(object obj){
	try { return obj.attr( "__norm__" )();	}
	catch( error_already_set ){ PyErr_Clear(); }
	return object(sqrt(extract<real_t>(pgl_py_normSquared(obj))));
}


object pgl_py_normL1(object obj){
	try { return obj.attr( "__normL1__" )(); }
	catch( error_already_set ){ PyErr_Clear(); }
	object iter_obj = boost::python::object( handle<>( PyObject_GetIter( obj.ptr() ) ) );
	real_t val = 0;
    while( 1 )
       {
		object lobj; 
		try {  lobj = iter_obj.attr( "next" )(); 		}
		catch( error_already_set ){ PyErr_Clear(); break; }
        real_t lval = boost::python::extract<real_t>( lobj );
		val += abs(lval);
   }
   return object(val);
}

object pgl_py_normLinf(object obj){
	try { return obj.attr( "__normLinf__" )(); }
	catch( error_already_set ){ PyErr_Clear(); }
	object iter_obj = boost::python::object( handle<>( PyObject_GetIter( obj.ptr() ) ) );
	real_t val = 0;
    while( 1 )
       {
		object lobj; 
		try {  lobj = iter_obj.attr( "next" )(); 		}
		catch( error_already_set ){ PyErr_Clear(); break; }
        real_t lval = abs(boost::python::extract<real_t>( lobj ));
		if(val < lval) val = lval;
   }
   return object(val);
}

object pgl_py_dir(object obj){
	try { return obj.attr( "__dir__" )(); }
	catch( error_already_set ){ PyErr_Clear(); }
	real_t f = extract<real_t>(pgl_py_norm(obj));
	list l;
	object iter_obj = boost::python::object( handle<>( PyObject_GetIter( obj.ptr() ) ) );
	real_t val = 0;
    while( 1 )
       {
		object lobj; 
		try {  lobj = iter_obj.attr( "next" )(); 		}
		catch( error_already_set ){ PyErr_Clear(); break; }
		lobj = lobj / f;
		l.append(lobj);
   }
   return l;
}

struct v2_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Vector2 const& v)
	{
		return make_tuple(v.x(),v.y());
	}
};

struct v3pol_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Vector2::Polar const& v)
	{
		return make_tuple(v.radius,v.theta);
	}
};

void class_Vector2()
{
  {
  scope v2 = class_< Vector2 >("Vector2", init< const Vector2 & >("Vector2(Vector2 v)",args("v")))
    .def(init< optional< real_t, real_t > >("Vector2(real_t x , real_t y)",args("x","y")))
	.def(init< const Vector2::Polar & >("Vector2(Polar p)",args("p")))
    .def(self_ns::str(self))
    .def( self + self )
    .def( -self )
    .def( self / other< real_t >() )
    .def( self * other< real_t >() )
    .def( self == self )
    .def( self != self )
    .def( self += self )
    .def( self /= other< real_t >() )
    .def( self - self )
    .def( self ^ self )
    .def( self * self )
    .def( "normalize", &Vector2::normalize, "Normalizes self and returns the norm before." )
    .def( "isNormalized", &Vector2::isNormalized , "Returns whether self is normalized." )
    .def( "isOrthogonalTo", &Vector2::isOrthogonalTo, args("v"), "Returns whether self is orthogonal to v." )
    .def( "isValid", &Vector2::isValid , "Returns whether self is valid.")
    .def( "getMaxAbsCoord", &Vector2::getMaxAbsCoord , "Returns the index of the maximum absolute coordinate.")
    .def( "getMinAbsCoord", &Vector2::getMinAbsCoord , "Returns the index of the minimum absolute coordinate.")
    .add_property( "x", v2_getx, v2_setx )
    .add_property( "y", v2_gety, v2_sety )
    .def( "__str__", v2_repr )
    .def( "__repr__", v2_repr )
    .def( "__getitem__", v2_getAt )
    .def( "__setitem__", v2_setAt )
    .def( "__len__", v2_size )
    .def( "__abs__",          (Vector2 (*) ( const Vector2&))abs ,         "Returns the absolute value of self.")
    .def( "__norm__",         (real_t  (*) ( const Vector2&))norm ,        "Returns the norm of self.")
    .def( "__normL1__",       (real_t  (*) ( const Vector2&))normL1 ,      "Returns the L1 (Manhattan) norm of self.")
    .def( "__normLinf__",     (real_t  (*) ( const Vector2&))normLinf ,    "Returns the L-infinite norm of self.")
    .def( "__normSquared__",  (real_t  (*) ( const Vector2&))normSquared , "Returns the square of the norm of self." )
    .def( "__dir__",          (Vector2 (*) ( const Vector2&))direction ,   "Returns the direction of self.")
    .add_static_property( "ORIGIN", make_getter(Vector2::ORIGIN))
    .add_static_property( "OX", make_getter(Vector2::OX))
    .add_static_property( "OY", make_getter(Vector2::OY))

	.def_pickle(v2_pickle_suite());
   ;

  class_<Vector2::Polar>("Polar", init<const Vector2&>("Polar(Vector2 v)", args("v")))
    .def(init< real_t,real_t>("Polar(real_t radius, real_t theta)",args("radius","theta")))
    .def("isValid",&Vector2::Polar::isValid, "Returns whether self is valid.")
    .def("__str__",&pol_str)
    .def("__repr__",&pol_repr)
    .add_property("radius",&pol_getradius,&pol_setradius)
    .add_property("theta",&pol_gettheta,&pol_settheta)
    ;
  }

  def("angle",(real_t (*)   ( const Vector2&, const Vector2&))angle , args("v1","v2"), "The angle between v1 and v2");
  def("dot",  (real_t (*)   ( const Vector2&, const Vector2&)) dot , args("v1","v2"), "The dot product of v1 and v2" );
  def("cross",(real_t (*) ( const Vector2&, const Vector2&)) cross , args("v1","v2"), "The cross product of v1 and v2");

  def("norm",     pgl_py_norm , args("v") , "The norm of the vector. If v.__norm__() exists, call it." );
  def("normL1",     pgl_py_normL1 , args("v") , "The L1 (Manhattan) norm of the vector. If v.__normL1__() exists, call it."  );
  def("normLinf",     pgl_py_normLinf , args("v") , "The L-infinite norm of the vector. If v.__normLinf__() exists, call it."  );
  def("normSquared",     pgl_py_normSquared , args("v") , "The square of the norm of the vector. If v.__normSquared__() exists, call it." );
  def("direction",  pgl_py_dir , args("v") , "The direction of the vector. Resulting vector is normed. If v.__dir__() exists, call it."  );

}

