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

#include "zbufferengine.h"
#include "projectionrenderer.h"
#include "projection_util.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <chrono>
/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

ZBufferEngine::ZBufferEngine(uint16_t imageWidth, 
                             uint16_t imageHeight,
                             eRenderingStyle style,
                             const Color3& backGroundColor,
                             uint32_t defaultid,
                             bool multithreaded,
                             eFaceCulling culling,
                             eIdPolicy idPolicy ):
    ImageProjectionEngine(imageWidth,imageHeight,idPolicy),
    __light(new Light()),
    __style(style),
    __defaultid(defaultid),
    __alphathreshold(0.99),
    __depthBuffer(new RealArray2(uint_t(imageWidth), uint_t(imageHeight), REAL_MAX)),
    __frameBuffer((style & eColorBased) ? new PglFrameBufferManager(imageWidth, imageHeight, 3, backGroundColor) : NULL),
    __idBuffer((style & eIdBased) ? new Uint32Array2(uint_t(imageWidth), uint_t(imageHeight), defaultid) : NULL),
    __orientationBuffer((style & eOrientationBased) ? new BoolArray2(uint_t(imageWidth), uint_t(imageHeight), defaultid) : NULL),
    __imageMutex(),
    __triangleshader((style != eDepthOnly) ? new TriangleShaderSelector(this) : NULL),
    __triangleshaderset(NULL),
    __multithreaded(multithreaded),
    __culling(culling)
{
    beginProcess();
}    

    
ZBufferEngine::~ZBufferEngine()
{
}

void ZBufferEngine::lock(uint_t x, uint_t y)
{
    if(__multithreaded) __imageMutex->lock(x,y);
}

void ZBufferEngine::unlock(uint_t x, uint_t y)
{
    if(__multithreaded) __imageMutex->unlock(x,y);    
}

bool ZBufferEngine::tryLock(uint_t x, uint_t y)
{
    if(__multithreaded) return __imageMutex->tryLock(x,y);
    return true;   
}

void ZBufferEngine::beginProcess()
{

    if (__multithreaded && is_null_ptr(__imageMutex)){
        // printf("begin rendering : create thread pool\n");
        __imageMutex = getImageMutex(__imageWidth, __imageHeight);
    }

}

void ZBufferEngine::endProcess()
{

    if(__multithreaded){
        ThreadManager::get().join();
    }
}

void ZBufferEngine::setHemisphericCamera(real_t nearValue, real_t farValue)
{  
    setSphericalCamera(180, nearValue, farValue);
}

void ZBufferEngine::setSphericalCamera(real_t viewAngle, real_t nearValue, real_t farValue)
{  
    __camera = ProjectionCamera::sphericalCamera(viewAngle, nearValue, farValue); 
    for (uint32_t x = 0; x < __imageWidth; ++x){
        for (uint32_t y = 0; y < __imageHeight; ++y){
            if (!__camera->isValidPixel(x,y,__imageWidth,__imageHeight)){
                if(__style & eColorBased) setFrameBufferAt(x,y,Color3::BLACK);
                __depthBuffer->setAt(x, y, nearValue);
            }
        }
    }
}
void ZBufferEngine::setEquirectangularCamera(real_t viewAngle, real_t nearValue, real_t farValue)
{  
    __camera = ProjectionCamera::equirectangularCamera(viewAngle, nearValue, farValue); 
}

void ZBufferEngine::setCylindricalCamera(real_t viewAngle, real_t bottom, real_t top, real_t nearValue, real_t farValue)
{  
    __camera = ProjectionCamera::cylindricalCamera(viewAngle, bottom, top, nearValue, farValue); 
}


ImagePtr ZBufferEngine::getImage() const
{
    PglFrameBufferManagerPtr fb = dynamic_pointer_cast<PglFrameBufferManager>(__frameBuffer);
    if (is_valid_ptr(fb)) { return fb->getImage(); }
    printf("No valid framebuffer\n");
    return ImagePtr();
}

ImagePtr ZBufferEngine::getIdBufferAsImage(Color4::eColor4Format conversionformat) const
{
    ImagePtr result(new Image(__imageWidth, __imageHeight, 4));

    for (uint_t x = 0 ; x < __imageWidth ; ++x)
        for (uint_t y = 0 ; y < __imageHeight ; ++y){
            result->setPixelAt(x,y,Color4::fromUint(__idBuffer->getAt(x,y), conversionformat));
        }

    return result;
}


bool ZBufferEngine::isTotallyTransparent(const real_t alpha) const
{
    return alpha >= __alphathreshold;
}


Color3 ZBufferEngine::getFrameBufferAt(uint32_t x, uint32_t y) const
{
    if(__frameBuffer) return __frameBuffer->getPixelAt(x,y);
    return Color3(0,0,0);
}

void ZBufferEngine::setFrameBufferAt(uint32_t x, uint32_t y, const Color3& rasterColor)
{
    if(__frameBuffer) __frameBuffer->setPixelAt(x,y,rasterColor);
}

void ZBufferEngine::setFrameBufferAt(uint32_t x, uint32_t y, const Color4& rasterColor)
{
    if (rasterColor.getAlpha() < GEOM_EPSILON )
        setFrameBufferAt(x,y, Color3(rasterColor));
    else if (!isTotallyTransparent(rasterColor.getAlpha())) {
        real_t alpha = rasterColor.getAlphaClamped();
        Color3 current = getFrameBufferAt(x,y);
        Color3 result = current * (1-alpha) + rasterColor * alpha;
        setFrameBufferAt(x,y, result);
    }
}

bool ZBufferEngine::isVisible(int32_t x, int32_t y, real_t z) const
{
    real_t cz = __depthBuffer->getAt(x, y);
    return (z < cz && (cz-z) > GEOM_EPSILON); 
}

