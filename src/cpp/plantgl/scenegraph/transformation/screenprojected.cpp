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




#include "screenprojected.h"

#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

bool ScreenProjected::DEFAULT_KEEPASPECTRATIO (true);

/* ----------------------------------------------------------------------- */


ScreenProjected::Builder::Builder() :
  Transformed::Builder(),
  Geometry(0) {
}


ScreenProjected::Builder::~Builder() {
}


void ScreenProjected::Builder::destroy() {
  if (Geometry) delete Geometry;
  if (KeepAspectRatio) delete KeepAspectRatio;
}



bool ScreenProjected::Builder::isValid( ) const {
  if (! Geometry) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"ScreenProjected","Geometry");
    return false;
  };
  if (! (*Geometry) ) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"ScreenProjected","Geometry","Must be a valid Geometry Object.");
    return false;
  };
  return true;
}

SceneObjectPtr ScreenProjected::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new ScreenProjected(*Geometry, (KeepAspectRatio?*KeepAspectRatio:ScreenProjected::DEFAULT_KEEPASPECTRATIO)));
  return SceneObjectPtr();
}

/* ----------------------------------------------------------------------- */


ScreenProjected::ScreenProjected( const GeometryPtr& geometry, bool keepAspectRatio ) :
  Transformed(),
  __geometry(geometry),
  __keepAspectRatio(keepAspectRatio){
}

ScreenProjected::ScreenProjected( ) :
  Transformed(),
  __geometry(),
  __keepAspectRatio(DEFAULT_KEEPASPECTRATIO){
}

ScreenProjected::~ScreenProjected( ) {
}

SceneObjectPtr ScreenProjected::copy(DeepCopier& copier) const {
  ScreenProjected * ptr = new ScreenProjected(*this);
  copier.copy_object_attribute(ptr->getGeometry());
  return SceneObjectPtr(ptr);
}

bool ScreenProjected::isValid( ) const {
  return __geometry->isValid();
}

/* ----------------------------------------------------------------------- */
