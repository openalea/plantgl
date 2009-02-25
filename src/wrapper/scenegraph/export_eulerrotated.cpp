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

#include <plantgl/scenegraph/transformation/eulerrotated.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(EulerRotated)


void export_EulerRotated()
{
  class_< EulerRotated, EulerRotatedPtr, bases< OrthoTransformed > , boost::noncopyable >
    ("EulerRotated", 
	"The EulerRotated describes an object to which it has been applied a composition of 3 rotations.\n"
	"Rotations angles are the roll around the x-axis, the elevation around the rotated y-axis\n"
	"and azimuth around the rotated z -axis. The equivalent rotation is given by the matrix:\n"
	" | ca*ce   ca*se*sr - sa*cr    ca*se*cr + sa*sr |\n"
	" | sa*ce   ca*cr + sa*se*sr    sa*se*cr - ca*sr |\n"
	" | -se     ce*sr               ce*cr            |\n"
	"where cr = cos(roll), sr = sin(roll), ce = cos(elevation), se = sin(elevation), ca = cos(azimuth) and sa = sin(azimuth).\n",
	init< const real_t&,const real_t&,const real_t&, const GeometryPtr& >
       ( "EulerRotated(azimuth,elevation,roll,geometry)",
	   (bp::arg("azimuth")=EulerRotated::DEFAULT_AZIMUTH,
		 bp::arg("elevation")=EulerRotated::DEFAULT_ELEVATION,
		 bp::arg("roll")=EulerRotated::DEFAULT_ROLL,
		 bp::arg("geometry")=GeometryPtr()) ))
	.DEC_ANGLE_PROPERTY_WDV(azimuth,EulerRotated,Azimuth,DEFAULT_AZIMUTH)
	.DEC_ANGLE_PROPERTY_WDV(elevation,EulerRotated,Elevation,DEFAULT_ELEVATION)
	.DEC_ANGLE_PROPERTY_WDV(roll,EulerRotated,Roll,DEFAULT_ROLL)
    ;

  implicitly_convertible< EulerRotatedPtr, OrthoTransformedPtr >();
}

