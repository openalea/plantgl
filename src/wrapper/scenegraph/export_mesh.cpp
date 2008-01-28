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
#include <plantgl/scenegraph/geometry/mesh.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/scenegraph/container/pointarray.h>

#include "../util/export_property.h"
#include "../util/export_refcountptr.h"

using namespace boost::python;

PGL_USING_NAMESPACE

DEF_POINTEE( ExplicitModel )
DEF_POINTEE( Mesh )

void export_ExplicitModel()
{
  class_<ExplicitModel, ExplicitModelPtr, bases<Primitive>, boost::noncopyable>( "ExplicitModel", no_init )
    .def( "transform", &ExplicitModel::transform )
	.DEC_PTR_PROPERTY(pointList,ExplicitModel,PointList,Point3ArrayPtr)
	.DEC_PTR_PROPERTY_WD(colorList,ExplicitModel,ColorList,Color4ArrayPtr)
    ;
  implicitly_convertible<ExplicitModelPtr, PrimitivePtr>();

}

void export_Mesh()
{
  class_<Mesh, MeshPtr, bases<ExplicitModel>, boost::noncopyable>( "Mesh", no_init )
	  .def("indexListSize",&Mesh::getIndexListSize)
	  .DEC_BT_NR_PROPERTY_WDV(solid,            Mesh, Solid,           bool,  DEFAULT_SOLID)
	  .DEC_BT_NR_PROPERTY_WDV(ccw,              Mesh, CCW,             bool,  DEFAULT_CCW)
	  .DEC_BT_NR_PROPERTY_WDV(normalPerVertex,  Mesh, NormalPerVertex, bool,  DEFAULT_NORMALPERVERTEX)
	  .DEC_BT_NR_PROPERTY_WDV(colorPerVertex,   Mesh, ColorPerVertex,  bool,  DEFAULT_COLORPERVERTEX)
	  .DEC_PTR_PROPERTY_WDV(skeleton,        Mesh, Skeleton,        PolylinePtr,DEFAULT_SKELETON)
	  .DEC_PTR_PROPERTY_WD(normalList,      Mesh, NormalList,      Point3ArrayPtr)
	  .DEC_PTR_PROPERTY_WD(texCoordList,    Mesh, TexCoordList,    Point2ArrayPtr)
	  .def("computeNormalList",  (void (Mesh::*)())&Mesh::computeNormalList)
	  .def( "pointAt",    (const Vector3& (Mesh::*)(uint32_t) const)&Mesh::getPointAt,    return_value_policy<copy_const_reference>() )
	  .def( "pointAt",    (const Vector3& (Mesh::*)(uint32_t,uint32_t) const)&Mesh::getFacePointAt,    return_value_policy<copy_const_reference>() )
	  .def( "getFaceCenter",    &Mesh::getFaceCenter )
	  .def( "getFaceSize",    &Mesh::getFaceSize )

	  ;
  implicitly_convertible<MeshPtr, ExplicitModelPtr>();

}