bool ZBufferEngine::isVisible(const Vector3& pos) const
{
    Vector3 deltapos = pos - camera()->position();
    Vector3 direction = - camera()->direction();
    real_t hdistancetocamera = dot(deltapos, direction);
    real_t _near = camera()->get_near();

    if (hdistancetocamera < _near){
        // printf("Position (%f,%f,%f) is behind near plane : %f\n", pos.x(), pos.y(), pos.z(), (dot(pos-camera()->position(), -camera()->getWorldToCameraMatrix().getColumn(2)) - camera()->near));
        return false;
    }
    Vector3 raster = worldToRaster(pos);
    // printf("Position (%f,%f,%f) isVisible : (%i,%i), (%f,%f)  %f %f %s\n", pos.x(), pos.y(), pos.z(), int(raster.x()), int(raster.y()), raster.x(), raster.y(), raster.z(), __depthBuffer->getAt(raster.x(), raster.y()), (raster.z()< __depthBuffer->getAt(raster.x(), raster.y()) ? "True" : "False"));
    return isVisible(raster.x(), raster.y(), raster.z());
}


bool ZBufferEngine::renderRaster(uint32_t x, uint32_t y, real_t z, const Color4& rasterColor, const uint32_t id)
{
    if (isTotallyTransparent(rasterColor.getAlpha())) return false;

    lock(x,y);
    if (isVisible(x,y,z)) {
        __depthBuffer->setAt(x, y, z);
        if(__style & eColorBased){
            setFrameBufferAt(x, y, rasterColor);
        }
        if(__style & eIdBased){
            __idBuffer->setAt(x, y, id);
        }
        unlock(x,y);
        return true;
    }
    unlock(x,y);
    return false;
}

void ZBufferEngine::_tryRenderRaster(const struct Fragment& fragment, FragmentQueue& failqueue)
{
    if (!_tryRenderRaster(fragment.x, fragment.y, fragment.z, fragment.color, fragment.id, fragment.orientation))
        failqueue.push(fragment);
}

bool ZBufferEngine::_tryRenderRaster(uint32_t x, uint32_t y, real_t z, const Color4& rasterColor, const uint32_t id, const bool orientation)
{
    if (isTotallyTransparent(rasterColor.getAlpha())) return false;

    if(tryLock(x,y)){
        if (isVisible(x,y,z)) {
            __depthBuffer->setAt(x, y, z);
            if(__style & eColorBased){
                setFrameBufferAt(x, y, rasterColor);
            }
            if(__style & eIdBased){
                __idBuffer->setAt(x, y, id);
            }
            if(__style & eOrientationBased){
                __idBuffer->setAt(x, y, orientation);
            }
        }
        unlock(x,y);
        return true;
    }
    else return false;

}

void ZBufferEngine::setLight(const Vector3& lightPosition, const Color3& lightColor, bool directional)
{
    __light->set(lightPosition, lightColor, directional);
}

void ZBufferEngine::setLight(const Vector3& lightPosition, const Color3& lightAmbient, const Color3& lightDiffuse, const Color3& lightSpecular, bool directional)
{
    __light->set(lightPosition, lightAmbient, lightDiffuse, lightSpecular, directional);
}


void ZBufferEngine::iprocess(TriangleSetPtr triangles, AppearancePtr appearance, uint32_t id, ProjectionCameraPtr _camera, uint32_t threadid)
{
    // printf("process TriangleSetPtr\n");
    const Point3ArrayPtr points(triangles->getPointList());
    const Index3ArrayPtr indices(triangles->getIndexList());

    bool ccw = triangles->getCCW();

    triangles->checkNormalList();

    size_t nbfaces = triangles->getIndexListSize();
    bool hasColor = triangles->hasColorList();

    TriangleShaderPtr shader;
    if (getRenderingStyle() & eColorBased){
        if (threadid != 0) {
            assert(threadid <= ThreadManager::get().nb_threads());
            //printf("session %u\n", threadid);
            shader = __triangleshaderset[threadid-1];
        }
        else {
            shader = __triangleshader;
        }
    }

    // ProjectionCameraPtr camera = (__multithreaded?ProjectionCameraPtr(_camera->copy()):_camera);
    for(uint32_t itidx = 0; itidx < nbfaces; ++itidx){
        //printf("triangle %i on %i\n", itidx, nbfaces);

        const Vector3& v0 = triangles->getFacePointAt(itidx,0);
        const Vector3& v1 = triangles->getFacePointAt(itidx,1);
        const Vector3& v2 = triangles->getFacePointAt(itidx,2);

        if(is_valid_ptr(shader)){
            shader->init(appearance, triangles, itidx, _camera, __light);
        }
      
        // shader->initEnv(_camera, __light);
        renderShadedTriangle(v0, v1, v2, ccw, getNewPrimitiveId(id), shader, _camera);

    }
    // printf("end process TriangleSetPtr\n");

}


