#include <string>
#include <sstream>
#include <scenegraph/container/indexarray.h>
#include "../util/exception.h"

#include <boost/python.hpp>

using namespace boost::python;

TOOLS_USING_NAMESPACE
PGL_USING_NAMESPACE

#include "arrays_macro.h"

Index * ind_fromlist( boost::python::object l ) 
{ 
  Index * array = new Index();
  object iter_obj = boost::python::object( handle<>( PyObject_GetIter( l.ptr() ) ) );
      while( 1 )
       {
		object obj; 
		try { 
          obj = iter_obj.attr( "next" )();
		}
		catch( error_already_set ){ PyErr_Clear(); break; }
        uint32_t val = boost::python::extract<uint32_t>( obj );
        array->pushBack( val );
       }
  return array;
}

EXPORT_FUNCTION2( ind, uint32_t, Index );

#define INDEX_SETGET(PREFIX,ARRAY,SIZE) \
 uint32_t PREFIX##_size( ARRAY* v) { return SIZE; } \
 uint32_t PREFIX##_getAt( ARRAY* v, size_t i ) \
 { \
   if (i < SIZE) return v->getAt(i); \
   else throw PythonExc_IndexError(); \
 } \
 void PREFIX##_setAt( ARRAY* v, size_t i, uint32_t val ) \
 { \
   if (i < SIZE) v->getAt(i) = val; \
   else throw PythonExc_IndexError(); \
 } \

INDEX_SETGET(i3,Index3,3);
INDEX_SETGET(i4,Index4,4);

std::string i3_repr( Index3* i )
{
  std::stringstream ss;
  ss << "Index3( " << i->getAt( 0 ) << ", " << i->getAt( 1 ) << ", "
     << i->getAt( 2 ) << " )";
  return ss.str();
}


std::string i4_repr( Index4* i )
{
  std::stringstream ss;
  ss << "Index4( " << i->getAt( 0 ) << ", " << i->getAt( 1 ) << ", "
     << i->getAt( 2 ) << ", " << i->getAt( 3 ) << " )";
  return ss.str();
}

void export_index()
{
  class_<Index3>( "Index3", init<optional<size_t,size_t,size_t> >() )
    .def( "__str__", i3_repr )
    .def( "__repr__", i3_repr )
    .def( "__getitem__", i3_getAt )
    .def( "__setitem__", i3_setAt )
    .def( "__len__", i3_size )
    .def( self == self )
    .def( self != self )
    ;
  class_<Index4>( "Index4", init<optional<size_t,size_t,size_t,size_t> >() )
    .def( "__str__", i4_repr )
    .def( "__repr__", i4_repr )
    .def( "__getitem__", i4_getAt )
    .def( "__setitem__", i4_setAt )
    .def( "__len__", i4_size )
    .def( self == self )
    .def( self != self )
    ;

  class_< Index >( "Index" , init<size_t>("Index(int size)",args("size")) )
    .def( "__init__", make_constructor( ind_fromlist ), "Index([int i, int j, int k, ...])"  )
    EXPORT_ARRAY_FUNC( ind )
    ;

}

