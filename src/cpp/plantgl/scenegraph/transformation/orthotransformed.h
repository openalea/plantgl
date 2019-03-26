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


/*! \file geom_orthotransformed.h
    \brief Definition of the transformation class OrthonormalBasis3D, OrthonormalBasis2D
    and the transformed class OrthoTransformed.
*/


#ifndef __geom_orthotransformed_h__
#define __geom_orthotransformed_h__

/* ----------------------------------------------------------------------- */

#include "mattransformed.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

// forward declaration
class OrthonormalBasis3D;
typedef RCPtr<OrthonormalBasis3D> OrthonormalBasis3DPtr;

/* ----------------------------------------------------------------------- */

/**
    \class OrthonormalBasis3D
    \brief A Transformation represented by an orthogonal 3x3 matrix.
*/

/* ----------------------------------------------------------------------- */

class SG_API OrthonormalBasis3D : public Matrix4Transformation
{

public:

  /// A structure which helps to build an OrthonormalBasis3D when parsing.
  struct SG_API Builder
    {
    /// Constructor.
      Builder( );

    /// Desructor.
      virtual ~Builder( );

    virtual OrthonormalBasis3DPtr build( ) const = 0;

    virtual void destroy( ) = 0;

    virtual bool isValid( ) const = 0;
    };

  /** Constructs a OrthonormalBasis.
      \pre
      - \e matrix must non null and orthogonal.
      \post
      - \e self is valid. */
  OrthonormalBasis3D( const Matrix3& matrix );

  /// Destructor.
  virtual ~OrthonormalBasis3D( );

  virtual Matrix3 getMatrix3( ) const ;

  virtual Matrix4 getMatrix( ) const ;

  /// extract Euler Angles from \e this
  virtual Vector3 extractEulerAngles() const;

  virtual bool isValid( ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point4ArrayPtr transform( const Point4ArrayPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

  virtual Point4MatrixPtr transform( const Point4MatrixPtr& points ) const;

protected:

  /// The orthonormal basis.
  Matrix3 __matrix;

};

/// OrthonormalBasis3D Pointer
typedef RCPtr<OrthonormalBasis3D> OrthonormalBasis3DPtr;

/* ----------------------------------------------------------------------- */


/**
    \class OrthoTransformed
    \brief Abstract base class for all OrthoTransformed 3D geometric objects.
*/


class SG_API OrthoTransformed : public MatrixTransformed
{

public:

  /// A structure which helps to build a OrthoTransformed when parsing.
  struct SG_API Builder : public MatrixTransformed::Builder {

    /// Constructor.
    Builder( );

    /// Desructor.
    virtual ~Builder( );

  };

  /// Default Constructor. Build object is invalid.
  OrthoTransformed();

  /// Constructor.
  explicit OrthoTransformed( const GeometryPtr& geometry );

  /// Destructor.
  virtual ~OrthoTransformed( );

  /// Get Orthonormal Basis Transformation.
  virtual OrthonormalBasis3DPtr getOrthonormalBasis() const = 0 ;

};

/// OrthoTransformed Pointer
typedef RCPtr<OrthoTransformed> OrthoTransformedPtr;


/* ----------------------------------------------------------------------- */


/**
    \class OrthonormalBasis2D
    \brief A Transformation represented by an orthogonal 2x2 matrix.
*/

class SG_API OrthonormalBasis2D : public Matrix3Transformation
{

public:

  /** Constructs a OrthonormalBasis2D.
      \pre
      - \e matrix must non null and orthogonal.
      \post
      - \e self is valid. */
  OrthonormalBasis2D( const Matrix2& matrix );

  /// Destructor.
  virtual ~OrthonormalBasis2D( );

  virtual Matrix3 getMatrix( ) const;

  virtual bool isValid( ) const;

  virtual Point2ArrayPtr transform( const Point2ArrayPtr& points ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point2MatrixPtr transform( const Point2MatrixPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

protected:

  /// The orthonormal basis.
  Matrix2 __matrix;

};

/// OrthonormalBasis2D Pointer
typedef RCPtr<OrthonormalBasis2D> OrthonormalBasis2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_orthotransformed_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

