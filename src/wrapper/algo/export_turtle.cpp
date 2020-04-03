/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */



#include <plantgl/algo/modelling/turtle.h>
#include <plantgl/python/export_property.h>

#include <boost/python.hpp>
using namespace boost::python;
#define bp boost::python
PGL_USING_NAMESPACE

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
    .def("interpolateColors", &Turtle::interpolateColors, return_self<>() )

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
    .def("rollToVert",  &Turtle::rollToVert , (bp::arg("alpha")=1.0,bp::arg("top")=Vector3::OZ), return_self<>())
    .def("rollToHorizontal",  &Turtle::rollToHorizontal , (bp::arg("alpha")=1.0,bp::arg("top")=Vector3::OZ), return_self<>())
    .def("setHead",(void (Turtle::*) (const Vector3&,const Vector3&)) &Turtle::setHead, args("head","up") , return_self<>())
    .def("setHead",(void (Turtle::*) (real_t,real_t,real_t,real_t,real_t,real_t))&Turtle::setHead, args("hx","hy","hz","ux","uy","uz") , return_self<>())
    .def("eulerAngles", &Turtle::eulerAngles, "Set the orientation of the turtle from the absolute euler angles.", (bp::arg("azimuth") = 180,bp::arg("elevation") =90,bp::arg("roll") = 0), return_self<>())
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

    .def("pinpoint",(void (Turtle::*) (const Vector3&))&Turtle::pinpoint , return_self<>())
    .def("pinpoint",(void (Turtle::*) (real_t,real_t,real_t))&Turtle::pinpoint , return_self<>())
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
    .def("box", (void (Turtle::*) ())       &Turtle::box , return_self<>())
    .def("box", (void (Turtle::*) (real_t))   &Turtle::box , return_self<>())
    .def("box", (void (Turtle::*) (real_t,real_t))   &Turtle::box , return_self<>())
    .def("quad", (void (Turtle::*) ())       &Turtle::quad , return_self<>())
    .def("quad", (void (Turtle::*) (real_t))   &Turtle::quad , return_self<>())
    .def("quad", (void (Turtle::*) (real_t,real_t))   &Turtle::quad , return_self<>())
    .def("surface", &Turtle::surface , return_self<>())
    .def("label", &Turtle::label , (bp::arg("text"),bp::arg("size")=-1.), return_self<>())
    .def("frame", (void (Turtle::*) ())&Turtle::frame , return_self<>())
    .def("frame", (void (Turtle::*) (real_t,real_t,real_t,real_t,real_t))&Turtle::frame, (bp::arg("heigth"),bp::arg("cap_heigth_ratio")=0.2,bp::arg("cap_radius_ratio")=2,bp::arg("colorV")=1.0,bp::arg("transparency")=0.0) , return_self<>())
    .def("arrow", (void (Turtle::*) ())&Turtle::arrow , return_self<>())
    .def("arrow", (void (Turtle::*) (real_t,real_t,real_t))&Turtle::arrow, (bp::arg("heigth"),bp::arg("cap_heigth_ratio")=0.2,bp::arg("cap_radius_ratio")=2) , return_self<>())

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

    .def("setScreenCoordinatesEnabled", &Turtle::setScreenCoordinatesEnabled, (bp::arg("enabled")=true), return_self<>())

    .add_property("sectionResolution",&get_prop_bt_from_class<uint_t,Turtle,&Turtle::getSectionResolution>,&Turtle::setSectionResolution)

    .def_readwrite("warn_on_error",&Turtle::warn_on_error)
    .def_readwrite("path_info_cache_enabled",&Turtle::path_info_cache_enabled)

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
