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
