/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr)
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

/*! \file scne_smbtable.h
    \brief Definition of the SymbolTable class SceneObjectSymbolTable,  GeometrySymbolTable and AppearanceSymbolTable.
*/

#ifndef __scne_smbtable_h__
#define __scne_smbtable_h__

/* ----------------------------------------------------------------------- */

#include <tool/rcobject.h>
#include <tool/gsmbtable.h>
#include "../sg_config.h"

#include <scenegraph/geometry/geometry.h>
#include <scenegraph/appearance/appearance.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class SceneObject;
typedef RCPtr<SceneObject> SceneObjectPtr;

/* ----------------------------------------------------------------------- */

/**
   \class SceneObjectSymbolTable
   \brief Class used for parsing a stream containing SceneObject.  
*/

/* ----------------------------------------------------------------------- */
class SG_API SceneObjectSymbolTable : public SymbolTable<SceneObjectPtr> {};

/* ----------------------------------------------------------------------- */

class Geometry;
typedef RCPtr<Geometry> GeometryPtr;

/* ----------------------------------------------------------------------- */

/**
   \class GeometrySymbolTable
   \brief Class used for parsing a stream containing Geometry.
*/

/* ----------------------------------------------------------------------- */

class SG_API GeometrySymbolTable : public SymbolTable<GeometryPtr> {};

/* ----------------------------------------------------------------------- */

class Appearance;
typedef RCPtr<Appearance> AppearancePtr;

/* ----------------------------------------------------------------------- */

/**
   \class AppearanceSymbolTable
   \brief Class used for parsing a stream containing Appearance.
*/

/* ----------------------------------------------------------------------- */

class SG_API AppearanceSymbolTable : public SymbolTable<AppearancePtr> {};

/* ----------------------------------------------------------------------- */

template<class T,class U>
void convertTable(const SymbolTable<U>& intable, SymbolTable<T>& outtable)
{
  T tobject;
  for(SymbolTable<class U>::const_iterator _it = intable.begin();
      _it != intable.end() ; ++_it){
      if(tobject.cast(_it->second)) outtable[_it->first] = tobject;
  }
}

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif
