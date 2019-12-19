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
#include <string>
#include <sstream>

#include "export_matrix.h"


PGL_USING_NAMESPACE
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
  class_< Matrix3 >( "Matrix3", init< boost::python::optional< real_t,real_t,real_t,
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

