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

/*! \file geom_nurbspatch.h
    \brief Definition of the geometry class NurbsPatch.
*/


#ifndef __geom_nurbspatch_h__
#define __geom_nurbspatch_h__

/* ----------------------------------------------------------------------- */

#include "bezierpatch.h"
#include "nurbscurve.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

class RealArray;
typedef RCPtr<RealArray> RealArrayPtr;
class RealArray2;
typedef RCPtr<RealArray2> RealArray2Ptr;

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE


/* ----------------------------------------------------------------------- */

/**
   \class NurbsPatch
   \brief A Nurbs Patch represented by 2 degrees, a matrix of control Points, 2 lists of knots and 2 stride values.
*/

class SG_API NurbsPatch : public BezierPatch {

public:

  /// The \b Degree field default value.
  static const uint_t DEFAULT_NURBS_DEGREE;

  /// A structure which helps to build a NurbsPatch when parsing. 
  struct SG_API Builder : public BezierPatch::Builder { 

    /// A pointer to the \b UKnotList field.
    RealArrayPtr * UKnotList;

    /// A pointer to the \b VKnotList field.
    RealArrayPtr * VKnotList;


    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  NurbsPatch();
  
  /** Constructs a NurbsPatch of degrees \e udegree and \e vdegree with the control points matrix \e ctrlPoints and the knots lists \e uKnotList and \e vKnotList.
      \pre
      - \e degrees must be strictly positive;
      - \e ctrlPoints must contain at least 2x2 points;
      - the number of \e uknot must be equal to \e udegree + 1 +  number of \e control points in a row.
      - the number of \e vknot must be equal to \e vdegree + 1 +  number of \e control points in a colum.
      \post
      - \e self is valid. */
  NurbsPatch( const Point4MatrixPtr& ctrlPoints, 
	      const RealArrayPtr& uKnotList = TOOLS(RealArrayPtr()), 
	      const RealArrayPtr& vKnotList = TOOLS(RealArrayPtr()), 
	      uint_t uDegree = DEFAULT_NURBS_DEGREE, 
	      uint_t vDegree = DEFAULT_NURBS_DEGREE, 
	      uint_t ustride = DEFAULT_STRIDE, 
	      uint_t vstride = DEFAULT_STRIDE, 
	      bool ccw = DEFAULT_CCW );

  NurbsPatch( const Point4MatrixPtr& ctrlPoints, 
	      uint_t uDegree, 
	      uint_t vDegree = DEFAULT_NURBS_DEGREE, 
	      const RealArrayPtr& uKnotList = TOOLS(RealArrayPtr()), 
	      const RealArrayPtr& vKnotList = TOOLS(RealArrayPtr()), 
	      uint_t ustride = DEFAULT_STRIDE, 
	      uint_t vstride = DEFAULT_STRIDE, 
	      bool ccw = DEFAULT_CCW );

  NurbsPatch( const Point3MatrixPtr& ctrlPoints, 
	      uint_t uDegree = DEFAULT_NURBS_DEGREE, 
	      uint_t vDegree = DEFAULT_NURBS_DEGREE, 
	      const RealArrayPtr& uKnotList = TOOLS(RealArrayPtr()), 
	      const RealArrayPtr& vKnotList = TOOLS(RealArrayPtr()), 
	      uint_t ustride = DEFAULT_STRIDE, 
	      uint_t vstride = DEFAULT_STRIDE, 
	      bool ccw = DEFAULT_CCW );

  /// Destructor
  virtual ~NurbsPatch( ) ;

  PGL_OBJECT(NurbsPatch)

  /// Returns \e UDegree value.
  virtual const uint_t getUDegree( ) const;

  /// Returns \e UDegree field.
  uint_t& getUDegree( ) ;

  /// Returns \e VDegree value.
  virtual const uint_t getVDegree( ) const ;

  /// Returns \e VDegree field.
  uint_t& getVDegree( ) ;

  /// Returns  whether \e UDegree is set to its default value.
  virtual bool  isUDegreeToDefault( ) const;

  /// Returns  whether \e VDegree is set to its default value.
  virtual uint_t isVDegreeToDefault( ) const ;

  /// Returns \e uknotsList value.
  virtual const RealArrayPtr& getUKnotList( ) const;

  /// Returns \e uknotsList field.
  RealArrayPtr& getUKnotList( );

  /// Returns the \e fist uknots value.
  virtual const real_t getFirstUKnot( ) const ;

  /// Returns the \e last uknots value.
  virtual const real_t getLastUKnot( ) const;

  /** Set \e UKnotsList field to a default computed value.
      \pre
      - \e Udegree must be fill;
      - \e points must be fill;
      \post
      - \e UKnotsList  is valid. */
  virtual bool setUKnotListToDefault( );

  /// Returns whether \e UKnotList is set to its default value.
  bool isUKnotListToDefault( ) const;

