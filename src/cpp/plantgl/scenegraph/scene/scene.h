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



/*! \file scne_scene.h
    \brief Definition of the scene class Scene.
*/

#ifndef __scne_scene_h__
#define __scne_scene_h__

/* ----------------------------------------------------------------------- */

#include <vector>
#include <plantgl/tool/util_hashmap.h>
#include "plantgl/scenegraph/core/sceneobject.h"
#include "shape.h"

/* ----------------------------------------------------------------------- */

class PglMutex;

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

class SG_API Scene : public RefCountObject
{

public:


  /// An iterator used to iterate through a Scene.
  typedef std::vector<Shape3DPtr>::iterator iterator;

  /// A const iterator used to iterate through a Scene.
  typedef std::vector<Shape3DPtr>::const_iterator const_iterator;

  /// Element types
  typedef Shape3DPtr element_type;

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

  Scene(const Scene::const_iterator begin, const Scene::const_iterator end);

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
  // compatibility with stl
  inline void push_back( const Shape3DPtr& shape ) { add(shape); }

  /** Remove a shape to the \e self
      \pre
      - shape must be non null and valid. */
  void remove( const Shape3DPtr& shape );
  
  // compatibility with stl
  void erase( Scene::iterator it );
  void erase( Scene::iterator itbeg, Scene::iterator itend );

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
  const ShapePtr findShapeId(uint_t id ) const ;

  /// Returns the \e i-th element of \e self.
  const Shape3DPtr findSceneObjectId(uint_t id ) const ;

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

  PglMutex* __mutex;

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
        PglMutex* __mutex;
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


