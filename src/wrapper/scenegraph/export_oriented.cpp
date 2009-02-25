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

#include <plantgl/scenegraph/transformation/oriented.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python


DEF_POINTEE(Oriented)

void export_Oriented()
{
  class_< Oriented, OrientedPtr, bases< OrthoTransformed > , boost::noncopyable >
    ("Oriented", 
	 "Oriented describes an object to a change of coordinate specified by an orthonormal basis has been applied.\n"
	 "The basis is expressed by the matrix:\n"
	 "|px sx tx|\n"
	 "|py sy ty|\n"
	 "|pz sz tz|\n"
	 "where (px, py, pz) denotes the primary direction, (sx,, sy, sz) the secondary direction and (tx,, ty, tz) the ternary direction, which is given by: t = p ^ s.",
	 init< const Vector3&,const Vector3&,const GeometryPtr& >
       ("Oriented(primary,secondary,geometry) with primary,secondary: Vector3" ,
	   (bp::arg("primary")   = Oriented::DEFAULT_PRIMARY,
	    bp::arg("secondary") = Oriented::DEFAULT_SECONDARY,
		bp::arg("geometry")  = GeometryPtr())   ) )
	.DEC_CT_PROPERTY_WDV(primary,Oriented,Primary,Vector3,DEFAULT_PRIMARY)
	.DEC_CT_PROPERTY_WDV(secondary,Oriented,Secondary,Vector3,DEFAULT_SECONDARY)
    ;

  implicitly_convertible< OrientedPtr, OrthoTransformedPtr >();
}