void ZBufferEngine::renderShadedTriangle(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1_, const TOOLS(Vector3)& v2_, bool ccw, const uint32_t id, const TriangleShaderPtr& shader,  const ProjectionCameraPtr& camera)
{
    Vector3 v1 = v1_;
    Vector3 v2 = v2_;
    if (ccw == false) {
        v1 = v2_;
        v2 = v1_;
    }

     // Projection in camera space
    Vector3 v0Cam = camera->worldToCamera(v0);
    Vector3 v1Cam = camera->worldToCamera(v1);
    Vector3 v2Cam = camera->worldToCamera(v2);

    // Convert the vertices of the triangle to raster space
    Vector3 v0Raster = camera->cameraToRaster(v0Cam,__imageWidth, __imageHeight);
    Vector3 v1Raster = camera->cameraToRaster(v1Cam,__imageWidth, __imageHeight);
    Vector3 v2Raster = camera->cameraToRaster(v2Cam,__imageWidth, __imageHeight);

    /*printf("rasterize t point [%f,%f,%f] \n",v0Raster.x(),v0Raster.y(),v0Raster.z());
    printf("rasterize t point [%f,%f,%f] \n",v1Raster.x(),v1Raster.y(),v1Raster.z());
    printf("rasterize t point [%f,%f,%f] \n",v2Raster.x(),v2Raster.y(),v2Raster.z());*/

    v0Raster.x() = std::round(v0Raster.x());
    v0Raster.y() = std::round(v0Raster.y());
    v1Raster.x() = std::round(v1Raster.x());
    v1Raster.y() = std::round(v1Raster.y());
    v2Raster.x() = std::round(v2Raster.x());
    v2Raster.y() = std::round(v2Raster.y());

    real_t xmin, ymin, zmin;
    real_t xmax, ymax, zmax;

    xmin = min3(v0Raster.x(), v1Raster.x(), v2Raster.x());
    ymin = min3(v0Raster.y(), v1Raster.y(), v2Raster.y());

    xmax = max3(v0Raster.x(), v1Raster.x(), v2Raster.x());
    ymax = max3(v0Raster.y(), v1Raster.y(), v2Raster.y());

    zmin = min3(v0Raster.z(), v1Raster.z(), v2Raster.z());
    zmax = max3(v0Raster.z(), v1Raster.z(), v2Raster.z());

    if(camera->methodType() == ProjectionCamera::eRayIntersection && (std::floor(xmax)-std::floor(xmin)>1) && (std::floor(ymax)-std::floor(ymin)>1)) {
        std::pair<Vector3, real_t> csphere = circumsphere(v0Raster,v1Raster,v2Raster);
        Vector3 vMeanRaster = csphere.first;
        real_t radius = csphere.second;

        xmin = vMeanRaster.x() - radius;
        ymin = vMeanRaster.y() - radius;

        xmax = vMeanRaster.x() + radius;
        ymax = vMeanRaster.y() + radius;

        if (zmin < 0 && zmax > 0) {
            if (xmax < __imageWidth/2){ xmin = 0;}
            else if  (xmin > __imageWidth/2){ xmax = __imageWidth;}
        }

    }

      // the triangle is out of screen
    if (xmin >= __imageWidth  || xmax < 0 || ymin >= __imageHeight || ymax < 0 || !camera->isInZRange(zmin, zmax)) {
        return;
    }

    // be careful xmin/xmax/ymin/ymax can be negative. Don't cast to uint32_t
    int32_t x0 = pglMax(int32_t(0), (int32_t)(std::floor(xmin)));
    int32_t x1 = pglMin(int32_t(__imageWidth) - 1, (int32_t)(std::ceil(xmax)));
    int32_t y0 = pglMax(int32_t(0), (int32_t)(std::floor(ymin)));
    int32_t y1 = pglMin(int32_t(__imageHeight) - 1, (int32_t)(std::ceil(ymax)));

    if (__multithreaded && (x1-x0+1)*(y1-y0+1) > 20) {
        std::tuple<Vector3,Vector3,Vector3> vRasters(v0Raster, v1Raster, v2Raster);
        std::tuple<Vector3,Vector3,Vector3> vCams(v0Cam,v1Cam,v2Cam);

        ThreadManager::get().new_task(boost::bind(&ZBufferEngine::rasterizeMT, this, Index4(x0,x1,y0,y1), 
                                                  // v0Raster, v1Raster, v2Raster, v0Cam,v1Cam,v2Cam,
                                                  vRasters, vCams, id, 
                                                  (getRenderingStyle() & eColorBased) ? TriangleShaderPtr(shader->copy()) : shader, ProjectionCameraPtr(camera->copy())));
    }
    else {
        rasterize(x0,x1,y0,y1,v0Raster,v1Raster,v2Raster,v0Cam,v1Cam,v2Cam,id,shader,camera);
    }
}



void ZBufferEngine::rasterizeMT(const Index4& rect,
                                const std::tuple<Vector3,Vector3,Vector3>& vRasters, 
                                const std::tuple<Vector3,Vector3,Vector3>& vCams,
                                const uint32_t id, 
                                const TriangleShaderPtr& shader, const ProjectionCameraPtr& camera)
{
    rasterize(rect[0],rect[1],rect[2],rect[3],
              std::get<0>(vRasters),std::get<1>(vRasters),std::get<2>(vRasters),
              std::get<0>(vCams),std::get<1>(vCams),std::get<2>(vCams),
             // v1Raster,v2Raster,v0Cam,v1Cam,v2Cam,
              id,shader,camera);  
}

void findorder(real_t * zs, int& firstpoint, int& secpoint, int& thirdpoint){
    firstpoint = 0; secpoint = 1; thirdpoint = 2;
    if (zs[firstpoint] < zs[secpoint]) std::swap(firstpoint, secpoint);
    if (zs[secpoint] < zs[thirdpoint]) std::swap(thirdpoint, secpoint);
    if (zs[firstpoint] < zs[secpoint]) std::swap(firstpoint, secpoint);
}

ZBufferEngine::eFaceIntersection ZBufferEngine::intersectionSignTest(real_t w0, real_t w1, real_t w2) const {
    if (w0 > -GEOM_EPSILON && w1 > -GEOM_EPSILON && w2 > -GEOM_EPSILON) return eFrontFaceIntersection;
    else if (w0 < GEOM_EPSILON && w1 < GEOM_EPSILON && w2 < GEOM_EPSILON) return eBackFaceIntersection;
    else return eNoIntersection;
}

ZBufferEngine::eFaceIntersection ZBufferEngine::culling(ZBufferEngine::eFaceIntersection intersection) const {
    if (__culling == eBothFaceCulling)
        return eNoIntersection;
    else if (__culling == eFrontFaceCulling && intersection == eFrontFaceIntersection)
        return eNoIntersection;
    else if (__culling == eBackFaceCulling && intersection == eBackFaceIntersection)
        return eNoIntersection;
    return intersection;
}

