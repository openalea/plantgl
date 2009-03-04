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

#include <plantgl/scenegraph/transformation/ifs.h>
#include <plantgl/scenegraph/transformation/mattransformed.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(IFS)


Matrix4ArrayPtr ifs_getAllTransformations(IFS * ifs)
{
  ITPtr transfos = dynamic_pointer_cast<IT>( ifs->getTransformation() );
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr& matrixList= transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);
  return matrixList;
}

void export_IFS()
{
  class_< IFS, IFSPtr, bases< Transformed > , boost::noncopyable >
    ("IFS", 
	 "The IFS is a recursive transformation applied to an object. Transformation are specified as a set of affine transformations.\n"
     "It is a method for generating fractals and the complexity is exponential with regard to numbers of iterations.",
	init< uchar_t, const Transform4ArrayPtr&, const GeometryPtr& >
       ("IFS(depth, transfoList, geometry)",
	   (bp::arg("depth")=IFS::DEFAULT_DEPTH,
	    bp::arg("transfoList")=Transform4ArrayPtr(),
		bp::arg("geometry")=GeometryPtr())))
    .DEF_PGLBASE(IFS)
    .DEC_BT_NR_PROPERTY_WDV(depth,IFS,Depth,uchar_t,DEFAULT_DEPTH)
	.DEC_PTR_PROPERTY(transfoList,IFS,TransfoList,Transform4ArrayPtr)
	.DEC_PTR_NR_PROPERTY(geometry,IFS,Geometry,GeometryPtr)
	.def("getAllTransformations",&ifs_getAllTransformations);
    ;

  implicitly_convertible< IFSPtr, TransformedPtr >();
}

