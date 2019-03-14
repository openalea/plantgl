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

#include "sceneobject.h"
#include "deepcopier.h"
#include <plantgl/tool/util_string.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

/*
std::ostream * SceneObject::commentStream =  & std::cerr ;
std::ostream * SceneObject::warningStream = & std::cerr ;
std::ostream * SceneObject::errorStream = & std::cerr ;
*/

/* ----------------------------------------------------------------------- */


SceneObject::Builder::Builder() {
  // nothing to do
}


SceneObject::Builder::~Builder() {
  // nothing to do
}


/* ----------------------------------------------------------------------- */


SceneObject::~SceneObject( ){
#ifdef GEOM_DEBUG
    if(!__name.empty())cerr << __name << " destroyed" << endl;
#endif
}

void SceneObject::setName( const std::string& name ) {
  __name = name;
}

void SceneObject::setDefaultName( ) {
  setName("OBJECT_"+number(getId()));
}

size_t SceneObject::getId( ) const {
  return (size_t)this;
}

const std::string&
SceneObject::getName( ) const {
  return __name;
}

bool
SceneObject::isNamed( ) const {
  return ! __name.empty();
}


/// Deep copy of \e this.
SceneObjectPtr SceneObject::deepcopy() const {
	DeepCopier map;
	return deepcopy(map);
}

SceneObjectPtr SceneObject::deepcopy(DeepCopier& objmap) const 
{
  return objmap.copy(this);
}

/* ----------------------------------------------------------------------- */

