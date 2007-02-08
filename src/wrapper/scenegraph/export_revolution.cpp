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

#include "revolution.h"
#include "swung.h"

#include <boost/python.hpp>

#include <scne_sceneobject.h>
#include <geom_geometry.h>

#include <util_vector.h>

#include <geom_parametricmodel.h>
#include <geom_sor.h>
#include <geom_sphere.h>
#include <geom_cone.h>
#include <geom_revolution.h>
#include <geom_curve.h>

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


SETGET(SOR,Slices,uchar_t);

void class_SOR()
{
  class_< SOR, SORPtr, bases< ParametricModel >,boost::noncopyable >("SOR",no_init)
  .DEC_SETGET_WD(slices,SOR,Slices,uchar_t);
  implicitly_convertible<SORPtr,ParametricModelPtr >();
}

SETGET(Sphere,Stacks,uchar_t);
SETGET(Sphere,Radius,real_t);

void class_Sphere()
{
  class_< Sphere, SpherePtr, bases< SOR > , boost::noncopyable >
    ("Sphere", init< optional<const real_t&, uchar_t, uchar_t > >
     (args("radius","slices", "stacks"),
      "Sphere(radius [,slices, stacks])"))
  .DEC_SETGET_WD(stacks,Sphere,Stacks,uchar_t)
  .DEC_SETGET_WD(radius,Sphere,Radius,real_t);

  implicitly_convertible<SpherePtr, SORPtr >();
}


SETGET(Cone,Radius,real_t);
SETGET(Cone,Height,real_t);
SETGET(Cone,Solid,bool);

void class_Cone()
{
  class_< Cone, ConePtr, bases< SOR > , boost::noncopyable >
    ("Cone", init< optional<const real_t&,const real_t&, bool,uchar_t > >
               (args("radius","height","solid","slices"),
		"Cone(radius, height [, solid, slices])"))
  .DEC_SETGET_WD(radius,Cone,Radius,real_t)
  .DEC_SETGET_WD(height,Cone,Height,real_t)
  .DEC_SETGET_WD(solid,Cone,Solid,bool);

  implicitly_convertible<ConePtr, SORPtr >();
}


void class_Revolution()
{
  class_SOR();
  class_Sphere();
  class_Cone();
  class_Cylinder();
  class_Frustum();
  class_Paraboloid();
  class_Swung();

  class_< Revolution, RevolutionPtr, bases< SOR > , boost::noncopyable >
    ("Revolution", init< Curve2DPtr, optional< uchar_t > >
               (args("profile","slices"),
		"Revolution(profile [, slices])"));

  implicitly_convertible< RevolutionPtr, SORPtr >();

}
