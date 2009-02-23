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

#include <plantgl/scenegraph/geometry/quadset.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/pointarray.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>

#include <boost/python.hpp>
#include <string>
#include <sstream>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;
#define bp boost::python

DEF_POINTEE( QuadSet )



void export_QuadSet()
{
  class_<QuadSet, QuadSetPtr, bases<Mesh>, boost::noncopyable> ( "QuadSet", 
	  "A QuadSet describes a surface formed by a set of connected quadrilaterals, i.e. four sided polygons.\n Quads are specified using set of tuples of 4 indices (Index4) pointing to a list of points.",
	  init<Point3ArrayPtr, Index4ArrayPtr, Point3ArrayPtr,
	           optional<Index4ArrayPtr,Color4ArrayPtr,Index4ArrayPtr,
			            Point2ArrayPtr,Index4ArrayPtr,
						bool, bool, bool, bool, PolylinePtr> >
			("QuadSet(Point3Array pointList, Index4Array indexList [, Point3Array normalList, Index4Array nomalIndexList, "
			 "Color4Array colorList, Index4Array colorIndexList, Point2Array texCoordList, Index4Array texCoordIndexList, "
			 "bool normalPerVertex, bool colorPerVertex, bool ccw, bool solid, Polyline skeleton])",
			 (bp::arg("pointList")         = Point3ArrayPtr(),
			  bp::arg("indexList")         = Index4ArrayPtr(),
			  bp::arg("normalList")        = Point3ArrayPtr(),
			  bp::arg("nomalIndexList")    = Index4ArrayPtr(),
			  bp::arg("colorList")         = Color4ArrayPtr(),
			  bp::arg("colorIndexList")    = Index4ArrayPtr(),
			  bp::arg("texCoordList")      = Point2ArrayPtr(),
			  bp::arg("texCoordIndexList") = Index4ArrayPtr(),
			  bp::arg("normalPerVertex")= Mesh::DEFAULT_NORMALPERVERTEX,
			  bp::arg("colorPerVertex") = Mesh::DEFAULT_COLORPERVERTEX,
			  bp::arg("ccw")            = Mesh::DEFAULT_CCW,
			  bp::arg("solid")          = Mesh::DEFAULT_SOLID,
			  bp::arg("skeleton")       = Mesh::DEFAULT_SKELETON)))

	.def( "deepcopy", &QuadSet::copy )

	.DEC_PTR_PROPERTY(indexList,            QuadSet,IndexList,        Index4ArrayPtr)
	.DEC_PTR_PROPERTY_WD(normalIndexList,   QuadSet,NormalIndexList,  Index4ArrayPtr)
	.DEC_PTR_PROPERTY_WD(colorIndexList,    QuadSet,ColorIndexList,   Index4ArrayPtr)
	.DEC_PTR_PROPERTY_WD(texCoordIndexList, QuadSet,TexCoordIndexList,Index4ArrayPtr)
 
    .def( "normalAt",   (const Vector3& (QuadSet::*)(uint_t,uint_t) const)&QuadSet::getNormalAt ,  return_value_policy<copy_const_reference>())
    .def( "colorAt",    (const Color4&  (QuadSet::*)(uint_t,uint_t) const)&QuadSet::getColorAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordAt", (const Vector2& (QuadSet::*)(uint_t,uint_t) const)&QuadSet::getTexCoordAt, return_value_policy<copy_const_reference>() )

    .def( "normalAt",   (const Vector3& (QuadSet::*)(uint_t) const)&QuadSet::getNormalAt,   return_value_policy<copy_const_reference>() )
    .def( "colorAt",    (const Color4&  (QuadSet::*)(uint_t) const)&QuadSet::getColorAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordAt", (const Vector2& (QuadSet::*)(uint_t) const)&QuadSet::getTexCoordAt, return_value_policy<copy_const_reference>() )

    .def( "indexAt",        (const Index4&  (QuadSet::*)(uint_t) const)&QuadSet::getIndexListAt,         return_value_policy<copy_const_reference>() )
    .def( "normalIndexAt",  (const Index4&  (QuadSet::*)(uint_t) const)&QuadSet::getNormalIndexListAt,   return_value_policy<copy_const_reference>() )
    .def( "colorIndexAt",   (const Index4&  (QuadSet::*)(uint_t) const)&QuadSet::getColorIndexListAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordIndexAt",(const Index4&  (QuadSet::*)(uint_t) const)&QuadSet::getTexCoordIndexListAt, return_value_policy<copy_const_reference>() )
	;
	
  implicitly_convertible<QuadSetPtr, MeshPtr>();
}


