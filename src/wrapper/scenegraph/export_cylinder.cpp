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

#include <plantgl/scenegraph/geometry/geometry.h>

#include <plantgl/math/util_vector.h>

#include <plantgl/scenegraph/geometry/parametricmodel.h>
#include <plantgl/scenegraph/geometry/sor.h>
#include <plantgl/scenegraph/geometry/cylinder.h>
#include <plantgl/scenegraph/geometry/frustum.h>
#include <plantgl/scenegraph/geometry/paraboloid.h>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Cylinder)
DEF_POINTEE(Frustum)
DEF_POINTEE(Paraboloid)



void export_Cylinder()
{
  class_< Cylinder, CylinderPtr, bases< Cone > , boost::noncopyable >
    ("Cylinder", init< optional<const real_t&,const real_t&,bool,uchar_t> >
                   (args("radius","height","solid","slices"),
		    "Cylinder(radius, height [, solid, slices])" ));

  implicitly_convertible<CylinderPtr, ConePtr >();
}

void export_Frustum()
{
  class_< Frustum, FrustumPtr, bases< Cone > , boost::noncopyable >
    ("Frustum", init< optional<const real_t&,const real_t&,const real_t&, bool,uchar_t> >
                   (args("radius","height","taper","solid","slices"),
		    "Frustum(radius, height, taper [, solid, slices])" ))
  .DEC_BT_PROPERTY_WD(taper,Frustum,Taper,real_t)
  .add_static_property( "DEFAULT_TAPER", make_getter(Frustum::DEFAULT_TAPER))
      ;

  implicitly_convertible<FrustumPtr, ConePtr >();
}

void export_Paraboloid()
{
  class_< Paraboloid, ParaboloidPtr, bases< Cone > , boost::noncopyable >
    ("Paraboloid", 
     init< optional< const real_t&, const real_t&, const real_t&, 
     bool, uchar_t, uchar_t> >
     (args("radius","height","shape","solid","slices","stacks"),
      "Paraboloid([radius, height, shape , solid, slices, stacks])" ))
  .DEC_BT_PROPERTY_WD(stacks,Paraboloid,Stacks,uchar_t)
  .DEC_BT_PROPERTY_WD(shape,Paraboloid,Shape,real_t)
  .add_static_property( "DEFAULT_SHAPE", make_getter(Paraboloid::DEFAULT_SHAPE))
  .add_static_property( "DEFAULT_STACKS", make_getter(Paraboloid::DEFAULT_STACKS))
    ;

  implicitly_convertible<ParaboloidPtr, ConePtr >();

}
