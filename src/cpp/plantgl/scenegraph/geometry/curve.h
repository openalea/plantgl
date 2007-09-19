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

/** 
    \class Curve2D
    \brief Abstract base class for objects of type of 2D parametric curve.
*/


class SG_API Curve2D : public PlanarModel
{

public:

  /// A structure which helps to build an object of type of Curve.
  struct SG_API Builder : public PlanarModel::Builder {

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

  };


  /// Constructor.
  Curve2D( );

  /// Destructor
  virtual ~Curve2D( );

  /// Returns the \e fist knots value.
  virtual const real_t getFirstKnot() const = 0;
  
  /// Returns the \e last  knots value.
  virtual const real_t getLastKnot() const = 0;
  
  /// Returns \e Stride value.
  virtual const uint32_t getStride() const = 0;

  /*! Returns the \e Point for u = \e u.
     \pre 
      - \e u must be in [FirstKnot,LastKnot];*/
  virtual TOOLS(Vector2) getPointAt(real_t u) const = 0;

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  /// Return the length of the lineic model.
  real_t getLength() { return getLength(getFirstKnot(),getLastKnot()); }

  /// Return the length of the lineic model starting from u = begin.
  real_t getLength(real_t begin) { return getLength(begin,getLastKnot()); }

  /// Return the length of the lineic model from u = begin to u = end.
  virtual real_t getLength(real_t begin, real_t end);

};

/// Curve2D Pointer
typedef RCPtr<Curve2D> Curve2DPtr;
 

/* ----------------------------------------------------------------------- */

// __geom_curve_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

