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


/*! \file geom_nurbspatch.h
    \brief Definition of the geometry class NurbsPatch.
*/


#ifndef __geom_nurbspatch_h__
#define __geom_nurbspatch_h__

/* ----------------------------------------------------------------------- */

#include "bezierpatch.h"

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

class RealArray;
typedef RCPtr<RealArray> RealArrayPtr;

TOOLS_END_NAMESPACE

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
  static const uint32_t DEFAULT_NURBS_DEGREE;

  /// A structure which helps to build a NurbsPatch when parsing. 
  struct SG_API Builder : public BezierPatch::Builder { 

    /// A pointer to the \b UKnotList field.
    TOOLS(RealArrayPtr) * UKnotList;

    /// A pointer to the \b VKnotList field.
    TOOLS(RealArrayPtr) * VKnotList;


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
	      const TOOLS(RealArrayPtr)& uKnotList, 
	      const TOOLS(RealArrayPtr)& vKnotList, 
	      uint32_t uDegree = DEFAULT_NURBS_DEGREE, 
	      uint32_t vDegree = DEFAULT_NURBS_DEGREE, 
	      uint32_t ustride = DEFAULT_STRIDE, 
	      uint32_t vstride = DEFAULT_STRIDE, 
	      bool ccw = DEFAULT_CCW );

  /// Destructor
  virtual ~NurbsPatch( ) ;

  virtual bool apply( Action& action ) ;

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \e UDegree value.
  virtual const uint32_t getUDegree( ) const;

  /// Returns \e UDegree field.
  uint32_t& getUDegree( ) ;

  /// Returns \e VDegree value.
  virtual const uint32_t getVDegree( ) const ;

  /// Returns \e VDegree field.
  uint32_t& getVDegree( ) ;

  /// Returns  whether \e UDegree is set to its default value.
  virtual bool  isUDegreeToDefault( ) const;

  /// Returns  whether \e VDegree is set to its default value.
  virtual uint32_t isVDegreeToDefault( ) const ;

  /// Returns \e uknotsList value.
  virtual const TOOLS(RealArrayPtr)& getUKnotList( ) const;

  /// Returns \e uknotsList field.
  TOOLS(RealArrayPtr)& getUKnotList( );

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
  virtual const TOOLS(RealArrayPtr)& getVKnotList( ) const ;

  /// Returns \e knotsList field.
  TOOLS(RealArrayPtr)& getVKnotList( );

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
  virtual TOOLS(Vector3) getPointAt(real_t u,real_t v) const;

  /* Computational Algorithms */

  /*!
    Determine the knot Span index.
    From the Nurbs Book : A2.1 p68
  */
  uint32_t findSpan(uint32_t deg, const TOOLS(RealArrayPtr)& U,  real_t u) const;  

  /*!
    Compute the Basis Functions Values 
    From the Nurbs Book : A2.1 p68
  */
  //RealArrayPtr computeBasisFunctions(uint32_t span, real_t u, uint32_t deg,const RealArrayPtr& U ) const;


  virtual bool isValid( ) const;

protected:

  /// The \b uKnotList field.
  TOOLS(RealArrayPtr) __uKnotList;

  /// The \b vKnotList field.
  TOOLS(RealArrayPtr) __vKnotList;

  /// The \b udegree field.
  uint32_t __udegree;

  /// The \b vdegree field.
  uint32_t __vdegree;

}; // NurbsPatch

/// NurbsPatch Pointer
typedef RCPtr<NurbsPatch> NurbsPatchPtr;


/* ----------------------------------------------------------------------- */

// __geom_nurbspatch_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

