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

/*! \file actn_tesselator.h
    \brief Definition of the action class Tesselator.
*/

#ifndef __actn_tesselator_h__
#define __actn_tesselator_h__


#include "discretizer.h"
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


class TriangleSet;
typedef RCPtr<TriangleSet> TriangleSetPtr;


/* ----------------------------------------------------------------------- */

/**
   \class Tesselator
   \brief An action which computes the discretization of \e Geometry objects.
*/


class ALGO_API Tesselator : public Discretizer
{

public:

  /// Constructs a Tesselator.
  Tesselator( );

  /// Destructor
  virtual ~Tesselator( );

  /// Returns the last computed triangulation when applying \e self.
  TriangleSetPtr getTriangulation( ) const;

  /// @name Geom3D
  //@{

  virtual bool process( AmapSymbol * amapSymbol );

  virtual bool process( BezierCurve * bezierCurve );

  virtual bool process( BezierPatch * bezierPatch );

  virtual bool process( Box * box );

  virtual bool process( Cylinder * cylinder );

  virtual bool process( FaceSet * faceSet );

  virtual bool process( Frustum * frustum );


  /** Applies \e self to an object of type Extrusion.
      Using Double-Cross Method To calculate Reference Frame along the Axis.
      See Bloomenthal Thesis, chap 5, for more Details.
      \warning
      - \e extrusion must be non null and valid.*/
  virtual bool process( Extrusion * extrusion );

  virtual bool process( NurbsCurve * nurbsCurve );

  virtual bool process( NurbsPatch * nurbsPatch );

  virtual bool process( PointSet * pointSet );

  virtual bool process( Polyline * polyline );

  virtual bool process( QuadSet * quadSet );

  //@}

  /// @name Geom2D
  //@{

  virtual bool process( BezierCurve2D * bezierCurve );

  virtual bool process( NurbsCurve2D * nurbsCurve );

  virtual bool process( PointSet2D * pointSet );

  virtual bool process( Polyline2D * polyline );

  //@}

};

enum TriangulationMethod {
    eStarTriangulation,
    eConvexTriangulation,
    eGreeneTriangulation,
    eOptimalTriangulation,
    eYMonotonePartitioning
};

ALGO_API bool is_simple_polygon(Point2ArrayPtr contour);

ALGO_API IndexArrayPtr polygonization(Point2ArrayPtr contour, TriangulationMethod method = eStarTriangulation);

ALGO_API Index3ArrayPtr triangulation(Point2ArrayPtr contour, TriangulationMethod method = eStarTriangulation);

/* ------------------------------------------------------------------------*/

// __actn_tesselator_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
