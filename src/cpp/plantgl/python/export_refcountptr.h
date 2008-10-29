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

#ifndef __export_refcountptr__
#define __export_refcountptr__

#include <boost/python.hpp>
#include <boost/pointee.hpp>
#include <plantgl/tool/rcobject.h>

TOOLS_USING_NAMESPACE


#ifdef NO_BOOST_REFCOUNTPTR
#define DEF_POINTEE(CLASS) \
namespace boost { namespace python { \
template <> struct pointee<RCPtr<CLASS> > { typedef CLASS type; }; } }
#else
#define DEF_POINTEE(CLASS)
#endif

#include "intrusive_ptr_python_helper.h"

// resolve conflict between initialize_wrapper for RefCountObject and for boost::python::base
#define BOOST_INITIALIZE_WRAPPER_FIX(T) \
	namespace boost { namespace python { namespace detail { \
		inline void initialize_wrapper(PyObject* self, T * w){ \
			initialize_wrapper(self,(boost::python::detail::wrapper_base*)w); \
			intrusive_ptr_set_pyobject(w,self); \
		} \
	}}} \

#endif
