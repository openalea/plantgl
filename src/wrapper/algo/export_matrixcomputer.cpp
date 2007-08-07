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

#include "../util/export_property.h"
#include <plantgl/algo/base/matrixcomputer.h>
#include <plantgl/math/util_matrix.h>
#include <boost/python.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;

/* ----------------------------------------------------------------------- */

Matrix4 mc_getMatrix( MatrixComputer* d )
{ return d->getMatrix(); }


void export_MatrixComputer()
{
  class_< MatrixComputer, bases < Action > > 
    ("MatrixComputer", init<>
     ( ( const char* )"MatrixComputer()" 
       "Compute the matrix4 representing the affine transformations. " ))
    .def("clear",&MatrixComputer::clear)
	.DEC_CT_PROPERTY(matrix,MatrixComputer,Matrix,Matrix4)
    .add_property("result",mc_getMatrix)
    ;
}

/* ----------------------------------------------------------------------- */


