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





#include "oriented.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const Vector3 Oriented::DEFAULT_PRIMARY(Vector3::OX);
const Vector3 Oriented::DEFAULT_SECONDARY(Vector3::OY);


/* ----------------------------------------------------------------------- */


BaseOrientation::Builder::Builder( ) :
  OrthonormalBasis3D::Builder(),
  Primary(0),
  Secondary(0) {
}


BaseOrientation::Builder::~Builder( ) {
  // noting to do
}


OrthonormalBasis3DPtr BaseOrientation::Builder::build( ) const {
  if (isValid())
    return OrthonormalBasis3DPtr
      (new BaseOrientation(Primary ? *Primary : Oriented::DEFAULT_PRIMARY,
                    Secondary ? *Secondary : Oriented::DEFAULT_SECONDARY));
  return OrthonormalBasis3DPtr();
}


void BaseOrientation::Builder::destroy( ) {
  if (Primary) delete Primary; Primary= 0;
  if (Secondary) delete Secondary; Secondary= 0;
}


bool BaseOrientation::Builder::isValid( ) const
{
  // Primary field
  if( Primary && (! Primary->isNormalized()) )
    {
    if(*Primary != Vector3::ORIGIN)Primary->normalize();
    else {
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Primary","Must be normalized.");
	  return false;
	  }
	}

  // Secondary field
  if( Secondary && (! Secondary->isNormalized()) )
    {
    if(*Secondary != Vector3::ORIGIN)Secondary->normalize();
    else {
	  genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Secondary","Must be normalized.");
	  return false;
	  }
    }

  Vector3 s= Secondary ? *Secondary : Oriented::DEFAULT_SECONDARY;
  if( Primary )
    {
    if (! Primary->isOrthogonalTo(s))
      {
      if( fabs(dot(*Primary,s)) <  2*GEOM_EPSILON )
        {
        // genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Primary","Must be orthogonal to Secondary vector (Numerical Approximation Error).");
        }
      else
        {
        genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Primary","Must be orthogonal to Secondary vector.");
        return false;
        }
      }
    }
  else
  if( Secondary && (! Secondary->isOrthogonalTo(Oriented::DEFAULT_PRIMARY)) )
    {
    if(fabs(dot(*Secondary,Oriented::DEFAULT_PRIMARY)) < 2*GEOM_EPSILON)
      {
        // genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Secondary","Must be orthogonal to Primary vector (Numerical Approximation Error).");
      }
    else
      {
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Secondary","Must be orthogonal to Primary vector.");
      return false;
      }
    }

  return true;
}


/* ----------------------------------------------------------------------- */

BaseOrientation::BaseOrientation( const Vector3& primary,
		 const Vector3& secondary ) :
  OrthonormalBasis3D(Matrix3( primary, secondary,
			      direction(cross(primary,secondary)))),
  __primary(primary),
  __secondary(secondary){
}

BaseOrientation::~BaseOrientation( ) {
}

/* ----------------------------------------------------------------------- */


Oriented::Builder::Builder( ) :
  OrthoTransformed::Builder(),
  Primary(0),
  Secondary(0) {
}


Oriented::Builder::~Builder( ) {
  // noting to do
}


SceneObjectPtr Oriented::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new Oriented(Primary ? *Primary : DEFAULT_PRIMARY,
                    Secondary ? *Secondary : DEFAULT_SECONDARY,
                    *Geometry));
  return SceneObjectPtr();
}


void Oriented::Builder::destroy( ) {
  MTDestroy();
  if (Primary) delete Primary;
  if (Secondary) delete Secondary;
}


bool Oriented::Builder::isValid( ) const {
  if (! MTValid()) return false;

  BaseOrientation::Builder _builder;
  _builder.Primary = Primary;
  _builder.Secondary = Secondary;

  return _builder.isValid();
}


/* ----------------------------------------------------------------------- */

Oriented::Oriented() :
    OrthoTransformed(),
    __primary(DEFAULT_PRIMARY),
    __secondary(DEFAULT_SECONDARY) {
}

Oriented::Oriented( const Vector3& primary,
                    const Vector3& secondary,
                    const GeometryPtr& geometry ) :
  OrthoTransformed(geometry),
  __primary(primary),
  __secondary(secondary) {
  GEOM_ASSERT(isValid());
}

Oriented::~Oriented( ) {
}

bool 
Oriented::apply( Action& action ) {    
  return action.process(this);
}

const Vector3&
Oriented::getPrimary( ) const {
  return __primary;
}

Vector3&
Oriented::getPrimary( ){
  return __primary;
}

const Vector3&
Oriented::getSecondary( ) const {
  return __secondary;
}

Vector3&
Oriented::getSecondary( ) {
  return __secondary;
}

Transformation3DPtr
Oriented::getTransformation( ) const {
  return Transformation3DPtr
    (new BaseOrientation(__primary,__secondary));
}

OrthonormalBasis3DPtr
Oriented::getOrthonormalBasis() const {
  return OrthonormalBasis3DPtr
    (new BaseOrientation(__primary,__secondary));
}


bool
Oriented::isPrimaryToDefault( ) const {
  return __primary == DEFAULT_PRIMARY;
}

bool
Oriented::isSecondaryToDefault( ) const {
  return __secondary == DEFAULT_SECONDARY;
}


bool Oriented::isValid( ) const {
  Builder _builder;
  _builder.Geometry = const_cast<GeometryPtr *>(&__geometry);
  _builder.Primary = const_cast<Vector3 *>(&__primary);
  _builder.Secondary = const_cast<Vector3 *>(&__secondary);
  return (_builder.isValid() && __geometry->isValid());
}

SceneObjectPtr Oriented::copy() const {
  Oriented * ptr = new Oriented(*this);
  if(__geometry)ptr->getGeometry().cast(__geometry->copy());
  return SceneObjectPtr(ptr);
}



/* ----------------------------------------------------------------------- */

