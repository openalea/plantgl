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


#include <plantgl/scenegraph/geometry/sphere.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <boost/python.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Sphere)

void export_Sphere()
{
  class_< Sphere, SpherePtr, bases< SOR > , boost::noncopyable >
    ("Sphere", init< optional<const real_t&, uchar_t, uchar_t > >
	("Sphere(radius [,slices, stacks])",
	(bp::arg("radius")=Sphere::DEFAULT_RADIUS,
	 bp::arg("slices")=Sphere::DEFAULT_SLICES, 
	 bp::arg("stacks")=Sphere::DEFAULT_STACKS)))      
  .DEC_BT_PROPERTY_WDV(stacks,Sphere,Stacks,uchar_t,DEFAULT_STACKS)
  .DEC_BT_PROPERTY_WDV(radius,Sphere,Radius,real_t,DEFAULT_RADIUS);

  implicitly_convertible<SpherePtr, SORPtr >();
}


