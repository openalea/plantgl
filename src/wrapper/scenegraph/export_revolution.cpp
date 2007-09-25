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
#include <plantgl/scenegraph/geometry/sphere.h>
#include <plantgl/scenegraph/geometry/cone.h>
#include <plantgl/scenegraph/geometry/revolution.h>
#include <plantgl/scenegraph/geometry/curve.h>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(SOR)
DEF_POINTEE(Sphere)
DEF_POINTEE(Cone)
DEF_POINTEE(Revolution)


void export_SOR()
{
  class_< SOR, SORPtr, bases< ParametricModel >,boost::noncopyable >("SOR",no_init)
  .DEC_BT_PROPERTY_WD(slices,SOR,Slices,uchar_t);
  implicitly_convertible<SORPtr,ParametricModelPtr >();
}

void export_Sphere()
{
  class_< Sphere, SpherePtr, bases< SOR > , boost::noncopyable >
    ("Sphere", init< optional<const real_t&, uchar_t, uchar_t > >
     (args("radius","slices", "stacks"),
      "Sphere(radius [,slices, stacks])"))
  .DEC_BT_PROPERTY_WD(stacks,Sphere,Stacks,uchar_t)
  .DEC_BT_PROPERTY_WD(radius,Sphere,Radius,real_t);

  implicitly_convertible<SpherePtr, SORPtr >();
}


void export_Cone()
{
  class_< Cone, ConePtr, bases< SOR > , boost::noncopyable >
    ("Cone", init< optional<const real_t&,const real_t&, bool,uchar_t > >
               (args("radius","height","solid","slices"),
		"Cone(radius, height [, solid, slices])"))
  .DEC_BT_PROPERTY_WD(radius,Cone,Radius,real_t)
  .DEC_BT_PROPERTY_WD(height,Cone,Height,real_t)
  .DEC_BT_NR_PROPERTY_WD(solid,Cone,Solid,bool);

  implicitly_convertible<ConePtr, SORPtr >();
}


void export_Revolution()
{
  class_< Revolution, RevolutionPtr, bases< SOR > , boost::noncopyable >
    ("Revolution", init< Curve2DPtr, optional< uchar_t > >
               (args("profile","slices"),
		"Revolution(profile [, slices])"))
    .DEC_PTR_PROPERTY(profile, Revolution, Profile, Curve2DPtr)
    ;


  implicitly_convertible< RevolutionPtr, SORPtr >();

}
