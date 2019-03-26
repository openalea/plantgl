/***************************************************************************
                          exception.hh  -  description
                             -------------------
    copyright            : (C) 2003 by Pierre Barbier de Reuille
    email                : pierre.barbier@cirad.fr
 ***************************************************************************/

/***************************************************************************
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
 ****************************************************************************/
#ifndef __py_exception_core_h__
#define __py_exception_core_h__

#include "exception.h"
#include <boost/python.hpp>


inline void index_trans(const PythonExc_IndexError& err)
{ PyErr_SetString(PyExc_IndexError, err.error_str); }

inline void value_trans(const PythonExc_ValueError& err)
{ PyErr_SetString(PyExc_ValueError, err.error_str); }

inline void type_trans(const PythonExc_TypeError& err)
{ PyErr_SetString(PyExc_TypeError, err.error_str); }

inline void key_trans(const PythonExc_KeyError& err)
{ PyErr_SetString(PyExc_KeyError, err.error_str); }

inline void stopiteration_trans(const PythonExc_StopIteration& err)
{ PyErr_SetString(PyExc_StopIteration, err.error_str); }

inline void define_stl_exceptions()
{
  boost::python::register_exception_translator<PythonExc_IndexError>(&index_trans);
  boost::python::register_exception_translator<PythonExc_ValueError>(&value_trans);
  boost::python::register_exception_translator<PythonExc_TypeError>(&type_trans);
  boost::python::register_exception_translator<PythonExc_KeyError>(&key_trans);
  boost::python::register_exception_translator<PythonExc_StopIteration>(&stopiteration_trans);
}


#endif // __py_exception_core_h__

