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
#include <plantgl/algo/codec/pyprinter.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

class PyStrPyPrinter : public PyStrPrinter, public PyPrinter {
public:
    PyStrPyPrinter() :  PyPrinter(_mystream) {}
};

class PyFilePyPrinter : public PyFilePrinter, public PyPrinter {
public:
    PyFilePyPrinter(const std::string& fname) : PyFilePrinter(fname), PyPrinter(_mystream) {}
};

/* ----------------------------------------------------------------------- */

// std::string get

void export_PyPrinter()
{
 class_< PyPrinter, bases< Printer >, boost::noncopyable > ( "PyPrinter" , no_init )
    .add_property("indentation", make_function(&PyPrinter::getIndentation,return_value_policy<return_by_value>()), &PyPrinter::setIndentation )
    .add_property("indentation_increment", make_function(&PyPrinter::getIndentationIncrement,return_value_policy<return_by_value>()), &PyPrinter::setIndentationIncrement )
    .add_property("pglnamespace", make_function(&PyPrinter::getPglNamespace,return_value_policy<return_by_value>()), &PyPrinter::setPglNamespace )
    .add_property("reference_dir", make_function(&PyPrinter::getReferenceDir,return_value_policy<return_by_value>()), &PyPrinter::setReferenceDir )
    .add_property("line_between_object", make_function(&PyPrinter::getLineBetweenObject,return_value_policy<return_by_value>()), &PyPrinter::setLineBetweenObject )
    .def("incrementIndentation",&PyPrinter::incrementIndentation)
    .def("decrementIndentation",&PyPrinter::decrementIndentation)

    ;

  class_< PyStrPyPrinter , bases< PyStrPrinter, PyPrinter > , boost::noncopyable>
      ("PyStrPrinter",init<>("String Printer in python format" ))
      .def(str_printer_clear<>());
      ;

  class_< PyFilePyPrinter , bases< PyFilePrinter, PyPrinter > , boost::noncopyable>
      ("PyFilePrinter",init<const std::string&>("File Printer in python format",args("filename")) );
    ;
}

/* ----------------------------------------------------------------------- */
