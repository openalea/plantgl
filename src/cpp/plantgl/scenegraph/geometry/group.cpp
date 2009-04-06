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





#include "group.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include "polyline.h"
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const PolylinePtr Group::DEFAULT_SKELETON;


/* ----------------------------------------------------------------------- */


Group::Builder::Builder() :
  Geometry::Builder(),
  GeometryList(0),
  Skeleton(0) {
}


Group::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr Group::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr(new Group(*GeometryList,
				 Skeleton ? *Skeleton : DEFAULT_SKELETON));
  return SceneObjectPtr();
}


void Group::Builder::destroy() {
  if (GeometryList) delete GeometryList;
  if (Skeleton) delete Skeleton;
}


bool Group::Builder::isValid( ) const {

  // GeometryList field
  if (! GeometryList) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Group","GeometryList");
    return false;
  };
  uint_t _size = (*GeometryList)->size();
  if (_size < 1) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Group","GeometryList","Must be greater than 1.");
    return false;
  };
  for (uint_t _i = 0; _i < _size; _i++) {
    const GeometryPtr& _geometry = (*GeometryList)->getAt(_i);
    if (! (_geometry) ) {
      pglErrorEx
	  (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Group","GeometryList",number(_i + 1).c_str(),"Must not be a null Geometry Object.");
      return false;
    };	
    if (!_geometry->isValid() ) {
      pglErrorEx
	  (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Group","GeometryList",number(_i + 1).c_str(),"Must be a valid Geometry Object.");
      return false;
    };	
 };
  
  return true;
}


/* ----------------------------------------------------------------------- */


Group::Group() :
  Geometry(),
  __geometryList(),
  __skeleton(DEFAULT_SKELETON) {
}

Group::Group( const GeometryArrayPtr& geometryList,
              const PolylinePtr& skeleton ) :
  Geometry(),
  __geometryList(geometryList),
  __skeleton(skeleton) {
  GEOM_ASSERT(isValid());
}

Group::~Group( ) {
}

/* ----------------------------------------------------------------------- */

const GeometryPtr&
Group::getGeometryListAt( uint_t i ) const {
  GEOM_ASSERT(i < __geometryList->size());
  return __geometryList->getAt(i);
}

GeometryPtr& 
Group::getGeometryListAt( uint_t i ) {
  GEOM_ASSERT(i < __geometryList->size());
  return __geometryList->getAt(i);
}

const GeometryArrayPtr& 
Group::getGeometryList( ) const {
  return __geometryList;
}
GeometryArrayPtr& 
Group::getGeometryList( ) {
  return __geometryList;
}

uint_t 
Group::getGeometryListSize( ) const {
  return __geometryList->size();
}

/* ----------------------------------------------------------------------- */

const PolylinePtr& 
Group::getSkeleton( ) const {
  return __skeleton;
}

PolylinePtr& 
Group::getSkeleton( ){
  return __skeleton;
}

bool 
Group::isSkeletonToDefault( ) const {
  return __skeleton == DEFAULT_SKELETON;
}

/* ----------------------------------------------------------------------- */

bool Group::isACurve( ) const {
  for (GeometryArray::const_iterator _i = __geometryList->begin();
       _i != __geometryList->end();
       _i++)
    if (! (*_i)->isACurve()) return false;
  return true;
}


bool Group::isASurface( ) const {
  for (GeometryArray::const_iterator _i = __geometryList->begin();
       _i != __geometryList->end();
       _i++)
    if (! (*_i)->isASurface()) return false;
  return true;
}


bool Group::isAVolume( ) const {
  for (GeometryArray::const_iterator _i = __geometryList->begin();
       _i != __geometryList->end();
       _i++)
    if (! (*_i)->isAVolume()) return false;
  return true;
}


bool Group::isExplicit( ) const {
  for (GeometryArray::const_iterator _i = __geometryList->begin();
       _i != __geometryList->end();
       _i++)
    if (! (*_i)->isExplicit()) return false;
  return true;
}

/* ----------------------------------------------------------------------- */

bool Group::isValid( ) const {
  Builder _builder;
  _builder.GeometryList = const_cast<GeometryArrayPtr *>(&__geometryList);
  _builder.Skeleton = const_cast<PolylinePtr *>(&__skeleton);
  if(!_builder.isValid())return false;
  uint_t _size = __geometryList->size();
  for (uint_t _i = 0; _i < _size ; _i++) {
      const GeometryPtr& _geometry = __geometryList->getAt(_i);
      if (! (_geometry) && (_geometry->isValid())) {
	  pglErrorEx
	      (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Group","GeometryList",number(_i + 1).c_str(),"Must be a valid Geometry Object.");
	  return false;
      };	
  };
  return true;
}

/* ----------------------------------------------------------------------- */

SceneObjectPtr Group::copy(DeepCopier& copier) const 
{
  Group * ptr = new Group(*this);
  copier.copy_recursive_object_attribute(ptr->getGeometryList());
  copier.copy_object_attribute(ptr->getSkeleton());
  return SceneObjectPtr(ptr);
}


/* ----------------------------------------------------------------------- */
