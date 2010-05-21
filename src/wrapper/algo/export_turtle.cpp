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

#include <plantgl/algo/modelling/turtle.h>
#include <plantgl/python/export_property.h>

#include <boost/python.hpp>
using namespace boost::python;
#define bp boost::python
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

void py_error_handler(const std::string& msg){
    PyErr_SetString(PyExc_ValueError, msg.c_str() );
	throw_error_already_set();
}

void py_warning_handler(const std::string& msg){
    PyErr_WarnEx(PyExc_Warning,msg.c_str(),1);
}

void setDefaultCrossSection0(Turtle * t) { t->setDefaultCrossSection(); }

void export_Turtle()
{
    Turtle::register_error_handler(&py_error_handler);
    Turtle::register_warning_handler(&py_warning_handler);


  class_< Turtle >("Turtle", init< optional<TurtleParam * > >("Turtle([TurtleParam]) -> Create Turtle"))

    .def("f", (void (Turtle::*) ())         &Turtle::f )
    .def("f", (void (Turtle::*) (real_t))     &Turtle::f )
    .def("F", (void (Turtle::*) ())         &Turtle::F )
    .def("F", (void (Turtle::*) (real_t))     &Turtle::F )
    .def("F", (void (Turtle::*) (real_t,real_t))&Turtle::F )

	.def("setColor", &Turtle::setColor )
    .def("incColor", &Turtle::incColor )
    .def("decColor", &Turtle::decColor )
    .def("setWidth", &Turtle::setWidth )
    .def("incWidth", &Turtle::incWidth )
    .def("decWidth", &Turtle::decWidth )
    .def("setTexture", &Turtle::setTexture )
    .def("incTexture", &Turtle::incTexture )
    .def("decTexture", &Turtle::decTexture )
    .def("setId", &Turtle::setId )
    .def("incId", &Turtle::incId )
    .def("decId", &Turtle::decId )

    .def("isValid", &Turtle::isValid)
    .def("__str__", &Turtle::str)
    .def("getParameters", 
		 (const TurtleParam& (Turtle::*) () const) &Turtle::getParameters,
		 return_value_policy<copy_const_reference>())
    .def("start", &Turtle::start)
    .def("stop", &Turtle::stop)
    .def("emptyStack", &Turtle::emptyStack)
    .def("getPosition", &Turtle::getPosition, return_value_policy<copy_const_reference>())
    .def("getHeading",  &Turtle::getHeading,  return_value_policy<copy_const_reference>())
    .def("getUp",       &Turtle::getUp,       return_value_policy<copy_const_reference>())
    .def("getLeft",     &Turtle::getLeft,     return_value_policy<copy_const_reference>())
    .def("getScale",    &Turtle::getScale,    return_value_policy<copy_const_reference>())
    .def("getWidth",    &Turtle::getWidth,    return_value_policy<return_by_value>())
    .def("getColor",    &Turtle::getColor,    return_value_policy<return_by_value>())

    .def("getOrientationMatrix", &Turtle::getOrientationMatrix)
    .def("getTransformationMatrix", &Turtle::getTransformationMatrix)

    .def("getColorListSize",  &Turtle::getColorListSize )
    .def("getTextureListSize",&Turtle::getTextureListSize )
    
    .def("reset", &Turtle::reset)
    .def("resetValues", &Turtle::resetValues)
    .def("dump", &Turtle::dump)

    .def("push", &Turtle::push)
    .def("pop", &Turtle::pop)


    .def("left",   (void (Turtle::*) ())    &Turtle::left )
    .def("left",   (void (Turtle::*) (real_t))&Turtle::left )
    .def("right",  (void (Turtle::*) ())    &Turtle::right )
    .def("right",  (void (Turtle::*) (real_t))&Turtle::right )
    .def("up",     (void (Turtle::*) ())    &Turtle::up )
    .def("up",     (void (Turtle::*) (real_t))&Turtle::up )
    .def("down",   (void (Turtle::*) ())    &Turtle::down )
    .def("down",   (void (Turtle::*) (real_t))&Turtle::down )
    .def("rollL",  (void (Turtle::*) ())    &Turtle::rollL )
    .def("rollL",  (void (Turtle::*) (real_t))&Turtle::rollL )
    .def("rollR",  (void (Turtle::*) ())    &Turtle::rollR )
    .def("rollR",  (void (Turtle::*) (real_t))&Turtle::rollR )
    .def("iRollL",  (void (Turtle::*) (real_t))&Turtle::iRollL )
    .def("iRollL",  (void (Turtle::*) ())&Turtle::iRollL )
    .def("iRollR",  (void (Turtle::*) (real_t))&Turtle::iRollR )
    .def("iRollR",  (void (Turtle::*) ())&Turtle::iRollR )
    .def("rollToVert",  (void (Turtle::*) ())&Turtle::rollToVert )
    .def("rollToVert",  (void (Turtle::*) (const Vector3&))&Turtle::rollToVert )
    .def("setHead",(void (Turtle::*) (const Vector3&,const Vector3&)) &Turtle::setHead, args("head","up") )
    .def("setHead",(void (Turtle::*) (real_t,real_t,real_t,real_t,real_t,real_t))&Turtle::setHead, args("hx","hy","hz","ux","uy","uz") )
    .def("transform",&Turtle::transform )
    .def("move",   (void (Turtle::*) (const Vector3&))&Turtle::move )
    .def("move",   (void (Turtle::*) (real_t,real_t,real_t))&Turtle::move )
    .def("shift",  (void (Turtle::*) (const Vector3&))&Turtle::shift )
    .def("shift",  (void (Turtle::*) (real_t,real_t,real_t))&Turtle::shift )
    .def("lineTo",(void (Turtle::*) (const Vector3&))&Turtle::lineTo )
    .def("lineTo",(void (Turtle::*) (real_t,real_t,real_t))&Turtle::lineTo )
    .def("lineRel",(void (Turtle::*) (const Vector3&))&Turtle::lineRel )
    .def("lineRel",(void (Turtle::*) (real_t,real_t,real_t))&Turtle::lineRel )
    .def("oLineTo",(void (Turtle::*) (const Vector3&))&Turtle::oLineTo )
    .def("oLineTo",(void (Turtle::*) (real_t,real_t,real_t))&Turtle::oLineTo )
    .def("oLineRel",(void (Turtle::*) (const Vector3&))&Turtle::oLineRel )
    .def("oLineRel",(void (Turtle::*) (real_t,real_t,real_t))&Turtle::oLineRel )
    .def("pinpointRel",(void (Turtle::*) (const Vector3&))&Turtle::pinpointRel )
    .def("pinpointRel",(void (Turtle::*) (real_t,real_t,real_t))&Turtle::pinpointRel )

    .def("scale",  (void (Turtle::*) (const Vector3&))&Turtle::scale )
    .def("scale",  (void (Turtle::*) (real_t,real_t,real_t))&Turtle::scale )
    .def("scale",  (void (Turtle::*) (real_t))          &Turtle::scale )
    .def("multScale",(void (Turtle::*) (const Vector3&))&Turtle::multScale )
    .def("multScale",(void (Turtle::*) (real_t,real_t,real_t))&Turtle::multScale )
    .def("multScale",(void (Turtle::*) (real_t))          &Turtle::multScale )
    .def("divScale", (void (Turtle::*) (const Vector3&))&Turtle::divScale )
    .def("divScale", (void (Turtle::*) (real_t,real_t,real_t))&Turtle::divScale )
    .def("divScale", (void (Turtle::*) (real_t))          &Turtle::divScale )

    .def("startPolygon", &Turtle::startPolygon )
    .def("stopPolygon",  &Turtle::stopPolygon )
    .def("startGC",      &Turtle::startGC )
    .def("stopGC",       &Turtle::stopGC )

    .def("sphere", (void (Turtle::*) ())       &Turtle::sphere )
    .def("sphere", (void (Turtle::*) (real_t))   &Turtle::sphere )
    .def("circle", (void (Turtle::*) ())       &Turtle::circle )
    .def("circle", (void (Turtle::*) (real_t))   &Turtle::circle )
    .def("surface", &Turtle::surface )
    .def("label", &Turtle::label )
	.def("frame", (void (Turtle::*) ())&Turtle::frame )
	.def("frame", (void (Turtle::*) (real_t,real_t,real_t))&Turtle::frame, (bp::arg("heigth"),bp::arg("cap_heigth_ratio")=0.2,bp::arg("cap_radius_ratio")=2) )

    .def("setDefaultStep",    &Turtle::setDefaultStep )
    .def("setAngleIncrement", &Turtle::setAngleIncrement )
    .def("setWidthIncrement", &Turtle::setWidthIncrement )
    .def("setColorIncrement", &Turtle::setColorIncrement )
    .def("setScaleMultiplier",&Turtle::setScaleMultiplier )

	.def("setCrossSection", &Turtle::setCrossSection, args("curve"))
	.def("setDefaultCrossSection", &Turtle::setDefaultCrossSection, args("slices"))
	.def("setDefaultCrossSection", &setDefaultCrossSection0)

	.def("setGuide",   (void(Turtle::*)(const LineicModelPtr&, real_t))&Turtle::setGuide, args("curve","length"))
	.def("setGuide",   (void(Turtle::*)(const Curve2DPtr&, real_t, bool, bool))&Turtle::setGuide, (bp::arg("curve"),bp::arg("length"),bp::arg("yorientation")=false,bp::arg("ccw")=false))
	.def("clearGuide", &Turtle::clearGuide)
	.def("setPositionOnGuide", &Turtle::setPositionOnGuide)

	.add_property("elasticity",&get_prop_bt_from_class<real_t,Turtle,&Turtle::getElasticity>,&Turtle::setElasticity)
	.add_property("tropism",&get_prop_bt_from_class<Vector3,Turtle,&Turtle::getTropism>,(void(Turtle::*)(const Vector3&))&Turtle::setTropism)
	.def("setTropism", (void(Turtle::*)(real_t,real_t,real_t))&Turtle::setTropism)

	.add_property("sectionResolution",&get_prop_bt_from_class<uint_t,Turtle,&Turtle::getSectionResolution>,&Turtle::setSectionResolution)

	.add_property("warn_on_error",make_getter(&Turtle::warn_on_error),make_setter(&Turtle::warn_on_error) )

/*    .def("_frustum",&Turtle::_frustum )
    .def("_cylinder",&Turtle::_cylinder )
//    .def("_polygon",&Turtle::_polygon )
//    .def("_generalizedCylinder",&Turtle::_generalizedCylinder )
    .def("_sphere",&Turtle::_sphere )
    .def("_circle",&Turtle::_circle )
    .def("_surface",&Turtle::_surface )
	.def("_label",&Turtle::_label )*/
    ;
}
