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


#include <plantgl/scenegraph/transformation/scaled.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(Scaled)


ScaledPtr sca_from_val(real_t x, real_t y, real_t z, const GeometryPtr& geom)
{
    return ScaledPtr(new Scaled(Vector3(x,y,z),geom));
}

ScaledPtr sca1_from_val(real_t x, const GeometryPtr& geom)
{
    return ScaledPtr(new Scaled(Vector3(x,x,x),geom));
}

void export_Scaled()
{
  class_< Scaled, ScaledPtr, bases< MatrixTransformed > , boost::noncopyable >
    ("Scaled", 
	"The Scaled describes an object to which an anisotropic scaling has been applied.\n"
	"The scaling transformation is given by the matrix:\n"
	"|sx 0 0|\n"
	"|0 sy 0|\n"
	"|0 0 sz|\n"
	"where (sx, sy, sz) denotes the scaling factors along the x, y and z-axis.",
	init< const Vector3&, const GeometryPtr& >
	("Scaled(scale,geometry)",
	(bp::arg("scale") = Scaled::DEFAULT_SCALE,
	 bp::arg("geometry") = GeometryPtr())) )
    .def( "__init__", make_constructor( sca_from_val ) ) 
    .def( "__init__", make_constructor( sca1_from_val ) ) 
	.DEC_CT_PROPERTY_WDV(scale,Scaled,Scale,Vector3,DEFAULT_SCALE)
    ;

  implicitly_convertible< ScaledPtr, MatrixTransformedPtr >();

}

