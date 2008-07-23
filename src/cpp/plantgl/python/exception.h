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
#ifndef __py_exception_h__
#define __py_exception_h__

#include <string>
#include <sstream>

struct PythonExc
{
  PythonExc(const char* error = "") : error_str(error) {}
  const char* error_str;
};

struct PythonExc_IndexError : public PythonExc { 
    PythonExc_IndexError(const char* error = "") :PythonExc(error){}  
};

struct PythonExc_ValueError : public PythonExc { 
    PythonExc_ValueError(const char* error = "") :PythonExc(error){}  
};

struct PythonExc_TypeError : public PythonExc { 
    PythonExc_TypeError(const char* error = "") :PythonExc(error){}  
};

struct PythonExc_KeyError : public PythonExc { 
    PythonExc_KeyError(const char* error = "") :PythonExc(error){}  
};

struct PythonExc_StopIteration : public PythonExc { 
    PythonExc_StopIteration(const char* error = "") :PythonExc(error){}  
};

void define_stl_exceptions();


#endif // __py_exception_h__

