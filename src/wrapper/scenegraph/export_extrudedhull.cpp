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

#include <plantgl/scenegraph/geometry/extrudedhull.h>
#include <plantgl/scenegraph/geometry/curve.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Hull)

void export_ExtrudedHull()
{
  class_< ExtrudedHull, ExtrudedHullPtr,  bases< Hull >,boost::noncopyable > 
    ("ExtrudedHull","A hull extruded by a vertical and an horizontal profiles.", init <Curve2DPtr, Curve2DPtr, optional< bool > >
	("ExtrudedHull(vertical,horizontal) : Constructs a ExtrudedHull with the profiles 'vertical' and 'horizontal'. ",
	(bp::arg("vertical"),bp::arg("horizontal"),bp::arg("ccw")=ExtrudedHull::DEFAULT_CCW)))
	.DEF_PGLBASE(ExtrudedHull)
	.DEC_PTR_PROPERTY(vertical,ExtrudedHull,Vertical,Curve2DPtr)
	.DEC_PTR_PROPERTY(horizontal,ExtrudedHull,Horizontal,Curve2DPtr)
	.DEC_BT_NR_PROPERTY_WDV(ccw,ExtrudedHull,CCW,bool,DEFAULT_CCW)
    ;
  implicitly_convertible<ExtrudedHullPtr,HullPtr >();

}

