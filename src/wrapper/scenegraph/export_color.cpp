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



#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <plantgl/python/tuple_converter.h>
#include <boost/python.hpp>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python

#define EXPORT(PREFIX,TYPE,CHANNEL) \
int PREFIX##_get##CHANNEL( TYPE* v ) \
{ \
  return (int)v->get##CHANNEL(); \
} \
\
void PREFIX##_set##CHANNEL( TYPE* v, int val ) \
{ \
  if (val >= 0 && val < 256) \
    v->get##CHANNEL() = (uchar_t)val; \
  else throw PythonExc_ValueError(); \
} \
\
real_t PREFIX##_get##CHANNEL##Clamped( TYPE* v ) \
{ \
  return v->get##CHANNEL##Clamped(); \
} \
\
void PREFIX##_set##CHANNEL##Clamped( TYPE* v, real_t val ) \
{ \
  if (val >= 0.0 && val <= 1.0) \
    v->get##CHANNEL() = (uchar_t)(val*255); \
  else throw PythonExc_ValueError(); \
} \

std::string col3_str( Color3* v )
{
  stringstream ss;
  ss << "Color3("
     <<(int)v->getRed() << ','
     <<(int)v->getGreen() << ','
     <<(int)v->getBlue() << ')' ;
  return ss.str();
}

std::string col4_str( Color4* v )
{
  stringstream ss;
  ss << "Color4("
     <<(int)v->getRed() << ','
     <<(int)v->getGreen() << ','
     <<(int)v->getBlue() << ','
     <<(int)v->getAlpha() << ')' ;
  return ss.str();
}

EXPORT(col3,Color3,Red)
EXPORT(col3,Color3,Green)
EXPORT(col3,Color3,Blue)

EXPORT(col4,Color4,Red)
EXPORT(col4,Color4,Green)
EXPORT(col4,Color4,Blue)
EXPORT(col4,Color4,Alpha)

struct col3_pickle_suite : boost::python::pickle_suite
{
  static boost::python::tuple getinitargs(Color3 const& c)
    {
      return boost::python::make_tuple(c.getRed(),c.getGreen(),c.getBlue());
    }
};

struct col4_pickle_suite : boost::python::pickle_suite
{
  static boost::python::tuple getinitargs(Color4 const& c)
    {
      return boost::python::make_tuple(c.getRed(),c.getGreen(),c.getBlue(),c.getAlpha());
    }
};

bp::object py_to_hsv8(Color3 * c){
    Tuple3<uchar_t> hsv = c->toHSV8();
    return boost::python::make_tuple(hsv[0],hsv[1],hsv[2]);
}

bp::object py_to_hsv(Color3 * c){
    Tuple3<real_t> hsv = c->toHSV();
    return boost::python::make_tuple(hsv[0],hsv[1],hsv[2]);
}

Color3 py_from_hsv( bp::object hsv){
    extract<uchar_t> ec0(hsv[0]);
    extract<uchar_t> ec1(hsv[1]);
    extract<uchar_t> ec2(hsv[2]);
    if (ec0.check() && ec1.check() && ec2.check()){
        return Color3::fromHSV(Tuple3<uchar_t>(ec0(),ec1(),ec2()));
    }
    else {
        return Color3::fromHSV(Tuple3<real_t>(extract<real_t>(hsv[0])(),
                                                     extract<real_t>(hsv[1])(),
                                                     extract<real_t>(hsv[2])()));

    }
}


bp::object py_to_hsva8(Color4 * c){
    Tuple4<uchar_t> hsv = c->toHSVA8();
    return boost::python::make_tuple(hsv[0],hsv[1],hsv[2],hsv[3]);
}

bp::object py_to_hsva(Color4 * c){
    Tuple4<real_t> hsv = c->toHSVA();
    return boost::python::make_tuple(hsv[0],hsv[1],hsv[2],hsv[3]);
}

