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

#include <boost/python.hpp>

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


SETGET(MatrixTransformed,Geometry,GeometryPtr)

DEF_POINTEE(Scaled)
DEF_POINTEE(Translated)
DEF_POINTEE(Transformed)
DEF_POINTEE(MatrixTransformed)
DEF_POINTEE(OrthoTransformed)
DEF_POINTEE(IFS)

void export_Transformed()
{
  class_< Transformed, TransformedPtr, bases< Geometry >, boost::noncopyable >("Transformation", no_init)
		.def("transformation",&Transformed::getTransformation);

  class_< MatrixTransformed, MatrixTransformedPtr, bases< Transformed >, boost::noncopyable  >
    ("MatrixTransformed", no_init)
	.DEC_SETGET(geometry,MatrixTransformed,Geometry,GeometryPtr)
	;
  class_< OrthoTransformed, OrthoTransformedPtr, bases< MatrixTransformed >, boost::noncopyable  >
    ("OrthoTransformed", no_init);

  implicitly_convertible< TransformedPtr, GeometryPtr >();
  implicitly_convertible< MatrixTransformedPtr, TransformedPtr >();
  implicitly_convertible< OrthoTransformedPtr,MatrixTransformedPtr >();
}

SETGET(Scaled,Scale,Vector3)

void export_Scaled()
{
  class_< Scaled, ScaledPtr, bases< MatrixTransformed > , boost::noncopyable >
    ("Scaled", init< const Vector3&, const GeometryPtr& >("Scaled(scale,geometry)",args("scale","geometry")) )
	.DEC_SETGET_WD(scale,Scaled,Scale,Vector3)
    ;

  implicitly_convertible< ScaledPtr, MatrixTransformedPtr >();

}

SETGET(Translated,Translation,Vector3)

void export_Translated()
{
  class_< Translated, TranslatedPtr, bases< MatrixTransformed > , boost::noncopyable >
    ("Translated", init< const Vector3&, const GeometryPtr& >
     ("Translated(translation,geometry)",args("translation","geometry")) )
    .DEC_SETGET_WD(translation,Translated,Translation,Vector3)
    ;

  implicitly_convertible< TranslatedPtr, MatrixTransformedPtr >();

}

SETGET(IFS,TransfoList,Transform4ArrayPtr)
SETGET(IFS,Depth,uchar_t)
SETGET(IFS,Geometry,GeometryPtr)

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
    .DEC_SETGET_WD(depth,IFS,Depth,uchar_t)
	.DEC_SETGET(transfoList,IFS,TransfoList,Transform4ArrayPtr)
	.DEC_SETGET(geometry,IFS,Geometry,GeometryPtr)
	.def("getAllTransformations",&ifs_getAllTransformations);
    ;

  implicitly_convertible< IFSPtr, TransformedPtr >();
}

