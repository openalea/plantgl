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

class SG_API Point2Matrix : public Array2<Vector2>
{

public:

  /** Constructs a Point2Matrix of size of \e size.
      \post
      - \e self is valid. */
  Point2Matrix( uint_t rows = 0, uint_t cols =0 );

  /** Constructs a Point2Matrix of size of \e size.
      \post
      - \e self is valid. */
  Point2Matrix( uint_t rows, uint_t cols, const Vector2& value);

  /** Constructs a PointMatrix with the range of points [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
      Point2Matrix( InIterator first, InIterator last, uint_t rows ) :
    Array2<Vector2>(first,last,rows) {
    GEOM_ASSERT(isValid());
  }

  /// Destructor
  virtual ~Point2Matrix( );

  /// Returns the mimimum and maximum points bounding \e self.
  std::pair<Vector2,Vector2> getBounds( ) const;

  /// Returns the center of \e self.
  Vector2 getCenter( ) const;

  /// Returns the extent along the \c axis and \c y axis of \e self.
  Vector2 getExtent( ) const;

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
PGL_DECLARE_TYPE(Point2Matrix)


/* ----------------------------------------------------------------------- */


/**
   \class Point3Matrix
   \brief A matrix of 3D points.
*/


class SG_API Point3Matrix : public Array2<Vector3>
{

public:

  /** Constructs a Point3Matrix of rows \e rows and columns \e cols.
      \post
      - \e self is valid. */
  Point3Matrix( uint_t rows = 0,uint_t cols = 0 );

  /** Constructs a Point3Matrix of rows \e rows and columns \e cols.
      \post
      - \e self is valid. */
  Point3Matrix( uint_t rows, uint_t cols, const  Vector3& value);

  /** Constructs a Point3Matrix with the range of points [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Point3Matrix( InIterator first, InIterator last, uint_t rows ) :
    Array2<Vector3>(first,last,rows) {
    GEOM_ASSERT(isValid());
  }

  /** Constructs a Point3Matrix with the Point2Matrix \e points2, \c z
      coordinate for each points is set to \e z.
      \pre
      - \e points2 must non null and valid.
      \post
      - \e self is valid. */
  Point3Matrix( const Point2MatrixPtr& points2, const real_t& z );
  Point3Matrix( const Point2Matrix& points2, const real_t& z );

  /// Destructor
  virtual ~Point3Matrix( );

  /// Returns the mimimum and maximum points bounding \e self.
  std::pair<Vector3,Vector3> getBounds( ) const;

  /// Returns the center of \e self.
  Vector3 getCenter( ) const;

  /// Returns the extent along the \c axis and \c y axis of \e self.
  Vector3 getExtent( ) const;

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
PGL_DECLARE_TYPE(Point3Matrix)


/* ----------------------------------------------------------------------- */


/**
   \class Point4Matrix
   \brief An of 4D points.
*/


class SG_API Point4Matrix : public Array2<Vector4>
{

public:

  /** Constructs a Point4Matrix with \e rows rows and \e cols columns.
      \post
      - \e self is valid. */
  Point4Matrix( uint_t rows = 0, uint_t cols = 0);

  /** Constructs a Point4Matrix with \e rows rows and \e cols columns.
      \post
      - \e self is valid. */
  Point4Matrix( uint_t rows, uint_t cols, const Vector4& value);

  /** Constructs a Point4Matrix with the range of points [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Point4Matrix( InIterator first, InIterator last ,uint_t rows) :
    Array2<Vector4>(first,last,rows) {
    GEOM_ASSERT(isValid());
  }

  /** Constructs a Point4Matrix with the Point3Matrix \e points3, \c w
      coordinate for each points is set to \e w.
      points
      \pre
      - \e points3 must non null and valid.
      \post
      - \e self is valid. */
  Point4Matrix( const Point3Matrix& points3, const real_t& w );
  Point4Matrix( const Point3MatrixPtr& points3, const real_t& w );

  /// Destructor
  virtual ~Point4Matrix( );

  /// Returns the mimimum and maximum points bounding \e self.
  std::pair<Vector4,Vector4> getBounds( ) const;

  /// Returns the center of \e self.
  Vector4 getCenter( ) const;

  /// Returns the extent along the \c axis and \c y axis of \e self.
  Vector4 getExtent( ) const;

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
PGL_DECLARE_TYPE(Point4Matrix)


/* ----------------------------------------------------------------------- */

// __geom_pointmatrix.h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif


