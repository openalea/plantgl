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

#ifndef __extract_list_h__
#define __extract_list_h__

#include <boost/python.hpp>
#include <vector>
#include <map>
#include "exception.h"

/* ----------------------------------------------------------------------- */

template<class T, 
    template < typename > class extractor_t = boost::python::extract, 
    class result_type = std::vector<T> >
struct extract_vec {

    typedef T element_type;
    typedef extractor_t<T> extractor_type;

    extract_vec(boost::python::object _pylist):pylist(_pylist) {}
    boost::python::object pylist;

    result_type extract() const {
        result_type result;
        if (pylist.ptr() == Py_None) return result;
        boost::python::object iter_obj = boost::python::object( boost::python::handle<>( PyObject_GetIter( pylist.ptr() ) ) );
        while( true )
          {
            boost::python::object obj; 
            try {  obj = iter_obj.attr( "next" )(); }
            catch( boost::python::error_already_set ){ PyErr_Clear(); break; }
            element_type val = extractor_type( obj )();
            result.push_back( val );
          }
        return result;
    }

    inline result_type operator()() const { return extract(); }
    inline operator result_type () const { return extract(); }
};

/* ----------------------------------------------------------------------- */

template<class key,  class value, 
	 template < typename> class key_extractor_t = boost::python::extract, 
	 template < typename> class value_extractor_t = boost::python::extract,
	 class result_type = std::map<key,value> >
struct extract_dict {

	typedef key key_type;
	typedef value value_type;
	typedef key_extractor_t<key>  key_extractor_type;
	typedef value_extractor_t<value>  value_extractor_type;

        extract_dict(boost::python::object _pydict):
	  pydict(boost::python::extract<boost::python::dict>(_pydict)) {}

	extract_dict(boost::python::dict _pydict):
	  pydict(_pydict) {}

	boost::python::dict pydict;

	result_type extract() const {
		result_type result;
		boost::python::object iter_obj =  pydict.iteritems();
		while( true )
		{
			boost::python::object obj; 
			try {  obj = iter_obj.attr( "next" )(); }
			catch( boost::python::error_already_set ){ PyErr_Clear(); break; }
			key_type k = key_extractor_type( obj[0] );
			value_type v = value_extractor_type( obj[1] );
			result[k] = v ;
		}
		return result;
	}
	inline result_type operator()() const { return extract(); }
	inline operator result_type () const { return extract(); }
};


#endif

/* ----------------------------------------------------------------------- */
