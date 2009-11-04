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


/*! \file geom_extrusion.h
    \brief Definition of the geometry class Extrusion.
*/

#ifndef __geom_extrusion_h__
#define __geom_extrusion_h__

/* ----------------------------------------------------------------------- */

#include "parametricmodel.h"
#include <plantgl/math/util_vector.h>

#ifndef GEOM_FWDEF

#include "curve.h"
#include "lineicmodel.h"
#include "pointarray.h"
#include "profile.h"

#endif

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

#ifdef GEOM_FWDEF
class RealArray;
typedef RCPtr<RealArray> RealArrayPtr;
#endif

TOOLS_END_NAMESPACE

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
  
  static const TOOLS(RealArrayPtr) DEFAULT_ORIENTATION_LIST;

  static const Point2ArrayPtr DEFAULT_SCALE_LIST;

  static const bool DEFAULT_CCW;

  static const bool DEFAULT_SOLID;

  static const TOOLS(Vector3) DEFAULT_INITIAL_NORMAL;

    /// A structure which helps to build a Extrusion when parsing.
    struct SG_API Builder : public ParametricModel::Builder { 

	    /// A pointee to a curve.
	    LineicModelPtr *  Axis;
	    
	    /// A pointee to a diameter function.
	    Curve2DPtr * CrossSection;
	    
	    /// A pointee to the \b Scaling \b List field.
	    Point2ArrayPtr * Scale;
	    
	    /// A pointee to the \b Orientation \b List field.
	    TOOLS(RealArrayPtr) * Orientation;
	    
	    /// A pointee to the \b KnotsList field.
	    TOOLS(RealArrayPtr) * KnotList;
	    
	    /// A pointee to the \b Solid field.
	    bool * Solid;
	    
	    /// A pointee to the \b CCW field.
	    bool * CCW;

	    /// A pointee to the \b InitialNormal field.
	    TOOLS(Vector3) * InitialNormal;

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
		const TOOLS(Vector3)& initialNormal = DEFAULT_INITIAL_NORMAL);
    
  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
	    const Curve2DPtr& _crossSection, 
	    const ProfileTransformationPtr _profile, 
	    const bool _solid = DEFAULT_SOLID,
	    const bool _ccw = DEFAULT_CCW,
		const TOOLS(Vector3)& initialNormal = DEFAULT_INITIAL_NORMAL);

  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
	    const Curve2DPtr& _crossSection, 
	    const Point2ArrayPtr& _scale,
	    const TOOLS(RealArrayPtr)& _orientation = DEFAULT_ORIENTATION_LIST,
	    const TOOLS(RealArrayPtr)& _knot = TOOLS(RealArrayPtr(0)),
	    const bool _solid = DEFAULT_SOLID,
	    const bool _ccw = DEFAULT_CCW,
		const TOOLS(Vector3)& initialNormal = DEFAULT_INITIAL_NORMAL);
    
   /// Constructor
  Extrusion(const LineicModelPtr& _axis,
	    const Curve2DPtr& _crossSection, 
	    const TOOLS(RealArrayPtr)& _knot,
	    const Point2ArrayPtr& _scale,
	    const bool _solid = DEFAULT_SOLID,
	    const bool _ccw = DEFAULT_CCW,
		const TOOLS(Vector3)& initialNormal = DEFAULT_INITIAL_NORMAL);
    
  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
	    const Curve2DPtr& _crossSection, 
	    const TOOLS(RealArrayPtr)& _orientation,
	    const TOOLS(RealArrayPtr)& _knot = TOOLS(RealArrayPtr(0)),
	    const bool _solid = DEFAULT_SOLID,
	    const bool _ccw = DEFAULT_CCW,
		const TOOLS(Vector3)& initialNormal = DEFAULT_INITIAL_NORMAL); 
    
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
  const TOOLS(RealArrayPtr)& getOrientation() const;

  /// Return the Orientation Factor List field.
  TOOLS(RealArrayPtr)& getOrientation();

  /// Return the KnotList Factor List value.
  const TOOLS(RealArrayPtr) getKnotList() const;

  /// Return the KnotList Factor List field.
  TOOLS(RealArrayPtr)& getKnotList();

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
  inline const TOOLS(Vector3)& getInitialNormal() const
  { return __initialNormal; }

  /// return the initial normal
  inline TOOLS(Vector3)& getInitialNormal()
  { return __initialNormal; }

  // return whether initial normal is to default
  inline bool isInitialNormalToDefault()
  { return __initialNormal == TOOLS(Vector3()); }

  // Get the value of initial normal
  TOOLS(Vector3) getInitialNormalValue() const ;

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
	TOOLS(Vector3) __initialNormal;

}; // Extrusion

/// Extrusion Pointer
typedef RCPtr<Extrusion> ExtrusionPtr;


/* ----------------------------------------------------------------------- */


// __geom_extrusion_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

