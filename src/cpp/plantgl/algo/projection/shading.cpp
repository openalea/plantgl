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

#include "shading.h"
#include "zbufferengine.h"
/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE




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



Shader::Shader(ZBufferEngine * engine) : RefCountObject(), __engine(engine), __camera() {}
Shader::~Shader() {}


void Shader::initEnv(const ProjectionCameraPtr& camera) { __camera = camera; }


const TOOLS(Vector3)& Shader::cameraPosition() const { return __camera->position(); }  
const TOOLS(Vector3)& Shader::lightPosition() const { return __engine->__lightPosition; }  
const Color3& Shader::lightAmbient() const { return __engine->__lightAmbient; }  
const Color3& Shader::lightDiffuse() const { return __engine->__lightDiffuse; }  
const Color3& Shader::lightSpecular() const { return __engine->__lightSpecular; }  

TriangleShader::TriangleShader(ZBufferEngine * engine) : Shader(engine) {}
TriangleShader::~TriangleShader() {}


IdBasedShader::IdBasedShader(ZBufferEngine * engine, uint32_t _defaultid, Color4::eColor4Format _conversionformat) : 
    TriangleShader(engine), defaultid(_defaultid), conversionformat(_conversionformat) 
{
    Color4 defcol = Color4::fromUint(_defaultid, conversionformat);
    engine->__frameBuffer = new PglFrameBufferManager(engine->__imageWidth, engine->__imageHeight, 4, defcol);

    // if(engine->getImage()->nbChannels() != 4){
    //     engine->getImage()->setNbChannels(4);
    //}
}
IdBasedShader::~IdBasedShader() {}

void IdBasedShader::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t _shapeid, const ProjectionCameraPtr& camera) 
{ shapeid = _shapeid;}

TriangleShader *  IdBasedShader::copy(bool deep) const
{
    return new IdBasedShader(*this);    
}


void IdBasedShader::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) 
{    
 __engine->getFrameBuffer()->setPixel4At(x,y,Color4::fromUint(shapeid, conversionformat));   
}

TextureShader::TextureShader(ZBufferEngine * engine) : TriangleShader(engine) {}
TextureShader::~TextureShader() {}

void TextureShader::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera)
{
    Texture2DPtr texture = dynamic_pointer_cast<Texture2D>(appearance);
    if (is_valid_ptr(texture) && is_valid_ptr(texture->getImage())){
        image = __engine->getTexture(texture->getImage());
        uv0 = triangles->getFaceTexCoordAt(trid, 0);    
        uv1 = triangles->getFaceTexCoordAt(trid, 1);    
        uv2 = triangles->getFaceTexCoordAt(trid, 2);    
        if (texture->getTransformation()){
            uv0 = texture->getTransformation()->transform(uv0);
            uv1 = texture->getTransformation()->transform(uv1);
            uv2 = texture->getTransformation()->transform(uv2);
        }
        repeatu = texture->getImage()->getRepeatS();
        repeatv = texture->getImage()->getRepeatT();
    }
}

TriangleShader *  TextureShader::copy(bool deep) const
{
    return new TextureShader(*this);    
}

void TextureShader::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) 
{
    Vector2 uv = uv0 * w0 + uv1 * w1 + uv2 * w2;
    Color4 rasterColor;
    if(is_valid_ptr(image))rasterColor = image->getPixelAtUV(uv.x(), uv.y(), repeatu, repeatv);
    __engine->setFrameBufferAt(x,y,rasterColor);    
}

ColorBasedShader::ColorBasedShader(ZBufferEngine * engine): TriangleShader(engine) {}
ColorBasedShader::~ColorBasedShader() {}

void ColorBasedShader::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera)
{ 
    if (triangles->hasColorList()) {
        c0 =  triangles->getFaceColorAt(trid,0);
        c1 =  triangles->getFaceColorAt(trid,1);
        c2 =  triangles->getFaceColorAt(trid,2);
    }
    else {
        MaterialPtr material = dynamic_pointer_cast<Material>(appearance);
        if(is_valid_ptr(material)) {
            c0 = material->getAmbient();
            c1 = material->getAmbient();
            c2 = material->getAmbient();
        }
        else {
            c0 = Color3::BLACK;
            c1 = Color3::BLACK;
            c2 = Color3::BLACK;
        }
    }
}

TriangleShader *  ColorBasedShader::copy(bool deep) const
{
    return new ColorBasedShader(*this);    
}

