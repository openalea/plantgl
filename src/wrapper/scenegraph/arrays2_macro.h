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

#include "../util/export_list.h"

// Array Macro
#define EXPORT_FUNCTION1( PREFIX, T, ARRAY) \
DEF_POINTEE( ARRAY ) \
\
     ARRAY##Ptr PREFIX##_fromlist( boost::python::object l )		\
{									\
  ARRAY* array= 0;							\
      object row_iter_obj = boost::python::object( handle<>( PyObject_GetIter( l.ptr() ) ) );\
      uint32_t rows= extract<uint32_t>(l.attr("__len__")());	\
      object col_obj= row_iter_obj.attr( "next" )();		\
      object col_iter_obj= boost::python::object( handle<>( PyObject_GetIter( col_obj.ptr() ) ) );		\
      uint32_t cols= extract<uint32_t>(col_obj.attr("__len__")());	\
      array= new ARRAY(rows,cols);					\
      for(uint32_t i=0; i < rows; ++i )							\
	 {								\
		if (i != 0) { \
			col_obj= row_iter_obj.attr( "next" )();			\
			col_iter_obj= boost::python::object( handle<>( PyObject_GetIter( col_obj.ptr() ) ) );		\
			uint32_t c= extract<uint32_t>(col_obj.attr("__len__")());	\
			if( c != cols ) throw PythonExc_IndexError();			\
		} \
	    for(uint32_t j=0; j < cols; ++j )						\
	    {								\
			object obj; \
			obj = col_iter_obj.attr( "next" )();		\
			T val = boost::python::extract<T>( obj );			\
			array->setAt(i,j,val);					\
	    } \
	}								\
  return array;								\
}									\
									\

#define EXPORT_FUNCTION2( PREFIX, T, ARRAY)			\
    T PREFIX##_getitem( ARRAY * array, boost::python::tuple indices )	\
{								\
  size_t i = extract<size_t>(indices[0])(), j = extract<size_t>(indices[1])(); \
  if( i < array->getRowsNb() && j < array->getColsNb() )	\
    return array->getAt( i, j );				\
  else throw PythonExc_IndexError();				\
}								\
								\
boost::python::object PREFIX##_getrow( ARRAY * array, size_t i)	\
{								\
  if( i < array->getRowsNb() )	\
    return make_list<std::vector<T> >(array->getRow( i ))();				\
  else throw PythonExc_IndexError();				\
}								\
								\
boost::python::object PREFIX##_getcolumn( ARRAY * array, size_t j)	\
{								\
  if( j < array->getColsNb() )	\
    return make_list<std::vector<T> >(array->getColumn( j ))();				\
  else throw PythonExc_IndexError();				\
}								\
								\
boost::python::object PREFIX##_getdiag( ARRAY * array)	\
{								\
  return make_list<std::vector<T> >(array->getDiagonal())();				\
}								\
								\
void PREFIX##_setitem( ARRAY * array, boost::python::tuple indices, T * v )\
{								 \
  size_t i = extract<size_t>(indices[0])(), j = extract<size_t>(indices[1])(); \
  if( i < array->getRowsNb() && j < array->getColsNb() )	 \
    array->setAt( i, j, *v );					 \
  else throw PythonExc_IndexError();				 \
}								 \
								 \
\
std::string PREFIX##_str( ARRAY * a )\
{\
  uint32_t r= a->getRowsNb(); uint32_t i= 0;				\
  std::stringstream ss;							\
  ss << #ARRAY <<"([";								\
  for( i= 0; i < r; i++ )						\
    {									\
      ss << "[";							\
      for(ARRAY::const_iterator it = a->getBeginRow(i); it != a->getEndRow(i); ++it){ \
            if (it != a->getBeginRow(i)) ss << ","; \
            ss << extract<std::string>(boost::python::str(boost::python::object(*it)))(); \
      } \
      ss << "]";				\
      if( i < r-1 ) ss << ",";						\
    }									\
  ss << "])";								\
  return ss.str();							\
}									\
									\
size_t PREFIX##_len( ARRAY * a )					\
{  return a->getRowsNb();}						\
               \
size_t PREFIX##_rownb( ARRAY * a )					\
{  return a->getRowsNb();}						\
               \
size_t PREFIX##_colnb( ARRAY * a )					\
{  return a->getColsNb();}						\

#define EXPORT_ARRAY( PREFIX, ARRAY,STRING )				\
  class_< ARRAY, ARRAY##Ptr, boost::noncopyable>( #ARRAY , init<optional<size_t,size_t> >(STRING) ) \
    .def( "__init__", make_constructor( PREFIX##_fromlist ) )		\
    .def( "__getitem__", &PREFIX##_getitem /*, return_internal_reference<1>()*/ ) \
    .def( "__getitem__", &PREFIX##_getrow  ) \
    .def( "__setitem__", &PREFIX##_setitem )				\
    .def( "__len__", &PREFIX##_len )					\
    .def( "__repr__", &PREFIX##_str )					\
    .def( "__str__", &PREFIX##_str )					\
    .def( "getRow", &PREFIX##_getrow  ) \
    .def( "getRowNb", &PREFIX##_rownb  ) \
    .def( "getColumn", &PREFIX##_getcolumn  ) \
    .def( "getColumnNb", &PREFIX##_colnb  ) \
    .def( "getDiagonal", &PREFIX##_getdiag  ) \
    ;									\

#define EXPORT_FUNCTION( PREFIX, T, ARRAY) \
  EXPORT_FUNCTION1( PREFIX, T, ARRAY) \
  EXPORT_FUNCTION2( PREFIX, T, ARRAY)

