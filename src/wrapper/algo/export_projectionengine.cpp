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



#include <plantgl/algo/projection/projectionengine.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/boost_python.h>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python


void export_ProjectionCamera()
{

     enum_<ProjectionCamera::eProjectionType>("eProjectionType")
    .value("eOrthographic",ProjectionCamera::eOrthographic)
    .value("ePerspective",ProjectionCamera::ePerspective)
      .export_values()
      ;

  class_< ProjectionCamera, ProjectionCameraPtr, bases<RefCountObject>, boost::noncopyable  >
      ("ProjectionCamera", init<real_t , real_t , real_t , real_t , real_t , real_t , ProjectionCamera::eProjectionType>
        ("Construct a ProjectionCamera", (bp::arg("left"),bp::arg("right"),bp::arg("bottom"),bp::arg("top"),bp::arg("near"),bp::arg("far"),bp::arg("projection")=ProjectionCamera::eOrthographic)))

      .def("worldToCamera", &ProjectionCamera::worldToCamera)
      .def("cameraToNDC", &ProjectionCamera::cameraToNDC)
      .def("ndcToRaster", &ProjectionCamera::ndcToRaster)
      .def("cameraToRaster", &ProjectionCamera::cameraToRaster)
      .def("worldToRaster", &ProjectionCamera::worldToRaster)
      .def("getBoundingBoxView", &ProjectionCamera::getBoundingBoxView)
      .def("getWorldToCameraMatrix", &ProjectionCamera::getWorldToCameraMatrix)
      .def("getCameraToWorldMatrix", &ProjectionCamera::getCameraToWorldMatrix)
      .def("getModelTransformationMatrix", &ProjectionCamera::getModelTransformationMatrix)


      ;

      implicitly_convertible< ProjectionCameraPtr, RefCountObjectPtr >();

}

void export_ProjectionEngine()
{


  class_< ProjectionEngine, boost::noncopyable >
      ("ProjectionEngine", no_init)
      .def("setPerspectiveCamera", &ProjectionEngine::setPerspectiveCamera)
      .def("setFrustumCamera", &ProjectionEngine::setFrustumCamera)
      .def("setOrthographicCamera", &ProjectionEngine::setOrthographicCamera)
      .def("lookAt", &ProjectionEngine::lookAt)
      .def("getBoundingBoxView", &ProjectionEngine::getBoundingBoxView)
      .def("camera", &ProjectionEngine::camera)

      .def("process", (void(ProjectionEngine::*)(TriangleSetPtr, AppearancePtr, uint32_t))&ProjectionEngine::process)
      .def("process", (void(ProjectionEngine::*)(PolylinePtr, MaterialPtr, uint32_t))&ProjectionEngine::process)
      .def("process", (void(ProjectionEngine::*)(PointSetPtr, MaterialPtr, uint32_t))&ProjectionEngine::process)
      .def("process", (void(ProjectionEngine::*)(ScenePtr))&ProjectionEngine::process)

      /*.def("worldToCamera", &ProjectionEngine::worldToCamera)
      .def("cameraToNDC", &ProjectionEngine::cameraToNDC)
      .def("ndcToRaster", &ProjectionEngine::ndcToRaster)
      .def("cameraToRaster", &ProjectionEngine::cameraToRaster)
      .def("worldToRaster", &ProjectionEngine::worldToRaster)
      .def("getWorldToCameraMatrix", &ProjectionEngine::getWorldToCameraMatrix)
      */

      ;


}


