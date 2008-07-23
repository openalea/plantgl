/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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

#include <plantgl/algo/modelling/pglturtle.h>

#include <boost/python.hpp>
#include <plantgl/python/export_list.h>

using namespace boost::python;
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

boost::python::object getTurtleColorList(PglTurtle * turtle) {
    return make_list<std::vector<AppearancePtr> >(turtle->getColorList())();
}

boost::python::object getTurtleSurfaceList(PglTurtle * turtle) {
    return make_dict<PglTurtle::SurfaceMap>(turtle->getSurfaceList())();
}

void export_PglTurtle()
{
  class_< PglTurtle , bases < Turtle > >("PglTurtle", init< optional<TurtleParam *> >("PglTurtle([TurtleParam]) -> Create a Pgl Turtle"))
    .def("getScene",  &PglTurtle::getScene, return_value_policy<return_by_value>() )
    .def("clearColorList",    &PglTurtle::clearColorList )
    .def("clearSurfaceList",  &PglTurtle::clearSurfaceList )
    .def("defaultValue",      &PglTurtle::defaultValue )
    .def("removeColor",       &PglTurtle::removeColor )
    .def("setSurface",        &PglTurtle::setSurface )
    .def("removeSurface",     &PglTurtle::removeSurface )
	.def("appendMaterial",    &PglTurtle::appendMaterial )
	.def("setMaterial",       &PglTurtle::setMaterial )
	.def("getMaterial",       &PglTurtle::getMaterial )
    .def("appendColor",       (void(PglTurtle::*)( uint_t, uint_t, uint_t))
							  &PglTurtle::appendColor )
    .def("appendColor",       (void(PglTurtle::*)( float, float, float))
							  &PglTurtle::appendColor )
    .def("setColorAt",          (void(PglTurtle::*)( size_t, uint_t, uint_t, uint_t))
							  &PglTurtle::setColorAt )
    .def("setColorAt",          (void(PglTurtle::*)( size_t, float, float, float))
							  &PglTurtle::setColorAt )
    .def("setColorAt",          (void(PglTurtle::*)( size_t,const Color3&))
							  &PglTurtle::setColorAt )
    .def("appendColor",       (void(PglTurtle::*)( const Color3&))
							  &PglTurtle::appendColor )
    .def("getColorList",      &getTurtleColorList )
    .def("getSurfaceList",    &getTurtleSurfaceList )
    .def("customGeometry",    &PglTurtle::customGeometry )
	;
}
