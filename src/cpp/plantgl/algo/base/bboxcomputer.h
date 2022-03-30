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



/*! \file actn_bboxcomputer.h
    \brief Definition of the action class BBoxComputer.
*/



#ifndef __actn_bboxcomputer_h__
#define __actn_bboxcomputer_h__

#include "../algo_config.h"
#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/util_cache.h>
#include <plantgl/scenegraph/core/action.h>
#ifndef GEOM_FWDEF
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>
#endif
/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Discretizer;
#ifdef GEOM_FWDEF
class Scene;
typedef RCPtr<Scene> ScenePtr;
class BoundingBox;
typedef RCPtr<BoundingBox> BoundingBoxPtr;
#endif

/* ----------------------------------------------------------------------- */


/**
   \class BBoxComputer
   \brief An action which computes the bounding box of \e Geometry objects.
*/


class ALGO_API BBoxComputer : public Action
{

public:

  /// Constructs a BBoxComputer.
  BBoxComputer( Discretizer& discretizer );

  /// Destructor
  virtual ~BBoxComputer( ) ;

  /// Clears \e self.
  void clear( );

  /** Returns the resulting bounding box when applying \e self for the
      last time. */
  BoundingBoxPtr getBoundingBox( );

  /// Returns the Discretizer attached to \e self.
  Discretizer& getDiscretizer( ) ;


  /** Applies \e self to an object of type of Shape.
      \warning
      - \e Shape must be non null and valid. */
  virtual bool process(Shape * Shape);

 /** Applies \e self to an object of type of Inline.
      \warning
      - \e inline must be non null and valid. */
  virtual bool process(Inline * geomInline);

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

  virtual bool process( Swung * swung );

  virtual bool process( Scaled * scaled );

  virtual bool process( ScreenProjected * screenprojected );

  virtual bool process( Sphere * sphere );

  virtual bool process( Tapered * tapered );

  virtual bool process( Translated * translated );

  virtual bool process( TriangleSet * triangleSet );

  //@}
  /// @name Appearance
  //@{

  virtual bool process( Material * material );

  virtual bool process( MonoSpectral * monoSpectral );

  virtual bool process( MultiSpectral * multiSpectral ) ;

  virtual bool process( ImageTexture * textureimg );

  virtual bool process( Texture2D * texture );

  virtual bool process( Texture2DTransformation * texturetransformation );

  //@}
  /// @name Geom2D
  //@{


  virtual bool process( BezierCurve2D * bezierCurve );

  virtual bool process( Disc * disc );

  virtual bool process( NurbsCurve2D * nurbsCurve );

  virtual bool process( PointSet2D * pointSet );

  virtual bool process( Polyline2D * polyline );


  //@}

  virtual bool process( Text * text );

  virtual bool process( Font * font );

  /// Compute bounding box of a scene.
  virtual bool process(const ScenePtr& scene);
  virtual bool process(const Scene& scene);

protected:

  /// The cache storing the already computed bounding boxes.
  Cache<BoundingBoxPtr> __cache;

  /// The resulting bounding box.
  BoundingBoxPtr __bbox;

  /** A Discretizer is used to compute the bounding box of objects we de not
      know how to compute the surface. It comes to compute the bounding box
      of the discretized representation. */
  Discretizer& __discretizer;

};


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ------------------------------------------------------------------------- */

// __actn_bboxcomputer_h__
#endif