void ZBufferEngine::rasterize(int32_t x0, int32_t x1, int32_t y0, int32_t y1,
                              TOOLS(Vector3) v0Raster, TOOLS(Vector3) v1Raster, TOOLS(Vector3) v2Raster, 
                              TOOLS(Vector3) v0Cam, TOOLS(Vector3) v1Cam, TOOLS(Vector3) v2Cam, const uint32_t id, 
                              const TriangleShaderPtr& shader, const ProjectionCameraPtr& camera)
{

    if (__culling == ZBufferEngine::eBothFaceCulling) return;

    real_t z0 = v0Raster.z();
    real_t z1 = v1Raster.z();
    real_t z2 = v2Raster.z();

    Vector3 normal = cross(v1Cam-v0Cam, v2Cam-v0Cam);

    if (camera->methodType() == ProjectionCamera::eProjection){
        bool orientation = normal.z() > -GEOM_EPSILON;
        if ( __culling == eFrontFaceCulling && orientation) return;
        else if ( __culling == eBackFaceCulling && !orientation) return;
    }

    // Precompute area of triangle
    real_t area;
    if (camera->methodType() == ProjectionCamera::eRayIntersection){
        area = norm(normal);
    }
    else {
        v0Raster.z() = 1. / v0Raster.z();
        v1Raster.z() = 1. / v1Raster.z();
        v2Raster.z() = 1. / v2Raster.z();

        area = edgeFunction(v0Raster, v1Raster, v2Raster);
    }

    FragmentQueue fragqueue;

    // Inner loop
    if ((x0 == x1) && (y0 == y1)) {
        if (camera->isValidPixel(x0,y0,__imageWidth, __imageHeight)){
            real_t z = (z0+z1+z2)/3;
            real_t w0 = 1/3.;
            _tryRenderRaster(Fragment(x0,y0,z, is_valid_ptr(shader) ? shader->process(x0, y0, z, (w0 * z / z0), (w0 * z / z1), (w0 * z / z2)) : Color4::BLACK, id, true), fragqueue );
        }
    }
    else {
        for (int32_t y = y0; y <= y1; ++y) {
            for (int32_t x = x0; x <= x1; ++x) {

                Vector2 pixelSample(x + 0.5, y + 0.5);
                if (camera->methodType() == ProjectionCamera::eProjection){

                    // find weight of pixel
                    real_t w0 = edgeFunction(v1Raster, v2Raster, pixelSample);
                    real_t w1 = edgeFunction(v2Raster, v0Raster, pixelSample);
                    real_t w2 = edgeFunction(v0Raster, v1Raster, pixelSample);

                    // printf("%i %i : %f %f %f\n", x,y,w0,w1,w2);

                    eFaceIntersection intersection = culling(intersectionSignTest(w0,w1,w2));

                    if (intersection != eNoIntersection){
                        w0 /= area;
                        w1 /= area;
                        w2 /= area;

                        real_t oneOverZ = v0Raster.z() * w0 + v1Raster.z() * w1 + v2Raster.z() * w2;
                        real_t z = 1. / oneOverZ;

                        // Depth-buffer test
                        if (camera->isInZRange(z)){
                            _tryRenderRaster(Fragment(x,y,z, is_valid_ptr(shader) ? shader->process(x, y, z, (w0 * z / z0), (w1 * z / z1), (w2 * z / z2)) : Color4::BLACK, id, intersection == eFrontFaceIntersection), fragqueue );
                        }
                    }
                }
                else {
                    if (camera->isValidPixel(pixelSample.x(),pixelSample.y(),__imageWidth, __imageHeight)){
                        Vector3 intersection;
                        Ray ray = __camera->rasterToCameraRay(pixelSample.x(), pixelSample.y(), __imageWidth, __imageHeight);
                        int intersected = ray.intersect(v0Cam, v1Cam, v2Cam, intersection);

                        bool orientation = dot(normal, ray.getDirection()) < GEOM_EPSILON;
                        if ( __culling == eFrontFaceCulling && orientation) continue;
                        else if ( __culling == eBackFaceCulling && !orientation) continue;

                        if (intersected == 1){
                            real_t w0 = norm(cross(v1Cam-v2Cam, v1Cam-intersection));
                            real_t w1 = norm(cross(v2Cam-v0Cam, v2Cam-intersection));
                            real_t w2 = norm(cross(v0Cam-v1Cam, v0Cam-intersection));
                            real_t z = norm(intersection);
                            _tryRenderRaster(Fragment(x,y,z, is_valid_ptr(shader) ? shader->process(x, y, z, (w0 / area), (w1 / area), (w2 / area)) : Color4::BLACK, id, orientation), fragqueue );
                        }
                    }
                }
            }
        }
    }
    while(!fragqueue.empty()) {
        Fragment f = fragqueue.front();
        fragqueue.pop();
        _tryRenderRaster(f, fragqueue );
    }
}



void ZBufferEngine::renderTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, 
                                     const Color4& c0,  const Color4& c1,  const Color4& c2, 
                                     bool ccw, const uint32_t id, ProjectionCameraPtr camera)
{
    if(is_null_ptr(camera)) camera = __camera;
    ColorBasedShader * shader = new ColorBasedShader(this);
    shader->setColors(c0, c1, c2);
    TriangleShaderPtr shaderptr(shader);
    renderShadedTriangle(v0, v1, v2, ccw, id, shaderptr, camera);
}


void ZBufferEngine::renderPoint(const TOOLS(Vector3)& v, const Color4& c, const uint32_t width, const uint32_t id, ProjectionCameraPtr camera)
{
    
    if(is_null_ptr(camera)) camera = __camera;

    Vector3 vCam = camera->worldToCamera(v);

    // Convert the vertices of the triangle to raster space
    Vector3 vRaster = camera->cameraToRaster(vCam,__imageWidth, __imageHeight);

    // printf("rasterize  point [%f,%f,%f] \n",vRaster.x(),vRaster.y(),vRaster.z());
    
    // v0Raster.z() = 1. / v0Raster.z();

    // the point is out of screen
    if (!camera->isValidPixel(vRaster.x(),vRaster.y(),__imageWidth, __imageHeight)||  
        !camera->isInZRange(vRaster.z())) {
        return;
    }

    int32_t halfwidth = width/2.;
    int32_t x0 = pglMax<int32_t>(int32_t(0), (int32_t)(vRaster.x()) - halfwidth );
    int32_t x1 = pglMin<int32_t>(int32_t(__imageWidth) - 1, (int32_t)(vRaster.x()) + halfwidth );
    int32_t y0 = pglMax<int32_t>(int32_t(0), (int32_t)(vRaster.y()) - halfwidth );
    int32_t y1 = pglMin<int32_t>(int32_t(__imageHeight) - 1, (int32_t)(vRaster.y()) + halfwidth );

    for (int32_t y = y0; y <= y1; ++y) {
        for (int32_t x = x0; x <= x1; ++x) {
            if (camera->isValidPixel(x,y,__imageWidth, __imageHeight)){
                renderRaster(x, y, vRaster.z(), c, id);
            }
        }
    }
}

