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



#include "extrudedhull.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include "curve.h"
PGL_USING_NAMESPACE


/* ----------------------------------------------------------------------- */


const bool ExtrudedHull::DEFAULT_CCW(true);

/* ----------------------------------------------------------------------- */


ExtrudedHull::Builder::Builder( ) :
  Hull::Builder(),
  CCW(0),
  Horizontal(0),
  Vertical(0) {
}


ExtrudedHull::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr ExtrudedHull::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new ExtrudedHull(*Vertical,
            *Horizontal,
//          Orientation ? *Orientation : DEFAULT_ORIENTATION,
            CCW ? *CCW : DEFAULT_CCW));
  // Returns null as self is not valid.
  return SceneObjectPtr();
}


void ExtrudedHull::Builder::destroy( ) {
  if (CCW) delete CCW;
  if (Horizontal) delete Horizontal;
//  if (Orientation) delete Orientation;
  if (Vertical) delete Vertical;
}


bool ExtrudedHull::Builder::isValid( ) const {

  // Vertical
  if (! Vertical) {
      pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Extruded Hull","Vertical");
      return false;
    };

  // Vertical
  if (! (*Vertical)->isValid()) {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Extruded Hull","Vertical","Must be a valid Object");
      return false;
    };

  // Horizontal
  if (! Horizontal) {
      pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Extruded Hull","Horizontal");
      return false;
    };

  // Horizontal
  if (! (*Horizontal)->isValid()) {
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Extruded Hull","Horizontal","Must be a valid Object");
      return false;
    };

  return true;
}


/* ----------------------------------------------------------------------- */


ExtrudedHull::ExtrudedHull() :
  Hull(),
  __vertical(),
  __horizontal(),
  __ccw(DEFAULT_CCW) {
}

ExtrudedHull::ExtrudedHull( const Curve2DPtr& vertical,
                const Curve2DPtr& horizontal,
                bool ccw ) :
  Hull(),
  __vertical(vertical),
  __horizontal(horizontal),
  __ccw(ccw) {
  GEOM_ASSERT(isValid());
}

ExtrudedHull::~ExtrudedHull( ) {
}

bool ExtrudedHull::isValid( ) const {
  Builder _builder;
  _builder.Horizontal = const_cast<Curve2DPtr *>(&__horizontal);
  _builder.Vertical = const_cast<Curve2DPtr *>(&__vertical);
  return _builder.isValid();
}

SceneObjectPtr ExtrudedHull::copy(DeepCopier& copier) const
{
  ExtrudedHull * ptr = new ExtrudedHull(*this);
  copier.copy_object_attribute(ptr->getHorizontal());
  copier.copy_object_attribute(ptr->getVertical());
  return SceneObjectPtr(ptr);
}

/* ----------------------------------------------------------------------- */

const bool
ExtrudedHull::getCCW( ) const {
  return __ccw;
}

bool&
ExtrudedHull::getCCW( ){
  return __ccw;
}

const Curve2DPtr&
ExtrudedHull::getHorizontal( ) const {
  return __horizontal;
}

Curve2DPtr&
ExtrudedHull::getHorizontal( ) {
  return __horizontal;
}

const Curve2DPtr&
ExtrudedHull::getVertical( ) const {
  return __vertical;
}

Curve2DPtr&
ExtrudedHull::getVertical( ) {
  return __vertical;
}

bool
ExtrudedHull::isCCWToDefault( ) const {
  return __ccw == DEFAULT_CCW;
}
/* ----------------------------------------------------------------------- */

