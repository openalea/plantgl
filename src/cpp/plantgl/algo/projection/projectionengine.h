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

      ProjectionEngine();

      ~ProjectionEngine();

      inline void setPerspectiveCamera(real_t angleOfView, real_t aspectRatio, real_t near, real_t far)
      { __camera = ProjectionCamera::perspectiveCamera(angleOfView, aspectRatio, near, far); }

      inline void setFrustumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
      { __camera = ProjectionCamera::frustumCamera(left, right, bottom, top, near, far); }

      inline void setOrthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
      {  __camera = ProjectionCamera::orthographicCamera(left, right, bottom, top, near, far); }

      inline void lookAt(const TOOLS(Vector3)& eyePosition3D, const TOOLS(Vector3)& center3D, const TOOLS(Vector3)& upVector3D)
      { __camera->lookAt(eyePosition3D, center3D, upVector3D); }

      ProjectionCameraPtr camera() const { return __camera; }

      inline void transformModel(const TOOLS(Matrix4)& transform)
      {  __camera->transformModel(transform); }

      inline void pushModelTransformation()
      {  __camera->pushModelTransformation(); }

      inline void popModelTransformation()
      {  __camera->popModelTransformation();  }

      inline void transformModelIdentity()
      {  __camera->transformModelIdentity();  }

      inline void translateModel(const TOOLS(Vector3)& v)
      {  __camera->translateModel(v);  }

      inline void scaleModel(const TOOLS(Vector3)& v)
      {  __camera->scaleModel(v);  }


      BoundingBoxPtr getBoundingBoxView() const
      {  return __camera->getBoundingBoxView();  }

      void process(ScenePtr scene);

      virtual void process(TriangleSetPtr triangles, AppearancePtr appearance, uint32_t id) = 0;
      virtual void process(PolylinePtr polyline, MaterialPtr material, uint32_t id) = 0;
      virtual void process(PointSetPtr pointset, MaterialPtr material, uint32_t id) = 0;


protected:
    ProjectionCameraPtr __camera;
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif