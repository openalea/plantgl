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

#include "export_printer.h"

#include <plantgl/algo/codec/povprinter.h>
#include <plantgl/algo/base/tesselator.h>

#include <boost/python.hpp>

PGL_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */


class PyStrPovPrinter : public PyStrPrinter, public PovPrinter {
	public:
		PyStrPovPrinter(Tesselator& t) : PovPrinter(_mystream,t) { }
		~PyStrPovPrinter(){}
};

class PyFilePovPrinter : public PyFilePrinter, public PovPrinter {
	public:
		PyFilePovPrinter(const std::string& fname,Tesselator& t) : 
		  PyFilePrinter(fname), PovPrinter(_mystream,t) { }
		~PyFilePovPrinter(){}
};

/* ----------------------------------------------------------------------- */

void export_PovPrinter()
{
  class_< PovPrinter, bases< Printer >, boost::noncopyable > ("PovPrinter", no_init )
    .def("setLight",&PovPrinter::setLight,           "setLight(Vector3 position, Color3 color)", args("position","color") )
    .def("setBackground",&PovPrinter::setBackGround, "setBackGround(Color3 color)",args("color"))
    ;

  class_< PyStrPovPrinter, bases< PovPrinter, PyStrPrinter >, boost::noncopyable > 
	  ("PovStrPrinter", init<Tesselator&>("String Printer in Povray format", args("t")) )
    ;

  class_< PyFilePovPrinter, bases< PovPrinter, PyFilePrinter >, boost::noncopyable > 
		("PovFilePrinter", init<const std::string&,Tesselator&>("File Printer in Povray format", args("fname","t")) )
    ;
}

/* ----------------------------------------------------------------------- */
