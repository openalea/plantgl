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

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/scenegraph/transformation/mattransformed.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/transformation/orthotransformed.h>
#include <plantgl/scenegraph/transformation/ifs.h>
#include <plantgl/scenegraph/geometry/primitive.h>

#include <plantgl/math/util_vector.h>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;


DEF_POINTEE(Scaled)
DEF_POINTEE(Translated)
DEF_POINTEE(Transformed)
DEF_POINTEE(MatrixTransformed)
DEF_POINTEE(OrthoTransformed)
DEF_POINTEE(IFS)

template<class T>
GeometryPtr tr_geometry(T * obj) { return obj->getGeometry(); }

void export_Transformed()
{
  class_< Transformed, TransformedPtr, bases< Geometry >, boost::noncopyable >("Transformed", no_init)
		.def("transformation",&Transformed::getTransformation);

  class_< MatrixTransformed, MatrixTransformedPtr, bases< Transformed >, boost::noncopyable  >
    ("MatrixTransformed", no_init)
	.add_property("geometry", &tr_geometry<MatrixTransformed>, &set_prop_bt_from_class<GeometryPtr,MatrixTransformed,&MatrixTransformed::getGeometry>)
	;
  class_< OrthoTransformed, OrthoTransformedPtr, bases< MatrixTransformed >, boost::noncopyable  >
    ("OrthoTransformed", no_init);

  implicitly_convertible< TransformedPtr, GeometryPtr >();
  implicitly_convertible< MatrixTransformedPtr, TransformedPtr >();
  implicitly_convertible< OrthoTransformedPtr,MatrixTransformedPtr >();
}

ScaledPtr sca_from_val(real_t x, real_t y, real_t z, const GeometryPtr& geom)
{
    return new Scaled(Vector3(x,y,z),geom);
}

ScaledPtr sca1_from_val(real_t x, const GeometryPtr& geom)
{
    return new Scaled(Vector3(x,x,x),geom);
}

void export_Scaled()
{
  class_< Scaled, ScaledPtr, bases< MatrixTransformed > , boost::noncopyable >
    ("Scaled", init< const Vector3&, const GeometryPtr& >("Scaled(scale,geometry)",args("scale","geometry")) )
    .def( "__init__", make_constructor( sca_from_val ) ) 
    .def( "__init__", make_constructor( sca1_from_val ) ) 
	.DEC_CT_PROPERTY_WDV(scale,Scaled,Scale,Vector3,DEFAULT_SCALE)
    ;

  implicitly_convertible< ScaledPtr, MatrixTransformedPtr >();

}

TranslatedPtr tr_from_val(real_t x, real_t y, real_t z, const GeometryPtr& geom)
{
    return new Translated(Vector3(x,y,z),geom);
}

void export_Translated()
{
  class_< Translated, TranslatedPtr, bases< MatrixTransformed > , boost::noncopyable >
    ("Translated", init< const Vector3&, const GeometryPtr& >
     ("Translated(translation,geometry)",args("translation","geometry")) )
    .def( "__init__", make_constructor( tr_from_val ) ) 
    .DEC_CT_PROPERTY_WDV(translation,Translated,Translation,Vector3,DEFAULT_TRANSLATION)
    ;

  implicitly_convertible< TranslatedPtr, MatrixTransformedPtr >();

}

Matrix4ArrayPtr ifs_getAllTransformations(IFS * ifs)
{
  ITPtr transfos;
  transfos.cast( ifs->getTransformation() );
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr& matrixList= transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);
  return matrixList;
}

void export_IFS()
{
  class_< IFS, IFSPtr, bases< Transformed > , boost::noncopyable >
    ("IFS", init< uchar_t, const Transform4ArrayPtr&, const GeometryPtr& >
       (args("depth","transfoList","geometry"),
	"IFS(depth, transfoList, geometry)") )
    .DEC_BT_NR_PROPERTY_WDV(depth,IFS,Depth,uchar_t,DEFAULT_DEPTH)
	.DEC_PTR_PROPERTY(transfoList,IFS,TransfoList,Transform4ArrayPtr)
	.DEC_PTR_NR_PROPERTY(geometry,IFS,Geometry,GeometryPtr)
	.def("getAllTransformations",&ifs_getAllTransformations);
    ;

  implicitly_convertible< IFSPtr, TransformedPtr >();
}

