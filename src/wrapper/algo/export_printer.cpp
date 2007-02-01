/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): C. Pradal (christophe.pradal@cirad.fr)
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

#include "printer.h"
#include "matrixcomputer.h"
#include "pystream.h"

#include "actn_printer.h"
#include "actn_binaryprinter.h"
#include "actn_plyprinter.h"
#include "actn_vrmlprinter.h"
#include "actn_povprinter.h"
#include "actn_x3dprinter.h"
//#include "actn_xmlprinter.h"
#include "actn_amaptranslator.h"
#include "actn_discretizer.h"
#include "actn_tesselator.h"

#include "appe_appearance.h"
#include "scne_scene.h"

#include <string>
#include "bfstream.h"

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

Printer* ap_fromfile( object file )
{
  pyostream pyos( file );
  return new Printer(pyos);
}

bool (Printer::*pr_header)(const char*)= &Printer::header;

void class_Printer()
{
  class_< Printer, bases< Action > > ("Printer",no_init)
    .def( "__init__", make_constructor( ap_fromfile ) ) 
    .def("clear",&Printer::clear)
    .def("addIndent",&Printer::addIndent)
    .def("isPrinted",&Printer::isPrinted)
    .def("header",pr_header)
    ;
}


BinaryPrinter* abp_fromfilename( boost::python::str file )
{
  std::string fn= extract<std::string>(file);
  leofstream pyos( fn );
  return new BinaryPrinter(pyos);
}
bool abp_print(BinaryPrinter* printer, ScenePtr scene)
{ 
  return printer->print(scene);
}

void class_BinaryPrinter()
{
  class_< BinaryPrinter, bases< Printer > > ("BinaryPrinter",no_init)
    .def("__init__",make_constructor( abp_fromfilename ))
    .def("print",abp_print)
    ;
}


class PyStrPovPrinter : public PovPrinter {
	public:
		PyStrPovPrinter(Tesselator& t) : PovPrinter(_mystream,t) { }
		~PyStrPovPrinter(){}

		std::stringstream _mystream;
		std::string str() { return _mystream.str(); }
		void clear() { return _mystream.clear(); }
};

class PyFilePrinter {
	public:
		PyFilePrinter(const std::string& fname) : _mystream(fname.c_str()) { }
		~PyFilePrinter(){}
		std::ofstream _mystream;
};

class PyFilePovPrinter : public PyFilePrinter, public PovPrinter {
	public:
		PyFilePovPrinter(const std::string& fname,Tesselator& t) : 
		  PyFilePrinter(fname), PovPrinter(_mystream,t) { }
		~PyFilePovPrinter(){}
};

void class_PovPrinter()
{
  class_< PovPrinter, bases< Printer >, boost::noncopyable > ("PovPrinter", no_init )
    .def("setLight",&PovPrinter::setLight,           "setLight(Vector3 position, Color3 color)", args("position","color") )
    .def("setBackground",&PovPrinter::setBackGround, "setBackGround(Color3 color)",args("color"))
    ;
}

void class_StrPovPrinter()
{
  class_< PyStrPovPrinter, bases< PovPrinter >, boost::noncopyable > ("StrPovPrinter", init<Tesselator&>("Create a StrPovPrinter", args("t")) )
	.def("str",    &PyStrPovPrinter::str,  "str() : return string")
	.def("__str__",&PyStrPovPrinter::str,  "__str__() : return string")
	.def("clear",  &PyStrPovPrinter::clear,  "clear() : clear buffer")
    ;
}

void class_FilePovPrinter()
{
	class_< PyFilePovPrinter, bases< PovPrinter >, boost::noncopyable > ("FilePovPrinter", init<const std::string&,Tesselator&>("Create a FilePovPrinter", args("fname","t")) )
    ;
}


VrmlPrinter* make_vrml(object file)
{
  pyostream pyos(file);
  Discretizer d;
  return new VrmlPrinter(pyos,d);
}

void class_VrmlPrinter()
{ 
  class_<VrmlPrinter,bases < Printer > > ("VrmlPrinter",no_init)
    .def("__init__",make_constructor( make_vrml ), ( const char* )"VrmlPrinter(file): file need to be open ")
    .def("setLight",&VrmlPrinter::setLight,( const char* )"setLight( position: Vector3, ambient: Color3, diffuse: Color3)")
    .def("setBackground",&VrmlPrinter::setBackGround,( const char* )"setBackGround( Color3)")
    .def("setCamera",&VrmlPrinter::setCamera,( const char* )"setCamera (Vector3 position, real_t az, real_t el, name)")
    ;
}

X3DPrinter* make_x3d(object file)
{
  pyostream pyos(file);
  Discretizer d;
  return new X3DPrinter(pyos, d);
}

void class_X3DPrinter()
{
  class_< X3DPrinter, bases< VrmlPrinter > > ("X3DPrinter",no_init)
    .def("__init__",make_constructor( make_x3d ), ( const char* )"X3DPrinter(file): file need to be open ")
    .def("header",&X3DPrinter::header)
    ;
}

PlyPrinter* make_ply( object file )
{
  pyostream pyos(file);
  Discretizer d;
  return new PlyPrinter(pyos, d);
}

void class_PlyPrinter() 
{
  class_<PlyPrinter , bases< Printer > > ("PlyPrinter",no_init)
    .def("__init__",make_constructor( make_ply ), ( const char* )"PlyPrinter(file): file need to be open ")
    .def("header",&PlyPrinter::header)
    ;
}

PlyBinaryPrinter* make_plyb( boost::python::str fn )
{ 
  std::string name= extract< std::string >(fn);
  bofstream pyos(name);
  Discretizer d;
  return new PlyBinaryPrinter(pyos, d);
}

void class_PlyBinaryPrinter() 
{
  class_<PlyBinaryPrinter , bases< PlyPrinter > > ("PlyBinaryPrinter",no_init)
    .def("__init__",make_constructor( make_plyb ), ( const char* )"PlyBinaryPrinter(file): file need to be open ")
    .def("isBinary",&PlyBinaryPrinter::isBinary)
    .def("isAscii",&PlyBinaryPrinter::isAscii)
    .def("isLittleEndian",&PlyBinaryPrinter::isLittleEndian)
    .def("isBigEndian",&PlyBinaryPrinter::isBigEndian)
    ;
}

void class_XMLPrinter(){ }

void module_printer()
{
  class_Printer();
  class_BinaryPrinter();
  class_PlyPrinter();
  class_PlyBinaryPrinter();
  class_PovPrinter();
  class_StrPovPrinter();
  class_FilePovPrinter();
  class_VrmlPrinter();
  class_X3DPrinter();
  class_XMLPrinter();

  class_MatrixComputer();
  class_LinetreePrinter();
  class_VgstarPrinter();

}
