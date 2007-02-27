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


/*! \file scne_scene.h
    \brief Definition of the scene class Scene.
*/

#ifndef __scne_scene_h__
#define __scne_scene_h__

/* ----------------------------------------------------------------------- */

#include <vector>
#include "scenegraph/core/sceneobject.h"
#include "shape.h"

#ifdef QT_THREAD_SUPPORT
// forward declaration of QMutex
class QMutex;
#else
#ifndef QT_NO_THREAD_SUPPORT
#ifdef _MSC_VER
#pragma message("QT_THREAD_SUPPORT macro not defined. Don't you Forget?")
#else
#warning "QT_THREAD_SUPPORT macro not defined. Don't you Forget?"
#endif
#endif
#endif

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Scene;

typedef RCPtr<Scene> ScenePtr;


/* ----------------------------------------------------------------------- */

/**
   \class Scene
   \brief A 3D Scene represented by a list of objects of type of Shape.
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

  void save( const std::string& fname , const std::string& format = "" );

  void read( const std::string& fname,
			 const std::string& format = "",
			 std::ostream& errlog=std::cerr, 
			 int max_error = -1 );
  
  /** Adds objects describe in table  */
  void convert( const SceneObjectSymbolTable& table );

  /** Adds a shape to the \e self
      \pre
      - shape must be non null and valid. */
  void add( const Shape& shape );
  void add( const ShapePtr& shape );

  /** Adds a shape to the \e self
      \pre
      - shape must be non null and valid. */
  void add( const Shape3DPtr& shape );

  /** Remove a shape to the \e self
      \pre
      - shape must be non null and valid. */
  void remove( const Shape3DPtr& shape );
  void remove( Scene::iterator& it );

  /// Deep copy of \e this.
  virtual ScenePtr copy() const ;

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
  const_iterator getBegin( ) const;

  /// Returns an iterator at the beginning of \e self.
  iterator getBegin( );

  /// Returns a const iterator at the end of \e self.
  const_iterator getEnd( ) const;

  /// Returns an iterator at the end of \e self.
  iterator getEnd( );

  /// Returns the \e i-th element of \e self.
  const Shape3DPtr getAt(uint32_t i ) const ;

  /// Returns the \e i-th element of \e self.
  void setAt(uint32_t i, const Shape3DPtr& );

  /// Returns the \e i-th element of \e self.
  const ShapePtr getShapeId(uint32_t id ) const ;

  /// Returns the size of \e self.
  uint32_t getSize( ) const ;

  /// Resize  \e self.
  void Resize(const uint32_t size );

  /// Returns whether \e self is empty.
  bool isEmpty( ) const ;

  /// Returns whether \e self is valid.
  bool isValid( ) const ;

  /// Returns whether \e self is valid and output error on stream.
  bool isValid( std::ostream& stream ) const ;

  /// Returns whether \e self is valid and output error on stream error, ...
  bool isValid( std::ostream& error,std::ostream& warning, std::ostream& info) const;

  /** Merges the Scene \e subScene to \e self.
      \pre
      - \e subScene must be valid. */
  void merge( const ScenePtr& subScene );

  void lock() const ;
  void unlock() const;

protected:

  /// The list of shapes constituting the subScene.
  std::vector<Shape3DPtr> __shapeList;

#ifdef QT_THREAD_SUPPORT
  QMutex* __mutex;
#endif

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