void ZBufferEngine::renderSegment(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const Color4& c0, const Color4& c1, const uint32_t width, const uint32_t id, ProjectionCameraPtr camera)
{
    if(is_null_ptr(camera)) camera = __camera;

    renderPoint(v0, c0, width, id, camera);
    renderPoint(v1, c1, width, id, camera);

    Vector3 v0Cam = camera->worldToCamera(v0);
    Vector3 v1Cam = camera->worldToCamera(v1);

    // Convert the vertices of the triangle to raster space
    Vector3 v0Raster = camera->cameraToRaster(v0Cam,__imageWidth, __imageHeight);
    Vector3 v1Raster = camera->cameraToRaster(v1Cam,__imageWidth, __imageHeight);

    if (fabs(v1Raster.y()-v0Raster.y()) < fabs(v1Raster.x()-v0Raster.x())){
        if(v0Raster.x() > v1Raster.x()){
            _renderSegment(0, v1Raster, v0Raster, c1, c0, width, id);
        }
        else{
            _renderSegment(0, v0Raster, v1Raster, c0, c1, width, id);
        }
    }
    else {
        if(v0Raster.y() > v1Raster.y()){
            _renderSegment(1, v1Raster, v0Raster, c1, c0, width, id);
        }
        else {
            _renderSegment(1, v0Raster, v1Raster, c0, c1, width, id);
        }
    }
}

void ZBufferEngine::_renderSegment(uchar dim, const TOOLS(Vector3)& v0Raster, const TOOLS(Vector3)& v1Raster, const Color4& c0, const Color4& c1, const uint32_t width, const uint32_t id)
{
    Vector3 dRaster = v1Raster - v0Raster;
    real_t totW = norm(dRaster);
    real_t Dz = dRaster.z()/dRaster[dim];
    uchar otherdim = (dim + 1) %2;
    uint32_t dims[2] = { __imageWidth , __imageHeight };

    real_t decal = 1;
    if (dRaster[otherdim] < 0){
        decal = -1;
        dRaster[otherdim] = -dRaster[otherdim];
    }

    real_t D = 2 * dRaster[otherdim] - dRaster[dim];
    Vector3 cRaster = v0Raster;

    int32_t halfwidth = width/2.;

    while (cRaster[dim] < v1Raster[dim]+1) {
        if ((0 <= cRaster[dim] && cRaster[dim] < dims[dim])&&
            (0 <= cRaster[otherdim] && cRaster[otherdim] < dims[otherdim])){
            real_t w = norm(cRaster - v0Raster) / totW;
            Color4 c = c0 * (1-w) + c1 * w;
            // printf("Render raster %f %f %f\n", cRaster.x(), cRaster.y(), cRaster.z());
            int32_t w0 = pglMax<int32_t>(int32_t(0), (int32_t)(cRaster[otherdim]) - halfwidth );
            int32_t w1 = pglMin<int32_t>(int32_t(dims[otherdim]) - 1, (int32_t)(cRaster[otherdim] + halfwidth ));
            Vector3 ciRaster = cRaster;
            for (int di = w0 ; di <= w1; ++di) {
                ciRaster[otherdim] = di;
                if (__camera->isValidPixel(ciRaster.x(),ciRaster.y(),__imageWidth, __imageHeight)){
                    renderRaster(ciRaster.x(), ciRaster.y(), ciRaster.z(), c, id);
                }
            }
        }
        if ( D > 0){
            cRaster[otherdim] += decal;
            D -= 2*dRaster[dim];
        }
        D += 2* dRaster[otherdim];
        cRaster.z() += Dz;
        cRaster[dim] += 1;
    }
}

void ZBufferEngine::iprocess(PolylinePtr polyline, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera, uint32_t threadid)
{
    if(is_null_ptr(camera)) camera = __camera;
    Point3ArrayPtr points = polyline->getPointList();
    if (is_valid_ptr(points) && points->size() > 1){
        Color4ArrayPtr colorlist = polyline->getColorList();
        if (is_valid_ptr(colorlist)){
            for(uint32_t i = 0; i < points->size()-1; ++i){
                renderSegment(points->getAt(i), points->getAt(i+1), colorlist->getAt(i), colorlist->getAt(i+1), polyline->getWidth(), id, camera );
            }
        }
        else {
            Color4 color = Color4(material->getDiffuseColor(),material->getTransparency());
            for(uint32_t i = 0; i < points->size()-1; ++i){
                renderSegment(points->getAt(i), points->getAt(i+1), color, color, polyline->getWidth(), id, camera );
            }

        }
    }
}

void ZBufferEngine::iprocess(PointSetPtr pointset, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera, uint32_t threadid)
{
    if(is_null_ptr(camera)) camera = __camera;

    const Point3ArrayPtr points(pointset->getPointList());
    Color4 defaultcolor = Color4(material->getDiffuseColor(),material->getTransparency());
    Color4Array::const_iterator itCol;
    bool colorPerPoint = pointset->hasColorList();
    if (colorPerPoint) {
        itCol = pointset->getColorList()->begin();
    }
    uint32_t pointsize = pointset->getWidth();
    for (Point3Array::const_iterator it = points->begin(); it!= points->end(); ++it)
    {
        renderPoint(*it, (colorPerPoint?*itCol:defaultcolor), pointsize, id, camera);
    }
}

ImagePtr ZBufferEngine::getTexture(const ImageTexturePtr imgdef)
{
    Cache<ImagePtr>::const_Iterator it = __cachetexture.find(imgdef->getObjectId());
    if (it != __cachetexture.end()){
        return it->second;
    }
    else {
        ImagePtr img(new Image(imgdef->getFilename()));
        __cachetexture.insert(imgdef->getObjectId(),img);
        return img;
    }
}


