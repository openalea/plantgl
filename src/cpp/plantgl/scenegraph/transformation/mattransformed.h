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
  virtual Matrix4 getMatrix( ) const = 0;

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
    Vector3 * Translation;

    /// A pointer to the \b Scale field.
    Vector3 * Scale;

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
  Transform4( const Matrix4& mat );

  /// Destructor.
  virtual ~Transform4( );

  virtual bool isValid() const;

  virtual Matrix4 getMatrix( ) const;

  virtual Matrix4& getMatrix( );

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;
  virtual Point4ArrayPtr transform( const Point4ArrayPtr& points ) const;
  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;
  virtual Point4MatrixPtr transform( const Point4MatrixPtr& points ) const;

  /// Add a translation at the transformation
  Transform4& translate( const Vector3& t );
  /// Add a scaling at the transformation
  Transform4& scale( const Vector3& s );
  /// Add a rotation at the transformation
  Transform4& rotate( const Matrix3& m );

  void getTransformation( Vector3& scale,
                          Vector3& rotate, // euler rotation
                          Vector3& translate );

  /// compute the Matrix3 determinant of the transformation
  real_t getVolume() const;

  private:

  Matrix4 __matrix;

};

/// Transform4 Pointer
typedef RCPtr<Transform4> Transform4Ptr;

/* ----------------------------------------------------------------------- */


/**
    \class MatrixTransformed
    \brief A matrix transformed object
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
  virtual Matrix3 getMatrix( ) const = 0;

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
  GeneralMatrix3Transformation( const Matrix3& mat );

  /// Destructor.
  virtual ~GeneralMatrix3Transformation( );

  virtual bool isValid() const;

  virtual Matrix3 getMatrix( ) const;

  virtual Point2ArrayPtr transform( const Point2ArrayPtr& points ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point2MatrixPtr transform( const Point2MatrixPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

private:

  Matrix3 __matrix;

};

/// GeneralMatrix3Transformation Pointer
typedef RCPtr<GeneralMatrix3Transformation> GeneralMatrix3TransformationPtr;


/* ----------------------------------------------------------------------- */


// __geom_matrixtransformed_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

