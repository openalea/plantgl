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



#ifndef __export_printer_h__
#define __export_printer_h__

/* ----------------------------------------------------------------------- */

#include <string>
#include <fstream>
#include <sstream>

#include <plantgl/algo/codec/printer.h>

PGL_USING( Printer )

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
        std::ostringstream _mystream;
        /// resulting string
        std::string str() { return _mystream.str(); }
        /// clear the buffer
        void clear() {  _mystream.str(""); }
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
    { c.def( "clear", &py_clear_all<typename classT::wrapped_type,Base> ) ; }
};

/* ----------------------------------------------------------------------- */

#endif
