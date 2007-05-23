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

#ifndef __export_list_h__
#define __export_list_h__

#include <boost/python.hpp>
#include <vector>
#include "exception.h"

/* ----------------------------------------------------------------------- */
template <class T>
struct make_object {
    boost::python::object operator()(T c_obj){ return boost::python::object(c_obj); }
};

template <class T, template <typename> class Translator = make_object>
struct make_list {
	typedef T list_type;
    typedef typename T::value_type list_element_type;
	typedef Translator<list_element_type> translator_type;

    boost::python::object operator()(T c_list){
        boost::python::list l;
        tr = translator_type();
        for (T::const_iterator it = c_list.begin(); it != c_list.end(); ++it){
            l.append(tr(*it));
        }
        return l;
    }
};
#endif

/* ----------------------------------------------------------------------- */
