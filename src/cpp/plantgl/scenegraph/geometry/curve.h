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




/*! \file geom_curve.h
    \brief Definition of the geometry class Curve2D.
*/

#ifndef __geom_curve_h__
#define __geom_curve_h__

/* ----------------------------------------------------------------------- */

#include "planarmodel.h"
#include <plantgl/math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/*
    \class Curve
    \brief Abstract base class for objects of type of 3D parametric curve.
*/

/*
class Curve : public ParametricModel, public LineicModel
{

public:

  /// A structure which helps to build an object of type of Curve.
  struct Builder : public ParametricModel::Builder {

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

  };


  /// Constructor.
  Curve( );

  /// Destructor
  virtual ~Curve( ) {
  }

  virtual bool isACurve( ) const {
    return true;
  }

  virtual bool isASurface( ) const {
    return false;
  }

  virtual bool isAVolume( ) const {
    return false;
  }

};

typedef RCPtr<Curve> CurvePtr;
*/

/* ----------------------------------------------------------------------- */

class QuantisedFunction;
typedef RCPtr<QuantisedFunction> QuantisedFunctionPtr;


/**
    \class Curve2D
    \brief Abstract base class for objects of type of 2D parametric curve.
*/


class SG_API Curve2D : public PlanarModel
{

public:
  static const uchar_t DEFAULT_WIDTH;

  /// A structure which helps to build an object of type of Curve.
  struct SG_API Builder : public PlanarModel::Builder {

    /// A pointer to the \b Width field.
    uchar_t * Width;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    void cdestroy( );
  };


  /// Constructor.
  Curve2D(uchar_t width = DEFAULT_WIDTH);

  /// Destructor
  virtual ~Curve2D();

  /// Returns the \e fist knots value.
  virtual const real_t getFirstKnot() const = 0;

  /// Returns the \e last  knots value.
  virtual const real_t getLastKnot() const = 0;

  /// Returns \e Stride value.
  virtual const uint_t getStride() const = 0;

  /*! Returns the \e Point for u = \e u.
     \pre
      - \e u must be in [FirstKnot,LastKnot];*/
  virtual Vector2 getPointAt(real_t u) const = 0;

  /*! Returns the \e Tangent for u = \e u.
    \pre
    - \e u must be in [FirstKnot,LastKnot];*/
  virtual Vector2 getTangentAt(real_t u) const = 0;


  /*! Returns the principal \e Normal for u = \e u.
    \pre
    - \e u must be in [FirstKnot,LastKnot];*/
  virtual Vector2 getNormalAt(real_t u) const = 0;

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  /// Return the length of the lineic model.
  inline real_t getLength() const { return getLength(getFirstKnot(),getLastKnot()); }

  /// Return the length of the lineic model starting from u = begin.
  inline real_t getLength(real_t begin) const { return getLength(begin,getLastKnot()); }

  /// Return the length of the lineic model from u = begin to u = end.
  virtual real_t getLength(real_t begin, real_t end) const ;

  virtual Vector2 findClosest(const Vector2& pt, real_t* u = NULL) const;

  QuantisedFunctionPtr getArcLengthToUMapping() const;
  QuantisedFunctionPtr getUToArcLengthMapping() const;

  // Get the width value of the curve
  inline const uchar_t getWidth() const { return __width; }
  inline uchar_t& getWidth() { return __width; }
  inline bool isWidthToDefault( ) const { return __width == DEFAULT_WIDTH; }


protected:
  uchar_t __width;
};

/// Curve2D Pointer
typedef RCPtr<Curve2D> Curve2DPtr;


/* ----------------------------------------------------------------------- */

SG_API real_t closestPointToSegment(Vector2& p,
                           const Vector2& segA,
                           const Vector2& segB,
                           real_t* u = NULL);

// __geom_curve_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

