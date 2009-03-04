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


#include <plantgl/scenegraph/geometry/paraboloid.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(Paraboloid)


void export_Paraboloid()
{
  class_< Paraboloid, ParaboloidPtr, bases< Cone > , boost::noncopyable >
    ("Paraboloid", "A paraboloid structure defined by a base radius, a height and a shape factor.\n Its base is centered at origin.",
     init< optional< const real_t&, const real_t&, const real_t&, 
     bool, uchar_t, uchar_t> >
     ("Paraboloid([radius, height, shape , solid, slices, stacks])",
	  (bp::arg("radius")=Paraboloid::DEFAULT_RADIUS,
	   bp::arg("height")=Paraboloid::DEFAULT_HEIGHT,
	   bp::arg("shape")=Paraboloid::DEFAULT_SHAPE,
	   bp::arg("solid")=Paraboloid::DEFAULT_SOLID,
	   bp::arg("slices")=Paraboloid::DEFAULT_SLICES,
	   bp::arg("stacks")=Paraboloid::DEFAULT_STACKS)))
  .DEF_PGLBASE(Paraboloid)
  .DEC_BT_PROPERTY_WDV(stacks,Paraboloid,Stacks,uchar_t,DEFAULT_STACKS)
  .DEC_BT_PROPERTY_WDV(shape,Paraboloid,Shape,real_t,DEFAULT_SHAPE)
    ;

  implicitly_convertible<ParaboloidPtr, ConePtr >();

}