Color4 py_from_hsva( bp::object hsv){
    extract<uchar_t> ec0(hsv[0]);
    extract<uchar_t> ec1(hsv[1]);
    extract<uchar_t> ec2(hsv[2]);
    extract<uchar_t> ec3(hsv[3]);
    if (ec0.check() && ec1.check() && ec2.check() && ec3.check()){
        return Color4::fromHSVA(Tuple4<uchar_t>(ec0(),ec1(),ec2(),ec3()));
    }
    else {
        return Color4::fromHSVA(Tuple4<real_t>(extract<real_t>(hsv[0])(),
                                         extract<real_t>(hsv[1])(),
                                         extract<real_t>(hsv[2])(),
                                         extract<real_t>(hsv[3])()));

    }
}



void export_Color3()
{
   enum_<Color3::eColor3Format>("eColor3Format")
    .value("eRGB",Color3::eRGB)
    .value("eRBG",Color3::eRBG)
    .value("eGRB",Color3::eGRB)
    .value("eGBR",Color3::eGBR)
    .value("eBGR",Color3::eBGR)
    .value("eBRG",Color3::eBRG)
    .export_values()
      ;

  class_< Color3 >("Color3", "A 3 components color expressed in red, green and blue.", init< const Color3 & >(args("other")))
      .def(init< uchar_t, uchar_t, uchar_t >("Color3(red,green,blue)",
            (bp::arg("red")=0,bp::arg("green")=0,bp::arg("blue")=0)))
      .def(init< optional< unsigned char > >(bp::arg("value")=0))
      .def(init< const Color4& >(args("other")))
      .def( self == self )
      .def( self != self )
      .def( self + self )
      .def( self += self )
      .def( self * self )
      .def( self *= self )
      .def( self * other< real_t >() )
      .def( other< real_t >() * self )
      .add_property( "red", col3_getRed, col3_setRed )
      .add_property( "green", col3_getGreen, col3_setGreen  )
      .add_property( "blue", col3_getBlue, col3_setBlue  )
      .def( "clampedRed", col3_getRedClamped)
      .def( "setClampedRed", col3_setRedClamped )
      .def( "clampedGreen", col3_getGreenClamped)
      .def( "setClampedGreen", col3_setGreenClamped )
      .def( "clampedBlue", col3_getBlueClamped)
      .def( "setClampedBlue", col3_setBlueClamped )
      .def( "getAverage", &Color3::getAverage )
      .def( "getAverageClamped", &Color3::getAverageClamped )
      .def( "toUint", &Color3::toUint )
      .def( "__int__", &Color3::toUint )
      .def( "fromUint", &Color3::fromUint )
      .staticmethod("fromUint")
      .def( "__repr__", col3_str )
      .def( "__str__", col3_str )
      .def( "toHSV", py_to_hsv )
      .def( "toHSV8", py_to_hsv8 )
      .def( "fromHSV", py_from_hsv, "Construct a Color3 from a tuple containing HSV values (H should be in [0,255] if int or [0.,360.[ if float, S and V should be in [0,255])" )
      .staticmethod("fromHSV")
      .def( "interpolate", Color3::interpolate, (bp::arg("c"),bp::arg("alpha")=0.5) )
      .add_static_property("BLACK",make_getter(&Color3::BLACK))
      .add_static_property("BLUE",make_getter(&Color3::BLUE))
      .add_static_property("CYAN",make_getter(&Color3::CYAN))
      .add_static_property("GREEN",make_getter(&Color3::GREEN))
      .add_static_property("RED",make_getter(&Color3::RED))
      .add_static_property("WHITE",make_getter(&Color3::WHITE))
      .add_static_property("YELLOW",make_getter(&Color3::YELLOW))
      .def_pickle(col3_pickle_suite());

  pgltuple_from_tuple<Color3,3>();
}

