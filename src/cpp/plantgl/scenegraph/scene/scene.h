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


/*! \file scne_scene.h
    \brief Definition of the scene class Scene.
*/

#ifndef __scne_scene_h__
#define __scne_scene_h__

/* ----------------------------------------------------------------------- */

#include <vector>
#include <plantgl/tool/util_hashmap.h>
#include <plantgl/tool/util_mutex.h>
#include "plantgl/scenegraph/core/sceneobject.h"
#include "shape.h"


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Scene;

typedef RCPtr<Scene> ScenePtr;


/* ----------------------------------------------------------------------- */

/**
   \class Scene
   \brief A 3D Scene defined as a list of objects of type of Shape.
*/

/* ----------------------------------------------------------------------- */

class SG_API Scene : public TOOLS(RefCountObject)
{

public:


  /// An iterator used to iterate through a Scene.
  typedef std::vector<Shape3DPtr>::iterator iterator;

  /// A const iterator used to iterate through a Scene.
  typedef std::vector<Shape3DPtr>::const_iterator const_iterator;

  /// Constructs an empty Scene.
  Scene(unsigned int size=0);

  Scene(const Scene&);

  /// Constructs a Scene with object describe in \e filename. Write error on \e _errlog.
  Scene(const std::string& filename,
	    const std::string& format = "",
	    std::ostream& errlog=std::cerr, 
		int max_error = -1);

  /// Constructs a Scene with objects describe in \e table.
  Scene(const SceneObjectSymbolTable& table);

  /// Destructs a Scene.
  virtual ~Scene( );

  Scene& operator=( const Scene& );

  bool save( const std::string& fname , const std::string& format = "" );

  bool read( const std::string& fname,
			 const std::string& format = "",
			 std::ostream& errlog=std::cerr, 
			 int max_error = -1 );
  
  /** Adds objects describe in table  */
  void convert( const SceneObjectSymbolTable& table );

  /** Adds a shape to the \e self
      \pre
      - shape must be non null and valid. */
  // void add( const Shape& shape );
  // void add( const ShapePtr& shape );

  /** Adds a shape to the \e self
      \pre
      - shape must be non null and valid. */
  void add( const Shape3DPtr& shape );
  void add( const ShapePtr& shape );

  /** Remove a shape to the \e self
      \pre
      - shape must be non null and valid. */
  void remove( const Shape3DPtr& shape );
  void remove( Scene::iterator& it );

  /// Deep copy of \e this.
  inline ScenePtr deepcopy() const { DeepCopier copier; return deepcopy(copier); }
  virtual ScenePtr deepcopy(DeepCopier& copier) const ;

  /** Applies the action \e action to each shape of \e self.
    */
  bool apply( Action& action );

  /** Applies the action \e action to \e self.
      For each shape within \e self, it applies the action first to the
      Geometry part and second to the Appearance part. */
  bool applyGeometryFirst( Action& action );

  /** Applies the action \e action to \e self.
      It applies the action to the Geometry part only. The Appearance part
      is skipped. */
  bool applyGeometryOnly( Action& action );

  /** Applies the action \e action to \e self.
      It applies the action first to the Appearance part and second to the
      Geometry part. */
  bool applyAppearanceFirst( Action& action );

  /** Applies the action \e action to \e self.
      It applies the action to the Appearance part only. The Geometry
      part is skipped. */
  bool applyAppearanceOnly( Action& action );

  /// Clears \e self.
  void clear( );

  /// Returns a const iterator at the beginning of \e self.
  inline const_iterator begin( ) const { return __shapeList.begin(); }

  /// Returns an iterator at the beginning of \e self.
  inline iterator begin( ) { return __shapeList.begin(); }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator end( ) const { return __shapeList.end(); }

  /// Returns an iterator at the end of \e self.
  inline iterator end( ) { return __shapeList.end(); }

  /// Returns the \e i-th element of \e self.
  const Shape3DPtr getAt(uint_t i ) const ;

  /// Returns the \e i-th element of \e self.
  void setAt(uint_t i, const Shape3DPtr& );

  /// Returns the \e i-th element of \e self.
  const ShapePtr getShapeId(uint_t id ) const ;

  /// Returns the \e i-th element of \e self.
  const Shape3DPtr getSceneObjectId(uint_t id ) const ;

  /// Returns the size of \e self.
  uint_t size( ) const ;

  /// Resize  \e self.
  void resize(const uint_t size );

  /// Returns whether \e self is empty.
  bool empty( ) const ;

  /// Returns whether \e self is valid.
  bool isValid( ) const ;

  /// Returns whether \e self is valid and output error on stream.
  bool isValid( std::ostream& stream ) const ;

  /// Returns whether \e self is valid and output error on stream error, ...
  bool isValid( std::ostream& error,std::ostream& warning, std::ostream& info) const;

  bool hasDynamicRendering() const;
  
  /** Merges the Scene \e subScene to \e self.
      \pre
      - \e subScene must be valid. */
  void merge( const ScenePtr& subScene );

  void lock() const ;
  void unlock() const;

  void sort();

#ifndef PGL_NO_DEPRECATED
  /// Returns a const iterator at the beginning of \e self.
  inline attribute_deprecated const_iterator getBegin( ) const { return begin(); }

  /// Returns an iterator at the beginning of \e self.
  inline attribute_deprecated iterator getBegin( ) { return begin(); }

  /// Returns a const iterator at the end of \e self.
  inline attribute_deprecated const_iterator getEnd( ) const { return end(); }

  /// Returns an iterator at the end of \e self.
  inline attribute_deprecated iterator getEnd( ) { return end(); }

  /// Return size of this
  inline attribute_deprecated uint_t getSize() const { return size(); }

  inline attribute_deprecated bool isEmpty() const { return empty(); }

  inline attribute_deprecated void Resize(const uint_t size ) { return resize(size); }
#endif
protected:

  /// The list of shapes constituting the subScene.
  std::vector<Shape3DPtr> __shapeList;

#ifdef PGL_THREAD_SUPPORT
  PglMutex* __mutex;
#endif
public:

    /// A Scene Pool class
    class SG_API Pool {
    public:
        typedef pgl_hash_map<size_t,Scene *> PoolList;
        friend class Scene;
        ~Pool();
 
        // get scene id
        ScenePtr get(size_t id) const;
        // get all scene
        std::vector<ScenePtr> getScenes() const;

    protected:
        void registerScene(Scene *);
        void unregisterScene(const Scene *);
        void lock() const ;
        void unlock() const;

        Pool();

        PoolList __pool;
#ifdef PGL_THREAD_SUPPORT
        PglMutex* __mutex;
#endif
    };

    // Singleton access
    static Pool& pool();

private:
    static Pool POOL;

};

/// Scene Pointer
typedef RCPtr<Scene> ScenePtr;


/* ----------------------------------------------------------------------- */

// __scne_scene_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/// Write Scene \b s in \b stream.
// SG_API std::ostream& operator<<( std::ostream& stream, PGL(Scene&) s );

/* ----------------------------------------------------------------------- */
#endif


