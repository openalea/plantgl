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

#include <plantgl/algo/projection/projectionengine.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/boost_python.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python


ProjectionCameraPtr get_camera(ProjectionEngine * engine) { return engine->camera(); }

void export_ProjectionEngine()
{


  class_< ProjectionEngine, boost::noncopyable > 
      ("ProjectionEngine", no_init)
      .def("setPerspectiveCamera", &ProjectionEngine::setPerspectiveCamera, bp::args("verticalAngleOfView : degrees","aspectRatio", "near", "far"))
      .def("setFrustumCamera", &ProjectionEngine::setFrustumCamera, bp::args("left","right","bottom", "top", "near", "far"))
      .def("setOrthographicCamera", &ProjectionEngine::setOrthographicCamera, bp::args("left","right","bottom", "top", "near", "far"))
      .def("lookAt", &ProjectionEngine::lookAt, bp::args("eye_position","target","up"))
      // .def("getBoundingBoxView", &ProjectionEngine::getBoundingBoxView)
      .def("camera", &get_camera)
      
      .def("process", (void(ProjectionEngine::*)(TriangleSetPtr, AppearancePtr, uint32_t))&ProjectionEngine::process, (bp::arg("triangleset"),bp::arg("appearance"),bp::arg("id")))
      .def("process", (void(ProjectionEngine::*)(PolylinePtr, MaterialPtr, uint32_t))&ProjectionEngine::process, (bp::arg("polyline"),bp::arg("appearance"),bp::arg("id")))
      .def("process", (void(ProjectionEngine::*)(PointSetPtr, MaterialPtr, uint32_t))&ProjectionEngine::process, (bp::arg("pointset"),bp::arg("appearance"),bp::arg("id")))
      .def("process", (void(ProjectionEngine::*)(ScenePtr))&ProjectionEngine::process, (bp::arg("scene")))

      /*.def("worldToCamera", &ProjectionEngine::worldToCamera)
      .def("cameraToNDC", &ProjectionEngine::cameraToNDC)
      .def("ndcToRaster", &ProjectionEngine::ndcToRaster)
      .def("cameraToRaster", &ProjectionEngine::cameraToRaster)
      .def("worldToRaster", &ProjectionEngine::worldToRaster)
      .def("getWorldToCameraMatrix", &ProjectionEngine::getWorldToCameraMatrix)
      */

      ;


}

