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
#include <plantgl/algo/codec/vgstarprinter.h>
#include <plantgl/algo/base/tesselator.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

class PyStrVgstarPrinter : public PyStrPrinter, public VgstarPrinter {
	public:
		PyStrVgstarPrinter(Tesselator& t) : VgstarPrinter(_mystream,t) { }
		~PyStrVgstarPrinter(){}
};

class PyFileVgstarPrinter : public PyFilePrinter, public VgstarPrinter {
	public:
		PyFileVgstarPrinter(const std::string& fname,Tesselator& t) : 
		  PyFilePrinter(fname), VgstarPrinter(_mystream,t) { }
		~PyFileVgstarPrinter(){}
};

/* ----------------------------------------------------------------------- */

void export_VgstarPrinter()
{ 
  class_<VgstarPrinter,bases < MatrixComputer > > ("VgstarPrinter",no_init)
    ;

  class_< PyStrVgstarPrinter, bases< VgstarPrinter, PyStrPrinter >, boost::noncopyable > 
	  ("VgStarStrPrinter", init<Tesselator&>("String Printer in VgStar format", args("t")) )
    ;

  class_< PyFileVgstarPrinter, bases< VgstarPrinter, PyFilePrinter >, boost::noncopyable > 
		("VgStarFilePrinter", init<const std::string&,Tesselator&>("File Printer in VgStar format", args("fname","t")) )
    ;

}

/* ----------------------------------------------------------------------- */


