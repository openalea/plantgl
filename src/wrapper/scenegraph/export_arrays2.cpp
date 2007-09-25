/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

#include "../util/export_refcountptr.h"
#include "../util/exception.h"

#include <plantgl/tool/util_array2.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include "arrays2_macro.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;


EXPORT_FUNCTION( p2m, Vector2, Point2Matrix )
EXPORT_FUNCTION( p3m, Vector3, Point3Matrix )
EXPORT_FUNCTION( p4m, Vector4, Point4Matrix )
//EXPORT_FUNCTION( ra, real_t, RealArray2 )

void export_arrays2()
{

  EXPORT_ARRAY( p2m, Point2Matrix, "Point2Matrix(rows,cols)" )
  EXPORT_ARRAY( p3m, Point3Matrix, "Point3Matrix(rows,cols)" )
  EXPORT_ARRAY( p4m, Point4Matrix, "Point4Matrix(rows,cols)" )
    //  EXPORT_ARRAY( ra, RealArray2, "RealArray2(rows,cols)" )
}


