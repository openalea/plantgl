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


#ifndef __extract_pgl_h__
#define __extract_pgl_h__

// #include <boost/python.hpp>
#include "export_refcountptr.h"
#include <plantgl/tool/rcobject.h>
#include "exception.h"

/* ----------------------------------------------------------------------- */
template<class T >
struct extract_pglptr {
    typedef typename T::element_type element_type;
    typedef T result_type;

    extract_pglptr(boost::python::object obj):pyobject(obj) {}
    boost::python::object pyobject;

    result_type extract() const {
        /// RefCountPtr cannot be extracted directly. First extract ptr and encapsulate it into RefCountPtr.
        boost::python::extract<element_type *> direct_extractor(pyobject);
        if (direct_extractor.check()) return result_type(direct_extractor());
        else return result_type(0);
    }
    inline result_type operator()() const { return extract(); }
    inline operator result_type () const { return extract(); }
};

/* ----------------------------------------------------------------------- */

template<class T, template < typename > class extractor_t = boost::python::extract >
struct extract_pgllist {
    typedef typename T::element_type element_type;
    typedef extractor_t<element_type> extractor_type;
    typedef T result_type;

    extract_pgllist(boost::python::object _pylist):pylist(_pylist) {}
    boost::python::object pylist;

    RCPtr<T> extract_rcptr() const {
        boost::python::extract<T *> direct_extractor(pylist);
        if (direct_extractor.check()) return RCPtr<T>(direct_extractor());
        else return RCPtr<T>();
    }

    T * extract_list() const {
        if (pylist.ptr() == Py_None) return NULL;
        T * result (new T());
#ifdef PYTHON_EXTRACT_WITH_EXCEPTION
        boost::python::object iter_obj = boost::python::object( boost::python::handle<PyObject>( PyObject_GetIter( pylist.ptr() ) ) );
        while( true )
        {
            boost::python::object obj;
            try  {  obj = iter_obj.attr( "next" )(); }
            catch( boost::python::error_already_set ){ PyErr_Clear(); break; }
            element_type val = extractor_type( obj )();
            result->push_back( val );
        }
#else
        PyObject *seq = PyObject_GetIter(pylist.ptr());

        if(!seq)
            return NULL;

        PyObject *item;
        while( (item=PyIter_Next(seq))) {
            element_type val = extractor_type( item )();
            result->push_back( val );

            Py_DECREF(item);
        }
        Py_DECREF(seq);
#endif
        return result;
    }
    T * extract() const {
        RCPtr<T> exact_list = extract_rcptr();
        if (exact_list) return exact_list.get();
        else return extract_list();
    }

    inline RCPtr<T> toRCPtr(bool copy = false) const {
        RCPtr<T> exact_list = extract_rcptr();
        if (exact_list) {
            if (!copy) return exact_list;
            else return RCPtr<T>(new T(*exact_list));
        }
        else return RCPtr<T>(extract_list());
    }

    inline T * operator()() const { return extract(); }
    inline operator T * () const { return extract(); }
    inline operator RCPtr<T> () const { return toRCPtr(); }

};

/* ----------------------------------------------------------------------- */

#endif

/* ----------------------------------------------------------------------- */
