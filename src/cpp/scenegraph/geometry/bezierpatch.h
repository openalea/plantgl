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



/*! \file geom_bezierpatch.h
    \brief Definition of the geometry class BezierPatch.
*/

#ifndef __geom_bezierpatch_h__
#define __geom_bezierpatch_h__

/* ----------------------------------------------------------------------- */

#include "patch.h"
#include <math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Point4Matrix;
typedef RCPtr<Point4Matrix> Point4MatrixPtr;

/* ----------------------------------------------------------------------- */

/**
   \class BezierPatch
   \brief A Bezier Patch represented by 2 stride values  and a matrix of control Points.
*/

class SG_API BezierPatch : public Patch {

public:


  /// The \b Stride field default value.
  static const uint32_t DEFAULT_STRIDE;

  /// A structure which helps to build a BezierPatch when parsing. 
  struct SG_API Builder : public Patch::Builder { 

    /// A pointer to the \b CtrlPointList field.
    Point4MatrixPtr * CtrlPointMatrix;

    /// A pointer to the \b UDegree field.
    uint32_t * UDegree;

    /// A pointer to the \b UStride field.
    uint32_t * UStride;

    /// A pointer to the \b VDegree field.
    uint32_t * VDegree;

    /// A pointer to the \b VStride field.
    uint32_t * VStride;

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
	       uint32_t ustride = DEFAULT_STRIDE,
	       uint32_t vstride = DEFAULT_STRIDE,
	       bool ccw = DEFAULT_CCW );

  /// Destructor
  virtual ~BezierPatch( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \e Control Points value.
  virtual  Point4MatrixPtr getCtrlPoints( ) const;

  /// Returns \e CtrlPointMatrix value.
  const Point4MatrixPtr& getCtrlPointMatrix( ) const;

  /// Returns \e CtrlPointMatrix field.
  Point4MatrixPtr& getCtrlPointMatrix( );

  /// Returns \e UStride value.
  const uint32_t& getUStride( ) const ;

  /// Returns \e UStride field.
  uint32_t& getUStride( );

  /// Returns \e VStride value.
  const uint32_t& getVStride( ) const;

  /// Returns \e VStride field.
  uint32_t& getVStride( ) ;
  
  /// Returns whether \b UStride is set to its default value.
  bool isUStrideToDefault( ) const;

  /// Returns whether \b VStride is set to its default value.
  bool isVStrideToDefault( ) const;

  /// Returns \e UDegree value.
  virtual const uint32_t getUDegree( ) const;

  /// Returns \e VDegree value.
  virtual const uint32_t getVDegree( ) const;

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

protected:

  /// The \b CtrlPointMatrix field.
  Point4MatrixPtr __ctrlPointMatrix;

  /// The ustride field
  uint32_t __ustride;

  /// The vstride field
  uint32_t __vstride;

}; // BezierPatch

/// BezierPatch Pointer
typedef RCPtr<BezierPatch> BezierPatchPtr;


/* ----------------------------------------------------------------------- */

// __geom_bezierpatch_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

