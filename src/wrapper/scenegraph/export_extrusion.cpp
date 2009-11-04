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

#include <plantgl/scenegraph/geometry/extrusion.h>
#include <plantgl/scenegraph/geometry/lineicmodel.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/geometry/profile.h>
#include <plantgl/scenegraph/geometry/curve.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;
#define bp boost::python

DEF_POINTEE( Extrusion )

void export_Extrusion()
{
  class_<Extrusion, ExtrusionPtr, bases<ParametricModel>, boost::noncopyable>
	( "Extrusion", "An Extrusion represented by an axis, a cross section and a tranformation function.", 
	  init<LineicModelPtr,Curve2DPtr,optional<Point2ArrayPtr, RealArrayPtr, RealArrayPtr,bool,bool,TOOLS(Vector3)> >
	 ( "Extrusion (LineicModel axis, Curve2D crossSection,[Point2Array scale, RealArray orientation, RealArray knotList, bool solid, bool ccw, Vector3 initialVector])",
	 (bp::arg("axis"),bp::arg("crossSection"),
	  bp::arg("scaleList")=Extrusion::DEFAULT_SCALE_LIST,
	  bp::arg("orientationList")=Extrusion::DEFAULT_ORIENTATION_LIST,
	  bp::arg("knotList")=RealArrayPtr(),
	  bp::arg("solid")=Extrusion::DEFAULT_SOLID,
	  bp::arg("ccw")=Extrusion::DEFAULT_CCW,
	  bp::arg("initialNormal")= Extrusion::DEFAULT_INITIAL_NORMAL)))
	.DEF_PGLBASE(Extrusion)
	.DEC_PTR_PROPERTY(axis,Extrusion,Axis,LineicModelPtr)
	.DEC_PTR_PROPERTY(crossSection,Extrusion,CrossSection,Curve2DPtr)
	.DEC_BT_NR_PROPERTY_WDV(solid,Extrusion,Solid,bool,DEFAULT_SOLID)
	.DEC_BT_NR_PROPERTY_WDV(ccw, Extrusion,CCW,bool,DEFAULT_CCW)
	.DEC_BT_PROPERTY_WDV(initialNormal, Extrusion,InitialNormal,Vector3,DEFAULT_INITIAL_NORMAL)

	.DEC_PTR_PROPERTY_WDV(scaleList,Extrusion,Scale,Point2ArrayPtr,DEFAULT_SCALE_LIST)
	.DEC_PTR_PROPERTY_WDV(orientationList,Extrusion,Orientation,RealArrayPtr,DEFAULT_ORIENTATION_LIST)
	.DEC_PTR_NR_PROPERTY(knotList,Extrusion,KnotList,RealArrayPtr)
	.def("getInitialNormalValue",&Extrusion::getInitialNormalValue);
    ;
  implicitly_convertible<ExtrusionPtr, ParametricModelPtr>();
}


