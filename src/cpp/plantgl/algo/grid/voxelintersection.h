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



/*! \file actn_voxelintersection.h
    \brief Algorithm of Voxel Intersection with GEOM object. see VoxelIntersection.
*/


#ifndef __actn_voxelintersection_h__
#define __actn_voxelintersection_h__

/* ----------------------------------------------------------------------- */
#include <plantgl/scenegraph/scene/scene.h>
#include "../base/bboxcomputer.h"
#include <plantgl/math/util_matrix.h>
#include "voxel.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class VoxelIntersection
    \brief Compute intersection beetwen a Voxel and a GEOM object.
*/

/* ----------------------------------------------------------------------- */

class ALGO_API VoxelIntersection : public Action {


    public :

    /// Default constructor.
    VoxelIntersection(BBoxComputer& bboxcomputer);

    /// Destructor
    virtual ~VoxelIntersection();

    /// set the Voxel to test to \e voxel.
    void setVoxel(Voxel * voxel);

    /// set the Voxel coordinates to test.
    // void setVoxel(const Vector3& lowerleftCorner, const Vector3& upperRightCorner);

    /// Returns the BBoxComputer attached to \e self.
    BBoxComputer& getBBoxComputer( ) {
        return __bboxComputer;
    }

   /// Return whether the voxel is filled by last GEOM object examined.
    bool isFilled() const{
        return __filled;
    }

    /** Applies \e self to an object of type of Shape.
        \warning
        - \e geomShape must be non null and valid. */
    virtual bool process(Shape * geomshape);

    /** Applies \e self to an object of type of Inline.
        \warning
       - \e inline must be non null and valid. */
    virtual bool process(Inline * geominline);

    /// @name Geom3D
    //@{


   virtual bool process( AmapSymbol * amapSymbol );

   virtual bool process( AsymmetricHull * asymmetricHull );

   virtual bool process( AxisRotated * axisRotated );

   virtual bool process( BezierCurve * bezierCurve );

   virtual bool process( BezierPatch * bezierPatch );

   virtual bool process( Box * box );

   virtual bool process( Cone * cone );

   virtual bool process( Cylinder * cylinder );

   virtual bool process( ElevationGrid * elevationGrid );

   virtual bool process( EulerRotated * eulerRotated );

   virtual bool process( ExtrudedHull * extrudedHull );

   virtual bool process( FaceSet * faceSet );

   virtual bool process( Frustum * frustum );

   virtual bool process( Extrusion * extrusion );

   virtual bool process( Group * group );

   virtual bool process( IFS * ifs );

   virtual bool process( NurbsCurve * nurbsCurve );

   virtual bool process( NurbsPatch * nurbsPatch );

   virtual bool process( Oriented * oriented );

   virtual bool process( Paraboloid * paraboloid );

   virtual bool process( PointSet * pointSet );

   virtual bool process( Polyline * polyline );

   virtual bool process( QuadSet * quadSet );

   virtual bool process( Revolution * revolution );

   virtual bool process( Scaled * scaled );

   virtual bool process( ScreenProjected * scp )
   { return false; }

   virtual bool process( Sphere * sphere );

   virtual bool process( Swung * swung );

   virtual bool process( Tapered * tapered );

   virtual bool process( Translated * translated );

   virtual bool process( TriangleSet * triangleSet );

   //@}
   /// @name Appearance
   //@{

   virtual bool process( Material * material ) {
       GEOM_ASSERT(material);
     // nothing to do.
     __filled = false;
     return false;
   }

   virtual bool process( ImageTexture * texture ) {
       GEOM_ASSERT(texture);
     // nothing to do.
     __filled = false;
     return false;
   }

   virtual bool process( Texture2D * texture ) {
       GEOM_ASSERT(texture);
     // nothing to do.
     __filled = false;
     return false;
   }

   virtual bool process( Texture2DTransformation * texture ) {
       GEOM_ASSERT(texture);
     // nothing to do.
     __filled = false;
     return false;
   }

   virtual bool process( MonoSpectral * monoSpectral ) {
       GEOM_ASSERT(monoSpectral);
     // nothing to do.
     __filled = false;
     return false;
   }

   virtual bool process( MultiSpectral * multiSpectral ) {
     GEOM_ASSERT(multiSpectral);
     // nothing to do.
     __filled = false;
     return false;
   }

   //@}
   /// @name Geom2D
   //@{


   virtual bool process( BezierCurve2D * bezierCurve );

   virtual bool process( Disc * disc );

   virtual bool process( NurbsCurve2D * nurbsCurve );

   virtual bool process( PointSet2D * pointSet );

   virtual bool process( Polyline2D * polyline );


   //@}

   virtual bool process( Text * text ){
       GEOM_ASSERT(text);
     // nothing to do.
     __filled = false;
     return false;
   }

   virtual bool process( Font * font ){
       GEOM_ASSERT(font);
     // nothing to do.
     __filled = false;
     return false;
   }

   // virtual bool process(ScenePtr& scene);

 protected:

   /// The result : is __voxel is filled by last GEOM object examined.
   bool __filled;

   /// The octree in which we insert the GEOM objetcs.
   Voxel * __voxel;
   // Vector3 __ll;
   // Vector3 __ur;

   /** A BBoxComputer is used to compute the bounding box of objects and compute a first
       value of intersection. */
   BBoxComputer& __bboxComputer;


   /// Stack of succesive transformation.
   std::vector<Matrix4> __transformstack;

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
/* ----------------------------------------------------------------------- */
