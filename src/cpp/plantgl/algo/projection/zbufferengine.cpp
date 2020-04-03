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
#include <queue>
#include <chrono>
/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

#define DEFAULT_MULTITHREAD true

ZBufferEngine::ZBufferEngine(uint16_t imageWidth, uint16_t imageHeight, const Color3& backGroundColor, eRenderingStyle style):
    ProjectionEngine(),
    __imageWidth(imageWidth), 
    __imageHeight(imageHeight), 
    __lightPosition(0,0,1),  
    __lightAmbient(255,255,255),
    __lightDiffuse(255,255,255),
    __lightSpecular(255,255,255),
    __alphathreshold(0.99),
    __depthBuffer(new RealArray2(uint_t(imageWidth), uint_t(imageHeight), REAL_MAX)),
    __frameBuffer(style != eDepthOnly ? new PglFrameBufferManager(imageWidth, imageHeight, style == eIdBased ? 4 : 3, backGroundColor) : NULL),
    __imageMutex(),
    __triangleshader(NULL),
    __triangleshaderset(NULL),
    __multithreaded(DEFAULT_MULTITHREAD)
{
    if (style != eDepthOnly) {
        if (style == eIdBased) __triangleshader = TriangleShaderPtr(new IdBasedShader(this));
        else __triangleshader = TriangleShaderPtr(new TriangleShaderSelector(this));
    }
}    


ZBufferEngine::ZBufferEngine(uint16_t imageWidth, uint16_t imageHeight, const Color4& backGroundColor, eRenderingStyle style):
    ProjectionEngine(),
    __imageWidth(imageWidth), 
    __imageHeight(imageHeight), 
    __lightPosition(0,0,1),  
    __lightAmbient(255,255,255),
    __lightDiffuse(255,255,255),
    __lightSpecular(255,255,255),
    __alphathreshold(0.99),
    __depthBuffer(new RealArray2(uint_t(imageWidth), uint_t(imageHeight), REAL_MAX)),
    __frameBuffer(style == eColorBased ? new PglFrameBufferManager(imageWidth, imageHeight, 3, backGroundColor) : NULL),
    __imageMutex(),
    __triangleshader(NULL),
    __triangleshaderset(NULL),
    __multithreaded(DEFAULT_MULTITHREAD)
{
    if (style != eDepthOnly) {
        if (style == eIdBased) __triangleshader = TriangleShaderPtr(new IdBasedShader(this, backGroundColor.toUint()));
        else __triangleshader = TriangleShaderPtr(new TriangleShaderSelector(this));
    }
}    
    
ZBufferEngine::ZBufferEngine(uint16_t imageWidth, uint16_t imageHeight,uint32_t defaultid, Color4::eColor4Format conversionformat):
    ProjectionEngine(),
    __imageWidth(imageWidth), 
    __imageHeight(imageHeight), 
    __lightPosition(0,0,1),  
    __lightAmbient(255,255,255),
    __lightDiffuse(255,255,255),
    __lightSpecular(255,255,255),
    __alphathreshold(0.99),
    __depthBuffer(new RealArray2(uint_t(imageWidth), uint_t(imageHeight), REAL_MAX)),
    __frameBuffer(), // will be initialized into IdBasedShader constructor
    __imageMutex(),
    __triangleshader(new IdBasedShader(this, defaultid, conversionformat)),
    __triangleshaderset(NULL),
    __multithreaded(DEFAULT_MULTITHREAD)
{
}    

ZBufferEngine::ZBufferEngine(uint16_t imageWidth, uint16_t imageHeight) :
	ProjectionEngine(),
	__imageWidth(imageWidth),
	__imageHeight(imageHeight),
	__lightPosition(0, 0, 1),
	__lightAmbient(255, 255, 255),
	__lightDiffuse(255, 255, 255),
	__lightSpecular(255, 255, 255),
	__alphathreshold(0.99),
	__depthBuffer(new RealArray2(uint_t(imageWidth), uint_t(imageHeight), REAL_MAX)),
	__frameBuffer(),
	__triangleshader(NULL)
{}  
    
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

    if (__multithreaded){
        // printf("begin rendering : create thread pool\n");
        __imageMutex = getImageMutex(__imageWidth, __imageHeight);
    }

}

