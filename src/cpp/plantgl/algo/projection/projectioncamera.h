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

inline Vector3 NDCtoRasterSpace(const Vector3& vertexNDC, const uint16_t imageWidth, const uint16_t imageHeight)
{
    // convert to raster space
    Vector3 vertexRaster( 
                          ((vertexNDC.x() + 1) / 2.) * imageWidth, 
                          ((1 - vertexNDC.y()) / 2.) * imageHeight,
                          vertexNDC.z());
    return vertexRaster;
}

inline Vector3 rasterSpaceToNDC(const Vector3& raster, const uint16_t imageWidth, const uint16_t imageHeight)
{
    // convert raster to NDC space
    Vector3 vertexNDC(  (raster.x()*2/imageWidth) - 1,
                        1 - (raster.y()*2/imageHeight),
                        raster.z());
    return vertexNDC;
}

/* ----------------------------------------------------------------------- */

/** 
    \class ProjectionCamera
    \brief A Rendering Engine based on ZBuffer.
*/

/* ----------------------------------------------------------------------- */

class ProjectionCamera;
typedef RCPtr<ProjectionCamera> ProjectionCameraPtr;


class ALGO_API ProjectionCamera : public RefCountObject{
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
   inline Vector3 NDCToRaster(const Vector3& vertexNDC, const uint16_t imageWidth, const uint16_t imageHeight) const
    { return NDCtoRasterSpace(vertexNDC, imageWidth, imageHeight); }

   Vector3 cameraToRaster(const Vector3& vertexCamera, const uint16_t imageWidth, const uint16_t imageHeight) const;

   Vector3 rasterToWorld(const Vector3& raster, const uint16_t imageWidth, const uint16_t imageHeight) const;

   inline Vector3 rasterToNDC(const Vector3& raster, const uint16_t imageWidth, const uint16_t imageHeight ) const 
   { return rasterSpaceToNDC(raster, imageWidth, imageHeight);}

   Vector3 NDCToCamera(const Vector3& vertexCamera) const ;
   Vector3 cameraToWorld(const Vector3& vertexCamera) const;

   BoundingBoxPtr getBoundingBoxView() const;

   Matrix4 getWorldToCameraMatrix() const { return __worldToCamera; }
   //  Matrix4 getCameraToWorldMatrix() const { return __cameraToWorld; }

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


    ProjectionCameraPtr copy();

protected:
   Vector3 screen2NDC(const real_t& xScreen, const real_t& yScreen, const real_t z) const ;
   Vector3 NDC2screen(const real_t& xNDC, const real_t& yNDC, const real_t z) const;

   real_t __xscale;
   real_t __xconstant;
   real_t __yscale;
   real_t __yconstant;

   Vector3 __position;

   // Matrix4 __cameraToWorld;
   Matrix4 __worldToCamera;
   std::stack<Matrix4> __modelMatrixStack;
   Matrix4 __currentModelMatrix;
   Matrix4 __currentWorldToCamera;


};


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
