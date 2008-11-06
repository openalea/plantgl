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

#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/python/export_refcountptr.h>
#include "arrays_macro.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;



EXPORT_FUNCTION( geoma, GeometryArray )
EXPORT_FUNCTION( crv2a, Curve2DArray )
EXPORT_FUNCTION( crva, CurveArray )

void export_geometryarrays()
{
  EXPORT_ARRAY_PTR( geoma, GeometryArray, "GeometryArray([Geometry,...])");
  EXPORT_CONVERTER(GeometryArray);

  EXPORT_ARRAY_PTR( crv2a, Curve2DArray, "Curve2DArray([Curve2D,...])");
  EXPORT_CONVERTER(Curve2DArray);

  EXPORT_ARRAY_PTR( crva, CurveArray, "CurveArray([Curve,...])");
  EXPORT_CONVERTER(CurveArray);
}

