/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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
/*! \file actn_action.h
    \brief Definition of the base class Action.
*/




#ifndef __actn_action_h__
#define __actn_action_h__


/* ----------------------------------------------------------------------- */

#include "../sg_config.h"

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/*
  Forward definitions
*/

// Shape

class Shape;
class Inline;

// Material
class Material;
class MonoSpectral;
class MultiSpectral;
class ImageTexture;

// 3D Geom
class AmapSymbol;
class AsymmetricHull;
class AxisRotated;
class BezierCurve;
class BezierPatch;
class Box;
class Cone;
class Cylinder;
class ElevationGrid;
class EulerRotated;
class ExtrudedHull;
class FaceSet;
class Frustum;
class Extrusion;
class Group;
class IFS;
class NurbsCurve;
class NurbsPatch;
class Oriented;
class Paraboloid;
class PointSet;
class Polyline;
class QuadSet;
class Revolution;
class Swung;
class Scaled;
class Sphere;
class Tapered;
class Translated;
class TriangleSet;

// 2D Geom
class BezierCurve2D;
class Disc;
class NurbsCurve2D;
class PointSet2D;
class Polyline2D;


// Text
class Font;
class Text;

/* ----------------------------------------------------------------------- */


/**
   \class Action
   \brief A action which can be applied on \e SceneObject objects.
*/

class SG_API Action
{

public:

  /// Constructs an Action.
  Action( );

  /// Destructor.
  virtual ~Action( );

  /// @name Pre and Post Processing
  //@{

  /// Begining of the Action.
  virtual bool beginProcess();

  /// End of the Action.
  virtual bool endProcess();

  //@}

  /// @name Shape
  //@{

  /** Applies \e self to an object of type of Shape.
      \warning
      - \e Shape must be non null and valid. */
  virtual bool process(Shape * Shape);

  /** Applies \e self to an object of type of Inline .
      \warning
      - \e Inline must be non null and valid. */
  virtual bool process(Inline * Inline);

  //@}

  /// @name Material
  //@{

  /** Applies \e self to an object of type Material.
      \warning
      - \e material must be non null and valid. */
  virtual bool process( Material * material ) = 0;

  /** Applies \e self to an object of type MonoSpectral.
      \warning
      - \e monoSpectral must be non null and valid. */
  virtual bool process( MonoSpectral * monoSpectral ) = 0;

  /** Applies \e self to an object of type MultiSpectral.
      \warning
      - \e multiSpectral must be non null and valid. */
  virtual bool process( MultiSpectral * multiSpectral ) = 0;

  /** Applies \e self to an object of type ImageTexture.
      \warning
      - \e texture must be non null and valid. */
  virtual bool process( ImageTexture * texture ) = 0;

  //@}


  /// @name Geom3D
  //@{

  /** Applies \e self to an object of type AmapSymbol.
      \warning
      - \e amapSymbol must be non null and valid. */
  virtual bool process( AmapSymbol * amapSymbol ) = 0;

  /** Applies \e self to an object of type AsymmetricHull.
      \warning
      - \e amapSymbol must be non null and valid. */
  virtual bool process( AsymmetricHull * asymmetricHull ) = 0;

  /** Applies \e self to an object of type AxisRotated.
      \warning
      - \e axisRotated must be non null and valid. */
  virtual bool process( AxisRotated * axisRotated ) = 0;

  /** Applies \e self to an object of type BezierCurve
      \warning
      - \e bezierCurve must be non null and valid. */
  virtual bool process( BezierCurve * bezierCurve ) = 0;

  /** Applies \e self to an object of type BezierPatch
      \warning
      - \e bezierPatch must be non null and valid. */
  virtual bool process( BezierPatch * bezierPatch ) = 0;

  /** Applies \e self to an object of type Box.
      \warning
      - \e box must be non null and valid. */
  virtual bool process( Box * box ) = 0;

  /** Applies \e self to an object of type Cone.
      \warning
      - \e cone must be non null and valid. */
  virtual bool process( Cone * cone ) = 0;

  /** Applies \e self to an object of type Cylinder.
      \warning
      - \e cylinder must be non null and valid. */
  virtual bool process( Cylinder * cylinder ) = 0;

  /** Applies \e self to an object of type ElevationGrid.
      \warning
      - \e elevationGrid must be non null and valid. */
  virtual bool process( ElevationGrid * elevationGrid ) = 0;

  /** Applies \e self to an object of type EulerRotated.
      \warning
      - \e eulerRotated must be non null and valid. */
  virtual bool process( EulerRotated * eulerRotated ) = 0;

