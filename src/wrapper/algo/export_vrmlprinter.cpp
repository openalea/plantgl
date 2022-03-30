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
#include <plantgl/algo/codec/vrmlprinter.h>
#include <plantgl/algo/base/discretizer.h>
#include <plantgl/scenegraph/appearance/appearance.h>
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

  class_< PyStrVrmlPrinter, bases< PyStrPrinter, VrmlPrinter >, boost::noncopyable >
      ("StrVrmlPrinter", init<Discretizer&>("String Printer in Vrml format", args("t")) )
      .def(str_printer_clear<>());
    ;

  class_< PyFileVrmlPrinter, bases< PyFilePrinter, VrmlPrinter >, boost::noncopyable >
        ("FileVrmlPrinter", init<const std::string&,Discretizer&>("File Printer in Vrml format", args("fname","t")) )
    ;

}

/* ----------------------------------------------------------------------- */


