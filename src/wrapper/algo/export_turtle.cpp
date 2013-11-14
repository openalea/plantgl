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

void setDefaultCrossSection1(Turtle * t, size_t slices) { t->setDefaultCrossSection(slices); }
void setDefaultCrossSection0(Turtle * t) { t->setDefaultCrossSection(); }

void export_Turtle()
{
    Turtle::register_error_handler(&py_error_handler);
    Turtle::register_warning_handler(&py_warning_handler);


  class_< Turtle >("Turtle", init< optional<TurtleParam * > >("Turtle([TurtleParam]) -> Create Turtle"))

    .def("f", (void (Turtle::*) ())         &Turtle::f, return_self<>() )
    .def("f", (void (Turtle::*) (real_t))     &Turtle::f, return_self<>() )
    .def("F", (void (Turtle::*) ())         &Turtle::F, return_self<>() )
    .def("F", (void (Turtle::*) (real_t))     &Turtle::F, return_self<>() )
    .def("F", (void (Turtle::*) (real_t,real_t))&Turtle::F, return_self<>() )

    .def("nF", (void (Turtle::*) (real_t,real_t))&Turtle::nF, (bp::arg("length"),bp::arg("dlength")), return_self<>())
    .def("nF", (void (Turtle::*) (real_t,real_t,real_t,const QuantisedFunctionPtr))&Turtle::nF, (bp::arg("length"),bp::arg("dlength"),bp::arg("radius")=1.0,bp::arg("radiusvariation")=QuantisedFunctionPtr()), return_self<>())

	.def("setColor", &Turtle::setColor, return_self<>() )
	.def("setCustomAppearance", &Turtle::setCustomAppearance, return_self<>() )
    .def("incColor", &Turtle::incColor, return_self<>() )
    .def("decColor", &Turtle::decColor, return_self<>() )
    .def("setWidth", &Turtle::setWidth, return_self<>() )
    .def("incWidth", &Turtle::incWidth, return_self<>() )
    .def("decWidth", &Turtle::decWidth, return_self<>() )

    .def("setTextureScale", (void (Turtle::*)(real_t,real_t))&Turtle::setTextureScale, return_self<>() )
    .def("setTextureScale", (void (Turtle::*)(const Vector2&))&Turtle::setTextureScale, return_self<>() )
    .def("setTextureVScale", &Turtle::setTextureVScale, return_self<>() )
    .def("setTextureUScale", &Turtle::setTextureUScale, return_self<>() )

	.def("setTextureRotation",(void (Turtle::*)(real_t,real_t,real_t)) &Turtle::setTextureRotation ,(bp::arg("angle")=0,bp::arg("ucenter")=0.5,bp::arg("vcenter")=0.5), return_self<>())
	.def("setTextureRotation",(void (Turtle::*)(real_t,const Vector2&)) &Turtle::setTextureRotation ,(bp::arg("angle")=0,bp::arg("center")=Vector2(0.5,0.5)), return_self<>())
	.def("setTextureTranslation", (void (Turtle::*)(real_t,real_t))&Turtle::setTextureTranslation ,(bp::arg("u")=0,bp::arg("v")=0), return_self<>())
	.def("setTextureTranslation", (void (Turtle::*)(const Vector2&))&Turtle::setTextureTranslation ,(bp::arg("t")), return_self<>())
	.def("setTextureTransformation", (void (Turtle::*)(real_t,real_t,real_t,real_t,real_t,real_t,real_t))&Turtle::setTextureTransformation ,
		(bp::arg("uscaling")=1,bp::arg("vscaling")=1,
		 bp::arg("utranslation")=0,bp::arg("vtranslation")=0,
		 bp::arg("angle")=0,bp::arg("urotcenter")=0.5,bp::arg("vrotcenter")=0.5), return_self<>())
	.def("setTextureTransformation", (void (Turtle::*)(const Vector2&,const Vector2&,real_t,const Vector2&))&Turtle::setTextureTransformation ,
		(bp::arg("scaling")=Vector2(1,1),
		 bp::arg("translation")=Vector2(0,0),
		 bp::arg("angle")=0,bp::arg("rotcenter")=Vector2(0.5,0.5)), return_self<>())
    
    .def("setId", &Turtle::setId, return_self<>() )
    .def("incId", &Turtle::incId, return_self<>() )
    .def("decId", &Turtle::decId, return_self<>() )

    .def("isValid", &Turtle::isValid)
    .def("__str__", &Turtle::str)
    .def("getParameters", 
		 (const TurtleParam& (Turtle::*) () const) &Turtle::getParameters,
		 return_value_policy<copy_const_reference>())
    .def("start", &Turtle::start, return_self<>())
    .def("stop", &Turtle::stop, return_self<>())
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
    
    .def("reset", &Turtle::reset)
    .def("resetValues", &Turtle::resetValues)
    .def("dump", &Turtle::dump)

    .def("push", &Turtle::push, return_self<>())
    .def("pop", &Turtle::pop, return_self<>())


    .def("left",   (void (Turtle::*) ())    &Turtle::left , return_self<>())
    .def("left",   (void (Turtle::*) (real_t))&Turtle::left , return_self<>())
    .def("right",  (void (Turtle::*) ())    &Turtle::right , return_self<>())
    .def("right",  (void (Turtle::*) (real_t))&Turtle::right , return_self<>())
    .def("up",     (void (Turtle::*) ())    &Turtle::up , return_self<>())
    .def("up",     (void (Turtle::*) (real_t))&Turtle::up , return_self<>())
    .def("down",   (void (Turtle::*) ())    &Turtle::down , return_self<>())
    .def("down",   (void (Turtle::*) (real_t))&Turtle::down , return_self<>())
    .def("rollL",  (void (Turtle::*) ())    &Turtle::rollL , return_self<>())
    .def("rollL",  (void (Turtle::*) (real_t))&Turtle::rollL , return_self<>())
    .def("rollR",  (void (Turtle::*) ())    &Turtle::rollR , return_self<>())
    .def("rollR",  (void (Turtle::*) (real_t))&Turtle::rollR , return_self<>())
    .def("iRollL",  (void (Turtle::*) (real_t))&Turtle::iRollL , return_self<>())
    .def("iRollL",  (void (Turtle::*) ())&Turtle::iRollL , return_self<>())
    .def("iRollR",  (void (Turtle::*) (real_t))&Turtle::iRollR , return_self<>())
    .def("iRollR",  (void (Turtle::*) ())&Turtle::iRollR , return_self<>())
    .def("rollToVert",  (void (Turtle::*) ())&Turtle::rollToVert , return_self<>())
    .def("rollToVert",  (void (Turtle::*) (const Vector3&))&Turtle::rollToVert , return_self<>())
    .def("setHead",(void (Turtle::*) (const Vector3&,const Vector3&)) &Turtle::setHead, args("head","up") , return_self<>())
    .def("setHead",(void (Turtle::*) (real_t,real_t,real_t,real_t,real_t,real_t))&Turtle::setHead, args("hx","hy","hz","ux","uy","uz") , return_self<>())
    .def("transform",&Turtle::transform , return_self<>())
    .def("move",   (void (Turtle::*) (const Vector3&))&Turtle::move , return_self<>())
    .def("move",   (void (Turtle::*) (real_t,real_t,real_t))&Turtle::move , return_self<>())
    .def("shift",  (void (Turtle::*) (const Vector3&))&Turtle::shift , return_self<>())
    .def("shift",  (void (Turtle::*) (real_t,real_t,real_t))&Turtle::shift , return_self<>())

    .def("lineTo",(void (Turtle::*) (const Vector3&,real_t))&Turtle::lineTo, (bp::arg("position")=Vector3(0,0,0),bp::arg("topradius")=-1.) , return_self<>())
    .def("lineTo",(void (Turtle::*) (real_t,real_t,real_t,real_t))&Turtle::lineTo, (bp::arg("x")=0.0,bp::arg("y")=0.0,bp::arg("z")=0.0,bp::arg("topradius")=-1.) , return_self<>())
    .def("lineRel",(void (Turtle::*) (const Vector3&,real_t))&Turtle::lineRel, (bp::arg("position")=Vector3(0,0,0),bp::arg("topradius")=-1.) , return_self<>())
    .def("lineRel",(void (Turtle::*) (real_t,real_t,real_t,real_t))&Turtle::lineRel, (bp::arg("x")=0.0,bp::arg("y")=0.0,bp::arg("z")=0.0,bp::arg("topradius")=-1.)  , return_self<>())
    .def("oLineTo",(void (Turtle::*) (const Vector3&,real_t))&Turtle::oLineTo, (bp::arg("position")=Vector3(0,0,0),bp::arg("topradius")=-1.) , return_self<>())
    .def("oLineTo",(void (Turtle::*) (real_t,real_t,real_t,real_t))&Turtle::oLineTo,(bp::arg("x")=0.0,bp::arg("y")=0.0,bp::arg("z")=0.0,bp::arg("topradius")=-1.)  , return_self<>())
    .def("oLineRel",(void (Turtle::*) (const Vector3&,real_t))&Turtle::oLineRel, (bp::arg("position")=Vector3(0,0,0),bp::arg("topradius")=-1.) , return_self<>())
    .def("oLineRel",(void (Turtle::*) (real_t,real_t,real_t,real_t))&Turtle::oLineRel, (bp::arg("x")=0.0,bp::arg("y")=0.0,bp::arg("z")=0.0,bp::arg("topradius")=-1.)  , return_self<>())

    .def("pinpointRel",(void (Turtle::*) (const Vector3&))&Turtle::pinpointRel , return_self<>())
    .def("pinpointRel",(void (Turtle::*) (real_t,real_t,real_t))&Turtle::pinpointRel , return_self<>())

    .def("scale",  (void (Turtle::*) (const Vector3&))&Turtle::scale , return_self<>())
    .def("scale",  (void (Turtle::*) (real_t,real_t,real_t))&Turtle::scale , return_self<>())
    .def("scale",  (void (Turtle::*) (real_t))          &Turtle::scale , return_self<>())
    .def("multScale",(void (Turtle::*) (const Vector3&))&Turtle::multScale , return_self<>())
    .def("multScale",(void (Turtle::*) (real_t,real_t,real_t))&Turtle::multScale , return_self<>())
    .def("multScale",(void (Turtle::*) (real_t))          &Turtle::multScale , return_self<>())
    .def("divScale", (void (Turtle::*) (const Vector3&))&Turtle::divScale , return_self<>())
    .def("divScale", (void (Turtle::*) (real_t,real_t,real_t))&Turtle::divScale , return_self<>())
    .def("divScale", (void (Turtle::*) (real_t))          &Turtle::divScale , return_self<>())

    .def("startPolygon", &Turtle::startPolygon , return_self<>())
    .def("stopPolygon",  &Turtle::stopPolygon, (bp::arg("concavetest")=false) , return_self<>())
    .def("polygonPoint",  &Turtle::polygonPoint , return_self<>())
    .def("startGC",      &Turtle::startGC , return_self<>())
    .def("stopGC",       &Turtle::stopGC , return_self<>())

    .def("sphere", (void (Turtle::*) ())       &Turtle::sphere , return_self<>())
    .def("sphere", (void (Turtle::*) (real_t))   &Turtle::sphere , return_self<>())
    .def("circle", (void (Turtle::*) ())       &Turtle::circle , return_self<>())
    .def("circle", (void (Turtle::*) (real_t))   &Turtle::circle , return_self<>())
    .def("surface", &Turtle::surface , return_self<>())
    .def("label", &Turtle::label , return_self<>())
	.def("frame", (void (Turtle::*) ())&Turtle::frame , return_self<>())
	.def("frame", (void (Turtle::*) (real_t,real_t,real_t,real_t,real_t))&Turtle::frame, (bp::arg("heigth"),bp::arg("cap_heigth_ratio")=0.2,bp::arg("cap_radius_ratio")=2,bp::arg("color")=1.0,bp::arg("transparency")=0.0) , return_self<>())

    .def("setDefaultStep",    &Turtle::setDefaultStep, return_self<>() )
    .def("setAngleIncrement", &Turtle::setAngleIncrement, return_self<>() )
    .def("setWidthIncrement", &Turtle::setWidthIncrement, return_self<>() )
    .def("setColorIncrement", &Turtle::setColorIncrement, return_self<>() )
    .def("setScaleMultiplier",&Turtle::setScaleMultiplier, return_self<>() )

	.def("setCrossSection", &Turtle::setCrossSection, args("curve"), return_self<>())
	.def("setDefaultCrossSection", &setDefaultCrossSection1, args("slices"))
	.def("setDefaultCrossSection", &setDefaultCrossSection0)

	.def("setGuide",   (void(Turtle::*)(const LineicModelPtr&, real_t))&Turtle::setGuide, args("curve","length"), return_self<>(), return_self<>())
	.def("setGuide",   (void(Turtle::*)(const Curve2DPtr&, real_t, bool, bool))&Turtle::setGuide, (bp::arg("curve"),bp::arg("length"),bp::arg("yorientation")=false,bp::arg("ccw")=false), return_self<>())
	.def("clearGuide", &Turtle::clearGuide, return_self<>())
	.def("setPositionOnGuide", &Turtle::setPositionOnGuide, return_self<>())

    .def("sweep", (void (Turtle::*) (const Curve2DPtr&,const Curve2DPtr&,real_t,real_t,real_t,const QuantisedFunctionPtr))&Turtle::sweep, (bp::arg("path"),bp::arg("section"),bp::arg("length"),bp::arg("dlength"),bp::arg("radiusmagnitude")=1.0,bp::arg("radiusvariation")=QuantisedFunctionPtr()), return_self<>())
    .def("sweep", (void (Turtle::*) (const LineicModelPtr&,const Curve2DPtr&,real_t,real_t,real_t,const QuantisedFunctionPtr))&Turtle::sweep, (bp::arg("path"),bp::arg("section"),bp::arg("length"),bp::arg("dlength"),bp::arg("radiusmagnitude")=1.0,bp::arg("radiusvariation")=QuantisedFunctionPtr()), return_self<>())

	.add_property("elasticity",&get_prop_bt_from_class<real_t,Turtle,&Turtle::getElasticity>,&Turtle::setElasticity)
	.add_property("tropism",&get_prop_bt_from_class<Vector3,Turtle,&Turtle::getTropism>,(void(Turtle::*)(const Vector3&))&Turtle::setTropism)
	.def("setTropism", (void(Turtle::*)(real_t,real_t,real_t))&Turtle::setTropism, return_self<>())

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
