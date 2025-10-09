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
             

/*! \file ProjectionEngine.h
    \brief Definition of An abstract Engine with projection capabilities.
*/



#ifndef __ProjectionEngine_h__
#define __ProjectionEngine_h__

/* ----------------------------------------------------------------------- */

#include "projectioncamera.h"
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/appearance/texture.h>
#include <plantgl/tool/util_hashmap.h>
/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class ProjectionEngine
    \brief An abstract Engine with projection capabilities.
*/

/* ----------------------------------------------------------------------- */


class ALGO_API ProjectionEngine  {

public :
    enum eIdPolicy {
        ePrimitiveIdBased,
        eShapeIdBased
    } ;

    typedef pgl_hash_map<uint32_t,uint32_t> IdMap;

      ProjectionEngine(eIdPolicy idPolicy = eShapeIdBased) ;

      virtual ~ProjectionEngine();

      inline void setPerspectiveCamera(real_t verticalAngleOfView, real_t aspectRatio, real_t near, real_t far)
      { __camera = ProjectionCamera::perspectiveCamera(verticalAngleOfView, aspectRatio, near, far); }

      inline void setFrustumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
      { __camera = ProjectionCamera::frustumCamera(left, right, bottom, top, near, far); }

      inline void setOrthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
      {  __camera = ProjectionCamera::orthographicCamera(left, right, bottom, top, near, far); }

      inline void lookAt(const Vector3& eyePosition3D, const Vector3& center3D, const Vector3& upVector3D)
      { __camera->lookAt(eyePosition3D, center3D, upVector3D); }

      const ProjectionCameraPtr& camera() const { return __camera; }

      inline void transformModel(const Matrix4& transform)
      {  __camera->transformModel(transform); }

      inline void pushModelTransformation()
      {  __camera->pushModelTransformation(); }

      inline void popModelTransformation()
      {  __camera->popModelTransformation();  }

      inline void transformModelIdentity()
      {  __camera->transformModelIdentity();  }

      inline void translateModel(const Vector3& v)
      {  __camera->translateModel(v);  }

      inline void scaleModel(const Vector3& v)
      {  __camera->scaleModel(v);  }


      // BoundingBoxPtr getBoundingBoxView() const
      // {  return __camera->getBoundingBoxView();  }

      virtual void process(ScenePtr scene);

      void process(TriangleSetPtr triangles, AppearancePtr appearance, uint32_t id) { iprocess(triangles, appearance,id,__camera); }
      void process(PolylinePtr polyline, MaterialPtr material, uint32_t id)  { iprocess(polyline, material, id, __camera); }
      void process(PointSetPtr pointset, MaterialPtr material, uint32_t id)  { iprocess(pointset, material, id, __camera); }

      virtual void iprocess(TriangleSetPtr triangles, AppearancePtr appearance, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0) = 0;
      virtual void iprocess(PolylinePtr polyline, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0) = 0;
      virtual void iprocess(PointSetPtr pointset, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera = ProjectionCameraPtr(), uint32_t threadid = 0) = 0;

      virtual void beginProcess() {}
      virtual void endProcess() {}

      IdMap getPrimitiveIdMap() const { return __idMap; } 
      
      pgl_hash_map<uint32_t,std::pair<real_t,std::vector<real_t> > > aggregate(pgl_hash_map<uint32_t,real_t>) const;

protected:

    uint32_t getNewPrimitiveId(uint32_t shapeId) 
    { 
        if (__idPolicy == ePrimitiveIdBased) {
            uint32_t id = __primitiveId++; 
            __idMap[id] = shapeId; 
            return id;
        }
        else { return shapeId; }    
    }

    ProjectionCameraPtr __camera;

    bool __idPolicy;
    uint32_t __primitiveId;
    IdMap __idMap;

};

class ALGO_API ImageProjectionEngine  : public ProjectionEngine {

public :

      ImageProjectionEngine(uint16_t imageWidth, 
                       uint16_t imageHeight,
                       eIdPolicy idPolicy = eShapeIdBased);

      virtual ~ImageProjectionEngine();

      virtual uint16_t getImageWidth() const { return __imageWidth; }
      virtual uint32_t getImageHeight() const { return __imageHeight; }


protected:
    uint16_t __imageWidth;
    uint16_t __imageHeight;

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif