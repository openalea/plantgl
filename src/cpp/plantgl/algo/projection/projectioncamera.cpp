/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2018 - Cirad/Inra/Inria
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://github.com/openalea/plantgl
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


/* ----------------------------------------------------------------------- */

#include "projectioncamera.h"
#include <plantgl/math/util_math.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE



ProjectionCamera::ProjectionCamera(real_t _left, real_t _right, real_t _bottom, real_t _top, real_t _near, real_t _far, eProjectionType _type):
        RefCountObject(),
        __xscale(2. / (_right - _left) ),
        __xconstant((_right + _left) / (_right - _left)),
        __yscale(2. / (_top - _bottom) ),
        __yconstant((_top + _bottom) / (_top - _bottom)),
        left(_left),
        right(_right),
        bottom(_bottom),
        top(_top),
        near(_near),
        far(_far),
        type(_type){
}

ProjectionCamera::~ProjectionCamera() {}

void ProjectionCamera::lookAt(const TOOLS(Vector3)& eyePosition3D, const TOOLS(Vector3)& center3D, const TOOLS(Vector3)& upVector3D)
{
        Vector3 forward = eyePosition3D - center3D;
        forward.normalize();
        Vector3 up(upVector3D);
        up.normalize();
        Vector3 side = cross(upVector3D, forward);
        side.normalize();
        up = cross(forward, side);
        up.normalize();
        __position = eyePosition3D;
        __cameraToWorld = Matrix4(side, up, forward, eyePosition3D);
        __worldToCamera = inverse(__cameraToWorld);
        __currentWorldToCamera = __worldToCamera * __currentModelMatrix;

}

Vector3 ProjectionCamera::screen2NDC(const real_t& xScreen, const real_t& yScreen, const real_t z) const {
    // now convert point from screen space to NDC space (in range [-1,1])
    Vector3 vertexNDC(__xscale * xScreen  - __xconstant, __yscale * yScreen  - __yconstant,  -z);

    return vertexNDC;
}

Vector3 ProjectionCamera::NDC2screen(const real_t& xNDC, const real_t& yNDC, const real_t z) const {
    // now convert point from NDC space to screen space 
    Vector3 vertexScreen((xNDC  + __xconstant) / __xscale,  (yNDC  + __yconstant) / __yscale,  -z);
    return vertexScreen;
}

Vector3 ProjectionCamera::cameraToNDC(const Vector3& vertexCamera) const {
    if (type == eOrthographic)
        return screen2NDC(vertexCamera.x(), vertexCamera.y(), vertexCamera.z());
    else {
        real_t z = -vertexCamera.z();
        if (z < 0) { return screen2NDC(0, 0, vertexCamera.z()); }
        return screen2NDC(near * vertexCamera.x() / z, near * vertexCamera.y() / z, vertexCamera.z());        
    }
}

Vector3 ProjectionCamera::ndcToCamera(const Vector3& vertexNDC) const {
    if (type == eOrthographic)
        return NDC2screen(vertexNDC.x(), vertexNDC.y(), vertexNDC.z());
    else {
        real_t z = -vertexNDC.z();
        return NDC2screen(vertexNDC.x() * z, vertexNDC.y() * z, vertexNDC.z());
    }
}


Vector3 ProjectionCamera::worldToRaster(const Vector3& vertexWorld, const uint16_t imageWidth, const uint16_t imageHeight) const
{

    Vector3 vertexCamera = __worldToCamera * vertexWorld;
    return toRasterSpace(cameraToNDC(vertexCamera), imageWidth, imageHeight);

}

TOOLS(Vector3) ProjectionCamera::worldToCamera(const TOOLS(Vector3)& vertexWorld) const
{
    return __currentWorldToCamera * vertexWorld;    
}

TOOLS(Vector3) ProjectionCamera::ndcToRaster(const TOOLS(Vector3)& vertexNDC, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    return toRasterSpace(vertexNDC, imageWidth, imageHeight);
}

TOOLS(Vector3) ProjectionCamera::cameraToRaster(const TOOLS(Vector3)& vertexCamera, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    return toRasterSpace(cameraToNDC(vertexCamera), imageWidth, imageHeight);
}

TOOLS(Vector3) ProjectionCamera::cameraToWorld(const TOOLS(Vector3)& vertexCamera) const
{
    return __cameraToWorld * vertexCamera;    
}

TOOLS(Vector3) ProjectionCamera::rasterToWorld(const TOOLS(Vector3)& raster, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    Vector3 vertexNDC = rasterToNDC(raster, imageWidth, imageHeight);
    return cameraToWorld(NDC2screen(vertexNDC.x(), vertexNDC.y(), vertexNDC.z()));

}

BoundingBoxPtr ProjectionCamera::getBoundingBoxView() const
{
    return BoundingBoxPtr(new BoundingBox(Vector3(left,bottom,near),Vector3(right,top,far)));    
}


bool ProjectionCamera::isInZRange(real_t z) const {
    return (z >= near && z <= far);
}

bool ProjectionCamera::isInZRange(real_t zmin, real_t zmax) const{
    return !(zmax < near || zmin > far);
}

void ProjectionCamera::transformModel(const TOOLS(Matrix4)& transform)
{
    __currentModelMatrix *= transform;
    __currentWorldToCamera = __worldToCamera * __currentModelMatrix;
}

void ProjectionCamera::pushModelTransformation()
{
    __modelMatrixStack.push(__currentModelMatrix);
}

void ProjectionCamera::popModelTransformation()
{
    if (!__modelMatrixStack.empty()){
        __currentModelMatrix = __modelMatrixStack.top();
        __currentWorldToCamera = __worldToCamera * __currentModelMatrix;    
        __modelMatrixStack.pop();
    }

}

void ProjectionCamera::transformModelIdentity()
{
    __currentModelMatrix = Matrix4();
    __currentWorldToCamera = __worldToCamera * __currentModelMatrix;    
}

void ProjectionCamera::translateModel(const Vector3& v)
{
    transformModel(Matrix4::translation(v));
}

void ProjectionCamera::scaleModel(const Vector3& v)
{
    transformModel(Matrix4(Matrix3::scaling(v)));
}


ProjectionCameraPtr ProjectionCamera::perspectiveCamera(real_t angleOfView, real_t aspectRatio, real_t near, real_t far)
{ 
    real_t top = near * tan(angleOfView * GEOM_RAD/2.);
    real_t bottom = -top;
    real_t right = top * aspectRatio;
    real_t left = -right;
    return ProjectionCamera::frustumCamera(left, right, bottom, top, near, far);
}


ProjectionCameraPtr ProjectionCamera::frustumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
{
    return ProjectionCameraPtr(new ProjectionCamera(left, right, bottom, top, near, far, ProjectionCamera::ePerspective));
}

ProjectionCameraPtr ProjectionCamera::orthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
{
    return ProjectionCameraPtr(new ProjectionCamera(left, right, bottom, top, near, far, ProjectionCamera::eOrthographic));
}