void ColorBasedShader::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) 
{
    Color4 rasterColor = c0 * w0  + c1 * w1  + c2 * w2 ;
    __engine->setFrameBufferAt(x,y,rasterColor);    
}


GouraudInterpolation::GouraudInterpolation(ZBufferEngine * engine) : TriangleShader(engine) {}
GouraudInterpolation::~GouraudInterpolation() {}

void GouraudInterpolation::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera)
{ 
    MaterialPtr material = dynamic_pointer_cast<Material>(appearance);

    if (material){
        const Vector3& v0 = triangles->getFacePointAt(trid,0);
        const Vector3& v1 = triangles->getFacePointAt(trid,1);
        const Vector3& v2 = triangles->getFacePointAt(trid,2);

        const Vector3& n0 = triangles->getFaceNormalAt(trid,0);
        const Vector3& n1 = triangles->getFaceNormalAt(trid,1);
        const Vector3& n2 = triangles->getFaceNormalAt(trid,2);

        c0 =  phong(v0, n0,  camera->position(), lightPosition(), lightAmbient(), lightDiffuse(), lightSpecular(), material);
        c1 =  phong(v1, n1,  camera->position(), lightPosition(), lightAmbient(), lightDiffuse(), lightSpecular(), material);
        c2 =  phong(v2, n2,  camera->position(), lightPosition(), lightAmbient(), lightDiffuse(), lightSpecular(), material);
    }
}

TriangleShader *  GouraudInterpolation::copy(bool deep) const
{
    return new GouraudInterpolation(*this);    
}


void GouraudInterpolation::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2)
{
    Color4 rasterColor = c0 * w0  + c1 * w1  + c2 * w2 ;
    __engine->setFrameBufferAt(x,y,rasterColor);

}


PhongInterpolation::PhongInterpolation(ZBufferEngine * engine) : TriangleShader(engine) {}
PhongInterpolation::~PhongInterpolation() {}

void PhongInterpolation::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera)
{
    material = dynamic_pointer_cast<Material>(appearance);

    n0 = triangles->getFaceNormalAt(trid,0);
    n1 = triangles->getFaceNormalAt(trid,1);
    n2 = triangles->getFaceNormalAt(trid,2);
}

TriangleShader *  PhongInterpolation::copy(bool deep ) const
{
    return new PhongInterpolation(*this);    
}

void PhongInterpolation::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2)
{
    Vector3 v = __engine->rasterToWorld(Vector3(x,y,z));
    Vector3 normal = n0 * w0 + n1 * w1 + n2 * w2;
    Color4 rasterColor =  phong(v, normal,  cameraPosition(), lightPosition(), lightAmbient(), lightDiffuse(), lightSpecular(), material);
    __engine->setFrameBufferAt(x,y,rasterColor);
}

TriangleShaderSelector::TriangleShaderSelector(ZBufferEngine * engine) : TriangleShader(engine) 
{
    registerShader(eTextureBased, TriangleShaderPtr(new TextureShader(__engine)));    
    registerShader(eMaterialBased, TriangleShaderPtr(new GouraudInterpolation(__engine)));    
    registerShader(eColorListBased, TriangleShaderPtr(new ColorBasedShader(__engine)));    
}

TriangleShaderSelector::~TriangleShaderSelector() {}

void TriangleShaderSelector::registerShader(eShadingStyle style, TriangleShaderPtr shader)
{
    __shadermap[style] = shader;    
}

void TriangleShaderSelector::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera)
{
    if (triangles->hasColorList()) __current = __shadermap[eColorListBased];
    else if (is_valid_ptr(dynamic_pointer_cast<Texture2D>(appearance))) __current = __shadermap[eTextureBased];
    else __current = __shadermap[eMaterialBased];
    __current->init(appearance, triangles, trid, shapeid, camera);
}

void TriangleShaderSelector::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) 
{
    __current->process(x, y, z, w0, w1, w2);
}

TriangleShader *  TriangleShaderSelector::copy(bool deep) const
{
    if (deep) {
        TriangleShaderSelector * res = new TriangleShaderSelector(*this);
        pgl_hash_map<int32_t, TriangleShaderPtr> mymap;
        for (pgl_hash_map<int32_t, TriangleShaderPtr>::const_iterator it = __shadermap.begin(); it != __shadermap.end(); ++it)
            mymap[it->first] = TriangleShaderPtr(it->second->copy());
        res->__shadermap = mymap;
        return res;
    }
    else 
    {
        return __current->copy();
    }
}


