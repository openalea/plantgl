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

#include "zbufferengine.h"
#include "zbufferrenderer.h"
/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE


ZBufferEngine::ZBufferEngine(uint16_t imageWidth, uint16_t imageHeight, const Color3& backGroundColor, eRenderingStyle style):
    __imageWidth(imageWidth), 
    __imageHeight(imageHeight), 
    __camera(0), 
    __lightPosition(0,0,1),  
    __lightAmbient(255,255,255),
    __lightDiffuse(255,255,255),
    __lightSpecular(255,255,255),
    __alphathreshold(0.99),
    __depthBuffer(new RealArray2(uint_t(imageWidth), uint_t(imageHeight), REAL_MAX)),
    __frameBuffer(style != eDepthOnly ? new PglFrameBufferManager(imageWidth, imageHeight, style == eIdBased ? 4 : 3, backGroundColor) : NULL),
    __triangleshader(NULL)
{
    setOrthographicCamera(-1, 1, -1, 1, -1, 1);
    lookAt(Vector3(0,1,0),Vector3(0,0,0),Vector3(0,0,1));
    if (style != eDepthOnly) {
        if (style == eIdBased) __triangleshader = TriangleShaderPtr(new IdBasedShader(this));
        else __triangleshader = TriangleShaderPtr(new TriangleShaderSelector(this));
    }
}    


ZBufferEngine::ZBufferEngine(uint16_t imageWidth, uint16_t imageHeight, const Color4& backGroundColor, eRenderingStyle style):
    __imageWidth(imageWidth), 
    __imageHeight(imageHeight), 
    __camera(0), 
    __lightPosition(0,0,1),  
    __lightAmbient(255,255,255),
    __lightDiffuse(255,255,255),
    __lightSpecular(255,255,255),
    __alphathreshold(0.99),
    __depthBuffer(new RealArray2(uint_t(imageWidth), uint_t(imageHeight), REAL_MAX)),
    __frameBuffer(style == eColorBased ? new PglFrameBufferManager(imageWidth, imageHeight, 3, backGroundColor) : NULL),
    __triangleshader(NULL)
{
    setOrthographicCamera(-1, 1, -1, 1, -1, 1);
    lookAt(Vector3(0,1,0),Vector3(0,0,0),Vector3(0,0,1));
    if (style != eDepthOnly) {
        if (style == eIdBased) __triangleshader = TriangleShaderPtr(new IdBasedShader(this, backGroundColor.toUint()));
        else __triangleshader = TriangleShaderPtr(new TriangleShaderSelector(this));
    }
}    
    
ZBufferEngine::ZBufferEngine(uint16_t imageWidth, uint16_t imageHeight,uint32_t defaultid, Color4::eColor4Format conversionformat):
    __imageWidth(imageWidth), 
    __imageHeight(imageHeight), 
    __camera(0), 
    __lightPosition(0,0,1),  
    __lightAmbient(255,255,255),
    __lightDiffuse(255,255,255),
    __lightSpecular(255,255,255),
    __alphathreshold(0.99),
    __depthBuffer(new RealArray2(uint_t(imageWidth), uint_t(imageHeight), REAL_MAX)),
    __frameBuffer(), // will be initialized into IdBasedShader constructor
    __triangleshader(new IdBasedShader(this, defaultid, conversionformat))
{
    setOrthographicCamera(-1, 1, -1, 1, -1, 1);
    lookAt(Vector3(0,1,0),Vector3(0,0,0),Vector3(0,0,1));
}    
    
    
ZBufferEngine::~ZBufferEngine()
{
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

bool ZBufferEngine::isVisible(const TOOLS(Vector3)& pos) const
{
    Vector3 raster = worldToRaster(pos);
    return isVisible(raster.x(), raster.y(), raster.z());
}


bool ZBufferEngine::renderRaster(uint32_t x, uint32_t y, real_t z, const Color4& rasterColor)
{
    if (isTotallyTransparent(rasterColor.getAlpha())) return false;
    if (isVisible(x,y,z)) {
        __depthBuffer->setAt(x, y, z);
        setFrameBufferAt(x,y,rasterColor);
        return true;
    }
    return false;

}


void ZBufferEngine::transformModel(const TOOLS(Matrix4)& transform)
{
    __camera->transformModel(transform);
}

void ZBufferEngine::pushModelTransformation()
{
    __camera->pushModelTransformation();
}

void ZBufferEngine::popModelTransformation()
{
    __camera->popModelTransformation();
}

void ZBufferEngine::transformModelIdentity()
{
    __camera->transformModelIdentity();
}

void ZBufferEngine::translateModel(const Vector3& v)
{
    __camera->translateModel(v);
}

void ZBufferEngine::scaleModel(const Vector3& v)
{
    __camera->scaleModel(v);
}


void ZBufferEngine::setPerspectiveCamera(real_t angleOfView, real_t aspectRatio, real_t near, real_t far){
    __camera = ProjectionCamera::perspectiveCamera(angleOfView, aspectRatio, near, far);
}

void ZBufferEngine::setFrustumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
{
    __camera = ProjectionCamera::frustumCamera(left, right, bottom, top, near, far);
}

void ZBufferEngine::setOrthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
{
    __camera = ProjectionCamera::orthographicCamera(left, right, bottom, top, near, far);
}

BoundingBoxPtr ZBufferEngine::getBoundingBoxView() const
{
    return __camera->getBoundingBoxView();
}

  
void ZBufferEngine::lookAt(const TOOLS(Vector3)& eyePosition3D, const TOOLS(Vector3)& center3D, const TOOLS(Vector3)& upVector3D)
{
    __camera->lookAt(eyePosition3D, center3D, upVector3D);
}

void ZBufferEngine::setLight(const TOOLS(Vector3)& lightPosition, const Color3& lightColor)
{
    // printf("Set Light Color : %u %u %u\n", lightColor.getRed(), lightColor.getGreen(), lightColor.getBlue());
    __lightPosition = lightPosition;
    __lightAmbient = lightColor;
    __lightDiffuse = lightColor;
    __lightSpecular = lightColor;
}

void ZBufferEngine::setLight(const TOOLS(Vector3)& lightPosition, const Color3& lightAmbient, const Color3& lightDiffuse, const Color3& lightSpecular)
{
    // printf("Set Light Ambient : %u %u %u\n", lightAmbient.getRed(), lightAmbient.getGreen(), lightAmbient.getBlue());
    __lightPosition = lightPosition;
    __lightAmbient = lightAmbient;
    __lightDiffuse = lightDiffuse;
    __lightSpecular = lightSpecular;
}

/*
Vector3 ZBufferEngine::worldToRaster(const Vector3& vertexWorld) const
{

    Vector3 vertexCamera = __worldToCamera * vertexWorld;
    return toRasterSpace(__camera->projecttoNDC(vertexCamera), __imageWidth, __imageHeight);

}

TOOLS(Vector3) ZBufferEngine::worldToCamera(const TOOLS(Vector3)& vertexWorld) const
{
    return __currentWorldToCamera * vertexWorld;
}

TOOLS(Vector3) ZBufferEngine::cameraToNDC(const TOOLS(Vector3)& vertexCamera) const
{
    return __camera->projecttoNDC(vertexCamera);
}

TOOLS(Vector3) ZBufferEngine::ndcToRaster(const TOOLS(Vector3)& vertexNDC) const
{
    return toRasterSpace(vertexNDC, __imageWidth, __imageHeight);
}

TOOLS(Vector3) ZBufferEngine::cameraToRaster(const TOOLS(Vector3)& vertexCamera) const
{
    return toRasterSpace(__camera->projecttoNDC(vertexCamera), __imageWidth, __imageHeight);
}

TOOLS(Vector3) ZBufferEngine::cameraToWorld(const TOOLS(Vector3)& vertexCamera) const
{
    return __cameraToWorld * vertexCamera;
}

TOOLS(Vector3) ZBufferEngine::rasterToWorld(const TOOLS(Vector3)& raster) const
{
    Vector3 vertexNDC = rasterToNDC(raster, __imageWidth, __imageHeight);
    return cameraToWorld(__camera->NDC2screen(vertexNDC.x(), vertexNDC.y(), vertexNDC.z()));
}
*/



real_t min3(const real_t &a, const real_t &b, const real_t &c)
{ return pglMin(a, pglMin(b, c)); }

real_t max3(const real_t &a, const real_t &b, const real_t &c)
{ return pglMax(a, pglMax(b, c)); }

real_t edgeFunction(const Vector3 &a, const Vector3 &b, const Vector3 &c, bool ccw)
{ 
    if (!ccw)
        return (c.x() - a.x()) * (b.y() - a.y()) - (c.y() - a.y()) * (b.x() - a.x()); 
    else 
        return (a.x() - b.x()) * (c.y() - a.y()) - (a.y() - b.y()) * (c.x() - a.x()); 
}


void ZBufferEngine::render(TriangleSetPtr triangles, AppearancePtr appearance)
{
    const Point3ArrayPtr points(triangles->getPointList());
    const Index3ArrayPtr indices(triangles->getIndexList());

    bool ccw = triangles->getCCW();

    triangles->checkNormalList();

    size_t nbfaces = triangles->getIndexListSize();
    bool hasColor = triangles->hasColorList();


    for(uint32_t itidx = 0; itidx < nbfaces; ++itidx){
        //printf("triangle %i on %i\n", itidx, nbfaces);
        const Vector3& v0 = triangles->getFacePointAt(itidx,0);
        const Vector3& v1 = triangles->getFacePointAt(itidx,1);
        const Vector3& v2 = triangles->getFacePointAt(itidx,2);

        if(is_valid_ptr(__triangleshader))__triangleshader->init(appearance, triangles, itidx, __currentId);
        renderShadedTriangle(__triangleshader, v0, v1, v2, ccw);

    }
}


void ZBufferEngine::renderShadedTriangle(TriangleShaderPtr shader, const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, bool ccw)
{
    // Projection in camera space
    Vector3 v0Cam = __camera->worldToCamera(v0);
    Vector3 v1Cam = __camera->worldToCamera(v1);
    Vector3 v2Cam = __camera->worldToCamera(v2);

    // Convert the vertices of the triangle to raster space
    Vector3 v0Raster = __camera->cameraToRaster(v0Cam,__imageWidth, __imageHeight);
    Vector3 v1Raster = __camera->cameraToRaster(v1Cam,__imageWidth, __imageHeight);
    Vector3 v2Raster = __camera->cameraToRaster(v2Cam,__imageWidth, __imageHeight);

    real_t z0 = v0Raster.z();
    real_t z1 = v1Raster.z();
    real_t z2 = v2Raster.z();

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

    // Precompute reciprocal of vertex z-coordinate
    v0Raster.z() = 1. / v0Raster.z();
    v1Raster.z() = 1. / v1Raster.z();
    v2Raster.z() = 1. / v2Raster.z();

    // the triangle is out of screen
    if (xmin >= __imageWidth  || xmax < 0 || ymin >= __imageHeight || ymax < 0 || !__camera->isInZRange(zmin, zmax)) {
        /*printf("skip \n");
        printf("%f %f %f \n", v0Raster.x(), v1Raster.x(), v2Raster.x());
        printf("%f %f %f \n", v0Raster.y(), v1Raster.y(), v2Raster.y());
        printf("%f %f %f \n", v0Raster.z(), v1Raster.z(), v2Raster.z());*/
        return;
    }

    // be careful xmin/xmax/ymin/ymax can be negative. Don't cast to uint32_t
    int32_t x0 = pglMax(int32_t(0), (int32_t)(std::floor(xmin)));
    int32_t x1 = pglMin(int32_t(__imageWidth) - 1, (int32_t)(std::floor(xmax)));
    int32_t y0 = pglMax(int32_t(0), (int32_t)(std::floor(ymin)));
    int32_t y1 = pglMin(int32_t(__imageHeight) - 1, (int32_t)(std::floor(ymax)));

    real_t area = edgeFunction(v0Raster, v1Raster, v2Raster, ccw);


    /*if (x0 == 0 && y0 == 0){
        printf("x range : %u %u\n", x0, x1);
        printf("y range : %u %u\n", y0, y1);
        printf("v0 %f %f %f\n", v0.x(), v0.y(), v0.z());
        printf("v1 %f %f %f\n", v1.x(), v1.y(), v1.z());
        printf("v2 %f %f %f\n", v2.x(), v2.y(), v2.z());
        printf("r0 %f %f %f\n", v0Raster.x(), v0Raster.y(), 1/v0Raster.z());
        printf("r1 %f %f %f\n", v1Raster.x(), v1Raster.y(), 1/v1Raster.z());
        printf("r2 %f %f %f\n", v2Raster.x(), v2Raster.y(), 1/v2Raster.z());
    }*/
    // Inner loop
    for (int32_t y = y0; y <= y1; ++y) {
        for (int32_t x = x0; x <= x1; ++x) {

            Vector3 pixelSample(x + 0.5, y + 0.5, 0);

            // find weight of pixel
            real_t w0 = edgeFunction(v1Raster, v2Raster, pixelSample, ccw);
            real_t w1 = edgeFunction(v2Raster, v0Raster, pixelSample, ccw);
            real_t w2 = edgeFunction(v0Raster, v1Raster, pixelSample, ccw);

            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                w0 /= area;
                w1 /= area;
                w2 /= area;

                real_t oneOverZ = v0Raster.z() * w0 + v1Raster.z() * w1 + v2Raster.z() * w2;
                real_t z = 1. / oneOverZ;

                // Depth-buffer test
                if (__camera->isInZRange(z)){

                    // Vec2f st = st0 * w0 + st1 * w1 + st2 * w2;                        
                    // st *= z;
                    if (isVisible(x, y, z)) {
                        __depthBuffer->setAt(x, y, z);
                        if(is_valid_ptr(shader))shader->process(x, y, z, (w0 * z / z0), (w1 * z / z1), (w2 * z / z2));
                    }                    
                }
            }
        }
    }        
}



