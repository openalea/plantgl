// Array Macro
#include "../util/extract_list.h"
#include <boost/python/def_visitor.hpp>

template<class T>
RCPtr<T> extract_array_from_list( boost::python::object l )
{ 
  boost::python::extract<int> e_int( l ); 
  if( e_int.check() )
    {
    return new T( e_int() );
    }
  return extract_pgllist<T>(l).toRCPtr();
} 

template<class T>
struct array_from_list {
  array_from_list() { 
	boost::python::converter::registry::push_back( &convertible, &construct, boost::python::type_id<T>()); 
  } 
  static void* convertible(PyObject* py_obj){ 
    if( !PySequence_Check( py_obj ) ) return 0; 
    return py_obj; 
  } 
  static void construct( PyObject* obj, boost::python::converter::rvalue_from_python_stage1_data* data){ 
   typedef boost::python::converter::rvalue_from_python_storage<T> vector_storage_t;  
   vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data ); 
   void* memory_chunk = the_storage->storage.bytes; 
   boost::python::list py_sequence( boost::python::handle<>( boost::python::borrowed( obj ) ) ); 
   RCPtr<T> result = extract_array_from_list<T>(py_sequence); 
   new (memory_chunk) T (*result); 
   delete result; 
   data->convertible = memory_chunk; 
  } 
}; 

template<class T>
struct array_ptr_from_list { 
  array_ptr_from_list() { 
	boost::python::converter::registry::push_back( &convertible, &construct, boost::python::type_id< RCPtr<T> >()); 
  } 
  static void* convertible(PyObject* py_obj){ 
    if( !PySequence_Check( py_obj ) ) return 0; 
    return py_obj; 
  } 
  static void construct( PyObject* obj, boost::python::converter::rvalue_from_python_stage1_data* data){ 
   typedef boost::python::converter::rvalue_from_python_storage<T> vector_storage_t;  
   vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data ); 
   void* memory_chunk = the_storage->storage.bytes; 
   boost::python::list py_sequence( boost::python::handle<>( boost::python::borrowed( obj ) ) ); 
   RCPtr<T> result = extract_array_from_list<T>(py_sequence); 
   new (memory_chunk) RCPtr<T> (result); 
   data->convertible = memory_chunk; 
  } 
}; 


#define EXPORT_FUNCTION2(PREFIX,ARRAY) \
std::string PREFIX##_str( ARRAY * a ) \
{ \
  std::stringstream ss; \
  ss << #ARRAY << "(";  \
  if(!a->isEmpty()){ \
    ss << '['; \
    for(ARRAY::const_iterator it = a->getBegin(); it != a->getEnd(); ++it){ \
        if (it != a->getBegin()) ss << ","; \
        ss << extract<std::string>(boost::python::str(boost::python::object(*it)))(); \
    } \
	ss << ']'; } \
  ss << ")"; \
  return ss.str(); \
} \

//    std::copy( a->getBegin(), a->getEnd()-1, std::ostream_iterator< ARRAY::element_type >( ss, "," ) ); \

template<class T>
typename T::element_type array_bt_getitem( T * a, int pos )
{ 
  if( pos < 0 && pos >= -a->getSize() ) return a->getAt( a->getSize() + pos );
  else if( pos < a->getSize() ) return a->getAt( pos );
  else throw PythonExc_IndexError();
}

template<class T>
typename T::element_type& array_ct_getitem( T * a, int pos )
{ 
  if( pos < 0 && pos >= -a->getSize() ) return a->getAt( a->getSize() + pos );
  else if( pos < a->getSize() ) return a->getAt( pos );
  else throw PythonExc_IndexError();
}

template<class T>
typename T::element_type array_ptr_getitem( T * a, int pos )
{ 
  if( pos < 0 && pos >= -a->getSize() ) return a->getAt( a->getSize() + pos );
  else if( pos < a->getSize() ) return a->getAt( pos );
  else throw PythonExc_IndexError();
}

template<class T>
T * array_getslice( T * array, int beg, int end ) 
{ 
  if( beg >= -array->getSize() && beg < 0  )  beg += array->getSize(); 
  else if( beg >= array->getSize() ) throw PythonExc_IndexError(); 
  if( end >= -array->getSize() && end < 0  )  end += array->getSize(); 
  else if( end > array->getSize() ) throw PythonExc_IndexError(); 
  return new T(array->getBegin()+beg,array->getBegin()+end);
}

