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
#include "projectioncamera.h"
/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class ZBufferShading
    \brief Shading tools for ZBufferEngine.
*/

/* ----------------------------------------------------------------------- */


Color4 phong(const Vector3& v, const Vector3& n, 
             const Vector3& cameraPosition, 
             const Vector3& lightPosition, 
             const Color3& lightAmbient, const Color3& lightDiffuse, const Color3& lightSpecular,
             MaterialPtr material);

class ZBufferEngine;

class Shader : public RefCountObject {
public:
    Shader(ZBufferEngine * engine);
    virtual ~Shader();
    ZBufferEngine * __engine;
    ProjectionCameraPtr __camera;

    void initEnv(const ProjectionCameraPtr& camera);

    const Vector3& cameraPosition() const ;
    const Vector3& lightPosition() const ;
    const Color3& lightAmbient() const ;
    const Color3& lightDiffuse() const ;
    const Color3& lightSpecular() const ;
};

typedef RCPtr<Shader> ShaderPtr;


class TriangleShader;
typedef RCPtr<TriangleShader> TriangleShaderPtr;

class TriangleShader : public Shader {
public:
    TriangleShader(ZBufferEngine * engine);
    virtual ~TriangleShader();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera) = 0;
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) = 0;
    virtual TriangleShader * copy(bool deep = false) const = 0;

};


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

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;
    virtual TriangleShader * copy(bool deep = false) const;

    uint32_t shapeid;
    uint32_t defaultid;
    Color4::eColor4Format conversionformat;
};

class TextureShader : public TriangleShader {
public:
    TextureShader(ZBufferEngine * engine);
    virtual ~TextureShader();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;
    virtual TriangleShader * copy(bool deep = false) const;

    ImagePtr image;
    Vector2 uv0;
    Vector2 uv1;
    Vector2 uv2;
    bool repeatu;
    bool repeatv;
};


class ColorBasedShader : public TriangleShader {
public:
    ColorBasedShader(ZBufferEngine * engine);
    virtual ~ColorBasedShader();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;
    virtual TriangleShader * copy(bool deep = false) const;

    void setColors(const Color4& _c0, const Color4& _c1, const Color4& _c2) { c0 = _c0; c1 = _c1; c2 = _c2; }
    Color4 c0;
    Color4 c1;
    Color4 c2;
};


class GouraudInterpolation : public TriangleShader {
public:
    GouraudInterpolation(ZBufferEngine * engine);
    virtual ~GouraudInterpolation();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;
    virtual TriangleShader * copy(bool deep = false) const;

    Color4 c0;
    Color4 c1;
    Color4 c2;

};

class PhongInterpolation : public TriangleShader {
public:
    PhongInterpolation(ZBufferEngine * engine);
    virtual ~PhongInterpolation();

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;
    virtual TriangleShader * copy(bool deep = false) const;

    Vector3 n0;
    Vector3 n1;
    Vector3 n2;

    MaterialPtr material;

};

class TriangleShaderSelector : public TriangleShader {
public:
    TriangleShaderSelector(ZBufferEngine * engine);
    virtual ~TriangleShaderSelector();

    void registerShader(eShadingStyle, TriangleShaderPtr);

    virtual void init(AppearancePtr appearance, TriangleSetPtr triangles, uint32_t trid, uint32_t shapeid, const ProjectionCameraPtr& camera);
    virtual void process(int32_t x, int32_t y, int32_t z, float w0, float w1, float w2) ;
    virtual TriangleShader * copy(bool deep = false) const;

    pgl_hash_map<int32_t, TriangleShaderPtr> __shadermap;
    TriangleShaderPtr __current;
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

