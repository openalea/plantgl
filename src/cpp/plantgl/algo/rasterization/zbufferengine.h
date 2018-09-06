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
#include <plantgl/math/util_matrix.h>
#include <plantgl/tool/util_array2.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/appearance/util_image.h>
#include "../algo_config.h"
#include <stack>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class ZBufferEngine
    \brief A Rendering Engine based on ZBuffer.
*/

/* ----------------------------------------------------------------------- */

class OrthographicProjection : public TOOLS(RefCountObject){
public:
    OrthographicProjection(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);

   virtual ~OrthographicProjection();

   TOOLS(Vector3) screen2NDC(const real_t& xScreen, const real_t& yScreen, const real_t z) const ;

   virtual TOOLS(Vector3) projecttoNDC(const TOOLS(Vector3)& vertexCamera) const ;

   bool isInZRange(real_t z) const ;
   bool isInZRange(real_t zmin, real_t zmax) const;

   real_t __xscale;
   real_t __xconstant;
   real_t __yscale;
   real_t __yconstant;

   real_t left;
   real_t right;
   real_t bottom;
   real_t top;
   real_t near;
   real_t far;

};

class PerspectiveProjection : public OrthographicProjection {
public:
    PerspectiveProjection(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);

    virtual ~PerspectiveProjection();
 
    virtual TOOLS(Vector3) projecttoNDC(const TOOLS(Vector3)& vertexCamera) const ;
};

typedef RCPtr<OrthographicProjection> ProjectionPtr;


class FrameBufferManager : public TOOLS(RefCountObject) {
public:
    FrameBufferManager(uint16_t imageWidth, uint16_t imageHeight, const Color3& backGroundColor) {}
    virtual ~FrameBufferManager() {}
    virtual void setPixelAt(uint_t x, uint_t y, const Color3& color) = 0;
    virtual Color3 getPixelAt(uint_t x, uint_t y) const = 0;
    virtual uint16_t width() const = 0;
    virtual uint16_t height() const = 0;
};

typedef RCPtr<FrameBufferManager> FrameBufferManagerPtr;


class PglFrameBufferManager : public FrameBufferManager {
public:
    PglFrameBufferManager(uint16_t imageWidth, uint16_t imageHeight, const Color3& backGroundColor);
    virtual ~PglFrameBufferManager();
    virtual void setPixelAt(uint_t x, uint_t y, const Color3& color);
    virtual Color3 getPixelAt(uint_t x, uint_t y) const ;
    virtual uint16_t width() const;
    virtual uint16_t height() const;
    ImagePtr getImage() const { return __image; }    

    ImagePtr __image;
};

typedef RCPtr<PglFrameBufferManager> PglFrameBufferManagerPtr;



Color4 phong(const TOOLS(Vector3)& v, const TOOLS(Vector3)& n, 
             const TOOLS(Vector3)& cameraPosition, 
             const TOOLS(Vector3)& lightPosition, 
             const Color3& lightAmbient, const Color3& lightDiffuse, const Color3& lightSpecular,
             MaterialPtr material);

class ALGO_API ZBufferEngine  {

    public :

  /*! Default constructor. 
  */
  ZBufferEngine(uint16_t imageWidth = 800, uint16_t imageHeight = 600, const Color3& backGroundColor = Color3(0,0,0));
    
    
  /// Destructor
  virtual ~ZBufferEngine();
  
  void setPerspectiveCamera(real_t angleOfView, real_t aspectRatio, real_t near, real_t far);
  
  void setFrustrumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);

  void setOrthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);

  void lookAt(const TOOLS(Vector3)& eyePosition3D, const TOOLS(Vector3)& center3D, const TOOLS(Vector3)& upVector3D);

  void setLight(const TOOLS(Vector3)& lightPosition, const Color3& lightColor = Color3(255,255,255));
  void setLight(const TOOLS(Vector3)& lightPosition, const Color3& lightAmbient = Color3(255,255,255), const Color3& lightDiffuse = Color3(255,255,255), const Color3& lightSpecular = Color3(255,255,255));
  
  void render(TriangleSetPtr triangles, MaterialPtr material, uint32_t id);
  void render(PolylinePtr polyline, MaterialPtr material, uint32_t id);
  void render(PointSetPtr pointset, MaterialPtr material, uint32_t id);

  void renderTriangle(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, const Color4& c0, const Color4& c1, const Color4& c2, bool ccw = true);
  void renderPoint(const TOOLS(Vector3)& v, const Color4& c0, const uint32_t width = 1);
  void renderSegment(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const Color4& c0, const Color4& c1, const uint32_t width = 1);

  TOOLS(Vector3) worldToRaster(const TOOLS(Vector3)& vertexWorld) const;
  TOOLS(Vector3) worldToCamera(const TOOLS(Vector3)& vertexWorld) const;
  TOOLS(Vector3) cameraToNDC(const TOOLS(Vector3)& vertexCamera) const;
  TOOLS(Vector3) ndcToRaster(const TOOLS(Vector3)& vertexNDC) const;
  TOOLS(Vector3) cameraToRaster(const TOOLS(Vector3)& vertexCamera) const;

    void setFrameBufferAt(uint32_t x, uint32_t y, const Color3& rasterColor);
   void setFrameBufferAt(uint32_t x, uint32_t y, const Color4& rasterColor);
   Color3 getFrameBufferAt(uint32_t x, uint32_t y);

   ImagePtr getImage() const;

   BoundingBoxPtr getBoundingBoxView() const;

   TOOLS(Matrix4) getWorldToCameraMatrix() const { return __worldToCamera; }

   inline bool isTotallyTransparent(const Color4& c) const { return isTotallyTransparent(c.getAlpha()); }
   bool isTotallyTransparent(const real_t alpha) const ;

   bool renderRaster(uint32_t x, uint32_t y, real_t z, const Color4& rasterColor);

  void transformModel(const TOOLS(Matrix4)& transform);
  void pushModelTransformation();
  void popModelTransformation();
  void transformModelIdentity();

  void translateModel(const TOOLS(Vector3)& v);
  void scaleModel(const TOOLS(Vector3)& v);

  void render(ScenePtr scene);

protected :

  
  TOOLS(Matrix4) __worldToCamera;
  std::stack<TOOLS(Matrix4)> __modelMatrixStack;
  TOOLS(Matrix4) __currentModelMatrix;
  TOOLS(Matrix4) __currentWorldToCamera;

  uint16_t __imageWidth;
  uint16_t __imageHeight;
  TOOLS(Vector3) __cameraPosition;
  ProjectionPtr __camera;

  TOOLS(Vector3) __lightPosition;
  Color3 __lightAmbient;
  Color3 __lightDiffuse;
  Color3 __lightSpecular;

  TOOLS(RealArray2Ptr) __depthBuffer;
  FrameBufferManagerPtr __frameBuffer;

  real_t __alphathreshold;
  
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

