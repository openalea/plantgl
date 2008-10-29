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

#ifndef __extract_pgl_h__
#define __extract_pgl_h__

#include <boost/python.hpp>
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
		boost::python::object iter_obj = boost::python::object( boost::python::handle<>( PyObject_GetIter( pylist.ptr() ) ) );
		while( true )
		{
			boost::python::object obj; 
			try  {  obj = iter_obj.attr( "next" )(); }
			catch( boost::python::error_already_set ){ PyErr_Clear(); break; }
			element_type val = extractor_type( obj )();
			result->pushBack( val );
		}
		return result;
	}
	T * extract() const {
        RCPtr<T> exact_list = extract_rcptr();
        if (exact_list) return exact_list.get();
        else return extract_list();
    }

	inline RCPtr<T> toRCPtr() const {
        RCPtr<T> exact_list = extract_rcptr();
        if (exact_list) return exact_list;
        else return RCPtr<T>(extract_list()); 
    }
 
	inline T * operator()() const { return extract(); }
	inline operator T * () const { return extract(); }
	inline operator RCPtr<T> () const { return toRCPtr(); }

};

/* ----------------------------------------------------------------------- */

#endif

/* ----------------------------------------------------------------------- */
