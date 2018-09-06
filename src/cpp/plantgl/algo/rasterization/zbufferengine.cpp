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


PglFrameBufferManager::PglFrameBufferManager(uint16_t imageWidth, uint16_t imageHeight, const Color3& backGroundColor) :
     FrameBufferManager(imageWidth, imageHeight,backGroundColor), 
     __image(new Image(imageWidth, imageHeight, 3, backGroundColor)) 
{}

PglFrameBufferManager::~PglFrameBufferManager() {}

void PglFrameBufferManager::setPixelAt(uint_t x, uint_t y, const Color3& color) 
{ 
    __image->setPixelAt(x,y,color); 
}

Color3 PglFrameBufferManager::getPixelAt(uint_t x, uint_t y) const
{ 
    return __image->getPixelAt(x,y); 
}

uint16_t PglFrameBufferManager::width() const { return __image->width(); }
uint16_t PglFrameBufferManager::height() const { return __image->height(); }

OrthographicProjection::OrthographicProjection(real_t _left, real_t _right, real_t _bottom, real_t _top, real_t _near, real_t _far):
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
        far(_far){
    }

OrthographicProjection::~OrthographicProjection() {}

Vector3 OrthographicProjection::screen2NDC(const real_t& xScreen, const real_t& yScreen, const real_t z) const {
    // now convert point from screen space to NDC space (in range [-1,1])
    Vector3 vertexNDC(__xscale * xScreen  - __xconstant, __yscale * yScreen  - __yconstant,  -z);

    return vertexNDC;
}

Vector3 OrthographicProjection::projecttoNDC(const Vector3& vertexCamera) const {
    return screen2NDC(vertexCamera.x(), vertexCamera.y(), vertexCamera.z());
}

bool OrthographicProjection::isInZRange(real_t z) const {
    return (z >= near && z <= far);
}

bool OrthographicProjection::isInZRange(real_t zmin, real_t zmax) const{
    return !(zmax < near || zmin > far);
}


PerspectiveProjection::PerspectiveProjection(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far):
        OrthographicProjection(left, right, bottom, top, near, far){
}

Vector3 PerspectiveProjection::projecttoNDC(const Vector3& vertexCamera) const {
    real_t z = -vertexCamera.z();
    return screen2NDC(near * vertexCamera.x() / z, near * vertexCamera.y() / z, vertexCamera.z());
}

PerspectiveProjection::~PerspectiveProjection() {}

inline Vector3 toRasterSpace(const Vector3& vertexNDC, const uint16_t imageWidth, const uint16_t imageHeight)
{
    // convert to raster space
    // in raster space y is down so invert direction
    Vector3 vertexRaster( 
                          ((vertexNDC.x() + 1) / 2.) * imageWidth, 
                          ((1 - vertexNDC.y()) / 2.) * imageHeight,
                          vertexNDC.z());
    return vertexRaster;
}


ZBufferEngine::ZBufferEngine(uint16_t imageWidth, uint16_t imageHeight, const Color3& backGroundColor):
    __imageWidth(imageWidth), 
    __imageHeight(imageHeight), 
    __camera(0), 
    __lightPosition(0,0,1),  
    __lightAmbient(255,255,255),
    __lightDiffuse(255,255,255),
    __lightSpecular(255,255,255),
    __alphathreshold(0.99),
    __depthBuffer(new RealArray2(uint_t(imageWidth), uint_t(imageHeight), REAL_MAX)),
    __frameBuffer(new PglFrameBufferManager(imageWidth, imageHeight, backGroundColor))
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

bool ZBufferEngine::renderRaster(uint32_t x, uint32_t y, real_t z, const Color4& rasterColor)
{
    if (isTotallyTransparent(rasterColor.getAlpha())) return false;

    if (z < __depthBuffer->getAt(x, y)) {
        __depthBuffer->setAt(x, y, z);
        setFrameBufferAt(x,y,rasterColor);
        return true;
    }
    return false;

}


void ZBufferEngine::transformModel(const TOOLS(Matrix4)& transform)
{
    __currentModelMatrix *= transform;
    __currentWorldToCamera = __worldToCamera * __currentModelMatrix;
}

