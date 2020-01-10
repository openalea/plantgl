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


#include "inline.h"
#include "scene.h"
#include <plantgl/scenegraph/transformation/translated.h>

#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/dirnames.h>

#include <fstream>

PGL_USING_NAMESPACE
using namespace std;


/* ----------------------------------------------------------------------- */

// The default translation value
const Vector3 Inline::DEFAULT_TRANSLATION(0,0,0);

// The default scaling value
const Vector3 Inline::DEFAULT_SCALE(1,1,1);

/* ----------------------------------------------------------------------- */

Inline::Builder::Builder( ) :
    Shape3D::Builder(),
    FileName(0),
    Scene(0),
    Translation(0),
    Scale(0){
}

Inline::Builder::~Builder( ){
}

SceneObjectPtr
Inline::Builder::build( ){
  if (isValid()){
    if(FileName){
        Inline * g = new Inline(absolute_filename(*FileName),
                                (Translation?*Translation:DEFAULT_TRANSLATION),
                                (Scale?*Scale:DEFAULT_SCALE));
      return SceneObjectPtr(g);
    }
  }
  return SceneObjectPtr();
}

void
Inline::Builder::destroy( ){
    if(FileName) delete FileName;
    if(Scene) delete Scene;
    if(Translation) delete Translation;
    if(Scale) delete Scale;
}


bool Inline::Builder::isValid( ) const {
  if (! FileName && ! Scene) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Inline","FileName");
    return false;
  };
  if(FileName){
      ifstream _file(FileName->c_str());
      if (!_file){
          string _mess = "Cannot open " + *FileName;
          pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","FileName",_mess.c_str());
          return false;
      }
  }

  if(Scene && !(*Scene)->isValid()){
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","Scene","Not Valid");
      return false;
  }

  if(Translation && !Translation->isValid()){
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","Translation","Not Valid");
      return false;
  }

  if(Scale && !Scale->isValid()){
      pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","Scale","Not Valid");
      return false;
  }

  return true;
}

/* ----------------------------------------------------------------------- */

/// Constructor.
Inline::Inline(const string& filename, const Vector3& translation, const Vector3& scale) :
    Shape3D(),
    __filename(filename),
    __translation(translation),
    __scale(scale),
    __scene(new Scene(filename, "")){
    if ( __name.empty() ) setDefaultName();
}

Inline::~Inline()
{
}

/// set a default name to \e this
void Inline::setDefaultName()
{
    __name = "INLINE_"+number(getObjectId());
}

const string&
Inline::getFileName() const {
  return __filename;
}

bool Inline::isValid( ) const{
    if(!__scene) return false;
    else return __scene->isValid();
}

SceneObjectPtr Inline::copy(DeepCopier& copier) const
{
  Inline * ptr = new Inline(*this);
  if(ptr->__scene)ptr->__scene = ptr->__scene->deepcopy(copier);
  return SceneObjectPtr(ptr);
}

bool
Inline::applyGeometryFirst( Action& action )
{
  if(__scene) return __scene->applyGeometryFirst(action );
  return false;
}

bool
Inline::applyGeometryOnly( Action& action )
{
  if(__scene) return __scene->applyGeometryOnly(action );
  return false;
}

bool
Inline::applyAppearanceFirst( Action& action )
{
  if(__scene) return __scene->applyAppearanceFirst(action );
  return false;
}

bool
Inline::applyAppearanceOnly( Action& action )
{
  if(__scene) return __scene->applyAppearanceOnly(action );
  return false;
}

  /// Return whether self should be rendered dynamically
bool Inline::hasDynamicRendering() const
{
    if(__scene) return __scene->hasDynamicRendering();
    return false;
}

