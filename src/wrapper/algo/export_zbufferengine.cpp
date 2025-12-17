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

#include <plantgl/algo/projection/zbufferengine.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/boost_python.h>
#include <plantgl/python/extract_list.h>
#include <plantgl/python/export_list.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python


boost::python::object py_grabZBufferPoints(ZBufferEngine * ze, real_t jitter = 0, real_t raywidth = 0){
    std::tuple<Point3ArrayPtr,Color3ArrayPtr,Uint32Array1Ptr> bufpoints = ze->grabZBufferPoints(jitter, raywidth);
    if (ze->getRenderingStyle() == ZBufferEngine::eIdAndColorBased)
        return boost::python::make_tuple(std::get<0>(bufpoints),std::get<1>(bufpoints),std::get<2>(bufpoints));
    else {
        if (ze->getRenderingStyle() & ZBufferEngine::eIdBased)
            return boost::python::make_tuple(std::get<0>(bufpoints),std::get<2>(bufpoints));
        else if (ze->getRenderingStyle() & ZBufferEngine::eColorBased)
            return boost::python::make_tuple(std::get<0>(bufpoints),std::get<1>(bufpoints));
        else
            return boost::python::object(std::get<0>(bufpoints));
    }
}

boost::python::object py_idhistogram(ZBufferEngine * ze, bool solidangle = true){
    pgl_hash_map<uint32_t,real_t> res = ze->idhistogram(solidangle);
    boost::python::list bres;
    for(pgl_hash_map<uint32_t,real_t>::const_iterator _it = res.begin(); _it != res.end(); ++_it){
      bres.append(boost::python::make_tuple(_it->first,_it->second));
    }
    return bres;
}



boost::python::object py_agregate_idsurfaces(ZBufferEngine * engine) {
    pgl_hash_map<uint32_t,std::pair<real_t,std::vector<real_t> > > result = 
        engine->aggregateIdSurfaces();
    boost::python::dict bres;
    for(pgl_hash_map<uint32_t,std::pair<real_t,std::vector<real_t> > >::const_iterator _it = result.begin(); _it != result.end(); ++_it){
      bres[boost::python::object(_it->first)] = boost::python::make_tuple(boost::python::object(_it->second.first), make_list(_it->second.second)());
    }
    return bres;
}


