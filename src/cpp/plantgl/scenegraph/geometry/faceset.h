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



/*! \file geom_faceset.h
    \brief Definition of the geometry class FaceSet.
*/

#ifndef __geom_faceset_h__
#define __geom_faceset_h__

/* ----------------------------------------------------------------------- */

#include "mesh.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

class TriangleSet;
class QuadSet;

/* ----------------------------------------------------------------------- */

/**
   \class FaceSet
   \brief A faceset
*/

class SG_API FaceSet : public IndexedMesh<IndexArray>
{

public:

  typedef IndexedMesh<IndexArray>::Builder<FaceSet> Builder;

  static std::string getClassName() { return "FaceSet"; }

  /// Default Constructor. Build object is invalid.
  FaceSet();


  /** Constructs a FaceSet with the points \e points, the faces indices 
      \e indices, the face ordering \e ccw, the solid flag \e solid and the 
      skeleton \e skeleton.
      \pre
      - \e points must contain a minimum of 3 points;
      - \e indices must contain a minimum of 1 index. Each index within \e 
      indices 
      must contain a minimum of 3 elements and each element must be unique.
      - \e skeleton must be valid in the case it is non null.
      \post
      - \e self is valid. */
  FaceSet( const Point3ArrayPtr& points,
	       const IndexArrayPtr& indices,
		   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
		   bool ccw = DEFAULT_CCW,
		   bool solid = DEFAULT_SOLID,
		   const PolylinePtr& skeleton = DEFAULT_SKELETON);
  
  
  /** Constructs a FaceSet with the points \e points, the faces indices 
      \e indices, the face ordering \e ccw, the solid flag \e solid and the 
      the skeleton \e skeleton and the normals \e normals.
      normals don't have to be normalized.
      \pre
      - \e points must contain a minimum of 3 points;
      - \e indices must contain a minimum of 1 index. Each index within \e 
      indices 
      must contain a minimum of 3 elements and each element must be unique.
      - \e skeleton must be valid in the case it is non null.
      - \e normals must be as big as points;
      \post
      - \e self is valid. */
  FaceSet( const Point3ArrayPtr& points,
	       const IndexArrayPtr& indices,
		   const Point3ArrayPtr& normals ,
		   const IndexArrayPtr& nomalIndices = IndexArrayPtr(),
		   const Color4ArrayPtr& colors  = Color4ArrayPtr(),
		   const IndexArrayPtr& colorIndices = IndexArrayPtr(),
		   const Point2ArrayPtr& texCoord = Point2ArrayPtr(),
		   const IndexArrayPtr& texCoordIndices = IndexArrayPtr(),
		   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
		   bool colorPerVertex = DEFAULT_COLORPERVERTEX,
		   bool CCW = DEFAULT_CCW,
		   bool solid = DEFAULT_SOLID,
		   const PolylinePtr& skeleton = DEFAULT_SKELETON);

  /** Constructs a FaceSet from a TriangleSet.  */
  FaceSet(const TriangleSet&);

  /** Constructs a FaceSet from a TriangleSet.  */
  FaceSet(const QuadSet&);

  /// Destructor
  virtual ~FaceSet( );

  PGL_OBJECT(FaceSet)

  virtual bool isValid( ) const;

  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const;

};

/// FaceSet Pointer
typedef RCPtr<FaceSet> FaceSetPtr;


/* ----------------------------------------------------------------------- */

// __geom_faceset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

