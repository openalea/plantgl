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



#include "axisrotated.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


AxisRotation::Builder::Builder( ) :
  OrthonormalBasis3D::Builder(),
  Axis(0),
  Angle(0)
{}


AxisRotation::Builder::~Builder( )
{}

OrthonormalBasis3DPtr AxisRotation::Builder::build( ) const
{
  if (isValid())
    return OrthonormalBasis3DPtr
      (new AxisRotation(Axis ? *Axis : AxisRotated::DEFAULT_AXIS,
                        Angle ? *Angle : AxisRotated::DEFAULT_ANGLE ));
  return OrthonormalBasis3DPtr();
}


void AxisRotation::Builder::destroy( )
{
    if (Axis) { delete Axis; Axis= 0; }
    if (Angle){ delete Angle; Angle= 0;}
}


bool AxisRotation::Builder::isValid( ) const
{
  // Axis field
  if( Axis )
    {
    Axis->normalize( );
    }

  // Angle field
  if( Angle && (! pglfinite(*Angle)) )
    {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Axis Rotated","Angle","Must be Finite");
    return false;
    }

  return true;
}
/* ----------------------------------------------------------------------- */

AxisRotation::AxisRotation( const Vector3& axis,
                const real_t& angle ) :
  OrthonormalBasis3D(Matrix3::axisRotation(axis,angle)),
  __axis(axis),
  __angle(angle){
}

AxisRotation::~AxisRotation( ) {}

/* ----------------------------------------------------------------------- */


const Vector3 AxisRotated::DEFAULT_AXIS(0,0,1);
const real_t AxisRotated::DEFAULT_ANGLE(0);


/* ----------------------------------------------------------------------- */


AxisRotated::Builder::Builder( ) :
  OrthoTransformed::Builder(),
  Axis(0),
  Angle(0) {
}


AxisRotated::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr AxisRotated::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new AxisRotated(Axis ? *Axis : DEFAULT_AXIS,
                       Angle ? *Angle : DEFAULT_ANGLE,
                       *Geometry));
  return SceneObjectPtr();
}


void AxisRotated::Builder::destroy( ) {
  MTDestroy();
  if (Axis) delete Axis;
  if (Angle) delete Angle;
}


bool AxisRotated::Builder::isValid( ) const {
  if (! MTValid()) return false;

  AxisRotation::Builder _builder;
  _builder.Axis = Axis;
  _builder.Angle = Angle;
  return _builder.isValid();
}


/* ----------------------------------------------------------------------- */


AxisRotated::AxisRotated():
    OrthoTransformed(),
    __axis(DEFAULT_AXIS),
    __angle(DEFAULT_ANGLE) {
}

AxisRotated::AxisRotated( const Vector3& axis,
                          const real_t& angle,
                          const GeometryPtr& geometry ) :
  OrthoTransformed(geometry),
  __axis(axis),
  __angle(angle) {
  GEOM_ASSERT(isValid());
}

AxisRotated::~AxisRotated( ) {
}

bool AxisRotated::isValid( ) const {
  Builder _builder;
  _builder.Geometry = const_cast<GeometryPtr *>(&__geometry);
  _builder.Axis = const_cast<Vector3 *>(&__axis);
  _builder.Angle = const_cast<real_t *>(&__angle);
  return (_builder.isValid() && __geometry->isValid());
}

SceneObjectPtr AxisRotated::copy(DeepCopier& copier) const {
  AxisRotated * ptr = new AxisRotated(*this);
  copier.copy_object_attribute(ptr->getGeometry());
  return SceneObjectPtr(ptr);
}

/* ----------------------------------------------------------------------- */

const real_t
AxisRotated::getAngle( ) const {
  return __angle;
}

real_t&
AxisRotated::getAngle( ){
  return __angle;
}

const Vector3&
AxisRotated::getAxis( ) const {
  return __axis;
}

Vector3&
AxisRotated::getAxis( ){
  return __axis;
}

Transformation3DPtr
AxisRotated::getTransformation( ) const {
  return Transformation3DPtr
    (new AxisRotation(__axis,__angle));
}

OrthonormalBasis3DPtr
AxisRotated::getOrthonormalBasis( ) const {
  return OrthonormalBasis3DPtr
    (new AxisRotation(__axis,__angle));
}


bool
AxisRotated::isAngleToDefault( ) const {
  return fabs(__angle - DEFAULT_ANGLE) < GEOM_TOLERANCE;
}

bool
AxisRotated::isAxisToDefault( ) const {
  return __axis == DEFAULT_AXIS;
}

/* ----------------------------------------------------------------------- */