void export_ZBufferEngine()
{

   enum_<ZBufferEngine::eRenderingStyle>("eRenderingStyle")
    .value("eColorBased",ZBufferEngine::eColorBased)
    .value("eIdBased",ZBufferEngine::eIdBased)
    .value("eDepthOnly",ZBufferEngine::eDepthOnly)
    .value("eIdAndColorBased",ZBufferEngine::eIdAndColorBased)
    .value("eOrientationBased",ZBufferEngine::eIdAndColorBased)
    .value("eColorAndOrientationBased",ZBufferEngine::eColorAndOrientationBased)
    .value("eIdAndOrientationBased",ZBufferEngine::eIdAndOrientationBased)
    .value("eIdAndColorAndOrientationBased",ZBufferEngine::eIdAndColorAndOrientationBased)
      .export_values()
      ;


   enum_<ZBufferEngine::eFaceCulling>("eFaceCulling")
    .value("eNoCulling",ZBufferEngine::eNoCulling)
    .value("eBackFaceCulling",ZBufferEngine::eBackFaceCulling)
    .value("eFrontFaceCulling",ZBufferEngine::eFrontFaceCulling)
    .value("eBothFaceCulling",ZBufferEngine::eBothFaceCulling)
    .export_values()
    ;


  class_< ZBufferEngine, bases<ProjectionEngine>, boost::noncopyable > 
      ("ZBufferEngine", init<uint16_t, uint16_t, ZBufferEngine::eRenderingStyle, const Color3&, uint32_t, bool, ZBufferEngine::eFaceCulling, ProjectionEngine::eIdPolicy>("Construct a ZBufferEngine.",
                        (bp::arg("imageWidth")=800, 
                         bp::arg("imageHeight")=600, 
                         bp::arg("renderingStyle") = ZBufferEngine::eColorBased, 
                         bp::arg("backGroundColor")=Color3(0,0,0), 
                         bp::arg("defaultId")=Shape::NOID, 
                         bp::arg("multithreaded")=true, 
                         bp::arg("faceculling")=ZBufferEngine::eNoCulling,
                         bp::arg("idPolicy")=ProjectionEngine::eShapeIdBased)))
      .def("setLight", (void(ZBufferEngine::*)(const Vector3&, const Color3&, bool))&ZBufferEngine::setLight, (bp::arg("position"), bp::arg("color")=Color3(255,255,255), bp::arg("directional")=false))
      .def("setLight", (void(ZBufferEngine::*)(const Vector3&, const Color3&, const Color3&, const Color3&, bool))&ZBufferEngine::setLight, (bp::arg("position"), bp::arg("ambient")=Color3(255,255,255), bp::arg("diffuse")=Color3(255,255,255), bp::arg("specular")=Color3(255,255,255), bp::arg("directional")=false))
      .def("setHemisphericCamera", &ZBufferEngine::setHemisphericCamera, (bp::arg("near")=0,bp::arg("far")=REAL_MAX))
      .def("setSphericalCamera", &ZBufferEngine::setSphericalCamera, (bp::arg("viewAngle")=180,bp::arg("near")=0,bp::arg("far")=REAL_MAX))
      .def("setEquirectangularCamera", &ZBufferEngine::setEquirectangularCamera, (bp::arg("viewAngle")=180,bp::arg("near")=0,bp::arg("far")=REAL_MAX))
      .def("setCylindricalCamera", &ZBufferEngine::setCylindricalCamera, (bp::arg("viewAngle")=180,bp::arg("bottom")=-1,bp::arg("top")=1,bp::arg("near")=0,bp::arg("far")=REAL_MAX))
      .def("getRenderingStyle", &ZBufferEngine::getRenderingStyle)
      .def("getImage", &ZBufferEngine::getImage)
      .def("getDepthBuffer", &ZBufferEngine::getDepthBuffer)
      .def("getIdBuffer", &ZBufferEngine::getIdBuffer)
      .def("getIdBufferAsImage", &ZBufferEngine::getIdBufferAsImage,(bp::arg("conversionFormat")=Color4::eARGB))
      .def("getOrientationBuffer", &ZBufferEngine::getOrientationBuffer)
      .add_property("multithreaded",&ZBufferEngine::isMultiThreaded, &ZBufferEngine::setMultiThreaded)
      .add_property("faceculling",&ZBufferEngine::getFaceCulling, &ZBufferEngine::setFaceCulling)


      .def("duplicateBuffer", (void(ZBufferEngine::*)(const Vector3&, const Vector3&, bool, const Color3&))&ZBufferEngine::duplicateBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("duplicateBuffer", (void(ZBufferEngine::*)(int32_t, int32_t, real_t, bool, const Color3&))&ZBufferEngine::duplicateBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("periodizeBuffer", (void(ZBufferEngine::*)(const Vector3&, const Vector3&, bool, const Color3&))&ZBufferEngine::periodizeBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("periodizeBuffer", (void(ZBufferEngine::*)(int32_t, int32_t, real_t, bool, const Color3&))&ZBufferEngine::periodizeBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("isVisible", (bool(ZBufferEngine::*)(int32_t, int32_t, real_t) const)&ZBufferEngine::isVisible,(bp::arg("x"), bp::arg("y"), bp::arg("z")))
      .def("isVisible", (bool(ZBufferEngine::*)(const Vector3&) const)&ZBufferEngine::isVisible,(bp::arg("position")))
      .def("grabZBufferPoints", &py_grabZBufferPoints,(bp::arg("jitter")=0, bp::arg("raywidth")=0))
      .def("grabSortedZBufferPoints", &ZBufferEngine::grabSortedZBufferPoints,(bp::arg("jitter")=0, bp::arg("raywidth")=0))
      .def("idhistogram", &py_idhistogram, (bp::arg("solidangle")=true))
      .def("aggregateIdSurfaces", &py_agregate_idsurfaces)
     ;

      def("formFactors", &formFactors, (bp::arg("points"), bp::arg("triangles"), bp::arg("normals")=Point3ArrayPtr(0), bp::arg("ccw")=true, bp::arg("discretization")=200, bp::arg("solidangle")=200));
}