void ZBufferEngine::pushModelTransformation()
{
    __modelMatrixStack.push(__currentModelMatrix);
}

void ZBufferEngine::popModelTransformation()
{
    if (!__modelMatrixStack.empty()){
        __currentModelMatrix = __modelMatrixStack.top();
        __currentWorldToCamera = __worldToCamera * __currentModelMatrix;    
        __modelMatrixStack.pop();
    }

}

void ZBufferEngine::transformModelIdentity()
{
    __currentModelMatrix = Matrix4();
    __currentWorldToCamera = __worldToCamera * __currentModelMatrix;    
}

void ZBufferEngine::translateModel(const Vector3& v)
{
    transformModel(Matrix4::translation(v));
}

void ZBufferEngine::scaleModel(const Vector3& v)
{
    transformModel(Matrix4(Matrix3::scaling(v)));
}


void ZBufferEngine::setPerspectiveCamera(real_t angleOfView, real_t aspectRatio, real_t near, real_t far){
    real_t top = near * tan(angleOfView * GEOM_RAD/2.);
    real_t bottom = -top;
    real_t right = top * aspectRatio;
    real_t left = -right;
    setFrustrumCamera(left, right, bottom, top, near, far);
}

void ZBufferEngine::setFrustrumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
{
    __camera = ProjectionPtr(new PerspectiveProjection(left, right, bottom, top, near, far));
}

void ZBufferEngine::setOrthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
{
    __camera = ProjectionPtr(new OrthographicProjection(left, right, bottom, top, near, far));
}

