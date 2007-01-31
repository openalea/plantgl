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

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include <math/util_matrix.h>
#include <string>
#include <sstream>

TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;



GET_ELT(Matrix2)
GET_ELT(Matrix3)
SET_ELT(Matrix2)
SET_ELT(Matrix3)

string m2_str( const Matrix2& m )
{
  stringstream ss;
  ss << "[ " << m(0,0) << " " << m(0,1) << " ]"<<endl;
  ss << "[ " << m(1,0) << " " << m(1,1) << " ]";
  return ss.str();
}

string m2_repr( const Matrix2& m )
{
  stringstream ss;
  ss << "Matrix2[[ " << m(0,0) << " " << m(0,1) << " ],"<<endl;
  ss << "        [ " << m(1,0) << " " << m(1,1) << " ]]";
  return ss.str();
}

void class_Matrix2()
{
  class_< Matrix2 >( "Matrix2", init< optional<real_t,real_t,real_t,real_t> >("Matrix2(f,f,f,f)") )
  .def(init<const Vector2&, const Vector2&>())
  .def( self == self )
  .def( self != self )
  .def( self += self )
  .def( self -= self )
  .def( self *= real_t() )
  .def( self /= real_t() )
  .def( self + self )
  .def( self - self )
  .def( self * self )
  .def( self * other<Vector2>() )
  .def( self * real_t() )
  .def( self / real_t() )
  .def( "getColumn", &Matrix2::getColumn )
  .def( "getDiagonal", &Matrix2::getDiagonal )
  .def( "getRow", &Matrix2::getRow )
  .def( "isOrthogonal", &Matrix2::isOrthogonal )
  .def( "isSingular", &Matrix2::isSingular )
  .def( "isValid", &Matrix2::isValid )
  .def( "adjoint", (Matrix2 (*) ( const Matrix2& )) adjoint )
  .def( "__getitem__", Matrix2_getElt )
  .def( "__setitem__", Matrix2_setElt )
  .def( "__str__", m2_str )
  .def( "__repr__", m2_repr )
  ;
}