void ZBufferEngine::process(ScenePtr scene)
{
    beginProcess();
    size_t msize = scene->size();
    if(__multithreaded && msize > 100){
        size_t nbthreads = ThreadManager::get().nb_threads();
        size_t nbShapePerThread = (msize / nbthreads);
        if (nbShapePerThread * nbthreads < msize) { nbShapePerThread += 1; }

        if(__triangleshaderset != NULL) delete [] __triangleshaderset;
        __triangleshaderset = new TriangleShaderPtr[nbthreads];
        for (size_t j = 0 ; j < nbthreads ; ++j){
            __triangleshaderset[j] = TriangleShaderPtr(is_valid_ptr(__triangleshader) ? __triangleshader->copy(true) : NULL);
        }


        uint32_t threadid = 1;
        for (size_t i = 0 ; i < msize ; i+=nbShapePerThread, ++threadid) {
            
            Scene::const_iterator itbegin = scene->begin() + i ;
            Scene::const_iterator itend = scene->begin() + pglMin(msize, i + nbShapePerThread);

            ThreadManager::get().new_task(boost::bind(&ZBufferEngine::processScene, this, itbegin, itend, ProjectionCameraPtr(__camera->copy()),threadid));

            
        }
    } 
    else {
        processScene(scene->begin(), scene->end(), __camera);
    }
    endProcess();
}

void ZBufferEngine::processScene(Scene::const_iterator scene_begin, Scene::const_iterator scene_end, ProjectionCameraPtr camera, uint32_t threadid)
{
    Discretizer d;
    Tesselator t;
    ProjectionRenderer r(*this, camera, t, d, threadid);
    for (Scene::const_iterator it = scene_begin; it != scene_end; ++it)
        (*it)->apply(r);
}


void ZBufferEngine::duplicateBuffer(const TOOLS(Vector3)& from, const TOOLS(Vector3)& to, bool useDefaultColor, const Color3& defaultcolor)
{
    Vector3 vRasterFrom = __camera->worldToRaster(from,__imageWidth, __imageHeight);
    Vector3 vRasterTo = __camera->worldToRaster(to,__imageWidth, __imageHeight);
    Vector3 vDiff = vRasterTo - vRasterFrom;

    duplicateBuffer(vDiff.x(), vDiff.y(), vDiff.z(), useDefaultColor, defaultcolor);
}

void ZBufferEngine::duplicateBuffer(int32_t xDiff, int32_t yDiff, real_t zDiff, bool useDefaultColor, const Color3& defaultcolor){
    if(xDiff == 0  && yDiff == 0 && fabs(zDiff) < GEOM_EPSILON) return;

    uint32_t width = __imageWidth;
    uint32_t height = __imageHeight;

    uint32_t xStart = 0;
    uint32_t xEnd = width;

    if (xDiff > 0) { 
        if (xDiff > width) { return; }
        xStart += xDiff;
    }
    else {
        if (-xDiff > width) { return; }
        xEnd += xDiff;

    }

    uint32_t yStart = 0;
    uint32_t yEnd = height;
    if (yDiff > 0) { 
        if (yDiff > height) { return; }
        yStart += yDiff;
    }
    else {
        if (-yDiff > height) { return; }
        yEnd += yDiff;
    }

    RealArray2Ptr depthBuffer(new RealArray2(*__depthBuffer));
    FrameBufferManagerPtr framebuffer = (is_valid_ptr(__frameBuffer) ? __frameBuffer->deepcopy() : NULL);

    for (uint32_t i = xStart ; i < xEnd ; ++i) {
        for (uint32_t j = yStart ; j < yEnd ; ++j) {
            real_t potentialz = depthBuffer->getAt(i-xDiff,j-yDiff)+zDiff;
            if (__camera->isInZRange(potentialz))
                renderRaster(i, j, potentialz, useDefaultColor || is_null_ptr(framebuffer) ? defaultcolor : framebuffer->getPixelAt(i-xDiff,j-yDiff));
        }
    }

}

void ZBufferEngine::periodizeBuffer(const Vector3& from, const Vector3& to, bool useDefaultColor, const Color3& defaultcolor)
{
    Vector3 vRasterFrom = __camera->worldToRaster(from,__imageWidth, __imageHeight);
    Vector3 vRasterTo = __camera->worldToRaster(to,__imageWidth, __imageHeight);
    Vector3 vDiff = vRasterTo - vRasterFrom;

    periodizeBuffer(vDiff.x(), vDiff.y(), vDiff.z(), useDefaultColor, defaultcolor);
}

void ZBufferEngine::periodizeBuffer(int32_t xDiff, int32_t yDiff, real_t zDiff, bool useDefaultColor, const Color3& defaultcolor)
{
    _bufferPeriodizationStep(xDiff, yDiff, zDiff, useDefaultColor, defaultcolor);
    _bufferPeriodizationStep(-xDiff, -yDiff, -zDiff, useDefaultColor, defaultcolor);

}

void ZBufferEngine::_bufferPeriodizationStep(int32_t xDiff, int32_t yDiff, real_t zDiff, bool useDefaultColor, const Color3& defaultcolor)
{
    if(xDiff == 0  && yDiff == 0 && fabs(zDiff) < GEOM_EPSILON) return;

    uint32_t width = __imageWidth;
    uint32_t height = __imageHeight;

    int32_t xStart = 0;
    int32_t xEnd = width;
    int32_t xStep = 1;

    if (fabs(xDiff) >= width) { return; }
    if (xDiff < 0) {
         xStart = width-1;
         xEnd = -1;
         xStep = -1;
    }
    xStart += xDiff;
    
    int32_t yStart = 0;
    int32_t yEnd = height;
    int32_t yStep = 1;

    if (fabs(yDiff) >= height) { return; }
    /*if (yDiff < 0) {
         yStart = height-1;
         yEnd = -1;
         yStep = -1;
    }*/
    if (yDiff < 0) {
        yEnd += yDiff;
    }
    else {
        yStart += yDiff;        
    }

    // printf("%i %i %f\n", xDiff, yDiff, zDiff);
    // printf("x:%i %i %i\n", xStart, xEnd, xStep);
    // printf("y:%i %i %i\n", yStart, yEnd, yStep);

    for (int32_t i = xStart ; i != xEnd ; i+=xStep) {
        for (int32_t j = yStart ; j < yEnd ; j+=yStep) {
            real_t potentialz = __depthBuffer->getAt(i-xDiff,j-yDiff)+zDiff;
            // if (__camera->isInZRange(potentialz))
                renderRaster(i, j, potentialz, useDefaultColor || is_null_ptr(__frameBuffer)? defaultcolor : getFrameBufferAt(i-xDiff,j-yDiff));
        }
    }
}


