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



#include "deformed.h"
#include <plantgl/scenegraph/geometry/primitive.h>
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

Deformation::Deformation( ) :
  Transformation3D() {
}

Deformation::~Deformation( ) {
}

/* ----------------------------------------------------------------------- */


/* ----------------------------------------------------------------------- */


Deformed::Builder::Builder() :
  Transformed::Builder(),
  Primitive(0) {
}


void Deformed::Builder::destroy() {
    DefDestroy();
}

void Deformed::Builder::DefDestroy() {
  if (Primitive) delete Primitive;
}


bool Deformed::Builder::isValid( ) const {
    return DefValid( ) ;
}

bool Deformed::Builder::DefValid( ) const {
  if (! Primitive) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Deformed","Primitive");
    return false;
  };
  if (! (*Primitive)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Deformed","Primitive","Must be a valid Primitive.");
    return false;
  };
  return true;
}


/* ----------------------------------------------------------------------- */


Deformed::Deformed() :
  Transformed(),
  __primitive() {
}

Deformed::Deformed( const PrimitivePtr& primitive ) :
  Transformed(),
  __primitive(primitive) {
}

Deformed::~Deformed( ) {
}

const GeometryPtr
Deformed::getGeometry( ) const {
  return GeometryPtr(__primitive);
}

const PrimitivePtr&
Deformed::getPrimitive( ) const {
  return __primitive;
}

PrimitivePtr&
Deformed::getPrimitive( ){
  return __primitive;
}

bool
Deformed::isACurve( ) const {
  return __primitive->isACurve();
}

bool
Deformed::isASurface( ) const {
  return __primitive->isASurface();
}

bool
Deformed::isAVolume( ) const {
  return __primitive->isAVolume();
}

bool
Deformed::isExplicit( ) const {
  return __primitive->isExplicit();
}

/* ----------------------------------------------------------------------- */

