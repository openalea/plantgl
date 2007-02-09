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
#include <algo/codec/vrmlprinter.h>
#include <algo/base/discretizer.h>
#include <scenegraph/appearance/appearance.h>
#include <boost/python.hpp>

PGL_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

class PyStrVrmlPrinter : public PyStrPrinter, public VrmlPrinter {
	public:
		PyStrVrmlPrinter(Discretizer& t) : VrmlPrinter(_mystream,t) { }
		~PyStrVrmlPrinter(){}
};

class PyFileVrmlPrinter : public PyFilePrinter, public VrmlPrinter {
	public:
		PyFileVrmlPrinter(const std::string& fname,Discretizer& t) : 
		  PyFilePrinter(fname), VrmlPrinter(_mystream,t) { }
		~PyFileVrmlPrinter(){}
};

/* ----------------------------------------------------------------------- */

void export_VrmlPrinter()
{ 
  class_<VrmlPrinter,bases < Printer > > ("VrmlPrinter",no_init)
    .def("setLight",&VrmlPrinter::setLight,( const char* )"setLight( position: Vector3, ambient: Color3, diffuse: Color3)")
    .def("setBackground",&VrmlPrinter::setBackGround,( const char* )"setBackGround( Color3)")
    .def("setCamera",&VrmlPrinter::setCamera,( const char* )"setCamera (Vector3 position, real_t az, real_t el, name)")
    ;

  class_< PyStrVrmlPrinter, bases< VrmlPrinter, PyStrPrinter >, boost::noncopyable > 
	  ("StrVrmlPrinter", init<Discretizer&>("String Printer in Vrml format", args("t")) )
    ;

  class_< PyFileVrmlPrinter, bases< VrmlPrinter, PyFilePrinter >, boost::noncopyable > 
		("FileVrmlPrinter", init<const std::string&,Discretizer&>("File Printer in Vrml format", args("fname","t")) )
    ;

}

/* ----------------------------------------------------------------------- */


