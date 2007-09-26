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



#include "eulerrotated.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

EulerRotation::Builder::Builder( ) :
  OrthonormalBasis3D::Builder(),
  Azimuth(0),
  Elevation(0),
  Roll(0)
{}


EulerRotation::Builder::~Builder( )
{}

OrthonormalBasis3DPtr EulerRotation::Builder::build( ) const
{
  if (isValid())
    return OrthonormalBasis3DPtr
      (new EulerRotation(Azimuth ? *Azimuth : EulerRotated::DEFAULT_AZIMUTH,
                         Elevation ? *Elevation : EulerRotated::DEFAULT_ELEVATION,
                         Roll ? *Roll : EulerRotated::DEFAULT_ROLL ));
  return OrthonormalBasis3DPtr();
}


void EulerRotation::Builder::destroy( )
{
	if (Azimuth) { delete Azimuth; Azimuth= 0;}
	if (Elevation) {delete Elevation; Elevation= 0;}
	if (Roll)  {delete Roll; Roll= 0;}
}


bool EulerRotation::Builder::isValid( ) const
{
  if( Azimuth && (! finite(*Azimuth)) )
    {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Azimuth","Must be finite.");
    return false;
    }

  if( Elevation && (! finite(*Elevation)) )
    {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Elevation","Must be finite.");
    return false;
    }

  if( Roll && (! finite(*Roll)) )
    {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Roll","Must be finite.");
    return false;
    }

  return true;
}

/* ----------------------------------------------------------------------- */
EulerRotation::EulerRotation( const real_t& azimuth,
                              const real_t& elevation,
                              const real_t& roll ) :
  OrthonormalBasis3D(Matrix3::eulerRotationZYX(Vector3(azimuth,elevation,roll))),
  /// With turn azimuth deg around Z, elevation deg around Y,  roll deg around X.
  __azimuth(azimuth),
  __elevation(elevation),
  __roll(roll){
}

EulerRotation::~EulerRotation( ) {
}

Vector3
EulerRotation::extractEulerAngles() const
{
  return Vector3(__roll, __elevation, __azimuth );
}

/* ----------------------------------------------------------------------- */


const real_t EulerRotated::DEFAULT_AZIMUTH(0);
const real_t EulerRotated::DEFAULT_ELEVATION(0);
const real_t EulerRotated::DEFAULT_ROLL(0);


/* ----------------------------------------------------------------------- */


EulerRotated::Builder::Builder() :
  OrthoTransformed::Builder(),
  Azimuth(0),
  Elevation(0),
  Roll(0) {
}


EulerRotated::Builder::~Builder( ) {
}


SceneObjectPtr EulerRotated::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new EulerRotated(Azimuth ? *Azimuth : DEFAULT_AZIMUTH,
                        Elevation ? *Elevation : DEFAULT_ELEVATION,
                        Roll ? *Roll : DEFAULT_ROLL,
                        *Geometry));
  return SceneObjectPtr();
}


void EulerRotated::Builder::destroy( ) {
  MTDestroy();
  if (Azimuth) delete Azimuth;
  if (Elevation) delete Elevation;
  if (Roll) delete Roll;
}


bool EulerRotated::Builder::isValid( ) const {
  if (! MTValid()) return false;

  if (Azimuth && (! finite(*Azimuth))) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Azimuth","Must be finite.");
    return false;
  };

  if (Elevation && (! finite(*Elevation))) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Elevation","Must be finite.");
    return false;
  };

  if (Roll && (! finite(*Roll))) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Roll","Must be finite.");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */

EulerRotated::EulerRotated() :
      OrthoTransformed(),
      __azimuth(DEFAULT_AZIMUTH),
      __elevation(DEFAULT_ELEVATION),
      __roll(DEFAULT_ROLL) {
  }

EulerRotated::EulerRotated( const real_t& azimuth,
                            const real_t& elevation,
                            const real_t& roll,
                            const GeometryPtr& geometry ) :
  OrthoTransformed(geometry),
  __azimuth(azimuth),
  __elevation(elevation),
  __roll(roll) {
  GEOM_ASSERT(isValid());
}

EulerRotated::~EulerRotated( ) {
}

bool
EulerRotated::apply( Action& action ) {
  return action.process(this);
}

bool EulerRotated::isValid( ) const {
  Builder _builder;
  _builder.Geometry = const_cast<GeometryPtr *>(&__geometry);
  _builder.Azimuth = const_cast<real_t *>(&__azimuth);
  _builder.Elevation = const_cast<real_t *>(&__elevation);
  _builder.Roll = const_cast<real_t *>(&__roll);
  return (_builder.isValid() && __geometry->isValid());
}

SceneObjectPtr EulerRotated::copy() const {
  EulerRotated * ptr = new EulerRotated(*this);
  if(__geometry)ptr->getGeometry().cast(__geometry->copy());
  return SceneObjectPtr(ptr);
}

/* ----------------------------------------------------------------------- */

const real_t&
EulerRotated::getAzimuth() const {
  return __azimuth;
}

real_t&
EulerRotated::getAzimuth() {
  return __azimuth;
}

const real_t&
EulerRotated::getElevation() const {
  return __elevation;
}

real_t&
EulerRotated::getElevation(){
  return __elevation;
}

const real_t&
EulerRotated::getRoll() const {
  return __roll;
}

real_t&
EulerRotated::getRoll() {
  return __roll;
}

Transformation3DPtr
EulerRotated::getTransformation( ) const {
  return Transformation3DPtr
    (new EulerRotation(__azimuth,__elevation,__roll));
}

OrthonormalBasis3DPtr
EulerRotated::getOrthonormalBasis( ) const {
  return OrthonormalBasis3DPtr
    (new EulerRotation(__azimuth,__elevation,__roll));
}

bool
EulerRotated::isAzimuthToDefault( ) const {
  return fabs(__azimuth - DEFAULT_AZIMUTH) < GEOM_TOLERANCE;
}

bool
EulerRotated::isElevationToDefault( ) const {
  return fabs(__elevation - DEFAULT_ELEVATION) < GEOM_TOLERANCE;
}

bool
EulerRotated::isRollToDefault( ) const {
  return fabs(__roll - DEFAULT_ROLL) < GEOM_TOLERANCE;
}

/* ----------------------------------------------------------------------- */