void export_Color4()
{
   enum_<Color4::eColor4Format>("eColor4Format")
    .value("eARGB",Color4::eARGB)
    .value("eARBG",Color4::eARBG)
    .value("eAGRB",Color4::eAGRB)
    .value("eAGBR",Color4::eAGBR)
    .value("eABGR",Color4::eABGR)
    .value("eABRG",Color4::eABRG)

    .value("eRGBA",Color4::eRGBA)
    .value("eRBGA",Color4::eRBGA)
    .value("eGRBA",Color4::eGRBA)
    .value("eGBRA",Color4::eGBRA)
    .value("eBGRA",Color4::eBGRA)
    .value("eBRGA",Color4::eBRGA)
    .export_values()

    ;

  class_< Color4 >("Color4", "A 4 component color expressed in red, green, blue and alpha.", init< const Color4 & >(args("other")))
      .def(init< uchar_t, uchar_t, uchar_t , uchar_t>("Color4(red,green,blue,alpha)",
            (bp::arg("red")=0,bp::arg("green")=0,bp::arg("blue")=0,bp::arg("alpha")=0)))
      .def(init< optional< unsigned char > >(bp::arg("value")=0))
      .def(init< const Color3&, optional< unsigned char > >((bp::arg("other")=0,bp::arg("alpha")=0)))
      .def( self == self )
      .def( self != self )
      .def( self + self )
      .def( self += self )
      .def( self * self )
      .def( self *= self )
      .def( self * other< real_t >() )
      .def( other< real_t >() * self )
      .add_property( "red", col4_getRed, col4_setRed )
      .add_property( "green", col4_getGreen, col4_setGreen  )
      .add_property( "blue", col4_getBlue, col4_setBlue  )
      .add_property( "alpha", col4_getAlpha, col4_setAlpha  )
      .def( "clampedRed", col4_getRedClamped)
      .def( "setClampedRed", col4_setRedClamped )
      .def( "clampedGreen", col4_getGreenClamped)
      .def( "setClampedGreen", col4_setGreenClamped )
      .def( "clampedBlue", col4_getBlueClamped)
      .def( "setClampedBlue", col4_setBlueClamped )
      .def( "clampedAlpha", col4_getAlphaClamped)
      .def( "setClampedAlpha", col4_setAlphaClamped )
      .def( "getAverage", &Color4::getAverage )
      .def( "getAverageClamped", &Color4::getAverageClamped )
      .def( "getRGBAverage", &Color4::getAverage )
      .def( "getRGBAverageClamped", &Color4::getAverageClamped )
      .def( "toUint", &Color4::toUint )
      .def( "__int__", &Color4::toUint )
      .def( "fromUint", &Color4::fromUint )
      .staticmethod("fromUint")
      .def( "toHSVA", py_to_hsva )
      .def( "toHSVA8", py_to_hsva8 )
      .def( "fromHSVA", py_from_hsva )
      .staticmethod("fromHSVA")
      .def( "interpolate", Color4::interpolate, (bp::arg("c"),bp::arg("alpha")=0.5) )
      .def( "__str__", col4_str )
      .def( "__repr__", col4_str )
      .add_static_property("BLACK",make_getter(&Color4::BLACK))
      .add_static_property("BLUE",make_getter(&Color4::BLUE))
      .add_static_property("CYAN",make_getter(&Color4::CYAN))
      .add_static_property("GREEN",make_getter(&Color4::GREEN))
      .add_static_property("RED",make_getter(&Color4::RED))
      .add_static_property("WHITE",make_getter(&Color4::WHITE))
      .add_static_property("YELLOW",make_getter(&Color4::YELLOW))
      .def_pickle(col4_pickle_suite());

    pgltuple_from_tuple<Color4,4>();

    def("apply_colormap",(Color3ArrayPtr(*)(const Color3ArrayPtr, const RealArrayPtr))&apply_colormap<Color3Array>,bp::args("colormap","values"));
    def("apply_colormap",(Color3ArrayPtr(*)(const Color3ArrayPtr, const RealArrayPtr, real_t, real_t))&apply_colormap<Color3Array>,bp::args("colormap","values","minvalue","maxvalue"));
    def("apply_colormap",(Color4ArrayPtr(*)(const Color4ArrayPtr, const RealArrayPtr))&apply_colormap<Color4Array>,bp::args("colormap","values"));
    def("apply_colormap",(Color4ArrayPtr(*)(const Color4ArrayPtr, const RealArrayPtr, real_t, real_t))&apply_colormap<Color4Array>,bp::args("colormap","values","minvalue","maxvalue"));
}

