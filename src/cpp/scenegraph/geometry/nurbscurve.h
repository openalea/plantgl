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

/*! \file geom_nurbscurve.h
    \brief Definition of the geometry class NurbsCurve and NurbsCurve2D.
*/


#ifndef __geom_nurbscurve_h__
#define __geom_nurbscurve_h__

/* ----------------------------------------------------------------------- */

#include "beziercurve.h"

/* ----------------------------------------------------------------------- */


TOOLS_BEGIN_NAMESPACE

class RealArray;
typedef RCPtr<RealArray> RealArrayPtr;
class RealArray2;
typedef RCPtr<RealArray2> RealArray2Ptr;

TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class NurbsCurve
   \brief A NURBS Curve represented by an array of control Points, a knots 
   list and a degree.
*/

/* ----------------------------------------------------------------------- */

class SG_API NurbsCurve : public BezierCurve
{

public:

  /// The \b Degree field default value.
  static const uint32_t DEFAULT_NURBS_DEGREE;


  /// A structure which helps to build a NurbsCurve when parsing. 
  struct Builder : public BezierCurve::Builder { 

    /// A pointer to the \b KnotsList field.
    TOOLS(RealArrayPtr) * KnotList;


    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  NurbsCurve();

  /** Constructs a NurbsCurve of degree \e degree with the points \e points and the knot vector knotsList.
      \pre
      - \e degree must be strictly positive;
      - \e points must contain at least 2 points;
      - the number of \e knots must be equal to \e degree + 1 + number of control points
      \post
      - \e self is valid. */
  NurbsCurve(  const Point4ArrayPtr& ctrlPoints, 
	       const TOOLS(RealArrayPtr) knots = TOOLS(RealArrayPtr(0)), 
	       uint32_t degree = DEFAULT_NURBS_DEGREE, 
	       uint32_t stride = DEFAULT_STRIDE );

  /// Destructor
  virtual ~NurbsCurve( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \e Degree value.
  virtual uint32_t getDegree( ) const;

  /// Returns \e Degree field.
  uint32_t& getDegree( );

  /// Returns whether \b Degree is set to its default value.
  bool isDegreeToDefault( ) const;

  /// Returns \e knotsList value.
  virtual TOOLS(RealArrayPtr) getKnotList( ) const ;

  /// Returns \e knotsList field.
  TOOLS(RealArrayPtr)& getKnotList( );

  virtual const real_t getFirstKnot( ) const ;

  virtual const real_t getLastKnot( ) const;

  /** Set \e knotsList field to a default computed value.
      \pre
      - \e degree must be fill;
      - \e points must be fill and must contain at least 2 points;
      \post
      - \e self is valid. */
   virtual bool setKnotListToDefault( );

  /// Returns whether \b KnotList is set to its default value.
  bool isKnotListToDefault( ) const;

  virtual bool isValid( ) const;

  /*! 
     Compute point on the NURBS for u = \b u
     From the NURBS Book A4.1 p 124.
  */
  virtual TOOLS(Vector3) getPointAt(real_t u) const;

  /* Returns the \e Tangent for u = \e u.
      (see the Nurbs book p.12) 
     \pre 
      - \e u must be in [0,1];*/
  virtual TOOLS(Vector3) getTangentAt(real_t u) const;

  /* Returns the principal \e Normal for u = \e u.
     \pre 
      - \e u must be in [0,1];*/
  virtual TOOLS(Vector3) getNormalAt(real_t u) const;

  /*!
    \brief Computes the derivative of degree \a d of the 
    curve at parameter \a u in the homonegeous domain
    \author Philippe Lavoie     
  */
  Point4ArrayPtr deriveAtH(real_t u, int d, int span ) const;
  
  /*!
    \brief Computes the derivative of the curve at the parameter \a u
    \author Philippe Lavoie     
  */  
  Point4ArrayPtr deriveAt(real_t  u, int d, int span  ) const;
  
  /*! Returns the \e derivative of degree \e d for u = \e u.
     \pre 
     - \e u must be in [0,1];*/
  virtual TOOLS(Vector4) getDerivativeAt(real_t u, int d) const;


  TOOLS(Vector3) projectTo(const TOOLS(Vector3)& p, 
					real_t guess, real_t& u,
					real_t e1=0.001, real_t e2=0.001,int maxTry=100) const;


  /// @name  Computational Algorithms 
  //@{


  /*! Determine the knot Span index.
    From the Nurbs Book : A2.1 p68
  */
  uint32_t findSpan(real_t u) const ;  

  /*! \brief Compute the Basis Functions Values 
    Algo 2.2 From The Nurbs Book p70
  */
  TOOLS(RealArrayPtr) computeBasisFunctions(uint32_t span, real_t u) const ;

  /*!
    \brief Compute the Derivates Basis Functions Values 
    Algo A2.3 p72 Nurbs Book 
    \author Philippe Lavoie     
  */
  TOOLS(RealArray2Ptr) computeDerivatesBasisFunctions(int n, real_t u, int span) const ;

  //@}

protected:

  /// The \b Degree field.
  uint32_t __degree;

  /// The \b knotsList field.
  TOOLS(RealArrayPtr) __knotList;


}; // NurbsCurve

/// NurbsCurve Pointer
typedef RCPtr<NurbsCurve> NurbsCurvePtr;


/* ----------------------------------------------------------------------- */


/**
   \class NurbsCurve2D
   \brief A 2D NURBS Curve represented by an array of control Points, a knots 
   list and a degree.
*/


class SG_API NurbsCurve2D : public BezierCurve2D
{

public:

