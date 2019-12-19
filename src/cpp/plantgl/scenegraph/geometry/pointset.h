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




/*! \file geom_pointset.h
    \brief Definition of the geometry class PointSet and PointSet2D.
*/


#ifndef __geom_pointset_h__
#define __geom_pointset_h__

/* ----------------------------------------------------------------------- */

#include "explicitmodel.h"
#include "planarmodel.h"
#include <plantgl/math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Point2Array;
typedef RCPtr<Point2Array> Point2ArrayPtr;

/* ----------------------------------------------------------------------- */

/**
   \class PointSet
   \brief A set of 3D points.
*/

/* ----------------------------------------------------------------------- */

class SG_API PointSet : public ExplicitModel
{

public:

  static const uchar_t DEFAULT_WIDTH;

  /// A structure which helps to build a PointSet when parsing.
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
  PointSet();

  /** Constructs a PointSet bounding the set of points \e points.
      \pre
      - \e points must contain at leat 3 points.

      \postt
      - \e self is valid. */
  PointSet( const Point3ArrayPtr& points,
            const Color4ArrayPtr& colors = Color4ArrayPtr(),
            uchar_t width = DEFAULT_WIDTH);

  /// Destructor
  virtual ~PointSet( );

  PGL_OBJECT(PointSet)

  /** Returns the value of the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  const Vector3& getPointListAt( uint_t i ) const;

  /** Returns the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  Vector3& getPointListAt( uint_t i );

  virtual bool isACurve( ) const ;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  virtual bool isValid( ) const;

  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const;

  // Get the width value of the points
  inline const uchar_t getWidth() const { return __width; }
  inline uchar_t& getWidth() { return __width; }
  inline bool isWidthToDefault( ) const { return __width == DEFAULT_WIDTH; }

  Vector3 findClosest(const Vector3& point, uint_t * index = 0) const;

protected:
  /// The \b width field
  uchar_t __width;

}; // PointSet

/// PointSet Pointer
typedef RCPtr<PointSet> PointSetPtr;


/* ----------------------------------------------------------------------- */


/**
   \class PointSet2D
   \brief A set of 2D points.
*/


class SG_API PointSet2D : public PlanarModel
{

public:


  /// A structure which helps to build a PointSet when parsing.
  struct SG_API Builder : public PlanarModel::Builder {

    /// A pointer to the \b PointList field.
    Point2ArrayPtr * PointList;

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
  PointSet2D();

  /** Constructs a PointSet2D bounding the set of points \e points.
      \pre
      - \e points must contain at leat 3 points.
      \post
      - \e self is valid. */
  PointSet2D( const Point2ArrayPtr& points, uchar_t width = PointSet::DEFAULT_WIDTH );

  /// Destructor
  virtual ~PointSet2D( ) ;

  PGL_OBJECT(PointSet2D)

  /** Returns the value of the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  const Vector2& getPointListAt( uint_t i ) const ;

  /** Returns the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  Vector2& getPointListAt( uint_t i ) ;

  /// Returns an iterator at the beginning of \b PointList.
/*  Point2Array::const_iterator getPointListBegin( ) const {
     return __pointList->begin();
  }

  /// Returns an iterator at the end of \b PointList.
  Point2Array::const_iterator getPointListEnd( ) const {
     return __pointList->end();
  }
*/
  /// Returns \b PointList value.
  const Point2ArrayPtr& getPointList( ) const;

  /// Returns \b PointList field.
  Point2ArrayPtr& getPointList( );

  /// Returns the size of \b PointList.
  uint_t getPointListSize( ) const;

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const ;

  virtual bool isValid( ) const;

/*  virtual ExplicitModelPtr
  transform( const Transformation2DPtr& transformation ) const;
*/

  // Get the width value of the points
  inline const uchar_t getWidth() const { return __width; }
  inline uchar_t& getWidth() { return __width; }
  inline bool isWidthToDefault( ) const { return __width == PointSet::DEFAULT_WIDTH; }

  Vector2 findClosest(const Vector2& point, uint_t * index = 0) const;

protected:

  /// The \b PointList field.
  Point2ArrayPtr __pointList;

  /// The \b width field
  uchar_t __width;

}; // Point2DSet

/// PointSet2D Pointer
typedef RCPtr<PointSet2D> PointSet2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_pointset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

