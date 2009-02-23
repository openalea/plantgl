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

#include <plantgl/scenegraph/core/sceneobject.h>
#include <plantgl/scenegraph/core/action.h>

#include <plantgl/python/export_refcountptr.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(SceneObject)

std::string get_sco_name(SceneObject * obj){ return obj->getName(); } 
void set_sco_name(SceneObject * obj, std::string v){ obj->setName(v); } 



void export_SceneObject()
{
  class_< SceneObject,SceneObjectPtr, boost::noncopyable >(
	  "SceneObject", 
	  "Abstract base class for all objects of the scenegraph.\n"
	  "It is named, has unique id and support reference counting.\n"
	  "It can support Action application.",
	  no_init)
 	// .def("__del__",&pydel<SceneObject>)
    .def("getName", &SceneObject::getName, return_value_policy< copy_const_reference >())
    .def("isNamed", &SceneObject::isNamed)
    .def("setName", &SceneObject::setName)
    .add_property("name",get_sco_name,&SceneObject::setName)
    .def("isValid", &SceneObject::isValid)
    .def("apply", &SceneObject::apply)
    .def("deepcopy", &SceneObject::copy)
    .def("getId", &SceneObject::getId)
	.def("getPglReferenceCount",&RefCountObject::getReferenceCount)
	// .enable_pickling()
    ;
}

