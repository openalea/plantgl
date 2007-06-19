/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

#include "inline.h"
#include "scene.h"
#include <plantgl/scenegraph/transformation/translated.h>

#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/dirnames.h>

#include <fstream>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
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
    genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Inline","FileName");
    return false;
  };
  if(FileName){
      ifstream _file(FileName->c_str());
      if (!_file){
          string _mess = "Cannot open " + *FileName;
          genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","FileName",_mess.c_str());
          return false;
      }
  }

  if(Scene && !(*Scene)->isValid()){
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","Scene","Not Valid");
      return false;      
  }

  if(Translation && !Translation->isValid()){
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","Translation","Not Valid");
      return false;      
  }

  if(Scale && !Scale->isValid()){
      genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Inline","Scale","Not Valid");
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
    __scene(new Scene(filename, "", *errorStream)){
    if ( __name.empty() ) setDefaultName();
}

Inline::~Inline()
{
}

/// set a default name to \e this
void Inline::setDefaultName()
{
	__name = "INLINE_"+number(getId());
}

bool Inline::apply( Action& action ){
  return action.process(this);
}

const string&
Inline::getFileName() const {
  return __filename;
}

bool Inline::isValid( ) const{
    if(!__scene) return false;
    else return __scene->isValid();
}

SceneObjectPtr Inline::copy() const
{
  Inline * ptr = new Inline(*this);
  if(__scene)ptr->__scene = __scene->copy();
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
