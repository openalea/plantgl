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

#include "matrixcomputer.h"
#include "pystream.h"

#include "actn_printer.h"
#include "actn_matrixcomputer.h"
#include "actn_linetreeprinter.h"
#include "actn_vgstarprinter.h"
#include "actn_amaptranslator.h"
#include "actn_discretizer.h"

#include "util_matrix.h"

#include "scne_scene.h"

#include <string>

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;


Matrix4 mc_getMatrix(MatrixComputer* mc)
{
  return mc->getMatrix();
}

void mc_setMatrix(MatrixComputer* mc, const Matrix4& m4)
{
  mc->getMatrix()= m4;
}

void class_MatrixComputer()
{
  class_< MatrixComputer, bases < Action > > 
    ("MatrixComputer", init<>
     ( ( const char* )"MatrixComputer()" 
       "Compute the matrix4 representing the affine transformations. " ))
    .def("clear",&MatrixComputer::clear)
    .add_property("matrix",mc_getMatrix, mc_setMatrix)
    ;
}


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