BoundingBoxPtr ZBufferEngine::getBoundingBoxView() const
{
    return BoundingBoxPtr(new BoundingBox(Vector3(__camera->left,__camera->bottom,__camera->near),Vector3(__camera->right,__camera->top,__camera->far)));
}

  
void ZBufferEngine::lookAt(const TOOLS(Vector3)& eyePosition3D, const TOOLS(Vector3)& center3D, const TOOLS(Vector3)& upVector3D)
{
        Vector3 forward = eyePosition3D - center3D;
        forward.normalize();
        Vector3 up(upVector3D);
        up.normalize();
        Vector3 side = cross(upVector3D, forward);
        side.normalize();
        up = cross(forward, side);
        up.normalize();
        __cameraPosition = eyePosition3D;
        Matrix4 camToWorld(side, up, forward, eyePosition3D);
        __worldToCamera = inverse(camToWorld);
        __currentWorldToCamera = __worldToCamera * __currentModelMatrix;
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


Color4 PGL(phong)(const Vector3& v, const Vector3& n, 
             const Vector3& cameraPosition, 
             const Vector3& lightPosition, 
             const Color3& lightAmbient, const Color3& lightDiffuse, const Color3& lightSpecular, 
             MaterialPtr material)
{
        Vector3 l = (lightPosition - v).normed();
        Vector3 vd = (cameraPosition - v).normed();
        Vector3 reflection = n.reflect(l); 

        real_t diffusecoef = pglMax(0.0, dot(n, l));
        real_t shininess = material->getShininess();
        real_t specularcoef = pow(pglMax(0.0, dot(vd, reflection)), shininess);

        Color3 ambient = material->getAmbient()      * lightAmbient;
        Color3 diffuse = material->getDiffuseColor() * lightDiffuse;
        Color3 specular = material->getSpecular()    * lightSpecular;

        // printf("%i %i %i\n", lightAmbient.getRed(), lightAmbient.getGreen(), lightAmbient.getBlue());
        // printf("%i %i %i\n", ambient.getRed(), ambient.getGreen(), ambient.getBlue());
        Color3 result = ambient + diffuse * diffusecoef +  specular * specularcoef; 

        return Color4(result, material->getTransparency());
}

void ZBufferEngine::render(TriangleSetPtr triangles, MaterialPtr material, uint32_t id)
{
    const Point3ArrayPtr points(triangles->getPointList());
    const Index3ArrayPtr indices(triangles->getIndexList());

    bool ccw = triangles->getCCW();

    triangles->checkNormalList();

    size_t nbfaces = triangles->getIndexListSize();

    for(uint32_t itidx = 0; itidx < nbfaces; ++itidx){
        //printf("triangle %i on %i\n", itidx, nbfaces);
        const Vector3& v0 = triangles->getFacePointAt(itidx,0);
        const Vector3& v1 = triangles->getFacePointAt(itidx,1);
        const Vector3& v2 = triangles->getFacePointAt(itidx,2);

        const Vector3& n0 = triangles->getFaceNormalAt(itidx,0);
        const Vector3& n1 = triangles->getFaceNormalAt(itidx,1);
        const Vector3& n2 = triangles->getFaceNormalAt(itidx,2);

        bool hasColor = triangles->hasColorList();

        Color4 c0 =  hasColor? triangles->getFaceColorAt(itidx,0) : phong(v0, n0, __cameraPosition, __lightPosition, __lightAmbient, __lightDiffuse, __lightSpecular, material);
        Color4 c1 =  hasColor? triangles->getFaceColorAt(itidx,1) : phong(v1, n1, __cameraPosition, __lightPosition, __lightAmbient, __lightDiffuse, __lightSpecular, material);
        Color4 c2 =  hasColor? triangles->getFaceColorAt(itidx,2) : phong(v2, n2, __cameraPosition, __lightPosition, __lightAmbient, __lightDiffuse, __lightSpecular, material);

        renderTriangle(v0,v1,v2,c0,c1,c2, ccw);

    }
}

void ZBufferEngine::renderTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, 
                                     const Color4& c0,  const Color4& c1,  const Color4& c2, 
                                     bool ccw)
{

    // Projection in camera space
    Vector3 v0Cam = worldToCamera(v0);
    Vector3 v1Cam = worldToCamera(v1);
    Vector3 v2Cam = worldToCamera(v2);

    // Convert the vertices of the triangle to raster space
    Vector3 v0Raster = cameraToRaster(v0Cam);
    Vector3 v1Raster = cameraToRaster(v1Cam);
    Vector3 v2Raster = cameraToRaster(v2Cam);

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


    // Inner loop
    // printf("x range : %u %u\n", x0, x1);
    // printf("y range : %u %u\n", y0, y1);
    for (int32_t y = y0; y <= y1; ++y) {
        for (int32_t x = x0; x <= x1; ++x) {

            Vector3 pixelSample(x + 0.5, y + 0.5, 0);

            // find weight of pixel
            real_t w0 = edgeFunction(v1Raster, v2Raster, pixelSample, ccw);
            real_t w1 = edgeFunction(v2Raster, v0Raster, pixelSample, ccw);
            real_t w2 = edgeFunction(v0Raster, v1Raster, pixelSample, ccw);

            //printf("weight of %f %f : %f %f %f\n", x + 0.5, y + 0.5, w0, w1, w2);
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;

                real_t oneOverZ = v0Raster.z() * w0 + v1Raster.z() * w1 + v2Raster.z() * w2;
                real_t z = 1. / oneOverZ;

                // Depth-buffer test
                if (__camera->isInZRange(z)){

                    // Vec2f st = st0 * w0 + st1 * w1 + st2 * w2;                        
                    // st *= z;

                    Color4 rasterColor = c0 * (w0 * z / z0) + c1 * (w1 * z / z1) + c2 * (w2 * z / z2);
                    renderRaster(x, y, z, rasterColor);
                    
                }
            }
        }
    }    
}


void ZBufferEngine::renderPoint(const TOOLS(Vector3)& v, const Color4& c, const uint32_t width)
{
    Vector3 vCam = worldToCamera(v);

    // Convert the vertices of the triangle to raster space
    Vector3 vRaster = cameraToRaster(vCam);

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


void ZBufferEngine::render(PolylinePtr polyline, MaterialPtr material, uint32_t id)
{
}

void ZBufferEngine::render(PointSetPtr pointset, MaterialPtr material, uint32_t id)
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

