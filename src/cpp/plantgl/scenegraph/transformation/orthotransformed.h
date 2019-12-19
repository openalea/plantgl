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