void ZBufferEngine::renderTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, 
                                     const Color4& c0,  const Color4& c1,  const Color4& c2, 
                                     bool ccw)
{
    ColorBasedShader * shader = new ColorBasedShader(this);
    shader->setColors(c0, c1, c2);
    renderShadedTriangle(shader, v0, v1, v2, ccw);
}


void ZBufferEngine::renderPoint(const TOOLS(Vector3)& v, const Color4& c, const uint32_t width)
{
    Vector3 vCam = __camera->worldToCamera(v);

    // Convert the vertices of the triangle to raster space
    Vector3 vRaster = __camera->cameraToRaster(vCam,__imageWidth, __imageHeight);

    // v0Raster.z() = 1. / v0Raster.z();

    // the point is out of screen
    if (vRaster.x() >= __imageWidth  || vRaster.x() < 0 || 
        vRaster.y() >= __imageHeight || vRaster.y() < 0 || 
        !__camera->isInZRange(vRaster.z())) {
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

void ZBufferEngine::renderSegment(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const Color4& c0, const Color4& c1, const uint32_t width)
{

}


void ZBufferEngine::render(PolylinePtr polyline, MaterialPtr material)
{
}

void ZBufferEngine::render(PointSetPtr pointset, MaterialPtr material)
{
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
        renderPoint(*it, (colorPerPoint?*itCol:defaultcolor), pointsize);
    }
}

void ZBufferEngine::render(ScenePtr scene)
{
    Discretizer d;
    Tesselator t;
    ZBufferRenderer r(*this, t, d);
    scene->apply(r);
}

ImagePtr ZBufferEngine::getTexture(const ImageTexturePtr imgdef)
{
    Cache<ImagePtr>::const_Iterator it = __cachetexture.find(imgdef->getId());
    if (it != __cachetexture.end()){
        return it->second;
    }
    else {
        ImagePtr img(new Image(imgdef->getFilename()));
        __cachetexture.insert(imgdef->getId(),img);
        return img;
    }
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

void ZBufferEngine::periodizeBuffer(const TOOLS(Vector3)& from, const TOOLS(Vector3)& to, bool useDefaultColor, const Color3& defaultcolor)
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