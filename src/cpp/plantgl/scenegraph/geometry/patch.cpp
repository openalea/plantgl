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






#include "patch.h"
#include "mesh.h"

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

const bool Patch::DEFAULT_CCW(Mesh::DEFAULT_CCW);

/* ----------------------------------------------------------------------- */


Patch::Builder::Builder() :
    ParametricModel::Builder(),
    CCW(0){
}


Patch::Builder::~Builder() {
  // nothing to do
}


/* ----------------------------------------------------------------------- */


Patch::Patch(bool ccw) :
  ParametricModel(),
    __ccw(ccw){
}

Patch::~Patch( ) {
}

/* ----------------------------------------------------------------------- */

bool
Patch::isACurve( ) const {
  return false;
}

bool
Patch::isASurface( ) const {
  return true;
}

bool
Patch::isAVolume( ) const {
  return false;
}

const bool
Patch::getCCW( ) const {
  return __ccw;
}

bool&
Patch::getCCW( ) {
  return __ccw;
}

bool
Patch::isCCWToDefault( ) const {
  return (__ccw==DEFAULT_CCW);
}
