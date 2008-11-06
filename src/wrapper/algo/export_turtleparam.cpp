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

#include <plantgl/algo/modelling/turtleparam.h>

#include <boost/python.hpp>
using namespace boost::python;
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE


#define EXPORT_PROP(PROPNAME,_CLASS) \
add_property(#PROPNAME, make_getter(&_CLASS::PROPNAME), make_setter(&_CLASS::PROPNAME)) \

void export_TurtleParam()
{
  class_< TurtleParam >("TurtleParam", init<>("TurtleParam() -> Create default turtle parameters"))
    .def("isValid", &TurtleParam::isValid)
    .def("reset", &TurtleParam::reset)
    .def("dump", &TurtleParam::dump)
    .def("keepLastPoint", &TurtleParam::keepLastPoint)
    .def("removePoints", &TurtleParam::removePoints)
    .def("isPolygonOn", &TurtleParam::isPolygonOn)
    .def("isGeneralizedCylinderOn", &TurtleParam::isGeneralizedCylinderOn)
    .def("polygon", &TurtleParam::polygon)
    .def("generalizedCylinder", &TurtleParam::generalizedCylinder)
    .def("pushPosition", &TurtleParam::pushPosition)
    .def("transform", &TurtleParam::transform)
    .def("getOrientationMatrix", &TurtleParam::getOrientationMatrix)
    .def("getTransformationMatrix", &TurtleParam::getTransformationMatrix)
    .def("copy", &TurtleParam::copy, return_value_policy< manage_new_object >())
    .EXPORT_PROP(position,TurtleParam)
    .EXPORT_PROP(heading,TurtleParam)
    .EXPORT_PROP(up,TurtleParam)
    .EXPORT_PROP(left,TurtleParam)
    .EXPORT_PROP(scale,TurtleParam)
    .EXPORT_PROP(color,TurtleParam)
    .EXPORT_PROP(texture,TurtleParam)
    .EXPORT_PROP(width,TurtleParam)
    .EXPORT_PROP(crossSection,TurtleParam)
    ;
}

