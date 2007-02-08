// Array Macro
#define EXPORT_FUNCTION1( PREFIX, T, ARRAY) \
ARRAY##Ptr PREFIX##_fromlist( boost::python::object l ) \
{ \
  extract<int> e_int( l ); \
  if( e_int.check() )\
    {\
    return new ARRAY( e_int() );\
    }\
  ARRAY##Ptr array = ARRAY##Ptr(new ARRAY());\
  object iter_obj = boost::python::object( handle<>( PyObject_GetIter( l.ptr() ) ) );\
  while( 1 )\
    {\
    object obj; \
    try \
      { \
      obj = iter_obj.attr( "next" )();\
      }\
    catch( error_already_set ){ PyErr_Clear(); break; }\
    T val = boost::python::extract<T>( obj );\
    array->pushBack( val );\
    }\
  return array;\
}\

#define EXPORT_FUNCTION2( PREFIX, T, ARRAY) \
T PREFIX##_getitem( ARRAY * array, int pos ) \
{ \
  if( pos < 0 && pos > -array->getSize() ) return array->getAt( array->getSize() + pos );\
  else if( pos < array->getSize() ) return array->getAt( pos );\
  else throw PythonExc_IndexError();\
}\
\
ARRAY * PREFIX##_getslice( ARRAY * array, int beg, int end ) \
{ \
  if( beg > -array->getSize() && beg < 0  )  beg += array->getSize(); \
  else if( beg >= array->getSize() ) throw PythonExc_IndexError(); \
  if( end > -array->getSize() && end < 0  )  end += array->getSize(); \
  else if( end >= array->getSize() ) throw PythonExc_IndexError(); \
  return new ARRAY(array->getBegin()+beg,array->getBegin()+end);\
}\
\
void PREFIX##_setitem( ARRAY * array, int pos, T * v )\
{\
  if( pos < 0 && pos > -array->getSize() ) array->setAt( array->getSize() + pos, *v );\
  else if( pos < array->getSize() ) array->setAt( pos, *v );\
  else throw PythonExc_IndexError();\
}\
\
void PREFIX##_delitem( ARRAY * array, int pos )\
{\
  if( pos < 0 && pos > -array->getSize() ) array->Erase( array->getBegin() + (array->getSize() + pos) );\
  else if( pos < array->getSize() ) array->Erase( array->getBegin() + pos );\
  else throw PythonExc_IndexError();\
}\
\
void PREFIX##_delslice( ARRAY * array, int beg, int end ) \
{ \
  if( beg > -array->getSize() && beg < 0  )  beg += array->getSize(); \
  else if( beg >= array->getSize() ) throw PythonExc_IndexError(); \
  if( end > -array->getSize() && end < 0  )  end += array->getSize(); \
  else if( end >= array->getSize() ) throw PythonExc_IndexError(); \
  array->Erase( array->getBegin()+beg,array->getBegin()+end); \
}\
\
bool PREFIX##_contains( ARRAY * array, T * v ) \
{\
  return array->contains(*v); \
} \
\
ARRAY * PREFIX##_additem( ARRAY * array, T * v ) \
{ \
	ARRAY * array2 = new ARRAY(array->getBegin(),array->getEnd()); \
	array2->pushBack(*v); \
	return array2; \
}\
\
ARRAY * PREFIX##_iadditem( ARRAY * array, T * v ) \
{ \
	array->pushBack(*v); \
	return array; \
}\
\
ARRAY * PREFIX##_iaddarray( ARRAY * array, ARRAY * array2 ) \
{ \
	array->insert(array->getEnd(),array2->getBegin(),array2->getEnd()); \
	return array; \
}\
\
std::string PREFIX##_str( ARRAY * a )\
{\
  std::stringstream ss;\
  ss << #ARRAY << "("; \
  if(!a->isEmpty()){ \
    ss << '['; \
	std::copy( a->getBegin(), a->getEnd()-1, std::ostream_iterator< T >( ss, "," ) ); \
	ss << *( a->getEnd() - 1 ) << ']'; } \
  ss << ")"; \
  return ss.str();\
}\
\
size_t PREFIX##_len( ARRAY * a )\
{  return a->getSize();}\
\
struct PREFIX##_pickle_suite : boost::python::pickle_suite \
{ \
	static tuple getinitargs(ARRAY const& ar) \
	{ \
		boost::python::list l; \
		for(ARRAY::const_iterator it = ar.getBegin(); it != ar.getEnd(); ++it) \
			l.append(*it); \
		return make_tuple(l);  \
	} \
}; \

#define EXPORT_ARRAY_FUNC( PREFIX ) \
    .def( "__getitem__",  PREFIX##_getitem   ) \
    .def( "__getslice__", PREFIX##_getslice , return_value_policy<manage_new_object>() ) \
    .def( "__setitem__",  PREFIX##_setitem   ) \
    .def( "__delitem__",  PREFIX##_delitem   ) \
    .def( "__delslice__", PREFIX##_delslice  ) \
    .def( "__contains__", PREFIX##_contains  ) \
    .def( "__add__",      PREFIX##_additem   , return_value_policy<manage_new_object>() ) \
    .def( "__iadd__",     PREFIX##_iadditem  , return_internal_reference<1>() ) \
    .def( "__iadd__",     PREFIX##_iaddarray , return_internal_reference<1>() ) \
    .def( "__len__",      PREFIX##_len ) \
    .def( "__repr__",     PREFIX##_str ) \
    .def( "__str__",      PREFIX##_str ) \
	.enable_pickling()

	// .def_pickle(PREFIX##_pickle_suite())

#define EXPORT_ARRAY( PREFIX, ARRAY, STRING )\
class_< ARRAY, ARRAY##Ptr, boost::noncopyable>( #ARRAY , init<size_t>(#ARRAY "(int size)", args("size") = 0) ) \
    .def( "__init__", make_constructor( PREFIX##_fromlist ), STRING ) \
	EXPORT_ARRAY_FUNC( PREFIX ) \

#define EXPORT_FUNCTION( PREFIX, T, ARRAY) \
  DEF_POINTEE( ARRAY ) \
  EXPORT_FUNCTION1( PREFIX, T, ARRAY) \
  EXPORT_FUNCTION2( PREFIX, T, ARRAY)

#ifdef USE_NUMPY
#define PY_ARRAY_UNIQUE_SYMBOL PlantGL_NUMPY_API_SYMBOL
#define NO_IMPORT_ARRAY
#include <numpy/arrayobject.h>
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

