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

#include <plantgl/math/util_matrix.h>
#include <string>
#include <sstream>

#include "export_matrix.h"


TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;


string m3_repr( const Matrix3& m )
{
  stringstream ss;
  ss << "Matrix3( " << m(0,0) << ", " << m(0,1) << ", " << m(0,2) << " ,"<<endl;
  ss << "         " << m(1,0) << ", " << m(1,1) << ", " << m(1,2) << " ,"<<endl;
  ss << "         " << m(2,0) << ", " << m(2,1) << ", " << m(2,2) << " )";
  return ss.str();
}


inline Matrix3 scaling3(const Vector3& v) { return Matrix3::scaling(v); }
inline Matrix3 scaling1(real_t v) { return Matrix3::scaling(v); }

object pyToAxisAngle(const Matrix3 *m) 
{ 
	Vector3 axis;
	real_t angle;
	m->toAxisAngle(axis,angle);
	return boost::python::make_tuple(axis,angle);
}

void export_Matrix3()
{
  class_< Matrix3 >( "Matrix3", init< optional< real_t,real_t,real_t,
			                        real_t,real_t, real_t,
		                                real_t,real_t, real_t > >
		     ("Matrix3(f,f,f,f,f,f,f,f,f)") )
    .def(init<const Vector3&, const Vector3&, const Vector3&>("Matrix3(vec3,vec3,vec3)"))
    .def(init<const Matrix2&>("Matrix3(matrix2)"))
    .def(init<const Matrix4&>("Matrix3(matrix4)"))
    .def(vector_matrix_func<Matrix3,Vector3>())
    .def( "data", &matrix_data<Matrix3,9> )
    .def( "eulerAnglesZYX", &Matrix3::eulerAnglesZYX )
    .def( "eulerAnglesXYZ", &Matrix3::eulerAnglesXYZ )
	.def( "toAxisAngle", &pyToAxisAngle)
    .def( "__str__", m3_repr )
    .def( "__repr__", m3_repr )
    .def("scaling",&scaling3)
    .def("scaling",&scaling1)
    .staticmethod("scaling")
    .def("eulerRotationZYX",&Matrix3::eulerRotationZYX)
    .staticmethod("eulerRotationZYX")
    .def("eulerRotationXYZ",&Matrix3::eulerRotationXYZ)
    .staticmethod("eulerRotationXYZ")
    .def("axisRotation",&Matrix3::axisRotation)
    .staticmethod("axisRotation")
    ;

  def("scaling",&scaling3);
  def("eulerRotationZYX",&Matrix3::eulerRotationZYX);
  def("eulerRotationXYZ",&Matrix3::eulerRotationXYZ);
  def("axisRotation",&Matrix3::axisRotation);
}