boost::asio::thread_pool * 
ThreadManager::getPool()
{
    if(__pool == NULL) __pool = new boost::asio::thread_pool(__nb_threads);
    return __pool;
}

ThreadManager::ThreadManager(): 
    __pool(NULL),
    __condition_lock(__condition_mutex),
    __nb_threads(boost::thread::hardware_concurrency()+1),
    __nb_tasks(0)
{}

ThreadManager::~ThreadManager()
{
    if(__pool != NULL) delete __pool;
}

void ThreadManager::new_task(std::function<void()> task){
    ++__nb_tasks;
    boost::asio::post(*getPool(), boost::bind(&ThreadManager::process_task, this, task));    
    //boost::asio::post(*__pool, task);    
}

void ThreadManager::process_task(std::function<void()> task)
{
    task();
    __threadend_mutex.lock();
    --__nb_tasks;
    __condition.notify_one();
    __threadend_mutex.unlock();
}

void ThreadManager::join()
{
    while(!hasCompletedTasks()){
        __condition.wait_for(__condition_lock, std::chrono::milliseconds(10), std::bind(&ThreadManager::hasCompletedTasks, this));
    }
    // __condition.wait(__condition_lock, std::bind(&ThreadManager::hasCompletedTasks, this));
}


bool ThreadManager::hasCompletedTasks() const { 
    // printf("rendering nb task : %u\n", uint32_t(__nb_tasks));
    return __nb_tasks == 0; 
}

// Singleton access
ThreadManager& ThreadManager::get() { return ThreadManager::THREADMANAGER; }

ThreadManager ThreadManager::THREADMANAGER;


ImageMutexPtr ZBufferEngine::IMAGEMUTEX;


ImageMutexPtr ZBufferEngine::getImageMutex(uint16_t imageWidth, uint16_t imageHeight)
{
    if (is_null_ptr(IMAGEMUTEX) ||  imageWidth > IMAGEMUTEX->width() || imageHeight > IMAGEMUTEX->height()){
        IMAGEMUTEX = ImageMutexPtr(new ImageMutex(pglMax<uint_t>(imageWidth, is_valid_ptr(IMAGEMUTEX)?IMAGEMUTEX->width():0), 
                                                  pglMax<uint_t>(imageHeight,is_valid_ptr(IMAGEMUTEX)?IMAGEMUTEX->height():0)));
    }
   return IMAGEMUTEX;
}


real_t pixweigth(const Vector2& a, const Vector2& b, real_t raywidth) {
    if (raywidth < 0.5) {
        if (norm(a-b) < GEOM_EPSILON) {
            return 1;
        }
        else return 0;
    }
    Vector2 c = abs(b-a);
    Vector2 d(*c.getMax(),*c.getMin());
    Vector2 dir(d);
    real_t n = dir.normalize();
    if (n <= GEOM_EPSILON) { return 1; }
    real_t n2 = norm(Vector2(0.5,dir.y()*0.5/dir.x()));
    if ((n+n2)<=raywidth) { return 1; }
    else if ((n-n2)>=raywidth) { return 0; }
    else {
        return (raywidth-(n-n2))/(2*n2);
    }
}

  std::tuple<PGL(Point3ArrayPtr),PGL(Color3ArrayPtr),PGL(Uint32Array1Ptr)> ZBufferEngine::grabZBufferPoints(real_t jitter, real_t raywidth) const
  {
   uint32_t width = __imageWidth;
   uint32_t height = __imageHeight;
   raywidth = fabs(raywidth);
   uint32_t raynbpix = ceil(raywidth);
   
   PGL(Point3ArrayPtr) points(new Point3Array());
   PGL(Color3ArrayPtr) colors;
   if (getRenderingStyle() & eColorBased)
        colors = new Color3Array();
   PGL(Uint32Array1Ptr) ids;
   if (getRenderingStyle() & eIdBased)
        ids = new Uint32Array1();
   
   Vector3 origin = __camera->position();
   for (int32_t i = 0 ; i < width ; i++) {
        for (int32_t j = 0 ; j < height ; j++) {
            real_t cz = 0;
            real_t count = 0;
            for (int ii = pglMax<int32_t>(0,i-raynbpix); ii <= pglMin<int32_t>(width-1,i+raynbpix); ++ii){
                for (int jj = pglMax<int32_t>(0,j-raynbpix); jj <= pglMin<int32_t>(height-1,j+raynbpix); ++jj){
                    real_t lcz = __depthBuffer->getAt(ii,jj);
                    if (__camera->isInZRange(lcz)) {
                        real_t w = pixweigth(Vector2(i,j),Vector2(ii,jj),raywidth);
                        if (w > GEOM_EPSILON) {
                            count += w;
                            cz += (lcz*w);
                        }
                    }
                }
            }
            if (count > GEOM_EPSILON) {
                cz /= count;
                Vector3 point = __camera->rasterToWorld(Vector3(i,j,cz), width, height);
                if (jitter > 0) {
                    Vector3 nml = point-origin;
                    real_t n = norm(nml);
                    if(n > GEOM_EPSILON){
                        nml /= n;
                        point += nml*jitter*(-1+2.*(rand()/double(RAND_MAX)));
                    }
                }
                if (point.isValid()) {
                    points->push_back(point);
                    if (getRenderingStyle() & eColorBased)
                        colors->push_back(getFrameBufferAt(i,j));
                    if (getRenderingStyle() & eIdBased){
                        uint32_t cid = __idBuffer->getAt(i,j);
                        if (cid == __defaultid){
                            typedef pgl_hash_map<int32_t,real_t> IdMap;
                            IdMap idcounts; 
                            for (int ii = pglMax<int32_t>(0,i-raynbpix); ii <= pglMin<int32_t>(width-1,i+raynbpix); ++ii){
                                for (int jj = pglMax<int32_t>(0,j-raynbpix); jj <= pglMin<int32_t>(height-1,j+raynbpix); ++jj){
                                    real_t lcz = __depthBuffer->getAt(ii,jj);
                                    if (__camera->isInZRange(lcz)) {
                                        real_t w = pixweigth(Vector2(i,j),Vector2(ii,jj),raywidth);
                                        if (w > GEOM_EPSILON) {
                                            uint32_t lid = __idBuffer->getAt(ii,jj);
                                            idcounts[lid] = (idcounts.find(lid) == idcounts.end()? 0 : idcounts[lid]) + w;
                                        }
                                    }
                                }
                            }
                            cid = idcounts.begin()->first;
                            real_t cw = idcounts.begin()->second;
                            for(IdMap::const_iterator itid = idcounts.begin() ; itid != idcounts.end() ; ++itid){
                                if (cw < itid->second){
                                    cw = itid->second;
                                    cid = itid->first;
                                }
                            }
                        }
                        ids->push_back(cid);
                    }
                }   
                else {
                    printf("invalid points\n");
                }
            }
        }
    }
    return  std::tuple<PGL(Point3ArrayPtr),PGL(Color3ArrayPtr),PGL(Uint32Array1Ptr)> (points, colors, ids);
  }

