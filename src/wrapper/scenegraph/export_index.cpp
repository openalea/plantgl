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



#include "arrays_macro.h"

#include <plantgl/python/tuple_converter.h>
#include <plantgl/python/extract_list.h>

#include <string>
#include <sstream>
#include <plantgl/scenegraph/container/indexarray.h>

#include <boost/python.hpp>

using namespace boost::python;

PGL_USING_NAMESPACE



Index * ind_fromlist( boost::python::object l )
{
    return new Index(extract_vec<Index::element_type,extract,Index>(l)());
}

Index * ind_fromvalue3( uint_t a, uint_t b, uint_t c)
{
  Index * ind = new Index();
  ind->push_back(a);
  ind->push_back(b);
  ind->push_back(c);
  return ind;
}

Index * ind_fromvalue4( uint_t a, uint_t b, uint_t c, uint_t d)
{
  Index * ind = new Index();
  ind->push_back(a);
  ind->push_back(b);
  ind->push_back(c);
  ind->push_back(d);
  return ind;
}

Index * ind_fromvalue5( uint_t a, uint_t b, uint_t c, uint_t d, uint_t e)
{
  Index * ind = new Index();
  ind->push_back(a);
  ind->push_back(b);
  ind->push_back(c);
  ind->push_back(d);
  ind->push_back(e);
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
    Index result = extract_vec<Index::element_type,extract,Index>(py_sequence);
    new (memory_chunk) Index (result);
    // delete result;
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

