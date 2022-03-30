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


#ifndef __extract_tuple_h__
#define __extract_tuple_h__

#include "boost_python.h"
#include <plantgl/tool/util_tuple.h>
#include "exception.h"
#include "pyseq_iterator.h"

/* ----------------------------------------------------------------------- */

template<class result_type,
        template < typename > class extractor_t = boost::python::extract>
struct extract_tuple {

    typedef typename result_type::value_type element_type;
    typedef extractor_t<element_type> extractor_type;

    extract_tuple(boost::python::object _pytuple):pylist(_pytuple) {}
    boost::python::object pylist;

    bool check() {
        if (pylist.ptr() == Py_None) return false;
        if (!PyIter_Check(pylist.ptr())) return false;
        if (len(pylist) != result_type::SIZE) return false;
        return true;
    }

    result_type extract() const {
        result_type result;
        if (pylist.ptr() == Py_None) return result;
        // boost::python::object iter_obj = boost::python::object( boost::python::handle<PyObject>( PyObject_GetIter( pylist.ptr() ) ) );
        PySeqIterator iter_obj ( pylist );
        for(size_t i = 0 ; i < result_type::SIZE && iter_obj.is_valid() ; ++i)
        {
            boost::python::object obj = iter_obj.next();
            result[i] = extractor_type( obj )();
        }
        return result;
    }

    inline result_type operator()() const { return extract(); }
    inline operator result_type () const { return extract(); }
};

/* ----------------------------------------------------------------------- */

#endif

/* ----------------------------------------------------------------------- */
