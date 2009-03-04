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
  Scaling( const TOOLS(Vector3)& factors );

  /// Destructor.
  virtual ~Scaling( );

  /// Return Matrix4(__factors.x(),0,0,0, 0,__factors.y(),0,0, 0,0,__factors.z(),0, 0,0,0,1);
  virtual TOOLS(Matrix4) getMatrix( ) const ;

  /// Returns the scaling factors of \e self.
  const TOOLS(Vector3)& getFactors( ) const ;

  virtual bool isValid( ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point4ArrayPtr transform( const Point4ArrayPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

  virtual Point4MatrixPtr transform( const Point4MatrixPtr& points ) const;

protected:

  /// The scaling factors.
  TOOLS(Vector3) __factors;

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
  static const TOOLS(Vector3) DEFAULT_SCALE;


  /// A structure which helps to build a Translated when parsing.
  struct SG_API Builder : public MatrixTransformed::Builder {

    /// A pointer to the \b Scale field.
    TOOLS(Vector3) * Scale;

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
  explicit Scaled( const TOOLS(Vector3)& scale,
                   const GeometryPtr& geometry );

  /// Destructor
  virtual ~Scaled( ) ;

  PGL_OBJECT(Scaled)

  /// Returns Scale value.
  const TOOLS(Vector3)& getScale( ) const ;

  /// Returns Scale field.
  TOOLS(Vector3)& getScale( );

  virtual Transformation3DPtr getTransformation( ) const;

  /// Returns whether Scale is set to its default value.
  bool isScaleToDefault( ) const;

  virtual bool isValid( ) const;

protected:

  /// The Scale field.
  TOOLS(Vector3) __scale;

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
  Scaling2D( const TOOLS(Vector2)& factors );

  /// Destructor.
  virtual ~Scaling2D( );

  /// Return Matrix3(__factors.x(),0,0, 0,__factors.y(),0, 0,0,1);
  virtual TOOLS(Matrix3) getMatrix( ) const ;

  /// Returns the scaling factors of \e self.
  const TOOLS(Vector2)& getFactors( ) const;

  virtual bool isValid( ) const;

  virtual Point2ArrayPtr transform( const Point2ArrayPtr& points ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point2MatrixPtr transform( const Point2MatrixPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

protected:

  /// The scaling factors.
  TOOLS(Vector2) __factors;

};

/// Scaling2D Pointer
typedef RCPtr<Scaling2D> Scaling2DPtr;

/* ----------------------------------------------------------------------- */

// __geom_scaled_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

