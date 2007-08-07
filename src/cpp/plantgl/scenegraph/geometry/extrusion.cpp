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


#include "extrusion.h"
#include <plantgl/scenegraph/core/pgl_messages.h>

#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/transformation/mattransformed.h>
#include <plantgl/scenegraph/transformation/orthotransformed.h>
#include "lineicmodel.h"
#include "curve.h"
#include "profile.h"
#include "mesh.h"
#include <plantgl/tool/util_array.h>
#include <plantgl/scenegraph/container/pointarray.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

// #define DEFAULT_ORIENTATION_LIST ProfileTransformation::DEFAULT_ORIENTATION_LIST
// #define DEFAULT_SCALE_LIST ProfileTransformation::DEFAULT_SCALE_LIST

const RealArrayPtr Extrusion::DEFAULT_ORIENTATION_LIST(ProfileTransformation::DEFAULT_ORIENTATION_LIST);
const Point2ArrayPtr Extrusion::DEFAULT_SCALE_LIST(ProfileTransformation::DEFAULT_SCALE_LIST);
const bool Extrusion::DEFAULT_CCW(Mesh::DEFAULT_CCW);
const bool Extrusion::DEFAULT_SOLID(Mesh::DEFAULT_SOLID);

Extrusion::Builder::Builder() :
    ParametricModel::Builder(),
    Axis(0),
    CrossSection(0),
    Scale(0),
    Orientation(0),
    KnotList(0),
    Solid(0),
    CCW(0){
}

Extrusion::Builder::~Builder(){
    // nothing to do
}

SceneObjectPtr Extrusion::Builder::build( ) const {
    
    if (isValid()){
	if((!Scale) && (!Orientation))
	    return SceneObjectPtr(new Extrusion(*Axis,*CrossSection,
						( Solid ? *Solid : Mesh::DEFAULT_SOLID ), 
						( CCW ? *CCW : Mesh::DEFAULT_CCW )));

	ProfileTransformationPtr _profile;
	if(KnotList)_profile = ProfileTransformationPtr(new ProfileTransformation(( Scale ? *Scale : DEFAULT_SCALE_LIST),
										  ( Orientation ? *Orientation : DEFAULT_ORIENTATION_LIST),*KnotList));
	else{
	    _profile = ProfileTransformationPtr(new ProfileTransformation(( Scale ? *Scale : DEFAULT_SCALE_LIST),
									  ( Orientation ? *Orientation : DEFAULT_ORIENTATION_LIST)));
	}
	return SceneObjectPtr(new Extrusion(*Axis,*CrossSection,_profile,
					    ( Solid ? *Solid : Mesh::DEFAULT_SOLID ), 
					    ( CCW ? *CCW : Mesh::DEFAULT_CCW )));
    }
    return SceneObjectPtr(0);
}

void Extrusion::Builder::destroy() {
    if (Axis) delete Axis;
    if (CrossSection) delete CrossSection;
    if (Scale) delete Scale;
    if (Orientation) delete Orientation;
    if (KnotList) delete KnotList;
    if (Solid) delete Solid;
    if (CCW) delete CCW;
}

bool Extrusion::Builder::isValid( ) const {
    // Axis field
    if(!Axis) {
	genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Extrusion","Axis");
	return false;
    };

    if(!(*Axis)) {
	genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Extrusion","*Axis");
	return false;
    };

    if(!(*Axis)->isValid()) {
	genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","Axis","Must be a valid Geometry Object.");
	return false;
    };

    if(!CrossSection) {
	genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Extrusion","CrossSection");
	return false;
    };

    if(!(*CrossSection)) {
	genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Extrusion","*CrossSection");
	return false;
    };

    if(!(*CrossSection)->isValid()) {
	genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","CrossSection","Must be a valid Geometry Object.");
	return false;
    };
    
    if(Scale){
	if(!(*Scale)->isValid()) {
	    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","Scale","Must be a valid Object.");
	    return false;
	};
	if((*Scale)->getSize() == 0 ){
	    genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Extrusion","Scale","Must have more values.");
	    return false;
	}

    }

    if(Orientation){
	if((*Orientation)->getSize() == 0 ){
	    genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Extrusion","Orientation","Must have more values.");
	    return false;
	}
    }

    if( ((Scale)&&(*Scale)->getSize() !=1) &&
	((Orientation)&&(*Orientation)->getSize() !=1) &&
	((*Scale)->getSize()!=((*Orientation)->getSize())) ){
	    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","Orientation",
		       "Must specifie Scale and Orientation with the same number of value."); 
	    return false;
    }

    if(KnotList){
	if(((!Scale)||(*Scale)->getSize() ==1)&&((!Orientation)||(*Orientation)->getSize() ==1)){
	    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","KnotList",
		       "Must specifie Scale or Orientation with more than one value."); 
	}
	else if(Scale){
	    if(((*Scale)->getSize() > 1)&&((*Scale)->getSize() != (*KnotList)->getSize())){
		genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Extrusion","KnotList",
			   "Must have the same number of value than Scale .");
		return false;
	    }
	}
	else {
	    if(((*Orientation)->getSize() ==1)&&(((*Orientation)->getSize()) != (*KnotList)->getSize())){
		genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Extrusion","KnotList",
			   "Must have the same number of value than Orientation .");
		return false;
	    }
	}
    }

    return true;
}


/* ----------------------------------------------------------------------- */

Extrusion::Extrusion( ) :
    ParametricModel(),
    __axis(0),
    __crossSection(0),
    __profile(0),
    __solid(Mesh::DEFAULT_SOLID),
    __ccw(Mesh::DEFAULT_CCW){
}

Extrusion::Extrusion(const LineicModelPtr& _axis,const Curve2DPtr& _crossSection, 
		     const ProfileTransformationPtr _profile, const bool _solid, const bool _ccw  ) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(_profile),
    __solid(_solid),
    __ccw(_ccw){
    GEOM_ASSERT(isValid());
}

Extrusion::Extrusion(const LineicModelPtr& _axis,const Curve2DPtr& _crossSection,
		     const bool _solid, const bool _ccw ) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(0),
    __solid(_solid),
    __ccw(_ccw){
    GEOM_ASSERT(isValid());
}

Extrusion::Extrusion(const LineicModelPtr& _axis,
		       const Curve2DPtr& _crossSection, 
		       const RealArrayPtr& _knot,
		       const Point2ArrayPtr& _scale,
		       const RealArrayPtr& _orientation,
		       const bool _solid,
		       const bool _ccw) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(new ProfileTransformation(_scale,_orientation,_knot)),
    __solid(_solid),
    __ccw(_ccw){
    GEOM_ASSERT(isValid());
}

Extrusion::Extrusion(const LineicModelPtr& _axis,
		       const Curve2DPtr& _crossSection, 
		       const Point2ArrayPtr& _scale,
		       const RealArrayPtr& _orientation,
		       const bool _solid,
		       const bool _ccw) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(new ProfileTransformation(_scale,_orientation)),
    __solid(_solid),
    __ccw(_ccw){
    GEOM_ASSERT(isValid());
}

Extrusion::Extrusion(const LineicModelPtr& _axis,
		       const Curve2DPtr& _crossSection, 
		       const RealArrayPtr& _orientation,
		       const bool _solid ,
		       const bool _ccw) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(new ProfileTransformation(ProfileTransformation::DEFAULT_SCALE_LIST,_orientation)),
    __solid(_solid),
    __ccw(_ccw){
    GEOM_ASSERT(isValid());
}

Extrusion::Extrusion(const LineicModelPtr& _axis,
		       const Curve2DPtr& _crossSection, 
		       const RealArrayPtr& _knot,
		       const RealArrayPtr& _orientation,
		       const bool _solid,
		       const bool _ccw) :
    ParametricModel(),
    __axis(_axis),
    __crossSection(_crossSection),
    __profile(new ProfileTransformation(DEFAULT_SCALE_LIST,_orientation,_knot)),
    __solid(_solid),
    __ccw(_ccw){
    GEOM_ASSERT(isValid());
}

Extrusion::~Extrusion(){
}

/* ----------------------------------------------------------------------- */

const Curve2DPtr& 
Extrusion::getCrossSection() const {
  return __crossSection;
}

Curve2DPtr&
Extrusion::getCrossSection() {
  return __crossSection;
}

const LineicModelPtr& 
Extrusion::getAxis() const {
  return __axis;
}

LineicModelPtr& 
Extrusion::getAxis() {
  return __axis;
}

const ProfileTransformationPtr& 
Extrusion::getProfileTransformation() const {
  return __profile;
}

ProfileTransformationPtr& 
Extrusion::getProfileTransformation(){
  return __profile;
}

bool 
Extrusion::isACurve( ) const {
  return false;
}

bool 
Extrusion::isASurface( ) const {
  return !__solid;
} 

bool
Extrusion::isAVolume( ) const {
  return __solid;
} 

bool 
Extrusion::isSolidToDefault() const{
  return (__solid == Mesh::DEFAULT_SOLID);
}

const bool 
Extrusion::getSolid() const {
  return __solid;
}

bool& 
Extrusion::getSolid() {
  return __solid;
}

bool 
Extrusion::isCCWToDefault() const {
  return (__ccw == Mesh::DEFAULT_CCW);
}

const bool 
Extrusion::getCCW() const {
  return __ccw;
}

bool& 
Extrusion::getCCW() {
  return __ccw;
}    

/// Return the Scaling Factor List value.
const Point2ArrayPtr& Extrusion::getScale() const
{ return __profile->getScale(); }

/// Return the Scaling Factor List field.
Point2ArrayPtr& Extrusion::getScale()
{ return __profile->getScale(); }

/// Return the Orientation Factor List value.
const TOOLS(RealArrayPtr)& Extrusion::getOrientation() const
{ return __profile->getOrientation(); }

/// Return the Orientation Factor List field.
TOOLS(RealArrayPtr)& Extrusion::getOrientation()
{ return __profile->getOrientation(); }

/// Return the KnotList Factor List value.
const TOOLS(RealArrayPtr) Extrusion::getKnotList() const
{ return __profile->getKnotList(); }

/// Return the KnotList Factor List field.
TOOLS(RealArrayPtr)& Extrusion::getKnotList()
{ return __profile->getKnotList(); }

/* ----------------------------------------------------------------------- */
bool 
Extrusion::apply( Action& action ) {
  return action.process(this);
}

bool
Extrusion::isValid( ) const {
  Builder _builder;
  RealArrayPtr _knot(0);
  _builder.Axis = const_cast<LineicModelPtr *>(&__axis);
  _builder.CrossSection = const_cast<Curve2DPtr *>(&__crossSection);
  if(__profile){
    if(!isScaleToDefault())
      _builder.Scale = const_cast<Point2ArrayPtr *>(&(__profile->getScale()));
      if(!isOrientationToDefault())
	_builder.Orientation = const_cast<RealArrayPtr *>(&(__profile->getOrientation()));
      if(!__profile->isKnotListToDefault()){
	_knot = __profile->getKnotList();
	_builder.KnotList = const_cast<RealArrayPtr *>(&(_knot));
      }
  }
  return _builder.isValid();
}

bool
Extrusion::isScaleToDefault() const {
  if((__profile.isValid())&&(__profile->getScale().isValid()))return (__profile->getScale() == DEFAULT_SCALE_LIST);
  else return true;
}

bool
Extrusion::isOrientationToDefault() const {
  if((__profile.isValid())&&(__profile->getOrientation().isValid()))return (__profile->getOrientation() == DEFAULT_ORIENTATION_LIST);
  else return true;
}

bool
Extrusion::isKnotListToDefault() const {
  if(__profile)return (__profile->isKnotListToDefault());
  else return true;
}

SceneObjectPtr
Extrusion::copy() const {
  Extrusion * ptr = new Extrusion(*this);
  if(__axis)ptr->getAxis().cast(__axis->copy());
  if(__crossSection)ptr->getCrossSection().cast(__crossSection->copy());
  if(__profile){
    Point2ArrayPtr scale((__profile->getScale()?new Point2Array(*(__profile->getScale())):0));
    RealArrayPtr orientation((__profile->getOrientation()?new RealArray(*(__profile->getOrientation())):0));
    RealArrayPtr knot((__profile->getKnotList()?new RealArray(*(__profile->getKnotList())):0));										       
    ptr->getProfileTransformation() = ProfileTransformationPtr(new ProfileTransformation(scale,orientation,knot));
  }
  return SceneObjectPtr(ptr);
}


/* ----------------------------------------------------------------------- */

#undef DEFAULT_ORIENTATION_LIST
#undef DEFAULT_SCALE_LIST

/* ----------------------------------------------------------------------- */
