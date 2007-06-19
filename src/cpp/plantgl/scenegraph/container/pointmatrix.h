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



/*! \file geom_pointmatrix.h
    \brief Definition of the container class Point2Matrix, Point3Matrix and Point4Matrix.
*/

#ifndef __geom_pointmatrix_h__
#define __geom_pointmatrix_h__

/* ----------------------------------------------------------------------- */
#include "../sg_config.h"
#include <plantgl/tool/util_array2.h>
#include <plantgl/math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Point2Matrix
   \brief A matrix of 2D points.
*/

/* ----------------------------------------------------------------------- */

class SG_API Point2Matrix : public TOOLS(Array2)<TOOLS(Vector2)>
{

public:

  /** Constructs a Point2Matrix of size of \e size.
      \post
      - \e self is valid. */
  Point2Matrix( uint32_t rows = 0, uint32_t cols =0 );

  /** Constructs a PointMatrix with the range of points [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
      Point2Matrix( InIterator first, InIterator last, uint32_t rows ) :
    TOOLS(Array2)<TOOLS(Vector2)>(first,last,rows) {
    GEOM_ASSERT(isValid());
  }

  /// Destructor
  virtual ~Point2Matrix( );

  /// Returns the mimimum and maximum points bounding \e self.
  std::pair<TOOLS(Vector2),TOOLS(Vector2)> getBounds( ) const;

  /// Returns the center of \e self.
  TOOLS(Vector2) getCenter( ) const;

  /// Returns the extent along the \c axis and \c y axis of \e self.
  TOOLS(Vector2) getExtent( ) const;

  /** Returns the total length of the segments formed adjacent points.
      It takes sense in the case that \e self denotes a polyline. */
  //real_t getLength( ) const;

  /// Returns an iterator at the point with the maximum \c x coordinate.
  const_iterator getXMax( ) const;

  /// Returns an iterator at the point with the minimum \c x coordinate.
  const_iterator getXMin( ) const;

  /** Returns a pair of iterator at first the point with the minimum and
      second the point with the maximum \c x coordinate. */
  std::pair<const_iterator,const_iterator> getXMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c y coordinate.
  const_iterator getYMax( ) const;

  /// Returns an iterator at the point with the minimum \c y coordinate.
  const_iterator getYMin( ) const;

  /** Returns first an iterator at the point with the minimum \c y
      coordinate and second an iterator at the point with the maximum
      \c y coordinate. */
  std::pair<const_iterator,const_iterator> getYMinAndMax( ) const;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  /// Normalize all the points.
  void normalize();

};

/// Point2Matrix Pointer
typedef RCPtr<Point2Matrix> Point2MatrixPtr;


/* ----------------------------------------------------------------------- */


/**
   \class Point3Matrix
   \brief A matrix of 3D points.
*/


class SG_API Point3Matrix : public TOOLS(Array2)<TOOLS(Vector3)>
{

public:

  /** Constructs a Point3Matrix of rows \e rows and columns \e cols.
      \post
      - \e self is valid. */
  Point3Matrix( uint32_t rows = 0,uint32_t cols = 0 );

  /** Constructs a Point3Matrix with the range of points [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Point3Matrix( InIterator first, InIterator last, uint32_t rows ) :
    TOOLS(Array2)<TOOLS(Vector3)>(first,last,rows) {
    GEOM_ASSERT(isValid());
  }

  /** Constructs a Point3Matrix with the Point2Matrix \e points2, \c z
      coordinate for each points is set to \e z.
      \pre
      - \e points2 must non null and valid.
      \post
      - \e self is valid. */
  Point3Matrix( const Point2MatrixPtr& points2, const real_t& z );

  /// Destructor
  virtual ~Point3Matrix( );

  /// Returns the mimimum and maximum points bounding \e self.
  std::pair<TOOLS(Vector3),TOOLS(Vector3)> getBounds( ) const;

  /// Returns the center of \e self.
  TOOLS(Vector3) getCenter( ) const;

  /// Returns the extent along the \c axis and \c y axis of \e self.
  TOOLS(Vector3) getExtent( ) const;

  /// Returns an iterator at the point with the maximum \c x coordinate.
  const_iterator getXMax( ) const;

  /// Returns an iterator at the point with the minimum \c x coordinate.
  const_iterator getXMin( ) const;

  /** Returns a pair of iterator at first the point with the minimum and
      second the point with the maximum \c x coordinate. */
  std::pair<const_iterator,const_iterator> getXMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c y coordinate.
  const_iterator getYMax( ) const;

