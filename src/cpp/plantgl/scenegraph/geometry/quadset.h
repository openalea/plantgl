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


/*! \file geom_quadset.h
    \brief Definition of the geometry class QuadSet.
*/


#ifndef __geom_quadset_h__
#define __geom_quadset_h__

/* ----------------------------------------------------------------------- */

#include "mesh.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class QuadSet
   \brief A quadset
*/

/* ----------------------------------------------------------------------- */

class SG_API QuadSet : public IndexedMesh<Index4Array>
{

public:
  friend class FaceSet;

  typedef IndexedMesh<Index4Array>::Builder<QuadSet> Builder;

  static std::string getClassName() { return "QuadSet"; }

  /// Default Constructor. Build object is invalid.
  QuadSet();

  /** Constructs a QuadSet with the points \e points, the quadrilaterals 
      indices \e indices, the face ordering \e ccw, the solid flag \e solid 
      and the skeleton \e skeleton.
      \pre
      - \e points must contain a minimum of 3 points;
      - \ indices must contain a minimum of 1 index. Each element of an
      index must be unique.
      - \e skeleton must be valid in the case it is non null.
      \post
      - \e self is valid. */
  QuadSet( const Point3ArrayPtr& points,
	   const Index4ArrayPtr& indices,
	   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
	   bool ccw = DEFAULT_CCW,
	   bool solid = DEFAULT_SOLID,
	   const PolylinePtr& skeleton = DEFAULT_SKELETON);

 
  /** Constructs a QuadSet with the points \e points, the quadrilaterals 
      indices \e indices, the face ordering \e ccw, the solid flag \e solid 
      the skeleton \e skeleton and the normals \e normals.
      normals don't have to be normalized.
      \pre
      - \e points must contain a minimum of 3 points;
      - \ indices must contain a minimum of 1 index. Each element of an
      index must be unique.
      - \e skeleton must be valid in the case it is non null.
      - \e normals must be as big as points;
      \post
      - \e self is valid. */
  QuadSet( const Point3ArrayPtr& points,
		   const Index4ArrayPtr& indices,
		   const Point3ArrayPtr& normals,
           const Index4ArrayPtr& nomalIndices = Index4ArrayPtr(),
		   const Color4ArrayPtr& colors  = Color4ArrayPtr(),
           const Index4ArrayPtr& colorIndices = Index4ArrayPtr(),
		   const Point2ArrayPtr& texCoord = Point2ArrayPtr(),
           const Index4ArrayPtr& texCoordIndices = Index4ArrayPtr(),
		   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
		   bool colorPerVertex = DEFAULT_COLORPERVERTEX,
		   bool ccw = DEFAULT_CCW,
		   bool solid = DEFAULT_SOLID,
		   const PolylinePtr& skeleton = DEFAULT_SKELETON);

  /// Destructor
  virtual ~QuadSet( );

  virtual bool apply( Action& action )
  { return action.process(this); }

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  virtual bool isValid( ) const;

  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const;

};

/// QuadSet Pointer
typedef RCPtr<QuadSet> QuadSetPtr;


/* ----------------------------------------------------------------------- */

// __geom_quadset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

