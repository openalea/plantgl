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
class Texture2D;
class Texture2DTransformation;

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
class ScreenProjected;
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

  /** Applies \e self to an object of type Texture2D.
      \warning
      - \e texture must be non null and valid. */
  virtual bool process( Texture2D * texture ) = 0;

  /** Applies \e self to an object of type ImageTexture.
      \warning
      - \e texture must be non null and valid. */
  virtual bool process( ImageTexture * texture ) = 0;

  /** Applies \e self to an object of type Texture2DTransformation.
      \warning
      - \e texturetransformation must be non null and valid. */
  virtual bool process( Texture2DTransformation * texturetransformation ) = 0;

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

  /** Applies \e self to an object of type Scaled.
      \warning
      - \e scaled must be non null and valid. */
  virtual bool process( ScreenProjected * screenprojected ) = 0;

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

