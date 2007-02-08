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
#include <algo/base/matrixcomputer.h>
#include <algo/codec/linetreeprinter.h>
#include <algo/codec/vgstarprinter.h>
#include <algo/base/amaptranslator.h>
#include <algo/base/discretizer.h>

#include <math/util_matrix.h>
#include <scenegraph/scene/scene.h>

#include <string>

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE
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



