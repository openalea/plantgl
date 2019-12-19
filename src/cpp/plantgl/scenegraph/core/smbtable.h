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


/*! \file scne_smbtable.h
    \brief Definition of the SymbolTable class SceneObjectSymbolTable,  GeometrySymbolTable and AppearanceSymbolTable.
*/

#ifndef __scne_smbtable_h__
#define __scne_smbtable_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/gsmbtable.h>
#include "../sg_config.h"

#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/appearance/appearance.h>

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
  RCPtr<T> tobject;
  typedef typename SymbolTable< U >::const_iterator const_iterator;
  for( const_iterator _it = intable.begin();
      _it != intable.end() ; ++_it){
      if((tobject = dynamic_pointer_cast<T>(_it->second))) outtable[_it->first] = tobject;
  }
}

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif
