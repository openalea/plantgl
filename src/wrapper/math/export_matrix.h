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

