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
             


/* ----------------------------------------------------------------------- */

#include "projectioncamera.h"
#include <plantgl/math/util_math.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE



ProjectionCamera::ProjectionCamera(real_t _near, real_t _far, eProjectionType _type, eProjectionMethodType methodtype):
        RefCountObject(),
        __near(_near),
        __far(_far),
        __type(_type),
        __methodtype(methodtype),
        __direction(1,0,0),
        __up(0,0,1){
}

ProjectionCamera::~ProjectionCamera() {}

void ProjectionCamera::lookAt(const Vector3& eyePosition3D, const Vector3& center3D, const Vector3& upVector3D)
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
        __direction = forward;
        __up = up;
        Matrix4 cameraToWorld(side, up, forward, eyePosition3D);
        __worldToCamera = inverse(cameraToWorld);
        __currentWorldToCamera = __worldToCamera * __currentModelMatrix;

}

// ProjectionCameraPtr ProjectionCamera::copy() { return ProjectionCameraPtr(new ProjectionCamera(*this));}


Vector3 ProjectionCamera::worldToRaster(const Vector3& vertexWorld, const uint16_t imageWidth, const uint16_t imageHeight) const
{

    Vector3 vertexCamera = __worldToCamera * vertexWorld;
    return NDCtoRasterSpace(cameraToNDC(vertexCamera), imageWidth, imageHeight);

}

Vector3 ProjectionCamera::worldToCamera(const Vector3& vertexWorld) const
{
    return __currentWorldToCamera * vertexWorld;    
}

Vector3 ProjectionCamera::cameraToRaster(const Vector3& vertexCamera, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    return NDCtoRasterSpace(cameraToNDC(vertexCamera), imageWidth, imageHeight);
}

Vector3 ProjectionCamera::rasterToCamera(const Vector3& raster, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    Vector3 vertexNDC = rasterToNDC(raster, imageWidth, imageHeight);
    return NDCToCamera(vertexNDC);

}

Vector3 ProjectionCamera::cameraToWorld(const Vector3& vertexCamera) const
{
    return inverse(__currentWorldToCamera) * vertexCamera;    
}

Vector3 ProjectionCamera::rasterToWorld(const Vector3& raster, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    return cameraToWorld(rasterToCamera(raster, imageWidth, imageHeight));

}


Ray ProjectionCamera::rasterToCameraRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const
{ 
    Vector3 directionStart = rasterToCamera(Vector3(x,y,0), imageWidth, imageHeight);
    Vector3 directionEnd = rasterToCamera(Vector3(x,y,1), imageWidth, imageHeight);
    return Ray(directionStart, (directionEnd-directionStart).normed());
}

Ray ProjectionCamera::rasterToWorldRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    Vector3 directionStart = rasterToWorld(Vector3(x,y,0), imageWidth, imageHeight);
    Vector3 directionEnd = rasterToWorld(Vector3(x,y,1), imageWidth, imageHeight);
    return Ray(directionStart, (directionEnd-directionStart).normed());
}


bool ProjectionCamera::isInZRange(real_t z) const {
    return ((z-__near) >= -GEOM_EPSILON && (z-__far) <= GEOM_EPSILON);
}

bool ProjectionCamera::isInZRange(real_t zmin, real_t zmax) const{
    return !(zmax < __near || zmin > __far);
}

void ProjectionCamera::transformModel(const Matrix4& transform)
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
/* ----------------------------------------------------------------------- */

real_t ProjectionCamera::projectedArea(uint16_t x, uint16_t y, real_t z, const uint16_t imageWidth, const uint16_t imageHeight)
{
    Vector3 dir1 = rasterToCamera(Vector3(x,y,1), imageWidth, imageHeight);
    Vector3 dir2 = rasterToCamera(Vector3(x+1,y,1), imageWidth, imageHeight);
    Vector3 dir3 = rasterToCamera(Vector3(x,y+1,1), imageWidth, imageHeight);
    real_t alpha = norm(Vector3(dir2)-Vector3(dir1));
    real_t beta = norm(Vector3(dir3)-Vector3(dir1));
    return alpha*beta;

}

/* ----------------------------------------------------------------------- */

real_t ProjectionCamera::solidAngle(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight)
{
    // This assumes that the camera has only one central point
    Vector3 dir1 = rasterToCamera(Vector3(x,y,1), imageWidth, imageHeight);
    Vector3 dir2 = rasterToCamera(Vector3(x+1,y,1), imageWidth, imageHeight);
    Vector3 dir3 = rasterToCamera(Vector3(x,y+1,1), imageWidth, imageHeight);
    real_t alpha = norm(Vector3(dir2)-Vector3(dir1));
    real_t beta = norm(Vector3(dir3)-Vector3(dir1));
    return 4 * atan2(alpha*beta, 2*sqrt(4+alpha*alpha+beta*beta));
}