  /// A structure which helps to build a NurbsCurve2D when parsing. 
  struct Builder : public BezierCurve2D::Builder { 

    /// A pointer to the \b KnotsList field.
    TOOLS(RealArrayPtr) * KnotList;


    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  NurbsCurve2D();
  
  /** Constructs a NurbsCurve2D of degree \e degree with the points \e points and the knot vector knotsList.
      \pre
      - \e degree must be strictly positive;
      - \e points must contain at least 2 points;
      - the number of \e knots must be equal to \e degree + 1 + number of control points
      \post
      - \e self is valid. */
  NurbsCurve2D(  const Point3ArrayPtr& ctrlPoints, 
		 const TOOLS(RealArrayPtr) knots = TOOLS(RealArrayPtr(0)), 
		 uint32_t degree = NurbsCurve::DEFAULT_NURBS_DEGREE, 
		 uint32_t stride = BezierCurve::DEFAULT_STRIDE );

  /// Destructor
  virtual ~NurbsCurve2D( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \e Degree value.
  virtual uint32_t getDegree( ) const;

  /// Returns \e Degree field.
  uint32_t& getDegree( );

  /// Returns whether \b Degree is set to its default value.
  bool isDegreeToDefault( ) const;

  /// Returns \e knotsList value.
  virtual const TOOLS(RealArrayPtr)& getKnotList( ) const;

  /// Returns \e knotsList field.
  TOOLS(RealArrayPtr)& getKnotList( );


  virtual const real_t getFirstKnot( ) const;

  virtual const real_t getLastKnot( ) const;

  /** Set \e knotsList field to a default computed value.
      \pre
      - \e degree must be fill;
      - \e points must be fill and must contain at least 2 points;
      \post
      - \e self is valid. */
  virtual bool setKnotListToDefault( );

  /// Returns whether \b KnotList is set to its default value.
  bool isKnotListToDefault( ) const;

  virtual bool isValid( ) const;

  /*! Determine the knot Span index.
    From the Nurbs Book : A2.1 p68
  */
  uint32_t findSpan(real_t u) const ;  

  /*! 
     Compute point on the NURBS for u = \b u
     From the NURBS Book A4.1 p 124.
  */
  virtual TOOLS(Vector2) getPointAt(real_t u) const;

protected:

  /// The \b Degree field.
  uint32_t __degree;

  /// The \b knotsList field.
  TOOLS(RealArrayPtr) __knotList;


}; // NurbsCurve

/// NurbsCurve2D Pointer
typedef RCPtr<NurbsCurve2D> NurbsCurve2DPtr;

  /*! Determine the knot Span index.
    From the Nurbs Book : A2.1 p68
  */
uint32_t SG_API findSpan(real_t u,  uint32_t _degree, 
		const TOOLS(RealArrayPtr)& _knotList);  

/*! \brief Compute the Basis Functions Values 
  Algo 2.2 From The Nurbs Book p70
*/
TOOLS(RealArrayPtr) SG_API basisFunctions(uint32_t span, real_t u,  
				   uint32_t _degree, 
				   const TOOLS(RealArrayPtr)& _knotList );

/*!
  \brief Compute the Derivates Basis Functions Values 
  Algo A2.3 p72 Nurbs Book 
  \author Philippe Lavoie     
*/
TOOLS(RealArray2Ptr) SG_API derivatesBasisFunctions(int n, real_t u, 
				      int span, 
				      uint32_t _degree, 
				      const TOOLS(RealArrayPtr)& _knotList );

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __geom_nurbscurve_h__
#endif