  /** Applies \e self to an object of type ExtrudedHull.
      \warning
      - \e amapSymbol must be non null and valid. */
  virtual bool process( ExtrudedHull * extrudedHull ) = 0;

  /** Applies \e self to an object of type FaceSet.
      \warning
      - \e faceSet must be non null and valid. */
  virtual bool process( FaceSet * faceSet ) = 0;

  /** Applies \e self to an object of type Frustum.
      \warning
      - \e frustum must be non null and valid. */
  virtual bool process( Frustum * frustum ) = 0;

  /** Applies \e self to an object of type Extrusion.
      \warning
      - \e extrusion must be non null and valid. */
  virtual bool process( Extrusion * extrusion ) = 0;

  /** Applies \e self to an object of type Group.
      \warning
      - \e group must be non null and valid. */
  virtual bool process( Group * group ) = 0;

  /** Applies \e self to an object of type IFS.
      \warning
      - \e ifs must be non null and valid. */
  virtual bool process( IFS * ifs ) = 0;

  /** Applies \e self to an object of type NurbsCurve.
      \warning
      - \e nurbsCurve must be non null and valid. */
  virtual bool process( NurbsCurve * nurbsCurve ) = 0;

  /** Applies \e self to an object of type NurbsPatch.
      \warning
      - \e nurbsPatch must be non null and valid. */
  virtual bool process( NurbsPatch * nurbsPatch ) = 0;

  /** Applies \e self to an object of type Oriented.
      \warning
      - \e oriented must be non null and valid. */
  virtual bool process( Oriented * oriented ) = 0;

 /** Applies \e self to an object of type Paraboloid.
      \warning
      - \e paraboloid must be non null and valid. */
  virtual bool process( Paraboloid * paraboloid ) = 0;

  /** Applies \e self to an object of type PointSet.
      \warning
      - \e pointSet must be non null and valid. */
  virtual bool process( PointSet * pointSet ) = 0;

  /** Applies \e self to an object of type Polyline.
      \warning
      - \e polyline must be non null and valid. */
  virtual bool process( Polyline * polyline ) = 0;

  /** Applies \e self to an object of type QuadSet.
      \warning
      - \e quadSet must be non null and valid. */
  virtual bool process( QuadSet * quadSet ) = 0;

  /** Applies \e self to an object of type Revolution.
      \warning
      - \e revolution must be non null and valid. */
  virtual bool process( Revolution * revolution ) = 0;

  /** Applies \e self to an object of type Scaled.
      \warning
      - \e scaled must be non null and valid. */
  virtual bool process( Scaled * scaled ) = 0;

  /** Applies \e self to an object of type Sphere.
      \warning
      - \e sphere must be non null and valid. */
  virtual bool process( Sphere * sphere ) = 0;

  /** Applies \e self to an object of type Swung.
      \warning
      - \e swung must be non null and valid. */
  virtual bool process( Swung * swung ) = 0;

  /** Applies \e self to an object of type Tapered.
      \warning
      - \e tapered must be non null and valid. */
  virtual bool process( Tapered * tapered ) = 0;

  /** Applies \e self to an object of type Translated.
      \warning
      - \e translated must be non null and valid. */
  virtual bool process( Translated * translated ) = 0;

  /** Applies \e self to an object of type TriangleSet.
      \warning
      - \e triangleSet must be non null and valid. */
  virtual bool process( TriangleSet * triangleSet ) = 0;

  //@}

  /// @name Geom2D
  //@{

  /** Applies \e self to an object of type BezierCurve2D
      \warning
      - \e bezierCurve must be non null and valid. */
  virtual bool process( BezierCurve2D * bezierCurve ) = 0;

 /** Applies \e self to an object of type Disc.
      \warning
      - \e disc must be non null and valid. */
  virtual bool process( Disc * disc ) = 0;

  /** Applies \e self to an object of type NurbsCurve2D.
      \warning
      - \e nurbsCurve must be non null and valid. */
  virtual bool process( NurbsCurve2D * nurbsCurve ) = 0;

  /** Applies \e self to an object of type PointSet2D.
      \warning
      - \e pointSet must be non null and valid. */
  virtual bool process( PointSet2D * pointSet ) = 0;

  /** Applies \e self to an object of type Polyline2D.
      \warning
      - \e polyline must be non null and valid. */
  virtual bool process( Polyline2D * polyline ) = 0;
  //@}

  /** Applies \e self to an object of type Text.
      \warning
      - \e text must be non null and valid. */
  virtual bool process( Text * text ) = 0;

  /** Applies \e self to an object of type Font.
      \warning
      - \e font must be non null and valid. */
  virtual bool process( Font * font ) = 0;



};


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __action_h__
#endif

