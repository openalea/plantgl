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



/*! \file geom_scaled.h
    \brief Definition of the transformation class Scaling, Scaling2D
    and the transformed class Scaled.
*/


#ifndef __geom_scaled_h__
#define __geom_scaled_h__

/* ----------------------------------------------------------------------- */

#include "mattransformed.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class Scaling
    \brief An anisotropic scaling.
*/

/* ----------------------------------------------------------------------- */

class SG_API Scaling : public Matrix4Transformation
{

public:

  /** Constructs a Scaling with the scaling factors \e factors.
      \pre
      - \e factors must be non null and each coordinates must be different
      from 0.
      \post
      - \e self is valid. */
  Scaling( const Vector3& factors );

  /// Destructor.
  virtual ~Scaling( );

  /// Return Matrix4(__factors.x(),0,0,0, 0,__factors.y(),0,0, 0,0,__factors.z(),0, 0,0,0,1);
  virtual Matrix4 getMatrix( ) const ;

  /// Returns the scaling factors of \e self.
  const Vector3& getFactors( ) const ;

  virtual bool isValid( ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point4ArrayPtr transform( const Point4ArrayPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

  virtual Point4MatrixPtr transform( const Point4MatrixPtr& points ) const;

protected:

  /// The scaling factors.
  Vector3 __factors;

};

/// Scaling Pointer
typedef RCPtr<Scaling> ScalingPtr;

/* ----------------------------------------------------------------------- */

/**
    \class Scaled
    \brief A geometry object anisotropically scaled  by an arbitrary vector
    representing the scaling factors along the \e x-axis, \e y-axis and
    \e z-axis.
*/

/* ----------------------------------------------------------------------- */

class SG_API Scaled : public MatrixTransformed
{

public:

  /// The Scale field default value
  static const Vector3 DEFAULT_SCALE;


  /// A structure which helps to build a Translated when parsing.
  struct SG_API Builder : public MatrixTransformed::Builder {

    /// A pointer to the \b Scale field.
    Vector3 * Scale;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  Scaled();


  /** Constructs a Scaled object with the scaling factors \e scale and
      the Geometry object \e geometry.
      \warning
      - \e scale must be valid and each components different from 0,
      - \e geometry must be non null and valid. */
  explicit Scaled( const Vector3& scale,
                   const GeometryPtr& geometry );

  /// Destructor
  virtual ~Scaled( ) ;

  PGL_OBJECT(Scaled)

  /// Returns Scale value.
  const Vector3& getScale( ) const ;

  /// Returns Scale field.
  Vector3& getScale( );

  virtual Transformation3DPtr getTransformation( ) const;

  /// Returns whether Scale is set to its default value.
  bool isScaleToDefault( ) const;

  virtual bool isValid( ) const;

protected:

  /// The Scale field.
  Vector3 __scale;

};

/* ----------------------------------------------------------------------- */

/// Scaled Pointer
typedef RCPtr<Scaled> ScaledPtr;

/* ----------------------------------------------------------------------- */

/**
    \class Scaling2D
    \brief An anisotropic 2D scaling.
*/

/* ----------------------------------------------------------------------- */

class SG_API Scaling2D : public Matrix3Transformation
{

public:

  /** Constructs a Scaling with the scaling factors \e factors.
      \pre
      - \e factors must be non null and each coordinates must be different
      from 0.
      \post
      - \e self is valid. */
  Scaling2D( const Vector2& factors );

  /// Destructor.
  virtual ~Scaling2D( );

  /// Return Matrix3(__factors.x(),0,0, 0,__factors.y(),0, 0,0,1);
  virtual Matrix3 getMatrix( ) const ;

  /// Returns the scaling factors of \e self.
  const Vector2& getFactors( ) const;

  virtual bool isValid( ) const;

  virtual Point2ArrayPtr transform( const Point2ArrayPtr& points ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point2MatrixPtr transform( const Point2MatrixPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

protected:

  /// The scaling factors.
  Vector2 __factors;

};

/// Scaling2D Pointer
typedef RCPtr<Scaling2D> Scaling2DPtr;

/* ----------------------------------------------------------------------- */

// __geom_scaled_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

