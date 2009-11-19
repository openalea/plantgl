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

#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/appearance/appearance.h>
#include <string>

#include <plantgl/python/export_property.h>
#include <plantgl/python/export_refcountptr.h>
#include <boost/python/make_constructor.hpp>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Shape)
DEF_POINTEE(Shape3D)

void export_Shape3D()
{
  class_< Shape3D, Shape3DPtr, bases< SceneObject >, boost::noncopyable >("Shape3D", 
	  "Abstract base class for shape that can be stored into a scene.",no_init);

  implicitly_convertible<Shape3DPtr, SceneObjectPtr >();
}

struct sh_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Shape const& sh)
	{
		return boost::python::make_tuple<GeometryPtr,AppearancePtr,uint_t,uint_t>
					(sh.getGeometry(),sh.getAppearance(),sh.getId(),sh.getParentId());
	}
};

uint_t sh_getptrid(const Shape * sh) { return sh->SceneObject::getId(); }

void export_Shape()
{
  class_< Shape, ShapePtr, bases< Shape3D > , boost::noncopyable >("Shape",
	  "A Shape is composed of a Geometry object and an Appearance object.\n"
      "There is an optional id to identy the shape and parent id to store shape relationship.",
	  init< optional<const GeometryPtr&, const AppearancePtr &, uint_t, uint_t > >
	        ("Shape( geometry, appearance, id, parentId )",
				(bp::arg("geometry")   = GeometryPtr(),
				 bp::arg("appearance") = Material::DEFAULT_MATERIAL, 
				 bp::arg("id")         = Shape::NOID,
				 bp::arg("parentId")   = Shape::NOID )))
   .add_static_property("NOID",make_getter(&Shape::NOID))
    .DEF_PGLBASE(Shape)
    .DEC_PTR_PROPERTY(appearance, Shape,Appearance, AppearancePtr)
    .DEC_PTR_PROPERTY(geometry, Shape, Geometry,GeometryPtr)
    //.def_readwrite("id", &Shape::id)
    //.def_readwrite("parentId", &Shape::parentId)
    .def("getSceneObjectId", &sh_getptrid)
    .def("setComputedName", &Shape::setComputedName)
	.def_pickle(sh_pickle_suite());
    ;

	/*
	boost::python::converter::intrusive_ptr_from_python<Shape>();
    register_ptr_to_python< intrusive_ptr<Shape> >();
	*/


  implicitly_convertible<ShapePtr, Shape3DPtr >();
}
