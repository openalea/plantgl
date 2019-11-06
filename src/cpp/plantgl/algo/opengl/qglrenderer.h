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


/*! \file glrenderer3.h
    \brief Definition of the action class GLRenderer3.
*/



#ifndef __actn_glrenderer3_h__
#define __actn_glrenderer3_h__

#define GL_SILENCE_DEPRECATION

#include <QtGui>


#include "util_gl.h"

#include <plantgl/scenegraph/core/action.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/util_cache.h>
#include <plantgl/scenegraph/appearance/appearance.h>
#include <plantgl/scenegraph/geometry/explicitmodel.h>
#include <plantgl/algo/base/matrixcomputer.h>
#include <plantgl/scenegraph/appearance/color.h>

/* ----------------------------------------------------------------------- */

class QOpenGLFunctions;
class QOpenGLShaderProgram;

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Discretizer;
class Tesselator;

/* ----------------------------------------------------------------------- */

/**
   \class QGLRenderer3
   \brief An action which draws objects of type of Geometry or of type
   of Material to the current GL context.
*/
#ifndef PGL_WITHOUT_QT

class ALGO_API QGLRenderer : public Action
{

public:

  /** Constructs a GLRenderer with the Discretizer \e discretizer. */
  QGLRenderer( Discretizer& discretizer, 
                Tesselator& tesselator);

   void setViewProjection(const QMatrix4x4& view, const QMatrix4x4& projection);


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

  /*
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
  */

  enum eRenderingMode {
    eFilled = 0x0001,
    eWire = 0x0002,
    eFilledNWired = 0x0003
  };

  /// Destructor
  virtual ~QGLRenderer( );

  /// Clears \e self.
  void clear( );

  void init();

  void setLight(const Vector3& lightPosition, 
                const Color3& lightAmbient, 
                const Color3& lightDiffuse, 
                const Color3& lightSpecular ) {
    __lightPosition = lightPosition;
    __lightAmbient  = lightAmbient; 
    __lightDiffuse  = lightDiffuse; 
    __lightSpecular = lightSpecular; 
  }

  void enableLight(bool enabled) {
    __lightEnabled = enabled;
  }

  bool isLightEnabled() const {
    return __lightEnabled;
  }

  void setRendering(eRenderingMode mode) {
    __mode = mode;
  }

  /// Returns the Discretizer attached to \e self.
  Discretizer& getDiscretizer( );

  /*
  /// Set the rendering mode
  void setRenderingMode(RenderingMode);

  /// Get the rendering mode
  const RenderingMode& getRenderingMode() const;

  void setSelectionMode(SelectionId);

  const SelectionId getSelectionMode() const;
  */

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

  const AppearancePtr& getAppearanceCache() const { return __appearance; }

  void registerTexture(ImageTexture * texture, GLuint id, bool erasePreviousIfExists = true);
  GLuint getTextureId(ImageTexture * texture);

protected:

    struct BufferInfo {
        QOpenGLVertexArrayObject * vao;
        QOpenGLBuffer *vertexBuf;
        QOpenGLBuffer *normalBuf;
        QOpenGLBuffer *colorBuf;
        QOpenGLBuffer *indexBuf;
    };

  typedef Cache<ExplicitModelPtr> GeometryCache;
  GeometryCache __geometrycache;

  typedef Cache<BufferInfo> BufferCache;
  BufferCache __buffercache;


  /// The discretizer used to store the discretize parametric while
  Discretizer& __discretizer;
  Tesselator& __tesselator;

  /// The current material
  AppearancePtr __appearance;

  QOpenGLFunctions __ogl;

  QOpenGLShaderProgram * __materialprogram;
  QOpenGLShaderProgram * __colorprogram;
  QOpenGLShaderProgram * __textureprogram;

  QOpenGLShaderProgram * __currentprogram;

  QMatrix4x4  __projectionmatrix;
  QMatrix4x4  __viewmatrix;
  MatrixStack __modelmatrix; 
  MatrixStack __texturematrix; 

  Vector3  __lightPosition;
  Color3   __lightAmbient;
  Color3   __lightDiffuse;
  Color3   __lightSpecular;
  bool     __lightEnabled;

  eRenderingMode __mode;
  
private:
  template<class T> 
  bool discretize_and_render(T * geom);
};

#endif

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// __actn_glsimplerenderer_h__
#endif