template<class T>
typename T::element_type array_popitem( T * a, int pos )
{ 
  if (a->isEmpty()) throw PythonExc_IndexError();
  if( pos < 0 && pos >= -a->getSize() ) pos = a->getSize() + pos;
  else if( pos >= a->getSize() ) throw PythonExc_IndexError();
  typename T::element_type elem =  a->getAt( pos );
  a->Erase(a->getBegin() + pos);
  return elem;
}

template<class T>
typename T::element_type array_poplastitem( T * a)
{ return array_popitem(a,-1); }


template<class T>
void array_setitem( T * array, int pos, typename T::element_type * v )
{
  if( pos < 0 && pos >= -array->getSize() ) array->setAt( array->getSize() + pos, *v );
  else if( pos < array->getSize() ) array->setAt( pos, *v );
  else throw PythonExc_IndexError();
}

template<class T>
void array_insertitem( T * array, int pos, typename T::element_type * v )
{
  if( pos < 0 && pos >= -array->getSize() ) array->insert( array->getBegin() + (array->getSize() + pos), *v );
  else if( pos < array->getSize() ) array->insert( array->getBegin() + pos, *v );
  else throw PythonExc_IndexError();
}

template<class T>
void array_delitem( T * array, int pos )
{
  if( pos < 0 && pos >= -array->getSize() ) array->Erase( array->getBegin() + (array->getSize() + pos) );
  else if( pos < array->getSize() ) array->Erase( array->getBegin() + pos );
  else throw PythonExc_IndexError();
}

template<class T>
void array_delslice( T * array, int beg, int end ) 
{ 
  if( beg >= -array->getSize() && beg < 0  )  beg += array->getSize(); 
  else if( beg >= array->getSize() ) throw PythonExc_IndexError(); 
  if( end >= -array->getSize() && end < 0  )  end += array->getSize(); 
  else if( end > array->getSize() ) throw PythonExc_IndexError(); 
  array->Erase( array->getBegin()+beg,array->getBegin()+end); 
}

template<class T>
bool array_contains( T * array, typename T::element_type * v ) 
{
  return array->contains(*v); 
} 

template<class T>
T * array_additem( T * array, typename T::element_type * v ) 
{ 
	T * array2 = new T(array->getBegin(),array->getEnd());
	array2->pushBack(*v);
	return array2; 
}

template<class T>
T * array_iadditem( T * array, typename T::element_type * v ) 
{ 
	array->pushBack(*v); 
	return array; 
}

template<class T>
void array_appenditem( T * array, typename T::element_type * v ) 
{ array->pushBack(*v); }

template<class T>
T * array_iaddarray( T * array, RCPtr<T> array2 ) 
{ 
	array->insert(array->getEnd(),array2->getBegin(),array2->getEnd()); 
	return array; 
}

template<class T>
size_t array_len( T * a )
{  return a->getSize();}


template<class T>
struct array_pickle_suite : boost::python::pickle_suite 
{ 
    static boost::python::tuple getinitargs(T const& ar) 
	{ 
		boost::python::list l; 
		for(typename T::const_iterator it = ar.getBegin(); it != ar.getEnd(); ++it) 
			l.append(*it); 
		return boost::python::make_tuple(l);  
	} 
}; 

template<class ARRAY>
class array_func : public boost::python::def_visitor<array_func<ARRAY> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
        c.def( "__getslice__", &array_getslice<ARRAY>, boost::python::return_value_policy<boost::python::manage_new_object>() ) \
        .def( "__setitem__",  &array_setitem<ARRAY>   ) \
        .def( "__delitem__",  &array_delitem<ARRAY>   ) \
        .def( "__delslice__", &array_delslice<ARRAY>  ) \
        .def( "__contains__", &array_contains<ARRAY>  ) \
        .def( "__add__",      &array_additem<ARRAY>   , boost::python::return_value_policy<boost::python::manage_new_object>() ) \
        .def( "__iadd__",     &array_iadditem<ARRAY>  , boost::python::return_internal_reference<1>() ) \
        .def( "__iadd__",     &array_iaddarray<ARRAY> , boost::python::return_internal_reference<1>() ) \
        .def( "__len__",      &array_len<ARRAY> ) \
        .def( "reverse",      &ARRAY::reverse ) \
        .def( "insert",       &array_insertitem<ARRAY> ) \
        .def( "append",       &array_appenditem<ARRAY> ) \
        .def( "pop",          &array_popitem<ARRAY> ) \
        .def( "pop",          &array_poplastitem<ARRAY> ) \
	    .enable_pickling() \
        ;
    }
};