  /// Returns \e knotsList value.
  virtual const RealArrayPtr& getVKnotList( ) const ;

  /// Returns \e knotsList field.
  RealArrayPtr& getVKnotList( );

  /// Returns the \e fist knots value.
  virtual real_t getFirstVKnot( ) const ;

  /// Returns the \e last  knots value.
  virtual real_t getLastVKnot( ) const;

  /** Set \e VKnotsList field to a default computed value.
      \pre
      - \e Vdegree must be fill;
      - \e points must be fill;
      \post
      - \e VKnotsList  is valid. */
  virtual bool setVKnotListToDefault( );

  /// Returns whether \b VKnotList is set to its default value.
  bool isVKnotListToDefault( ) const;

  /*! Returns the \e Point for u = \e u and v= \e v.
      using the Nurbs book algorithm A4.3 p.134. 
     \pre 
      - \e u must be in [0,1];
      - \e v must be in [0,1];*/
  virtual Vector3 getPointAt(real_t u,real_t v) const;

  /* Returns the \e Metric for  u = \e u and v = \e v.
      (see Differential Geometry, Kreyszig p. 82)
     \author Michael Walker
     \pre 
      - \e u must be in [0,1];
      - \e v must be in [0,1];*/
  // Point4MatrixPtr getMetric(real_t u, real_t v) const;

  /* Returns the determinant of the metric for u = \e u and v = \e v.
      (see Differential Geometry, Kreyszig p. 82)
     \author Michael Walker
     \pre 
      - \e u must be in [0,1];
      - \e v must be in [0,1];*/
  // real_t getDetMetric(real_t u, real_t v) const;

  /* Returns the \e Inverse \e Metric for  u = \e u and v = \e v.
      (see Differential Geometry, Kreyszig p. 104)
     \author Michael Walker
     \pre 
      - \e u must be in [0,1];
      - \e v must be in [0,1];*/
  //Point4MatrixPtr getInvMetric(real_t u, real_t v) const;

  /* Returns the \e Connection \e Coefficients for  u = \e u and v = \e v.
      (see Differential Geometry, Kreyszig p. 140)
     \author Michael Walker
     \pre 
      - \e u must be in [0,1];
      - \e v must be in [0,1];*/
  //Point4MatrixPtr getConnectCoeffs(real_t  u, real_t  v, int d, int uspan, int vspan ) const;

  /* Returns the \e Tangent for u = \e u and v = \e v.
      (see the Nurbs book p.12) 
     \pre 
      - \e u, \e v must be in [0,1];*/
  virtual Vector3 getUTangentAt(real_t u,real_t v) const;

  /* Returns the \e Tangent for u = \e u and v = \e v.
      (see the Nurbs book p.12) 
     \pre 
      - \e u, \e v must be in [0,1];*/
  virtual Vector3 getVTangentAt(real_t u,real_t v) const;

  /* Returns the principal \e Normal for u,v = \e u, \e v.
     \pre 
      - \e u, \e v must be in [0,1];*/
  virtual Vector3 getNormalAt(real_t u,real_t v) const;

  /*!
    \brief Computes the derivative of degree \a d of the 
    curve at parameters \a u and \a v in the homogeneous domain
    \author Michael Walker
  */
  Point4MatrixPtr deriveAtH(real_t u, real_t v, int d, int uspan, int vspan) const;
  
  /*!
    \brief Computes the derivatives of the curve at the parameter \a u, \a v
    \author Michael Walker
  */  
  Point4MatrixPtr deriveAt(real_t  u, real_t v, int d, int uspan , int vspan  ) const;
  
  
  /*! Returns the \e derivative of degree \e d for u = \e u, v = \e v.
     \pre 
     - \e u, \e v must be in [0,1];*/
  virtual Vector4 getDerivativeAt(real_t u, real_t v, int du, int dv) const;

  /*! Returns the \e derivative of degree \e d for u = \e u, v = \e v.
     \pre 
     - \e u, \e v must be in [0,1];*/
  virtual Point4MatrixPtr getDerivativesAt(real_t u, real_t v) const;



  
  /*!
    Compute a section line of the patch corresponding to a constant u value
   */
  LineicModelPtr getIsoUSectionAt(real_t u) const;

  /*!
    Compute a section line of the patch corresponding to a constant v value
   */
  LineicModelPtr getIsoVSectionAt(real_t v) const;

  virtual bool isValid( ) const;

protected:

  /// The \b uKnotList field.
  RealArrayPtr __uKnotList;

  /// The \b vKnotList field.
  RealArrayPtr __vKnotList;

  /// The \b udegree field.
  uint_t __udegree;

  /// The \b vdegree field.
  uint_t __vdegree;

}; // NurbsPatch

/// NurbsPatch Pointer
typedef RCPtr<NurbsPatch> NurbsPatchPtr;


/* ----------------------------------------------------------------------- */

// __geom_nurbspatch_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