void ZBufferEngine::endProcess()
{

    if(__multithreaded){
        ThreadManager::get().join();
        // __pool->join();
        // printf("end rendering : %u\n", uint32_t(__nb_tasks));
        //printf("end rendering done\n");
    }
}


ImagePtr ZBufferEngine::getImage() const
{
    PglFrameBufferManagerPtr fb = dynamic_pointer_cast<PglFrameBufferManager>(__frameBuffer);
    if (is_valid_ptr(fb)) { return fb->getImage(); }
    return ImagePtr();
}


bool ZBufferEngine::isTotallyTransparent(const real_t alpha) const
{
    return alpha >= __alphathreshold;
}


Color3 ZBufferEngine::getFrameBufferAt(uint32_t x, uint32_t y)
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
    Vector3 raster = worldToRaster(pos);
    return isVisible(raster.x(), raster.y(), raster.z());
}


bool ZBufferEngine::renderRaster(uint32_t x, uint32_t y, real_t z, const Color4& rasterColor)
{
    if (isTotallyTransparent(rasterColor.getAlpha())) return false;

    lock(x,y);
    if (isVisible(x,y,z)) {
        __depthBuffer->setAt(x, y, z);
        setFrameBufferAt(x,y,rasterColor);
        unlock(x,y);
        return true;
    }
    unlock(x,y);

    return false;

}

void ZBufferEngine::setLight(const Vector3& lightPosition, const Color3& lightColor)
{
    // printf("Set Light Color : %u %u %u\n", lightColor.getRed(), lightColor.getGreen(), lightColor.getBlue());
    __lightPosition = lightPosition;
    __lightAmbient = lightColor;
    __lightDiffuse = lightColor;
    __lightSpecular = lightColor;
}

void ZBufferEngine::setLight(const Vector3& lightPosition, const Color3& lightAmbient, const Color3& lightDiffuse, const Color3& lightSpecular)
{
    // printf("Set Light Ambient : %u %u %u\n", lightAmbient.getRed(), lightAmbient.getGreen(), lightAmbient.getBlue());
    __lightPosition = lightPosition;
    __lightAmbient = lightAmbient;
    __lightDiffuse = lightDiffuse;
    __lightSpecular = lightSpecular;
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
    if (threadid != 0) {
        assert(threadid <= ThreadManager::get().nb_threads());
        //printf("session %u\n", threadid);
        shader = __triangleshaderset[threadid-1];
    }
    else {
        shader = __triangleshader;
    }

    // ProjectionCameraPtr camera = (__multithreaded?ProjectionCameraPtr(_camera->copy()):_camera);
    for(uint32_t itidx = 0; itidx < nbfaces; ++itidx){
        //printf("triangle %i on %i\n", itidx, nbfaces);

        const Vector3& v0 = triangles->getFacePointAt(itidx,0);
        const Vector3& v1 = triangles->getFacePointAt(itidx,1);
        const Vector3& v2 = triangles->getFacePointAt(itidx,2);

        if(is_valid_ptr(shader)){
            shader->init(appearance, triangles, itidx, id, _camera);
        }
      
        shader->initEnv(_camera);
        renderShadedTriangle(v0, v1, v2, ccw, shader, _camera);

    }

}

