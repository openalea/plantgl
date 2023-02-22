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



/*! \file geom_extrusion.h
    \brief Definition of the geometry class Extrusion.
*/

#ifndef __geom_extrusion_h__
#define __geom_extrusion_h__

/* ----------------------------------------------------------------------- */

#include "parametricmodel.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/math/util_matrix.h>

#ifndef GEOM_FWDEF

#include "curve.h"
#include "lineicmodel.h"
#include "pointarray.h"
#include "profile.h"

#endif

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

#ifdef GEOM_FWDEF
class RealArray;
typedef RCPtr<RealArray> RealArrayPtr;
#endif

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

#ifdef GEOM_FWDEF

class Curve2D;
class LineicModel;
class ProfileTransformation;
class Point2Array;

typedef RCPtr<Curve2D> Curve2DPtr;
typedef RCPtr<LineicModel> LineicModelPtr;
typedef RCPtr<ProfileTransformation> ProfileTransformationPtr;
typedef RCPtr<Point2Array> Point2ArrayPtr;

#endif

/* ----------------------------------------------------------------------- */

/**
   \class Extrusion
   \brief An Extrusion represented by an axis, a cross section and a tranformation function.
*/


class SG_API Extrusion : public ParametricModel
{

  public:

  static const RealArrayPtr DEFAULT_ORIENTATION_LIST;

  static const Point2ArrayPtr DEFAULT_SCALE_LIST;

  static const bool DEFAULT_CCW;

  static const bool DEFAULT_SOLID;

  static const Vector3 DEFAULT_INITIAL_NORMAL;

    /// A structure which helps to build a Extrusion when parsing.
    struct SG_API Builder : public ParametricModel::Builder {

        /// A pointee to a curve.
        LineicModelPtr *  Axis;

        /// A pointee to a diameter function.
        Curve2DPtr * CrossSection;

        /// A pointee to the \b Scaling \b List field.
        Point2ArrayPtr * Scale;

        /// A pointee to the \b Orientation \b List field.
        RealArrayPtr * Orientation;

        /// A pointee to the \b KnotsList field.
        RealArrayPtr * KnotList;

        /// A pointee to the \b Solid field.
        bool * Solid;

        /// A pointee to the \b CCW field.
        bool * CCW;

        /// A pointee to the \b InitialNormal field.
        Vector3 * InitialNormal;

        /// Constructor.
        Builder( );

        /// Destructor.
        ~Builder( );

        virtual SceneObjectPtr build( ) const;

        virtual void destroy( );

        virtual bool isValid( ) const;

    };

  /// Default Constructor. Build object is invalid.
  Extrusion();

