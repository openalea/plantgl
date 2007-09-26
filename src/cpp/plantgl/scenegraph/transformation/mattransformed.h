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


/*! \file geom_mattransformed.h
    \brief Definition of the transformation class Matrix4Transformation, Transform4,
	GeneralMatrix3Transformation, Transform3 and the transformed class MatrixTransformed.
*/


#ifndef __geom_matrixtransformed_h__
#define __geom_matrixtransformed_h__

/* ----------------------------------------------------------------------- */

#include "transformed.h"
#include <plantgl/math/util_matrix.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class Matrix4Transformation
    \brief Abstract base class for 4x4 matrix algebraic transformations.
*/

class SG_API Matrix4Transformation : public Transformation3D
{

public:

  /// Constructs a MatrixTransformation.
  Matrix4Transformation( );

  /// Destructor.
  virtual ~Matrix4Transformation( );

  /// Returns the homogeneous matrix \e self represents.
  virtual TOOLS(Matrix4) getMatrix( ) const = 0;

};

/// Matrix4Transformation Pointer
typedef RCPtr<Matrix4Transformation> Matrix4TransformationPtr;

/* ----------------------------------------------------------------------- */

/**
    \class Transform4
    \brief class for all 4x4 matrix algebraic transformations.
*/

// forward declaration
class Transform4;
typedef RCPtr<Transform4> Transform4Ptr;
class OrthonormalBasis3D;
typedef RCPtr<OrthonormalBasis3D> OrthonormalBasis3DPtr;

class SG_API Transform4 : public Matrix4Transformation
{

public:

  /// A structure which helps to build a Transform4Ptr when parsing.
  struct SG_API Builder
    {

    /// A pointer to the \b Translation field.
    TOOLS(Vector3) * Translation;

    /// A pointer to the \b Scale field.
    TOOLS(Vector3) * Scale;

    /// A pointer to the \b OrthonormalBasis3D field.
    OrthonormalBasis3DPtr * Rotation;

    /// Constructor.
    Builder();

    /// Desructor.
    virtual ~Builder();

    virtual Transform4Ptr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;
    };

  /// Constructs a Transform4.
  Transform4();

  /// Constructs a Transform4.
  Transform4( const TOOLS(Matrix4)& mat );

  /// Destructor.
  virtual ~Transform4( );

  virtual bool isValid() const;

  virtual TOOLS(Matrix4) getMatrix( ) const;

  virtual TOOLS(Matrix4)& getMatrix( );

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;
  virtual Point4ArrayPtr transform( const Point4ArrayPtr& points ) const;
  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;
  virtual Point4MatrixPtr transform( const Point4MatrixPtr& points ) const;

  /// Add a translation at the transformation
  Transform4& translate( const TOOLS(Vector3)& t );
  /// Add a scaling at the transformation
  Transform4& scale( const TOOLS(Vector3)& s );
  /// Add a rotation at the transformation
  Transform4& rotate( const TOOLS(Matrix3)& m );

  void getTransformation( TOOLS(Vector3)& scale,
                          TOOLS(Vector3)& rotate, // euler rotation
                          TOOLS(Vector3)& translate );

  /// compute the Matrix3 determinant of the transformation
  real_t getVolume() const;

  private:

  TOOLS(Matrix4) __matrix;

};

/// Transform4 Pointer
typedef RCPtr<Transform4> Transform4Ptr;

/* ----------------------------------------------------------------------- */


/**
    \class MatrixTransformed
    \brief A matrixined transformed object
*/


class SG_API MatrixTransformed : public Transformed
{

public:

  /// A structure which helps to build an object of type of Transformed.
  struct SG_API Builder : public Transformed::Builder {

    /// A pointer to the \b Geometry field.
    GeometryPtr * Geometry;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual void destroy( );

    virtual bool isValid( ) const;

    virtual void MTDestroy( );

    virtual bool MTValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  MatrixTransformed();

  /** Construcs a MatrixTransformed with \e geometry.
      \pre
      - \e geometry must be non null. */
  MatrixTransformed( const GeometryPtr& geometry );

  /// Destructor.
  virtual ~MatrixTransformed( );

  /// Returns Geometry value.
  virtual const GeometryPtr getGeometry( ) const;

  /// Returns Geometry field.
  GeometryPtr& getGeometry( );

  virtual bool isACurve( ) const ;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const ;

  virtual bool isExplicit( ) const ;

protected:

  /// The Geometry field.
  GeometryPtr __geometry;

};

/// MatrixTransformed Pointer
typedef RCPtr<MatrixTransformed> MatrixTransformedPtr;

/* ----------------------------------------------------------------------- */

/**
    \class Matrix3Transformation
    \brief Abstract base class for 3x3 matrix algebraic transformations.
*/

class SG_API Matrix3Transformation : public Transformation2D
{

public:

  /// Constructs a MatrixTransformation.
  Matrix3Transformation( );

  /// Destructor.
  virtual ~Matrix3Transformation( );

  /// Returns the homogeneous matrix \e self represents.
  virtual TOOLS(Matrix3) getMatrix( ) const = 0;

};

/// Matrix3Transformation Pointer
typedef RCPtr<Matrix3Transformation> Matrix3TransformationPtr;


/* ----------------------------------------------------------------------- */

/**
    \class GeneralMatrix3Transformation
    \brief class for all 3x3 matrix algebraic transformations.
*/

class SG_API GeneralMatrix3Transformation : public Matrix3Transformation
{

public:

  /// Constructs a GeneralMatrixTransformation.
  GeneralMatrix3Transformation( const TOOLS(Matrix3)& mat );

  /// Destructor.
  virtual ~GeneralMatrix3Transformation( );

  virtual bool isValid() const;

  virtual TOOLS(Matrix3) getMatrix( ) const;

  virtual Point2ArrayPtr transform( const Point2ArrayPtr& points ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point2MatrixPtr transform( const Point2MatrixPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

private:

  TOOLS(Matrix3) __matrix;

};

/// GeneralMatrix3Transformation Pointer
typedef RCPtr<GeneralMatrix3Transformation> GeneralMatrix3TransformationPtr;


/* ----------------------------------------------------------------------- */


// __geom_matrixtransformed_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

