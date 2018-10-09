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

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python


void export_ZBufferEngine()
{

   enum_<ZBufferEngine::eRenderingStyle>("eRenderingStyle")
    .value("eColorBased",ZBufferEngine::eColorBased)
    .value("eIdBased",ZBufferEngine::eIdBased)
    .value("eDepthOnly",ZBufferEngine::eDepthOnly)
      .export_values()
      ;
    
  class_< ZBufferEngine, boost::noncopyable > 
      ("ZBufferEngine", init<uint16_t, uint16_t , const Color3&, ZBufferEngine::eRenderingStyle>("Construct a ZBufferEngine.",(bp::arg("imageWidth")=800, bp::arg("imageHeight")=600, bp::arg("backGroundColor")=Color3(0,0,0), bp::arg("renderingStyle") = ZBufferEngine::eColorBased)) )
      .def(init<uint16_t, uint16_t , const Color4&, ZBufferEngine::eRenderingStyle>("Construct a ZBufferEngine.",(bp::arg("imageWidth")=800, bp::arg("imageHeight")=600, bp::arg("defaultColor")=Color4::fromUint(Shape::NOID), bp::arg("renderingStyle") = ZBufferEngine::eIdBased)))
      .def(init<uint16_t, uint16_t , uint32_t, Color4::eColor4Format>("Construct a ZBufferEngine.",(bp::arg("imageWidth")=800, bp::arg("imageHeight")=600, bp::arg("defaultId")=Shape::NOID, bp::arg("conversionformat")=Color4::eARGB)))
      .def("setPerspectiveCamera", &ZBufferEngine::setPerspectiveCamera)
      .def("setFrustumCamera", &ZBufferEngine::setFrustumCamera)
      .def("setOrthographicCamera", &ZBufferEngine::setOrthographicCamera)
      .def("lookAt", &ZBufferEngine::lookAt)
      .def("setId", &ZBufferEngine::setId)
      .def("setLight", (void(ZBufferEngine::*)(const Vector3&, const Color3&))&ZBufferEngine::setLight)
      .def("setLight", (void(ZBufferEngine::*)(const Vector3&, const Color3&, const Color3&, const Color3&))&ZBufferEngine::setLight)
      .def("render", (void(ZBufferEngine::*)(TriangleSetPtr, AppearancePtr))&ZBufferEngine::render)
      .def("render", (void(ZBufferEngine::*)(PolylinePtr, MaterialPtr))&ZBufferEngine::render)
      .def("render", (void(ZBufferEngine::*)(PointSetPtr, MaterialPtr))&ZBufferEngine::render)
      .def("render", (void(ZBufferEngine::*)(ScenePtr))&ZBufferEngine::render)
      .def("getImage", &ZBufferEngine::getImage)
      .def("getDepthBuffer", &ZBufferEngine::getDepthBuffer)

/*
      .def("worldToCamera", &ZBufferEngine::worldToCamera)
      .def("cameraToNDC", &ZBufferEngine::cameraToNDC)
      .def("ndcToRaster", &ZBufferEngine::ndcToRaster)
      .def("cameraToRaster", &ZBufferEngine::cameraToRaster)
      .def("worldToRaster", &ZBufferEngine::worldToRaster)
      .def("getBoundingBoxView", &ZBufferEngine::getBoundingBoxView)
      .def("getWorldToCameraMatrix", &ZBufferEngine::getWorldToCameraMatrix)
*/
      .def("duplicateBuffer", (void(ZBufferEngine::*)(const Vector3&, const Vector3&, bool, const Color3&))&ZBufferEngine::duplicateBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("duplicateBuffer", (void(ZBufferEngine::*)(int32_t, int32_t, real_t, bool, const Color3&))&ZBufferEngine::duplicateBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("periodizeBuffer", (void(ZBufferEngine::*)(const Vector3&, const Vector3&, bool, const Color3&))&ZBufferEngine::periodizeBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("periodizeBuffer", (void(ZBufferEngine::*)(int32_t, int32_t, real_t, bool, const Color3&))&ZBufferEngine::periodizeBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("setIdRendering", &ZBufferEngine::setIdRendering)
      .def("isVisible", (bool(ZBufferEngine::*)(int32_t, int32_t, real_t) const)&ZBufferEngine::isVisible,(bp::arg("x"), bp::arg("y"), bp::arg("z")))
      .def("isVisible", (bool(ZBufferEngine::*)(const Vector3&) const)&ZBufferEngine::isVisible,(bp::arg("position")))
      ;


}


