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





#include "oriented.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

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
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Primary","Must be normalized.");
      return false;
      }
    }

  // Secondary field
  if( Secondary && (! Secondary->isNormalized()) )
    {
    if(*Secondary != Vector3::ORIGIN)Secondary->normalize();
    else {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Secondary","Must be normalized.");
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
        // pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Primary","Must be orthogonal to Secondary vector (Numerical Approximation Error).");
        }
      else
        {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Primary","Must be orthogonal to Secondary vector.");
        return false;
        }
      }
    }
  else
  if( Secondary && (! Secondary->isOrthogonalTo(Oriented::DEFAULT_PRIMARY)) )
    {
    if(fabs(dot(*Secondary,Oriented::DEFAULT_PRIMARY)) < 2*GEOM_EPSILON)
      {
        // pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Secondary","Must be orthogonal to Primary vector (Numerical Approximation Error).");
      }
    else
      {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Oriented","Secondary","Must be orthogonal to Primary vector.");
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

SceneObjectPtr Oriented::copy(DeepCopier& copier) const {
  Oriented * ptr = new Oriented(*this);
  copier.copy_object_attribute(ptr->getGeometry());
  return SceneObjectPtr(ptr);
}



/* ----------------------------------------------------------------------- */

