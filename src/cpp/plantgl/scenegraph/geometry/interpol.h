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
/*! \file util_interpol.h
    \brief Interpolation algorithms.
*/


#ifndef __util_interpol_h__
#define __util_interpol_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/tool/rcobject.h>
#include <plantgl/math/util_vector.h>
#include "../sg_config.h"

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

class RealArray;
typedef RCPtr<RealArray> RealArrayPtr;

TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

class Point3Array;
typedef RCPtr<Point3Array> Point3ArrayPtr;
class NurbsCurve;
typedef RCPtr<NurbsCurve> NurbsCurvePtr;
class NurbsCurve2D;
typedef RCPtr<NurbsCurve2D> NurbsCurve2DPtr;

/* ----------------------------------------------------------------------- */

/**
   \class Interpol
   \brief Class for interpolate 2D or 3D points with parameters.
*/

/* ----------------------------------------------------------------------- */

class SG_API Interpol
{
public:

  /// Default Constructor. Build object is invalid.
  Interpol();

  /// Constructs Points interpolator
  /// When it is closed, the last point have to be equal to the first,
  /// but parameters have to be different.
  // With 2D points, 3rd coordinate must be constant

  Interpol( const Point3ArrayPtr& _points,
            const TOOLS(RealArrayPtr)&   _params,
            uint_t _degree,
            bool _closed );

  /// Destructor
  virtual ~Interpol();

  /// Return the Point List value
  virtual const Point3ArrayPtr& getPoints( ) const;

  /// Return the Point List field
  virtual Point3ArrayPtr& getPoints( );

  /// Return the Parameter List value
  virtual const TOOLS(RealArrayPtr)& getParameters( ) const;

  /// Return the Parameter List field
  virtual TOOLS(RealArrayPtr)& getParameters( );

  /// Return the Interpolate Degree value
  virtual uint_t getDegree( ) const;

  /// Return the Interpolate Degree field
  virtual uint_t& getDegree( );

  /// Return the Closure value
  virtual bool getClosure( ) const;

  /// Return the Closure field
  virtual bool& getClosure( );

  /// Return the 2D result curve which interpolate datas
  virtual NurbsCurve2DPtr get2DCurve();

  /// Return the 3D result curve which interpolate datas
  virtual NurbsCurvePtr get3DCurve();

  /// Intersection of 2 lines
  /// L1(u): P + u * T1
  /// L2(u): Q + u * T2
  /// R= L1(a)= L2(b) the two intersect themselves
  /// else return false
  static bool intersectLine( TOOLS(Vector3) P, TOOLS(Vector3) T1,
                             TOOLS(Vector3) Q, TOOLS(Vector3) T2,
                             real_t& a, real_t& b  );

protected:

  /// Points to interpolate
  Point3ArrayPtr points;

  /// Parameter at the points
  TOOLS(RealArrayPtr) knots;

  /// Interpolation degree
  uint_t degree;

  /// the set of points is it closed?
  bool closed;

private:

  /// Control Points
  Point3ArrayPtr CP;

  /// List of limit arcs parameters
  TOOLS(RealArrayPtr) params;

  /// Tangents
  Point3ArrayPtr T;

private:

  /// Compute tangent at each point
  virtual bool initTangent();

  /// Compute a trivial knot vector
  virtual TOOLS(RealArrayPtr) bezierKV( const TOOLS(RealArrayPtr)& _knots ) const;

  /// check the continuity of the result curve
  virtual uint_t checkContinuity() const;

  /// compute one or two arcs between points (degree 2)
  virtual uint_t bezierArc2( uint_t _arcNum );

  /// compute one or two arcs between points (degree 3)
  virtual uint_t bezierArc3( uint_t _arcNum );

  /// compute curve data
  virtual bool run();
};


/* ----------------------------------------------------------------------- */

// __util_interpol_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