void ZBufferEngine::renderShadedTriangle(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, bool ccw, const TriangleShaderPtr& shader,  const ProjectionCameraPtr& camera)
{
    
     // Projection in camera space
    Vector3 v0Cam = camera->worldToCamera(v0);
    Vector3 v1Cam = camera->worldToCamera(v1);
    Vector3 v2Cam = camera->worldToCamera(v2);

    // Convert the vertices of the triangle to raster space
    Vector3 v0Raster = camera->cameraToRaster(v0Cam,__imageWidth, __imageHeight);
    Vector3 v1Raster = camera->cameraToRaster(v1Cam,__imageWidth, __imageHeight);
    Vector3 v2Raster = camera->cameraToRaster(v2Cam,__imageWidth, __imageHeight);

    v0Raster.x() = std::round(v0Raster.x());
    v0Raster.y() = std::round(v0Raster.y());
    v1Raster.x() = std::round(v1Raster.x());
    v1Raster.y() = std::round(v1Raster.y());
    v2Raster.x() = std::round(v2Raster.x());
    v2Raster.y() = std::round(v2Raster.y());

    // Prepare vertex attributes. Divde them by their vertex z-coordinate
    // (though we use a multiplication here because v.z = 1 / v.z)
    //Vec2f st0 = st[stindices[i * 3]];
    //Vec2f st1 = st[stindices[i * 3 + 1]];
    //Vec2f st2 = st[stindices[i * 3 + 2]];
    // st0 *= v0Raster.z, st1 *= v1Raster.z, st2 *= v2Raster.z;

    real_t xmin = min3(v0Raster.x(), v1Raster.x(), v2Raster.x());
    real_t ymin = min3(v0Raster.y(), v1Raster.y(), v2Raster.y());
    real_t zmin = min3(v0Raster.z(), v1Raster.z(), v2Raster.z());

    real_t xmax = max3(v0Raster.x(), v1Raster.x(), v2Raster.x());
    real_t ymax = max3(v0Raster.y(), v1Raster.y(), v2Raster.y());
    real_t zmax = max3(v0Raster.z(), v1Raster.z(), v2Raster.z());


      // the triangle is out of screen
    if (xmin >= __imageWidth  || xmax < 0 || ymin >= __imageHeight || ymax < 0 || !camera->isInZRange(zmin, zmax)) {
        // printf("skip \n");
        // printf("%f %f %f \n", v0Raster.x(), v1Raster.x(), v2Raster.x());
        // printf("%f %f %f \n", v0Raster.y(), v1Raster.y(), v2Raster.y());
        // printf("%f %f %f \n", v0Raster.z(), v1Raster.z(), v2Raster.z());
        return;
    }

    // be careful xmin/xmax/ymin/ymax can be negative. Don't cast to uint32_t
    int32_t x0 = pglMax(int32_t(0), (int32_t)(std::floor(xmin)));
    int32_t x1 = pglMin(int32_t(__imageWidth) - 1, (int32_t)(std::floor(xmax)));
    int32_t y0 = pglMax(int32_t(0), (int32_t)(std::floor(ymin)));
    int32_t y1 = pglMin(int32_t(__imageHeight) - 1, (int32_t)(std::floor(ymax)));

    if (__multithreaded && (x1-x0+1)*(y1-y0+1) > 20) {
        ThreadManager::get().new_task(boost::bind(&ZBufferEngine::rasterizeMT, this, Index4(x0,x1,y0,y1), v0Raster, v1Raster, v2Raster, ccw, TriangleShaderPtr(shader->copy()), ProjectionCameraPtr(camera->copy())));
    }
    else {
        rasterize(x0,x1,y0,y1,v0Raster,v1Raster,v2Raster,ccw,shader,camera);
    }
}




struct Fragment {
    real_t x,y,z;
    real_t w0,w1,w2;
    Fragment(real_t _x,real_t _y,real_t _z,real_t _w0,real_t _w1,real_t _w2): x(_x),y(_y),z(_z),w0(_w0),w1(_w1),w2(_w2){ }
};


void ZBufferEngine::rasterizeMT(const Index4& rect,
                              const TOOLS(Vector3)& v0Raster, const TOOLS(Vector3)& v1Raster, const TOOLS(Vector3)& v2Raster, bool ccw, 
                              const TriangleShaderPtr& shader, const ProjectionCameraPtr& camera)
{
    rasterize(rect[0],rect[1],rect[2],rect[3],v0Raster,v1Raster,v2Raster,ccw,shader,camera);  
}

#define PROCESS_FRAGMENT(x,y,z,w0,w1,w2) \
        if (camera->isInZRange(z)){ \
            if (isVisible(x, y, z)) { \
                if(tryLock(x,y)){ \
                    if (isVisible(x, y, z)) { \
                        __depthBuffer->setAt(x, y, z); \
                        if(is_valid_ptr(shader))shader->process(x, y, z, w0, w1, w2); \
                    } \
                    unlock(x,y); \
                } \
                else { \
                    fragqueue.push(Fragment(x,y,z, w0, w1, w2)); \
                } \
            }  \
        }

void ZBufferEngine::rasterize(int32_t x0, int32_t x1, int32_t y0, int32_t y1,
                              TOOLS(Vector3) v0Raster, TOOLS(Vector3) v1Raster, TOOLS(Vector3) v2Raster, bool ccw, 
                              const TriangleShaderPtr& shader, const ProjectionCameraPtr& camera)
{

    // printf("rasterize [%i,%i]*[%i,%i]\n",x0,x1,y0,y1);
    // printf("rasterize [%f,%f] - [%f,%f] - [%f,%f]\n",v0Raster.x(),v0Raster.y(),v1Raster.x(),v1Raster.y(),v2Raster.x(),v2Raster.y());

    real_t z0 = v0Raster.z();
    real_t z1 = v1Raster.z();
    real_t z2 = v2Raster.z();

    // Precompute reciprocal of vertex z-coordinate
    v0Raster.z() = 1. / v0Raster.z();
    v1Raster.z() = 1. / v1Raster.z();
    v2Raster.z() = 1. / v2Raster.z();

    real_t area = edgeFunction(v0Raster, v1Raster, v2Raster, ccw);

    std::queue<Fragment> fragqueue;

    // Inner loop
    if ((x0 == x1) && (y0 == y1)) {
        real_t z = (z0+z1+z2)/3;
        real_t w0 = 1/3.;
        PROCESS_FRAGMENT(x0, y0, z, w0, w0, w0)
    }
    else if (x0 == x1){
        real_t zs[3];
        zs[0] = z0; zs[1] = z1; zs[2] = z2; 
        uint32_t ys[3];
        ys[0] = v0Raster.y(); ys[1] = v1Raster.y(); ys[2] = v2Raster.y(); 

        int firstpoint = (z0 >= z1? (z0 >= z2?0:2) : (z1 >= z2?1:2) );
        int secpoint   = (z0 >= z1? (z0 >= z2?2:0) : (z1 >= z2?2:1) );
        int thirdpoint = (z0 < z1? (z0 >= z2?2:0)  : (z1 >= z2?2:1) );

        real_t ws[3];
        ws[0] = 0; ws[1] = 0; ws[2] = 0;

        std::vector<std::pair<int,int> > toprocess;
        toprocess.push_back(std::pair<int,int>(firstpoint, secpoint));
        if ((ys[thirdpoint]-ys[firstpoint]) * (ys[secpoint]-ys[firstpoint]) < 0){
            // they are in opposite direction, compare to third point. easy
            toprocess.push_back(std::pair<int,int>(firstpoint, thirdpoint));
        }
        else if ((ys[thirdpoint]-ys[firstpoint]) > (ys[secpoint]-ys[firstpoint]) ){
            // thirdpoint is more far that second
            toprocess.push_back(std::pair<int,int>(secpoint, thirdpoint));
        }

        for (std::vector<std::pair<int,int> >::const_iterator itp = toprocess.begin(); itp != toprocess.end(); ++itp) {
            int apoint = itp->first;
            int bpoint = itp->second;

            int deltay = ys[bpoint]-ys[apoint];
            // printf("process %i to %i : %i %i\n", apoint, bpoint, ys[apoint], ys[bpoint]);

            if (deltay == 0) {
                ws[apoint] = 1;
                PROCESS_FRAGMENT(x0, ys[apoint], zs[apoint], ws[0], ws[1], ws[2])
            }
            else {
                real_t fdeltay = fabs(deltay);
                int32_t dy = (deltay>=0?1:-1);
                for (int32_t y = ys[apoint]; y != ys[bpoint]+dy; y+=dy) {
                    ws[0] = 0; ws[1] = 0; ws[2] = 0;
                    ws[apoint] = fabs(real_t(ys[bpoint])-y)/fdeltay;
                    ws[bpoint] = fabs(real_t(ys[apoint])-y)/fdeltay;
                    real_t z = zs[apoint]*ws[apoint]+zs[bpoint]*ws[bpoint];
                    PROCESS_FRAGMENT(x0, y, z, ws[0], ws[1], ws[2])
                }
            }
        }

    }
    else if (y0 == y1){
        real_t zs[3];
        zs[0] = z0; zs[1] = z1; zs[2] = z2; 
        real_t xs[3];
        xs[0] = v0Raster.x(); xs[1] = v1Raster.x(); xs[2] = v2Raster.x(); 

        int firstpoint = (z0 >= z1? (z0 >= z2?0:2) : (z1 >= z2?1:2) );
        int secpoint   = (z0 >= z1? (z0 >= z2?2:0) : (z1 >= z2?2:1) );
        int thirdpoint = (z0 < z1? (z0 >= z2?2:0)  : (z1 >= z2?2:1) );

        real_t ws[3];
        ws[0] = 0; ws[1] = 0; ws[2] = 0;

        std::vector<std::pair<int,int> > toprocess;
        toprocess.push_back(std::pair<int,int>(firstpoint, secpoint));
        if ((xs[thirdpoint]-xs[firstpoint]) * (xs[secpoint]-xs[firstpoint]) < 0){
            // they are in opposite direction, compare to third point. easy
            toprocess.push_back(std::pair<int,int>(firstpoint, thirdpoint));
        }
        else if ((xs[thirdpoint]-xs[firstpoint]) > (xs[secpoint]-xs[firstpoint]) ){
            // thirdpoint is more far that second
            toprocess.push_back(std::pair<int,int>(secpoint, thirdpoint));
        }

        for (std::vector<std::pair<int,int> >::const_iterator itp = toprocess.begin(); itp != toprocess.end(); ++itp) {
            int apoint = itp->first;
            int bpoint = itp->second;

            int deltax = xs[bpoint]-xs[apoint];

            if (deltax == 0) {
                ws[apoint] = 1;
                PROCESS_FRAGMENT(xs[apoint], y0, zs[apoint], ws[0], ws[1], ws[2])
            }
            else {
                real_t fdeltax = fabs(deltax);
                int32_t dx = (deltax>=0?1:-1);
                for (int32_t x = xs[apoint]; x != xs[bpoint]+dx; x+=dx) {
                    ws[0] = 0; ws[1] = 0; ws[2] = 0;
                    ws[apoint] = fabs(xs[bpoint]-x)/fdeltax;
                    ws[bpoint] = fabs(xs[apoint]-x)/fdeltax;
                    real_t z = zs[apoint]*ws[apoint]+zs[bpoint]*ws[bpoint];
                    PROCESS_FRAGMENT(x, y0, z, ws[0], ws[1], ws[2])
                }
            }
        }

    }
    else {
        for (int32_t y = y0; y <= y1; ++y) {
            for (int32_t x = x0; x <= x1; ++x) {

                Vector2 pixelSample(x + 0.5, y + 0.5);

                // find weight of pixel
                real_t w0 = edgeFunction(v1Raster, v2Raster, pixelSample, ccw);
                real_t w1 = edgeFunction(v2Raster, v0Raster, pixelSample, ccw);
                real_t w2 = edgeFunction(v0Raster, v1Raster, pixelSample, ccw);

                // printf("%i %i : %f %f %f\n", x,y,w0,w1,w2);

                if ((w0 > -GEOM_EPSILON && w1 > -GEOM_EPSILON && w2 > -GEOM_EPSILON) || (w0 < GEOM_EPSILON && w1 < GEOM_EPSILON && w2 < GEOM_EPSILON)) {
                    w0 /= area;
                    w1 /= area;
                    w2 /= area;

                    real_t oneOverZ = v0Raster.z() * w0 + v1Raster.z() * w1 + v2Raster.z() * w2;
                    real_t z = 1. / oneOverZ;

                    // Depth-buffer test
                    if (camera->isInZRange(z)){

                        // Vec2f st = st0 * w0 + st1 * w1 + st2 * w2;                        
                        // st *= z;
                        if (isVisible(x, y, z)) {
                            if(tryLock(x,y)){
                                if (isVisible(x, y, z)) {
                                    __depthBuffer->setAt(x, y, z);
                                    if(is_valid_ptr(shader))shader->process(x, y, z, (w0 * z / z0), (w1 * z / z1), (w2 * z / z2));
                                }
                                unlock(x,y);
                            }
                            else {
                                fragqueue.push(Fragment(x,y,z,(w0 * z / z0), (w1 * z / z1), (w2 * z / z2)));

                            }
                        }                   
                    }
                }
            }
        }
    }
    while(!fragqueue.empty()) {
        Fragment f = fragqueue.front();
        fragqueue.pop();
        if(tryLock(f.x,f.y)){
            if (isVisible(f.x, f.y, f.z)) {
                __depthBuffer->setAt(f.x, f.y, f.z);
                if(is_valid_ptr(shader))shader->process(f.x, f.y, f.z, f.w0, f.w1, f.w2);
            }
            unlock(f.x,f.y);
        }
        else {
            fragqueue.push(f);
        }        
    }

}



void ZBufferEngine::renderTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, 
                                     const Color4& c0,  const Color4& c1,  const Color4& c2, 
                                     bool ccw, ProjectionCameraPtr camera)
{
    if(is_null_ptr(camera)) camera = __camera;
    ColorBasedShader * shader = new ColorBasedShader(this);
    shader->setColors(c0, c1, c2);
    renderShadedTriangle(v0, v1, v2, ccw, shader, camera);
}


void ZBufferEngine::renderPoint(const TOOLS(Vector3)& v, const Color4& c, const uint32_t width, ProjectionCameraPtr camera)
{
    
    if(is_null_ptr(camera)) camera = __camera;

    Vector3 vCam = camera->worldToCamera(v);

    // Convert the vertices of the triangle to raster space
    Vector3 vRaster = camera->cameraToRaster(vCam,__imageWidth, __imageHeight);

    // v0Raster.z() = 1. / v0Raster.z();

    // the point is out of screen
    if (vRaster.x() >= __imageWidth  || vRaster.x() < 0 || 
        vRaster.y() >= __imageHeight || vRaster.y() < 0 || 
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
            renderRaster(x, y, vRaster.z(), c);
        }
    }
}

void ZBufferEngine::renderSegment(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const Color4& c0, const Color4& c1, const uint32_t width, ProjectionCameraPtr camera)
{
    if(is_null_ptr(camera)) camera = __camera;

}


void ZBufferEngine::iprocess(PolylinePtr polyline, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera, uint32_t threadid)
{
    if(is_null_ptr(camera)) camera = __camera;

}

void ZBufferEngine::iprocess(PointSetPtr pointset, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera, uint32_t threadid)
{
    if(is_null_ptr(camera)) camera = __camera;

    const Point3ArrayPtr points(pointset->getPointList());
    Color4 defaultcolor = Color4(material->getAmbient(),material->getTransparency());
    Color4Array::const_iterator itCol;
    bool colorPerPoint = pointset->hasColorList();
    if (colorPerPoint) {
        itCol = pointset->getColorList()->begin();
    }
    uint32_t pointsize = pointset->getWidth();
    for (Point3Array::const_iterator it = points->begin(); it!= points->end(); ++it)
    {
        renderPoint(*it, (colorPerPoint?*itCol:defaultcolor), pointsize, camera);
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
            __triangleshaderset[j] = TriangleShaderPtr(__triangleshader->copy(true));
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
