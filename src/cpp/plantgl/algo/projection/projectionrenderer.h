/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2018 UMR CIRAD/INRIA/INRA AGAP
 *
 *       File author(s): F. Boudon et al.
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


/*! \file actn_glrenderer.h
    \brief Definition of the action class GLRenderer.
*/



#ifndef __actn_projectionrenderer_h__
#define __actn_projectionrenderer_h__



#include "../algo_config.h"
#include "projectionengine.h"

#include <plantgl/scenegraph/core/action.h>
#include <plantgl/algo/base/tesselator.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/util_cache.h>


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */

/**
   \class GLRenderer
   \brief An action which draws objects of type of Geometry or of type
   of Material to the current GL context.
*/

class ALGO_API ProjectionRenderer : public Action
{

public:

  /** Constructs a GLRenderer with the Discretizer \e discretizer. */
  ProjectionRenderer( ProjectionEngine&, Tesselator& tesselator, Discretizer& discretizer);


  /// Destructor
  virtual ~ProjectionRenderer( );

  /// Returns the Discretizer attached to \e self.
  Discretizer& getDiscretizer( );

  /// @name Pre and Post Processing
  //@{
  virtual bool beginProcess();

  virtual bool endProcess();

  //@}

  /** Applies \e self to an object of type of Inline .
      \warning
      - \e geomInline must be non null and valid. */
  virtual bool process(Inline * geomInline);

  /// @name Material
  //@{

  virtual bool process( Material * material );

  virtual bool process( MonoSpectral * monoSpectral );

  virtual bool process( MultiSpectral * multiSpectral );

  virtual bool process( Texture2D * texture );

  virtual bool process( ImageTexture * texture );

  virtual bool process( Texture2DTransformation * texturetransformation );

  //@}
  virtual bool process(Shape *  Shape );

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

  virtual bool process( ScreenProjected * scp );

  virtual bool process( Sphere * sphere );

  virtual bool process( Tapered * tapered );

  virtual bool process( Translated * translated );

  virtual bool process( TriangleSet * triangleSet );


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

  const AppearancePtr& getAppearanceCache() const { return __appearance; }

protected:

  /// The discretizer used to store the discretize parametric while
  Tesselator& __tesselator;

  Discretizer& __discretizer;

  // The engine used to render
  ProjectionEngine& __engine;

  /// The current material
  AppearancePtr __appearance;

  /// The current id;
  uint32_t __id;
  
private:
  template<class T> 
  bool discretize_and_process(T * geom);

  template<class T> 
  bool tesselate_and_process(T * geom);

  template<class T> 
  bool transform_and_process(T * geom);

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// __actn_glsimplerenderer_h__
#endif

