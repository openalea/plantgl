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

#include <plantgl/scenegraph/geometry/elevationgrid.h>
#include <plantgl/tool/util_array2.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

using namespace boost::python;
#define bp boost::python

PGL_USING_NAMESPACE

DEF_POINTEE(ElevationGrid)

real_t eg_getHeightAt(ElevationGrid *  e, uint_t i, uint_t j)
{ return e->getHeightAt(i,j); }


void export_ElevationGrid()
{
  class_< ElevationGrid, ElevationGridPtr, bases< Patch >,boost::noncopyable >
    ("ElevationGrid","A regular grid of elevations.",init<RealArray2Ptr, optional<real_t,real_t,bool> >
     ("ElevationGrid(heightList [,xspacing,yspacing,ccw])",
	 (bp::arg("heightList"),
	 bp::arg("xspacing")=ElevationGrid::DEFAULT_X_SPACING,
	  bp::arg("yspacing")=ElevationGrid::DEFAULT_Y_SPACING,
	  bp::arg("ccw")=ElevationGrid::DEFAULT_CCW)))
    .def( "getPointAt", &ElevationGrid::getPointAt )
    .def( "getHeightAt", &eg_getHeightAt )
    .def( "getXDim", &ElevationGrid::getXDim )
    .def( "getYDim", &ElevationGrid::getYDim )
    .def( "getXSize", &ElevationGrid::getXSize )
    .def( "getYSize", &ElevationGrid::getYSize )
    .DEC_BT_PROPERTY_WDV(xspacing,ElevationGrid,XSpacing,real_t,DEFAULT_X_SPACING)
    .DEC_BT_PROPERTY_WDV(yspacing,ElevationGrid,YSpacing,real_t,DEFAULT_Y_SPACING)
    .DEC_PTR_PROPERTY(heightList,ElevationGrid,HeightList,RealArray2Ptr)
	.DEF_PGLBASE(ElevationGrid)
    ;

  implicitly_convertible< ElevationGridPtr,PatchPtr >();
}
