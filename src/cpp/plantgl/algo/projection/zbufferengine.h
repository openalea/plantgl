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



#ifndef __ZBufferEngine_h__
#define __ZBufferEngine_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/math/util_vector.h>
#include <plantgl/tool/util_array2.h>
#include <plantgl/tool/util_cache.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/appearance/texture.h>
#include "../algo_config.h"
#include "shading.h"
#include "projectioncamera.h"
#include "framebuffermanager.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class ZBufferEngine
    \brief A Rendering Engine based on ZBuffer.
*/

/* ----------------------------------------------------------------------- */


class ALGO_API ZBufferEngine  {

public :
    friend class Shader;
    friend class IdBasedShader;

    enum eRenderingStyle {
        eColorBased,
        eIdBased,
        eDepthOnly
    } ;

  /*! Default constructor. 
  */
  ZBufferEngine(uint16_t imageWidth = 800, 
                uint16_t imageHeight = 600, 
                const Color3& backGroundColor = Color3(0,0,0),
                eRenderingStyle style = eColorBased);
    
    
  ZBufferEngine(uint16_t imageWidth = 800, 
                uint16_t imageHeight = 600, 
                const Color4& backGroundColor = Color4::fromUint(Shape::NOID),
                eRenderingStyle style = eIdBased);
    
  ZBufferEngine(uint16_t imageWidth = 800, 
                uint16_t imageHeight = 600, 
                uint32_t defaultid = Shape::NOID,
                Color4::eColor4Format conversionformat = Color4::eARGB);

  ZBufferEngine(uint16_t imageWidth = 800, 
                uint16_t imageHeight = 600);    
    
  /// Destructor
  virtual ~ZBufferEngine();
  
  void setPerspectiveCamera(real_t angleOfView, real_t aspectRatio, real_t near, real_t far);
  void setFrustumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);
  void setOrthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);
  void lookAt(const TOOLS(Vector3)& eyePosition3D, const TOOLS(Vector3)& center3D, const TOOLS(Vector3)& upVector3D);

  void setLight(const TOOLS(Vector3)& lightPosition, const Color3& lightColor = Color3(255,255,255));
  void setLight(const TOOLS(Vector3)& lightPosition, const Color3& lightAmbient = Color3(255,255,255), const Color3& lightDiffuse = Color3(255,255,255), const Color3& lightSpecular = Color3(255,255,255));
  
  void setId(uint32_t id) { __currentId = id; }

  void render(TriangleSetPtr triangles, AppearancePtr appearance );
  void render(PolylinePtr polyline, MaterialPtr material);
  void render(PointSetPtr pointset, MaterialPtr material);

  void renderTriangle(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, const Color4& c0, const Color4& c1, const Color4& c2, bool ccw = true);
  void renderPoint(const TOOLS(Vector3)& v, const Color4& c0, const uint32_t width = 1);
  void renderSegment(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const Color4& c0, const Color4& c1, const uint32_t width = 1);

  /*
  TOOLS(Vector3) worldToCamera(const TOOLS(Vector3)& vertexWorld) const;
  TOOLS(Vector3) cameraToNDC(const TOOLS(Vector3)& vertexCamera) const;
  TOOLS(Vector3) ndcToRaster(const TOOLS(Vector3)& vertexNDC) const;
  TOOLS(Vector3) cameraToRaster(const TOOLS(Vector3)& vertexCamera) const;
  TOOLS(Vector3) cameraToWorld(const TOOLS(Vector3)& vertexCamera) const;
  */

  TOOLS(Vector3) worldToRaster(const TOOLS(Vector3)& vertexWorld) const 
  { return __camera->worldToRaster(vertexWorld, __imageWidth, __imageHeight); }

  TOOLS(Vector3) rasterToWorld(const TOOLS(Vector3)& raster) const 
  { return __camera->rasterToWorld(raster, __imageWidth, __imageHeight); }
  
   void setFrameBufferAt(uint32_t x, uint32_t y, const Color3& rasterColor);
   void setFrameBufferAt(uint32_t x, uint32_t y, const Color4& rasterColor);
   Color3 getFrameBufferAt(uint32_t x, uint32_t y);

   ImagePtr getImage() const;
   TOOLS(RealArray2Ptr) getDepthBuffer() const { return __depthBuffer; }

   BoundingBoxPtr getBoundingBoxView() const;

   inline bool isTotallyTransparent(const Color4& c) const { return isTotallyTransparent(c.getAlpha()); }
   bool isTotallyTransparent(const real_t alpha) const ;

   bool isVisible(int32_t x, int32_t y, real_t z) const;
   bool isVisible(const TOOLS(Vector3)& pos) const;

   bool renderRaster(uint32_t x, uint32_t y, real_t z, const Color4& rasterColor);

  void transformModel(const TOOLS(Matrix4)& transform);
  void pushModelTransformation();
  void popModelTransformation();
  void transformModelIdentity();

  void translateModel(const TOOLS(Vector3)& v);
  void scaleModel(const TOOLS(Vector3)& v);

  void render(ScenePtr scene);

  ImagePtr getTexture(const ImageTexturePtr imgdef);

  void renderShadedTriangle(TriangleShaderPtr shader, const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, bool ccw = true);

  TriangleShaderPtr getShader() const { return __triangleshader; }
  void setShader(TriangleShaderPtr shader) { __triangleshader = shader; }
  void setIdRendering(uint32_t defaultid = Shape::NOID) { setShader(TriangleShaderPtr(new IdBasedShader(this, defaultid)));}

  void duplicateBuffer(const TOOLS(Vector3)& from, const TOOLS(Vector3)& to, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));
  void duplicateBuffer(int32_t xDiff, int32_t yDiff, real_t zDiff, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));

  void periodizeBuffer(const TOOLS(Vector3)& from, const TOOLS(Vector3)& to, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));
  void periodizeBuffer(int32_t xDiff, int32_t yDiff, real_t zDiff, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));

  void setFrameBuffer(FrameBufferManagerPtr fb) { __frameBuffer = fb; }
  FrameBufferManagerPtr getFrameBuffer() const { return __frameBuffer; }

  ProjectionCameraPtr camera() const { return __camera; }

protected :

  void _bufferPeriodizationStep(int32_t xDiff, int32_t yDiff, real_t zDiff, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));

  uint16_t __imageWidth;
  uint16_t __imageHeight;
  ProjectionCameraPtr __camera;

  TOOLS(Vector3) __lightPosition;
  Color3 __lightAmbient;
  Color3 __lightDiffuse;
  Color3 __lightSpecular;

  TOOLS(RealArray2Ptr) __depthBuffer;
  FrameBufferManagerPtr __frameBuffer;

  real_t __alphathreshold;
  uint32_t __currentId;

  TOOLS(Cache)<ImagePtr> __cachetexture;

  TriangleShaderPtr __triangleshader;
  
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

