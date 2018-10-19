

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

/*! \file depthsortengine.h
    \brief Definition of Rendering Engine based on ZBuffer.
*/



#ifndef __DepthSortEngine_h__
#define __DepthSortEngine_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/math/util_vector.h>
#include <plantgl/math/util_matrix.h>
#include <plantgl/tool/util_array2.h>
#include <plantgl/tool/util_cache.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/scene/scene.h>
#include "projectioncamera.h"
#include <list>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class DepthSortEngine
    \brief A Rendering Engine based on Painter's algorithm (depth sort).
*/

/* ----------------------------------------------------------------------- */

class ALGO_API DepthSortEngine  {

public :

      DepthSortEngine();

      ~DepthSortEngine();

      inline void setPerspectiveCamera(real_t angleOfView, real_t aspectRatio, real_t near, real_t far)
      { __camera = ProjectionCamera::perspectiveCamera(angleOfView, aspectRatio, near, far); }

      inline void setFrustumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
      { __camera = ProjectionCamera::frustumCamera(left, right, bottom, top, near, far); }

      inline void setOrthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far)
      {  __camera = ProjectionCamera::orthographicCamera(left, right, bottom, top, near, far); }

      inline void lookAt(const TOOLS(Vector3)& eyePosition3D, const TOOLS(Vector3)& center3D, const TOOLS(Vector3)& upVector3D)
      { __camera->lookAt(eyePosition3D, center3D, upVector3D); }

      ProjectionCameraPtr camera() const { return __camera; }

      void processTriangle(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, uint32_t id);

      ScenePtr getResult(Color4::eColor4Format format = Color4::eARGB) const;
      ScenePtr getProjectionResult(Color4::eColor4Format format = Color4::eARGB) const;

    struct PolygonInfo {
        Point3ArrayPtr points;
        TOOLS(Vector3) pmin, pmax;
        uint32_t id;
    };
    typedef std::list<PolygonInfo> PolygonInfoList;

protected:


    PolygonInfo _toPolygonInfo(const Point3ArrayPtr& points, uint32_t id) const;
    DepthSortEngine::PolygonInfoList _toPolygonInfo(const std::vector<Point3ArrayPtr>& polygons, uint32_t id) const;

    PolygonInfoList::iterator _processTriangle( PolygonInfo polygon, PolygonInfoList::iterator begin);

    PolygonInfoList __polygonlist; 
    ProjectionCameraPtr __camera;     
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif