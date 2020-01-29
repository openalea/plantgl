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


#include "sceneobject.h"
#include "deepcopier.h"
#include <plantgl/tool/util_string.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

/*
std::ostream * SceneObject::commentStream =  & std::cerr ;
std::ostream * SceneObject::warningStream = & std::cerr ;
std::ostream * SceneObject::errorStream = & std::cerr ;
*/

/* ----------------------------------------------------------------------- */


SceneObject::Builder::Builder() {
  // nothing to do
}


SceneObject::Builder::~Builder() {
  // nothing to do
}


/* ----------------------------------------------------------------------- */


SceneObject::~SceneObject( ){
#ifdef GEOM_DEBUG
    if(!__name.empty())cerr << __name << " destroyed" << endl;
#endif
}

void SceneObject::setName( const std::string& name ) {
  __name = name;
}

void SceneObject::setDefaultName( ) {
  setName("OBJECT_"+number(getObjectId()));
}

size_t SceneObject::getObjectId( ) const {
  return (size_t)this;
}

const std::string&
SceneObject::getName( ) const {
  return __name;
}

bool
SceneObject::isNamed( ) const {
  return ! __name.empty();
}


/// Deep copy of \e this.
SceneObjectPtr SceneObject::deepcopy() const {
    DeepCopier map;
    return deepcopy(map);
}

SceneObjectPtr SceneObject::deepcopy(DeepCopier& objmap) const
{
  return objmap.copy(this);
}

/* ----------------------------------------------------------------------- */

