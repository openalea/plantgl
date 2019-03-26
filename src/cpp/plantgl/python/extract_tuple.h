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

#ifndef __extract_tuple_h__
#define __extract_tuple_h__

#include "boost_python.h"
#include <plantgl/tool/util_tuple.h>
#include "exception.h"

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
        boost::python::object iter_obj = boost::python::object( boost::python::handle<>( PyObject_GetIter( pylist.ptr() ) ) );
        for(size_t i = 0 ; i < result_type::SIZE ; ++i)
        {
            boost::python::object obj = iter_obj.attr( "next" )();
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
