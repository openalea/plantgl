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
#include <algo/codec/printer.h>
#include <algo/codec/binaryprinter.h>
#include <scenegraph/scene/scene.h>
#include <tool/bfstream.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

PyStrPrinter::~PyStrPrinter()   {}
PyFilePrinter::~PyFilePrinter() {}

/* ----------------------------------------------------------------------- */

void export_StrPrinter()
{
  class_< PyStrPrinter, boost::noncopyable > ("StrPrinter", no_init  )
	.def("str",    &PyStrPrinter::str,  "str() : return string")
	.def("__str__",&PyStrPrinter::str,  "__str__() : return string")
	.def("clear",  &PyStrPrinter::clear,  "clear() : clear the buffer")
	.add_property("result", &PyStrPrinter::str)
    ;
}

void export_FilePrinter()
{
	class_< PyFilePrinter, boost::noncopyable > 
		("FilePrinter", no_init )
    ;
}

/* ----------------------------------------------------------------------- */

class PyStrPGLPrinter : public PyStrPrinter, public Printer { 
public:
	PyStrPGLPrinter() :  Printer(_mystream,_mystream,_mystream) {}     
};

class PyFilePGLPrinter : public PyFilePrinter, public Printer { 
public:
	PyFilePGLPrinter(const std::string& fname) : PyFilePrinter(fname), Printer(_mystream,_mystream,_mystream) {}   
};  

/* ----------------------------------------------------------------------- */


void print_header0(Printer * p) 
{ p->header(); }

void print_header(Printer * p, const std::string comment)
{ p->header(comment.c_str()); }

void export_PglPrinter()
{
 class_< Printer, bases< Action >, boost::noncopyable > ( "PglPrinter" , no_init )
    .def("clear",&Printer::clear)
    .def("addIndent",&Printer::addIndent)
    .def("isPrinted",&Printer::isPrinted)
    .def("header",&print_header0)
    .def("header",&print_header)
    ;

  class_< PyStrPGLPrinter , bases< Printer, PyStrPrinter > , boost::noncopyable> 
	  ("StrPglPrinter",init<>("String Pgl Printer" ));

  class_< PyFilePGLPrinter , bases< Printer, PyFilePrinter > , boost::noncopyable> 
	  ("StrPglPrinter",init<const std::string&>("File Pgl Printer",args("filename")) );
    ;
}

/* ----------------------------------------------------------------------- */

class PyFileBinaryPrinter : public BinaryPrinter {
	public:
		PyFileBinaryPrinter(const std::string& fname) : 
		  BinaryPrinter(_mystream), _mystream(fname.c_str())  { }

		~PyFileBinaryPrinter(){}
		leofstream _mystream;
};

bool abp_print(BinaryPrinter* printer, ScenePtr scene)
{ 
  return printer->print(scene);
}

void export_PglBinaryPrinter()
{
  class_< PyFileBinaryPrinter, bases< Printer >, boost::noncopyable> 
	  ("PglBinaryPrinter",init<const std::string&>("Binary Pgl Printer",args("filename")))
    .def("print",abp_print)
    ;
}
