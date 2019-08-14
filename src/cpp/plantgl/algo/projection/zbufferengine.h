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
#include <plantgl/scenegraph/container/indexarray.h>
#include "../algo_config.h"
#include "shading.h"
#include "projectionengine.h"
#include "framebuffermanager.h"
#include "imagemutex.h"
#include <condition_variable>
// #include <boost/fiber/mutex.hpp>
#include <atomic>

/* ----------------------------------------------------------------------- */


namespace boost { namespace asio { class thread_pool; };};

PGL_BEGIN_NAMESPACE

class ThreadManager;

class ThreadManager {
public:
    ~ThreadManager();

    void new_task(std::function<void()> task);
    void join();
    size_t nb_threads() { return __nb_threads; }

    // Singleton access
    static ThreadManager& get();
protected:


    ThreadManager();
    boost::asio::thread_pool * getPool();

    bool hasActiveTasks() const { return __nb_tasks > 0; }
    bool hasCompletedTasks() const ;

    void process_task(std::function<void()> task);

    boost::asio::thread_pool * __pool;
    size_t __nb_threads;

    std::atomic<uint32_t> __nb_tasks;
    std::condition_variable  __condition;
    std::mutex   __condition_mutex;
    std::unique_lock< std::mutex > __condition_lock ;
    std::mutex   __threadend_mutex;

    static ThreadManager THREADMANAGER;
};




/* ----------------------------------------------------------------------- */

/** 
    \class ZBufferEngine
    \brief A Rendering Engine based on ZBuffer.
*/

/* ----------------------------------------------------------------------- */

class ZBufferEngine;

class ALGO_API ZBufferEngine : public ProjectionEngine {

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
  
  void setLight(const TOOLS(Vector3)& lightPosition, const Color3& lightColor = Color3(255,255,255));
  void setLight(const TOOLS(Vector3)& lightPosition, const Color3& lightAmbient = Color3(255,255,255), const Color3& lightDiffuse = Color3(255,255,255), const Color3& lightSpecular = Color3(255,255,255));
  
  void iprocess(TriangleSetPtr triangles, AppearancePtr appearance, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0);
  void iprocess(PolylinePtr polyline, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0);
  void iprocess(PointSetPtr pointset, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0);

  void beginProcess();
  void endProcess();


  void renderTriangle(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, const Color4& c0, const Color4& c1, const Color4& c2, bool ccw = true, ProjectionCameraPtr camera = ProjectionCameraPtr());
  void renderPoint(const TOOLS(Vector3)& v, const Color4& c0, const uint32_t width = 1, ProjectionCameraPtr camera = ProjectionCameraPtr());
  void renderSegment(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const Color4& c0, const Color4& c1, const uint32_t width = 1, ProjectionCameraPtr camera = ProjectionCameraPtr());

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

   inline bool isTotallyTransparent(const Color4& c) const { return isTotallyTransparent(c.getAlpha()); }
   bool isTotallyTransparent(const real_t alpha) const ;

   bool isVisible(int32_t x, int32_t y, real_t z) const;
   bool isVisible(const TOOLS(Vector3)& pos) const;

   bool renderRaster(uint32_t x, uint32_t y, real_t z, const Color4& rasterColor);

  void render(ScenePtr scene);

  ImagePtr getTexture(const ImageTexturePtr imgdef);

  void renderShadedTriangle(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, bool ccw = true, const TriangleShaderPtr& shader = TriangleShaderPtr(), const ProjectionCameraPtr& camera = ProjectionCameraPtr());
  void renderShadedTriangleMT(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, bool ccw = true, const TriangleShaderPtr& shader = TriangleShaderPtr(), const ProjectionCameraPtr& camera = ProjectionCameraPtr());

  TriangleShaderPtr getShader() const { return __triangleshader; }
  void setShader(TriangleShaderPtr shader) { __triangleshader = shader; }
  void setIdRendering(uint32_t defaultid = Shape::NOID) { setShader(TriangleShaderPtr(new IdBasedShader(this, defaultid)));}

  void duplicateBuffer(const TOOLS(Vector3)& from, const TOOLS(Vector3)& to, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));
  void duplicateBuffer(int32_t xDiff, int32_t yDiff, real_t zDiff, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));

  void periodizeBuffer(const TOOLS(Vector3)& from, const TOOLS(Vector3)& to, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));
  void periodizeBuffer(int32_t xDiff, int32_t yDiff, real_t zDiff, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));

  void setFrameBuffer(FrameBufferManagerPtr fb) { __frameBuffer = fb; }
  FrameBufferManagerPtr getFrameBuffer() const { return __frameBuffer; }

  bool isMultiThreaded() const { return __multithreaded; }
  void setMultiThreaded(bool value) { __multithreaded = value; }

  virtual void process(ScenePtr scene);


protected :

  void _bufferPeriodizationStep(int32_t xDiff, int32_t yDiff, real_t zDiff, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));

  void rasterize(int32_t x0, int32_t x1, int32_t y0, int32_t y1,
                 TOOLS(Vector3) v0Raster, TOOLS(Vector3) v1Raster, TOOLS(Vector3) v2Raster, bool ccw, 
                 const TriangleShaderPtr& shader, const ProjectionCameraPtr& camera);
  void rasterizeMT(const Index4& rect,
                 const TOOLS(Vector3)& v0Raster, const TOOLS(Vector3)& v1Raster, const TOOLS(Vector3)& v2Raster, bool ccw, 
                 const TriangleShaderPtr& shader, const ProjectionCameraPtr& camera);

  void processScene(Scene::const_iterator scene_begin, Scene::const_iterator scene_end, ProjectionCameraPtr camera, uint32_t threadid = 0);
  void processSceneMT(Scene::const_iterator scene_begin, Scene::const_iterator scene_end, ProjectionCameraPtr camera, uint32_t threadid = 0);

  void lock(uint_t x, uint_t y);
  void unlock(uint_t x, uint_t y);
  bool tryLock(uint_t x, uint_t y);

  uint16_t __imageWidth;
  uint16_t __imageHeight;

  TOOLS(Vector3) __lightPosition;
  Color3 __lightAmbient;
  Color3 __lightDiffuse;
  Color3 __lightSpecular;

  TOOLS(RealArray2Ptr) __depthBuffer;
  FrameBufferManagerPtr __frameBuffer;

  real_t __alphathreshold;

  TOOLS(Cache)<ImagePtr> __cachetexture;

  TriangleShaderPtr __triangleshader;
  TriangleShaderPtr * __triangleshaderset;

  bool __multithreaded;
  ImageMutexPtr __imageMutex;


  static ImageMutexPtr getImageMutex(uint16_t imageWidth, uint16_t imageHeight);


private:
    static ImageMutexPtr IMAGEMUTEX;
  
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

