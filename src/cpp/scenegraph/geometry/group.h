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



/*! \file geom_group.h
    \brief Definition of the geometry class Group.
*/

#ifndef __geom_group_h__
#define __geom_group_h__

/* ----------------------------------------------------------------------- */

#include <scenegraph/geometry/geometry.h>
#ifndef GEOM_FWDEF
#include <scenegraph/geometry/polyline.h>
#include <scenegraph/container/geometryarray2.h>
#endif

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

#ifdef GEOM_FWDEF
class Polyline;
typedef RCPtr<Polyline> PolylinePtr;
class GeometryArray;
typedef RCPtr<GeometryArray> GeometryArrayPtr;
#endif

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

  /// Default Constructor. Build object is invalid.
  Group();

  /** Constructs a Group with the Geometry objects \e geometries.
      \warning
      - \e geometries must contain at leat one non null and valid element;
      - \e skeleton must be valid if non null. */
  Group( const GeometryArrayPtr& geometries,
         const PolylinePtr& skeleton = DEFAULT_SKELETON );

  /// Destructor
  virtual ~Group( ) ;

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /** Returns the value of the \e i-th geom of  \b GeometryList.
      \e pre
      - \e i must belong to the range [0,size of \b GeometryList[. */
  const GeometryPtr& getGeometryListAt( uint32_t i ) const;

  /** Returns the \e i-th geom of  \b GeometryList.
      \e pre
      - \e i must belong to the range [0,size of \b GeometryList[. */
  GeometryPtr& getGeometryListAt( uint32_t i );

  /// Returns \b GeometryList values.
  const GeometryArrayPtr& getGeometryList( ) const;

  /// Returns \b GeometryList field.
  GeometryArrayPtr& getGeometryList( );

  /// Returns \b GeometryList size.
  uint32_t getGeometryListSize( ) const;

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

