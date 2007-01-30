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



 

#include "revolution.h"
#include "curve.h"

#include <scenegraph/core/pgl_messages.h>
#include <tool/util_string.h>
#include <math/util_math.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


Revolution::Builder::Builder() :
  SOR::Builder(),
  Profile(0) {
}


Revolution::Builder::~Builder() {
  // nothing to do
}


SceneObjectPtr Revolution::Builder::build( ) const {
  if (isValid()) 
    return SceneObjectPtr
      (new Revolution(*Profile,
		      Slices ? *Slices : DEFAULT_SLICES));
  return SceneObjectPtr();
}


void Revolution::Builder::destroy() {
  SORDestroy();
  if (Profile) delete Profile;
}


bool Revolution::Builder::isValid( ) const {
  if (! SORValid()) return false;

  /*
    Profile field
  */
  // Initialization check
  if (! Profile) { 
    genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Revolution","Profile");
    return false;
  };
  // Size check
  uint32_t _pointListSize = (*Profile)->getStride();
  if (_pointListSize < 3) {
    genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Revolution","Profile","Must have more than 2 points.");
    return false;
  };
  return true;
}


/* ----------------------------------------------------------------------- */


Revolution::Revolution() :
  SOR(),
  __profile(0) {
}

Revolution::Revolution( const Curve2DPtr& profile,
			uchar_t slices ) :
  SOR(slices),
  __profile(profile) {
  GEOM_ASSERT(isValid());
}

Revolution::~Revolution( ) {
}

bool 
Revolution::apply( Action& action ) {    
  return action.process(this);
}

bool Revolution::isValid( ) const {
  Builder _builder;
  _builder.Profile = const_cast<Curve2DPtr *>(&__profile);
  _builder.Slices = const_cast<uchar_t *>(&__slices);
  return _builder.isValid();
}

SceneObjectPtr Revolution::copy() const {
  Revolution * ptr = new Revolution(*this);
  if(__profile)ptr->getProfile().cast(__profile->copy());
  return SceneObjectPtr(ptr);
}

/* ----------------------------------------------------------------------- */

const Curve2DPtr& 
Revolution::getProfile( ) const {
  return __profile;
}

Curve2DPtr& 
Revolution::getProfile( ) {
  return __profile;
}

bool
Revolution::isAVolume( ) const {
  if(!__profile)return false;
  return abs(__profile->getPointAt(__profile->getFirstKnot()).x() < REAL_EPSILON) &&
    abs(__profile->getPointAt(__profile->getLastKnot()).x() < REAL_EPSILON);
}