    /// Constructor
  Extrusion(const LineicModelPtr& _axis,
        const Curve2DPtr& _crossSection,
        const bool _solid = DEFAULT_SOLID,
        const bool _ccw = DEFAULT_CCW,
        const Vector3& initialNormal = DEFAULT_INITIAL_NORMAL);

  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
        const Curve2DPtr& _crossSection,
        const ProfileTransformationPtr _profile,
        const bool _solid = DEFAULT_SOLID,
        const bool _ccw = DEFAULT_CCW,
        const Vector3& initialNormal = DEFAULT_INITIAL_NORMAL);

  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
        const Curve2DPtr& _crossSection,
        const Point2ArrayPtr& _scale,
        const RealArrayPtr& _orientation = DEFAULT_ORIENTATION_LIST,
        const RealArrayPtr& _knot = TOOLS(RealArrayPtr(0)),
        const bool _solid = DEFAULT_SOLID,
        const bool _ccw = DEFAULT_CCW,
        const Vector3& initialNormal = DEFAULT_INITIAL_NORMAL);

   /// Constructor
  Extrusion(const LineicModelPtr& _axis,
        const Curve2DPtr& _crossSection,
        const RealArrayPtr& _knot,
        const Point2ArrayPtr& _scale,
        const bool _solid = DEFAULT_SOLID,
        const bool _ccw = DEFAULT_CCW,
        const Vector3& initialNormal = DEFAULT_INITIAL_NORMAL);

  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
        const Curve2DPtr& _crossSection,
        const RealArrayPtr& _orientation,
        const RealArrayPtr& _knot = TOOLS(RealArrayPtr(0)),
        const bool _solid = DEFAULT_SOLID,
        const bool _ccw = DEFAULT_CCW,
        const Vector3& initialNormal = DEFAULT_INITIAL_NORMAL);

  /// Destructor
  virtual ~Extrusion();

  PGL_OBJECT(Extrusion)

    /// Return the cross section value of \e self.
  virtual const Curve2DPtr& getCrossSection() const;

    /// Return the cross section field of \e self.
  Curve2DPtr& getCrossSection();

    /// Return the axis value of \e self.
  virtual const LineicModelPtr& getAxis() const;

    /// Return the axis field of \e self.
  LineicModelPtr& getAxis();

  /// Return the profile transformation value of \e self.
  virtual const ProfileTransformationPtr& getProfileTransformation() const ;

  /// Return the profile transformation field of \e self.
  ProfileTransformationPtr& getProfileTransformation();

  /// Return the Scaling Factor List value.
  const Point2ArrayPtr& getScale() const;

  /// Return the Scaling Factor List field.
  Point2ArrayPtr& getScale();

  /// Return the Orientation Factor List value.
  const RealArrayPtr& getOrientation() const;

  /// Return the Orientation Factor List field.
  RealArrayPtr& getOrientation();

  /// Return the KnotList Factor List value.
  const RealArrayPtr getKnotList() const;

  /// Return the KnotList Factor List field.
  RealArrayPtr& getKnotList();

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  virtual bool isValid( ) const;

  /// return whether Solid is set to its default value.
  virtual bool isSolidToDefault() const;

  /// return whether \e self is solid or not.
  virtual const bool getSolid() const ;

  /// return solid field.
  virtual bool& getSolid();

  /// return whether CCW is set to its default value.
  virtual bool isCCWToDefault() const ;

  /// return whether the vertices of the cross-section are listed in CCW order or not.
  virtual const bool getCCW() const;

  /// return CCW field.
  virtual bool& getCCW();

  /// return whether Scaling Factor List is set to its default value.
  virtual bool isScaleToDefault() const;

  /// return whether Orientation Factor List is  set to its default value.
  virtual bool isOrientationToDefault() const;

  /// return whether KnotList is set to its default value.
  virtual bool isKnotListToDefault() const;

  /// return the initial normal
  inline const Vector3& getInitialNormal() const
  { return __initialNormal; }

  /// return the initial normal
  inline Vector3& getInitialNormal()
  { return __initialNormal; }

  // return whether initial normal is to default
  inline bool isInitialNormalToDefault()
  { return __initialNormal == TOOLS(Vector3()); }

  // Get the value of initial normal
  Vector3 getInitialNormalValue() const ;

  // Get the point value at u,v.
  Vector3 getPointAt(real_t u, real_t v) const;

  // Get the point value at u,v.
  Vector3 getPointAt(real_t u, real_t v, const Matrix3& frame) const;

  /* Returns the \e Tangent for u = \e u and v = \e v.
     \pre
      - \e u, \e v must be in [0,1];*/
  Vector3 getUTangentAt(real_t u,real_t v) const;

  /* Returns the \e Tangent for u = \e u and v = \e v.
     \pre
      - \e u, \e v must be in [0,1];*/
  Vector3 getUTangentAt(real_t u,real_t v, const Matrix3& frame) const;

  /* Returns the \e Tangent for u = \e u and v = \e v.
     \pre
      - \e u, \e v must be in [0,1];*/
  Vector3 getVTangentAt(real_t u,real_t v) const;

  /* Returns the \e Tangent for u = \e u and v = \e v.
     \pre
      - \e u, \e v must be in [0,1];*/
  Vector3 getVTangentAt(real_t u,real_t v, const Matrix3& frame) const;

  /* Returns the principal \e Normal for u,v = \e u, \e v.
     \pre
      - \e u, \e v must be in [0,1];*/
  Vector3 getNormalAt(real_t u,real_t v) const;

  /* Returns the principal \e Normal for u,v = \e u, \e v.
     \pre
      - \e u, \e v must be in [0,1];*/
  Vector3 getNormalAt(real_t u,real_t v, const Matrix3& frame) const;

  /*! Returns the \e derivative of degree \e d for u = \e u, v = \e v.
     \pre
     - \e u, \e v must be in [0,1];*/
  // Vector3 getDerivativeAt(real_t u, real_t v, int du, int dv) const;

  // Get the matrix frame value at u.
  Matrix3 getFrameAt(real_t u) const;

  // Get the next matrix frame value at u+du.
  Matrix3 getNextFrameAt(real_t u, Matrix3 m, real_t du) const;

  // Get the derivative matrix frame value at u+du.
  Matrix3 getFrameDerivativeAt(real_t u, real_t du = 0.01) const;

  // Get the derivative matrix frame value at u+du.
  Matrix3 getFrameDerivativeAt(real_t u, Matrix3 m, real_t du = 0.01) const;

  // Compute the derivative.
  Vector3 getSecondDerivativeUUAt(real_t u, real_t v) const;

  // Compute the derivative.
  Vector3 getSecondDerivativeUUAt(real_t u, real_t v, const Matrix3& frame) const;

  // Compute the derivative.
  Vector3 getSecondDerivativeUVAt(real_t u, real_t v) const;

  // Compute the derivative.
  Vector3 getSecondDerivativeUVAt(real_t u, real_t v, const Matrix3& frame) const;

  // Compute the derivative.
  Vector3 getSecondDerivativeVVAt(real_t u, real_t v) const;

  // Compute the derivative.
  Vector3 getSecondDerivativeVVAt(real_t u, real_t v, const Matrix3& frame) const;

  real_t getUToTransformationU(real_t u) const;

  /*!
    Compute a section line of the patch corresponding to a constant u value
   */
  LineicModelPtr getIsoUSectionAt(real_t u) const;
    
  /*!
    Compute a section line of the patch corresponding to a constant v value
   */
  LineicModelPtr getIsoVSectionAt(real_t v) const;
    
protected:

    /// The axis of the extrusion.
    LineicModelPtr __axis;

    /// The cross section of the extrusion.
    Curve2DPtr __crossSection;

    /// The Profile Transformation Object
    ProfileTransformationPtr __profile;

    /// The Solid Field.
    bool __solid;

    /// The CCW Field.
    bool __ccw;

    /// The initial normal
    Vector3 __initialNormal;

}; // Extrusion

/// Extrusion Pointer
typedef RCPtr<Extrusion> ExtrusionPtr;


/* ----------------------------------------------------------------------- */


// __geom_extrusion_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

