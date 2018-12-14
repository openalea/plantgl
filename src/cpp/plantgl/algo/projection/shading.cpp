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

#include "shading.h"
#include "zbufferengine.h"
/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE




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



Shader::Shader(ZBufferEngine * engine) : RefCountObject(), __engine(engine) {}
Shader::~Shader() {}

const TOOLS(Vector3)& Shader::cameraPosition() const { return __engine->camera()->position(); }  
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

void IdBasedShader::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t _shapeid) 
{ shapeid = _shapeid; }
void IdBasedShader::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) 
{    
 __engine->getFrameBuffer()->setPixel4At(x,y,Color4::fromUint(shapeid, conversionformat));   
}

TextureShader::TextureShader(ZBufferEngine * engine) : TriangleShader(engine) {}
TextureShader::~TextureShader() {}

void TextureShader::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid)
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

void TextureShader::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) 
{
    Vector2 uv = uv0 * w0 + uv1 * w1 + uv2 * w2;
    Color4 rasterColor;
    if(is_valid_ptr(image))rasterColor = image->getPixelAtUV(uv.x(), uv.y(), repeatu, repeatv);
    __engine->setFrameBufferAt(x,y,rasterColor);    
}

ColorBasedShader::ColorBasedShader(ZBufferEngine * engine): TriangleShader(engine) {}
ColorBasedShader::~ColorBasedShader() {}

void ColorBasedShader::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid)
{ 
    c0 =  triangles->getFaceColorAt(trid,0);
    c1 =  triangles->getFaceColorAt(trid,1);
    c2 =  triangles->getFaceColorAt(trid,2);
}

void ColorBasedShader::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) 
{
    Color4 rasterColor = c0 * w0  + c1 * w1  + c2 * w2 ;
    __engine->setFrameBufferAt(x,y,rasterColor);    
}


GouraudInterpolation::GouraudInterpolation(ZBufferEngine * engine) : TriangleShader(engine) {}
GouraudInterpolation::~GouraudInterpolation() {}

void GouraudInterpolation::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid)
{ 
    MaterialPtr material = dynamic_pointer_cast<Material>(appearance);

    if (material){
        const Vector3& v0 = triangles->getFacePointAt(trid,0);
        const Vector3& v1 = triangles->getFacePointAt(trid,1);
        const Vector3& v2 = triangles->getFacePointAt(trid,2);

        const Vector3& n0 = triangles->getFaceNormalAt(trid,0);
        const Vector3& n1 = triangles->getFaceNormalAt(trid,1);
        const Vector3& n2 = triangles->getFaceNormalAt(trid,2);

        c0 =  phong(v0, n0,  cameraPosition(), lightPosition(), lightAmbient(), lightDiffuse(), lightSpecular(), material);
        c1 =  phong(v1, n1,  cameraPosition(), lightPosition(), lightAmbient(), lightDiffuse(), lightSpecular(), material);
        c2 =  phong(v2, n2,  cameraPosition(), lightPosition(), lightAmbient(), lightDiffuse(), lightSpecular(), material);
    }
}

void GouraudInterpolation::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2)
{
    Color4 rasterColor = c0 * w0  + c1 * w1  + c2 * w2 ;
    __engine->setFrameBufferAt(x,y,rasterColor);

}


PhongInterpolation::PhongInterpolation(ZBufferEngine * engine) : TriangleShader(engine) {}
PhongInterpolation::~PhongInterpolation() {}

void PhongInterpolation::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid)
{
    material = dynamic_pointer_cast<Material>(appearance);

    n0 = triangles->getFaceNormalAt(trid,0);
    n1 = triangles->getFaceNormalAt(trid,1);
    n2 = triangles->getFaceNormalAt(trid,2);
}
void PhongInterpolation::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2)
{
    Vector3 v = __engine->rasterToWorld(Vector3(x,y,z));
    Vector3 normal = n0 * w0 + n1 * w1 + n2 * w2;
    Color4 rasterColor =  phong(v, normal,  cameraPosition(), lightPosition(), lightAmbient(), lightDiffuse(), lightSpecular(), material);

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

void TriangleShaderSelector::init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid)
{
    if (triangles->hasColorList()) __current = __shadermap[eColorListBased];
    else if (is_valid_ptr(dynamic_pointer_cast<Texture2D>(appearance))) __current = __shadermap[eTextureBased];
    else __current = __shadermap[eMaterialBased];
    __current->init(appearance, triangles, trid, shapeid);
}

void TriangleShaderSelector::process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) 
{
    __current->process(x, y, z, w0, w1, w2);
}


