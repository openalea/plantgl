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

