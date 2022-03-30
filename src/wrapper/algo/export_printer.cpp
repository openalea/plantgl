/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */



#include "export_printer.h"
#include <plantgl/algo/codec/printer.h>
#include <plantgl/algo/codec/binaryprinter.h>
#include <plantgl/algo/codec/scne_binaryparser.h>
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/tool/bfstream.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

PyStrPrinter::~PyStrPrinter()   { _mystream << std::endl;  }
PyFilePrinter::~PyFilePrinter() { _mystream << std::endl; _mystream.close(); }

/* ----------------------------------------------------------------------- */

void export_StrPrinter()
{
  class_< PyStrPrinter, boost::noncopyable > ("StrPrinter", no_init  )
    .def("str",    &PyStrPrinter::str,  "str() : return string")
    .def("__str__",&PyStrPrinter::str,  "__str__() : return string")
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
    .def("incrementIndentation",&Printer::addIndent)
    .def("isPrinted",&Printer::isPrinted)
    .def("flush",&Printer::flush)
    .def("header",&print_header0)
    .def("header",&print_header)
    ;

  class_< PyStrPGLPrinter , bases< PyStrPrinter, Printer > , boost::noncopyable>
      ("PglStrPrinter",init<>("String Printer in PGL format" ))
      .def(str_printer_clear<>());

  class_< PyFilePGLPrinter , bases< PyFilePrinter, Printer > , boost::noncopyable>
      ("PglFilePrinter",init<const std::string&>("File Printer in PGL format",args("filename")) );
    ;
}

/* ----------------------------------------------------------------------- */

class PyFileBinaryPrinter : public BinaryPrinter {
    public:
        PyFileBinaryPrinter(const std::string& fname) :
          BinaryPrinter(_mystream), _mystream(fname.c_str(), std::ios::out | std::ios::binary)  { }

        ~PyFileBinaryPrinter(){}
        std::ofstream _mystream;
};

template<class Printer>
bool abp_print(Printer* printer, ScenePtr scene)
{
  return printer->print(scene);
}


class PyStrBinaryPrinter : public BinaryPrinter {
public:
    PyStrBinaryPrinter() :  
        BinaryPrinter(_mystream) { }

        /// resulting string
        boost::python::object result() { 
            std::string res = _mystream.str().c_str();
            return object( handle<>( PyBytes_FromStringAndSize(res.c_str(), res.size()))); 
        }
        /// clear the buffer
        void clear() {  this->BinaryPrinter::clear(); _mystream.str(""); }

protected:
    std::ostringstream _mystream;
};

boost::python::object py_tobinarystring(ScenePtr scene, bool double_precision = true,  const char * comment = NULL) { 
    std::string res = BinaryPrinter::tobinarystring(scene, double_precision, comment);
    return object( handle<>( PyBytes_FromStringAndSize(res.c_str(), res.size()))); 
}

ScenePtr py_frombinarystring(boost::python::object bytes) { 
    return BinaryParser::frombinarystring(extract<std::string>(bytes)());
}

void export_PglBinaryPrinter()
{
  class_< PyFileBinaryPrinter, bases< Printer >, boost::noncopyable>
      ("PglBinaryPrinter",init<const std::string&>("Binary Pgl Printer",args("filename")))
    .def("print",abp_print<PyFileBinaryPrinter>)
    .def("getCanonicalFilename",BinaryPrinter::getCanonicalFilename,args("filename"))
    .staticmethod("getCanonicalFilename");
    ;

  class_< PyStrBinaryPrinter , bases< Printer > , boost::noncopyable>
      ("PglStrBinaryPrinter",init<>("String Printer in PGL binary format" ))
      .def("print",abp_print<PyStrBinaryPrinter>)
      .def( "clear", &PyStrBinaryPrinter::clear)
      .add_property("result", &PyStrBinaryPrinter::result)
      ;
    def("tobinarystring", &py_tobinarystring,(bp::arg("scene"),bp::arg("double_precision")=true,bp::arg("comment")=""));
    def("frombinarystring", &py_frombinarystring);
}