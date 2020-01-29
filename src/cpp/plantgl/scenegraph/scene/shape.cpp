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
    id(),
    parentId(NOID){
        setDefaultId();
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
    if(id == NOID)setDefaultId();
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
    if(id == NOID)setDefaultId();
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
    if(id == NOID)setDefaultId();
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
    if(id == NOID)setDefaultId();
    setName(name);
}
Shape::~Shape() {
#ifdef GEOM_DEBUG
    cerr <<"Shape " <<  __name << " destroyed" << endl;
#endif
}

void Shape::setDefaultId()
{
    id = (uintptr_t)getObjectId();    
}


/* ----------------------------------------------------------------------- */

void Shape::setComputedName(){
    // Sets the label to the Geometry object
    if (geometry)
        if (! geometry->isNamed()) {
          string _name;
          if(id != NOID)
            _name = "GEOMID_"+number(id)+"_"+number(geometry->getObjectId());
          else
            _name = "GEOM_"+number(geometry->getObjectId());
          geometry->setName(_name);
        };

    // Sets the label to the Material object
    if (appearance)
      if (! appearance->isNamed()) {
        string _name;
        if(id != NOID)
          _name = "APPID_"+number(id)+"_"+number(appearance->getObjectId());
        else
          _name = "APP_"+number(appearance->getObjectId());
        appearance->setName(_name);
      }

    // Sets the label to the self
    if ( __name.empty() ) {
      if(id != NOID)
        __name = "SHAPEID_"+number(id)+"_"+number(getObjectId());
      else
        __name = "SHAPE_"+number(getObjectId());
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

uint_t Shape::getId() const {
  return id;
}

uint_t& Shape::getId(){
  return id;
}

#ifndef PGL_NO_DEPRECATED
size_t Shape::getSceneObjectId() const
{
    return (size_t)this;
}
#endif

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
