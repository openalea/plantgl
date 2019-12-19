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



/*! \file actn_x3dprinter.h
    \brief Definition of the action class X3DPrinter.
*/

#ifndef __actn_x3dprinter_h__
#define __actn_x3dprinter_h__


#include "vrmlprinter.h"


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
  \class X3DPrinter
  \brief An action which prints scene objects to the stream \e vrmlStream
   using X3D file format. (to be implemented)
*/



class CODEC_API X3DPrinter : public VrmlPrinter
{

public :

  /// Constructor.
  X3DPrinter(std::ostream& stream,
              Discretizer& discretizer);

  /// header of the X3D File.
  virtual bool header(const char * comment = NULL);

  /// Destructor.
  virtual ~X3DPrinter();


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
  virtual bool process( Shape * Shape );

  /** Applies \e self to an object of type of Inline.
    \warning
      - \e geominline must be non null and valid. */
  virtual bool process( Inline * geominline );

  //@}

  /// @name Material
  //@{

  /** Applies \e self to an object of type of Material.
    \warning
      - \e material must be non null and valid. */
  virtual bool process( Material * material );

  /** Applies \e self to an object of type of MonoSpectral.
    \warning
      - \e monoSpectral must be non null and valid. */
  virtual bool process( MonoSpectral * monoSpectral );

  /** Applies \e self to an object of type of MultiSpectral.
    \warning
      - \e multiSpectral must be non null and valid. */
  virtual bool process( MultiSpectral * multiSpectral );

  /** Applies \e self to an object of type Texture2D.
      \warning
      - \e texture must be non null and valid. */
  virtual bool process( Texture2D * texture );

  /** Applies \e self to an object of type ImageTexture.
      \warning
      - \e texture must be non null and valid. */
  virtual bool process( ImageTexture * texture );

  /** Applies \e self to an object of type Texture2DTransformation.
      \warning
      - \e texturetransformation must be non null and valid. */
  virtual bool process( Texture2DTransformation * texturetransformation );

  //@}

  /// @name Geom3D
  //@{

  /** Applies \e self to an object of type of AmapSymbol.
    \warning
      - \e amapSymbol must be non null and valid. */
  virtual bool process( AmapSymbol * amapSymbol );

  /** Applies \e self to an object of type of AsymmetricHull.
    \warning
      - \e amapSymbol must be non null and valid. */
  virtual bool process( AsymmetricHull * amapSymbol );

  /** Applies \e self to an object of type of AxisRotated.
    \warning
      - \e axisRotated must be non null and valid. */
  virtual bool process( AxisRotated * axisRotated );

  /** Applies \e self to an object of type of BezierCurve.
    \warning
      - \e bezierCurve must be non null and valid. */
  virtual bool process( BezierCurve * bezierCurve );

  /** Applies \e self to an object of type of BezierPatch.
    \warning
      - \e bezierPatch must be non null and valid. */
  virtual bool process( BezierPatch * bezierPatch );

  /** Applies \e self to an object of type of Box.
    \warning
      - \e box must be non null and valid. */
  virtual bool process( Box * box );

  /** Applies \e self to an object of type of Cone.
    \warning
      - \e cone must be non null and valid. */
  virtual bool process( Cone * cone );

  /** Applies \e self to an object of type of Cylinder.
    \warning
      - \e cylinder must be non null and valid. */
  virtual bool process( Cylinder * cylinder );

  /** Applies \e self to an object of type of ElevationGrid.
    \warning
      - \e elevationGrid must be non null and valid. */
  virtual bool process( ElevationGrid * elevationGrid );

  /** Applies \e self to an object of type of EulerRotated.
    \warning
      - \e eulerRotated must be non null and valid. */
  virtual bool process( EulerRotated * eulerRotated );

  /** Applies \e self to an object of type of ExtrudedHull.
    \warning
      - \e amapSymbol must be non null and valid. */
  virtual bool process( ExtrudedHull * amapSymbol );

  /** Applies \e self to an object of type of FaceSet.
    \warning
      - \e faceSet must be non null and valid. */
  virtual bool process( FaceSet * faceSet );

  /** Applies \e self to an object of type of Frustum.
    \warning
      - \e frustum must be non null and valid. */
  virtual bool process( Frustum * frustum );

  /** Applies \e self to an object of type of Extrusion.
    \warning
      - \e extrusion must be non null and valid. */
  virtual bool process( Extrusion * extrusion );

  /** Applies \e self to an object of type of Group.
    \warning
      - \e group must be non null and valid. */
  virtual bool process( Group * group );

  /** Applies \e self to an object of type of NurbsCurve.
    \warning
      - \e nurbsCurve must be non null and valid. */
  virtual bool process( NurbsCurve * nurbsCurve );

  /** Applies \e self to an object of type of NurbsPatch.
    \warning
      - \e nurbsPatch must be non null and valid. */
  virtual bool process( NurbsPatch * nurbsPatch );

  /** Applies \e self to an object of type of Oriented.
    \warning
      - \e oriented must be non null and valid. */
  virtual bool process( Oriented * oriented );

  /** Applies \e self to an object of type of Paraboloid.
    \warning
      - \e paraboloid must be non null and valid. */
  virtual bool process( Paraboloid * paraboloid );

  /** Applies \e self to an object of type of PointSet.
    \warning
      - \e pointSet must be non null and valid. */
  virtual bool process( PointSet * pointSet );

  /** Applies \e self to an object of type of Polyline.
    \warning
      - \e polyline must be non null and valid. */
  virtual bool process( Polyline * polyline );

  /** Applies \e self to an object of type of QuadSet.
    \warning
      - \e quadSet must be non null and valid. */
  virtual bool process( QuadSet * quadSet );

  /** Applies \e self to an object of type of Revolution.
    \warning
      - \e revolution must be non null and valid. */
  virtual bool process( Revolution * revolution );

  /** Applies \e self to an object of type of Scaled.
    \warning
      - \e scaled must be non null and valid. */
  virtual bool process( Scaled * scaled );

  /** Applies \e self to an object of type of Sphere.
    \warning
      - \e sphere must be non null and valid. */
  virtual bool process( Sphere * sphere );

  virtual bool process( ScreenProjected * scp)
  { return false; }

  /** Applies \e self to an object of type of Tapered.
    \warning
      - \e tapered must be non null and valid. */
  virtual bool process( Tapered * tapered );

  /** Applies \e self to an object of type of Translated.
    \warning
      - \e translated must be non null and valid. */
  virtual bool process( Translated * translated );

  /** Applies \e self to an object of type of TriangleSet.
    \warning
      - \e triangleSet must be non null and valid. */
  virtual bool process( TriangleSet * triangleSet );

  //@}

  /// @name Geom2D
  //@{

  /** Applies \e self to an object of type of BezierCurve2D.
    \warning
      - \e bezierCurve must be non null and valid. */
  virtual bool process( BezierCurve2D * bezierCurve );

  /** Applies \e self to an object of type of Disc.
    \warning
      - \e disc must be non null and valid. */
  virtual bool process( Disc * disc );

  /** Applies \e self to an object of type of NurbsCurve2D.
    \warning
      - \e nurbsCurve must be non null and valid. */
  virtual bool process( NurbsCurve2D * nurbsCurve );

  /** Applies \e self to an object of type of PointSet2D.
    \warning
      - \e pointSet must be non null and valid. */
  virtual bool process( PointSet2D * pointSet );

  /** Applies \e self to an object of type of Polyline2D.
    \warning
      - \e polyline must be non null and valid. */
  virtual bool process( Polyline2D * polyline );

  //@}

  virtual bool process( Text * text );

  virtual bool process( Font * font );


};


/* ----------------------------------------------------------------------- */

// __actn_x3dprinter_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

