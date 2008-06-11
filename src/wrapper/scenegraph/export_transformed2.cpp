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

#include <plantgl/scenegraph/transformation/eulerrotated.h>
#include <plantgl/scenegraph/transformation/axisrotated.h>
#include <plantgl/scenegraph/transformation/oriented.h>
#include <plantgl/scenegraph/transformation/tapered.h>
#include <plantgl/scenegraph/geometry/primitive.h>

#include <plantgl/math/util_vector.h>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;



DEF_POINTEE(Transformed)
DEF_POINTEE(EulerRotated)
DEF_POINTEE(AxisRotated)
DEF_POINTEE(Oriented)


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
       (args("azimuth","elevation","roll","geometry"),
	"EulerRotated(azimuth,elevation,roll,geometry)") )
	.DEC_ANGLE_PROPERTY_WDV(azimuth,EulerRotated,Azimuth,DEFAULT_AZIMUTH)
	.DEC_ANGLE_PROPERTY_WDV(elevation,EulerRotated,Elevation,DEFAULT_ELEVATION)
	.DEC_ANGLE_PROPERTY_WDV(roll,EulerRotated,Roll,DEFAULT_ROLL)
    ;

  implicitly_convertible< EulerRotatedPtr, OrthoTransformedPtr >();
}

void export_AxisRotated()
{
  class_< AxisRotated, AxisRotatedPtr, bases< OrthoTransformed > , boost::noncopyable >
    ("AxisRotated", 
	 "The AxisRotated describes an object to which it has been applied a rotation of a specified angle about a specified axis.\n"
	 "The rotation is given by the matrix: |(1-c)x2 + c    (1-c)xy - sz (1-c)xy + sy|\n"
	 "                                     |(1-c)xy + sz   (1-c)y2 + c  (1-c)yz - sx|\n"
     "                                     |(1-c)xz - sy   (1-c)yz + sx (1-c)z2 + c |\n"
     "where s = sin(angle), c = cos(angle), x,y and z the coordinates of the axis.",
	 init< const Vector3&, const real_t&, const GeometryPtr& >
     (args("axis","angle","geometry"),
	"AxisRotated( Vector3 axis, radian angle, geometry)") )
    .DEC_CT_PROPERTY_WDV(axis,AxisRotated,Axis,Vector3,DEFAULT_AXIS)
	.DEC_ANGLE_NR_PROPERTY_WDV(angle,AxisRotated,Angle,DEFAULT_ANGLE)
    ;

  implicitly_convertible< AxisRotatedPtr, OrthoTransformedPtr >();
}

void export_Oriented()
{
  class_< Oriented, OrientedPtr, bases< OrthoTransformed > , boost::noncopyable >
    ("Oriented", init< const Vector3&,const Vector3&,const GeometryPtr& >
       (args("primary","secondary","geometry"),
	"Oriented(primary,secondary,geometry) with primary,secondary: Vector3" ) )
	.DEC_CT_PROPERTY_WDV(primary,Oriented,Primary,Vector3,DEFAULT_PRIMARY)
	.DEC_CT_PROPERTY_WDV(secondary,Oriented,Secondary,Vector3,DEFAULT_SECONDARY)
    ;

  implicitly_convertible< OrientedPtr, OrthoTransformedPtr >();
}


DEF_POINTEE(Tapered)

void export_Tapered()
{
  class_< Tapered, TaperedPtr, bases< Transformed > , boost::noncopyable >
    ("Tapered", init< real_t,real_t,const PrimitivePtr & >
       (args("base","top","primitive"),
	"Tapered(base, top, primitive)") )
	.DEC_BT_PROPERTY_WDV(topRadius,Tapered,TopRadius,real_t,DEFAULT_TOP_RADIUS)
	.DEC_BT_PROPERTY_WDV(baseRadius,Tapered,BaseRadius,real_t,DEFAULT_BASE_RADIUS)
	.DEC_PTR_PROPERTY(primitive,Deformed,Primitive,PrimitivePtr)
    ;

  implicitly_convertible< TaperedPtr, TransformedPtr >();
}



