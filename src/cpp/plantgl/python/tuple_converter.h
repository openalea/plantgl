/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP
 *
 *       File author(s): F. Boudon et al.
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

#ifndef __tuple_converter_h__
#define __tuple_converter_h__

#include "exception.h"
#include "extract_list.h"
#include "boost_python.h"

template<class T, int tuplesize>
T * extract_pgltuple_from_tuple_at(const boost::python::tuple& t, void * addr = NULL){
    int size = len(t);
    if (size != tuplesize)throw PythonExc_IndexError();
    else {
        T * result = new (addr)T();
        for(int i = 0; i < tuplesize; ++i)
            result->setAt(i,boost::python::extract<typename T::value_type>(t[i]));
        return result;
    }
}

template<class T, int tuplesize>
struct pgltuple_from_tuple {
  pgltuple_from_tuple() {
    boost::python::converter::registry::push_back( &convertible, &construct, boost::python::type_id<T>());
  }

  static void* convertible(PyObject* py_obj){
    if( !PySequence_Check( py_obj ) ) return 0;
    if( !PyObject_HasAttrString( py_obj, "__len__" ) ) return 0;
    boost::python::object py_sequence( boost::python::handle<>( boost::python::borrowed( py_obj ) ) );
    int _len = 0;
    try { _len = len(py_sequence); }
    catch ( boost::python::error_already_set ){ PyErr_Clear(); return 0; }
    if( _len != tuplesize ) return 0;
    return py_obj;
  }

  static void construct( PyObject* obj, boost::python::converter::rvalue_from_python_stage1_data* data){
    typedef boost::python::converter::rvalue_from_python_storage<T> vector_storage_t;
    vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data );
    void* memory_chunk = the_storage->storage.bytes;
    boost::python::tuple py_sequence( boost::python::handle<>( boost::python::borrowed( obj ) ) );
    T * result = extract_pgltuple_from_tuple_at<T,tuplesize>(py_sequence,memory_chunk);
    data->convertible = memory_chunk;
  }
};

#endif
