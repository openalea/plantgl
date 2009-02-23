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

#include <plantgl/scenegraph/geometry/asymmetrichull.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(AsymmetricHull)

void export_AsymmetricHull()
{
  class_< AsymmetricHull, AsymmetricHullPtr, bases< Hull >,boost::noncopyable >
    ("AsymmetricHull",
	"An Asymmetric Hull describes an anvelop defined by 6 morphological points.\n"
	"This is an implementation of the asymmetric crowns introduced by [Koop,89] and [Cescatti,97]."
	"The two first morphological points are the bottom and top points of the hull."
	"The four other points are used to defined the peripheral line of the hull (P1,P2,P3,P4)."
	"The two first points are located along the x -axis (P1,P2) and the two other along the y-axis (P3,P4)."
	"As such Pi points are described with only two dimensions, i.e. a radius and a height using corresponding parameters"
	"Finally, the shape coefficients are versatile indices which describe the curvature of the hull above and below the peripheral line.",
	 init< optional < const real_t&, const real_t&, const real_t&,
     const real_t&, const real_t&, const real_t&,
     const real_t&, const real_t&, const Vector3&, const Vector3&, const real_t&, const real_t&,
     uchar_t, uchar_t > > 
     ( "AsymmetricHull([negXRadius,posXRadius,negYRadius,posYRadius,"
	        "negXHeight,posXHeight,negYHeight,posYHeight,"
            "bottom, top, bottomShape, topShape, slices, stacks])",
			(bp::arg("negXRadius")=AsymmetricHull::DEFAULT_NEG_X_RADIUS,
			 bp::arg("posXRadius")=AsymmetricHull::DEFAULT_POS_X_RADIUS,
			 bp::arg("negYRadius")=AsymmetricHull::DEFAULT_NEG_Y_RADIUS,
			 bp::arg("posYRadius")=AsymmetricHull::DEFAULT_POS_Y_RADIUS,
 	         bp::arg("negXHeight")=AsymmetricHull::DEFAULT_NEG_X_HEIGHT,
			 bp::arg("posXHeight")=AsymmetricHull::DEFAULT_POS_X_HEIGHT,
			 bp::arg("negYHeight")=AsymmetricHull::DEFAULT_NEG_Y_HEIGHT,
			 bp::arg("posYHeight")=AsymmetricHull::DEFAULT_POS_Y_HEIGHT,
			 bp::arg("bottom")=AsymmetricHull::DEFAULT_BOTTOM,
			 bp::arg("top")=AsymmetricHull::DEFAULT_TOP, 
			 bp::arg("bottomShape")=AsymmetricHull::DEFAULT_BOTTOM_SHAPE,
			 bp::arg("topShape")=AsymmetricHull::DEFAULT_TOP_SHAPE,
			 bp::arg("slices")=AsymmetricHull::DEFAULT_SLICES,
			 bp::arg("stacks")=AsymmetricHull::DEFAULT_STACKS )) )
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