/* ----------------------------------------------------------------------- */

ProjectionCameraPtr ProjectionCamera::perspectiveCamera(real_t verticalAngleOfView, real_t aspectRatio, real_t near, real_t far)
{ 
    return ProjectionCameraPtr(new PerspectiveCamera(verticalAngleOfView, aspectRatio, near, far));
}


ProjectionCameraPtr ProjectionCamera::frustumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
{
    return ProjectionCameraPtr(new PerspectiveCamera(left, right, bottom, top, near, far));
}

ProjectionCameraPtr ProjectionCamera::orthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
{
    return ProjectionCameraPtr(new OrthographicCamera(left, right, bottom, top, near, far));
}

ProjectionCameraPtr ProjectionCamera::hemisphericCamera(real_t near, real_t far)
{
    return ProjectionCameraPtr(new SphericalCamera(180, near, far));
}

ProjectionCameraPtr ProjectionCamera::sphericalCamera(real_t viewAngle, real_t near, real_t far)
{
    return ProjectionCameraPtr(new SphericalCamera(viewAngle, near, far));
}

ProjectionCameraPtr ProjectionCamera::equirectangularCamera(real_t viewAngle, real_t near, real_t far)
{
    return ProjectionCameraPtr(new EquirectangularCamera(viewAngle, near, far));
}

ProjectionCameraPtr ProjectionCamera::cylindricalCamera(real_t viewAngle, real_t bottom, real_t top, real_t near, real_t far)
{
    return ProjectionCameraPtr(new CylindricalCamera(viewAngle, bottom, top, near, far));
}

/* ----------------------------------------------------------------------- */

FrustumCamera::FrustumCamera(real_t _left, real_t _right, real_t _bottom, real_t _top, real_t _near, real_t _far, eProjectionType type):
        ProjectionCamera(_near, _far, type, eProjection){
        setFrustum(_left, _right, _bottom, _top);
}

FrustumCamera::~FrustumCamera() {}

// ProjectionCameraPtr FrustumCamera::copy() { return ProjectionCameraPtr(new FrustumCamera(*this));}

void FrustumCamera::setFrustum(real_t _left, real_t _right, real_t _bottom, real_t _top){
        left = _left;
        right = _right;
        bottom = _bottom;
        top = _top;  
        __xscale = 2. / (_right - _left) ;
        __xconstant = (_right + _left) / (_right - _left);
        __yscale = 2. / (_top - _bottom) ;
        __yconstant = (_top + _bottom) / (_top - _bottom);
}


Vector3 FrustumCamera::screen2NDC(const real_t& xScreen, const real_t& yScreen, const real_t z) const {
    // now convert point from screen space to NDC space (in range [-1,1])
    Vector3 vertexNDC(__xscale * xScreen  - __xconstant, __yscale * yScreen  - __yconstant,  -z);

    return vertexNDC;
}

Vector3 FrustumCamera::NDC2screen(const real_t& xNDC, const real_t& yNDC, const real_t z) const {
    // now convert point from NDC space to screen space 
    Vector3 vertexScreen((xNDC  + __xconstant) / __xscale,  (yNDC  + __yconstant) / __yscale,  -z);
    return vertexScreen;
}

BoundingBoxPtr FrustumCamera::getBoundingBoxView() const
{
    return BoundingBoxPtr(new BoundingBox(Vector3(left,bottom,__near),Vector3(right,top,__far)));    
}

/* ----------------------------------------------------------------------- */

PerspectiveCamera::PerspectiveCamera(real_t _left, real_t _right, real_t _bottom, real_t _top, real_t _near, real_t _far):
        FrustumCamera(_left, _right, _bottom, _top, _near, _far, ePerspective)
{
}

PerspectiveCamera::PerspectiveCamera(real_t verticalAngleOfView, real_t aspectRatio, real_t _near, real_t _far):
        FrustumCamera(-1, 1, -1, 1, _near, _far, ePerspective)
{   
    real_t top = _near * tan(verticalAngleOfView * GEOM_RAD/2.);
    real_t bottom = -top;
    real_t right = top * aspectRatio;
    real_t left = -right;
    setFrustum(left, right, bottom, top);
}

PerspectiveCamera::~PerspectiveCamera() {}

ProjectionCameraPtr PerspectiveCamera::copy() { return ProjectionCameraPtr(new PerspectiveCamera(*this));}

