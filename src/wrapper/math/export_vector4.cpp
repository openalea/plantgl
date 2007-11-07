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
#include "../util/tuple_converter.h"

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
    .def( "__str__", v4_repr )
    .def( "__repr__", v4_repr )
    .def(vector_dim4_func<Vector4>())
	.def_pickle(v4_pickle_suite())
	;


   pgltuple_from_tuple<Vector4,4>();

}
