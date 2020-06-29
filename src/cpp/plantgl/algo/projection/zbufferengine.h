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
#include <functional>

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

class ALGO_API ZBufferEngine : public ImageProjectionEngine {

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
  ZBufferEngine(uint16_t imageWidth, 
                uint16_t imageHeight, 
                const Color3& backGroundColor,
                eRenderingStyle style);
    
    
  ZBufferEngine(uint16_t imageWidth, 
                uint16_t imageHeight, 
                const Color4& backGroundColor,
                eRenderingStyle style);
    
  ZBufferEngine(uint16_t imageWidth, 
                uint16_t imageHeight, 
                uint32_t defaultid,
                Color4::eColor4Format conversionformat);

  ZBufferEngine(uint16_t imageWidth = 800, 
                uint16_t imageHeight = 600);    
    
  /// Destructor
  virtual ~ZBufferEngine();
  
  void setLight(const Vector3& lightPosition, const Color3& lightColor = Color3(255,255,255));
  void setLight(const Vector3& lightPosition, const Color3& lightAmbient = Color3(255,255,255), const Color3& lightDiffuse = Color3(255,255,255), const Color3& lightSpecular = Color3(255,255,255));
  void enableLight(bool enabled);
  
  void iprocess(TriangleSetPtr triangles, AppearancePtr appearance, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0);
  void iprocess(PolylinePtr polyline, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0);
  void iprocess(PointSetPtr pointset, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0);

  void beginProcess();
  void endProcess();

  void renderTriangle(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, const Color4& c0, const Color4& c1, const Color4& c2, bool ccw = true, ProjectionCameraPtr camera = ProjectionCameraPtr());
  void renderPoint(const TOOLS(Vector3)& v, const Color4& c0, const uint32_t width = 1, ProjectionCameraPtr camera = ProjectionCameraPtr());
  void renderSegment(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const Color4& c0, const Color4& c1, const uint32_t width = 1, ProjectionCameraPtr camera = ProjectionCameraPtr());

  /*
  Vector3 worldToCamera(const Vector3& vertexWorld) const;
  Vector3 cameraToNDC(const Vector3& vertexCamera) const;
  Vector3 ndcToRaster(const Vector3& vertexNDC) const;
  Vector3 cameraToRaster(const Vector3& vertexCamera) const;
  Vector3 cameraToWorld(const Vector3& vertexCamera) const;
  */

  Vector3 worldToRaster(const Vector3& vertexWorld) const 
  { return __camera->worldToRaster(vertexWorld, __imageWidth, __imageHeight); }

  Vector3 rasterToWorld(const Vector3& raster) const 
  { return __camera->rasterToWorld(raster, __imageWidth, __imageHeight); }
  
   void setFrameBufferAt(uint32_t x, uint32_t y, const Color3& rasterColor);
   void setFrameBufferAt(uint32_t x, uint32_t y, const Color4& rasterColor);
   Color3 getFrameBufferAt(uint32_t x, uint32_t y);

   ImagePtr getImage() const;
   RealArray2Ptr getDepthBuffer() const { return __depthBuffer; }

   inline bool isTotallyTransparent(const Color4& c) const { return isTotallyTransparent(c.getAlpha()); }
   bool isTotallyTransparent(const real_t alpha) const ;

   bool isVisible(int32_t x, int32_t y, real_t z) const;
   bool isVisible(const Vector3& pos) const;

   bool renderRaster(uint32_t x, uint32_t y, real_t z, const Color4& rasterColor);

  void render(ScenePtr scene);

  ImagePtr getTexture(const ImageTexturePtr imgdef);

  void renderShadedTriangle(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, bool ccw = true, const TriangleShaderPtr& shader = TriangleShaderPtr(), const ProjectionCameraPtr& camera = ProjectionCameraPtr());
  void renderShadedTriangleMT(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, bool ccw = true, const TriangleShaderPtr& shader = TriangleShaderPtr(), const ProjectionCameraPtr& camera = ProjectionCameraPtr());

  TriangleShaderPtr getShader() const { return __triangleshader; }
  void setShader(TriangleShaderPtr shader) { __triangleshader = shader; }
  void setIdRendering(uint32_t defaultid = Shape::NOID) { setShader(TriangleShaderPtr(new IdBasedShader(this, defaultid)));}

  void duplicateBuffer(const Vector3& from, const Vector3& to, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));
  void duplicateBuffer(int32_t xDiff, int32_t yDiff, real_t zDiff, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));

  void periodizeBuffer(const Vector3& from, const Vector3& to, bool useDefaultColor = true, const Color3& defaultcolor = Color3(0,0,0));
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

  bool __lightEnabled;
  Vector3 __lightPosition;
  Color3 __lightAmbient;
  Color3 __lightDiffuse;
  Color3 __lightSpecular;

  RealArray2Ptr __depthBuffer;
  FrameBufferManagerPtr __frameBuffer;

  real_t __alphathreshold;

  Cache<ImagePtr> __cachetexture;

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