ScenePtr ZBufferEngine::grabSortedZBufferPoints(real_t jitter, real_t raywidth ) const
{
    ScenePtr result(new Scene());
    std::tuple<PGL(Point3ArrayPtr),PGL(Color3ArrayPtr),PGL(Uint32Array1Ptr)> pointinfos = grabZBufferPoints(jitter, raywidth);
    bool hasId = is_valid_ptr(std::get<2>(pointinfos));
    bool hasColor = is_valid_ptr(std::get<1>(pointinfos));
    if (!hasId) {
        Point3ArrayPtr points = std::get<0>(pointinfos);
        if(hasColor){
            Color4ArrayPtr colors = Color4ArrayPtr(new Color4Array(*std::get<1>(pointinfos)));
            result->add(ShapePtr(new Shape(GeometryPtr(new PointSet(points,colors)))));
        }
        else {
            result->add(ShapePtr(new Shape(GeometryPtr(new PointSet(points)))));
        }
    }
    else {
        typedef pgl_hash_map<uint32_t,PointSetPtr> PointSetMap;
        Point3Array::const_iterator piter = std::get<0>(pointinfos)->begin();
        Point3Array::const_iterator piterend = std::get<0>(pointinfos)->end();
        bool hasColor = is_valid_ptr(std::get<1>(pointinfos));
        Color3Array::const_iterator citer;
        if (hasColor) citer = std::get<1>(pointinfos)->begin();
        Uint32Array1::const_iterator iiter = std::get<2>(pointinfos)->begin();
        PointSetMap idmap;
        for(;piter != piterend; ++piter, ++citer, ++iiter){
            PointSetMap::const_iterator imiter = idmap.find(*iiter);
            PointSetPtr pset;
            if (imiter == idmap.end()){
                pset = PointSetPtr(new PointSet(new Point3Array(), (hasColor ? Color4ArrayPtr(new Color4Array()) : Color4ArrayPtr())));
                idmap[*iiter] = pset;
            }
            else { pset = imiter->second; }
            pset->getPointList()->push_back(*piter);
            if(hasColor) pset->getColorList()->push_back(*citer);
        }

        for(PointSetMap::const_iterator imiter = idmap.begin(); imiter != idmap.end(); ++imiter){
            result->add(ShapePtr(new Shape(GeometryPtr(imiter->second), AppearancePtr(0), imiter->first)));
        }
    }
    return result;
}


pgl_hash_map<uint32_t,real_t> ZBufferEngine::idhistogram(bool solidangle) const
{
    pgl_hash_map<uint32_t,real_t> histo;

    if ((__style & eIdBased) && is_valid_ptr(__idBuffer)){
       for (int32_t i = 0 ; i < __imageWidth ; i++) {
            for (int32_t j = 0 ; j < __imageHeight ; j++) {
            // for (Uint32Array2::const_iterator it = __idBuffer->begin(); it != __idBuffer->end(); ++it){
                uint32_t pid = __idBuffer->getAt(i,j);
                // printf("consider id %i\n",pid);
                if (pid != __defaultid){
                    pgl_hash_map<uint32_t,real_t>::iterator itId = histo.find(pid);
                    real_t value = (solidangle ? __camera->solidAngle(i,j,__imageWidth,__imageHeight) : 1);
                    if (itId == histo.end()){
                        histo[pid] = value;
                    }
                    else {
                        histo[pid] = itId->second+value;
                    }
                }
            }
        }
    }

    return histo;
}

RealArray2Ptr PGL(formFactors)(const Point3ArrayPtr& points, 
                               const Index3ArrayPtr& triangles, 
                               const Point3ArrayPtr& normals,
                               bool ccw,
                               uint16_t discretization,
                               bool solidangle)
{
    RealArray2Ptr result(new RealArray2(triangles->size(),triangles->size(),0));
    uint32_t id = 0;
    Color4 color = Color4::BLACK;
    for(Index3Array::const_iterator it = triangles->begin(); it != triangles->end(); ++it, ++id){
        const Vector3& v0 = points->getAt((*it)[0]); 
        const Vector3& v1 = points->getAt((*it)[1]); 
        const Vector3& v2 = points->getAt((*it)[2]); 
        Vector3 center = (v0+v1+v2)/3;
        Vector3 normal = (is_valid_ptr(normals) ? normals->getAt(id) : cross(v1-v0,v2-v0));
        if(normal.normalize() > GEOM_EPSILON){
            Vector3 up = v0-center;
            ZBufferEngine ze(discretization,discretization,ZBufferEngine::eIdBased, Color3::BLACK, Shape::NOID, true);
            ze.setHemisphericCamera();
            ze.lookAt(center, center+normal, up);
            uint32_t id2 = 0;
            ze.beginProcess();
            for(Index3Array::const_iterator it2 = triangles->begin(); it2 != triangles->end(); ++it2, ++id2){
                if (it2 != it){
                    ze.renderTriangle(points->getAt((*it2)[0]), points->getAt((*it2)[1]), points->getAt((*it2)[2]), 
                                      color, color, color, ccw, id2);
                }
            }
            ze.endProcess();
            pgl_hash_map<uint32_t,real_t> histo = ze.idhistogram(solidangle);
            for (pgl_hash_map<uint32_t,real_t>::const_iterator histoit = histo.begin(); histoit != histo.end(); ++histoit){
                result->setAt(id, histoit->first, histoit->second);
            }
        }
    }
    return result;
}

