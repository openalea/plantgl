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
#include <plantgl/math/linearalgebra.h>
#include <plantgl/math/util_deformation.h>
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

object py_svd (const Matrix2& m){
    if (fabs(m(1,0) - m(0,1)) > 0 ){
        PyErr_SetString(PyExc_ValueError, "Non symmetrical Matrix" );
	    throw_error_already_set();
    }
    real_t lambda1, lambda2;
    Matrix2 rot = svd(m,lambda1, lambda2);
    return make_tuple(rot,make_tuple(lambda1, lambda2));
}

void export_Matrix2()
{
  class_< Matrix2 >( "Matrix2", init< optional<real_t,real_t,real_t,real_t> >("Matrix2(f,f,f,f)") )
  .def(init<const Vector2&, const Vector2&>())
  .def(vector_matrix_func<Matrix2,Vector2>())
  .def( "data", &matrix_data<Matrix2,4> )
  .def( "__str__", m2_repr )
  .def( "__repr__", m2_repr )
  .def( "svd",&py_svd, "Singular Value Decomposition of a Matrix2. Return rotation matrix and the two singular values.")
  //.def( "diagonal", &Matrix2::diagonal, args("val1","val2"))
  //.staticmethod("diagonal")
  .def( "rotation", &Matrix2::rotation, args("angle"))
  .staticmethod("rotation")
  .def( "linearTransformation", &Matrix2::linearTransformation, args("i1","j1","i2","j2"))
  .staticmethod("linearTransformation")
  ;

  def ("strain",(Matrix2 (*) (const Matrix2&))& strain,args("transformation"),"Return the engineering strain associated to the transformation") ;
  def ("strain",(Matrix2 (*) (const Vector2&,const Vector2&,const Vector2&,const Vector2&))& strain,args("i1","j1","i2","j2")) ;
  def ("stress",&stress,args("strain","material"),"Return the stress associated to a given strain using Hook's law");
}

