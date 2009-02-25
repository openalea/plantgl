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


/*! \file geom_triangleset.h
    \brief Definition of the geometry class TriangleSet.
*/

#ifndef __geom_triangleset_h__
#define __geom_triangleset_h__

/* ----------------------------------------------------------------------- */

#include "mesh.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class TriangleSet
   \brief A triangleset
*/

/* ----------------------------------------------------------------------- */

class SG_API TriangleSet : public IndexedMesh<Index3Array>
{

public:
   friend class FaceSet;

  typedef IndexedMesh<Index3Array>::Builder<TriangleSet> Builder;

  static std::string getClassName() { return "TriangleSet"; }

  /// Default Constructor. Build object is invalid.
  TriangleSet();

  /** Constructs a TriangleSet with the points \e points, the triangles 
      indices \e indices, the face ordering \e ccw, the solid flag \e solid 
      and the skeleton \e skeleton.
      \pre
      - \e points must contain a minimum of 3 points;
      - \ indices must contain a minimum of 1 index. Each element of an
      index must be unique.
      - \e skeleton must be valid in the case it is non null.
      \post
      - \e self is valid. */
  TriangleSet( const Point3ArrayPtr& points,
	           const Index3ArrayPtr& indices,
			   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
			   bool ccw = DEFAULT_CCW,
			   bool solid = DEFAULT_SOLID,
			   const PolylinePtr& skeleton = DEFAULT_SKELETON );


  /** Constructs a TriangleSet with the points \e points, the triangles 
      indices \e indices, the face ordering \e ccw, the solid flag \e solid,
      the skeleton \e skeleton and the normals \e normals.
      normals don't have to be normalized.
      \pre
      - \e points must contain a minimum of 3 points;
      - \e indices must contain a minimum of 1 index. Each element of an
      index must be unique.
      - \e skeleton must be valid in the case it is non null.
      - \e normals must be as big as points;
      - \e pointsColor must ne as big as points
      \post
      - \e self is valid. */
  TriangleSet( const Point3ArrayPtr& points,
	           const Index3ArrayPtr& indices,
			   const Point3ArrayPtr& normals ,
	           const Index3ArrayPtr& nomalIndices = Index3ArrayPtr(),
			   const Color4ArrayPtr& colors  = Color4ArrayPtr(),
	           const Index3ArrayPtr& colorIndices = Index3ArrayPtr(),
			   const Point2ArrayPtr& texCoord = Point2ArrayPtr(),
	           const Index3ArrayPtr& texCoordIndices = Index3ArrayPtr(),
			   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
			   bool colorPerVertex = DEFAULT_COLORPERVERTEX,
			   bool CCW = DEFAULT_CCW,
			   bool solid = DEFAULT_SOLID,
			   const PolylinePtr& skeleton = DEFAULT_SKELETON);

  /// Destructor
  virtual ~TriangleSet( );

  virtual bool apply( Action& action )
  { return action.process(this); }

  virtual bool isValid() const;

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const;

};

/// TriangleSet Pointer
typedef RCPtr<TriangleSet> TriangleSetPtr;


/* ----------------------------------------------------------------------- */

// __geom_triangleset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

