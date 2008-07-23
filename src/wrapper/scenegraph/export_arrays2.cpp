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

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/exception.h>

#include <plantgl/tool/util_array2.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include "arrays2_macro.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;


EXPORT_FUNCTION( p2m, Point2Matrix )
EXPORT_FUNCTION( p3m, Point3Matrix )
EXPORT_FUNCTION( p4m, Point4Matrix )
EXPORT_FUNCTION( ra,  RealArray2 )

void export_arrays2()
{

  EXPORT_ARRAY_CT( p2m, Point2Matrix );
  EXPORT_CONVERTER(Point2Matrix);

  EXPORT_ARRAY_CT( p3m, Point3Matrix );
  EXPORT_CONVERTER(Point3Matrix);

  EXPORT_ARRAY_CT( p4m, Point4Matrix );
  EXPORT_CONVERTER(Point4Matrix);

  EXPORT_ARRAY_BT( ra, RealArray2 )
   .def(numarray2_func<RealArray2>());
  EXPORT_CONVERTER(RealArray2);
}


