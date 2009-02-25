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

#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/scenegraph/transformation/mattransformed.h>
#include <plantgl/scenegraph/transformation/orthotransformed.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;

DEF_POINTEE(Transformed)
DEF_POINTEE(MatrixTransformed)
DEF_POINTEE(OrthoTransformed)
DEF_POINTEE(IFS)

template<class T>
GeometryPtr tr_geometry(T * obj) { return obj->getGeometry(); }

void export_Transformed()
{
  class_< Transformed, TransformedPtr, bases< Geometry >, boost::noncopyable >("Transformed", 
	  "Abstract base class for trasnformed node.\n It suppose to contain object on which the transformation is applied.", no_init)
		.def("transformation",&Transformed::getTransformation);

  class_< MatrixTransformed, MatrixTransformedPtr, bases< Transformed >, boost::noncopyable  >
    ("MatrixTransformed", "Abstract base class for objects on which a transformation that can be express as a matrix is applied.", no_init)
	.add_property("geometry", &tr_geometry<MatrixTransformed>, &set_prop_bt_from_class<GeometryPtr,MatrixTransformed,&MatrixTransformed::getGeometry>)
	;
  class_< OrthoTransformed, OrthoTransformedPtr, bases< MatrixTransformed >, boost::noncopyable  >
    ("OrthoTransformed", "Abstract base class for objects on which a transformation that can be express as an orthonormal matrix is applied.", no_init);

  implicitly_convertible< TransformedPtr, GeometryPtr >();
  implicitly_convertible< MatrixTransformedPtr, TransformedPtr >();
  implicitly_convertible< OrthoTransformedPtr,MatrixTransformedPtr >();
}