Vector3 PerspectiveCamera::cameraToNDC(const Vector3& vertexCamera) const {
    real_t z = -vertexCamera.z();
    if (z < 0) { return screen2NDC(0, 0, vertexCamera.z()); }
    return screen2NDC(__near * vertexCamera.x() / z, __near * vertexCamera.y() / z, vertexCamera.z());        
}

Vector3 PerspectiveCamera::NDCToCamera(const Vector3& vertexNDC) const {
    real_t z = vertexNDC.z();
    return NDC2screen(vertexNDC.x() * z / __near, vertexNDC.y() * z / __near, vertexNDC.z());
}

Ray PerspectiveCamera::rasterToCameraRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    Vector3 direction = rasterToCamera(Vector3(x,y,1), imageWidth, imageHeight);
    return Ray(Vector3(0,0,0), direction.normed());

}

Ray PerspectiveCamera::rasterToWorldRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    Vector3 direction = rasterToWorld(Vector3(x,y,1), imageWidth, imageHeight);
    return Ray(__position, direction.normed());
}

/*
bool fitViewTo(const BoundingBoxPtr bbx, uint16_t margin)
{

}
*/
/* ----------------------------------------------------------------------- */

OrthographicCamera::OrthographicCamera(real_t _left, real_t _right, real_t _bottom, real_t _top, real_t _near, real_t _far):
        FrustumCamera(_left, _right, _bottom, _top, _near, _far, eOrthographic)
{
    __methodtype = eRayIntersection;
}

OrthographicCamera::~OrthographicCamera() {}

ProjectionCameraPtr OrthographicCamera::copy() { return ProjectionCameraPtr(new OrthographicCamera(*this));}

Vector3 OrthographicCamera::cameraToNDC(const Vector3& vertexCamera) const {
    return screen2NDC(vertexCamera.x(), vertexCamera.y(), vertexCamera.z());
}

Vector3 OrthographicCamera::NDCToCamera(const Vector3& vertexNDC) const {
    return NDC2screen(vertexNDC.x(), vertexNDC.y(), vertexNDC.z());
}

/* ----------------------------------------------------------------------- */

SphericalCamera::SphericalCamera(real_t viewAngle, real_t _near, real_t _far):
        ProjectionCamera(_near, _far, eHemispheric, eRayIntersection),
        __viewAngle(viewAngle*GEOM_RAD/2){
}

SphericalCamera::~SphericalCamera() {}

ProjectionCameraPtr SphericalCamera::copy() { return ProjectionCameraPtr(new SphericalCamera(*this));}


Vector3::Spherical SphericalCamera::NDCToSpherical(const Vector3& vertexNDC) const
{
    real_t x = vertexNDC.x();
    real_t y = vertexNDC.y();

    real_t rho = std::sqrt(x*x + y*y);
    real_t theta = std::atan2(y, x);
    real_t phi = rho * __viewAngle;   // projection équidistante

    return Vector3::Spherical(vertexNDC.z(), theta, phi);
}


Vector3 SphericalCamera::SphericalToNDC(const Vector3::Spherical& sph) const
{
    real_t rho = sph.phi / __viewAngle;
    real_t x = rho * std::cos(sph.theta);
    real_t y = rho * std::sin(sph.theta);

    return Vector3(x, y, sph.radius);
}


Vector3 SphericalCamera::cameraToNDC(const Vector3& vertexCamera) const {
    return SphericalToNDC(Vector3::Spherical(Vector3(vertexCamera.x(),vertexCamera.y(),-vertexCamera.z())));
}

Vector3 SphericalCamera::NDCToCamera(const Vector3& vertexNDC) const {
    Vector3 vertexScreen(NDCToSpherical(vertexNDC));

    return Vector3(vertexScreen.x(), vertexScreen.y(), -vertexScreen.z());
}

real_t SphericalCamera::getViewAngle() const 
{
    return __viewAngle*GEOM_DEG*2;
}  

void SphericalCamera::setViewAngle(real_t angle)
{
    __viewAngle = angle*GEOM_RAD/2;
}

Ray SphericalCamera::rasterToCameraRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    Vector3 direction = rasterToCamera(Vector3(x,y,1), imageWidth, imageHeight);
    return Ray(Vector3(0,0,0), direction.normed());

}

Ray SphericalCamera::rasterToWorldRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    Vector3 direction = rasterToWorld(Vector3(x,y,1), imageWidth, imageHeight);
    return Ray(__position, direction.normed());
}

