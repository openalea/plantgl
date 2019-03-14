/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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

/*! \file deepcopier.h
    \brief Definition of DeepCopier.
*/

#ifndef __deepcopier_h__
#define __deepcopier_h__

/* ----------------------------------------------------------------------- */

#include "../sg_config.h"
#include <plantgl/tool/util_hashmap.h>
#include "sceneobject.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

// Forward declaration

class SceneObject;
typedef RCPtr<SceneObject> SceneObjectPtr;

/* ----------------------------------------------------------------------- */

/**
    \class DeepCopier
    \brief Base class for recursive deep copy of a scene.
*/

/* ----------------------------------------------------------------------- */

class SG_API DeepCopier 
{

public:
  // typedef const RefCountObject * KeyType;
  typedef size_t KeyType;
  typedef RefCountObjectPtr ValueType;
  typedef pgl_hash_map<KeyType,ValueType> RCObjectMap;

  /** Default constructor.  */
  DeepCopier( )  { }

  /// Destructor
  virtual ~DeepCopier( );

  inline void clear() { __map.clear(); }

  inline KeyType to_key(const RefCountObject * ptr) { return (size_t)ptr; }

  template<class T>
  inline void set(const RefCountObject * source, 
	              const RCPtr<T> target) 
  { __map[to_key(source)] = RefCountObjectPtr(target);  }

  inline void set(const RefCountObject * source,
	              RefCountObject * target) 
  { __map[to_key(source)] = RefCountObjectPtr(target);  }

  inline RefCountObjectPtr get(const RefCountObject * source) 
  { 
	  RCObjectMap::const_iterator it = __map.find(to_key(source));
	  if(it != __map.end()) return it->second;
	  else return RefCountObjectPtr(0);
  }

  inline bool remove(const RefCountObject * source) 
  { 
	  RCObjectMap::iterator it = __map.find(to_key(source));
	  if(it != __map.end()) { __map.erase(it); return true; }
	  else return false;
  }

  template<class T>
  SceneObjectPtr copy(const T * obj){
	  if(obj == NULL)return SceneObjectPtr();
	  RCObjectMap::const_iterator it = __map.find(to_key(obj));
	  if(it != __map.end()) return dynamic_pointer_cast<SceneObject>(it->second);
	  else{
		  SceneObjectPtr newobj = obj->copy(*this);
	      if(!obj->unique())set(obj,newobj);
		  return newobj;
	  }
  }

  template<class T>
  void copy_attribute(RCPtr<T>& att) {
	  att = get_attribute_copy(att);
  }

  template<class T>
  void copy_recursive_attribute(RCPtr<T>& att) {
	  att = get_recursive_copy(att);
  }

  template<class T>
  void copy_object_attribute(RCPtr<T>& att) {
	  if(!is_null_ptr(att)){
		att = dynamic_pointer_cast<T>(att->deepcopy(*this));
	  }
  }

  template<class T>
  void copy_recursive_object_attribute(RCPtr<T>& att) {
	  att = get_recursive_object_copy(att);
  }

  template<class T>
  RCPtr<T> get_attribute_copy(const RCPtr<T>& att) {
	  if(is_null_ptr(att))return att;
	  if(att->unique()) return RCPtr<T>(new T(*att));
	  RCObjectMap::const_iterator it = __map.find(to_key(att.get()));
	  if(it != __map.end()) return dynamic_pointer_cast<T>(it->second);
	  else {
		T * ptr = new T(*att);
		set(att.get(),ptr);
		return RCPtr<T>(ptr);
	  }
  }

  template<class T>
  RCPtr<T> get_recursive_copy(const RCPtr<T>& att) {	  
	  typedef typename T::iterator TIterator;
	  if(is_null_ptr(att))return att;
	  if(!att->unique()) {
		RCObjectMap::const_iterator it = __map.find(to_key(att.get()));
		if(it != __map.end()) return dynamic_pointer_cast<T>(it->second);
	  }
	  T * ptr = new T(*att);
	  for(TIterator itGeom = att->begin();itGeom != att->end(); ++itGeom)
		   { copy_attribute(*itGeom); }
	  if(!att->unique())set(att.get(),ptr);
	  return RCPtr<T>(ptr);
  }

  template<class T>
  RCPtr<T> get_recursive_object_copy(const RCPtr<T>& att) {	  
	  typedef typename T::iterator TIterator;
	  if(is_null_ptr(att))return att;
	  if(!att->unique()){
		RCObjectMap::const_iterator it = __map.find(to_key(att.get()));
		if(it != __map.end()) return dynamic_pointer_cast<T>(it->second);
	  }
	  T * ptr = new T(*att);
	  for(TIterator itGeom = att->begin();itGeom != att->end(); ++itGeom)
		   { copy_object_attribute(*itGeom); }
	  if(!att->unique())set(att.get(),ptr);
	  return RCPtr<T>(ptr);
  }

protected:


  /// Object map
  RCObjectMap __map;

}; // class SceneObject

/* ------------------------------------------------------------------------- */

PGL_END_NAMESPACE;

/* ------------------------------------------------------------------------- */

// __scne_sceneobject_h__
#endif

