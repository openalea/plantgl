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






#include "sor.h"
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const uchar_t SOR::DEFAULT_SLICES(8);


/* ----------------------------------------------------------------------- */


SOR::Builder::Builder() :
  ParametricModel::Builder(),
  Slices(0) {
}


SOR::Builder::~Builder() {
  // nothing to do
}


void SOR::Builder::destroy( ) {
  SORDestroy( );
}

void SOR::Builder::SORDestroy( ) {
  if (Slices) delete Slices;
}


bool SOR::Builder::isValid( ) const {
  return SORValid( );
}

bool SOR::Builder::SORValid( ) const {
  if (Slices && (*Slices < 4)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Surface Of Revolution","Slices","Must be greater than 3.");
    return false;
  };
  return true;
}


/* ----------------------------------------------------------------------- */


SOR::SOR( uchar_t slices ) :
  ParametricModel(),
  __slices(slices) {
}

SOR::~SOR( ) {
}

const uchar_t&
SOR::getSlices( ) const {
  return __slices;
}

uchar_t&
SOR::getSlices( ){
  return __slices;
}

bool
SOR::isACurve( ) const {
  return false;
}

bool
SOR::isASurface( ) const {
  return true;
}

bool
SOR::isSlicesToDefault( ) const {
  return __slices == DEFAULT_SLICES;
}

/* ----------------------------------------------------------------------- */


SOR2D::Builder::Builder() :
  PlanarModel::Builder(),
  Slices(0) {
}


SOR2D::Builder::~Builder() {
  // nothing to do
}


void SOR2D::Builder::destroy( ) {
    SOR2DDestroy( );
}

void SOR2D::Builder::SOR2DDestroy( ) {
  if (Slices) delete Slices;
}


bool SOR2D::Builder::isValid( ) const {
    return SOR2DValid( );
}

bool SOR2D::Builder::SOR2DValid( ) const {
  if (Slices && (*Slices < 4)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Surface Of Revolution 2D","Slices","Must be greater than 3.");
    return false;
  };
  return true;
}


/* ----------------------------------------------------------------------- */


SOR2D::SOR2D( uchar_t slices ) :
  PlanarModel(),
  __slices(slices) {
}

 SOR2D::~SOR2D( ) {
  }

const uchar_t&
SOR2D::getSlices( ) const {
  return __slices;
}

uchar_t&
SOR2D::getSlices( ) {
  return __slices;
}

bool
SOR2D::isACurve( ) const {
  return false;
}

bool
SOR2D::isASurface( ) const {
  return true;
}

bool
SOR2D::isSlicesToDefault( ) const {
  return __slices == SOR::DEFAULT_SLICES;
}

/* ----------------------------------------------------------------------- */

