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



/*! \file geom_beziercurve.h
    \brief Definition of the geometry class BezierCurve and BezierCurve2D.
*/

#ifndef __geom_beziercurve_h__
#define __geom_beziercurve_h__

/* ----------------------------------------------------------------------- */

#include "curve.h"
#include "lineicmodel.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class  Point4Array;
typedef RCPtr<Point4Array> Point4ArrayPtr;
class Point3Array;
typedef RCPtr<Point3Array> Point3ArrayPtr;

/* ----------------------------------------------------------------------- */

/**
   \class BezierCurve
   \brief A Bezier Curve represented by a degree and a list of control Points.
*/


class SG_API BezierCurve : public  ParametricModel, public LineicModel
{

public:


  /// The \b Stride field default value.
  static const uint32_t DEFAULT_STRIDE;

  /// A structure which helps to build a BezierCurve when parsing. 
  struct SG_API Builder : public ParametricModel::Builder { 

    /// A pointer to the \b CtrlPointList field.
    Point4ArrayPtr * CtrlPointList;

    /// A pointer to the \b Degree field.
    uint32_t * Degree;

    /// A pointer to the \b Stride field.
    uint32_t * Stride;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  BezierCurve();

  /** Constructs a BezierCurve of degree \e degree with the points \e points.
      \pre
      - \e degree must be strictly positive;
      - \e points must contain at least 2 points;
      - the number of \e control points must be equal to \e degree + 1.
      \post
      - \e self is valid. */
  BezierCurve( const Point4ArrayPtr& ctrlPoints ,uint32_t stride = DEFAULT_STRIDE );

  /// Destructor
  virtual ~BezierCurve( ); 

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \e Control Points value.
  virtual Point4ArrayPtr getCtrlPoints( ) const;

  /// Returns \e CtrlPointList value.
  const Point4ArrayPtr& getCtrlPointList( ) const ;

  /// Returns \e CtrlPointList field.
  Point4ArrayPtr& getCtrlPointList( );

  virtual const real_t getFirstKnot() const;

  virtual const real_t getLastKnot() const;

  virtual const uint32_t getStride( ) const;

  /// Returns \e Stride field.
  uint32_t& getStride( );
  
  /// Returns whether \b Stride is set to its default value.
  virtual bool isStrideToDefault( ) const;

  /// Returns \e Degree value.
  virtual uint32_t getDegree( ) const ;

  /** Returns the \e Point for u = \e u.
      using deCasteljau algorithm (see the Nurbs book p.24) 
     \pre 
      - \e u must be in [0,1];*/
  virtual TOOLS(Vector3) getPointAt(real_t u) const;

  /** Returns the \e Point for u = \e u.
      using classical algorithm (see the Nurbs book p.22) 
     \pre 
      - \e u must be in [0,1];*/
  virtual TOOLS(Vector3) getPointAt2(real_t u) const;

  /** Returns the \e Tangent for u = \e u.
      (see the Nurbs book p.22) 
     \pre 
      - \e u must be in [0,1];*/
  virtual TOOLS(Vector3) getTangentAt(real_t u) const;

  /** Returns the principal \e Normal for u = \e u.
     \pre 
      - \e u must be in [0,1];*/
  virtual TOOLS(Vector3) getNormalAt(real_t u) const;

  virtual bool isValid( ) const;

protected:

  /// The \b CtrlPointList field.
  Point4ArrayPtr __ctrlPointList;

  /// The stride field
  uint32_t __stride;

}; // BezierCurve

/// BezierCurve Pointer
SG_API typedef RCPtr<BezierCurve> BezierCurvePtr;


/* ----------------------------------------------------------------------- */


/**
   \class BezierCurve2D
   \brief A 2D Bezier Curve represented by a degree and a list of control Points.
*/


class SG_API BezierCurve2D : public Curve2D
{

public:


  /// A structure which helps to build a BezierCurve when parsing. 
  struct SG_API Builder : public Curve2D::Builder { 

    /// A pointer to the \b CtrlPointList field.
    Point3ArrayPtr * CtrlPointList;

    /// A pointer to the \b Degree field.
    uint32_t * Degree;

    /// A pointer to the \b Stride field.
    uint32_t * Stride;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  BezierCurve2D();

  /** Constructs a BezierCurve2D of degree \e degree with the points \e points.
      \pre
      - \e degree must be strictly positive;
      - \e points must contain at least 2 points;
      - the number of \e control points must be equal to \e degree + 1.
      \post
      - \e self is valid. */
  BezierCurve2D( const Point3ArrayPtr& ctrlPoints ,uint32_t stride = BezierCurve::DEFAULT_STRIDE );

  /// Destructor
  virtual ~BezierCurve2D( ) ;

  virtual bool apply( Action& action ) ;

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \e Control Points value.
  virtual Point3ArrayPtr getCtrlPoints( ) const;

  /// Returns \e CtrlPointList value.
  const Point3ArrayPtr& getCtrlPointList( ) const;

  /// Returns \e CtrlPointList field.
  Point3ArrayPtr& getCtrlPointList( ) ;

  virtual const real_t getFirstKnot() const ;

  virtual const real_t getLastKnot() const ;

  virtual const uint32_t getStride( ) const ;
  
  /// Return Stride field.
  uint32_t& getStride( ) ;
  
  /// Returns whether \b Stride is set to its default value.
  bool isStrideToDefault( ) const;

  /// Returns \e Degree value.
  virtual uint32_t getDegree( ) const;

  /*! Returns the \e Point for u = \e u.
      using deCasteljau algorithm (see the Nurbs book p.24) 
     \pre 
      - \e u must be in [0,1];*/
  virtual TOOLS(Vector2) getPointAt(real_t u) const;

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

  /// The \b CtrlPointList field.
  Point3ArrayPtr __ctrlPointList;

  /// The stride field
  uint32_t __stride;

}; // BezierCurve2D

/// BezierCurve2D Pointer
typedef RCPtr<BezierCurve2D> BezierCurve2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_beziercurve_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

