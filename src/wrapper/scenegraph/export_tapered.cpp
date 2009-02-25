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

#include <plantgl/scenegraph/transformation/tapered.h>
#include <plantgl/scenegraph/geometry/primitive.h>


#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <boost/python.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(Tapered)

void export_Tapered()
{
  class_< Tapered, TaperedPtr, bases< Transformed > , boost::noncopyable >
    ("Tapered",
	 "Tapered describes an object to which it has been applied a Taper deformation.\n" 
	 "A Taper deforms an object in order to be able to bound the object within a cone frustum of a specified base radius and top radius.\n"
     "For each point composing an object, a Taper scale the polar coordinates according the zcoordinate.\n"
     "The amplitude of the scale is given by the radii.\n",
	 init< real_t,real_t,const PrimitivePtr & >
       ("Tapered(baseRadius, topRadius, primitive)",
	   (bp::arg("baseRadius") = Tapered::DEFAULT_BASE_RADIUS,
	    bp::arg("topRadius")  = Tapered::DEFAULT_TOP_RADIUS,
		bp::arg("primitive")  = PrimitivePtr()) ))
	.DEC_BT_PROPERTY_WDV(topRadius,Tapered,TopRadius,real_t,DEFAULT_TOP_RADIUS)
	.DEC_BT_PROPERTY_WDV(baseRadius,Tapered,BaseRadius,real_t,DEFAULT_BASE_RADIUS)
	.DEC_PTR_PROPERTY(primitive,Deformed,Primitive,PrimitivePtr)
    ;

  implicitly_convertible< TaperedPtr, TransformedPtr >();
}



