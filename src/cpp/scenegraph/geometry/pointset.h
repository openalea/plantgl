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



/*! \file geom_pointset.h
    \brief Definition of the geometry class PointSet and PointSet2D.
*/


#ifndef __geom_pointset_h__
#define __geom_pointset_h__

/* ----------------------------------------------------------------------- */

#include "explicitmodel.h"
#include "planarmodel.h"
#include <math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Point2Array;
typedef RCPtr<Point2Array> Point2ArrayPtr;

/* ----------------------------------------------------------------------- */

/**
   \class PointSet
   \brief An point set bounding a set of points 3D.
*/

/* ----------------------------------------------------------------------- */

class SG_API PointSet : public ExplicitModel
{

public:


  /// A structure which helps to build a PointSet when parsing. 
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
  PointSet();

  /** Constructs a PointSet bounding the set of points \e points.
      \pre
      - \e points must contain at leat 3 points.
    
      \postt
      - \e self is valid. */
  PointSet( const Point3ArrayPtr& points, const Color4ArrayPtr& colors = 0 );

  /// Destructor
  virtual ~PointSet( );

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

/*  /// Returns an iterator at the beginning of \b PointList.
  Point3Array::const_iterator getPointListBegin( ) const {
     return __pointList->getBegin();   
  }

  /// Returns an iterator at the end of \b PointList.
  Point3Array::const_iterator getPointListEnd( ) const {
     return __pointList->getEnd();   
  }*/

  virtual bool isACurve( ) const ;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  virtual bool isValid( ) const;

  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const;

}; // PointSet

/// PointSet Pointer
typedef RCPtr<PointSet> PointSetPtr;


/* ----------------------------------------------------------------------- */


/**
   \class PointSet2D
   \brief An point set bounding a set of points 2D.
*/


class SG_API PointSet2D : public PlanarModel
{

public:


  /// A structure which helps to build a PointSet when parsing. 
  struct Builder : public PlanarModel::Builder { 

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
  PointSet2D();

  /** Constructs a PointSet2D bounding the set of points \e points.
      \pre
      - \e points must contain at leat 3 points.    
      \post
      - \e self is valid. */
  PointSet2D( const Point2ArrayPtr& points );

  /// Destructor
  virtual ~PointSet2D( ) ;

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /** Returns the value of the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  const TOOLS(Vector2)& getPointListAt( uint32_t i ) const ;

  /** Returns the \e i-th point of \b PointList.
      \pre
      - \e i must belong to the range [0,size of \b PointList). */
  TOOLS(Vector2)& getPointListAt( uint32_t i ) ;

  /// Returns an iterator at the beginning of \b PointList.
/*  Point2Array::const_iterator getPointListBegin( ) const {
     return __pointList->getBegin();   
  }

  /// Returns an iterator at the end of \b PointList.
  Point2Array::const_iterator getPointListEnd( ) const {
     return __pointList->getEnd();   
  }
*/
  /// Returns \b PointList value.
  const Point2ArrayPtr& getPointList( ) const;

  /// Returns \b PointList field.
  Point2ArrayPtr& getPointList( );

  /// Returns the size of \b PointList.
  uint32_t getPointListSize( ) const;

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const ;

  virtual bool isValid( ) const;

/*  virtual ExplicitModelPtr
  transform( const Transformation2DPtr& transformation ) const;
*/
protected:
  
  /// The \b PointList field.
  Point2ArrayPtr __pointList;

}; // Point2DSet

/// PointSet2D Pointer
typedef RCPtr<PointSet2D> PointSet2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_pointset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

