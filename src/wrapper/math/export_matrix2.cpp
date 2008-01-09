/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
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

#include <plantgl/math/util_matrix.h>
#include <string>
#include <sstream>

#include "export_matrix.h"

TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;



string m2_repr( const Matrix2& m )
{
  stringstream ss;
  ss << "Matrix2( " << m(0,0) << ", " << m(0,1) << " ," <<endl;
  ss << "         " << m(1,0) << ", " << m(1,1) << " )";
  return ss.str();
}

void export_Matrix2()
{
  class_< Matrix2 >( "Matrix2", init< optional<real_t,real_t,real_t,real_t> >("Matrix2(f,f,f,f)") )
  .def(init<const Vector2&, const Vector2&>())
  .def(vector_matrix_func<Matrix2,Vector2>())
  .def( "data", &matrix_data<Matrix2,4> )
  .def( "__str__", m2_repr )
  .def( "__repr__", m2_repr )
  ;
}

