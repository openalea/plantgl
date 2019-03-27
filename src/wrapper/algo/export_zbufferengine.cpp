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



#include <plantgl/algo/projection/zbufferengine.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/boost_python.h>

PGL_USING_NAMESPACE
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

  class_< ZBufferEngine, bases<ProjectionEngine>, boost::noncopyable >
      ("ZBufferEngine", init<uint16_t, uint16_t , const Color3&, ZBufferEngine::eRenderingStyle>("Construct a ZBufferEngine.",(bp::arg("imageWidth")=800, bp::arg("imageHeight")=600, bp::arg("backGroundColor")=Color3(0,0,0), bp::arg("renderingStyle") = ZBufferEngine::eColorBased)) )
      .def(init<uint16_t, uint16_t , const Color4&, ZBufferEngine::eRenderingStyle>("Construct a ZBufferEngine.",(bp::arg("imageWidth")=800, bp::arg("imageHeight")=600, bp::arg("defaultColor")=Color4::fromUint(Shape::NOID), bp::arg("renderingStyle") = ZBufferEngine::eIdBased)))
      .def(init<uint16_t, uint16_t , uint32_t, Color4::eColor4Format>("Construct a ZBufferEngine.",(bp::arg("imageWidth")=800, bp::arg("imageHeight")=600, bp::arg("defaultId")=Shape::NOID, bp::arg("conversionformat")=Color4::eARGB)))
      .def("setLight", (void(ZBufferEngine::*)(const Vector3&, const Color3&))&ZBufferEngine::setLight)
      .def("setLight", (void(ZBufferEngine::*)(const Vector3&, const Color3&, const Color3&, const Color3&))&ZBufferEngine::setLight)
      .def("getImage", &ZBufferEngine::getImage)
      .def("getDepthBuffer", &ZBufferEngine::getDepthBuffer)


      .def("duplicateBuffer", (void(ZBufferEngine::*)(const Vector3&, const Vector3&, bool, const Color3&))&ZBufferEngine::duplicateBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("duplicateBuffer", (void(ZBufferEngine::*)(int32_t, int32_t, real_t, bool, const Color3&))&ZBufferEngine::duplicateBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("periodizeBuffer", (void(ZBufferEngine::*)(const Vector3&, const Vector3&, bool, const Color3&))&ZBufferEngine::periodizeBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("periodizeBuffer", (void(ZBufferEngine::*)(int32_t, int32_t, real_t, bool, const Color3&))&ZBufferEngine::periodizeBuffer,(bp::arg("from"), bp::arg("to")=600, bp::arg("useDefaultColor")=true, bp::arg("defaultcolor")=Color3(0,0,0)))
      .def("setIdRendering", &ZBufferEngine::setIdRendering)
      .def("isVisible", (bool(ZBufferEngine::*)(int32_t, int32_t, real_t) const)&ZBufferEngine::isVisible,(bp::arg("x"), bp::arg("y"), bp::arg("z")))
      .def("isVisible", (bool(ZBufferEngine::*)(const Vector3&) const)&ZBufferEngine::isVisible,(bp::arg("position")))
      ;


}


