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

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/exception.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"


#include <plantgl/scenegraph/geometry/disc.h>

#include <boost/python.hpp>

using namespace boost::python;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DEF_POINTEE( Disc )

#define bp boost::python

void export_Disc()
{
  class_<Disc,DiscPtr, bases<SOR2D>, boost::noncopyable>( "Disc", 
	"A 2D disc structure centered on origin and defined by a radius.", 
	init< optional<real_t,uchar_t> >("Disc(radius, slices)",
	(bp::arg("radius")=Disc::DEFAULT_RADIUS, bp::arg("slices")=SOR::DEFAULT_SLICES)))
    .DEF_PGLBASE(Disc)
    .DEC_BT_PROPERTY_WDV(radius,Disc,Radius,real_t,DEFAULT_RADIUS)
    ;

  implicitly_convertible<DiscPtr,SOR2DPtr>();

}
