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
#include <boost/python/make_constructor.hpp>


PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

string m4_repr( const Matrix4& m )
{
  stringstream ss;
  ss << "Matrix4( " << m(0,0) << ", " << m(0,1) << ", " << m(0,2) << ", " << m(0,3) << " ,"<<endl;
  ss << "         " << m(1,0) << ", " << m(1,1) << ", " << m(1,2) << ", " << m(1,3) << " ,"<<endl;
  ss << "         " << m(2,0) << ", " << m(2,1) << ", " << m(2,2) << ", " << m(2,3) << " ,"<<endl;
  ss << "         " << m(3,0) << ", " << m(3,1) << ", " << m(3,2) << ", " << m(3,3) << " )";
  return ss.str();
}

Matrix4* m4_init( boost::python::tuple t)
{
  int n= PyObject_Size(t.ptr());
  if( n == 0 )
    return new Matrix4();
  else
  if ( n <= 16 )
    {
      real_t m[16]={1.,0.,0.,0.,
            0.,1.,0.,0.,
            0.,0.,1.,0.,
            0.,0.,0.,1.};

      for( int i= 0; i < n; i++ )
        {
    extract<real_t> o(t[i]);
    if( o.check() )
      m[i]= o();
    else
      throw PythonExc_TypeError();
    }
      return new Matrix4(m);
    }
  else
    throw PythonExc_ValueError();
}

void m4_set( Matrix4 & m, boost::python::tuple t )
{
  if( PyObject_Size(t.ptr()) != 16 )
    throw PythonExc_ValueError();
  extract<real_t> m00(t[0]); extract<real_t> m01(t[1]); extract<real_t> m02(t[2]); extract<real_t> m03(t[3]);
  extract<real_t> m10(t[4]); extract<real_t> m11(t[5]); extract<real_t> m12(t[6]); extract<real_t> m13(t[7]);
  extract<real_t> m20(t[8]); extract<real_t> m21(t[9]); extract<real_t> m22(t[10]); extract<real_t> m23(t[11]);
  extract<real_t> m30(t[12]); extract<real_t> m31(t[13]); extract<real_t> m32(t[14]); extract<real_t> m33(t[15]);
  if ( m00.check() && m01.check() && m02.check()&& m03.check() &&
       m10.check() && m11.check() && m12.check()&& m13.check() &&
       m20.check() && m21.check() && m22.check()&& m23.check() &&
       m30.check() && m31.check() && m32.check()&& m33.check()  )
    m.set(m00(),m01(),m02(),m03(),
      m10(),m11(),m12(),m13(),
      m20(),m21(),m22(),m23(),
      m30(),m31(),m32(),m33());
  else
    throw PythonExc_TypeError();
}

boost::python::tuple mat4_getTransformation(Matrix4 * t)
{
    Vector3 scale,rotate,translate;
    t->getTransformation(scale,rotate,translate);
    return boost::python::make_tuple(scale,rotate,translate);
}

boost::python::tuple mat4_getTransformationB(Matrix4 * t)
{
    Vector3 scale,rotate,translate;
    t->getTransformationB(scale,rotate,translate);
    return boost::python::make_tuple(scale,rotate,translate);
}

boost::python::tuple mat4_getTransformation2(Matrix4 * t)
{
    Vector3 scale,rotate,translate;
    t->getTransformation2(scale,rotate,translate);
    return boost::python::make_tuple(scale,rotate,translate);
}

void export_Matrix4()
{
  class_< Matrix4 > m2( "Matrix4");
  m2.def("__init__", make_constructor(m4_init));
  m2.def(init<const Vector4&, const Vector4&, const Vector4&, const Vector4&>());
  m2.def(init<const Vector3&, const Vector3&, const Vector3&, const Vector3&>());
  m2.def(init<const Matrix3&>());
  m2.def(vector_matrix_func<Matrix4,Vector4>());
  m2.def( "data", &matrix_data<Matrix4,16> );
  m2.def( self * other<Vector3>() );
  m2.def( "__str__", m4_repr );
  m2.def( "__repr__", m4_repr );
  m2.def( "set", m4_set );
  m2.def( "setTransformation", &Matrix4::setTransformation );
  m2.def( "getTransformation", &mat4_getTransformation , "Return scaling, rotation and translation corresponding the decomposition of the matrix into R(rotate) * S(scale) + T(translate) where R( rotate=(az,ay,ax) ) is the product of 3 matrices Rz(az)Ry(ay)Rx(ax)");
  m2.def( "setTransformation2", &Matrix4::setTransformation2 );
  m2.def( "getTransformation2", &mat4_getTransformation2,  "Return scaling, rotation and translation corresponding the decomposition of the matrix into S(scale) * R(rotate) + T(translate) where R( rotate=(az,ay,ax) ) is the product of 3 matrices Rz(az)Ry(ay)Rx(ax)");
  m2.def( "getTransformationB", &mat4_getTransformationB, "Alternative method to getTransformation." );

  m2.def("translation",&Matrix4::translation);
  m2.staticmethod("translation");

}


