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

#include <algo/codec/printer.h>
#include <algo/codec/binaryprinter.h>
#include <algo/codec/plyprinter.h>
#include <algo/codec/vrmlprinter.h>
#include <algo/codec/povprinter.h>
#include <algo/codec/x3dprinter.h>
//#include <algo/codec/xmlprinter.h>
#include <algo/base/amaptranslator.h>
#include <algo/base/discretizer.h>
#include <algo/base/tesselator.h>

#include <scenegraph/appearance/appearance.h>
#include <scenegraph/scene/scene.h>

#include <string>
#include <tool/bfstream.h>

#include <boost/python.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;


class PyStrPrinter {
	public:
		PyStrPrinter() { }
		~PyStrPrinter(){}

		std::stringstream _mystream;
		std::string str() { return _mystream.str(); }
		void clear() { return _mystream.clear(); }
};

void class_StrPrinter()
{
  class_< PyStrPrinter, boost::noncopyable > ("StrPrinter", no_init  )
	.def("str",    &PyStrPrinter::str,  "str() : return string")
	.def("__str__",&PyStrPrinter::str,  "__str__() : return string")
	.def("clear",  &PyStrPrinter::clear,  "clear() : clear the buffer")
    ;
}

class PyFilePrinter {
	public:
		PyFilePrinter(const std::string& fname) : _mystream(fname.c_str()) { }
		~PyFilePrinter(){}
		std::ofstream _mystream;
};

void class_FilePrinter()
{
	class_< PyFilePrinter, boost::noncopyable > 
		("FilePrinter", no_init )
    ;
}
class PyStrPGLPrinter : public PyStrPrinter, public Printer { 
public:
	PyStrPGLPrinter() :  Printer(_mystream,_mystream,_mystream) {}     
};

class PyFilePGLPrinter : public PyFilePrinter, public Printer { 
public:
	PyFilePGLPrinter(const std::string& fname) : PyFilePrinter(fname), Printer(_mystream,_mystream,_mystream) {}   
};  

bool (Printer::*pr_header)(const char*)= &Printer::header;

void class_Printer()
{
 class_< Printer, bases< Action >, boost::noncopyable > ( "PglPrinter" , no_init )
    .def("clear",&Printer::clear)
    .def("addIndent",&Printer::addIndent)
    .def("isPrinted",&Printer::isPrinted)
    .def("header",&pr_header)
    ;

/*  class_< PyStrPGLPrinter , bases< Printer, PyStrPrinter > , boost::noncopyable> 
	  ("StrPglPrinter",init<>("String Pgl Printer" ));
  class_< PyFilePGLPrinter , bases< Printer, PyFilePrinter > , boost::noncopyable> 
	  ("StrPglPrinter",init<const std::string&>("File Pgl Printer",args("filename")) );*/
    ;
}
/*

class PyFileBinaryPrinter : public BinaryPrinter {
	public:
		PyFileBinaryPrinter(const std::string& fname) : BinaryPrinter(_mystream), _mystream(fname.c_str())  { }
		~PyFileBinaryPrinter(){}
		leofstream _mystream;
};

bool abp_print(BinaryPrinter* printer, ScenePtr scene)
{ 
  return printer->print(scene);
}

void class_BinaryPrinter()
{
  class_< PyFileBinaryPrinter, bases< Printer > > ("PglBinaryPrinter",init<const std::string&>("Binary Pgl Printer",args("filename")))
    .def("print",abp_print)
    ;
}*/

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

void class_PovPrinter()
{
  class_< PovPrinter, bases< Printer >, boost::noncopyable > ("PovPrinter", no_init )
    .def("setLight",&PovPrinter::setLight,           "setLight(Vector3 position, Color3 color)", args("position","color") )
    .def("setBackground",&PovPrinter::setBackGround, "setBackGround(Color3 color)",args("color"))
    ;

  class_< PyStrPovPrinter, bases< PovPrinter, PyStrPrinter >, boost::noncopyable > 
	  ("StrPovPrinter", init<Tesselator&>("Create a StrPovPrinter", args("t")) )
    ;

  class_< PyFilePovPrinter, bases< PovPrinter, PyFilePrinter >, boost::noncopyable > 
		("FilePovPrinter", init<const std::string&,Tesselator&>("Create a FilePovPrinter", args("fname","t")) )
    ;
}


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

void class_VrmlPrinter()
{ 
  class_<VrmlPrinter,bases < Printer > > ("VrmlPrinter",no_init)
    .def("setLight",&VrmlPrinter::setLight,( const char* )"setLight( position: Vector3, ambient: Color3, diffuse: Color3)")
    .def("setBackground",&VrmlPrinter::setBackGround,( const char* )"setBackGround( Color3)")
    .def("setCamera",&VrmlPrinter::setCamera,( const char* )"setCamera (Vector3 position, real_t az, real_t el, name)")
    ;

  class_< PyStrVrmlPrinter, bases< VrmlPrinter, PyStrPrinter >, boost::noncopyable > 
	  ("StrVrmlPrinter", init<Discretizer&>("Create a StrVrmlPrinter", args("t")) )
    ;

  class_< PyFileVrmlPrinter, bases< VrmlPrinter, PyFilePrinter >, boost::noncopyable > 
		("FileVrmlPrinter", init<const std::string&,Discretizer&>("Create a FileVrmlPrinter", args("fname","t")) )
    ;

}

/*
LinetreePrinter* alp_make( boost::python::str lig_fn, object dta_file, 
	  boost::python::str smbpath )
{

  std::string lig= extract<std::string>(lig_fn);
  pyostream dtastream(dta_file);
  std::string smbstr= extract<std::string>(smbpath);
  Discretizer d;
  AmapTranslator a(d);
  beofstream pyos(lig );
  return new LinetreePrinter(pyos, dtastream, smbstr,a );

}

void class_LinetreePrinter()
{
  class_< LinetreePrinter, bases< MatrixComputer > > 
    ("LinetreePrinter",no_init)
    .def("__init__",make_constructor( alp_make ))
    ;
}

VgstarPrinter* make_vg( object file )
{
  Tesselator t;
  pyostream pyos(file);
  return new VgstarPrinter(pyos, t);
}

void class_VgstarPrinter()
{
  class_< VgstarPrinter, bases< MatrixComputer > > ("VgstarPrinter",no_init)
    .def("__init__",make_constructor( make_vg ), ( const char* )"VgstarPrinter(file): file need to be open ")
    ;
 }
 */