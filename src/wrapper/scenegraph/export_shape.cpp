/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): C. Pradal (christophe.pradal@cirad.fr)
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

#include <boost/python.hpp>

#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/appearance/appearance.h>
#include <string>

#include "../util/export_refcountptr.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Shape)
DEF_POINTEE(Shape3D)

void export_Shape3D()
{
  class_< Shape3D, Shape3DPtr, bases< SceneObject >, boost::noncopyable >("Shape3D", no_init);
}

void gs_setGeometry(Shape* shape, GeometryPtr geom )
{ shape->getGeometry()= geom; }

GeometryPtr gs_getGeometry(Shape* shape)
{ return shape->getGeometry(); }

void gs_setAppearance(Shape* shape, AppearancePtr app )
{ shape->getAppearance()= app; }

AppearancePtr gs_getAppearance(Shape* shape)
{ return shape->getAppearance(); }


struct sh_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Shape const& sh)
	{
		return make_tuple(sh.getGeometry(),sh.getAppearance(),sh.getId());
	}
};

void export_Shape()
{
  class_< Shape,ShapePtr, bases< Shape3D > , boost::noncopyable >("Shape", init<>())
    .def( init< const RefCountPtr<Geometry> &, 
	          optional< const RefCountPtr<Appearance> &,
	                     long unsigned int > >("Shape( geometry, appearance, id )") )
    .add_property("appearance", gs_getAppearance, gs_setAppearance)
    .add_property("geometry", gs_getGeometry, gs_setGeometry)
    .def_readwrite("id", &Shape::id)
    .def("setComputedName", &Shape::setComputedName)
	.def_pickle(sh_pickle_suite());
    ;

  implicitly_convertible<ShapePtr, Shape3DPtr >();
  
}
