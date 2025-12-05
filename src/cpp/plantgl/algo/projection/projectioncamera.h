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
#include <plantgl/algo/raycasting/ray.h>
#include <stack>


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */
// Normalized Device Coordinates (NDC) space conversion functions
/*
Espace NDC
- Domaine : [-1,1] × [-1,1]
- Origine : centre de l’image
- Axe X : droite positive
- Axe Y : haut positif (contrairement à un raster classique)
- Profondeur Z : libre (z_w après projection perspective)
*/
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
        ePerspective,
        eHemispheric,
        eEquirectangular,
        eCylindrical
    };

   enum eProjectionMethodType {
        eProjection,
        eRayIntersection,
    };


   ProjectionCamera(real_t near, real_t far, eProjectionType type, eProjectionMethodType methodtype);

   virtual ~ProjectionCamera();

   Vector3 worldToRaster(const Vector3& vertexWorld, const uint16_t imageWidth, const uint16_t imageHeight) const;
   Vector3 worldToCamera(const Vector3& vertexWorld) const;
   
   virtual Vector3 cameraToNDC(const Vector3& vertexCamera) const = 0;
   virtual Vector3 NDCToCamera(const Vector3& vertexCamera) const = 0;

   inline Vector3 NDCToRaster(const Vector3& vertexNDC, const uint16_t imageWidth, const uint16_t imageHeight) const
    { return NDCtoRasterSpace(vertexNDC, imageWidth, imageHeight); }

   Vector3 cameraToRaster(const Vector3& vertexCamera, const uint16_t imageWidth, const uint16_t imageHeight) const;
   Vector3 rasterToCamera(const Vector3& vertexRaster, const uint16_t imageWidth, const uint16_t imageHeight) const;

   Vector3 rasterToWorld(const Vector3& raster, const uint16_t imageWidth, const uint16_t imageHeight) const;

   virtual Ray rasterToCameraRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const;
   virtual Ray rasterToWorldRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const;

   inline Vector3 rasterToNDC(const Vector3& raster, const uint16_t imageWidth, const uint16_t imageHeight ) const 
   { return rasterSpaceToNDC(raster, imageWidth, imageHeight);}

   Vector3 cameraToWorld(const Vector3& vertexCamera) const;

   // BoundingBoxPtr getBoundingBoxView() const;

   Matrix4 getWorldToCameraMatrix() const { return __worldToCamera; }

   void transformModel(const Matrix4& transform);
   void pushModelTransformation();
   void popModelTransformation();
   void transformModelIdentity();

   void translateModel(const Vector3& v);
   void scaleModel(const Vector3& v);

   Matrix4 getModelTransformationMatrix() const { return __currentModelMatrix; }

   bool isInZRange(real_t z) const ;
   bool isInZRange(real_t zmin, real_t zmax) const;
   virtual bool isValidPixel(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const {
     return (x < imageWidth) && (y < imageHeight);
   }

   void lookAt(const Vector3& eyePosition3D, const Vector3& center3D, const Vector3& upVector3D);

   static ProjectionCameraPtr perspectiveCamera(real_t verticalAngleOfView, real_t aspectRatio, real_t near, real_t far);
   static ProjectionCameraPtr frustumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);
   static ProjectionCameraPtr orthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);
   static ProjectionCameraPtr hemisphericCamera(real_t near, real_t far);
   static ProjectionCameraPtr sphericalCamera(real_t viewAngle, real_t near, real_t far);
   static ProjectionCameraPtr equirectangularCamera(real_t viewAngle, real_t near, real_t far);
   static ProjectionCameraPtr cylindricalCamera(real_t viewAngle, real_t bottom, real_t top, real_t near, real_t far);

   eProjectionType type() const { return __type; }
   eProjectionMethodType methodType() const { return __methodtype; }

   virtual ProjectionCameraPtr copy() = 0;

   inline const Vector3& position() const { return __position; }
   inline const Vector3& direction() const { return __direction; }
   inline const Vector3& up() const  { return __up; }

   inline real_t near() const { return __near; }
   inline real_t get_near() const { return __near; }
   inline real_t far() const { return __far; }

   virtual real_t projectedArea(uint16_t x, uint16_t y, real_t z, const uint16_t imageWidth, const uint16_t imageHeight);
   virtual real_t solidAngle(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight);

   // virtual bool fitViewTo(const BoundingBoxPtr bbx, uint16_t margin) = 0;

protected:
   
   eProjectionType __type;
   eProjectionMethodType __methodtype;
   Vector3 __position;
   Vector3 __direction;
   Vector3 __up;

   real_t __near;
   real_t __far;


   Matrix4 __worldToCamera;
   std::stack<Matrix4> __modelMatrixStack;
   Matrix4 __currentModelMatrix;
   Matrix4 __currentWorldToCamera;
};

/* ----------------------------------------------------------------------- */

class ALGO_API FrustumCamera : public ProjectionCamera {
public:

   FrustumCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far, eProjectionType type);

   virtual ~FrustumCamera();

   BoundingBoxPtr getBoundingBoxView() const;

   void setFrustum(real_t left, real_t right, real_t bottom, real_t top);

   real_t left;
   real_t right;
   real_t bottom;
   real_t top;

protected:
   Vector3 screen2NDC(const real_t& xScreen, const real_t& yScreen, const real_t z) const ;
   Vector3 NDC2screen(const real_t& xNDC, const real_t& yNDC, const real_t z) const;

   real_t __xscale;
   real_t __xconstant;
   real_t __yscale;
   real_t __yconstant;

};

typedef RCPtr<FrustumCamera> FrustumCameraPtr;

/* ----------------------------------------------------------------------- */

class ALGO_API PerspectiveCamera : public FrustumCamera {
public:

   PerspectiveCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);
   PerspectiveCamera(real_t verticalAngleOfView, real_t aspectRatio, real_t near, real_t far);

   virtual ~PerspectiveCamera();

   virtual Vector3 cameraToNDC(const Vector3& vertexCamera) const;
   virtual Vector3 NDCToCamera(const Vector3& vertexCamera) const;

   virtual ProjectionCameraPtr copy();

   virtual Ray rasterToCameraRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const;
   virtual Ray rasterToWorldRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const;

   // virtual bool fitViewTo(const BoundingBoxPtr bbx, uint16_t margin) ;

};

typedef RCPtr<PerspectiveCamera> PerspectiveCameraPtr;

/* ----------------------------------------------------------------------- */

class ALGO_API OrthographicCamera : public FrustumCamera {
public:

   OrthographicCamera(real_t left, real_t right, real_t bottom, real_t top, real_t near, real_t far);

   virtual ~OrthographicCamera();

   virtual Vector3 cameraToNDC(const Vector3& vertexCamera) const;
   virtual Vector3 NDCToCamera(const Vector3& vertexCamera) const;

   virtual ProjectionCameraPtr copy();

   virtual real_t solidAngle(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) { return 0; }

};

typedef RCPtr<OrthographicCamera> OrthographicCameraPtr;

/* ----------------------------------------------------------------------- */

class ALGO_API SphericalCamera : public ProjectionCamera {
public:

   SphericalCamera(real_t viewAngle = 180, real_t near = 0, real_t far = REAL_MAX);

   virtual ~SphericalCamera();

   virtual Vector3 cameraToNDC(const Vector3& vertexCamera) const;
   virtual Vector3 NDCToCamera(const Vector3& vertexCamera) const;

   virtual ProjectionCameraPtr copy();

   real_t getViewAngle() const ;
   void setViewAngle(real_t angle);

   virtual bool isValidPixel(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const;
   
   virtual Ray rasterToCameraRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const;
   virtual Ray rasterToWorldRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const;

   virtual Vector3::Spherical NDCToSpherical(const Vector3& vertexNDC) const;
   virtual Vector3 SphericalToNDC(const Vector3::Spherical& sph) const;


protected:
   real_t __viewAngle;

};

typedef RCPtr<SphericalCamera> SphericalCameraPtr;


/* ----------------------------------------------------------------------- */

class ALGO_API EquirectangularCamera : public SphericalCamera {
public:

   EquirectangularCamera(real_t viewAngle = 180, real_t near = 0, real_t far = REAL_MAX);

   virtual ~EquirectangularCamera();

   virtual ProjectionCameraPtr copy();

   virtual bool isValidPixel(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const {
     return (x < imageWidth) && (y < imageHeight);
   } 

   virtual Vector3::Spherical NDCToSpherical(const Vector3& vertexNDC) const;
   virtual Vector3 SphericalToNDC(const Vector3::Spherical& sph) const;


protected:
   real_t __viewAngle;

};

typedef RCPtr<EquirectangularCamera> EquirectangularCameraPtr;


/* ----------------------------------------------------------------------- */

class ALGO_API CylindricalCamera : public ProjectionCamera {
public:

   CylindricalCamera(real_t viewAngle = 180, real_t bottom = -1, real_t top = 1, real_t near = 0, real_t far = REAL_MAX);

   virtual ~CylindricalCamera();

   virtual Vector3 cameraToNDC(const Vector3& vertexCamera) const;
   virtual Vector3 NDCToCamera(const Vector3& vertexCamera) const;

   virtual ProjectionCameraPtr copy();

   real_t getViewAngle() const ;
   void setViewAngle(real_t angle);

   virtual Ray rasterToCameraRay(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) const;

   virtual real_t solidAngle(uint16_t x, uint16_t y, const uint16_t imageWidth, const uint16_t imageHeight) { return 0; }

   real_t bottom;
   real_t top;

protected:
   real_t __viewAngle;
   real_t __yscale;
   real_t __yconstant;

};

typedef RCPtr<CylindricalCamera> CylindricalCameraPtr;


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
