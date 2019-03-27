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
