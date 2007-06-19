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

#include <boost/python.hpp>
#include "../util/tuple_converter.h"

#include <plantgl/math/util_vector.h>
#include <string>
#include <sstream>

TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

std::string v4_repr( Vector4* v ) 
{ 
  stringstream ss; 
  ss << "Vector4(" << v->x() << ',' << v->y() << ',' << v->z() << ',' << v->w() << ')';
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

EXPORT_FUNCTION(v4, 4, Vector4 )

real_t v4_getz( Vector4* v )
{
  return v->z();
}

real_t v4_getw( Vector4* v )
{
  return v->w();
}

void v4_setw( Vector4* v, real_t val )
{
  v->w() = val;
}

void v4_setz( Vector4* v, real_t val )
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

VECTOR_SETGET(v4,Vector4,4);

struct v4_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Vector4 const& v)
	{
		return make_tuple(v.x(),v.y(),v.z(),v.w());
	}
};


void export_Vector4()
{
  class_< Vector4 >("Vector4", init< const Vector4 & >("Vector4(Vector4 v)",args("v")))
    .def(init< optional< real_t, real_t, real_t, real_t > >("Vector4(real_t x, real_t y, real_t z, real_t w)",args("x","y","z","w")))
    .def(init< const Vector3&,  real_t >("Vector4(Vector3 v, real_t w)",args("v","w")))
    .def( self + self )
    .def( -self )
    .def( self / other< real_t >() )
    .def( self * other< real_t >() )
    .def( self == self )
    .def( self != self )
    .def( self += self )
    .def( self /= other< real_t >() )
    .def( self - self )
    .def( "normalize", &Vector4::normalize, "Normalizes self and returns the norm before." )
    .def( "isNormalized", &Vector4::isNormalized, "Returns whether self is normalized." )
    .def( "isOrthogonalTo", &Vector4::isOrthogonalTo, args("v"), "Returns whether self is orthogonal to v." )
    .def( "isValid", &Vector4::isValid , "Returns whether self is valid.")
    // .def( "getMaxAbsCoord", &Vector4::getMaxAbsCoord, "Returns the index of the maximum absolute coordinate." )
    // .def( "getMinAbsCoord", &Vector4::getMinAbsCoord, "Returns the index of the minimum absolute coordinate." )
    .add_property( "x", v4_getx, v4_setx )
    .add_property( "y", v4_gety, v4_sety )
    .add_property( "z", v4_getz, v4_setz )
    .add_property( "w", v4_getw, v4_setw )
    .def( "__str__", v4_repr )
    .def( "__repr__", v4_repr )
    .def( "__getitem__", v4_getAt )
    .def( "__setitem__", v4_setAt )
    .def( "__len__", v4_size )
    .def( "__abs__",        (Vector4 (*) ( const Vector4&)) &abs , "Returns the absolute value of self.")
    .def( "__norm__",       (real_t (*) ( const Vector4&)) &norm , "Returns the norm of self.")
    .def( "__normL1__",     (real_t (*) ( const Vector4&)) &normL1 , "Returns the L1 (Manhattan) norm of self." )
    .def( "__normLinf__",   (real_t (*) ( const Vector4&)) &normLinf , "Returns the L-infinite norm of self.")
    .def( "__normSquared__",(real_t (*) ( const Vector4&)) &normSquared , "Returns the square of the norm of self.")
    .def( "__dir__",        (Vector4 (*) ( const Vector4&)) &direction , "Returns the direction of self.")
	.add_static_property( "ORIGIN", make_getter(Vector4::ORIGIN))
	.add_static_property( "OX", make_getter(Vector4::OX))
	.add_static_property( "OY", make_getter(Vector4::OY))
	.add_static_property( "OZ", make_getter(Vector4::OZ))
	.add_static_property( "OW", make_getter(Vector4::OW))
	.def_pickle(v4_pickle_suite());
	;


   pgltuple_from_tuple<Vector4,4>();

}
