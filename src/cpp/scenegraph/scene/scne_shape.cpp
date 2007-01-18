/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (boudon@cirad.fr) nouguier
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

#include "scne_shape.h"
#include "geom_geometry.h"
#include "appe_appearance.h"

#include "actn_printer.h"
#include "util_messages.h"

#include "Tools/util_string.h"

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

/// The undef value for the Id field.
const uint32_t GeomShape::NOID = 0;

/* ----------------------------------------------------------------------- */

GeomShape3D::Builder::Builder( ) :
    SceneObject::Builder(){
};

GeomShape3D::Builder::~Builder( ){
}

/* ----------------------------------------------------------------------- */

GeomShape3D::GeomShape3D( ) :
  SceneObject(){
}

GeomShape3D::~GeomShape3D() {
}

/* ----------------------------------------------------------------------- */

GeomShape::Builder::Builder( ) :
    GeomShape3D::Builder(),
    Appearance(0),
    Geometry(0),
    Id(0){
}


GeomShape::Builder::~Builder( ) {
  /// Nothng To do.
}

SceneObjectPtr GeomShape::Builder::build( ) {
  if (isValid())
    return SceneObjectPtr
      (new GeomShape(*Geometry,(Appearance ? *Appearance : AppearancePtr()),
                     Id ? *Id : NOID));
  return SceneObjectPtr();
}

void GeomShape::Builder::destroy() {
  if (Appearance) delete Appearance;
  if (Geometry) delete Geometry;
  if (Id) delete Id;
}

bool GeomShape::Builder::isValid( ) const{
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

GeomShape::GeomShape( ) :
    GeomShape3D(),
    appearance(0),
    geometry(0),
    id(NOID){
}

GeomShape::GeomShape( const GeometryPtr& _geom,
                      const AppearancePtr& _app,
                      uint32_t _id) :
    GeomShape3D(),
    appearance(_app),
    geometry(_geom),
    id(_id) {
    setComputedName();
}

GeomShape::GeomShape( const string& name,
					  const GeometryPtr& _geom,
                      const AppearancePtr& _app,
                      uint32_t _id) :
    GeomShape3D(),
    appearance(_app),
    geometry(_geom),
    id(_id) {
    setName(name);
}

GeomShape::~GeomShape() {
#ifdef GEOM_DEBUG
    cerr <<"GeomShape " <<  __name << " destroyed" << endl;
#endif
}

bool GeomShape::apply( Action& action ){
  return action.process(this);
}

/* ----------------------------------------------------------------------- */

void GeomShape::setComputedName(){
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

SceneObjectPtr GeomShape::copy() const
{
  GeomShape * ptr = new GeomShape(*this);
  if(geometry)ptr->getGeometry().cast(geometry->copy());
  if(appearance)ptr->getAppearance().cast(appearance->copy());
  return SceneObjectPtr(ptr);
}


/* ----------------------------------------------------------------------- */
bool GeomShape::applyGeometryFirst( Action& action ){
  if(!geometry->apply(action )) return false;
  else if(appearance)return appearance->apply(action );
  else return false;
}

bool GeomShape::applyGeometryOnly( Action& action ){
  return geometry->apply(action );
}

bool GeomShape::applyAppearanceFirst( Action& action ){
  if(appearance){
    if(!appearance->apply(action )) return false;
    else return geometry->apply(action );
  }
  else return geometry->apply(action );
}

bool GeomShape::applyAppearanceOnly( Action& action ){
  if(appearance)return appearance->apply(action );
  else return false;
}

const GeometryPtr& GeomShape::getGeometry() const{
  return geometry;
}

GeometryPtr& GeomShape::getGeometry(){
  return geometry;
}

const AppearancePtr& GeomShape::getAppearance() const{
  return appearance;
}

AppearancePtr& GeomShape::getAppearance(){
  return appearance;
}

uint32_t GeomShape::getId() const {
  return id;
}

uint32_t& GeomShape::getId(){
  return id;
}

/* ----------------------------------------------------------------------- */

ostream& operator<<( ostream& stream, GeomShape3D& s ){
    Printer p(stream,stream,stream);
    s.apply(p);
    return stream;
}

/* ----------------------------------------------------------------------- */

bool GeomShape::isValid( ) const {
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
