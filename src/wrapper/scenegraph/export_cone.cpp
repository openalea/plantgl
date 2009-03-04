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


#include <plantgl/scenegraph/geometry/cone.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Cone)


void export_Cone()
{
  class_< Cone, ConePtr, bases< SOR > , boost::noncopyable >
    ("Cone", "A cone structure defined by a radius and a height. Its base is centered at origin.",
	init< optional<const real_t&,const real_t&, bool,uchar_t > >
               ("Cone(radius, height [, solid, slices])",
			   (bp::arg("radius")=Cone::DEFAULT_RADIUS,
			    bp::arg("height")=Cone::DEFAULT_HEIGHT,
			    bp::arg("solid") =Cone::DEFAULT_SOLID,
				bp::arg("slices")=Cone::DEFAULT_SLICES)
				)
	)
	.DEF_PGLBASE(Cone)
  .DEC_BT_PROPERTY_WDV(radius,Cone,Radius,real_t,DEFAULT_RADIUS)
  .DEC_BT_PROPERTY_WDV(height,Cone,Height,real_t,DEFAULT_HEIGHT)
  .DEC_BT_NR_PROPERTY_WDV(solid,Cone,Solid,bool,DEFAULT_SOLID);

  implicitly_convertible<ConePtr, SORPtr >();
}