bool SphericalCamera::isValidPixel(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const {
    float ndc_x, ndc_y;

    // 1) tester le centre (rapide)
    Vector3 ndc = rasterToNDC(Vector3(x + 0.5f, y + 0.5f, 1), imageWidth, imageHeight);
    if (ndc.x()*ndc.x() + ndc.y()*ndc.y() <= 1.0f)
        return true;

    // 2) tester les coins
    for (int dx = 0; dx <= 1; ++dx)
        for (int dy = 0; dy <= 1; ++dy)
        {
            Vector3 ndc = rasterToNDC(Vector3(x + dx, y + dy, 1), imageWidth, imageHeight);
            if (ndc.x()*ndc.x() + ndc.y()*ndc.y() <= 1.0f)
                return true;
        }

    return false;

//     return (norm(Vector2(x-imageWidth/2,y-imageHeight/2)) <= pglMin(imageWidth,imageHeight)/2);
}

/* ----------------------------------------------------------------------- */
EquirectangularCamera::EquirectangularCamera(real_t viewAngle, real_t _near, real_t _far):
        SphericalCamera(viewAngle, _near, _far){
            __type = eEquirectangular;
            __viewAngle = viewAngle*GEOM_RAD/2;
}


EquirectangularCamera::~EquirectangularCamera() {}

ProjectionCameraPtr EquirectangularCamera::copy() { return ProjectionCameraPtr(new EquirectangularCamera(*this));}

Vector3::Spherical EquirectangularCamera::NDCToSpherical(const Vector3& ndc) const
{
    
    // Horizontal : plein 360° (classique en equirectangular)
    real_t theta = ndc.x() * M_PI;    // [-1,1] → [-π,π]

    // Vertical : limité par viewAngle
    // φ = 0 (centre vers +Z) quand ndc.y = +1
    // φ = viewAngle quand ndc.y = -1
    real_t phi = ((real_t)1.0 - ndc.y()) * (__viewAngle * real_t(0.5));

    return Vector3::Spherical(ndc.z(), theta, phi);
}

Vector3 EquirectangularCamera::SphericalToNDC(const Vector3::Spherical& sph) const
{
    // Horizontal : 360°
    real_t ndc_x = sph.theta / M_PI;   // [-π,π] → [-1,1]

    // Vertical inverse
    real_t ndc_y = 1.0 - (2.0 * sph.phi / __viewAngle);

    return Vector3(ndc_x, ndc_y, sph.radius);
}

/* ----------------------------------------------------------------------- */

CylindricalCamera::CylindricalCamera(real_t viewAngle, real_t _bottom, real_t _top, real_t _near, real_t _far):
        ProjectionCamera(_near, _far, eCylindrical, eRayIntersection),
        top(_top), bottom(_bottom),
        __viewAngle(viewAngle*GEOM_RAD/2){
        __yscale = 2. / (_top - _bottom) ;
        __yconstant = (_top + _bottom) / (_top - _bottom);
}

CylindricalCamera::~CylindricalCamera() {}

ProjectionCameraPtr CylindricalCamera::copy() { return ProjectionCameraPtr(new CylindricalCamera(*this));}


Vector3 CylindricalCamera::cameraToNDC(const Vector3& vertexCamera) const {
    Vector2::Polar cyl(Vector2(vertexCamera.x(),-vertexCamera.z()));
    real_t normedtheta = (GEOM_HALF_PI - cyl.theta)/__viewAngle;

    // return Vector3(vertexCamera.x(), __yscale * vertexCamera.y()  - __yconstant,  -vertexCamera.z());
    return Vector3(normedtheta, __yscale * vertexCamera.y()  - __yconstant,  cyl.radius);

}

Vector3 CylindricalCamera::NDCToCamera(const Vector3& vertexNDC) const {
    // return Vector3(vertexNDC.x(), __yscale * vertexNDC.y()  - __yconstant,  -vertexNDC.z());

    real_t theta = (GEOM_HALF_PI - vertexNDC.x() * __viewAngle);
    real_t radius = vertexNDC.z();
    Vector2 vertexScreen(Vector2::Polar(radius, theta));

    real_t y = (vertexNDC.y()  + __yconstant) / __yscale;

    return Vector3(vertexScreen.x(), y, -vertexScreen.y());

}

real_t CylindricalCamera::getViewAngle() const 
{
    return __viewAngle*GEOM_DEG*2;
}  

void CylindricalCamera::setViewAngle(real_t angle)
{
    __viewAngle = angle*GEOM_RAD/2;
}

Ray CylindricalCamera::rasterToCameraRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const
{
    Vector3 direction = rasterToCamera(Vector3(x,y,1), imageWidth, imageHeight);
    return Ray(Vector3(0,direction.y(),0), direction.normed());

}

