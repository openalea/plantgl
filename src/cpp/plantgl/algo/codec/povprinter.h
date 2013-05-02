/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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



/*! \file actn_povprinter.h
    \brief Definition of the action class PovPrinter.
*/


#ifndef __actn_povprinter_h__
#define __actn_povprinter_h__


#include <plantgl/tool/rcobject.h>
#include "printer.h"
#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/appearance/appearance.h>

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE
class Vector3;
TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Tesselator;
class Color3;
class BoundingBox;
typedef RCPtr<BoundingBox> BoundingBoxPtr;

/* ----------------------------------------------------------------------- */

/**
   \class PovPrinter
   \brief An action which prints scene objects to the stream \e povStream
   using PovRay file format.

*/

/* ----------------------------------------------------------------------- */

class CODEC_API PovPrinter : public Printer
{

public:

  /** Constructs a PovPrinter with the output streams \e povStream and
      the Tesselator \e tesselator and the bbox of the scene to print. */
  PovPrinter( std::ostream& povStream,
              Tesselator& tesselator,
              const BoundingBox& bbox );

  /** Constructs a PovPrinter with the output streams \e povStream and
      the Tesselator \e tesselator. */
  PovPrinter( std::ostream& povStream,
              Tesselator& tesselator);

  /// Destructor
  virtual ~PovPrinter( );

  void setLineWidth(real_t l) { __linewidth = l; }
  real_t getLineWidth() { return __linewidth; }

  virtual bool header(const char * comment = NULL);

  /// Set CENTER variable.
  bool setCenter(const BoundingBox& bbox );

  /// Set a camera to the scene. All vector must be given in the GEOM basis and will be transformed in PovRay basis.
  /** Dont work */
  virtual bool setCamera(const TOOLS(Vector3)& location,
                                                 const TOOLS(Vector3)& direction,
                                                 const TOOLS(Vector3)& up,
                                                 const TOOLS(Vector3)& right);

  /// Set a camera to the scene. All vector must be given in the GEOM basis and will be transformed in PovRay basis.
  virtual bool setCamera(const TOOLS(Vector3)& eye, 
						 const TOOLS(Vector3)& distance, 
					     const double&  ang, 
						 const double& az, 
						 const double& el);

  void beginHeader(); 
  void endHeader(); 

  /// Set a light to the scene.
  virtual bool setLight(const TOOLS(Vector3)& position, const Color3& color = Color3::WHITE);

  /// Set the background.
  virtual bool setBackGround(const Color3& color);

  /// @name Shape
  //@{
  virtual bool process(Shape * Shape);

  virtual bool process(Inline * geomInline);

  //@}

  /// @name Material
  //@{

  virtual bool process( Material * material );

  virtual bool process( MonoSpectral * monoSpectral );

  virtual bool process( MultiSpectral * multiSpectral );

  /** Applies \e self to an object of type of Texture2D.
    \warning
      - \e texture must be non null and valid. */
  virtual bool process( Texture2D * texture );

  /** Applies \e self to an object of type of ImageTexture.
    \warning
      - \e texture must be non null and valid. */
  virtual bool process( ImageTexture * texture );

  /** Applies \e self to an object of type of Texture2DTransformation.
    \warning
      - \e texture must be non null and valid. */
  virtual bool process( Texture2DTransformation * texture );

  //@}


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

protected:

  /// The tesselator used to triangulate objects when needed
  Tesselator& __tesselator;

  /// The name of the current texture is cached using a string.
  std::string __texture;
  AppearancePtr __appearance;
  typedef pgl_hash_map<size_t,std::string> TextureNameMap;
  TextureNameMap __texturenamemap;

  real_t __linewidth;

  bool __linecache;
  bool __pointcache;

};


/* ------------------------------------------------------------------------- */

// __actn_povprinter_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif


