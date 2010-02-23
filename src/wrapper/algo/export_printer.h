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

#ifndef __export_printer_h__
#define __export_printer_h__

/* ----------------------------------------------------------------------- */

#include <string>
#include <fstream>
#include <sstream>

/* ----------------------------------------------------------------------- */

/**
   \class PyStrPrinter
   \brief The abstract class for Printer with string output.
*/	

class PyStrPrinter {
	public:
		PyStrPrinter() { }
		~PyStrPrinter();

		/// The output stream
		std::stringstream _mystream;
		/// resulting string
		std::string str() { return _mystream.str(); }
		/// clear the buffer
        void clear() { 	_mystream.str(""); }
};

/* ----------------------------------------------------------------------- */

/**
   \class PyFilePrinter
   \brief The abstract class for Printer with file output.
*/	

class PyFilePrinter {
	public:
		PyFilePrinter(const std::string& fname) : _mystream(fname.c_str()) { }
		~PyFilePrinter();

		/// The output stream
		std::ofstream _mystream;
};

#include <plantgl/python/boost_python.h>

template <class T, class Base>
void py_clear_all(T * printer) {
	printer->Base::clear();
	printer->PyStrPrinter::clear();
}

template <class Base = Printer>
class str_printer_clear : public boost::python::def_visitor<str_printer_clear<Base> >
{
    friend class boost::python::def_visitor_access;
    template <class classT>
    void visit(classT& c) const
	{ c.def( "clear",        &py_clear_all<classT::wrapped_type,Base> ) ; }
};

/* ----------------------------------------------------------------------- */

#endif