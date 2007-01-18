/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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

/*! \file actn_statisticcomputer.h
    \brief Definition of the action class StatisticComputer.
*/


#ifndef __actn_statisticcomputer_h__
#define __actn_statisticcomputer_h__


#include "GEOM/actn_action.h"

#ifndef _WIN32
#include <features.h>
#endif

#include <vector>
#include "Tools/util_types.h"
#include "Tools/util_hashmap.h"

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class StatisticComputer
   \brief An action which compute statistics on a scene.
*/



class GEOM_API StatisticComputer : public Action
{
public:

  /** Constructs a  StatisticComputer*/
  StatisticComputer();

  /// Destructor
  virtual ~StatisticComputer( ) ;

  /// Get the number of element of a scene.
  virtual const uint32_t getSize() const;

  /// Get the number of named element of a scene.
  virtual const uint32_t getNamed() const;

  /// Get the memory size of the scene.
  virtual const uint32_t getMemorySize() const ;


  /// Get the all elements of the scene.
  virtual const std::vector<uint32_t>& getElements() const;



  /// @name GeomShape
  //@{
  virtual bool process(GeomShape * geomShape);

  /// Get the number of shape.
  const uint32_t getShape() const ;
  //@}

  /// @name Material
  //@{

  virtual bool process( Material * material );

  /// Get the number of Material.
  const uint32_t getMaterial() const;

  virtual bool process( MonoSpectral * monoSpectral );

  /// Get the number of MonoSpectral.
  const uint32_t getMonoSpectral() const;

  virtual bool process( MultiSpectral * multiSpectral );

  /// Get the number of MultiSpectral.
  const uint32_t getMultiSpectral() const;

  virtual bool process( ImageTexture * texture );

  /// Get the number of ImageTexture.
  const uint32_t getImageTexture() const;
  //@}

  /// @name Geom3D
  //@{
  virtual bool process( AmapSymbol * amapSymbol );

  /// Get the number of AmapSymbol.
  const uint32_t getAmapSymbol() const;

  virtual bool process( AsymmetricHull * asymmetricHull );

  /// Get the number of AsymmetricHull.
  const uint32_t getAsymmetricHull() const;

  virtual bool process( AxisRotated * axisRotated );

  /// Get the number of AxisRotated.
  const uint32_t getAxisRotated() const ;

  virtual bool process( BezierCurve * bezierCurve );

  /// Get the number of BezierCurve.
  const uint32_t getBezierCurve() const ;

  virtual bool process( BezierPatch * bezierPatch );

  /// Get the number of BezierPatch.
  const uint32_t getBezierPatch() const ;

  virtual bool process( Box * box );

  /// Get the number of Box.
  const uint32_t getBox() const ;

  virtual bool process( Cone * cone );

  /// Get the number of Cone.
  const uint32_t getCone() const ;

  virtual bool process( Cylinder * cylinder );

  /// Get the number of Cylinder.
  const uint32_t getCylinder() const ;

  virtual bool process( ElevationGrid * elevationGrid );

  /// Get the number of ElevationGrid.
  const uint32_t getElevationGrid() const ;

  virtual bool process( EulerRotated * eulerRotated );

  /// Get the number of EulerRotated.
  const uint32_t getEulerRotated() const ;

  virtual bool process( ExtrudedHull * extrudedHull );

  /// Get the number of ExtrudedHull.
  const uint32_t getExtrudedHull() const;

  virtual bool process( FaceSet * faceSet );

  /// Get the number of FaceSet.
  const uint32_t getFaceSet() const ;

  virtual bool process( Frustum * frustum );

  /// Get the number of Frustum.
  const uint32_t getFrustum() const;

  virtual bool process( Extrusion * extrusion );

  /// Get the number of Extrusion.
  const uint32_t getExtrusion() const ;

  virtual bool process( Group * group );

  /// Get the number of Group.
  const uint32_t getGroup() const;

  virtual bool process( NurbsCurve * nurbsCurve );

  /// Get the number of NurbsCurve.
  const uint32_t getNurbsCurve() const ;

  virtual bool process( NurbsPatch * nurbsPatch );

  /// Get the number of NurbsPatch.
  const uint32_t getNurbsPatch() const ;

  virtual bool process( Oriented * oriented );

  /// Get the number of Oriented.
  const uint32_t getOriented() const;

  virtual bool process( Paraboloid * paraboloid );

  /// Get the number of Paraboloid.
  const uint32_t getParaboloid() const;

  virtual bool process( PointSet * pointSet );

  /// Get the number of PointSet.
  const uint32_t getPointSet() const;

  virtual bool process( GeomPolyline * polyline );

  /// Get the number of GeomPolyline.
  const uint32_t getGeomPolyline() const;

  virtual bool process( QuadSet * quadSet );

  /// Get the number of QuadSet.
  const uint32_t getQuadSet() const ;

  virtual bool process( Revolution * revolution );

  /// Get the number of Revolution.
  const uint32_t getRevolution() const ;

  virtual bool process( Scaled * scaled );

  /// Get the number of Scaled.
  const uint32_t getScaled() const ;

  virtual bool process( Sphere * sphere );

  /// Get the number of Sphere.
  const uint32_t getSphere() const ;

  virtual bool process( Tapered * tapered );

  /// Get the number of Tapered.
  const uint32_t getTapered() const ;

  virtual bool process( Translated * translated );

  /// Get the number of Translated.
  const uint32_t getTranslated() const;

  virtual bool process( TriangleSet * triangleSet );

  /// Get the number of TriangleSet.
  const uint32_t getTriangleSet() const;

  //@}

  /// @name Geom2D
  //@{
  virtual bool process( BezierCurve2D * bezierCurve );

  /// Get the number of BezierCurve2D.
  const uint32_t getBezierCurve2D() const ;

  virtual bool process( Disc * disc );

  /// Get the number of Disc.
  const uint32_t getDisc() const ;

  virtual bool process( NurbsCurve2D * nurbsCurve );

  /// Get the number of NurbsCurve2D.
  const uint32_t getNurbsCurve2D() const;

  virtual bool process( PointSet2D * pointSet );

  /// Get the number of PointSet2D.
  const uint32_t getPointSet2D() const ;

  virtual bool process( GeomPolyline2D * polyline );

  /// Get the number of GeomPolyline2D.
  const uint32_t getGeomPolyline2D() const;

  virtual bool process( Swung * revolution );

  /// Get the number of Swung.
  const uint32_t getSwung() const;

  virtual bool process( IFS * ifs );

  /// Get the number of IFS.
  const uint32_t getIFS() const;
  //@}

  virtual bool process( Text * text );

  const uint32_t getText() const;

  virtual bool process( Font * font );

  const uint32_t getFont() const;

  protected:

  /// The cache where to store the already printed objects
#ifndef WIN32_STL_EXTENSION
  STDEXT::hash_set<uint32_t,STDEXT::hash<uint32_t>,std::equal_to<uint32_t> > __cache;
#else
  STDEXT::hash_set<uint32_t> __cache;
#endif

  /// nb element.
  uint32_t __element;

  /// nb named element.
  uint32_t __named;

  /// nb shape by class.
  std::vector<uint32_t> __shape;

  /// memory size.
  uint32_t __memsize;

};


/* ------------------------------------------------------------------------- */

// __actn_statisticcomputer_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

