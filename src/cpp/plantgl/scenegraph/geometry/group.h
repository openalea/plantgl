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



/*! \file geom_group.h
    \brief Definition of the geometry class Group.
*/

#ifndef __geom_group_h__
#define __geom_group_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/container/geometryarray2.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


/* ----------------------------------------------------------------------- */

/**
    \class Group
    \brief A object group
*/

class SG_API Group : public Geometry
{

public:

  /// The \b Skeleton field default value.
  static const PolylinePtr DEFAULT_SKELETON;


  /// A structure which helps to build a Group when parsing.
  struct SG_API Builder : public Geometry::Builder {

    /// A pointer to the \b GeometryList field.
    GeometryArrayPtr * GeometryList;

    /// A pointer to the \b Skeleton field.
    PolylinePtr * Skeleton;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// An iterator used to iterate through a Scene.
  typedef GeometryArray::iterator iterator;

  /// A const iterator used to iterate through a Scene.
  typedef GeometryArray::const_iterator const_iterator;

  /// Element types
  typedef GeometryArray::element_type element_type;


  /// Default Constructor. Build object is invalid.
  Group();

  /** Constructs a Group with the Geometry objects \e geometries.
      \warning
      - \e geometries must contain at leat one non null and valid element;
      - \e skeleton must be valid if non null. */
  Group( const GeometryArrayPtr& geometries,
         const PolylinePtr& skeleton = DEFAULT_SKELETON );

  Group( const iterator begin, const iterator end,
         const PolylinePtr& skeleton = DEFAULT_SKELETON );

  /// Destructor
  virtual ~Group( ) ;

  PGL_OBJECT(Group)

  /** Returns the value of the \e i-th geom of  \b GeometryList.
      \e pre
      - \e i must belong to the range [0,size of \b GeometryList[. */
  const GeometryPtr& getGeometryListAt( uint_t i ) const;

  /** Returns the \e i-th geom of  \b GeometryList.
      \e pre
      - \e i must belong to the range [0,size of \b GeometryList[. */
  GeometryPtr& getGeometryListAt( uint_t i );

  /// Returns \b GeometryList values.
  const GeometryArrayPtr& getGeometryList( ) const;

  /// Returns \b GeometryList field.
  GeometryArrayPtr& getGeometryList( );

  /// Returns \b GeometryList size.
  uint_t getGeometryListSize( ) const;

  /// Returns \b Skeleton value.
  virtual const PolylinePtr& getSkeleton( ) const ;

  /// Returns \b Skeleton field.
  PolylinePtr& getSkeleton( );

  /// Returns whether \b Skeleton is set to its default value.
  bool isSkeletonToDefault( ) const ;

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  virtual bool isExplicit( ) const;

  virtual bool isValid( ) const;

  virtual bool hasDynamicRendering() const;

    /// Returns a const iterator at the beginning of \e self.
  inline const_iterator begin( ) const { return __geometryList->begin(); }

  /// Returns an iterator at the beginning of \e self.
  inline iterator begin( ) { return __geometryList->begin(); }

  /// Returns a const iterator at the end of \e self.
  inline const_iterator end( ) const { return __geometryList->end(); }

  /// Returns an iterator at the end of \e self.
  inline iterator end( ) { return __geometryList->end(); }

  inline GeometryPtr getAt(size_t pos) { return __geometryList->getAt(pos); }

  inline size_t size() const { return __geometryList->size(); }

  inline void setAt(size_t pos, GeometryPtr value) { __geometryList->setAt(pos, value); }

  inline void erase(iterator it) { __geometryList->erase(it); }

  inline void erase(iterator itbeg, iterator itend) { __geometryList->erase(itbeg, itend); }

  inline void push_back(GeometryPtr value) { __geometryList->push_back(value); }

protected:

  /// The \b GeometryList field.
  GeometryArrayPtr __geometryList;

  /// The \b Skeleton field.
  PolylinePtr __skeleton;

};

/// Group Pointer
typedef RCPtr<Group> GroupPtr;


/* ----------------------------------------------------------------------- */

// __geom_group_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

