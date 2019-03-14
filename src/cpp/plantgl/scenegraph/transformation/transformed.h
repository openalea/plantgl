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



/*! \file geom_transformed.h
    \brief Definition of the transformation class Transformation, Transformation3D, Transformation2D
	and the transformed class Transformed.
*/

#ifndef __geom_transformed_h__
#define __geom_transformed_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/scenegraph/geometry/geometry.h>

#ifndef GEOM_FWDEF

#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/container/pointarray.h>

#endif

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

#ifdef GEOM_FWDEF

class Point2Array;
class Point3Array;
class Point4Array;
class Point2Matrix;
class Point3Matrix;
class Point4Matrix;

typedef RCPtr<Point2Array> Point2ArrayPtr;
typedef RCPtr<Point3Array> Point3ArrayPtr;
typedef RCPtr<Point4Array> Point4ArrayPtr;

typedef RCPtr<Point2Matrix> Point2MatrixPtr;
typedef RCPtr<Point3Matrix> Point3MatrixPtr;
typedef RCPtr<Point4Matrix> Point4MatrixPtr;

#endif

/* ----------------------------------------------------------------------- */

/** 
    \class Transformation
    \brief Abstract base class for affine transformations.
*/

/* ----------------------------------------------------------------------- */

class SG_API Transformation : public RefCountObject
{

public:

  /// Constructs a Transformation.
  Transformation( );

  /// Destructor.
  virtual ~Transformation( );

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const = 0;

};

/// Transformation Pointer
typedef RCPtr<Transformation> TransformationPtr;


/* ----------------------------------------------------------------------- */

/** 
    \class Transformation3D
    \brief Abstract base class for 3D affine transformations.
*/

class SG_API Transformation3D : public Transformation
{

public:

  /// Constructs a Transformation.
  Transformation3D( ) ;

  /// Destructor.
  virtual ~Transformation3D( );

  /// Transforms the array of 3D points \e points.
  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const = 0;

  /// Transforms the array of 4D points \e points.
  virtual Point4ArrayPtr transform( const Point4ArrayPtr& points ) const = 0;

  /// Transforms the matrix of 3D points \e points.
  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const = 0;

  /// Transforms the matrix of 4D points \e points.
  virtual Point4MatrixPtr transform( const Point4MatrixPtr& points ) const = 0;

};

/// Transformation3D Pointer
typedef RCPtr<Transformation3D> Transformation3DPtr;


/* ----------------------------------------------------------------------- */

/** 
    \class Transformed
    \brief A transformed 
*/


class SG_API Transformed : public Geometry
{

public:

   /// A structure which helps to build an object of type of Transformed.
  struct SG_API Builder : public Geometry::Builder {

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

  };


  /// Construcs a Transformed.
  Transformed( );

  /// Destructor.
  virtual ~Transformed( );

  /// Returns the transformation attached to \e self.
  virtual Transformation3DPtr getTransformation( ) const = 0;

  virtual const GeometryPtr getGeometry() const = 0;

  virtual bool hasDynamicRendering() const { return getGeometry()->hasDynamicRendering(); }

};
 
/// Transformed Pointer
typedef RCPtr<Transformed> TransformedPtr;

/* ----------------------------------------------------------------------- */

/** 
    \class Transformation2D
    \brief Abstract base class for 2D affine transformations.
*/

class SG_API Transformation2D : public Transformation
{

public:

  /// Constructs a Transformation.
  Transformation2D( );

  /// Destructor.
  virtual ~Transformation2D( );

  /// Transforms the array of 2D points \e points.
  virtual Point2ArrayPtr transform( const Point2ArrayPtr& points ) const = 0;

  /// Transforms the array of 3D points \e points.
  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const = 0;

  /// Transforms the matrix of 2D points \e points.
  virtual Point2MatrixPtr transform( const Point2MatrixPtr& points ) const = 0;

  /// Transforms the matrix of 3D points \e points.
  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const = 0;

};

/// Transformation2D Pointer
typedef RCPtr<Transformation2D> Transformation2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_transformed_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

