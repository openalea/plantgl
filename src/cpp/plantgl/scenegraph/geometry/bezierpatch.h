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
  virtual Vector3 getPointAt(real_t u,real_t v) const;

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

