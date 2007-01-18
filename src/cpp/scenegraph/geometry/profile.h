/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier, F. Boudon & C. Pradal
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


/*! \file geom_profile.h
    \brief Definition of the geometry class ProfileTransformation and ProfileInterpolation.
*/


#ifndef __geom_profile_h__
#define __geom_profile_h__

/* ----------------------------------------------------------------------- */

#include "Tools/rcobject.h"
#include "geom_namespace.h"
#ifndef GEOM_FWDEF
#include "geom_geometryarray2.h"
#include "geom_pointarray.h"
#include "geom_transformed.h"
#endif
/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

#ifdef GEOM_FWDEF
class Curve2DArray;
class CurveArray;
class Point2Array;
class Point3Array;
class RealArray;
class Transformation2D;
typedef RCPtr<Curve2DArray> Curve2DArrayPtr;
typedef RCPtr<CurveArray> CurveArrayPtr;
typedef RCPtr<Point2Array> Point2ArrayPtr;
typedef RCPtr<Point3Array> Point3ArrayPtr;
typedef RCPtr<RealArray> RealArrayPtr;
typedef RCPtr<Transformation2D> Transformation2DPtr;
#endif

/* ----------------------------------------------------------------------- */

/**
    \class ProfileTransformation
    \brief Class for profile transformations functions.
*/

/* ----------------------------------------------------------------------- */

class GEOM_API ProfileTransformation : public TOOLS(RefCountObject) {

    public :

  /// The Default ScalingList Field Value.
  static const Point2ArrayPtr DEFAULT_SCALE_LIST;

  /// The Default OrientationList Field Value.
  static const RealArrayPtr DEFAULT_ORIENTATION_LIST;

    /// Constructs a ProfileScaling with default value.
    ProfileTransformation();

    /// Constructs a ProfileScaling.
    ProfileTransformation(Point2ArrayPtr _scalingList,
                          RealArrayPtr _orientationList,
                          RealArrayPtr _knotList );

    /// Constructs a ProfileScaling.
    ProfileTransformation(Point2ArrayPtr _scalingList,
                          RealArrayPtr _orientationList);

    /// Destructor
    virtual ~ProfileTransformation( ) ;

    /// Return the minimal \e u value.
    virtual real_t getUMin() const;

    /// Return the maximal \e u value.
    virtual real_t getUMax() const;

    /// Return the tranformation at u = \e u
    virtual Transformation2DPtr operator() (real_t u) const ;

    /// Return the Scaling Factor List value.
    const Point2ArrayPtr& getScale() const;

    /// Return the Orientation Factor List value.
    const RealArrayPtr& getOrientation() const;

    /// Return the Scaling Factor List field.
    Point2ArrayPtr& getScale();

    /// Return the Orientation Factor List field.
    RealArrayPtr& getOrientation();

    /// Return the KnotList Factor List value.
    const RealArrayPtr getKnotList() const;

    /// Return the KnotList Factor List field.
    RealArrayPtr& getKnotList();

    /// return whether KnotList is set to its default value.
    const bool isKnotListToDefault() const;

    /// Returns whether \e self is valid.
    virtual bool isValid( ) const;

  protected:

    /// The Scaling array list.
    Point2ArrayPtr __scalingList;

    /// A pointee to the \b Orientation \b List field.
    RealArrayPtr  __orientationList;

    /// The \b knotsList field.
    RealArrayPtr __knotList;

};

/// ProfileTransformation Pointer
typedef RCPtr<ProfileTransformation> ProfileTransformationPtr;

/* ----------------------------------------------------------------------- */

// forward declaration
class ProfileInterpolation;
typedef RCPtr<ProfileInterpolation> ProfileInterpolationPtr;

/* ----------------------------------------------------------------------- */

/**
    \class ProfileInterpolation
    \brief Class for profiles Interpolation.
*/

/* ----------------------------------------------------------------------- */

class GEOM_API ProfileInterpolation : public TOOLS(RefCountObject)
{
  public :

  /// The Default Degree Field Value.
  static const uint32_t DEFAULT_DEGREE;

  /// The \b Slices field default value.
  static const uint32_t DEFAULT_STRIDE;

  /// A structure which helps to build a Profile Interpolation when parsing.
  struct Builder
    {

    /// A pointee to the \b profile list.
    Curve2DArrayPtr* ProfileList;

    /// The \b knotsList field.
    RealArrayPtr* KnotList;

    /// The \b degree field
    uint32_t* Degree;

    /// The \b stride field
    uint32_t* Stride;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual ProfileInterpolationPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

    };

  /// Default Constructor. Build object is invalid.
  ProfileInterpolation();

  /// Constructs Profiles Interpolant.
  ProfileInterpolation( Curve2DArrayPtr _profileList,
                        RealArrayPtr _knotList,
                        uint32_t _degree= DEFAULT_DEGREE,
                        uint32_t _stride= DEFAULT_STRIDE );

  /// Destructor
  virtual ~ProfileInterpolation( );

  /// Return the minimal \e u value.
  virtual real_t getUMin() const;

  /// Return the maximal \e u value.
  virtual real_t getUMax() const;

  /// Return the section at u = \e u
  virtual const Point2ArrayPtr& getSection2DAt(real_t u) const;
  virtual const Point3ArrayPtr& getSection3DAt(real_t u) const;

  /// Return the Profile List value.
  virtual const Curve2DArrayPtr& getProfileList( ) const;

  /// Return the Profile List field.
  virtual Curve2DArrayPtr& getProfileList( );

  /// Return the Knot List value.
  virtual const RealArrayPtr& getKnotList() const;

  /// Return the KnotList field.
  RealArrayPtr& getKnotList();

  /// Return whether KnotList is set to its default value.
  virtual bool isKnotListToDefault() const;

  /// Return whether KnotList is set to its default value.
  virtual bool isDegreeToDefault() const;

  /// Return whether KnotList is set to its default value.
  virtual bool isStrideToDefault() const;

  /// Returns \e Stride value.
  virtual const uint32_t& getStride( ) const;

  /// Returns \e Stride field.
  uint32_t& getStride( );

  /// Returns \e Interpolation Degree value.
  virtual const uint32_t& getDegree( ) const;

  /// Returns \e Interpolation Degree field.
  uint32_t& getDegree( );

  /// Interpolate 2D pts (r,z) or 3D pts (r cos,r sin,z)
  /// Returns \e Interpolation Mode value.
  virtual const bool& is2DInterpolMode() const;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  virtual bool interpol();

  protected:

  /// A pointee to the \b profile list.
  Curve2DArrayPtr __profileList;

  /// The \b knotsList field.
  RealArrayPtr __knotList;

  /// The stride field
  uint32_t __stride;

  /// The degree value
  uint32_t __degree;

  private:

  Point2ArrayPtr __evalPt2D;
  Curve2DArrayPtr __fctList2D; // Nurbs Curve 2D [Stride]
                             // Profiles are discretize and points are interpolled.
                             // __fctList are the functions of interpollation
  Point3ArrayPtr __evalPt3D;
  CurveArrayPtr __fctList3D;

  bool __is2D;
};

/// ProfileInterpolation Pointer
typedef RCPtr<ProfileInterpolation> ProfileInterpolationPtr;

/* ----------------------------------------------------------------------- */

// __geom_profile_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

