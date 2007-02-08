// Array Macro
#define EXPORT_FUNCTION1( PREFIX, T, ARRAY) \
DEF_POINTEE( ARRAY ) \
\
     ARRAY##Ptr PREFIX##_fromlist( boost::python::object l )		\
{									\
  ARRAY* array= 0;							\
      object row_iter_obj = boost::python::object( handle<>( PyObject_GetIter( l.ptr() ) ) );\
      uint32_t rows= extract<uint32_t>(row_iter_obj.attr("__len__")());	\
      object col_obj= row_iter_obj.attr( "next" )();		\
      object col_iter_obj= boost::python::object( handle<>( PyObject_GetIter( col_obj.ptr() ) ) );		\
      uint32_t cols= extract<uint32_t>(col_iter_obj.attr("__len__")());	\
      array= new ARRAY(rows,cols);					\
      for(uint32_t i=0; i < rows; ++i )							\
	 {								\
		if (i != 0) { \
			col_obj= row_iter_obj.attr( "next" )();			\
			col_iter_obj= boost::python::object( handle<>( PyObject_GetIter( col_obj.ptr() ) ) );		\
			uint32_t c= extract<uint32_t>(col_iter_obj.attr("__len__")());	\
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
T PREFIX##_getitem( ARRAY * array, size_t i, size_t j )	\
{								\
  if( i < array->getRowsNb() && j < array->getColsNb() )	\
    return array->getAt( i, j );				\
  else throw PythonExc_IndexError();				\
}								\
								\
void PREFIX##_setitem( ARRAY * array, size_t i, size_t j, T * v )\
{								 \
  if( i < array->getRowsNb() && j < array->getColsNb() )	 \
    array->setAt( i, j, *v );					 \
  else throw PythonExc_IndexError();				 \
}								 \
								 \
std::string PREFIX##_str( ARRAY * a )					\
{									\
  uint32_t r= a->getRowsNb(); uint32_t i= 0;				\
  std::stringstream ss;							\
  ss << "[ ";								\
  for( i= 0; i < r; i++ )						\
    {									\
      ss << "[ ";							\
      std::copy( a->getBeginRow(i), a->getEndRow(i), std::ostream_iterator< T >( ss, " " ) ); \
      ss << "]";							\
    }									\
  ss << "]";\
  return ss.str();\
}\
\
std::string PREFIX##_repr( ARRAY * a )\
{\
  uint32_t r= a->getRowsNb(); uint32_t i= 0;				\
  std::stringstream ss;							\
  ss << "ARRAY([ ";							\
  for( i= 0; i < r; i++ )						\
    {									\
      ss << "[ ";							\
      std::copy( a->getBeginRow(i), a->getEndRow(i)-1, std::ostream_iterator< T >( ss, ", " ) ); \
      ss << *( a->getEndRow(i) - 1 ) << "]";				\
      if( i < r-1 ) ss << ", ";						\
    }									\
  ss << "])";								\
  return ss.str();							\
}									\
									\
size_t PREFIX##_len( ARRAY * a )					\
{  return a->getRowsNb();}						\

#define EXPORT_ARRAY( PREFIX, ARRAY,STRING )				\
  class_< ARRAY, ARRAY##Ptr, boost::noncopyable>( #ARRAY , init<optional<size_t,size_t> >(#STRING) ) \
    .def( "__init__", make_constructor( PREFIX##_fromlist ) )		\
    .def( "__getitem__", PREFIX##_getitem /*, return_internal_reference<1>()*/ ) \
    .def( "__setitem__", PREFIX##_setitem )				\
    .def( "__len__", PREFIX##_len )					\
    .def( "__repr__", PREFIX##_repr )					\
    .def( "__str__", PREFIX##_str )					\
    ;									\

#define EXPORT_FUNCTION( PREFIX, T, ARRAY) \
  EXPORT_FUNCTION1( PREFIX, T, ARRAY) \
  EXPORT_FUNCTION2( PREFIX, T, ARRAY)

