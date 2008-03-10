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


#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/geometry/parametricmodel.h>
#include <plantgl/scenegraph/geometry/hull.h>
#include <plantgl/scenegraph/geometry/asymmetrichull.h>
#include <plantgl/scenegraph/geometry/extrudedhull.h>
#include <plantgl/scenegraph/geometry/curve.h>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(AsymmetricHull)
DEF_POINTEE(Hull)
DEF_POINTEE(ExtrudedHull)



void export_AsymmetricHull()
{
  class_< AsymmetricHull, AsymmetricHullPtr, bases< Hull >,boost::noncopyable >
    ("AsymmetricHull", init< optional < const real_t&, const real_t&, const real_t&,
     const real_t&, const real_t&, const real_t&,
     const real_t&, const real_t&, const Vector3&, const Vector3&, const real_t&, const real_t&,
     uchar_t, uchar_t > > 
     ( args("negXRadius","posXRadius","negYRadius","posYRadius",
	        "negXHeight","posXHeight","negYHeight","posYHeight",
	    "bottom","top", "bottomShape","topShape","slices","stacks"),
            "AsymmetricHull([negXRadius,posXRadius,negYRadius,posYRadius,"
	    "negXHeight,posXHeight,negYHeight,posYHeight,"
            "bottom, top, bottomShape, topShape, slices, stacks])") )
		.DEC_BT_PROPERTY_WDV(negXRadius,AsymmetricHull,NegXRadius,real_t,DEFAULT_NEG_X_RADIUS)
		.DEC_BT_PROPERTY_WDV(posXRadius,AsymmetricHull,PosXRadius,real_t,DEFAULT_POS_X_RADIUS)
		.DEC_BT_PROPERTY_WDV(negYRadius,AsymmetricHull,NegYRadius,real_t,DEFAULT_NEG_Y_RADIUS)
		.DEC_BT_PROPERTY_WDV(posYRadius,AsymmetricHull,PosYRadius,real_t,DEFAULT_POS_Y_RADIUS)
		.DEC_BT_PROPERTY_WDV(negXHeight,AsymmetricHull,NegXHeight,real_t,DEFAULT_NEG_X_HEIGHT)
		.DEC_BT_PROPERTY_WDV(posXHeight,AsymmetricHull,PosXHeight,real_t,DEFAULT_POS_X_HEIGHT)
		.DEC_BT_PROPERTY_WDV(negYHeight,AsymmetricHull,NegYHeight,real_t,DEFAULT_NEG_Y_HEIGHT)
		.DEC_BT_PROPERTY_WDV(posYHeight,AsymmetricHull,PosYHeight,real_t,DEFAULT_POS_Y_HEIGHT)
		.DEC_CT_PROPERTY_WDV(bottom,AsymmetricHull,Bottom,Vector3,DEFAULT_BOTTOM)
		.DEC_CT_PROPERTY_WDV(top, AsymmetricHull,Top,Vector3,DEFAULT_TOP)
		.DEC_BT_PROPERTY_WDV(bottomShape,AsymmetricHull,BottomShape,real_t,DEFAULT_BOTTOM_SHAPE)
		.DEC_BT_PROPERTY_WDV(topShape,AsymmetricHull,TopShape,real_t,DEFAULT_TOP_SHAPE)
		.DEC_BT_PROPERTY_WDV(slices,AsymmetricHull,Slices,uchar_t,DEFAULT_SLICES)
		.DEC_BT_PROPERTY_WDV(stacks,AsymmetricHull,Stacks,uchar_t,DEFAULT_STACKS)
	;

  implicitly_convertible<AsymmetricHullPtr,HullPtr >();
}

void export_ExtrudedHull()
{
  class_< ExtrudedHull, ExtrudedHullPtr,  bases< Hull >,boost::noncopyable > 
    ("ExtrudedHull",init <Curve2DPtr, Curve2DPtr, optional< bool > >
     ("ExtrudedHull(vertical,horizontal)" "Constructs a ExtrudedHull with the profiles horizontal and vertical. "))
    .def("copy",&ExtrudedHull::copy)
	.DEC_PTR_PROPERTY(horizontal,ExtrudedHull,Horizontal,Curve2DPtr)
	.DEC_PTR_PROPERTY(vertical,ExtrudedHull,Vertical,Curve2DPtr)
	.DEC_BT_NR_PROPERTY_WD(ccw,ExtrudedHull,CCW,bool)
    ;
  implicitly_convertible<ExtrudedHullPtr,HullPtr >();

}

void export_Hull()
{
  class_< Hull, HullPtr, bases< ParametricModel >,boost::noncopyable >("Hull",no_init);
  implicitly_convertible< HullPtr, ParametricModelPtr >();
}