  /// Returns an iterator at the point with the minimum \c y coordinate.
  const_iterator getYMin( ) const;

  /** Returns first an iterator at the point with the minimum \c y
      coordinate and second an iterator at the point with the maximum
      \c y coordinate. */
  std::pair<const_iterator,const_iterator> getYMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c z coordinate.
  const_iterator getZMax( ) const;

  /// Returns an iterator at the point with the minimum \c z coordinate.
  const_iterator getZMin( ) const;

 /** Returns first an iterator at the point with the minimum \c z
      coordinate and second an iterator at the point with the maximum
      \c z coordinate. */
  std::pair<const_iterator,const_iterator> getZMinAndMax( ) const;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  /** Projects \e self in 2D space.
      For each points, the \c x and \c y  coordinates are divided by the
      \c z coordinate. */
  Point2MatrixPtr project( ) const;

  /// Normalize all the points.
  void normalize();

};

/// Point3Matrix Pointer
typedef RCPtr<Point3Matrix> Point3MatrixPtr;


/* ----------------------------------------------------------------------- */


/**
   \class Point4Matrix
   \brief An of 4D points.
*/


class SG_API Point4Matrix : public TOOLS(Array2)<TOOLS(Vector4)>
{

public:

  /** Constructs a Point4Matrix with \e rows rows and \e cols columns.
      \post
      - \e self is valid. */
  Point4Matrix( uint32_t rows = 0, uint32_t cols = 0);

  /** Constructs a Point4Matrix with the range of points [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Point4Matrix( InIterator first, InIterator last ,uint32_t rows) :
    TOOLS(Array2)<TOOLS(Vector4)>(first,last,rows) {
    GEOM_ASSERT(isValid());
  }

  /** Constructs a Point4Matrix with the Point3Matrix \e points3, \c w
      coordinate for each points is set to \e w.
      points
      \pre
      - \e points3 must non null and valid.
      \post
      - \e self is valid. */
  Point4Matrix( const Point3MatrixPtr& points3, const real_t& w );

  /// Destructor
  virtual ~Point4Matrix( );

  /// Returns the mimimum and maximum points bounding \e self.
  std::pair<TOOLS(Vector4),TOOLS(Vector4)> getBounds( ) const;

  /// Returns the center of \e self.
  TOOLS(Vector4) getCenter( ) const;

  /// Returns the extent along the \c axis and \c y axis of \e self.
  TOOLS(Vector4) getExtent( ) const;

  /** Returns the total length of the segments formed adjacent points.
      It takes sense in the case that \e self denotes a polyline. */
  //real_t getLength( ) const;

  /// Returns an iterator at the point with the maximum \c x coordinate.
  const_iterator getXMax( ) const;

  /// Returns an iterator at the point with the minimum \c x coordinate.
  const_iterator getXMin( ) const;

  /** Returns a pair of iterator at first the point with the minimum and
      second the point with the maximum \c x coordinate. */
  std::pair<const_iterator,const_iterator> getXMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c y coordinate.
  const_iterator getYMax( ) const;

  /// Returns an iterator at the point with the minimum \c y coordinate.
  const_iterator getYMin( ) const;

  /** Returns first an iterator at the point with the minimum \c y
      coordinate and second an iterator at the point with the maximum
      \c y coordinate. */
  std::pair<const_iterator,const_iterator> getYMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c z coordinate.
  const_iterator getZMax( ) const;

  /// Returns an iterator at the point with the minimum \c z coordinate.
  const_iterator getZMin( ) const;

 /** Returns first an iterator at the point with the minimum \c z
      coordinate and second an iterator at the point with the maximum
      \c z coordinate. */
  std::pair<const_iterator,const_iterator> getZMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c w coordinate.
  const_iterator getWMax( ) const;

  /// Returns an iterator at the point with the minimum \c w coordinate.
  const_iterator getWMin( ) const;

 /** Returns first an iterator at the point with the minimum \c w
      coordinate and second an iterator at the point with the maximum
      \c w coordinate. */
  std::pair<const_iterator,const_iterator> getWMinAndMax( ) const;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  /** Projects \e self in 2D space.
      For each points, the \c x, \c y and \c z coordinates are divided by
      the \c w coordinate. */
  Point3MatrixPtr project( ) const;

  /// Normalize all the points.
  void normalize();

};

/// Point4Matrix Pointer
typedef RCPtr<Point4Matrix> Point4MatrixPtr;


/* ----------------------------------------------------------------------- */

// __geom_pointmatrix.h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif


