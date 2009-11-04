/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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
    \brief Definition of the geometry class Polyline and Polyline2D.
*/


#ifndef __geom_polyline_h__
#define __geom_polyline_h__

/* ----------------------------------------------------------------------- */

#include "explicitmodel.h"
#include "lineicmodel.h"
#include "curve.h"
#include "disc.h"
#include <plantgl/math/util_math.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Point2Array;
typedef RCPtr<Point2Array> Point2ArrayPtr;

/* ----------------------------------------------------------------------- */

class Polyline;
/// Polyline Pointer
typedef RCPtr<Polyline> PolylinePtr;

/**
   \class Polyline
   \brief A polyline represented by an array of points 3D.
*/

class SG_API Polyline : public  ExplicitModel, public LineicModel
{

public:

  /// A structure which helps to build a Polyline when parsing.
  struct SG_API Builder : public ExplicitModel::Builder {

    /// A pointer to the \b Width field.
 	uchar_t * Width;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  Polyline(uchar_t width = DEFAULT_WIDTH);

  /** Constructs a Polyline with the segment defined by \e point1 and
     \e point2.
     \post
     - \e self is valid. */
  Polyline( const TOOLS(Vector3)& point1, const TOOLS(Vector3)& point2, uchar_t width = DEFAULT_WIDTH);

  /** Constructs a Polyline with the points \e points.
      \pre
      - \e points must contain at least 2 points.
      \post
      - \e self is valid. */
  Polyline( const Point3ArrayPtr& points, uchar_t width = DEFAULT_WIDTH);

  /** Constructs a Polyline with the points \e points.
      \pre
      - \e points must contain at least 2 points.
      \post
      - \e self is valid. */
  Polyline( const Point3ArrayPtr& points, const Color4ArrayPtr& colors, uchar_t width = DEFAULT_WIDTH);

  /// Destructor
  virtual ~Polyline( );

  PGL_OBJECT(Polyline)

  /** Returns the value of the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  const TOOLS(Vector3)& getPointListAt( uint_t i ) const;

  /** Returns the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  TOOLS(Vector3)& getPointListAt( uint_t i );

  virtual const real_t getFirstKnot() const;

  virtual const real_t getLastKnot() const;

  virtual const uint_t getStride() const;

  virtual TOOLS(Vector3) getPointAt(real_t u) const;

  virtual TOOLS(Vector3) getTangentAt(real_t u) const;

  virtual TOOLS(Vector3) getNormalAt(real_t u) const;

  virtual bool isValid( ) const;

  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const;

  std::pair<PolylinePtr,PolylinePtr> split(real_t u) const ;

}; // Polyline



/* ----------------------------------------------------------------------- */

class Polyline2D;
/// Polyline2D Pointer
typedef RCPtr<Polyline2D> Polyline2DPtr;

/**
   \class Polyline2D
   \brief A polyline represented by an array of 2D points.
*/


class SG_API Polyline2D : public Curve2D
{

public:

    static Polyline2DPtr Circle(real_t radius = Disc::DEFAULT_RADIUS, uchar_t slices = SOR::DEFAULT_SLICES);
    static Polyline2DPtr ArcOfCircle(real_t radius = Disc::DEFAULT_RADIUS,
									 real_t starting_angle = 0,
									 real_t angle_range = GEOM_PI,
									 uchar_t slices = SOR::DEFAULT_SLICES);

    static Polyline2DPtr SuperEllipsis(real_t radius = Disc::DEFAULT_RADIUS,
									   real_t heigth = Disc::DEFAULT_RADIUS,
									   real_t degree = 2,
									   real_t starting_angle = 0,
									   real_t angle_range = GEOM_TWO_PI,
									   uchar_t slices = SOR::DEFAULT_SLICES);

  /// A structure which helps to build a Polyline when parsing.
  struct SG_API Builder : public Curve2D::Builder {

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
  Polyline2D(uchar_t width = DEFAULT_WIDTH);

  /** Constructs a Polyline2D with the segment defined by \e point1 and
     \e point2.
     \post
     - \e self is valid. */
  Polyline2D( const TOOLS(Vector2)& point1, const TOOLS(Vector2)& point2, uchar_t width = DEFAULT_WIDTH );

  /** Constructs a Polyline2D with the points \e points.
      \pre
      - \e points must contain at least 2 points.
      \post
      - \e self is valid. */
  Polyline2D( const Point2ArrayPtr& points, uchar_t width = DEFAULT_WIDTH );

  /// Destructor
  virtual ~Polyline2D( );

  PGL_OBJECT(Polyline2D)

  /** Returns the value of the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  const TOOLS(Vector2)& getPointListAt( uint_t i ) const;

  /** Returns the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  TOOLS(Vector2)& getPointListAt( uint_t i );

  /// Returns \b PointList value.
  const Point2ArrayPtr& getPointList( ) const ;

  /// Returns \b PointList field.
  Point2ArrayPtr& getPointList( );

  /// Returns the size of \b PointList.
  uint_t getPointListSize( ) const ;

  virtual const real_t getFirstKnot() const;

  virtual const real_t getLastKnot() const;

  virtual const uint_t getStride() const;

  virtual TOOLS(Vector2) getPointAt(real_t u) const;

  virtual TOOLS(Vector2) getTangentAt(real_t u) const;

  virtual TOOLS(Vector2) getNormalAt(real_t u) const;

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

  std::pair<Polyline2DPtr,Polyline2DPtr> split(real_t u) const ;

/*  virtual ExplicitModel2DPtr
  transform( const Transformation2DPtr& transformation ) const;
*/


protected:

  /// The \b PointList field.
  Point2ArrayPtr __pointList;

}; // Polyline2D



/* ----------------------------------------------------------------------- */

// __geom_polyline_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

