/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
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



#ifndef __actn_glrenderer_h__
#define __actn_glrenderer_h__



#include "util_gl.h"

#include <plantgl/scenegraph/core/action.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/util_cache.h>
#include <plantgl/scenegraph/appearance/appearance.h>

/* ----------------------------------------------------------------------- */

class QGLWidget;

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Discretizer;

/* ----------------------------------------------------------------------- */

/**
   \class GLRenderer
   \brief An action which draws objects of type of Geometry or of type
   of Material to the current GL context.
*/


class ALGO_API GLRenderer : public Action
{

public:

  /** Constructs a GLRenderer with the Discretizer \e discretizer. */
  GLRenderer( Discretizer& discretizer, QGLWidget * glframe = NULL );


  /*! \enum RenderingMode
    This enum specifies the Rendering mode.
	*/
  /*!
  \var RenderingMode Normal 
	Normal rendering
  */
  /*!
  \var RenderingMode Selection
	Selection rendering.
  */
  /*!
  \var RenderingMode Dynamic
    Rendering of dynamic object. Not using display list.
  */
  enum RenderingMode {
    Normal = 0x0001,
    Selection = 0x0002,
    DynamicScene = 0x0004,
	DynamicPrimitive = 0x0008,
    Dynamic = DynamicPrimitive | DynamicScene,
  };


  enum SelectionId {
	ShapeId,
	SceneObjectId
  };

  /// Destructor
  virtual ~GLRenderer( );

  /// Clears \e self.
  void clear( );

  /// Returns the Discretizer attached to \e self.
  Discretizer& getDiscretizer( );

  /// Set the rendering mode
  void setRenderingMode(RenderingMode);

  /// Get the rendering mode
  const RenderingMode& getRenderingMode() const;

  void setSelectionMode(SelectionId);

  const SelectionId getSelectionMode() const;

  /// Create (true) or Call (false) a display list for all the scene.
  bool beginSceneList();

  /// finish the Creation of the display list for all the scene.
  void endSceneList();

  /// clear the display list for all the scene.
  void clearSceneList();

  /// Set the gl frame in which the scene must be rendered
  void setGLFrame(QGLWidget * frame) { __glframe = frame; }

  /// Get the gl frame in which the scene must be rendered
  QGLWidget * getGLFrame() const { return __glframe; }

  /// Set the gl frame in which the scene must be rendered
  bool setGLFrameFromId(uint_t);

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

  virtual bool processAppereance(Shape *  Shape );

  virtual bool processGeometry(Shape *  Shape );

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

  bool check(uint_t id, GLuint& displaylist);
  bool call(uint_t id);
  void update(uint_t id, GLuint displaylist);
  const AppearancePtr& getAppearanceCache() const { return __appearance; }

  void registerTexture(ImageTexture * texture, GLuint id, bool erasePreviousIfExists = true);
  GLuint getTextureId(ImageTexture * texture);

protected:

  /// A cache used to store display list.
  TOOLS(Cache)<GLuint> __cache;

  /// A cache used to store texture.
  TOOLS(Cache)<GLuint> __cachetexture;

  /// A cache used to store display list of all scene.
  GLuint __scenecache;

  /// The discretizer used to store the discretize parametric while
  Discretizer& __discretizer;

  /// The current material
  AppearancePtr __appearance;

  /// Rendering Mode.
  RenderingMode __Mode;

  SelectionId __selectMode;

  int __compil;

  QGLWidget * __glframe;

private:
  template<class T> 
  bool discretize_and_render(T * geom);

  bool __currentdisplaylist;
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// __actn_glsimplerenderer_h__
#endif

