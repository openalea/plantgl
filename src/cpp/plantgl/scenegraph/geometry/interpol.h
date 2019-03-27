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

PGL_BEGIN_NAMESPACE

class RealArray;
typedef RCPtr<RealArray> RealArrayPtr;

PGL_END_NAMESPACE

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
            const RealArrayPtr&   _params,
            uint_t _degree,
            bool _closed );

  /// Destructor
  virtual ~Interpol();

  /// Return the Point List value
  virtual const Point3ArrayPtr& getPoints( ) const;

  /// Return the Point List field
  virtual Point3ArrayPtr& getPoints( );

  /// Return the Parameter List value
  virtual const RealArrayPtr& getParameters( ) const;

  /// Return the Parameter List field
  virtual RealArrayPtr& getParameters( );

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
  static bool intersectLine( Vector3 P, Vector3 T1,
                             Vector3 Q, Vector3 T2,
                             real_t& a, real_t& b  );

protected:

  /// Points to interpolate
  Point3ArrayPtr points;

  /// Parameter at the points
  RealArrayPtr knots;

  /// Interpolation degree
  uint_t degree;

  /// the set of points is it closed?
  bool closed;

private:

  /// Control Points
  Point3ArrayPtr CP;

  /// List of limit arcs parameters
  RealArrayPtr params;

  /// Tangents
  Point3ArrayPtr T;

private:

  /// Compute tangent at each point
  virtual bool initTangent();

  /// Compute a trivial knot vector
  virtual RealArrayPtr bezierKV( const RealArrayPtr& _knots ) const;

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

