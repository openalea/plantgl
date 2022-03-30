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






#include "group.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include "polyline.h"
PGL_USING_NAMESPACE

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

Group::Group( const iterator begin, const iterator end,
         const PolylinePtr& skeleton ):
  Geometry(),
  __geometryList(new GeometryArray(begin, end)),
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

bool Group::hasDynamicRendering() const {
  for (GeometryArray::const_iterator _i = __geometryList->begin();
       _i != __geometryList->end();
       _i++)
    if ((*_i)->hasDynamicRendering()) return true;
  return false;
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
