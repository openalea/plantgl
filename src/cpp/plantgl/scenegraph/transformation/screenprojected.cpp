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





#include "screenprojected.h"

#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

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
