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

/*! \file ProjectionCamera.h
    \brief Definition of Rendering Engine based on ZBuffer.
*/



#ifndef __ProjectionCamera_h__
#define __ProjectionCamera_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/math/util_matrix.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <stack>


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class ProjectionCamera
    \brief A Rendering Engine based on ZBuffer.
*/

/* ----------------------------------------------------------------------- */

class ProjectionCamera;
typedef RCPtr<ProjectionCamera> ProjectionCameraPtr;


class ProjectionCamera : public TOOLS(RefCountObject){
public:
    enum eProjectionType {
        eOrthographic,
        ePerspective
    };

    ProjectionCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far, eProjectionType projection = eOrthographic);

   virtual ~ProjectionCamera();

   TOOLS(Vector3) worldToRaster(const TOOLS(Vector3)& vertexWorld, const uint16_t imageWidth, const uint16_t imageHeight) const;

   TOOLS(Vector3) worldToCamera(const TOOLS(Vector3)& vertexWorld) const;
   TOOLS(Vector3) cameraToNDC(const TOOLS(Vector3)& vertexCamera) const;
   TOOLS(Vector3) ndcToRaster(const TOOLS(Vector3)& vertexNDC, const uint16_t imageWidth, const uint16_t imageHeight) const;
   TOOLS(Vector3) cameraToRaster(const TOOLS(Vector3)& vertexCamera, const uint16_t imageWidth, const uint16_t imageHeight) const;

   TOOLS(Vector3) rasterToWorld(const TOOLS(Vector3)& raster, const uint16_t imageWidth, const uint16_t imageHeight) const;
   TOOLS(Vector3) cameraToWorld(const TOOLS(Vector3)& vertexCamera) const;

   TOOLS(Vector3) ndcToCamera(const TOOLS(Vector3)& vertexCamera) const ;

   BoundingBoxPtr getBoundingBoxView() const;

   TOOLS(Matrix4) getWorldToCameraMatrix() const { return __worldToCamera; }
   TOOLS(Matrix4) getCameraToWorldMatrix() const { return __cameraToWorld; }

   void transformModel(const TOOLS(Matrix4)& transform);
   void pushModelTransformation();
   void popModelTransformation();
   void transformModelIdentity();

   void translateModel(const TOOLS(Vector3)& v);
   void scaleModel(const TOOLS(Vector3)& v);

   TOOLS(Matrix4) getModelTransformationMatrix() const { return __currentModelMatrix; }

   bool isInZRange(real_t z) const ;
   bool isInZRange(real_t zmin, real_t zmax) const;

   void lookAt(const TOOLS(Vector3)& eyePosition3D, const TOOLS(Vector3)& center3D, const TOOLS(Vector3)& upVector3D);


   real_t left;
   real_t right;
   real_t bottom;
   real_t top;
   real_t near;
   real_t far;
   eProjectionType type;

   static ProjectionCameraPtr perspectiveCamera(real_t angleOfView, real_t aspectRatio, real_t near, real_t far);
   static ProjectionCameraPtr frustumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);
   static ProjectionCameraPtr orthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);

   const TOOLS(Vector3)& position() const { return __position; }


protected:
   TOOLS(Vector3) screen2NDC(const real_t& xScreen, const real_t& yScreen, const real_t z) const ;
   TOOLS(Vector3) NDC2screen(const real_t& xNDC, const real_t& yNDC, const real_t z) const;

   real_t __xscale;
   real_t __xconstant;
   real_t __yscale;
   real_t __yconstant;

   TOOLS(Vector3) __position;

   TOOLS(Matrix4) __cameraToWorld;
   TOOLS(Matrix4) __worldToCamera;
   std::stack<TOOLS(Matrix4)> __modelMatrixStack;
   TOOLS(Matrix4) __currentModelMatrix;
   TOOLS(Matrix4) __currentWorldToCamera;


};


inline TOOLS(Vector3) toRasterSpace(const TOOLS(Vector3)& vertexNDC, const uint16_t imageWidth, const uint16_t imageHeight)
{
    // convert to raster space
    TOOLS(Vector3) vertexRaster( 
                          ((vertexNDC.x() + 1) / 2.) * imageWidth, 
                          ((1 - vertexNDC.y()) / 2.) * imageHeight,
                          vertexNDC.z());
    return vertexRaster;
}

inline TOOLS(Vector3) rasterToNDC(const TOOLS(Vector3)& raster, const uint16_t imageWidth, const uint16_t imageHeight)
{
    // convert raster to NDC space
    TOOLS(Vector3) vertexNDC(  (raster.x()*2/imageWidth) - 1,
                        1 - (raster.y()*2/imageWidth),
                        raster.z());
    return vertexNDC;
}

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
