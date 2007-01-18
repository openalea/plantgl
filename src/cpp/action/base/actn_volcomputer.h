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

/*! \file actn_volcomputer.h
    \brief Definition of the action class VolComputer.
*/

#ifndef __actn_volcomputer_h__
#define __actn_volcomputer_h__


#include "actn_surfcomputer.h"

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class VolComputer
   \brief An action which computes volume of objects of type of Geometry.
*/


class GEOM_API VolComputer : public SurfComputer
{

public:

  /// Constructs a SurfComputer.
  VolComputer( Discretizer& discretizer );

  /// Destructor
  virtual ~VolComputer( ) ;

  /// Returns the resulting volume when applying \e self for the last time.
  real_t getVolume( ) ;

  /// @name GeomShape
  //@{
  virtual bool process(GeomShape * geomShape);

  virtual bool process(GeomInline * geomInline);

  //@}

  /// @name Material
  //@{

  virtual bool process( Material * material ) ;

  virtual bool process( MonoSpectral * monoSpectral );

  virtual bool process( MultiSpectral * multiSpectral );

  virtual bool process( ImageTexture * texture );

  //@}

  /// @name Geom3D
  //@{

  virtual bool process( AmapSymbol * amapSymbol );

  virtual bool process( AsymmetricHull * asymmetricHull );

  virtual bool process( AxisRotated * axisRotated );

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

  virtual bool process( NurbsPatch * nurbsPatch );

  virtual bool process( Oriented * oriented );

  virtual bool process( Paraboloid * paraboloid );

  virtual bool process( QuadSet * quadSet );

  virtual bool process( Revolution * revolution );

  virtual bool process( Swung * swung );

  virtual bool process( Scaled * scaled );

  virtual bool process( Sphere * sphere );

  virtual bool process( Tapered * tapered );

  virtual bool process( Translated * translated );

  virtual bool process( TriangleSet * triangleSet );

  //@}

  /// @name Geom2D
  //@{

  virtual bool process( Disc * disc );


  //@}

  /// process \e this on \e scene.
  virtual bool process(const ScenePtr scene);
  virtual bool process(const Scene& scene);

};

/// Compute the volume of the objects in the scene \e _scene
const real_t GEOM_API sceneVolume(const ScenePtr scene);

const real_t GEOM_API sceneVolume(const Scene& scene);

/* ------------------------------------------------------------------------- */

// __actn_surfcomputer_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif


