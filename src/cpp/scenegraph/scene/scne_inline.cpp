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

#include "scne_inline.h"
#include "scne_scene.h"
#include "geom_boundingbox.h"
#include "geom_translated.h"

#include "util_messages.h"
#include "Tools/util_string.h"
#include "Tools/dirnames.h"

#include "actn_bboxcomputer.h"
#include "actn_discretizer.h"

#include <fstream>

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;
//Inline::DEFAULT_BBoxSize

/* ----------------------------------------------------------------------- */

GeomInline::Builder::Builder( ) :
    GeomShape3D::Builder(),
    FileName(0),
    Scene(0),
    BBoxCenter(0),
    Translation(0),
    BBoxSize(0),
    Scale(0){
}

GeomInline::Builder::~Builder( ){
}

SceneObjectPtr
GeomInline::Builder::build( ){
  if (isValid()){
	if(FileName){
	  if(!BBoxSize){
	    if(!BBoxCenter){
	      GeomInline * g = new GeomInline(expand_dirname(*FileName));  
	      if(Scale)g->setScale(*Scale);
	      if(Translation)g->setTranslation(*Translation);
	      return SceneObjectPtr(g);
	    }
	    else {
	      GeomInline * g = new GeomInline(expand_dirname(*FileName),
					      *BBoxCenter);
	      if(Scale)g->setScale(*Scale);
	      return SceneObjectPtr(g);
	    }
	  }
	  else {
	    if(!BBoxCenter){
	      GeomInline * g = new GeomInline(*BBoxSize,
					      expand_dirname(*FileName));
	      if(Translation)g->setTranslation(*Translation);
	      return SceneObjectPtr(g);
	    }
	    else {
	      GeomInline * g = 
		new GeomInline(expand_dirname(*FileName),
			       *BBoxCenter,*BBoxSize);
	      return SceneObjectPtr(g);
	    }
	  }
	}
	else{
	  if(!BBoxSize){
	    if(!BBoxCenter){
	      GeomInline * g = new GeomInline(*Scene);
	      if(Scale)g->setScale(*Scale);
	      if(Translation)g->setTranslation(*Translation);
	      return SceneObjectPtr(g);
	    }
	    else {
	      GeomInline * g = new GeomInline(*Scene,*BBoxCenter);
	      if(Scale)g->setScale(*Scale);
	      return SceneObjectPtr(g);
	    }
	  }
	  else {
	    if(!BBoxCenter){
	      GeomInline * g = new GeomInline(*BBoxSize,*Scene);
	      if(Translation)g->setTranslation(*Translation);
	      return SceneObjectPtr(g);						   
	    }
	    else {
	      GeomInline * g = new GeomInline(*Scene,*BBoxCenter,*BBoxSize);
	      return SceneObjectPtr(g);
	    }
	  }
	}
  }
  return SceneObjectPtr();   
}

void
GeomInline::Builder::destroy( ){
    if(FileName) delete FileName;
    if(Scene) delete Scene;
    if(BBoxCenter) delete BBoxCenter;
    if(BBoxSize) delete BBoxSize;
    if(Translation) delete Translation;
    if(Scale) delete Scale;
}


bool GeomInline::Builder::isValid( ) const {
  if (! FileName && ! Scene) {
    genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Inline","FileName");
    return false;
  };
  if(FileName){
      ifstream _file(FileName->c_str());
      if (!_file){
          string _mess = "Cannot open " + *FileName;
          genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","FileName",_mess.c_str());
          return false;
      }
  }

  if(Scene && !(*Scene)->isValid()){
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","Scene","Not Valid");
      return false;      
  }

  if(BBoxCenter && Translation){
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","BBoxCenter","Translation and BBoxCenter defined");
      return false;      
  }

  if(BBoxCenter && !BBoxCenter->isValid()){
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","BBoxCenter","Not Valid");
      return false;      
  }

  if(Translation && !Translation->isValid()){
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","Translation","Not Valid");
      return false;      
  }

  if(BBoxSize && Scale){
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","BBoxSize","Scale and BBoxSize defined");
      return false;      
  }

  if(BBoxSize && !BBoxSize->isValid()){
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","BBoxSize","Not Valid");
      return false;
  }
      
  if(Scale && !Scale->isValid()){
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","Scale","Not Valid");
      return false;
  }

  return true;
}

/* ----------------------------------------------------------------------- */

/// Default constructor.
GeomInline::GeomInline(const string& filename) :
    GeomShape3D(),
    __filename(filename),
    __filled(0),
    __scene(0),
    __bbox(0){
    __scene = ScenePtr(new Scene(filename, *errorStream));
    initBBox();
    if ( __name.empty() )
        __name = "INLINE_"+number(getId());
}

/// Constructor.
GeomInline::GeomInline(const string& filename, const Vector3& bboxCenter) :
    GeomShape3D(),
    __filename(filename),
    __bboxCenter(bboxCenter),
    __filled(1),
    __scene(0),
    __bbox(0){
  __scene = ScenePtr(new Scene(filename, *errorStream));
  initBBox();
    if ( __name.empty() )
        __name = "INLINE_"+number(getId());
}

/// Constructor.
GeomInline::GeomInline(const Vector3& bboxSize, const string& filename) :
    GeomShape3D(),
    __filename(filename),
    __bboxSize(bboxSize),
    __filled(2),
    __scene(0),
    __bbox(0){
    __scene = ScenePtr(new Scene(filename, *errorStream));
    initBBox();
    if ( __name.empty() )
        __name = "INLINE_"+number(getId());
}

/// Constructor.
GeomInline::GeomInline(const string& filename, const Vector3& bboxCenter, const Vector3& bboxSize) :
    GeomShape3D(),
    __filename(filename),
    __bboxCenter(bboxCenter),
    __bboxSize(bboxSize),
    __filled(3),
    __scene(0),
    __bbox(0){
    __scene = ScenePtr(new Scene(filename, *errorStream));
    initBBox();
    if ( __name.empty() )
        __name = "INLINE_"+number(getId());
}

/// Constructor.
GeomInline::GeomInline(const ScenePtr& scene) :
    GeomShape3D(),
    __filename(""),
    __filled(0),
    __scene(scene),
    __bbox(0){
    initBBox();
    if ( __name.empty() )
        __name = "INLINE_"+number(getId());
}

/// Constructor.
GeomInline::GeomInline(const ScenePtr& scene, const Vector3& bboxCenter) :
    GeomShape3D(),
    __filename(""),
    __bboxCenter(bboxCenter),
    __filled(1),
    __scene(scene),
    __bbox(0){
  initBBox();
    if ( __name.empty() )
        __name = "INLINE_"+number(getId());
}

/// Constructor.
GeomInline::GeomInline(const Vector3& bboxSize,const ScenePtr& scene) :
    GeomShape3D(),
    __filename(""),
    __bboxSize(bboxSize),
    __filled(2),
    __scene(scene),
    __bbox(0){
    initBBox();
    if ( __name.empty() )
        __name = "INLINE_"+number(getId());
}

/// Constructor.
GeomInline::GeomInline(const ScenePtr& scene, const Vector3& bboxCenter, const Vector3& bboxSize) :
    GeomShape3D(),
    __filename(""),
    __bboxCenter(bboxCenter),
    __bboxSize(bboxSize),
    __filled(3),
    __scene(scene),
    __bbox(0){
    initBBox();
    if ( __name.empty() )
        __name = "INLINE_"+number(getId());
}

GeomInline::~GeomInline()
{
}

bool GeomInline::apply( Action& action ){
  return action.process(this);
}

void GeomInline::initBBox(){
    Discretizer d;
    BBoxComputer b(d);
    if (__scene.isValid() && (! __scene->isEmpty())) {
        Scene::iterator _i = __scene->getBegin();
        while ((_i != __scene->getEnd())&&(!((*(_i++))->applyGeometryOnly(b))));
        if(b.getBoundingBox()){
            __bbox = BoundingBoxPtr(b.getBoundingBox());
        }
        else {
            __bbox = BoundingBoxPtr(new BoundingBox(Vector3(-1,-1,-1),
                                                   Vector3(1,1,1)));
        }
        while (_i != __scene->getEnd()) {
            if((*(_i++))->applyGeometryOnly(b)){
                __bbox->extend(b.getBoundingBox());
            }
        }
    }
 }

const string&
GeomInline::getFileName() const {
  return __filename;
}

const BoundingBoxPtr&
GeomInline::getRealBBox() const{
  return __bbox;
}

bool GeomInline::isValid( ) const{
    if(!__scene) return false;
    else return __scene->isValid();
}

const Vector3 GeomInline::getBBoxCenter() const {
    if(__filled == 1 || __filled == 3)
        return __bboxCenter;
    else if(__bbox) return __bbox->getCenter();
    else return Vector3(0,0,0);
}

const Vector3 GeomInline::getBBoxSize() const{
    if(__filled == 2 || __filled == 3)
        return __bboxSize;
    else if(__bbox) return __bbox->getSize();
    else return Vector3(1,1,1);
}

Vector3 GeomInline::getTranslation() const{
    if(__bbox.isValid() && !isBBoxCenterToDefault()){
        Vector3 scale = getScale();
        Vector3 center = Vector3(__bbox->getCenter().x()*scale.x(),
                                __bbox->getCenter().y()*scale.y(),
                                __bbox->getCenter().z()*scale.z());
        return (__bboxCenter - center);
    }
    else return Vector3(0,0,0);
}

void GeomInline::setTranslation(const Vector3& t) {
    if(__bbox.isValid()){
	  Vector3 scale = getScale();
	  Vector3 center = Vector3(__bbox->getCenter().x()*scale.x(),
							   __bbox->getCenter().y()*scale.y(),
							   __bbox->getCenter().z()*scale.z());
	  __bboxCenter =  center + t;
    }
    else __bboxCenter = t;
    if(isBBoxCenterToDefault())__filled +=1;
}

Vector3 GeomInline::getScale() const {
    if(__bbox.isValid() && !isBBoxSizeToDefault())
        return ( Vector3((!__bbox->getSize().x() ? 1 : __bboxSize.x() / (__bbox->getSize().x())),
                         (!__bbox->getSize().y() ? 1 : __bboxSize.y() / (__bbox->getSize().y())),
                         (!__bbox->getSize().z() ? 1 : __bboxSize.z() / (__bbox->getSize().z()))));
    else return Vector3(1,1,1);
}

void GeomInline::setScale(const Vector3& s)  {
  if(__bbox.isValid()) {
    __bboxSize = Vector3(__bbox->getSize().x()*s.x(),__bbox->getSize().y()*s.y(),__bbox->getSize().z()*s.z());
  }
  else __bboxSize = s;
  if(isBBoxSizeToDefault())__filled+=2;
}

bool GeomInline::isBBoxCenterToDefault() const {
  return (__filled != 1 && __filled != 3);
}

bool GeomInline::isBBoxSizeToDefault() const {
  return (__filled != 2 && __filled != 3);
}

const ScenePtr& GeomInline::getScene() const {
  return __scene;
}

SceneObjectPtr GeomInline::copy() const
{
  GeomInline * ptr = new GeomInline(*this);
  if(__bbox)ptr->__bbox = BoundingBoxPtr(new BoundingBox(*__bbox));
  if(__scene)ptr->__scene = __scene->copy();
  return SceneObjectPtr(ptr);
}

bool
GeomInline::applyGeometryFirst( Action& action )
{
  if(__scene) return __scene->applyGeometryFirst(action );
  return false;
}

bool
GeomInline::applyGeometryOnly( Action& action )
{
  if(__scene) return __scene->applyGeometryOnly(action );
  return false;
}

bool
GeomInline::applyAppearanceFirst( Action& action )
{
  if(__scene) return __scene->applyAppearanceFirst(action );
  return false;
}

bool
GeomInline::applyAppearanceOnly( Action& action )
{
  if(__scene) return __scene->applyAppearanceOnly(action );
  return false;
}
