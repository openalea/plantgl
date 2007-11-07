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


template<class Matrix>
real_t matrix_getElt( const Matrix & m, boost::python::tuple t ) 
{ 
  if( PyObject_Size(t.ptr()) != 2 ) throw PythonExc_ValueError(); 
  return m(boost::python::extract<uchar_t>(t),boost::python::extract<uchar_t>(t)); 
} 

template<class Matrix>
void matrix_setElt( Matrix & m, boost::python::tuple t, real_t v ) 
{ 
  if( PyObject_Size(t.ptr()) != 2 ) throw PythonExc_ValueError(); 
  m(boost::python::extract<uchar_t>(t),boost::python::extract<uchar_t>(t)) = v ; 
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
         .def( "adjoint", (Matrix (*) ( const Matrix& )) &adjoint )
         .def( "inverse", (Matrix (*) ( const Matrix& )) &inverse )
         .def( "det", (real_t (*) ( const Matrix& )) &det )
         .def( "transpose", (Matrix (*) ( const Matrix& )) &transpose )
         .def( "trace", (real_t (*) ( const Matrix& )) &trace )
         .def( "__getitem__", matrix_getElt<Matrix> )
         .def( "__setitem__", matrix_setElt<Matrix> )
         .add_static_property( "IDENTITY", make_getter(Matrix::IDENTITY))
         ;
    }
};


#endif

