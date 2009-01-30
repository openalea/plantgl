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


#include <plantgl/scenegraph/geometry/cylinder.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>

PGL_USING_NAMESPACE
using namespace boost::python;

#define bp boost::python

DEF_POINTEE(Cylinder)



void export_Cylinder()
{
  class_< Cylinder, CylinderPtr, bases< Cone > , boost::noncopyable >
    ("Cylinder", "A cylinder structure defined by a radius and a height.\n Its base is centered at origin.", 
	             init< optional<const real_t&,const real_t&,bool,uchar_t> >
                 ("Cylinder([radius, height, solid, slices])",
				 (bp::arg("radius")=Cylinder::DEFAULT_RADIUS,
				  bp::arg("height")=Cylinder::DEFAULT_HEIGHT,
				  bp::arg("solid")=Cylinder::DEFAULT_SOLID,
				  bp::arg("slices")=Cylinder::DEFAULT_SLICES)));

  implicitly_convertible<CylinderPtr, ConePtr >();
}

