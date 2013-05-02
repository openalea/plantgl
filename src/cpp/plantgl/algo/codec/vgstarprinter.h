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



/*! \file actn_vgstarprinter.h
    \brief Definition of the action class VgstarPrinter.
*/


#ifndef __actn_vgstarprinter_h__
#define __actn_vgstarprinter_h__

#include "codec_config.h"
#include <plantgl/algo/base/matrixcomputer.h>
#include <plantgl/algo/base/tesselator.h>
#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class VgstarPrinter
   \brief An action which prints scene objects to the stream \e vgstarStream
   using VgstarRay file format.

*/

class Geometry;

/* ----------------------------------------------------------------------- */


class CODEC_API VgstarPrinter : public MatrixComputer
{
  /*! \enum VGSTAR_TYPE
  VGSTAR constants for the different types of primitives.
  */
  /*!
  \var VGSTAR_TYPE POLYGONE
        Polygon type
  */
  /*!
  \var VGSTAR_TYPE SPHERE
    Sphere type
  */
  /*!
  \var VGSTAR_TYPE CYLINDER
    Cylinder type
  */
  /*!
  \var VGSTAR_TYPE ELLIPSIS
    Ellipsis type
  */

  enum VGSTAR_TYPE {
    POLYGONE=1,
    SPHERE,
    CYLINDER,
    ELLIPSIS
  };

public:

  /** Constructs a VgstarPrinter with the output streams \e vgstarStream and
      the Tesselator \e tesselator. */
  VgstarPrinter( std::ostream& vgstarStream,
              Tesselator& tesselator);

  /// Destructor
  virtual ~VgstarPrinter( ) {
  }

  virtual bool header(const char * comment = NULL);

  void printTransformation();

  void printNullTransformation();

  void printColor();

  void printNullTriangle();

  /// @name Shape
  //@{
  virtual bool process(Shape * Shape);

  //@}

  /// @name Material
  //@{

  virtual bool process( Material * material );

  virtual bool process( MonoSpectral * monoSpectral );

  virtual bool process( MultiSpectral * multiSpectral );

  virtual bool process( Texture2D * texture );

  virtual bool process( ImageTexture * texture );

  virtual bool process( Texture2DTransformation * texturetransfo );

  //@}


  /// @name Geom3D
  //@{

  virtual bool process( AmapSymbol * amapSymbol );

  virtual bool process( AsymmetricHull * asymmetricHull );

  virtual bool process( BezierCurve * bezierCurve );

  virtual bool process( BezierPatch * bezierPatch );

  virtual bool process( Box * box );

  virtual bool process( Cone * cone );

  virtual bool process( Cylinder * cylinder );

  virtual bool process( ElevationGrid * elevationGrid );

  virtual bool process( ExtrudedHull * extrudedHull );

  virtual bool process( FaceSet * faceSet );

  virtual bool process( Frustum * frustum );

  virtual bool process( Extrusion * extrusion );

  virtual bool process( Group * group );

  virtual bool process( NurbsCurve * nurbsCurve );

  virtual bool process( NurbsPatch * nurbsPatch );

  virtual bool process( Paraboloid * paraboloid );

  virtual bool process( PointSet * pointSet );

  virtual bool process( Polyline * polyline );

  virtual bool process( QuadSet * quadSet );

  virtual bool process( Revolution * revolution );

  virtual bool process( Swung * swung );

  virtual bool process( Sphere * sphere );

  virtual bool process( Tapered * tapered );

  virtual bool process( TriangleSet * triangleSet );

  virtual bool process( ScreenProjected * scp)
  { return false; }

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

  std::ostream& __vgstarStream;

  /// The tesselator used to triangulate objects when needed
  Tesselator& __tesselator;

  /// The name of the current texture is cached using a string.
  Color3 __color;

  bool triangle_process( Geometry* object );
};


/* ------------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __actn_vgstarprinter_h__
#endif

