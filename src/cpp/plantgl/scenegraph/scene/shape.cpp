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

#include "shape.h"
#include <plantgl/scenegraph/core/pgl_messages.h>

#include <plantgl/tool/util_string.h>
#include <plantgl/tool/util_types.h>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

/// The undef value for the Id field.
const uint_t Shape::NOID = UINT32_MAX;

/* ----------------------------------------------------------------------- */

Shape3D::Builder::Builder( ) :
    SceneObject::Builder(){
};

Shape3D::Builder::~Builder( ){
}

/* ----------------------------------------------------------------------- */

Shape3D::Shape3D( ) :
  SceneObject(){
}

Shape3D::~Shape3D() {
}

/* ----------------------------------------------------------------------- */

Shape::Builder::Builder( ) :
    Shape3D::Builder(),
    Appearance(0),
    Geometry(0),
    Id(0),
    ParentId(0){
}


Shape::Builder::~Builder( ) {
  /// Nothng To do.
}

SceneObjectPtr Shape::Builder::build( ) {
  if (isValid())
    return SceneObjectPtr
      (new Shape(*Geometry,(Appearance ? *Appearance : AppearancePtr()),
                     Id ? *Id : NOID, ParentId ? *ParentId : NOID));
  return SceneObjectPtr();
}

void Shape::Builder::destroy() {
  if (Appearance) delete Appearance;
  if (Geometry) delete Geometry;
  if (Id) delete Id;
  if (ParentId) delete ParentId;
}

bool Shape::Builder::isValid( ) const{
  if (! (Geometry)){
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Shape","Geometry","Must be not null.");
	return false;
  }
  if (! (*Geometry)){
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Shape","Geometry","Must be not null.");
	return false;
  }
  return true;
}

/* ----------------------------------------------------------------------- */

Shape::Shape( ) :
    Shape3D(),
    appearance(),
    geometry(),
    id(getSceneObjectId()),
    parentId(NOID){
}

Shape::Shape( const GeometryPtr& _geom,
                      const AppearancePtr& _app,
                      uint_t _id,
                      uint_t _parentId) :
    Shape3D(),
    appearance(_app),
    geometry(_geom),
    id(_id),
    parentId(_parentId){
    if(id == NOID)id = getSceneObjectId();
    setComputedName();
}

Shape::Shape( const string& name,
					  const GeometryPtr& _geom,
                      const AppearancePtr& _app,
                      uint_t _id,
                      uint_t _parentId) :
    Shape3D(),
    appearance(_app),
    geometry(_geom),
    id(_id),
    parentId(_parentId) {
    if(id == NOID)id = getSceneObjectId();
    setName(name);
}

Shape::Shape( const GeometryPtr& _geom,
                      const ImageTexturePtr& _app,
                      uint_t _id,
                      uint_t _parentId) :
    Shape3D(),
    appearance(new Texture2D(_app)),
    geometry(_geom),
    id(_id),
    parentId(_parentId){
    if(id == NOID)id = getSceneObjectId();
    setComputedName();
}

Shape::Shape( const string& name,
					  const GeometryPtr& _geom,
                      const ImageTexturePtr& _app,
                      uint_t _id,
                      uint_t _parentId) :
    Shape3D(),
    appearance(new Texture2D(_app)),
    geometry(_geom),
    id(_id),
    parentId(_parentId) {
    if(id == NOID)id = getSceneObjectId();
    setName(name);
}
Shape::~Shape() {
#ifdef GEOM_DEBUG
    cerr <<"Shape " <<  __name << " destroyed" << endl;
#endif
}

/* ----------------------------------------------------------------------- */

void Shape::setComputedName(){
    // Sets the label to the Geometry object
    if (geometry)
        if (! geometry->isNamed()) {
          string _name;
          if(id != NOID)
            _name = "GEOMID_"+number(id)+"_"+number(geometry->getId());
          else
            _name = "GEOM_"+number(geometry->getId());
          geometry->setName(_name);
        };

    // Sets the label to the Material object
    if (appearance)
      if (! appearance->isNamed()) {
        string _name;
        if(id != NOID)
          _name = "APPID_"+number(id)+"_"+number(appearance->getId());
        else
          _name = "APP_"+number(appearance->getId());
        appearance->setName(_name);
      }

    // Sets the label to the self
    if ( __name.empty() ) {
      if(id != NOID)
        __name = "SHAPEID_"+number(id)+"_"+number(SceneObject::getId());
      else
        __name = "SHAPE_"+number(SceneObject::getId());
    };
}

SceneObjectPtr Shape::copy(DeepCopier& copier) const
{
  Shape * ptr = new Shape(*this);
  copier.copy_object_attribute(ptr->getGeometry());
  copier.copy_object_attribute(ptr->getAppearance());
  return SceneObjectPtr(ptr);
}


/* ----------------------------------------------------------------------- */
bool Shape::applyGeometryFirst( Action& action ){
  if(!geometry->apply(action )) return false;
  else if(appearance)return appearance->apply(action );
  else return false;
}

bool Shape::applyGeometryOnly( Action& action ){
  return geometry->apply(action );
}

bool Shape::applyAppearanceFirst( Action& action ){
  if(appearance){
    if(!appearance->apply(action )) return false;
    else return geometry->apply(action );
  }
  else return geometry->apply(action );
}

bool Shape::applyAppearanceOnly( Action& action ){
  if(appearance)return appearance->apply(action );
  else return false;
}

const GeometryPtr& Shape::getGeometry() const{
  return geometry;
}

GeometryPtr& Shape::getGeometry(){
  return geometry;
}

const AppearancePtr& Shape::getAppearance() const{
  return appearance;
}

AppearancePtr& Shape::getAppearance(){
  return appearance;
}

size_t Shape::getId() const {
  return id;
}

uint_t& Shape::getId(){
  return id;
}

size_t Shape::getSceneObjectId() const
{
    return (size_t)this;
}

/* ----------------------------------------------------------------------- */

bool Shape::isValid( ) const {
  if (! (geometry)){
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Shape","Geometry","Must be not null.");
	return false;
  }
  if (! (geometry->isValid())) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Shape","Geometry","Must be valid.");
	return false;
  }
  if ((appearance) && (! appearance->isValid())) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Shape","Appearance","Must be valid.");
	return false;
  }
  return true;
}
