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

#include <plantgl/scenegraph/transformation/axisrotated.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python


DEF_POINTEE(AxisRotated)

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
     ("AxisRotated( Vector3 axis, radian angle, geometry)",
	 (bp::arg("axis") = AxisRotated::DEFAULT_AXIS,
	  bp::arg("angle") = AxisRotated::DEFAULT_ANGLE,
	  bp::arg("geometry") = GeometryPtr())))
    .DEC_CT_PROPERTY_WDV(axis,AxisRotated,Axis,Vector3,DEFAULT_AXIS)
	.DEC_ANGLE_NR_PROPERTY_WDV(angle,AxisRotated,Angle,DEFAULT_ANGLE)
    ;

  implicitly_convertible< AxisRotatedPtr, OrthoTransformedPtr >();
}



