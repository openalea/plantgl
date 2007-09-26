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

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

/// The undef value for the Id field.
const uint32_t Shape::NOID = 0;

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
    Id(0){
}


Shape::Builder::~Builder( ) {
  /// Nothng To do.
}

SceneObjectPtr Shape::Builder::build( ) {
  if (isValid())
    return SceneObjectPtr
      (new Shape(*Geometry,(Appearance ? *Appearance : AppearancePtr()),
                     Id ? *Id : NOID));
  return SceneObjectPtr();
}

void Shape::Builder::destroy() {
  if (Appearance) delete Appearance;
  if (Geometry) delete Geometry;
  if (Id) delete Id;
}

bool Shape::Builder::isValid( ) const{
  if (! (Geometry)){
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Shape","Geometry","Must be not null.");
	return false;
  }
  if (! (*Geometry)){
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Shape","Geometry","Must be not null.");
	return false;
  }
  return true;
}

/* ----------------------------------------------------------------------- */

Shape::Shape( ) :
    Shape3D(),
    appearance(0),
    geometry(0),
    id(NOID){
}

Shape::Shape( const GeometryPtr& _geom,
                      const AppearancePtr& _app,
                      uint32_t _id) :
    Shape3D(),
    appearance(_app),
    geometry(_geom),
    id(_id) {
    setComputedName();
}

Shape::Shape( const string& name,
					  const GeometryPtr& _geom,
                      const AppearancePtr& _app,
                      uint32_t _id) :
    Shape3D(),
    appearance(_app),
    geometry(_geom),
    id(_id) {
    setName(name);
}

Shape::~Shape() {
#ifdef GEOM_DEBUG
    cerr <<"Shape " <<  __name << " destroyed" << endl;
#endif
}

bool Shape::apply( Action& action ){
  return action.process(this);
}

/* ----------------------------------------------------------------------- */

void Shape::setComputedName(){
    // Sets the label to the Geometry object
    if (geometry)
        if (! geometry->isNamed()) {
          string _name;
          if(id)
            _name = "GEOMID_"+number(id);
          else
            _name = "GEOM_"+number(geometry->getId());
          geometry->setName(_name);
        };

    // Sets the label to the Material object
    if (appearance)
      if (! appearance->isNamed()) {
        string _name;
        if(id)
          _name = "APPID_"+number(id);
        else
          _name = "APP_"+number(appearance->getId());
        appearance->setName(_name);
      }

    // Sets the label to the self
    if ( __name.empty() ) {
      if(id)
        __name = "ID_"+number(id);
      else if (geometry)
        __name = "SHAPE_"+number(geometry->getId());
      else
        __name = "SHAPE_"+number(SceneObject::getId());
    };
}

SceneObjectPtr Shape::copy() const
{
  Shape * ptr = new Shape(*this);
  if(geometry)ptr->getGeometry().cast(geometry->copy());
  if(appearance)ptr->getAppearance().cast(appearance->copy());
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

uint32_t Shape::getId() const {
  return id;
}

uint32_t& Shape::getId(){
  return id;
}

/* ----------------------------------------------------------------------- */

bool Shape::isValid( ) const {
  if (! (geometry)){
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Shape","Geometry","Must be not null.");
	return false;
  }
  if (! (geometry->isValid())) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Shape","Geometry","Must be valid.");
	return false;
  }
  if ((appearance.isValid()) && (! appearance->isValid())) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Shape","Appearance","Must be valid.");
	return false;
  }
  return true;
}
