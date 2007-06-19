/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): C. Pradal (christophe.pradal@cirad.fr)
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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


SETGET_ANGLE(EulerRotated,Azimuth)
SETGET_ANGLE(EulerRotated,Elevation)
SETGET_ANGLE(EulerRotated,Roll)

void export_EulerRotated()
{
  class_< EulerRotated, EulerRotatedPtr, bases< OrthoTransformed > , boost::noncopyable >
    ("EulerRotated", init< const real_t&,const real_t&,const real_t&, const GeometryPtr& >
       (args("azimuth","elevation","roll","geometry"),
	"EulerRotated(azimuth,elevation,roll,geometry)") )
	.DEC_SETGET_WD(azimuth,EulerRotated,Azimuth,real_t)
	.DEC_SETGET_WD(elevation,EulerRotated,Elevation,real_t)
	.DEC_SETGET_WD(roll,EulerRotated,Roll,real_t)
    ;

  implicitly_convertible< EulerRotatedPtr, OrthoTransformedPtr >();
}

SETGET(AxisRotated,Axis,Vector3)
SETGET_ANGLE(AxisRotated,Angle)

void export_AxisRotated()
{
  class_< AxisRotated, AxisRotatedPtr, bases< OrthoTransformed > , boost::noncopyable >
    ("AxisRotated", init< const Vector3&, const real_t&, const GeometryPtr& >
     (args("axis","angle","geometry"),
	"AxisRotated( Vector3 axis, radian angle, geometry)") )
    .DEC_SETGET_WD(axis,AxisRotated,Axis,Vector3)
	.DEC_SETGET_WD(angle,AxisRotated,Angle,real_t)
    ;

  implicitly_convertible< AxisRotatedPtr, OrthoTransformedPtr >();
}

SETGET(Oriented,Primary,Vector3)
SETGET(Oriented,Secondary,Vector3)

void export_Oriented()
{
  class_< Oriented, OrientedPtr, bases< OrthoTransformed > , boost::noncopyable >
    ("Oriented", init< const Vector3&,const Vector3&,const GeometryPtr& >
       (args("primary","secondary","geometry"),
	"Oriented(primary,secondary,geometry) with primary,secondary: Vector3" ) )
	.DEC_SETGET_WD(primary,Oriented,Primary,Vector3)
	.DEC_SETGET_WD(secondary,Oriented,Secondary,Vector3)
    ;

  implicitly_convertible< OrientedPtr, OrthoTransformedPtr >();
}


DEF_POINTEE(Tapered)

SETGET(Tapered,TopRadius,real_t)
SETGET(Tapered,BaseRadius,real_t)
SETGET(Tapered,Primitive,PrimitivePtr)

void export_Tapered()
{
  class_< Tapered, TaperedPtr, bases< Transformed > , boost::noncopyable >
    ("Tapered", init< real_t,real_t,const PrimitivePtr & >
       (args("base","top","primitive"),
	"Tapered(base, top, primitive)") )
	.DEC_SETGET_WD(topRadius,Tapered,TopRadius,real_t)
	.DEC_SETGET_WD(baseRadius,Tapered,BaseRadius,real_t)
	.DEC_SETGET(primitive,Tapered,Primitive,PrimitivePtr)
    ;

  implicitly_convertible< TaperedPtr, TransformedPtr >();
}



