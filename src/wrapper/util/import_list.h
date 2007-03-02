/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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

#include <boost/python.hpp>
#include <vector>

/* ----------------------------------------------------------------------- */

template<class T,class extractor = boost::python::extract<T::element_type> >
struct extract_pgllist {
	typename T result_type;

	extract_pgllist(boost::python::object _pylist):pylist(_pylist) {}
	boost::python::object pylist;

	T * operator()() const {
		T* array = new T();
		object iter_obj = boost::python::object( handle<>( PyObject_GetIter( pylist.ptr() ) ) );
		while( true )
		{
			object obj; 
			try  {  obj = iter_obj.attr( "next" )(); }
			catch( error_already_set ){ PyErr_Clear(); break; }
			T::element_type val = extractor( obj )();
			array->pushBack( val );
		}
		return array;
	}
	operator T * () const { return operator()(); }

};

/* ----------------------------------------------------------------------- */

template<class T,class extractor = boost::python::extract<T> >
struct extract_vec {

	typename T result_type;

	extract_vec(boost::python::object _pylist):pylist(_pylist) {}
	boost::python::object pylist;

	std::vector<T> operator()() const {
		std::vector<T> array;
		object iter_obj = boost::python::object( handle<>( PyObject_GetIter( pylist.ptr() ) ) );
		while( true )
		{
			object obj; 
			try {  obj = iter_obj.attr( "next" )(); }
			catch( error_already_set ){ PyErr_Clear(); break; }
			T val = boost::python::extract<T>( obj );
			array->push_back( val );
		}
		return array;
	}
	operator T * () const { return operator()(); }
};

/* ----------------------------------------------------------------------- */
