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

