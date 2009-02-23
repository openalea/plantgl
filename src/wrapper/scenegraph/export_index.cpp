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

#include <string>
#include <sstream>
#include <plantgl/scenegraph/container/indexarray.h>

#include <plantgl/python/tuple_converter.h>
#include "arrays_macro.h"

#include <boost/python.hpp>

using namespace boost::python;

TOOLS_USING_NAMESPACE
PGL_USING_NAMESPACE



Index * ind_fromlist( boost::python::object l ) 
{ 
  return extract_pgllist<Index >(l)();
}

Index * ind_fromvalue3( uint_t a, uint_t b, uint_t c) 
{ 
  Index * ind = new Index();
  ind->pushBack(a);
  ind->pushBack(b);
  ind->pushBack(c);
  return ind;
}

Index * ind_fromvalue4( uint_t a, uint_t b, uint_t c, uint_t d) 
{ 
  Index * ind = new Index();
  ind->pushBack(a);
  ind->pushBack(b);
  ind->pushBack(c);
  ind->pushBack(d);
  return ind;
}

Index * ind_fromvalue5( uint_t a, uint_t b, uint_t c, uint_t d, uint_t e) 
{ 
  Index * ind = new Index();
  ind->pushBack(a);
  ind->pushBack(b);
  ind->pushBack(c);
  ind->pushBack(d);
  ind->pushBack(e);
  return ind;
}

EXPORT_FUNCTION2( ind,Index );

#define INDEX_SETGET(PREFIX,ARRAY,SIZE) \
 uint_t PREFIX##_size( ARRAY* v) { return SIZE; } \
 uint_t PREFIX##_getAt( ARRAY* v, size_t i ) \
 { \
   if (i < SIZE) return v->getAt(i); \
   else throw PythonExc_IndexError(); \
 } \
 void PREFIX##_setAt( ARRAY* v, size_t i, uint_t val ) \
 { \
   if (i < SIZE) v->getAt(i) = val; \
   else throw PythonExc_IndexError(); \
 } \

INDEX_SETGET(i3,Index3,3);
INDEX_SETGET(i4,Index4,4);

std::string i3_repr( Index3* i )
{
  std::stringstream ss;
  ss << "Index3(" << i->getAt( 0 ) << "," << i->getAt( 1 ) << ","
     << i->getAt( 2 ) << ")";
  return ss.str();
}


std::string i4_repr( Index4* i )
{
  std::stringstream ss;
  ss << "Index4(" << i->getAt( 0 ) << "," << i->getAt( 1 ) << ","
     << i->getAt( 2 ) << "," << i->getAt( 3 ) << ")";
  return ss.str();
}


struct index_from_tuple {
  index_from_tuple () {
	boost::python::converter::registry::push_back( &convertible, &construct, boost::python::type_id<Index>());
  }

  static void* convertible(PyObject* py_obj){
    if( !PySequence_Check( py_obj ) ) return 0;
	return py_obj;
  }

  static void construct( PyObject* obj, boost::python::converter::rvalue_from_python_stage1_data* data){
    typedef boost::python::converter::rvalue_from_python_storage<Index> vector_storage_t;
	vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data );
	void* memory_chunk = the_storage->storage.bytes;
	boost::python::object py_sequence( handle<>( borrowed( obj ) ) );
	Index * result = extract_pgllist<Index>(py_sequence);
    new (memory_chunk) Index (*result);
	delete result;
	data->convertible = memory_chunk;
  }
};



void export_index()
{
  class_<Index3>( "Index3", "A set of 3 indices", init<optional<size_t,size_t,size_t> >() )
    .def( "__str__", i3_repr )
    .def( "__repr__", i3_repr )
    .def( "__getitem__", i3_getAt )
    .def( "__setitem__", i3_setAt )
    .def( "__len__", i3_size )
    .def( self == self )
    .def( self != self )
    ;
  pgltuple_from_tuple<Index3,3>();

  class_<Index4>( "Index4", "A set of 4 indices", init<optional<size_t,size_t,size_t,size_t> >() )
    .def( "__str__", i4_repr )
    .def( "__repr__", i4_repr )
    .def( "__getitem__", i4_getAt )
    .def( "__setitem__", i4_setAt )
    .def( "__len__", i4_size )
    .def( self == self )
    .def( self != self )
    ;
  pgltuple_from_tuple<Index4,4>();

  class_< Index >( "Index" , "A set of indices", init<size_t>("Index(int size)",args("size")) )
    .def( "__init__", make_constructor( ind_fromlist ), "Index([int i, int j, int k, ...])"  )
    .def( "__init__", make_constructor( ind_fromvalue3 ), "Index(int i, int j, int k)"  )
    .def( "__init__", make_constructor( ind_fromvalue4 ), "Index(int i, int j, int k, int l)"  )
    .def( "__init__", make_constructor( ind_fromvalue5 ), "Index(int i, int j, int k, int l, int m)"  )
    EXPORT_ARRAY_FUNC_BT( Index, ind )
    ;
  index_from_tuple();

}

