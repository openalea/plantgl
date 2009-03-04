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


#include <plantgl/scenegraph/geometry/frustum.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(Frustum)

void export_Frustum()
{
  class_< Frustum, FrustumPtr, bases< Cone > , boost::noncopyable >
    ("Frustum", "A frustum structure defined by a base radius, a height and a taper factor.\n Its base is centered at origin.", 
	   init< optional<const real_t&,const real_t&,const real_t&, bool,uchar_t> >
		    ("Frustum([radius, height, taper, solid, slices])",
			(bp::arg("radius")=Frustum::DEFAULT_RADIUS,
			bp::arg("height")=Frustum::DEFAULT_HEIGHT,
			bp::arg("taper")=Frustum::DEFAULT_TAPER,
			bp::arg("solid")=Frustum::DEFAULT_SOLID,
			bp::arg("slices")=Frustum::DEFAULT_SLICES)))
   .DEF_PGLBASE(Frustum)
   .DEC_BT_PROPERTY_WDV(taper,Frustum,Taper,real_t,DEFAULT_TAPER)
      ;

  implicitly_convertible<FrustumPtr, ConePtr >();
}
