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
class Point2Array;
typedef RCPtr<Point2Array> Point2ArrayPtr;

/* ----------------------------------------------------------------------- */

/**
   \class BezierCurve
   \brief A Bezier Curve represented by a degree and a list of control Points.
*/


class SG_API BezierCurve : public  ParametricModel, public LineicModel
{

public:


  /// The \b Stride field default value.
  static const uint_t DEFAULT_STRIDE;

  /// A structure which helps to build a BezierCurve when parsing.
  struct SG_API Builder : public ParametricModel::Builder {

    /// A pointer to the \b CtrlPointList field.
    Point4ArrayPtr * CtrlPointList;

    /// A pointer to the \b Degree field.
    uint_t * Degree;

    /// A pointer to the \b Stride field.
    uint_t * Stride;

    /// A pointer to the \b Width field.
    uchar_t * Width;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );
    void BCdestroy( );

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
  BezierCurve( const Point4ArrayPtr& ctrlPoints, uint_t stride = DEFAULT_STRIDE, uchar_t width = DEFAULT_WIDTH);
  BezierCurve( const Point3ArrayPtr& ctrlPoints, uint_t stride = DEFAULT_STRIDE, uchar_t width = DEFAULT_WIDTH);

  /// Destructor
  virtual ~BezierCurve( );

  PGL_OBJECT(BezierCurve)

  /// Returns \e Control Points value.
  virtual Point4ArrayPtr getCtrlPoints( ) const;

  /// Returns \e CtrlPointList value.
  const Point4ArrayPtr& getCtrlPointList( ) const ;

  /// Returns \e CtrlPointList field.
  Point4ArrayPtr& getCtrlPointList( );

  virtual const real_t getFirstKnot() const;

  virtual const real_t getLastKnot() const;

  virtual const uint_t getStride( ) const;

  /// Returns \e Stride field.
  uint_t& getStride( );

  /// Returns whether \b Stride is set to its default value.
  virtual bool isStrideToDefault( ) const;

  /// Returns \e Degree value.
  virtual const uint_t getDegree( ) const ;

  /** Returns the \e Point for u = \e u.
      using deCasteljau algorithm (see the Nurbs book p.24)
     \pre
      - \e u must be in [0,1];*/
  virtual Vector3 getPointAt(real_t u) const;

  /** Returns the \e Point for u = \e u.
      using classical algorithm (see the Nurbs book p.22)
     \pre
      - \e u must be in [0,1];*/
  virtual Vector3 getPointAt2(real_t u) const;

  /** Returns the \e Tangent for u = \e u.
      (see the Nurbs book p.22)
     \pre
      - \e u must be in [0,1];*/
  virtual Vector3 getTangentAt(real_t u) const;

  /** Returns the principal \e Normal for u = \e u.
     \pre
      - \e u must be in [0,1];*/
  virtual Vector3 getNormalAt(real_t u) const;

  virtual bool isValid( ) const;

protected:

  /// The \b CtrlPointList field.
  Point4ArrayPtr __ctrlPointList;

  /// The stride field
  uint_t __stride;

}; // BezierCurve

/// BezierCurve Pointer
typedef RCPtr<BezierCurve> BezierCurvePtr;


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
    uint_t * Degree;

    /// A pointer to the \b Stride field.
    uint_t * Stride;

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
  BezierCurve2D( const Point3ArrayPtr& ctrlPoints, uint_t stride = BezierCurve::DEFAULT_STRIDE, uchar_t width = DEFAULT_WIDTH);
  BezierCurve2D( const Point2ArrayPtr& ctrlPoints, uint_t stride = BezierCurve::DEFAULT_STRIDE, uchar_t width = DEFAULT_WIDTH);

  /// Destructor
  virtual ~BezierCurve2D( ) ;

  PGL_OBJECT(BezierCurve2D)

  /// Returns \e Control Points value.
  virtual Point3ArrayPtr getCtrlPoints( ) const;

  /// Returns \e CtrlPointList value.
  const Point3ArrayPtr& getCtrlPointList( ) const;

  /// Returns \e CtrlPointList field.
  Point3ArrayPtr& getCtrlPointList( ) ;

  virtual const real_t getFirstKnot() const ;

  virtual const real_t getLastKnot() const ;

  virtual const uint_t getStride( ) const ;

  /// Return Stride field.
  uint_t& getStride( ) ;

  /// Returns whether \b Stride is set to its default value.
  bool isStrideToDefault( ) const;

  /// Returns \e Degree value.
  virtual const uint_t getDegree( ) const;

  /*! Returns the \e Point for u = \e u.
      using deCasteljau algorithm (see the Nurbs book p.24)
     \pre
      - \e u must be in [0,1];*/
  virtual Vector2 getPointAt(real_t u) const;

  /* Returns the \e Point for u = \e u.
      using classical algorithm (see the Nurbs book p.22)
     \pre
      - \e u must be in [0,1];*/
  //virtual Vector3 getPointAt2(real_t u) const;

  /* Returns the \e Tangent for u = \e u.
      (see the Nurbs book p.22)
     \pre
      - \e u must be in [0,1];*/
  virtual Vector2 getTangentAt(real_t u) const;

  /** Returns the principal \e Normal for u = \e u.
     \pre
      - \e u must be in [0,1];*/
  virtual Vector2 getNormalAt(real_t u) const;

  virtual bool isValid( ) const;

protected:

  /// The \b CtrlPointList field.
  Point3ArrayPtr __ctrlPointList;

  /// The stride field
  uint_t __stride;

}; // BezierCurve2D

/// BezierCurve2D Pointer
typedef RCPtr<BezierCurve2D> BezierCurve2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_beziercurve_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

