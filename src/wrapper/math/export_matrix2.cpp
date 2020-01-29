/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */





#include <plantgl/math/util_matrix.h>
#include <plantgl/math/linearalgebra.h>
#include <plantgl/math/util_deformation.h>
#include <string>
#include <sstream>

#include "export_matrix.h"

PGL_USING_NAMESPACE
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
    return boost::python::make_tuple(rot,make_tuple(lambda1, lambda2));
}

void export_Matrix2()
{
  class_< Matrix2 >( "Matrix2", init< boost::python::optional<real_t,real_t,real_t,real_t> >("Matrix2(f,f,f,f)") )
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

