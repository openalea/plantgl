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

#ifndef __export_matrix_h__
#define __export_matrix_h__

#include <boost/python.hpp>
#include <boost/python/def_visitor.hpp>
#include <plantgl/python/exception.h>

using namespace boost::python;

template<class Matrix>
real_t matrix_getElt( const Matrix & m, boost::python::tuple t ) 
{ 
  if( PyObject_Size(t.ptr()) != 2 ) throw PythonExc_ValueError(); 
  return m(boost::python::extract<uchar_t>(t[0]),boost::python::extract<uchar_t>(t[1])); 
} 

template<class Matrix>
void matrix_setElt( Matrix & m, boost::python::tuple t, real_t v ) 
{ 
  if( PyObject_Size(t.ptr()) != 2 ) throw PythonExc_ValueError(); 
  m(boost::python::extract<uchar_t>(t[0]),boost::python::extract<uchar_t>(t[1])) = v ; 
} 

template<class Matrix>
Matrix matrix_adjoint(const Matrix& mat) { return adjoint(mat); }

template<class Matrix>
Matrix matrix_inverse(const Matrix& mat) { return inverse(mat); }

template<class Matrix>
real_t matrix_det(const Matrix& mat) { return det(mat); }

template<class Matrix>
Matrix matrix_transpose(const Matrix& mat) { return transpose(mat); }

template<class Matrix>
real_t matrix_trace(const Matrix& mat) { return trace(mat); }

template<class Matrix,int size>
boost::python::list 
matrix_data(const Matrix& mat) 
{ 
    boost::python::list l;
    for (const real_t * it = mat.getData() ; it != mat.getData()+size; ++it)
        l.append(*it);
    return l;
}

template<class Matrix, class Vector>
class vector_matrix_func : public boost::python::def_visitor<vector_matrix_func<Matrix,Vector> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {          
        c.def( self == self )
         .def( self != self )
         .def( self += self )
         .def( self -= self )
         .def( self *= self )
         .def( self *= real_t() )
         .def( self /= real_t() )
         .def( self + self )
         .def( self - self )
         .def( self * self )
         .def( self * other<Vector>() )
         .def( self * real_t() )
         .def( self / real_t() )
         .def( "getColumn", &Matrix::getColumn )
         .def( "getDiagonal", &Matrix::getDiagonal )
         .def( "getRow", &Matrix::getRow )
         .def( "isOrthogonal", &Matrix::isOrthogonal )
         .def( "isSingular", &Matrix::isSingular )
         .def( "isValid", &Matrix::isValid )
         .def( "adjoint", &matrix_adjoint<Matrix> )
         .def( "inverse", &matrix_inverse<Matrix> )
         .def( "det",  &matrix_det<Matrix> )
         .def( "transpose", &matrix_transpose<Matrix> )
         .def( "trace", &matrix_trace<Matrix> )
         .def( "__getitem__", matrix_getElt<Matrix> )
         .def( "__setitem__", matrix_setElt<Matrix> )
         .add_static_property( "IDENTITY", make_getter(Matrix::IDENTITY))
         ;
    }
};


#endif

