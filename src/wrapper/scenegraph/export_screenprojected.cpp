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


#include <plantgl/scenegraph/transformation/screenprojected.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <boost/python/make_constructor.hpp>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(ScreenProjected)

void setKeepAspectRatio(ScreenProjected * obj, bool val) { obj->getKeepAspectRatio() = val; }

void export_ScreenProjected()
{
  class_< ScreenProjected, ScreenProjectedPtr, bases< Transformed > , boost::noncopyable >
    ("ScreenProjected", 
	"The ScreenProjected describes an object that is defined in the camera coordinates system.\n",
	init< const GeometryPtr&, bool >
	("ScreenProjected(geometry,keepAspectRatio=True)",
	(bp::arg("geometry") = GeometryPtr(),bp::arg("keepAspectRatio") = true)) )
    .DEF_PGLBASE(ScreenProjected)
	.add_property("keepAspectRatio",(bool(ScreenProjected::*)()const)&ScreenProjected::getKeepAspectRatio, 
                           &setKeepAspectRatio) 

	// .DEC_CT_PROPERTY_WDV(keepAspectRatio,ScreenProjected,KeepAspectRatio,bool,DEFAULT_KEEPASPECTRATIO)
    ;

  implicitly_convertible< ScreenProjectedPtr, TransformedPtr >();

}

