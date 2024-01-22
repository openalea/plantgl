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

#include <plantgl/algo/projection/projectioncamera.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/boost_python.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python


void export_ProjectionCamera()
{

     enum_<ProjectionCamera::eProjectionType>("eProjectionType")
    .value("eOrthographic",ProjectionCamera::eOrthographic)
    .value("ePerspective",ProjectionCamera::ePerspective)
    .value("eHemispheric",ProjectionCamera::eHemispheric)
    .value("eCylindrical",ProjectionCamera::eCylindrical)
      .export_values()
      ;

     enum_<ProjectionCamera::eProjectionMethodType>("eProjectionMethodType")
    .value("eProjection",ProjectionCamera::eProjection)
    .value("eRayIntersection",ProjectionCamera::eRayIntersection)
      .export_values()
      ;

  class_< ProjectionCamera, ProjectionCameraPtr, bases<RefCountObject>, boost::noncopyable  > 
      ("ProjectionCamera", no_init )

      .def("worldToRaster", &ProjectionCamera::worldToRaster)
      .def("worldToCamera", &ProjectionCamera::worldToCamera)
      .def("cameraToNDC", &ProjectionCamera::cameraToNDC)
      .def("NDCToRaster", &ProjectionCamera::NDCToRaster)
      .def("cameraToRaster", &ProjectionCamera::cameraToRaster)

      .def("rasterToWorld", &ProjectionCamera::rasterToWorld)
      .def("rasterToCamera", &ProjectionCamera::rasterToCamera)
      .def("rasterToNDC", &ProjectionCamera::rasterToNDC)
      .def("NDCToCamera", &ProjectionCamera::NDCToCamera)
      .def("cameraToWorld", &ProjectionCamera::cameraToWorld)

      .def("rasterToWorldRay", &ProjectionCamera::rasterToWorldRay)
      .def("rasterToCameraRay", &ProjectionCamera::rasterToCameraRay)

      .def("isValidPixel", &ProjectionCamera::isValidPixel)
      
      .def("getWorldToCameraMatrix", &ProjectionCamera::getWorldToCameraMatrix)
      .def("getModelTransformationMatrix", &ProjectionCamera::getModelTransformationMatrix)
      .def("transformModel", &ProjectionCamera::transformModel)
      .def("pushModelTransformation", &ProjectionCamera::pushModelTransformation)
      .def("popModelTransformation", &ProjectionCamera::popModelTransformation)
      .def("transformModelIdentity", &ProjectionCamera::transformModelIdentity)

      .def("translateModel", &ProjectionCamera::translateModel)
      .def("scaleModel", &ProjectionCamera::scaleModel)

      .def("isInZRange", (bool (ProjectionCamera::*)(real_t) const)&ProjectionCamera::isInZRange)
      .def("isInZRange", (bool (ProjectionCamera::*)(real_t,real_t) const)&ProjectionCamera::isInZRange)

      .def_readwrite("near", &ProjectionCamera::near)
      .def_readwrite("far", &ProjectionCamera::far)

      .def("position", &ProjectionCamera::position, return_value_policy<copy_const_reference>())
      .def("type", &ProjectionCamera::type)
      .def("methodType", &ProjectionCamera::methodType)

      .def("projectedArea", &ProjectionCamera::projectedArea)
      .def("solidAngle", &ProjectionCamera::solidAngle)

      .def("lookAt", &ProjectionCamera::lookAt)
      ;

      implicitly_convertible< ProjectionCameraPtr, RefCountObjectPtr >();


  class_< PerspectiveCamera, PerspectiveCameraPtr, bases<ProjectionCamera>, boost::noncopyable  > 
      ("PerspectiveCamera", init<real_t , real_t , real_t , real_t , real_t , real_t > 
        ("Construct a PerspectiveCamera", (bp::arg("left"),bp::arg("right"),bp::arg("bottom"),bp::arg("top"),bp::arg("near"),bp::arg("far"))))
      .def("getBoundingBoxView", &FrustumCamera::getBoundingBoxView)
      ;

      implicitly_convertible< PerspectiveCameraPtr, ProjectionCameraPtr >();


  class_< OrthographicCamera, OrthographicCameraPtr, bases<ProjectionCamera>, boost::noncopyable  > 
      ("OrthographicCamera", init<real_t , real_t , real_t , real_t , real_t , real_t > 
        ("Construct an OrthographicCamera", (bp::arg("left"),bp::arg("right"),bp::arg("bottom"),bp::arg("top"),bp::arg("near"),bp::arg("far"))))
      .def("getBoundingBoxView", &FrustumCamera::getBoundingBoxView)
      ;

      implicitly_convertible< OrthographicCameraPtr, ProjectionCameraPtr >();


  class_< SphericalCamera, SphericalCameraPtr, bases<ProjectionCamera>, boost::noncopyable  > 
      ("SphericalCamera", init< real_t , real_t , real_t > 
        ("Construct an SphericalCamera", (bp::arg("viewAngle")=180,bp::arg("near")=0,bp::arg("far")=REAL_MAX)))
        .def("solidAngle",&SphericalCamera::solidAngle)
        .def("NDCToSpherical",&SphericalCamera::NDCToSpherical)
        .def("SphericalToNDC",&SphericalCamera::SphericalToNDC)
      ;

      implicitly_convertible< SphericalCameraPtr, ProjectionCameraPtr >();


}
