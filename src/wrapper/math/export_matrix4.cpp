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

#include <plantgl/math/util_matrix.h>
#include <string>
#include <sstream>

TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

GET_ELT(Matrix4)
SET_ELT(Matrix4)

string m4_repr( const Matrix4& m )
{
  stringstream ss;
  ss << "Matrix4( " << m(0,0) << ", " << m(0,1) << ", " << m(0,2) << ", " << m(0,3) << " ,"<<endl;
  ss << "         " << m(1,0) << ", " << m(1,1) << ", " << m(1,2) << ", " << m(1,3) << " ,"<<endl;
  ss << "         " << m(2,0) << ", " << m(2,1) << ", " << m(2,2) << ", " << m(2,3) << " ,"<<endl;
  ss << "         " << m(3,0) << ", " << m(3,1) << ", " << m(3,2) << ", " << m(3,3) << " )";
  return ss.str();
}

Matrix4* m4_init( tuple t)
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

void m4_set( Matrix4 & m, tuple t )
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

tuple mat4_getTransformation(Matrix4 * t)
{
	Vector3 scale,rotate,translate;
	t->getTransformation(scale,rotate,translate);
	return make_tuple(scale,rotate,translate);
}

tuple mat4_getTransformationB(Matrix4 * t)
{
	Vector3 scale,rotate,translate;
	t->getTransformationB(scale,rotate,translate);
	return make_tuple(scale,rotate,translate);
}

tuple mat4_getTransformation2(Matrix4 * t)
{
	Vector3 scale,rotate,translate;
	t->getTransformation2(scale,rotate,translate);
	return make_tuple(scale,rotate,translate);
}

void export_Matrix4()
{
  class_< Matrix4 > m2( "Matrix4");
  m2.def("__init__", make_constructor(m4_init));
  m2.def(init<const Vector4&, const Vector4&, const Vector4&, const Vector4&>());
  m2.def(init<const Matrix3&>());
  m2.def( self == self );
  m2.def( self != self );
  m2.def( self += self );
  m2.def( self -= self );
  m2.def( self *= real_t() );
  m2.def( self *= self );
  m2.def( self * other<Vector3>() );
  m2.def( self * other<Vector4>() );
  m2.def( self * real_t() );
  m2.def( self *= self );
  m2.def( self *= self );
  m2.def( self /= real_t() );
  m2.def( self / real_t() );
  m2.def( self + self );
  m2.def( self - self );
  m2.def( self * self ) ;
  m2.def( "getColumn", &Matrix4::getColumn );
  m2.def( "getDiagonal", &Matrix4::getDiagonal );
  m2.def( "getRow", &Matrix4::getRow );
  m2.def( "isOrthogonal", &Matrix4::isOrthogonal );
  m2.def( "isSingular", &Matrix4::isSingular );
  m2.def( "isValid", &Matrix4::isValid );
  m2.def( "set", m4_set );
  m2.def( "setTransformation", &Matrix4::setTransformation );
  m2.def( "getTransformation", &mat4_getTransformation );
  m2.def( "setTransformation2", &Matrix4::setTransformation2 );
  m2.def( "getTransformation2", &mat4_getTransformation2 );
  m2.def( "getTransformationB", &mat4_getTransformationB );
  m2.def( "adjoint", (Matrix4 (*) ( const Matrix4& )) adjoint );
  m2.def( "det", (real_t (*) ( const Matrix4& )) det );
  m2.def( "inverse", (Matrix4 (*) ( const Matrix4& )) inverse );
  m2.def( "transpose", (Matrix4 (*) ( const Matrix4& )) transpose );
  m2.def( "trace", (real_t (*) ( const Matrix4& )) trace );
  m2.def( "__getitem__", Matrix4_getElt );
  m2.def( "__setitem__", Matrix4_setElt );
  m2.def( "__str__", m4_repr );
  m2.def( "__repr__", m4_repr );

  def("translation",&Matrix4::translation);

}


