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
//#include <plantgl/math/util_math.h>

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
  Polyline( const Vector3& point1, const Vector3& point2, uchar_t width = DEFAULT_WIDTH);

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
  const Vector3& getPointListAt( uint_t i ) const;

  /** Returns the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  Vector3& getPointListAt( uint_t i );

  virtual const real_t getFirstKnot() const;

  virtual const real_t getLastKnot() const;

  virtual const uint_t getStride() const;

  virtual Vector3 getPointAt(real_t u) const;

  virtual Vector3 getTangentAt(real_t u) const;

  virtual Vector3 getNormalAt(real_t u) const;

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
    static const real_t DEFAULT_ANGLE;

    static Polyline2DPtr Circle(real_t radius = Disc::DEFAULT_RADIUS, uchar_t slices = SOR::DEFAULT_SLICES);
    static Polyline2DPtr ArcOfCircle(real_t radius = Disc::DEFAULT_RADIUS,
                                     real_t starting_angle = 0,
                                     real_t angle_range = DEFAULT_ANGLE,
                                     uchar_t slices = SOR::DEFAULT_SLICES);

    static Polyline2DPtr SuperEllipsis(real_t radius = Disc::DEFAULT_RADIUS,
                                       real_t heigth = Disc::DEFAULT_RADIUS,
                                       real_t degree = 2,
                                       real_t starting_angle = 0,
                                       real_t angle_range = DEFAULT_ANGLE*2,
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
  Polyline2D( const Vector2& point1, const Vector2& point2, uchar_t width = DEFAULT_WIDTH );

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
  const Vector2& getPointListAt( uint_t i ) const;

  /** Returns the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  Vector2& getPointListAt( uint_t i );

  /// Returns \b PointList value.
  const Point2ArrayPtr& getPointList( ) const ;

  /// Returns \b PointList field.
  Point2ArrayPtr& getPointList( );

  /// Returns the size of \b PointList.
  uint_t getPointListSize( ) const ;

  virtual const real_t getFirstKnot() const;

  virtual const real_t getLastKnot() const;

  virtual const uint_t getStride() const;

  virtual Vector2 getPointAt(real_t u) const;

  virtual Vector2 getTangentAt(real_t u) const;

  virtual Vector2 getNormalAt(real_t u) const;

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

