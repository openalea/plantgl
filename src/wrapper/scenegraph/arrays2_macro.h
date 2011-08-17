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

#include <plantgl/python/export_list.h>
#include <plantgl/python/extract_list.h>
#include <plantgl/math/util_matrixmath.h>
#include <boost/python.hpp>
#include <sstream>

template<class T>
RCPtr<T> extract_array2_from_list( boost::python::object l )
{
  if (l.ptr() == Py_None) return RCPtr<T>();
  boost::python::object row_iter_obj = boost::python::object( boost::python::handle<>( PyObject_GetIter( l.ptr() ) ) );
  uint_t rows= boost::python::extract<uint_t>(l.attr("__len__")());
  boost::python::object col_obj= row_iter_obj.attr( "next" )();
  uint_t cols= boost::python::extract<uint_t>(col_obj.attr("__len__")());
  boost::python::object col_iter_obj= boost::python::object( boost::python::handle<>( PyObject_GetIter( col_obj.ptr() ) ) );
  RCPtr<T> array= RCPtr<T>(new T(rows,cols));

  for(uint_t i=0; i < rows; ++i )	
	 {	
		if (i != 0) {
			col_obj= row_iter_obj.attr( "next" )();	
			col_iter_obj= boost::python::object( boost::python::handle<>( PyObject_GetIter( col_obj.ptr() ) ) );
			uint_t c= boost::python::extract<uint_t>(col_obj.attr("__len__")());
			if( c != cols ) throw PythonExc_IndexError("Array2 has invalid number of element in a row.");
		}
	    for(uint_t j=0; j < cols; ++j )
	    {
			boost::python::object obj;
			obj = col_iter_obj.attr( "next" )();
            array->setAt(i,j,boost::python::extract<typename T::element_type>(obj));
	    }
	}
  return array;
}

template<class T>
struct array2_from_list {
  array2_from_list() { 
	boost::python::converter::registry::push_back( &convertible, &construct, boost::python::type_id<T>()); 
  } 
  static void* convertible(PyObject* py_obj){ 
    if( py_obj !=  Py_None && !PySequence_Check( py_obj ) ) return 0; 
    return py_obj; 
  } 
  static void construct( PyObject* py_obj, boost::python::converter::rvalue_from_python_stage1_data* data){ 
   typedef boost::python::converter::rvalue_from_python_storage<T> vector_storage_t;  
   vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data ); 
   void* memory_chunk = the_storage->storage.bytes; 
   if (py_obj != Py_None){
    boost::python::list py_sequence( boost::python::handle<>( boost::python::borrowed( py_obj ) ) ); 
    RCPtr<T> result = extract_array2_from_list<T>(py_sequence); 
    new (memory_chunk) T (*result);
   }
   else { new (memory_chunk) T(0); }
   data->convertible = memory_chunk; 
  } 
}; 

template<class T>
struct array2_ptr_from_list { 
  array2_ptr_from_list() { 
	boost::python::converter::registry::push_back( &convertible, &construct, boost::python::type_id< RCPtr<T> >()); 
  } 
  static void* convertible(PyObject* py_obj){ 
    if( py_obj !=  Py_None && !PySequence_Check( py_obj ) ) return 0; 
    return py_obj; 
  } 
  static void construct( PyObject* py_obj, boost::python::converter::rvalue_from_python_stage1_data* data){ 
   typedef boost::python::converter::rvalue_from_python_storage<T> vector_storage_t;  
   vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data ); 
   void* memory_chunk = the_storage->storage.bytes; 
   RCPtr<T> result;
   if (py_obj != Py_None){
    boost::python::list py_sequence( boost::python::handle<>( boost::python::borrowed( py_obj ) ) ); 
    result = extract_array2_from_list<T>(py_sequence); 
   }
   new (memory_chunk) RCPtr<T> (result); 
   data->convertible = memory_chunk; 
  } 
}; 



template<class T>
typename T::element_type array2_bt_getitem( T * array, boost::python::tuple indices )
{
  size_t i = boost::python::extract<size_t>(indices[0])();
  size_t j = boost::python::extract<size_t>(indices[1])();
  if( i < array->getRowNb() && j < array->getColumnNb() )
    return array->getAt( i, j );
  else {
      std::stringstream ss;
      ss << (int)i << "," << (int)j << " should be in [0," << array->getRowNb() << "]x[0," << array->getColumnNb() << "]";
      throw PythonExc_IndexError(ss.str().c_str());
  }
}

template<class T>
typename T::element_type& array2_ct_getitem( T * array, boost::python::tuple indices )
{
  size_t i = boost::python::extract<size_t>(indices[0])();
  size_t j = boost::python::extract<size_t>(indices[1])();
  if( i < array->getRowNb() && j < array->getColumnNb() )
    return array->getAt( i, j );
  else {
      std::stringstream ss;
      ss << (int)i << "," << (int)j << " should be in [0," << array->getRowNb() << "]x[0," << array->getColumnNb() << "]";
      throw PythonExc_IndexError(ss.str().c_str());
  }
}


template<class T>
boost::python::object array2_getrow( T * array, size_t i)
{
  if( i < array->getRowNb() )
    return make_list<std::vector<typename T::element_type> >(array->getRow( i ))();
  else throw PythonExc_IndexError();
}


template<class T>
class Array2Iter {
public:
    T * __array;
    size_t __current;

    Array2Iter(T* a) : __array(a), __current(0) { }

    boost::python::object next() {
        if( __current < __array->getRowNb() ){
            return make_list<std::vector<typename T::element_type> >(__array->getRow( __current++ ))();
        }
        else throw PythonExc_StopIteration();
    }
    inline void nothing() {  }
};

template<class T>
Array2Iter<T> array2_getiter( T * array) { return Array2Iter<T>(array); }


template<class T>
boost::python::object array2_getcolumn( T * array, size_t j)
{
  if( j < array->getColumnNb() )
    return make_list<std::vector<typename T::element_type> >(array->getColumn( j ))();
  else throw PythonExc_IndexError();
}

template<class T>
boost::python::object array2_getdiag( T * array)
{
  return make_list<std::vector<typename T::element_type> >(array->getDiagonal())();
}

template<class T>
void array2_setitem( T * array, boost::python::tuple indices, typename T::element_type v )
{
  size_t i = boost::python::extract<size_t>(indices[0])(), j = boost::python::extract<size_t>(indices[1])();
  if( i < array->getRowNb() && j < array->getColumnNb() )
    array->setAt( i, j, v );
  else throw PythonExc_IndexError();
}

template<class T>
void array2_insertRow( T * array, size_t i, boost::python::object v )
{
  std::vector<typename T::element_type> row = extract_vec<typename T::element_type>(v)();
  if( row.size() != array->getRowSize() ) throw PythonExc_ValueError();
  if( i <= array->getRowNb() )
    array->insertRow( i, row );
  else throw PythonExc_IndexError();
}

template<class T>
void array2_pushRow( T * array, boost::python::object v )
{
  std::vector<typename T::element_type> row = extract_vec<typename T::element_type>(v)();
  if( row.size() != array->getRowSize() ) throw PythonExc_ValueError();
  array->insertRow(array->getRowNb(), row );
}

template<class T>
void array2_insertColumn( T * array, size_t i, boost::python::object v )
{
  std::vector<typename T::element_type> col = extract_vec<typename T::element_type>(v)();
  if( col.size() != array->getColumnSize() ) throw PythonExc_ValueError();
  if( i <= array->getColumnNb() )
    array->insertColumn( i, col.begin(),col.end() );
  else throw PythonExc_IndexError();
}

template<class T>
void array2_pushColumn( T * array, boost::python::object v )
{
  std::vector<typename T::element_type> col = extract_vec<typename T::element_type>(v)();
  if( col.size() != array->getColumnSize() ) throw PythonExc_ValueError();
  array->insertColumn( array->getColumnNb(), col );
}

template<class T>
size_t array2_rownb( T * a )
{  return a->getRowNb();}

template<class T>
size_t array2_colnb( T * a )
{  return a->getColumnNb();}

template<class T>
size_t array2_rowsize( T * a )
{  return a->getRowSize();}

template<class T>
size_t array2_colsize( T * a )
{  return a->getColumnSize();}

template<class T>
boost::python::object array2_sizes( T * a )
{  return make_tuple(a->getRowNb(),a->getColumnNb()); }

template<class T>
size_t array2_size( T * a )
{  return a->size(); }


template<class T>
std::string array2_str( T * a, const char * name )
{
    uint_t r= a->getRowNb();
    std::stringstream ss;
    ss << name <<"([";
    for(uint_t i= 0; i < r; i++ )
    {
        ss << "[";
        for(typename T::const_iterator it = a->beginRow(i); it != a->endRow(i); ++it){
            if (it != a->beginRow(i)) ss << ",";
            ss << boost::python::extract<std::string>(boost::python::str(boost::python::object(*it)))();
        }
        ss << "]";
        if( i < r-1 ) ss << ",";
    }
    ss << "])";
    return ss.str();
}

template<class T>
RCPtr<T> array2_to_T(Array2<typename T::element_type> a)
{ return RCPtr<T>(new T(a.begin(),a.end(),a.getRowSize())); }

template<class T>
RCPtr<T> array2_transpose( T * array)
{
  return array2_to_T<T>(transpose(*array));
}

template<class T>
RCPtr<T> array2_submatrix( T * array, uint_t rw, uint_t cl, uint_t nr, uint_t nc)
{
  if( (rw + nr) < array->getRowNb() && (cl + nc) < array->getColumnNb() )
      return array2_to_T<T>(array->get(rw,cl,nr,nc));
  else throw PythonExc_IndexError();
}

template<class T>
void array2_reshape( T * array, size_t nbrow, size_t nbcol)
{
  if( nbrow * nbcol != array->size() )
     array->reshape(nbrow,nbcol);
  else throw PythonExc_ValueError();
}



template<class T>
struct array2_pickle_suite : boost::python::pickle_suite 
{ 
    static boost::python::tuple getinitargs(T const& ar) 
	{ 
		boost::python::list args; 
        for(uint_t i= 0; i < ar.getRowNb(); i++ ){
		    boost::python::list l; 
		    for(typename T::const_iterator it = ar.beginRow(i); it != ar.endRow(i); ++it) 
			    l.append(*it); 
			args.append(l); 
        }
		return boost::python::make_tuple(args);  
	} 
}; 

template<class ARRAY>
class array2_func : public boost::python::def_visitor<array2_func<ARRAY> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
        c // .def( "__getitem__", &array2_getrow<ARRAY>  ) 
         .def( "__setitem__", &array2_setitem<ARRAY> )				
         .def( "__len__", &array2_rownb<ARRAY> )					
         .def( "__contains__", &ARRAY::contains )					
         .def( "empty", &ARRAY::empty )					
         .def( "reshape", &array2_reshape<ARRAY>, boost::python::args("nbrow","nbcolumn") )					
         .def( "clear", &ARRAY::clear )					
         .def( "isUnique", &ARRAY::isUnique )					
         .def( "getRow", &array2_getrow<ARRAY>  ) 
         .def( "getRowNb", &array2_rownb<ARRAY>  ) 
         .def( "getRowSize", &array2_rowsize<ARRAY>  ) 
         .def( "getColumn", &array2_getcolumn<ARRAY>  ) 
         .def( "getColumnNb", &array2_colnb<ARRAY>  ) 
         .def( "getColumnSize", &array2_colsize<ARRAY>  ) 
         .def( "getDiagonal", &array2_getdiag<ARRAY>  ) 
         .def( "transpose", &array2_transpose<ARRAY>  ) 
	     .def( "submatrix", &array2_submatrix<ARRAY>, boost::python::args("row","column","nbrow","nbcolumn") ) 
         .def( "insertRow", &array2_insertRow<ARRAY>  ) 
         .def( "insertColumn", &array2_insertColumn<ARRAY>  ) 
         .def( "pushRow", &array2_pushRow<ARRAY>  ) 
         .def( "pushColumn", &array2_pushColumn<ARRAY>  )
         .def( "sizes", &array2_sizes<ARRAY>  )
         .def( "size", &array2_size<ARRAY>  )
         .def( "__iter__", &array2_getiter<ARRAY> )					
 	    .def_pickle(array2_pickle_suite<ARRAY>());

        boost::python::class_<Array2Iter<ARRAY> >("Iterator",boost::python::no_init)
         .def("next",&Array2Iter<ARRAY>::next)
         .def("__iter__",&Array2Iter<ARRAY>::nothing,boost::python::return_self<>())
        ;
    }
};



template<class T>
RCPtr<T> array2_iadd( T * array, T * array2)
{ (*array) += (*array2); return RCPtr<T>(array); }

template<class T>
RCPtr<T> array2_add( T * array, T * array2)
{ return array2_to_T<T>( (*array) + (*array2)); }

template<class T>
RCPtr<T> array2_isub( T * array, T * array2)
{ (*array) -= (*array2); return RCPtr<T>(array); }

template<class T>
RCPtr<T> array2_sub( T * array, T * array2)
{ return array2_to_T<T>( (*array) - (*array2)); }

template<class T>
RCPtr<T> array2_mul( T * array, T * array2)
{
  if(array->getColumnNb() != array2->getRowNb())
      throw PythonExc_ValueError("Incompatible sizes of matrices.");
  return array2_to_T<T>( (*array) * (*array2));
}

template<class T>
RCPtr<T> array2_iadde( T * array, typename T::element_type el)
{ (*array) += el; return RCPtr<T>(array); }

template<class T>
RCPtr<T> array2_adde( T * array, typename T::element_type el)
{ return array2_to_T<T>( (*array) + el); }

template<class T>
RCPtr<T> array2_isube( T * array, typename T::element_type el)
{ (*array) -= el; return RCPtr<T>(array); }

template<class T>
RCPtr<T> array2_sube( T * array, typename T::element_type el)
{ return array2_to_T<T>( (*array) - el); }

template<class T>
RCPtr<T> array2_mule( T * array, typename T::element_type el)
{ return array2_to_T<T>( (*array) * el); }

template<class T>
RCPtr<T> array2_imule( T * array, typename T::element_type el)
{ (*array) *= el; return RCPtr<T>(array); }

template<class T>
RCPtr<T> array2_inverse( T * array )
{
  if(array->getColumnNb() <= array->getRowNb())
      throw PythonExc_ValueError("Incompatible sizes of matrices.");
  return array2_to_T<T>( inverse(*array));
}

template<class T>
boost::python::object array2_svd( T * array )
{
	SVDMatrix<typename T::element_type> m;
  int res = m.decompose(*array);
  if(!res) throw PythonExc_ValueError();
  return boost::python::make_tuple(make_list<std::vector<typename T::element_type> >(m.getSig())(),m.getU(),m.getV());
}



template<class ARRAY>
class numarray2_func : public boost::python::def_visitor<numarray2_func<ARRAY> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
        c.def( "__iadd__",&array2_iadd<ARRAY>  ) 
         .def( "__iadd__",&array2_iadde<ARRAY>  ) 
         .def( "__add__", &array2_add<ARRAY> )				
         .def( "__add__", &array2_adde<ARRAY> )				
         .def( "__isub__",&array2_isub<ARRAY>  ) 
         .def( "__isub__",&array2_isube<ARRAY>  ) 
         .def( "__sub__", &array2_sub<ARRAY> )				
         .def( "__sub__", &array2_sube<ARRAY> )				
         .def( "__imul__",&array2_imule<ARRAY>  ) 
         .def( "__mul__", &array2_mul<ARRAY> )				
         .def( "__mul__", &array2_mule<ARRAY> )				
         .def( "inverse", &array2_inverse<ARRAY> )				
         .def( "svd",     &array2_svd<ARRAY> )
        ;
    }
};

#define EXPORT_FUNCTION2( PREFIX, ARRAY ) \
std::string PREFIX##_str(ARRAY * a) { return array2_str<ARRAY>(a, #ARRAY); }

#define EXPORT_FUNCTION( PREFIX, ARRAY) \
  DEF_POINTEE( ARRAY ) \
  EXPORT_FUNCTION2( PREFIX, ARRAY) 


#define EXPORT_CLASS_ARRAY( PREFIX, ARRAY )\
    class_< ARRAY, ARRAY##Ptr, boost::noncopyable>( #ARRAY , init<size_t,size_t>( #ARRAY "(int rows, int cols)", args("rows","cols")) ) \
    .def( "__init__", make_constructor( &extract_array2_from_list<ARRAY> ), #ARRAY "([[a,b,c],[d,e,f]])" ) \

#define EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \
    .def(array2_func<ARRAY>()) \
    .def( "__repr__",     &PREFIX##_str ) \
    .def( "__str__",      &PREFIX##_str ) \

#define EXPORT_ARRAYITERATOR( ARRAY ) \
    class_<Array2Iter<ARRAY> >(#ARRAY "Iterator",no_init) \
    .def("next",&Array2Iter<ARRAY>::next) \
    .def("__iter__",&Array2Iter<ARRAY>::nothing,return_self<>()); \

#define EXPORT_ARRAY_BT( PREFIX, ARRAY ) \
    EXPORT_CLASS_ARRAY( PREFIX, ARRAY ) \
    .def( "__getitem__", &array2_bt_getitem<ARRAY> ) \
	EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \


#define EXPORT_ARRAY_CT( PREFIX, ARRAY ) \
    EXPORT_CLASS_ARRAY( PREFIX, ARRAY  ) \
    .def( "__getitem__", &array2_ct_getitem<ARRAY> , return_internal_reference<1>() ) \
	EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \


#define EXPORT_CONVERTER( ARRAY ) \
	array2_from_list<ARRAY>();  \
	array2_ptr_from_list<ARRAY>(); 

