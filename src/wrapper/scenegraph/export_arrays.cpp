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

#include <plantgl/tool/util_array.h>
#include <plantgl/tool/util_tuple.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include <plantgl/python/exception.h>
#include <plantgl/python/export_refcountptr.h>
#include "arrays_macro.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;

EXPORT_FUNCTION( c3a,  Color3Array )
EXPORT_FUNCTION( c4a,  Color4Array )
EXPORT_FUNCTION( i3a,  Index3Array )
EXPORT_FUNCTION( i4a,  Index4Array )
EXPORT_FUNCTION( inda, IndexArray )
EXPORT_FUNCTION( ra,   RealArray )

EXPORT_NUMPY( c3a, Color3, Color3Array, 0, 3, uchar_t )
EXPORT_NUMPY( c4a, Color4, Color4Array, 0, 4, uchar_t )
EXPORT_NUMPY( i3a, Index3, Index3Array, 0, 3, uint_t )
EXPORT_NUMPY( i4a, Index4, Index4Array, 0, 4, uint_t )
EXPORT_NUMPY( inda, Index, IndexArray, 0, 0, uint_t )
EXPORT_NUMPY_1DIM( ra, real_t, RealArray, 0, real_t )

void export_arrays()
{
  EXPORT_ARRAY_CT( c3a, Color3Array, "Color3Array([Index3(i,j,k),...])" )
    DEFINE_NUMPY( c3a );
  EXPORT_CONVERTER(Color3Array);
  EXPORT_ARRAY_CT( c4a, Color4Array, "Color4Array([Index4(i,j,k,l),...])" )
    DEFINE_NUMPY( c4a );
  EXPORT_CONVERTER(Color4Array);

  EXPORT_ARRAY_CT( i3a, Index3Array, "Index3Array([Index3(i,j,k),...])" )
    DEFINE_NUMPY( i3a );
  EXPORT_CONVERTER(Index3Array);
  EXPORT_ARRAY_CT( i4a, Index4Array, "Index4Array([Index4(i,j,k,l),...])" )
    .def( "triangulate", &Index4Array::triangulate)
    DEFINE_NUMPY( i4a );
  EXPORT_CONVERTER(Index4Array);
  EXPORT_ARRAY_CT( inda,IndexArray,  "IndexArray([Index([i,j,..]),...])" )
    .def( "triangulate", &IndexArray::triangulate)
    DEFINE_NUMPY( inda );
  EXPORT_CONVERTER(IndexArray);
  EXPORT_ARRAY_BT( ra, RealArray,  "IndexArray([a,b,...])" )
    DEFINE_NUMPY( ra );
  EXPORT_CONVERTER(RealArray);
}


