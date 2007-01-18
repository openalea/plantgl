/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon
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



/*! \file geom_extrusion.h
    \brief Definition of the geometry class Extrusion.
*/

#ifndef __geom_extrusion_h__
#define __geom_extrusion_h__

/* ----------------------------------------------------------------------- */

#include "geom_parametricmodel.h"

#ifndef GEOM_FWDEF

#include "geom_curve.h"
#include "geom_lineicmodel.h"
#include "geom_pointarray.h"
#include "geom_profile.h"

#endif

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

#ifdef GEOM_FWDEF

class Curve2D;
class LineicModel;
class ProfileTransformation;
class Point2Array;
class RealArray;

typedef RCPtr<Curve2D> Curve2DPtr;
typedef RCPtr<LineicModel> LineicModelPtr;
typedef RCPtr<ProfileTransformation> ProfileTransformationPtr;
typedef RCPtr<Point2Array> Point2ArrayPtr;
typedef RCPtr<RealArray> RealArrayPtr;

#endif

/* ----------------------------------------------------------------------- */

/**
   \class Extrusion
   \brief An Extrusion represented by an axis, a cross section and a tranformation function.
*/


class GEOM_API Extrusion : public ParametricModel
{

  public:
  
  static const RealArrayPtr DEFAULT_ORIENTATION_LIST;

  static const Point2ArrayPtr DEFAULT_SCALE_LIST;

  static const bool DEFAULT_CCW;

  static const bool DEFAULT_SOLID;

    /// A structure which helps to build a Extrusion when parsing.
    struct Builder : public ParametricModel::Builder { 

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
	    const bool _ccw = DEFAULT_CCW);
    
  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
	    const Curve2DPtr& _crossSection, 
	    const ProfileTransformationPtr _profile, 
	    const bool _solid = DEFAULT_SOLID,
	    const bool _ccw = DEFAULT_CCW);

  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
	    const Curve2DPtr& _crossSection, 
	    const RealArrayPtr& _knot,
	    const Point2ArrayPtr& _scale,
	    const RealArrayPtr& _orientation = DEFAULT_ORIENTATION_LIST,
	    const bool _solid = DEFAULT_SOLID,
	    const bool _ccw = DEFAULT_CCW);
    
  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
	    const Curve2DPtr& _crossSection, 
	    const Point2ArrayPtr& _scale,
	    const RealArrayPtr& _orientation = DEFAULT_ORIENTATION_LIST,
	    const bool _solid = DEFAULT_SOLID,
	    const bool _ccw = DEFAULT_CCW);
    
  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
	    const Curve2DPtr& _crossSection, 
	    const RealArrayPtr& _orientation,
	    const bool _solid = DEFAULT_SOLID,
	    const bool _ccw = DEFAULT_CCW);
    
  /// Constructor
  Extrusion(const LineicModelPtr& _axis,
	    const Curve2DPtr& _crossSection, 
	    const RealArrayPtr& _knot,
	    const RealArrayPtr& _orientation,
	    const bool _solid = DEFAULT_SOLID,
	    const bool _ccw = DEFAULT_CCW);
    
  /// Destructor
  virtual ~Extrusion();

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

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

}; // Extrusion

/// Extrusion Pointer
typedef RCPtr<Extrusion> ExtrusionPtr;


/* ----------------------------------------------------------------------- */


// __geom_extrusion_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

