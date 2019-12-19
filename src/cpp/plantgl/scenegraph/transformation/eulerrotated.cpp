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




#include "eulerrotated.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

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
  if( Azimuth && (! pglfinite(*Azimuth)) )
    {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Azimuth","Must be pglfinite.");
    return false;
    }

  if( Elevation && (! pglfinite(*Elevation)) )
    {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Elevation","Must be pglfinite.");
    return false;
    }

  if( Roll && (! pglfinite(*Roll)) )
    {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Roll","Must be pglfinite.");
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

  if (Azimuth && (! pglfinite(*Azimuth))) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Azimuth","Must be pglfinite.");
    return false;
  };

  if (Elevation && (! pglfinite(*Elevation))) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Elevation","Must be pglfinite.");
    return false;
  };

  if (Roll && (! pglfinite(*Roll))) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Euler Rotated","Roll","Must be pglfinite.");
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


bool EulerRotated::isValid( ) const {
  Builder _builder;
  _builder.Geometry = const_cast<GeometryPtr *>(&__geometry);
  _builder.Azimuth = const_cast<real_t *>(&__azimuth);
  _builder.Elevation = const_cast<real_t *>(&__elevation);
  _builder.Roll = const_cast<real_t *>(&__roll);
  return (_builder.isValid() && __geometry->isValid());
}

SceneObjectPtr EulerRotated::copy(DeepCopier& copier) const {
  EulerRotated * ptr = new EulerRotated(*this);
  copier.copy_object_attribute(ptr->getGeometry());
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
