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
#define PGL_QT_FWD_DECL
#include <plantgl/gui/pglqopenglwidget.h>
#undef PGL_QT_FWD_DECL

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

#define BUFSIZE 40000000

class ALGO_API GLRenderer : public Action
{

public:


  /** Constructs a GLRenderer with the Discretizer \e discretizer. */
  GLRenderer( Discretizer& discretizer,
#ifndef PGL_WITHOUT_QT
    QOpenGLBaseWidget * glframe = NULL,
#endif
    PGLOpenGLFunctionsPtr ogl = NULL
    );


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
    ShapeId = 0x0001 ,
    SceneObjectId = 0x0002,
    PrimitiveId = 0x0004,
    ShapeNPrimitiveIds = ShapeId | PrimitiveId,
    SceneObjectNPrimitive = SceneObjectId | PrimitiveId
  };

  /// Destructor
  virtual ~GLRenderer( );

  /// Clears \e self.
  void clear( );

  void init();

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

#ifndef PGL_WITHOUT_QT
  /// Set the gl frame in which the scene must be rendered
  void setGLFrame(QOpenGLBaseWidget * frame) { __glframe = frame; }

  /// Get the gl frame in which the scene must be rendered
  QOpenGLBaseWidget * getGLFrame() const { return __glframe; }

  /// Set the gl frame in which the scene must be rendered
  bool setGLFrameFromId(WId);

  void setOpenGLFunctions(PGLOpenGLFunctionsPtr ogl) { __ogl = ogl; }
#endif

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

  bool check(size_t id, GLuint& displaylist);
  bool call(size_t id);
  void update(size_t id, GLuint displaylist);
  const AppearancePtr& getAppearanceCache() const { return __appearance; }

  void registerTexture(ImageTexture * texture, GLuint id, bool erasePreviousIfExists = true);
  GLuint getTextureId(ImageTexture * texture);

  void useVertexArray(bool value) { __withvertexarray = value; }
  bool isVertexArrayUsed() const { return __withvertexarray; }

protected:

  /// A cache used to store display list.
  Cache<GLuint> __cache;

  /// A cache used to store texture.
  Cache<GLuint> __cachetexture;

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

#ifndef PGL_WITHOUT_QT
  QOpenGLBaseWidget * __glframe;
#endif

  PGLOpenGLFunctionsPtr __ogl;
  bool __ogltoinit;

private:
  template<class T>
  bool discretize_and_render(T * geom);

  bool __currentdisplaylist;

  bool __dopushpop;
  GLenum __executionmode;

  int __precompildepth;
  int __maxprecompildepth;

  bool __withvertexarray;

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// __actn_glsimplerenderer_h__
#endif

