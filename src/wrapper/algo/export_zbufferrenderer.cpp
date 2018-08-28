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

#include <plantgl/algo/rasterization/zbufferrenderer.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/boost_python.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python


void export_ZBufferRenderer()
{
  class_< ZBufferRenderer, boost::noncopyable > 
      ("ZBufferRenderer", init<uint16_t, uint16_t , const Color3&>("Construct a ZBufferRenderer.",(bp::arg("imageWidth")=800, bp::arg("imageHeight")=600, bp::arg("backGroundColor")=Color3(0,0,0))) )
      .def("setPerspectiveCamera", &ZBufferRenderer::setPerspectiveCamera)
      .def("setFrustrumCamera", &ZBufferRenderer::setFrustrumCamera)
      .def("setOrthographicCamera", &ZBufferRenderer::setOrthographicCamera)
      .def("lookAt", &ZBufferRenderer::lookAt)
      .def("setLight", &ZBufferRenderer::setLight)
      .def("render", (void(ZBufferRenderer::*)(TriangleSetPtr, MaterialPtr, uint32_t))&ZBufferRenderer::render)
      .def("render", (void(ZBufferRenderer::*)(PolylinePtr, MaterialPtr, uint32_t))&ZBufferRenderer::render)
      .def("render", (void(ZBufferRenderer::*)(PointSetPtr, MaterialPtr, uint32_t))&ZBufferRenderer::render)
      .def("getImage", &ZBufferRenderer::getImage)
      .def("worldToCamera", &ZBufferRenderer::worldToCamera)
      .def("cameraToNDC", &ZBufferRenderer::cameraToNDC)
      .def("ndcToRaster", &ZBufferRenderer::ndcToRaster)
      .def("cameraToRaster", &ZBufferRenderer::cameraToRaster)
      .def("worldToRaster", &ZBufferRenderer::worldToRaster)
      .def("getBoundingBoxView", &ZBufferRenderer::getBoundingBoxView)
      .def("getWorldToCameraMatrix", &ZBufferRenderer::getWorldToCameraMatrix)
      ;

}