#define EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \
    .def(array_func<ARRAY>()) \
    .def( "__repr__",     &PREFIX##_str ) \
    .def( "__str__",      &PREFIX##_str ) \

#define EXPORT_ARRAY_FUNC_BT( ARRAY, PREFIX ) \
    .def( "__getitem__",  &array_bt_getitem<ARRAY> ) \
    EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \

#define EXPORT_ARRAY_FUNC_CT( ARRAY, PREFIX ) \
    .def( "__getitem__",  &array_ct_getitem<ARRAY>, return_internal_reference<1>() ) \
    EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \

#define EXPORT_ARRAY_FUNC_PTR( ARRAY, PREFIX ) \
    .def( "__getitem__",  &array_ptr_getitem<ARRAY> ) \
    EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \


#define EXPORT_CLASS_ARRAY( PREFIX, ARRAY, STRING )\
class_< ARRAY, ARRAY##Ptr, boost::noncopyable>( #ARRAY , init<size_t>(#ARRAY "(int size)", args("size") = 0) ) \
    .def( "__init__", make_constructor( &extract_array_from_list<ARRAY> ), STRING ) \


/* --------------------
  Type of the element :
   _BT : Basic Type : real_t, int, uint32_t, ...
   _CT : Complex Type : Vector3, Color3, Index, ...
   _PTR : Pointer : GeometryPtr
   -------------------- */

#define EXPORT_ARRAY_BT( PREFIX, ARRAY, STRING )\
    EXPORT_CLASS_ARRAY( PREFIX, ARRAY, STRING ) \
	EXPORT_ARRAY_FUNC_BT( ARRAY, PREFIX ) \

#define EXPORT_ARRAY_PTR( PREFIX, ARRAY, STRING )\
    EXPORT_CLASS_ARRAY( PREFIX, ARRAY, STRING ) \
	EXPORT_ARRAY_FUNC_PTR( ARRAY, PREFIX ) \

#define EXPORT_ARRAY_CT( PREFIX, ARRAY, STRING )\
    EXPORT_CLASS_ARRAY( PREFIX, ARRAY, STRING ) \
	EXPORT_ARRAY_FUNC_CT( ARRAY, PREFIX ) \

#define EXPORT_CONVERTER( ARRAY )\
	array_from_list<ARRAY>(); \
	array_ptr_from_list<ARRAY>(); \

#define EXPORT_FUNCTION( PREFIX, ARRAY) \
  DEF_POINTEE( ARRAY ) \
  EXPORT_FUNCTION2( PREFIX, ARRAY)

#ifdef USE_NUMPY
#define PY_ARRAY_UNIQUE_SYMBOL PlantGL_NUMPY_API_SYMBOL
#define NO_IMPORT_ARRAY
#include <numpy/arrayobject.h>
#include <sstream>

#define CHECK_TYPE( store, C_TYPE )\
  if( typeid( C_TYPE ) == typeid( int ) )\
    store = PyArray_INT; \
  else if( typeid( C_TYPE ) == typeid( unsigned int ) )\
    store = PyArray_UINT; \
  else if( typeid( C_TYPE ) == typeid( long ) )\
    store = PyArray_LONG; \
  else if( typeid( C_TYPE ) == typeid( unsigned long ) )\
    store = PyArray_ULONG; \
  else if( typeid( C_TYPE ) == typeid( char ) )\
    store = PyArray_BYTE; \
  else if( typeid( C_TYPE ) == typeid( unsigned char ) )\
    store = PyArray_UBYTE; \
  else if( typeid( C_TYPE ) == typeid( float ) )\
    store = PyArray_FLOAT; \
  else if( typeid( C_TYPE ) == typeid( double ) )\
    store = PyArray_DOUBLE; \
  else\
    {\
    PyErr_SetString( PyExc_ValueError, "Error, the wanted type is unknown" );\
    throw error_already_set();\
    }

#define EXPORT_NUMPY( PREFIX, T, ARRAY, DIM0, DIM1, C_TYPE ) \
ARRAY##Ptr PREFIX##_fromnumpy( boost::python::object l ) \
{\
  size_t dim0 = DIM0;\
  size_t dim1 = DIM1;\
  PyObject* obj = l.ptr(); \
  if( PyArray_Check( obj ) ) \
    { \
    PyArrayObject *a_obj = ( PyArrayObject* )obj; \
    if( a_obj->nd != 2 )\
      {\
      PyErr_SetString(PyExc_TypeError, "The array as argument must have 2 dimensions" );\
      throw error_already_set();\
      }\
    if( dim0 != 0 && a_obj->dimensions[ 0 ] != dim0 ) \
      {\
      std::stringstream ss;\
      ss << "The first dimension of the argument must be of size " << dim0;\
      PyErr_SetString(PyExc_TypeError, ss.str().c_str() );\
      throw error_already_set();\
      }\
    if( dim1 != 0 && a_obj->dimensions[ 1 ] != dim1 ) \
      {\
      std::stringstream ss;\
      ss << "The second dimension of the argument must be of size " << dim1;\
      PyErr_SetString(PyExc_TypeError, ss.str().c_str() );\
      throw error_already_set();\
      }\
    int a_type = 0; \
    int a_require = NPY_CONTIGUOUS|NPY_FORCECAST; \
    CHECK_TYPE( a_type, C_TYPE );\
    PyObject *array = PyArray_FromArray( a_obj, PyArray_DescrFromType( a_type ), a_require ); \
    if( array == NULL )\
      {\
      throw error_already_set();\
      }\
    int nb_elmt0 = PyArray_DIM( array, 0 ); \
    int nb_elmt1 = PyArray_DIM( array, 1 ); \
    int i, j; \
    ARRAY##Ptr result = new ARRAY( nb_elmt0 ); \
    for( i = 0 ; i < nb_elmt0 ; ++i ) \
      {\
      T elmt( nb_elmt1 );\
      for( j = 0 ; j < nb_elmt1 ; ++j )\
        {\
        elmt.setAt( j, *( C_TYPE* )PyArray_GETPTR2( array, i, j ) );\
        }\
      result->setAt( i, elmt );\
      }\
    Py_DECREF( array );\
    return result;\
    } \
  return PREFIX##_fromlist( l ); \
}\

#define EXPORT_NUMPY_1DIM( PREFIX, T, ARRAY, DIM, C_TYPE ) \
ARRAY##Ptr PREFIX##_fromnumpy( boost::python::object l ) \
{\
  size_t dim = DIM;\
  PyObject* obj = l.ptr(); \
  if( PyArray_Check( obj ) ) \
    { \
    PyArrayObject *a_obj = ( PyArrayObject* )obj; \
    if( a_obj->nd != 1 )\
      {\
      PyErr_SetString(PyExc_TypeError, "The array as argument must have 1 dimension" );\
      throw error_already_set();\
      }\
    if( dim != 0 && a_obj->dimensions[ 0 ] != dim ) \
      {\
      std::stringstream ss;\
      ss << "The argument must be of size " << dim;\
      PyErr_SetString(PyExc_TypeError, ss.str().c_str() );\
      throw error_already_set();\
      }\
    int a_type = 0; \
    int a_require = NPY_CONTIGUOUS|NPY_FORCECAST; \
    CHECK_TYPE( a_type, C_TYPE );\
    PyObject *array = PyArray_FromArray( a_obj, PyArray_DescrFromType( a_type ), a_require ); \
    if( array == NULL )\
      {\
      throw error_already_set();\
      }\
    int nb_elmt = PyArray_DIM( array, 0 ); \
    int i, j; \
    ARRAY##Ptr result = new ARRAY( nb_elmt ); \
    for( i = 0 ; i < nb_elmt ; ++i ) \
      {\
      result->setAt( i, *( C_TYPE*) PyArray_GETPTR1( array, i ) );\
      }\
    Py_DECREF( array );\
    return result;\
    } \
  return PREFIX##_fromlist( l ); \
}\

#define DEFINE_NUMPY( PREFIX ) .def( "__init__", make_constructor( PREFIX##_fromnumpy ) )

#else

#define EXPORT_NUMPY( PREFIX, T, ARRAY, DIM0, DIM1, C_TYPE ) 
#define EXPORT_NUMPY_1DIM( PREFIX, T, ARRAY, DIM, C_TYPE )

#define DEFINE_NUMPY( PREFIX )

#endif

