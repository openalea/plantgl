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


class ProjectionCamera : public RefCountObject{
public:
    enum eProjectionType {
        eOrthographic,
        ePerspective
    };

    ProjectionCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far, eProjectionType projection = eOrthographic);

   virtual ~ProjectionCamera();

   Vector3 worldToRaster(const Vector3& vertexWorld, const uint16_t imageWidth, const uint16_t imageHeight) const;

   Vector3 worldToCamera(const Vector3& vertexWorld) const;
   Vector3 cameraToNDC(const Vector3& vertexCamera) const;
   Vector3 ndcToRaster(const Vector3& vertexNDC, const uint16_t imageWidth, const uint16_t imageHeight) const;
   Vector3 cameraToRaster(const Vector3& vertexCamera, const uint16_t imageWidth, const uint16_t imageHeight) const;

   Vector3 rasterToWorld(const Vector3& raster, const uint16_t imageWidth, const uint16_t imageHeight) const;
   Vector3 cameraToWorld(const Vector3& vertexCamera) const;

   Vector3 ndcToCamera(const Vector3& vertexCamera) const ;

   BoundingBoxPtr getBoundingBoxView() const;

   Matrix4 getWorldToCameraMatrix() const { return __worldToCamera; }
   Matrix4 getCameraToWorldMatrix() const { return __cameraToWorld; }

   void transformModel(const Matrix4& transform);
   void pushModelTransformation();
   void popModelTransformation();
   void transformModelIdentity();

   void translateModel(const Vector3& v);
   void scaleModel(const Vector3& v);

   Matrix4 getModelTransformationMatrix() const { return __currentModelMatrix; }

   bool isInZRange(real_t z) const ;
   bool isInZRange(real_t zmin, real_t zmax) const;

   void lookAt(const Vector3& eyePosition3D, const Vector3& center3D, const Vector3& upVector3D);


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

   const Vector3& position() const { return __position; }


protected:
   Vector3 screen2NDC(const real_t& xScreen, const real_t& yScreen, const real_t z) const ;
   Vector3 NDC2screen(const real_t& xNDC, const real_t& yNDC, const real_t z) const;

   real_t __xscale;
   real_t __xconstant;
   real_t __yscale;
   real_t __yconstant;

   Vector3 __position;

   Matrix4 __cameraToWorld;
   Matrix4 __worldToCamera;
   std::stack<Matrix4> __modelMatrixStack;
   Matrix4 __currentModelMatrix;
   Matrix4 __currentWorldToCamera;


};


inline Vector3 toRasterSpace(const Vector3& vertexNDC, const uint16_t imageWidth, const uint16_t imageHeight)
{
    // convert to raster space
    Vector3 vertexRaster( 
                          ((vertexNDC.x() + 1) / 2.) * imageWidth, 
                          ((1 - vertexNDC.y()) / 2.) * imageHeight,
                          vertexNDC.z());
    return vertexRaster;
}

inline Vector3 rasterToNDC(const Vector3& raster, const uint16_t imageWidth, const uint16_t imageHeight)
{
    // convert raster to NDC space
    Vector3 vertexNDC(  (raster.x()*2/imageWidth) - 1,
                        1 - (raster.y()*2/imageWidth),
                        raster.z());
    return vertexNDC;
}

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
