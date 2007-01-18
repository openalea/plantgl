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


/*! \file geom_polyline.h
    \brief Definition of the geometry class GeomPolyline and GeomPolyline2D.
*/


#ifndef __geom_polyline_h__
#define __geom_polyline_h__

/* ----------------------------------------------------------------------- */

#include "geom_explicitmodel.h"
#include "geom_lineicmodel.h"
#include "geom_curve.h"

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Point2Array;
typedef RCPtr<Point2Array> Point2ArrayPtr;

/* ----------------------------------------------------------------------- */

/**
   \class GeomPolyline
   \brief A polyline represented by an array of points 3D.
*/

class GEOM_API GeomPolyline : public  ExplicitModel, public LineicModel
{

public:

  /// A structure which helps to build a GeomPolyline when parsing.
  struct Builder : public ExplicitModel::Builder {

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  GeomPolyline();

  /** Constructs a GeomPolyline with the segment defined by \e point1 and
     \e point2.
     \post
     - \e self is valid. */
  GeomPolyline( const TOOLS(Vector3)& point1, const TOOLS(Vector3)& point2 );

  /** Constructs a GeomPolyline with the points \e points.
      \pre
      - \e points must contain at least 2 points.
      \post
      - \e self is valid. */
  GeomPolyline( const Point3ArrayPtr& points, const Color4ArrayPtr& colors = 0 );

  /// Destructor
  virtual ~GeomPolyline( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /** Returns the value of the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  const TOOLS(Vector3)& getPointListAt( uint32_t i ) const;

  /** Returns the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  TOOLS(Vector3)& getPointListAt( uint32_t i );

  virtual const real_t getFirstKnot() const;

  virtual const real_t getLastKnot() const;

  virtual const uint32_t getStride() const;

  virtual TOOLS(Vector3) getPointAt(real_t u) const;

  virtual TOOLS(Vector3) getTangentAt(real_t u) const;

  virtual TOOLS(Vector3) getNormalAt(real_t u) const;

  virtual bool isValid( ) const;

  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const;

}; // GeomPolyline

/// GeomPolyline Pointer
typedef RCPtr<GeomPolyline> GeomPolylinePtr;


/* ----------------------------------------------------------------------- */

/**
   \class GeomPolyline2D
   \brief A polyline represented by an array of 2D points.
*/


class GEOM_API GeomPolyline2D : public Curve2D
{

public:

  /// A structure which helps to build a GeomPolyline when parsing.
  struct Builder : public Curve2D::Builder {

    /// A pointer to the \b PointList field.
    Point2ArrayPtr * PointList;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  GeomPolyline2D( );

  /** Constructs a GeomPolyline2D with the segment defined by \e point1 and
     \e point2.
     \post
     - \e self is valid. */
  GeomPolyline2D( const TOOLS(Vector2)& point1, const TOOLS(Vector2)& point2 );

  /** Constructs a GeomPolyline2D with the points \e points.
      \pre
      - \e points must contain at least 2 points.
      \post
      - \e self is valid. */
  GeomPolyline2D( const Point2ArrayPtr& points );

  /// Destructor
  virtual ~GeomPolyline2D( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /** Returns the value of the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  const TOOLS(Vector2)& getPointListAt( uint32_t i ) const;

  /** Returns the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  TOOLS(Vector2)& getPointListAt( uint32_t i );

  /// Returns \b PointList value.
  const Point2ArrayPtr& getPointList( ) const ;

  /// Returns \b PointList field.
  Point2ArrayPtr& getPointList( );

  /// Returns the size of \b PointList.
  uint32_t getPointListSize( ) const ;

  virtual const real_t getFirstKnot() const;

  virtual const real_t getLastKnot() const;

  virtual const uint32_t getStride() const;

  virtual TOOLS(Vector2) getPointAt(real_t u) const;

  virtual bool isACurve( ) const {
    return true;
  }

  virtual bool isASurface( ) const {
    return false;
  }

  virtual bool isAVolume( ) const {
    return false;
  }

  virtual bool isValid( ) const;

/*  virtual ExplicitModel2DPtr
  transform( const Transformation2DPtr& transformation ) const;
*/
protected:

  /// The \b PointList field.
  Point2ArrayPtr __pointList;

}; // GeomPolyline2D

/// GeomPolyline2D Pointer
typedef RCPtr<GeomPolyline2D> GeomPolyline2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_polyline_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

