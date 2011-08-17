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


/*! \file geom_bezierpatch.h
    \brief Definition of the geometry class BezierPatch.
*/

#ifndef __geom_bezierpatch_h__
#define __geom_bezierpatch_h__

/* ----------------------------------------------------------------------- */

#include "patch.h"
#include <plantgl/math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Point4Matrix;
typedef RCPtr<Point4Matrix> Point4MatrixPtr;
class Point3Matrix;
typedef RCPtr<Point3Matrix> Point3MatrixPtr;
class LineicModel;
typedef RCPtr<LineicModel> LineicModelPtr;

/* ----------------------------------------------------------------------- */

/**
   \class BezierPatch
   \brief A Bezier Patch represented by 2 degrees and a matrix of control Points.
*/

class SG_API BezierPatch : public Patch {

public:


  /// The \b Stride field default value.
  static const uint_t DEFAULT_STRIDE;

  /// A structure which helps to build a BezierPatch when parsing. 
  struct SG_API Builder : public Patch::Builder { 

    /// A pointer to the \b CtrlPointList field.
    Point4MatrixPtr * CtrlPointMatrix;

    /// A pointer to the \b UDegree field.
    uint_t * UDegree;

    /// A pointer to the \b UStride field.
    uint_t * UStride;

    /// A pointer to the \b VDegree field.
    uint_t * VDegree;

    /// A pointer to the \b VStride field.
    uint_t * VStride;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  BezierPatch();

  /** Constructs a BezierPatch  with the control points matrix \e ctrlPoints.
      \pre
      - \e points must contain at least 2x2 points;
      - \e ustride and \e vstride must be superior to 2
      \post
      - \e self is valid. */
  BezierPatch( const Point4MatrixPtr& ctrlPoints, 
	       uint_t ustride = DEFAULT_STRIDE,
	       uint_t vstride = DEFAULT_STRIDE,
	       bool ccw = DEFAULT_CCW );

  BezierPatch( const Point3MatrixPtr& ctrlPoints, 
	       uint_t ustride = DEFAULT_STRIDE,
	       uint_t vstride = DEFAULT_STRIDE,
	       bool ccw = DEFAULT_CCW );

  /// Destructor
  virtual ~BezierPatch( );

  PGL_OBJECT(BezierPatch)

  /// Returns \e Control Points value.
  virtual  Point4MatrixPtr getCtrlPoints( ) const;

  /// Returns \e CtrlPointMatrix value.
  const Point4MatrixPtr& getCtrlPointMatrix( ) const;

  /// Returns \e CtrlPointMatrix field.
  Point4MatrixPtr& getCtrlPointMatrix( );

  /// Returns \e UStride value.
  const uint_t& getUStride( ) const ;

  /// Returns \e UStride field.
  uint_t& getUStride( );

  /// Returns \e VStride value.
  const uint_t& getVStride( ) const;

  /// Returns \e VStride field.
  uint_t& getVStride( ) ;
  
  /// Returns whether \b UStride is set to its default value.
  bool isUStrideToDefault( ) const;

  /// Returns whether \b VStride is set to its default value.
  bool isVStrideToDefault( ) const;

  /// Returns \e UDegree value.
  virtual const uint_t getUDegree( ) const;

  /// Returns \e VDegree value.
  virtual const uint_t getVDegree( ) const;

  /*! Returns the \e Point for u = \e u and  v = \e v.
      using deCasteljau algorithm (see the Nurbs book A1.7 p.39) 
     \pre 
      - \e u must be in [0,1];
      - \e v must be in [0,1];*/
  virtual TOOLS(Vector3) getPointAt(real_t u,real_t v) const;

  /* Returns the \e Point for u = \e u.
      using classical algorithm (see the Nurbs book p.22) 
     \pre 
      - \e u must be in [0,1];*/
  //virtual Vector3 getPointAt2(real_t u) const;

  /* Returns the \e Tangent for u = \e u.
      (see the Nurbs book p.22) 
     \pre 
      - \e u must be in [0,1];*/
  //virtual Vector3 getTangentAt(real_t u) const;

  virtual bool isValid( ) const;
  /*!
    Compute a section line of the patch corresponding to a constant u value
   */
  virtual LineicModelPtr getIsoUSectionAt(real_t u) const;

  /*!
    Compute a section line of the patch corresponding to a constant v value
   */
  virtual LineicModelPtr getIsoVSectionAt(real_t v) const;


protected:

  /// The \b CtrlPointMatrix field.
  Point4MatrixPtr __ctrlPointMatrix;

  /// The ustride field
  uint_t __ustride;

  /// The vstride field
  uint_t __vstride;

}; // BezierPatch

/// BezierPatch Pointer
typedef RCPtr<BezierPatch> BezierPatchPtr;


/* ----------------------------------------------------------------------- */

// __geom_bezierpatch_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

