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

#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/scenegraph/geometry/mesh.h>
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/appearance/material.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"
#include <plantgl/python/extract_list.h>

#include <boost/python/make_constructor.hpp>
#include <string>
#include <sstream>

using namespace boost::python;
#define bp boost::python

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DEF_POINTEE( PointSet )


std::string gps_repr( PointSet* p )
{
  std::stringstream ss;
  ss << "PointSet(" << extract<std::string>(boost::python::str(boost::python::object(p->getPointList())))();
  if (p->getColorList())
    ss << "," << extract<std::string>(boost::python::str(boost::python::object(p->getColorList())))() ;
  ss << ")";
  return ss.str();
}

ScenePtr convertPointSetToShapes(PointSet * ps, Vector3 translation = Vector3(0,0,0)) {
	Point3ArrayPtr points = ps->getPointList();
	Color4ArrayPtr colors = ps->getColorList();
	Color4Array::const_iterator itColor;
	if(colors) itColor = colors->begin();
	ScenePtr result(new Scene());
	size_t id = 0;
	AppearancePtr mat(new Material("DefaultPointSetMat"));
	for(Point3Array::const_iterator itPoint = points->begin(); itPoint != points->end(); ++itPoint,++id){
		result->add(Shape3DPtr(new Shape(GeometryPtr(new PointSet(Point3ArrayPtr(new Point3Array(1,(*itPoint)+translation)),
			                                                      Color4ArrayPtr(colors?new Color4Array(1,*itColor):NULL))),
						                  mat,id)));		
		if(colors) ++itColor;
	}
	return result;
}

void export_PointSet()
{
  class_< PointSet, PointSetPtr, bases<ExplicitModel>, boost::noncopyable>( "PointSet",
	  "PointSet describes an explicit set of points",
	  init<Point3ArrayPtr, optional<Color4ArrayPtr, uchar_t> >("PointSet(Point3Array pointList, Color4Array colorList = None)",
	  (bp::arg("pointList"),bp::arg("colorList")=Color4ArrayPtr(),bp::arg("width") = PointSet::DEFAULT_WIDTH))) 
    .DEF_PGLBASE(PointSet)
    .def( "__repr__", gps_repr )
    .def( "transform", &PointSet::transform )
    .DEC_BT_NR_PROPERTY_WDV(width,PointSet,Width,uchar_t,DEFAULT_WIDTH)
	.def("convertToShapes",&convertPointSetToShapes,(bp::arg("translation")=Vector3(0,0,0)))
    ;
  implicitly_convertible<PointSetPtr, ExplicitModelPtr>();
}

DEF_POINTEE( PointSet2D )

std::string gps2d_repr( PointSet2D* p )
{
  std::stringstream ss;
  ss << "PointSet2D(" << extract<std::string>(boost::python::str(boost::python::object(p->getPointList())))() << ")";
  return ss.str();
}


void export_PointSet2D()
{
  class_< PointSet2D, PointSet2DPtr, bases<PlanarModel>, boost::noncopyable>( 
	  "PointSet2D", "PointSet2D describes an explicit set of 2D points. See PointSet.",
	  init<Point2ArrayPtr,optional<uchar_t> >("PointSet2D(pointList[,width])",(bp::arg("pointList"),bp::arg("width") = PointSet::DEFAULT_WIDTH)) )
    .DEF_PGLBASE(PointSet2D)
    .def( "__repr__", gps2d_repr )
	.DEC_PTR_PROPERTY(pointList,PointSet2D,PointList,Point2ArrayPtr)
    .DEC_BT_NR_PROPERTY(width,PointSet2D,Width,uchar_t)
    DEC_DEFAULTVALTEST(PointSet2D,Width)
    ;
  implicitly_convertible<PointSet2DPtr, PlanarModelPtr>();
}

