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

/*! \file ZBufferEngine.h
    \brief Definition of Rendering Engine based on ZBuffer.
*/



#ifndef __ZBufferShading_h__
#define __ZBufferShading_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/appearance/texture.h>
#include <plantgl/scenegraph/appearance/util_image.h>
#include "../algo_config.h"
// #include "zbufferengine.h"
/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class ZBufferShading
    \brief Shading tools for ZBufferEngine.
*/

/* ----------------------------------------------------------------------- */


Color4 phong(const TOOLS(Vector3)& v, const TOOLS(Vector3)& n, 
             const TOOLS(Vector3)& cameraPosition, 
             const TOOLS(Vector3)& lightPosition, 
             const Color3& lightAmbient, const Color3& lightDiffuse, const Color3& lightSpecular,
             MaterialPtr material);

class ZBufferEngine;

class Shader : public TOOLS(RefCountObject) {
public:
    Shader(ZBufferEngine * engine);
    virtual ~Shader();
    ZBufferEngine * __engine;

    const TOOLS(Vector3)& cameraPosition() const ;
    const TOOLS(Vector3)& lightPosition() const ;
    const Color3& lightAmbient() const ;
    const Color3& lightDiffuse() const ;
    const Color3& lightSpecular() const ;
};

typedef RCPtr<Shader> ShaderPtr;



class TriangleShader : public Shader {
public:
    TriangleShader(ZBufferEngine * engine);
    virtual ~TriangleShader();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid) = 0;
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) = 0;

};

typedef RCPtr<TriangleShader> TriangleShaderPtr;

enum eShadingStyle {
    eTextureBased,
    eMaterialBased,
    eColorListBased
} ;

class IdBasedShader : public TriangleShader {
public:
    IdBasedShader(ZBufferEngine * engine, uint32_t defaultid = Shape::NOID,
                  Color4::eColor4Format conversionformat = Color4::eARGB);
    virtual ~IdBasedShader();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;

    uint32_t shapeid;
    uint32_t defaultid;
    Color4::eColor4Format conversionformat;
};

class TextureShader : public TriangleShader {
public:
    TextureShader(ZBufferEngine * engine);
    virtual ~TextureShader();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;

    ImagePtr image;
    TOOLS(Vector2) uv0;
    TOOLS(Vector2) uv1;
    TOOLS(Vector2) uv2;
    bool repeatu;
    bool repeatv;
};


class ColorBasedShader : public TriangleShader {
public:
    ColorBasedShader(ZBufferEngine * engine);
    virtual ~ColorBasedShader();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;

    void setColors(const Color4& _c0, const Color4& _c1, const Color4& _c2) { c0 = _c0; c1 = _c1; c2 = _c2; }
    Color4 c0;
    Color4 c1;
    Color4 c2;
};


class GouraudInterpolation : public TriangleShader {
public:
    GouraudInterpolation(ZBufferEngine * engine);
    virtual ~GouraudInterpolation();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;

    Color4 c0;
    Color4 c1;
    Color4 c2;

};

class PhongInterpolation : public TriangleShader {
public:
    PhongInterpolation(ZBufferEngine * engine);
    virtual ~PhongInterpolation();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;

    TOOLS(Vector3) n0;
    TOOLS(Vector3) n1;
    TOOLS(Vector3) n2;

    MaterialPtr material;

};

class TriangleShaderSelector : public TriangleShader {
public:
    TriangleShaderSelector(ZBufferEngine * engine);
    virtual ~TriangleShaderSelector();

    void registerShader(eShadingStyle, TriangleShaderPtr);

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;

    pgl_hash_map<int32_t, TriangleShaderPtr> __shadermap;
    TriangleShaderPtr __current;
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

