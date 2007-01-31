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

#include "export_matrix.h"
#include "../util/exception.h"

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include <math/util_matrix.h>
#include <string>
#include <sstream>

TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;



GET_ELT(Matrix3)
SET_ELT(Matrix3)

string m3_repr( const Matrix3& m )
{
  stringstream ss;
  ss << "Matrix3( " << m(0,0) << ", " << m(0,1) << ", " << m(0,2) << " ,"<<endl;
  ss << "         " << m(1,0) << ", " << m(1,1) << ", " << m(1,2) << " ,"<<endl;
  ss << "         " << m(2,0) << ", " << m(2,1) << ", " << m(2,2) << " )";
  return ss.str();
}

void class_Matrix3()
{
  class_< Matrix3 >( "Matrix3", init< optional< real_t,real_t,real_t,
			                        real_t,real_t, real_t,
		                                real_t,real_t, real_t > >
		     ("Matrix3(f,f,f,f,f,f,f,f,f)") )
    .def(init<const Vector3&, const Vector3&, const Vector3&>("Matrix3(vec3,vec3,vec3)"))
    .def(init<const Matrix2&>("Matrix3(matrix2)"))
    .def(init<const Matrix4&>("Matrix3(matrix4)"))
    .def( self == self )
    .def( self != self )
    .def( self += self )
    .def( self -= self )
    .def( self *= real_t() )
    .def( self /= real_t() )
    .def( self + self )
    .def( self - self )
    .def( self * self )
    .def( self * other<Vector3>() )
    .def( self * real_t() )
    .def( self / real_t() )
    .def( "getColumn", &Matrix3::getColumn )
    .def( "getDiagonal", &Matrix3::getDiagonal )
    .def( "getRow", &Matrix3::getRow )
    .def( "isOrthogonal", &Matrix3::isOrthogonal )
    .def( "isSingular", &Matrix3::isSingular )
    .def( "isValid", &Matrix3::isValid )
    .def( "adjoint", (Matrix3 (*) ( const Matrix3& )) adjoint )
    .def( "det", (real_t (*) ( const Matrix3& )) det )
    .def( "inverse", (Matrix3 (*) ( const Matrix3& )) inverse )
    .def( "transpose", (Matrix3 (*) ( const Matrix3& )) transpose )
    .def( "trace", (real_t (*) ( const Matrix3& )) trace )
    .def( "eulerAnglesZYX", &Matrix3::eulerAnglesZYX )
    .def( "eulerAnglesXYZ", &Matrix3::eulerAnglesXYZ )
    .def( "__getitem__", Matrix3_getElt )
    .def( "__setitem__", Matrix3_setElt )
    .def( "__str__", m3_repr )
    .def( "__repr__", m3_repr );
  def("scaling",&Matrix3::scaling);
  def("eulerRotationZYX",&Matrix3::eulerRotationZYX);
  def("eulerRotationXYZ",&Matrix3::eulerRotationXYZ);
  def("axisRotation",&Matrix3::axisRotation);
}

